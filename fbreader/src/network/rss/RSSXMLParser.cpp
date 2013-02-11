#include "RSSXMLParser.h"

#include <iostream> //udmv

static const std::string TAG_RSS = "rss";
static const std::string TAG_CHANNEL = "channel";
static const std::string TAG_ITEM = "item";
static const std::string TAG_TITLE = "title";
static const std::string TAG_LINK = "link";
static const std::string TAG_GUID = "guid";

RSSXMLParser::RSSXMLParser(shared_ptr<RSSChannelReader> channelReader) : myState(START), myChannelReader(channelReader) {
}

void RSSXMLParser::startElementHandler(const char *tag, const char **attributes){
    std::map<std::string,std::string> attributeMap = getAttributesMap(attributes);
    //std::cout << "[RSSXMLParser] start TAG = " << tag << std::endl;
    switch (myState) {
        case START:
            if (testTag(ZLXMLNamespace::Atom, TAG_RSS, tag)) {
                std::cout << "[RSSXMLParser] >> start TAG = " << tag << std::endl;
                myState = RSS;
            }
            break;
        case RSS:
            if (testTag(ZLXMLNamespace::Atom, TAG_CHANNEL, tag)) {
                std::cout << "[RSSXMLParser] >> start TAG = " << tag << std::endl;
                myState = CHANNEL;
            }
            break;
        case CHANNEL:
            if (testTag(ZLXMLNamespace::Atom, TAG_TITLE, tag)) {
                std::cout << "[RSSXMLParser] >> start TAG = " << tag << std::endl;
                myState = C_TITLE;
            }
            if (testTag(ZLXMLNamespace::Atom, TAG_LINK, tag)) {
                std::cout << "[RSSXMLParser] >> start TAG = " << tag << std::endl;
                myState = C_LINK;
            }
            if (testTag(ZLXMLNamespace::Atom, TAG_ITEM, tag)) {
                std::cout << "[RSSXMLParser] >> start TAG = " << tag << std::endl;
                myRSSItem = new RSSItem();
                myState = ITEM;
            }
            break;
        case ITEM:
            if (testTag(ZLXMLNamespace::Atom, TAG_TITLE, tag)) {
                std::cout << "[RSSXMLParser] >> start TAG = " << tag << std::endl;
                myState = TITLE;
            }
            if (testTag(ZLXMLNamespace::Atom, TAG_LINK, tag)) {
                std::cout << "[RSSXMLParser] >> start TAG = " << tag << std::endl;
                myState = LINK;
            }
            if (testTag(ZLXMLNamespace::Atom, TAG_GUID, tag)) {
                std::cout << "[RSSXMLParser] >> start TAG = " << tag << std::endl;
                myState = GUID;
            }
    }
}

void RSSXMLParser::endElementHandler(const char *tag){
    ZLUnicodeUtil::utf8Trim(myBuffer);
    //std::cout << "[RSSXMLParser] end TAG = " << tag << std::endl;
    switch (myState) {
        case START:
            std::cout << "[RSSXMLParser] << end START TAG = " << tag << std::endl;
            break;
        case RSS:
            if (testTag(ZLXMLNamespace::Atom, TAG_RSS, tag)) {
                std::cout << "[RSSXMLParser] << end TAG = " << tag << std::endl;
                myState = START;
            }
            break;
        case CHANNEL:
            if (testTag(ZLXMLNamespace::Atom, TAG_CHANNEL, tag)) {
                std::cout << "[RSSXMLParser] << end TAG = " << tag << std::endl;
                myState = RSS;
            }
            break;
        case C_TITLE:
            if (testTag(ZLXMLNamespace::Atom, TAG_TITLE, tag)) {
                std::cout << "[RSSXMLParser] << end TAG = " << tag << ", " << myBuffer << std::endl;
                myState = CHANNEL;
            }
            break;
        case C_LINK:
            if (testTag(ZLXMLNamespace::Atom, TAG_LINK, tag)) {
                std::cout << "[RSSXMLParser] << end TAG = " << tag << ", " << myBuffer << std::endl;
                myState = CHANNEL;
            }
            break;
        case ITEM:
            if (testTag(ZLXMLNamespace::Atom, TAG_ITEM, tag)) {
                std::cout << "[RSSXMLParser] << end TAG = " << tag << std::endl;
                myChannelReader->processChannelItem(myRSSItem);
                myRSSItem.reset();
                myState = CHANNEL;
            }
        case TITLE:
            if (testTag(ZLXMLNamespace::Atom, TAG_TITLE, tag)) {
                myRSSItem->setTitle(myBuffer);
                std::cout << "[RSSXMLParser] << end TAG = " << tag << ", " << myBuffer << std::endl;
                myState = ITEM;
            }
            break;
        case GUID:
            if (testTag(ZLXMLNamespace::Atom, TAG_GUID, tag)) {
                myId = new ATOMId(myBuffer);
                myRSSItem->setId(myId);
                myUpdated = new ATOMUpdated();
                myRSSItem->setUpdated(myUpdated);
                std::cout << "[RSSXMLParser] << end ! TAG = " << tag << ", " << myBuffer << std::endl;
                myState = ITEM;
            }
            break;
        case LINK:
            if (testTag(ZLXMLNamespace::Atom, TAG_LINK, tag)) {
                std::cout << "[RSSXMLParser] << end TAG = " << tag << ", " << myBuffer << std::endl;
                myLink = new ATOMLink(myBuffer);
                myRSSItem->links().push_back(myLink);
                myLink.reset();
                myState = ITEM;
            }
            break;
    }
    myBuffer.clear();
}

void RSSXMLParser::characterDataHandler(const char *text, std::size_t len){
    myBuffer.append(text, len);
}

bool RSSXMLParser::processNamespaces() const{
    return true;
}
