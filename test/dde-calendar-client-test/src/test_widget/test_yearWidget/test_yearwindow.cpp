/*
* Copyright (C) 2019 ~ 2021 Uniontech Software Technology Co.,Ltd.
*
* Author:     zhengxiaokang <zhengxiaokang@uniontech.com>
*
* Maintainer: zhengxiaokang <zhengxiaokang@uniontech.com>
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
#include "test_yearwindow.h"

test_yearwindow::test_yearwindow()
{
    mYearWindow = new CYearWindow();
    dateaManger = new CalendarDateDataManager();
}

test_yearwindow::~test_yearwindow()
{
    delete mYearWindow;
    mYearWindow = nullptr;
    delete dateaManger;
    dateaManger = nullptr;
}

//void CYearWindow::setTheMe(int type)
TEST_F(test_yearwindow, setTheMe)
{
    int type = 1;
    mYearWindow->setTheMe(type);
    type = 2;
    mYearWindow->setTheMe(type);
}

//void CYearWindow::setSearchWFlag(bool flag)
//TEST_F(test_yearwindow, setSearchWFlag)
//{
//    bool searchFlag = false;
//    mYearWindow->setSearchWFlag(searchFlag);
//}

////void CYearWindow::updateShowDate(const bool isUpdateBar)
//TEST_F(test_yearwindow, updateShowDate)
//{
//    bool updateFlag = true;
//    cYearWindow->updateShowDate(updateFlag);
//}

////void CYearWindow::updateShowSchedule()
//TEST_F(test_yearwindow, updateShowSchedule)
//{
//    cYearWindow->updateShowSchedule();
//}

////void CYearWindow::updateShowLunar()
//TEST_F(test_yearwindow, updateShowLunar)
//{
//    cYearWindow->updateShowLunar();
//}

////void CYearWindow::updateSearchScheduleInfo()
//TEST_F(test_yearwindow, updateSearchScheduleInfo)
//{
//    cYearWindow->updateSearchScheduleInfo();
//}

////void CYearWindow::slotSetScheduleHide()
//TEST_F(test_yearwindow, slotSetScheduleHide)
//{
//    cYearWindow->slotSetScheduleHide();
//}

////void CYearWindow::slotprev()
//TEST_F(test_yearwindow, slotprev)
//{
//    mYearWindow->slotprev();
//}

////void CYearWindow::slotnext()
//TEST_F(test_yearwindow, slotnext)
//{
//    FAIL();
//    mYearWindow->slotnext();
//}

////void CYearWindow::slottoday()
//TEST_F(test_yearwindow, slottoday)
//{
//    cYearWindow->slottoday();
//}

////void CYearWindow::switchYear(const int offsetYear)
//TEST_F(test_yearwindow, switchYear)
//{
//    dateaManger->setSelectDate(QDate::currentDate(), false);
//    cYearWindow->switchYear(1);
//    cYearWindow->switchYear(-1);
//}

////void CYearWindow::setLunarShow()
//TEST_F(test_yearwindow, setLunarShow)
//{
//    cYearWindow->setLunarShow();
//}

////void CYearWindow::setYearData()
//TEST_F(test_yearwindow, setYearData)
//{
//    dateaManger->setSelectDate(QDate::currentDate(), false);
//    dateaManger->setCurrentDateTime(QDateTime::currentDateTime());
//    cYearWindow->setYearData();

//    dateaManger->setCurrentDateTime(QDateTime::currentDateTime().addDays(1));
//    cYearWindow->setYearData();
//}

////void CYearWindow::slotMousePress(const QDate &selectDate, const int pressType)
////TEST_F(test_yearwindow, slotMousePress)
////{
////cYearWindow->slotMousePress(QDate::currentDate(), 0);
////}
