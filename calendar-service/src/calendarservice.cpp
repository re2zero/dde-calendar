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
#include "src/commondatastruct.h"

CalendarService::CalendarService(QObject *parent)
    : QObject(parent)
{
    CaLunarDayInfo::registerMetaType();
    CaLunarMonthInfo::registerMetaType();
    CaHuangLiDayInfo::registerMetaType();
    CaHuangLiMonthInfo::registerMetaType();
    qRegisterMetaType<Job>("Job");
    qRegisterMetaType<QList<Job>>("QList<Job>");
    m_scheduler = new CalendarScheduler(this);
    m_huangli = new CalendarHuangLi(this);
    initConnections();
}

void CalendarService::initConnections()
{
    connect(m_scheduler, &CalendarScheduler::JobsUpdated, this, &CalendarService::JobsUpdated);
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

// 根据日程json创建日程信息，并返回jobID
qint64 CalendarService::CreateJob(const QString &jobInfo)
{
    return m_scheduler->CreateJob(jobInfo);
}

qint64 CalendarService::CreateType(const QString &typeInfo)
{
    Q_UNUSED(typeInfo);
    return 0;
}

// 根据日程id来删除日程记录
void CalendarService::DeleteJob(qint64 id)
{
    m_scheduler->DeleteJob(id);
}

// 根据日程id来删除job类型记录
void CalendarService::DeleteType(qint64 id)
{
    m_scheduler->DeleteType(id);
}

QString CalendarService::GetJob(qint64 id)
{
    return m_scheduler->GetJob(id);
}

/**
 * @brief  GetJobs 获取指定范围内的日程
 * @param startYear 起始年信息
 * @param startMonth 起始月信息
 * @param startDay 起始日信息
 * @param endYear 结束年信息
 * @param endMonth 结束月信息
 * @param endDay 结束日信息
 * @return 返回指定范围内的日程JSON格式
 */
QString CalendarService::GetJobs(quint32 startYear, quint32 startMonth, quint32 startDay, quint32 endYear, quint32 endMonth, quint32 endDay)
{
    //getjobs查询的时间为一整天,给查询的日期赋上精准的时间，和queryjobs查询保持一致
    QDate startdate(static_cast<int>(startYear), static_cast<int>(startMonth), static_cast<int>(startDay));
    //查询的开始时间为当天的0：0
    QDateTime start(startdate);
    QDate enddate(static_cast<int>(endYear), static_cast<int>(endMonth), static_cast<int>(endDay));
    //getjobs查询是以天为单位的，如果没有设置时间，则默认时间为00:00，那么查询的就是一个时间点，不符合期望，所以需要显示的设置一个当天最晚的时间点，来代表一整天的时间。
    QDateTime end(enddate, QTime(23, 59));
    return m_scheduler->GetJobs(start, end);
}

//根据id返回指定日程类型
QString CalendarService::GetType(qint64 id)
{
    return m_scheduler->GetType(id);
}

//返回所有日程类型
QString CalendarService::GetTypes()
{
    return m_scheduler->GetTypes();
}

QString CalendarService::QueryJobs(const QString &params)
{
    return m_scheduler->QueryJobs(params);
}

// 传入要改动的日程信息来更新数据库
void CalendarService::UpdateJob(const QString &jobInfo)
{
    m_scheduler->UpdateJob(jobInfo);
}

// 传入要改动的日程类型信息来更新数据库
void CalendarService::UpdateType(const QString &typeInfo)
{
    m_scheduler->UpdateType(typeInfo);
}

QString CalendarService::QueryJobsWithLimit(const QString &params, qint32 maxNum)
{
    return m_scheduler->QueryJobsWithLimit(params, maxNum);
}

QString CalendarService::QueryJobsWithRule(const QString &params, const QString &rules)
{
    return m_scheduler->QueryJobsWithRule(params, rules);
}
