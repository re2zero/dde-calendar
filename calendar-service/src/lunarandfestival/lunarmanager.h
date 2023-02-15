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
#ifndef LUNARMANAGER_H
#define LUNARMANAGER_H
#include "lunarandfestival.h"
#include "method_interface.h"

stLunarDayInfo SolarToLunar(qint32 year, qint32 month, qint32 day);
SolarMonthInfo GetSolarMonthCalendar(qint32 year, qint32 month, bool fill);
LunarMonthInfo GetLunarMonthCalendar(qint32 year, qint32 month, bool fill);
LunarMonthInfo GetLunarMonthCalendar(const SolarMonthInfo &solarMonth);
QList<stDay> GetMonthDays(qint32 year, qint32 month, qint32 start, qint32 days);
QList<int> GetPreMonth(qint32 year, qint32 month);
QList<int> GetNextMonth(qint32 year, qint32 month);
QList<stDayFestival> GetFestivalsInRange(const QDateTime &start, const QDateTime &end);
QList<stDayFestival> FilterDayFestival(QList<stDayFestival> &festivaldays, const QString &querykey);
//清空黄历数据
void logOffEmptyData();

#endif // LUNARMANAGER_H
