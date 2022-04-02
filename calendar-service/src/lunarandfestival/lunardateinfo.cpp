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
#include "lunardateinfo.h"

#include "lunarcalendar.h"

#include <QDebug>

//农历一年最少有363天
const int LunarYearMiniDays = 353;

LunarDateInfo::LunarDateInfo()
{
}

//通过公历时间获取范围内该时间的农历天的具体公历日期
QVector<QDate> LunarDateInfo::getAllNextMonthLunarDayBySolar(const QDate &beginDate, const QDate &endDate, const QDate &solarDate)
{
    QVector<QDate> solar;
    //如果需要通过公历信息获取下一个对应农历信息对应的天
    //不在范围内直接返回
    if (endDate < beginDate || solarDate > endDate) {
        qWarning() << QString("开始时间小于结束时间或者需要计算的起始时间晚于结束时间:");
        qWarning() << "beginDate:" << beginDate
                   << " endDate:" << endDate
                   << " solarDate:" << solarDate;
        return solar;
    }
    LunarCalendar *lunc = LunarCalendar::GetLunarCalendar(solarDate.year());
    lunarInfo info = lunc->SolarDayToLunarDay(solarDate.month(), solarDate.day());

    QDate nextSolar = beginDate;
    //如果日程开始时间在查询起始时间之前
    if (solarDate > nextSolar) {
        nextSolar = solarDate;
    }
    while (true) {
        info = getNextMonthLunarDay(nextSolar, info);
        //如果超过查询范围则退出
        if (nextSolar > endDate) {
            break;
        }
        solar.append(nextSolar);
        nextSolar = nextSolar.addDays(info.LunarMonthDays);
    }
    return solar;
}

QVector<QDate> LunarDateInfo::getAllNextYearLunarDayBySolar(const QDate &beginDate, const QDate &endDate, const QDate &solarDate)
{
    QVector<QDate> solar;
    //不在范围内直接返回
    if (endDate < beginDate || solarDate > endDate) {
        qWarning() << QString("开始时间小于结束时间或者需要计算的起始时间晚于结束时间:");
        qWarning() << "beginDate:" << beginDate
                   << " endDate:" << endDate
                   << " solarDate:" << solarDate;
        return solar;
    }

    //TODO: 需要优化
    //日程的农历日期
    LunarCalendar *solarDateLunar = LunarCalendar::GetLunarCalendar(solarDate.year());
    lunarInfo info = solarDateLunar->SolarDayToLunarDay(solarDate.month(), solarDate.day());
    QDate bDate = beginDate;
    //查询起始时间之前的时间不参加计算
    //如果日程开始时间在查询起始时间之前
    if (solarDate > bDate) {
        //更新查询起始时间为开始日程时间
        bDate = solarDate;
    }

    //如果日程的开始时间小于查询的起始时间，则从查询起始时间开始计算
    while (bDate <= endDate) {
        //开始时间农历日期
        LunarCalendar *lunc = LunarCalendar::GetLunarCalendar(bDate.year());
        lunarInfo startLunarInfo = lunc->SolarDayToLunarDay(bDate.month(), bDate.day());
        //判断起始时间的农历月份是否大于日程的月份，如果大于则说明起始时间的农历年份没有对应的重复日程，直接计算下一个农历年份
        if (startLunarInfo.LunarMonthName > info.LunarMonthName) {
            //更新起始时间
            //农历一个月的天数范围为28-30，则农历月份最小时间查为（间隔月份*28)天
            int miniInterval = (12 - startLunarInfo.LunarMonthName + info.LunarMonthName) * 28 - startLunarInfo.LunarDay;
            bDate = bDate.addDays(miniInterval);
            continue;
        } else if (startLunarInfo.LunarMonthName == info.LunarMonthName) {
            //如果为同一个月，判断是否都为闰月或都不是闰月
            if (startLunarInfo.IsLeap == info.IsLeap) {
                //如果起始农历天日程农历天之前或为同一农历天
                if (startLunarInfo.LunarDay <= info.LunarDay) {
                    int intervalDay = info.LunarDay - startLunarInfo.LunarDay;
                    //如果该农历月没有这一天，那就计算下一年
                    if (startLunarInfo.LunarDay + intervalDay > startLunarInfo.LunarMonthDays) {
                        //农历一年最少有363天
                        bDate = bDate.addDays(LunarYearMiniDays);
                        continue;
                    }
                    //如果有这一天则添加对应的日期
                    bDate = bDate.addDays(intervalDay);
                    solar.append(bDate);
                    //更新查询开始时间
                    bDate = bDate.addDays(LunarYearMiniDays);
                    continue;
                } else {
                    //如果起始农历天在日程农历天之后，则更新到下一年
                    int offsetMiniDay = LunarYearMiniDays - (startLunarInfo.LunarDay - info.LunarDay);
                    bDate = bDate.addDays(offsetMiniDay);
                    continue;
                }
            } else {
                //如果起始时间为闰月
                if (startLunarInfo.IsLeap) {
                    //如果起始农历天在日程农历天之后，则更新到下一年
                    int offsetMiniDay = LunarYearMiniDays - (startLunarInfo.LunarDay - info.LunarDay);
                    bDate = bDate.addDays(offsetMiniDay);
                    continue;
                }
                //如果日程开始时间为闰月
                if (info.IsLeap) {
                    //更新起始时间的日期，更新为农历日期的下个月的初一
                    bDate = bDate.addDays(startLunarInfo.LunarMonthDays - startLunarInfo.LunarDay + 1);
                    continue;
                }
            }

        } else {
            //如果起始时间的农历月份早于日期的月份
            //农历一个月的天数范围为28-30
            //相差的月份
            int offsetMonth = info.LunarMonthName - startLunarInfo.LunarMonthName;
            int offsetMiniDay = 0;
            if (offsetMonth > 1) {
                //如果间隔大于一个月
                offsetMiniDay = (info.LunarMonthName - startLunarInfo.LunarMonthName) * 28 - startLunarInfo.LunarMonthName + info.LunarMonthName;
            } else {
                //如果间隔等于一个月，因为前面已经判断了是否为同一个月，所以不存在间隔为0的情况
                offsetMiniDay = startLunarInfo.LunarMonthDays - -startLunarInfo.LunarMonthName + info.LunarMonthName;
            }

            bDate = bDate.addDays(offsetMiniDay);
            continue;
        }
    }
    return solar;
}

lunarInfo LunarDateInfo::getNextMonthLunarDay(QDate &nextDate, const lunarInfo &info)
{
    LunarCalendar *lunc = LunarCalendar::GetLunarCalendar(nextDate.year());
    lunarInfo nextinfo = lunc->SolarDayToLunarDay(nextDate.month(), nextDate.day());
    //判断农历的天是否为重复的天，比如一月初一，加上一月份的天数应该为二月初一
    //如果不一样，则说明这个月没有这一天，比如正月三十，加上正月的月份天数，到了二月份是没有三十的，
    if (nextinfo.LunarDay != info.LunarDay) {
        nextDate = nextDate.addDays(info.LunarDay - nextinfo.LunarDay);
        return getNextMonthLunarDay(nextDate, info);
    }
    return nextinfo;
}
