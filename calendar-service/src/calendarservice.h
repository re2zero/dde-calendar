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
    Q_SCRIPTABLE QString GetFestivalMonth(int32_t year, int32_t month);
    Q_SCRIPTABLE QString GetHuangLiDay(int32_t year, int32_t month, int32_t day);
    Q_SCRIPTABLE QString GetHuangLiMonth(int32_t year, int32_t month, bool fill);
    Q_SCRIPTABLE QString GetLunarInfoBySolar(int32_t year, int32_t month, int32_t day);
    Q_SCRIPTABLE QString GetLunarMonthCalendar(int32_t year, int32_t month, bool fill);

    //CalendarScheduler
};

#endif // CALENDARSERVICE_H
