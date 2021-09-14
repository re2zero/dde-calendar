/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     hejinghai <hejinghai@uniontech.com>
*
* Maintainer: hejinghai <hejinghai@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "test_calendarmainwindow.h"
#include "../third-party_stub/stub.h"
#include "cdynamicicon.h"
#include "configsettings.h"
#include <QDebug>

void stub_setDate() {}

void stub_setIcon() {}

QVariant stub_value()
{
    return QVariant{};
}

test_calendarmainwindow::test_calendarmainwindow()
{
//    Stub stub;
//    stub.set(ADDR(CDynamicIcon, setDate), stub_setDate);
//    stub.set(ADDR(CDynamicIcon, setIcon), stub_setIcon);
//    //CConfigSettings
//    stub.set(ADDR(CConfigSettings, value), stub_value);
//    mainWindow = new Calendarmainwindow();
}

test_calendarmainwindow::~test_calendarmainwindow()
{
//    delete mainWindow;
//    mainWindow = nullptr;
}

//void Calendarmainwindow::onViewShortcut()
TEST_F(test_calendarmainwindow, onViewShortcut)
{
//    mainWindow->onViewShortcut();
//    assert(1 == 1);
}
