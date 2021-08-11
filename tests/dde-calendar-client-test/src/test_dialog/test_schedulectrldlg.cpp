/*
   * Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
   *
   * Author:     chenhaifeng <chenhaifeng@uniontech.com>
   *
   * Maintainer: chenhaifeng <chenhaifeng@uniontech.com>
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
#include "test_schedulectrldlg.h"

#include <QEvent>
#include <QApplication>

test_schedulectrldlg::test_schedulectrldlg()
{
    mScheduleCtrlDlg = new CScheduleCtrlDlg();
}

test_schedulectrldlg::~test_schedulectrldlg()
{
    delete mScheduleCtrlDlg;
    mScheduleCtrlDlg = nullptr;
}

//void CScheduleCtrlDlg::setTheMe(const int type)
TEST_F(test_schedulectrldlg, setTheMe)
{
    mScheduleCtrlDlg->setTheMe(1);
    mScheduleCtrlDlg->setTheMe(2);
}

//void CScheduleCtrlDlg::buttonJudge(int id)
TEST_F(test_schedulectrldlg, buttonJudge)
{
    mScheduleCtrlDlg->buttonJudge(1);
}

//QAbstractButton *CScheduleCtrlDlg::addPushButton(QString btName, bool type)
TEST_F(test_schedulectrldlg, addPushButton)
{
    mScheduleCtrlDlg->addPushButton("schedule", true);
    mScheduleCtrlDlg->addPushButton("schedule", false);
}

//QAbstractButton *CScheduleCtrlDlg::addsuggestButton(QString btName, bool type)
TEST_F(test_schedulectrldlg, addsuggestButton)
{
    mScheduleCtrlDlg->addsuggestButton("schedule", true);
    mScheduleCtrlDlg->addsuggestButton("schedule", false);
}

//QAbstractButton *CScheduleCtrlDlg::addWaringButton(QString btName, bool type)
TEST_F(test_schedulectrldlg, addWaringButton)
{
    mScheduleCtrlDlg->addsuggestButton("schedule", true);
    mScheduleCtrlDlg->addsuggestButton("schedule", false);
}

//void CScheduleCtrlDlg::setText(QString str)
TEST_F(test_schedulectrldlg, setText)
{
    mScheduleCtrlDlg->setText("schedule");
}

//void CScheduleCtrlDlg::setInformativeText(QString str)
TEST_F(test_schedulectrldlg, setInfomativeText)
{
    mScheduleCtrlDlg->setInformativeText("schedule");
}

//int CScheduleCtrlDlg::clickButton()
TEST_F(test_schedulectrldlg, clickButton)
{
    mScheduleCtrlDlg->clickButton();
}

//changeEvent
TEST_F(test_schedulectrldlg, changeEvent)
{
    QEvent event(QEvent::FontChange);
    mScheduleCtrlDlg->setText(tr("You are changing the repeating rule of this event."));
    mScheduleCtrlDlg->setInformativeText(tr("Do you want to change all occurrences?"));
    mScheduleCtrlDlg->addPushButton(tr("Cancel", "button"), true);
    mScheduleCtrlDlg->addWaringButton(tr("Change All"), true);
    QApplication::sendEvent(mScheduleCtrlDlg, &event);
}
