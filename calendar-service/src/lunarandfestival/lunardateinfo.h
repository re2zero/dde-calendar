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
#ifndef LUNARDATEINFO_H
#define LUNARDATEINFO_H

#include "lunarandfestival.h"
#include "method_interface.h"

#include <QDate>
#include <QVector>

/**
 * @brief The LunarDateInfo class
 * 农历时间，用于计算农历重复日程
 */
class LunarDateInfo
{
public:
    LunarDateInfo();
    QVector<QDate> getAllNextMonthLunarDayBySolar(const QDate &beginDate, const QDate &endDate, const QDate &solarDate);
    QVector<QDate> getAllNextYearLunarDayBySolar(const QDate &beginDate, const QDate &endDate, const QDate &solarDate);

private:
    lunarInfo getNextMonthLunarDay(QDate &nextDate, const lunarInfo &info);
};

#endif // LUNARDATEINFO_H
