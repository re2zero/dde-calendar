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
#ifndef CALENDARSERVICE_H
#define CALENDARSERVICE_H

#include "src/commondef.h"
#include "src/dbusdatastruct.h"
#include "calendarscheduler.h"
#include "calendarhuangli.h"

#include <QObject>
#include <QDBusContext>

class CalendarService : public QObject
    , protected QDBusContext
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.dataserver.calendar")
public:
    explicit CalendarService(QObject *parent = nullptr);

Q_SIGNALS:

public Q_SLOTS:
    //LunarCalendar
    Q_SCRIPTABLE QString GetFestivalMonth(quint32 year, quint32 month);
    Q_SCRIPTABLE QString GetHuangLiDay(quint32 year, quint32 month, quint32 day);
    Q_SCRIPTABLE QString GetHuangLiMonth(quint32 year, quint32 month, bool fill);
    Q_SCRIPTABLE CaLunarDayInfo GetLunarInfoBySolar(quint32 year, quint32 month, quint32 day);
    Q_SCRIPTABLE CaLunarMonthInfo GetLunarMonthCalendar(quint32 year, quint32 month, bool fill);

    //CalendarScheduler
    Q_SCRIPTABLE qint64 CreateJob(const QString &jobInfo);
    Q_SCRIPTABLE qint64 CreateType(const QString &typeInfo);
    Q_SCRIPTABLE void DeleteJob(qint64 id);
    Q_SCRIPTABLE void DeleteType(qint64 id);
    Q_SCRIPTABLE QString GetJob(qint64 id);
    Q_SCRIPTABLE QString GetJobs(quint32 startYear, quint32 startMonth, quint32 startDay, quint32 endYear, quint32 endMonth, quint32 endDay);
    Q_SCRIPTABLE QString GetJobsWithLimit(quint32 startYear, quint32 startMonth, quint32 startDay, quint32 endYear, quint32 endMonth, quint32 endDay, quint32 maxNum);
    Q_SCRIPTABLE QString GetJobsWithRule(quint32 startYear, quint32 startMonth, quint32 startDay, quint32 endYear, quint32 endMonth, quint32 endDay, const QString &rule);
    Q_SCRIPTABLE QString GetType(qint64 id);
    Q_SCRIPTABLE QString GetTypes();
    Q_SCRIPTABLE QString QueryJobs(const QString &params);
    Q_SCRIPTABLE void UpdateJob(const QString &jobInfo);
    Q_SCRIPTABLE void UpdateType(const QString &typeInfo);

private:
    CalendarScheduler *m_scheduler;
    CalendarHuangLi *m_huangli;
};

#endif // CALENDARSERVICE_H
