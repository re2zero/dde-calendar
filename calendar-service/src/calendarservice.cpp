/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     kongyunzhen <kongyunzhen@uniontech.com>
*
* Maintainer: kongyunzhen <kongyunzhen@uniontech.com>
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
#include "calendarservice.h"

CalendarService::CalendarService(QObject *parent)
    : QObject(parent)
{
    CaLunarDayInfo::registerMetaType();
    CaLunarMonthInfo::registerMetaType();
    CaHuangLiDayInfo::registerMetaType();
    CaHuangLiMonthInfo::registerMetaType();
    m_scheduler = new CalendarScheduler(this);
    m_huangli = new CalendarHuangLi(this);
}

//获取指定公历月的假日信息
QString CalendarService::GetFestivalMonth(quint32 year, quint32 month)
{
    return m_huangli->GetFestivalMonth(year, month);
}

//获取指定公历日的黄历信息
QString CalendarService::GetHuangLiDay(quint32 year, quint32 month, quint32 day)
{
    return m_huangli->GetHuangLiDay(year, month, day);
}

//获取指定公历月的黄历信息
QString CalendarService::GetHuangLiMonth(quint32 year, quint32 month, bool fill)
{
    return m_huangli->GetHuangLiMonth(year, month, fill);
}

//通过公历回去阴历信息
CaLunarDayInfo CalendarService::GetLunarInfoBySolar(quint32 year, quint32 month, quint32 day)
{
    return m_huangli->GetLunarInfoBySolar(year, month, day);
}

//获取阴历月信息
CaLunarMonthInfo CalendarService::GetLunarMonthCalendar(quint32 year, quint32 month, bool fill)
{
    return m_huangli->GetLunarCalendarMonth(year, month, fill);
}

qint64 CalendarService::CreateJob(const QString &jobInfo)
{
    return 0;
}

qint64 CalendarService::CreateType(const QString &typeInfo)
{
    return 0;
}

void CalendarService::DeleteJob(qint64 id)
{
}

void CalendarService::DeleteType(qint64 id)
{
}

QString CalendarService::GetJob(qint64 id)
{
    return "";
}

QString CalendarService::GetJobs(quint32 startYear, quint32 startMonth, quint32 startDay, quint32 endYear, quint32 endMonth, quint32 endDay)
{
    return "";
}

QString CalendarService::GetJobsWithLimit(quint32 startYear, quint32 startMonth, quint32 startDay, quint32 endYear, quint32 endMonth, quint32 endDay, quint32 maxNum)
{
    return "";
}

QString CalendarService::GetJobsWithRule(quint32 startYear, quint32 startMonth, quint32 startDay, quint32 endYear, quint32 endMonth, quint32 endDay, const QString &rule)
{
    return "";
}

QString CalendarService::GetType(qint64 id)
{
    return m_scheduler->GetType(id);
}

QString CalendarService::GetTypes()
{
    return "";
}

QString CalendarService::QueryJobs(const QString &params)
{
    return "";
}

void CalendarService::UpdateJob(const QString &jobInfo)
{
}

void CalendarService::UpdateType(const QString &typeInfo)
{
}
