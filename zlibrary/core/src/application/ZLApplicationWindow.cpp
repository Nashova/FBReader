/*
 * Copyright (C) 2004-2012 Geometer Plus <contact@geometerplus.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include "ZLApplicationWindow.h"

#include "../view/ZLViewWidget.h"

ZLApplicationWindow *ZLApplicationWindow::ourInstance = 0;

ZLApplicationWindow &ZLApplicationWindow::Instance() {
	return *ourInstance;
}

ZLApplicationWindow::ZLApplicationWindow(ZLApplication *application) : myApplication(application) {
	ourInstance = this;
	myApplication->myWindow = this;
}

void ZLApplicationWindow::init() {
	myApplication->myViewWidget = createViewWidget();

	initToolbar();
	initMenu();
}

void ZLApplicationWindow::initToolbar() {
	const ZLToolbar::ItemVector &toolbarItems = myApplication->toolbar().items();
	for (ZLToolbar::ItemVector::const_iterator it = toolbarItems.begin(); it != toolbarItems.end(); ++it) {
		addToolbarItem(*it);
	}
}

void ZLApplicationWindow::refreshToolbar() {
	const ZLToolbar::ItemVector &items = application().toolbar().items();
	bool canAddSeparator = false;
	ZLToolbar::ItemPtr lastSeparator = 0;
	for (ZLToolbar::ItemVector::const_iterator it = items.begin(); it != items.end(); ++it) {
		switch ((*it)->type()) {
			case ZLToolbar::Item::PLAIN_BUTTON:
			case ZLToolbar::Item::MENU_BUTTON:
				{
					ZLToolbar::ActionItem &button = (ZLToolbar::ActionItem&)**it;
					const std::string &id = button.actionId();

					const bool visible = application().isActionVisible(id);
					const bool enabled = application().isActionEnabled(id);

					if (visible) {
						if (!lastSeparator.isNull()) {
							setToolbarItemState(lastSeparator, true, true);
							lastSeparator = 0;
						}
						canAddSeparator = true;
					}
					setToolbarItemState(*it, visible, enabled);
				}
				break;
			case ZLToolbar::Item::SEPARATOR:
				if (canAddSeparator) {
					lastSeparator = *it;
					canAddSeparator = false;
				} else {
					setToolbarItemState(*it, false, true);
				}
				break;
		}
	}
	if (!lastSeparator.isNull()) {
		setToolbarItemState(lastSeparator, false, true);
	}
}

void ZLApplicationWindow::onButtonPress(const ZLToolbar::AbstractButtonItem &button) {
	application().doAction(button.actionId());
}

void ZLApplicationWindow::setVisualParameter(const std::string &id, const std::string &value) {
	std::map<std::string,shared_ptr<VisualParameter> >::iterator it = myParameterMap.find(id);
	if (it != myParameterMap.end()) {
		it->second->setValue(value);
	}
}

void ZLApplicationWindow::setParameterValueList(const std::string &id, const std::vector<std::string> &values) {
	std::map<std::string,shared_ptr<VisualParameter> >::iterator it = myParameterMap.find(id);
	if (it != myParameterMap.end()) {
		it->second->setValueList(values);
	}
}

const std::string &ZLApplicationWindow::visualParameter(const std::string &id) {
	std::map<std::string,shared_ptr<VisualParameter> >::const_iterator it = myParameterMap.find(id);
	if (it != myParameterMap.end()) {
		return it->second->value();
	}
	static const std::string EMPTY;
	return EMPTY;
}

void ZLApplicationWindow::addVisualParameter(const std::string &id, shared_ptr<VisualParameter> parameter) {
	if (!parameter.isNull()) {
		myParameterMap[id] = parameter;
	}
}

ZLApplicationWindow::VisualParameter::~VisualParameter() {
}

const std::string &ZLApplicationWindow::VisualParameter::value() const {
	myValue = internalValue();
	return myValue;
}

void ZLApplicationWindow::VisualParameter::setValue(const std::string &value) {
	if (value != myValue) {
		myValue = value;
		internalSetValue(value);
	}
}

void ZLApplicationWindow::VisualParameter::restoreOldValue() {
	internalSetValue(myValue);
}
