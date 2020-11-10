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
#include "lunarcalendar.h"

#include <QTime>
#include <QDateTime>
#include <QDate>

QMap<int, LunarCalendar *> LunarCalendar::glYearCache;

LunarCalendar *LunarCalendar::GetLunarCalendar(qint32 year)
{
    auto it = glYearCache.find(year);
    LunarCalendar *plcal = nullptr;
    if (it != glYearCache.end()) {
        plcal = it.value();
    } else {
        plcal = new LunarCalendar(year);
        glYearCache.insert(year, plcal);
    }
    return plcal;
}

/**
 * 公历某月日历
 * year,month 公历年，月
 * fill 是否用上下月数据补齐首尾空缺，首例数据从周日开始(7*6阵列)
 */
SolarMonthInfo LunarCalendar::GetSolarMonthCalendar(qint32 year, qint32 month, bool fill)
{
}

//指定年份内公历日期转换为农历日
lunarInfo LunarCalendar::SolarDayToLunarDay(qint32 month, qint32 day)
{
    lunarInfo dayinfo;
    QDateTime dt(QDate(Year, month, day), QTime(0, 0, 0, 0), Qt::TimeSpec::UTC);
    int yd = dt.date().dayOfYear();

    // 求月地支
    int monthZhi = 0;
    while (monthZhi < solarTermYearDays.size()) {
        if (yd >= solarTermYearDays[monthZhi]) {
            monthZhi++;
        } else {
            break;
        }
    }
    dayinfo.MonthZhi = monthZhi;
    // 求农历年、月、日
    foreach (lunarInfo lm, Months) {
        int dd = deltaDays(lm.ShuoTime, dt) + 1;
        if (1 <= dd && dd <= lm.LunarMonthDays) {
            dayinfo.LunarYear = lm.LunarYear;
            dayinfo.LunarMonthName = lm.LunarMonthName;
            dayinfo.LunarMonthDays = lm.LunarMonthDays;
            dayinfo.ShuoJD = lm.ShuoJD;
            dayinfo.ShuoTime = lm.ShuoTime;
            dayinfo.IsLeap = lm.IsLeap;
            dayinfo.LunarDay = dd;
            break;
        }
    }
    // 求二十四节气
    dayinfo.SolarTerm = getSolarTermInfo(month, day);
    return dayinfo;
}

LunarCalendar::LunarCalendar(qint32 year)
{
    Year = year;
    Months.reserve(13);
    calcProcData();
    fillMonths();
    calcLeapMonth();
}

void LunarCalendar::calcProcData()
{
    SolarTermJDs = get25SolarTermJDs(Year - 1, DongZhi);
    for (int i = 0; i < 25; i++) {
        SolarTermTimes.append(GetDateTimeFromJulianDay(SolarTermJDs[i]));
    }
    for (int i = 1; i < 25; i += 2) {
        int yd = SolarTermTimes[i].date().dayOfYear();
        solarTermYearDays.append(yd);
    }
    double beijin2utc = JDBeijingTime2UTC(SolarTermJDs[0]);
    double tmpNewMoonJD = getNewMoonJD(beijin2utc);
    if (tmpNewMoonJD > SolarTermJDs[0]) {
        tmpNewMoonJD -= 29.53;
    }
    NewMoonJDs = get15NewMoonJDs(tmpNewMoonJD);
}

void LunarCalendar::fillMonths()
{
    //采用夏历建寅，冬至所在月份为农历11月(冬月)
    int yuejian = 11;
    for (int i = 0; i < 14; i++) {
        lunarInfo info;
        if (yuejian <= 12) {
            info.LunarMonthName = yuejian;
            info.LunarYear = Year - 1;
        } else {
            info.LunarMonthName = yuejian - 12;
            info.LunarYear = Year;
        }

        info.ShuoJD = NewMoonJDs[i];
        info.ShuoTime = GetDateTimeFromJulianDay(info.ShuoJD);
        double nextShuoJD = NewMoonJDs[i + 1];
        QDateTime nextShuoTime = GetDateTimeFromJulianDay(nextShuoJD);
        info.LunarMonthDays = deltaDays(info.ShuoTime, nextShuoTime);
        Months.append(info);
        yuejian++;
    }
}

void LunarCalendar::calcLeapMonth()
{
    // 根据节气计算是否有闰月，如果有闰月，根据农历月命名规则，调整月名称
    if (int(NewMoonJDs[13] + 0.5) <= int(SolarTermJDs[24] + 0.5)) {
        // 第13月的月末没有超过冬至，说明今年需要闰一个月
        int i = 1;
        while (i < 14) {
            if (int(NewMoonJDs[i + 1] + 0.5) <= int(SolarTermJDs[2 * i] + 0.5)) {
                //NewMoonJDs[i + 1] 是第i个农历月的下一个月的月首
                //本该属于第i个月的中气如果比下一个月的月首还晚，或者与下个月的月首是同一天（民间历法），则说明第 i 个月没有中气, 是闰月
                break;
            }
            i++;
        }
        if (i < 14) {
            // 找到闰月
            //qDebug()<<QString("找到闰月 %1").arg(i);
            Months[i].IsLeap = true;
            // 对后面的农历月调整月名
            while (i < 14) {
                Months[i].LunarMonthName--;
                i++;
            }
        }
    }
}

qint32 LunarCalendar::getSolarTermInfo(qint32 month, qint32 day)
{
    int index = 2 * month - 1;
    qint32 SolarTerm = -1;
    QDateTime dt1 = SolarTermTimes[index];
    QDateTime dt2 = SolarTermTimes[index + 1];
    if (dt1.date().day() == day) {
        SolarTerm = (index + DongZhi) % 24;
    } else if (dt2.date().day() == day) {
        SolarTerm = (index + 1 + DongZhi) % 24;
    }
    return SolarTerm;
}
