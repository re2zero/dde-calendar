/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     chenhaifeng  <chenhaifeng@uniontech.com>
*
* Maintainer: chenhaifeng  <chenhaifeng@uniontech.com>
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
#include "testmainwindowgui.h"
#include "configsettings.h"
#include "view/monthgraphiview.h"
#include "yearWidget/yearwindow.h"
#include "monthWidget/monthwindow.h"

testMainWindowGUI::testMainWindowGUI()
{
    CConfigSettings::init();
}

testMainWindowGUI::~testMainWindowGUI()
{
}

TEST_F(testMainWindowGUI, yearGUITest)
{
    Calendarmainwindow mainWindow(0);

    CYearWindow *myYearWindow = mainWindow.findChild<CYearWindow *>("yearwindow");
    if (myYearWindow != nullptr) {
        DIconButton *m_prevButton = myYearWindow->findChild<DIconButton *>("PrevButton");
        DIconButton *m_nextButton = myYearWindow->findChild<DIconButton *>("NextButton");
        LabelWidget *m_today = myYearWindow->findChild<LabelWidget *>("yearToDay");

        if (m_prevButton != nullptr) {
            QTest::mouseClick(m_prevButton, Qt::LeftButton);
            QTest::mouseClick(m_prevButton, Qt::LeftButton);
            QTest::mouseClick(m_prevButton, Qt::LeftButton);
        }

        if (m_today != nullptr) {
            QTest::mouseClick(m_today, Qt::LeftButton);
        }

        if (m_nextButton != nullptr) {
            QTest::mouseClick(m_nextButton, Qt::LeftButton);
            QTest::mouseClick(m_nextButton, Qt::LeftButton);
            QTest::mouseClick(m_nextButton, Qt::LeftButton);
        }
    }
}

TEST_F(testMainWindowGUI, monthGUITest)
{
    Calendarmainwindow mainWindow(1);
    CMonthWindow *mymonthWindow = mainWindow.findChild<CMonthWindow *>("monthWindow");
    if (mymonthWindow != nullptr) {
    }
}
TEST_F(testMainWindowGUI, weekGUIKeyTest)
{
    Calendarmainwindow mainWindow(2);
}

TEST_F(testMainWindowGUI, dayGUIKeyTest)
{
    Calendarmainwindow mainWindow(3);
}
