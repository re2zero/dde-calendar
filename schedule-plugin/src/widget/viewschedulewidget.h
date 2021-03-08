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
#ifndef VIEWSCHEDULEWIDGET_H
#define VIEWSCHEDULEWIDGET_H

#include <QWidget>
#include "icondframe.h"
#include "../data/schedulestructs.h"
#include "scheduleitemwidget.h"

class viewschedulewidget : public IconDFrame
{
    Q_OBJECT
public:
    explicit viewschedulewidget(QWidget *parent = nullptr);
    ~viewschedulewidget();
    void setScheduleDateRangeInfo(QVector<ScheduleDateRangeInfo> &scheduleDateInfo);
    void viewScheduleInfoShow(QVector<ScheduleDateRangeInfo> m_showdate);
    int getScheduleNum(QVector<ScheduleDateRangeInfo> m_showdate);
    void setQueryBeginDateTime(QDateTime begindatetime);
    void setQueryEndDateTime(QDateTime enddatetime);

    QVector<ScheduleDateRangeInfo> queryScheduleWithTime(QVector<ScheduleDateRangeInfo> &scheduleinfo, QTime beginT, QTime endT);
    QVector<ScheduleDateRangeInfo> queryScheduleWithDate(QVector<ScheduleDateRangeInfo> &scheduleinfo, QDate beginD, QDate endD);
    QVector<ScheduleDateRangeInfo> queryScheduleWithWeek(QVector<ScheduleDateRangeInfo> &scheduleinfo, QVector<int> weekDay, int dayofweek = 0, QTime beginT = QTime(0, 0, 0), QTime endT = QTime(0, 0, 0));
    QVector<ScheduleDateRangeInfo> queryScheduleWithMonth(QVector<ScheduleDateRangeInfo> &scheduleinfo, QVector<int> monthDay, int dayofmonth = 0, QTime beginT = QTime(0, 0, 0), QTime endT = QTime(0, 0, 0));

    QVector<ScheduleDateRangeInfo> getAllScheduleInfo();
    QVector<ScheduleDateRangeInfo> getAllRpeatScheduleInfo(int rpeat);
    /**
     * @brief getNextScheduleInfo 获取下一个日程
     * @return 下一个日程信息
     */
    QVector<ScheduleDateRangeInfo> getNextScheduleInfo();
public slots:
    void slotItemPress(const ScheduleDtailInfo &info);

private:
    QVector<ScheduleDtailInfo> m_scheduleInfo;
    QVector<ScheduleDateRangeInfo> m_scheduleDateInfo;
    QVector<ScheduleDateRangeInfo> m_showdate;
    QDateTime m_beginDateTime;
    QDateTime m_endDateTime;
};

#endif // QUERYSCHEDULEWIDGET_H
