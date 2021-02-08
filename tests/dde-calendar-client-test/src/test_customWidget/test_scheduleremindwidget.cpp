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
#include "test_scheduleremindwidget.h"

test_scheduleremindwidget::test_scheduleremindwidget()
{
    mScheduleRemindWidget = new SchecduleRemindWidget();
    mCenterWidget = new CenterWidget();
}

test_scheduleremindwidget::~test_scheduleremindwidget()
{
    delete mScheduleRemindWidget;
    mScheduleRemindWidget =  nullptr;
    delete mCenterWidget;
    mCenterWidget = nullptr;
}

QVector<ScheduleDataInfo> getScheduleRemindData()
{
    ScheduleDataInfo schedule1, schedule2;

    QDateTime currentDateTime = QDateTime::currentDateTime();
    schedule1.setID(1);
    schedule1.setBeginDateTime(currentDateTime);
    schedule1.setEndDateTime(currentDateTime.addDays(1));
    schedule1.setTitleName("scheduleOne");
    schedule1.setAllDay(true);
    schedule1.setType(1);
    schedule1.setRecurID(0);

    schedule2.setID(2);
    schedule2.setBeginDateTime(currentDateTime.addDays(1));
    schedule2.setEndDateTime(currentDateTime.addDays(1).addSecs(60 * 60));
    schedule2.setTitleName("scheduleTwo");
    schedule2.setAllDay(false);
    schedule2.setType(2);
    schedule2.setRecurID(0);

    QVector<ScheduleDataInfo> scheduleList{};
    scheduleList.append(schedule1);
    scheduleList.append(schedule2);
    return scheduleList;
}

//void SchecduleRemindWidget::setData(const ScheduleDataInfo &vScheduleInfo, const CSchedulesColor &gcolor)
TEST_F(test_scheduleremindwidget, setData)
{
    CSchedulesColor gdcolor = CScheduleDataManage::getScheduleDataManage()->getScheduleColorByType(2);
    mScheduleRemindWidget->setData(getScheduleRemindData().first(), gdcolor);
}

//void SchecduleRemindWidget::setDirection(DArrowRectangle::ArrowDirection value)
TEST_F(test_scheduleremindwidget, setdirection)
{
    mScheduleRemindWidget->setDirection(DArrowRectangle::ArrowDirection::ArrowRight);
}

//void CenterWidget::setData(const ScheduleDataInfo &vScheduleInfo, const CSchedulesColor &gcolor)
TEST_F(test_scheduleremindwidget, setDate)
{
    CSchedulesColor gdcolor = CScheduleDataManage::getScheduleDataManage()->getScheduleColorByType(2);
    mCenterWidget->setData(getScheduleRemindData().first(), gdcolor);
}

//void CenterWidget::setTheMe(const int type)
TEST_F(test_scheduleremindwidget, setTheMe)
{
    mCenterWidget->setTheMe(1);
    mCenterWidget->setTheMe(2);
}

//void CenterWidget::UpdateTextList()
TEST_F(test_scheduleremindwidget, updateTextList)
{
    mCenterWidget->UpdateTextList();
}
