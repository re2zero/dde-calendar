/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     hejinghai <hejinghai@uniontech.com>
*
* Maintainer: hejinghai <hejinghai@uniontech.com>
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
#include "test_method_interface.h"
#include <QVector>
#include <QDebug>

test_method_interface::test_method_interface()
{

}

//QString GetLunarMonthName(int lunarmonthname, bool isleap)
TEST_F(test_method_interface, GetLunarMonthName)
{
    // 腊月
    int lunarmonthname = 12;
    bool isleap = false;
    QString lunarMName = GetLunarMonthName(lunarmonthname, isleap);
    assert("腊月" == lunarMName);

    // 闰腊月
    isleap = true;
    lunarMName = GetLunarMonthName(lunarmonthname, isleap);
    assert("闰腊月" == lunarMName);
}

//QString GetLunarDayName(int lundayname)
TEST_F(test_method_interface, GetLunarDayName)
{
    // "初八"
    int lundayname = 8;
    QString lunarDayName = GetLunarDayName(lundayname);
    //qInfo() << lunarDayName;
    assert("初八" == lunarDayName);

    // "廿五"
    lundayname = 25;
    lunarDayName = GetLunarDayName(lundayname);
    //qInfo() << lunarDayName;
    assert("廿五" == lunarDayName);
}

//QString GetLunarDayFestival(int monthname, int lunarday, int lunarmonthdays, int solarterm)
TEST_F(test_method_interface, GetLunarDayFestival)
{
    // "除夕"
    int monthname = 12;
    int lunarday = 30;
    int lunarmonthdays = 30;
    int solarterm = 1;
    QString dayFestival = GetLunarDayFestival(monthname, lunarday, lunarmonthdays, solarterm);
    //qInfo() << dayFestival;
    assert("除夕" == dayFestival);

    // "清明节"
    monthname = 4;
    lunarday = 2;
    lunarmonthdays = 23;
    solarterm = 1;
    dayFestival = GetLunarDayFestival(monthname, lunarday, lunarmonthdays, solarterm);
    //qInfo() << dayFestival;
    assert("清明节" == dayFestival);

    // ""
    monthname = 5;
    lunarday = 1;
    lunarmonthdays = 13;
    solarterm = 0;
    dayFestival = GetLunarDayFestival(monthname, lunarday, lunarmonthdays, solarterm);
    //qInfo() << dayFestival;
    assert("" == dayFestival);
}

//QString GetSolarTermName(int order)
TEST_F(test_method_interface, GetSolarTermName)
{
    // "小满"
    int order = 4;
    QString stName = GetSolarTermName(order);
    //qInfo() << stName;
    assert("小满" == stName);

    // ""
    order = 24;
    stName = GetSolarTermName(order);
    //qInfo() << stName;
    assert("" == stName);
}

//QString GetGanZhiMonth(int year, int monthzhi)
TEST_F(test_method_interface, GetGanZhiMonth)
{
    // "戊子"
    int year = 2020;
    int month = 12;
    QString ganzhi = GetGanZhiMonth(year, month);
    //qInfo() << ganzhi;
    assert("戊子" == ganzhi);
}

//QString GetGanZhiYear(int lunaryear)
TEST_F(test_method_interface, GetGanZhiYear)
{
    // "庚子"
    int lunaryear = 2020;
    QString ganzhi = GetGanZhiYear(lunaryear);
    //qInfo() << ganzhi;
    assert("庚子" == ganzhi);
}

//QString GetGanZhiDay(int year, int month, int day)
TEST_F(test_method_interface, GetGanZhiDay)
{
    // "己丑"
    int year = 2020;;
    int month = 12;
    int day = 12;
    QString ganzhi = GetGanZhiDay(year, month, day);
    assert("己丑" == ganzhi);
}

//QString GetYearZodiac(int lunaryear)
TEST_F(test_method_interface, GetYearZodiac)
{
    // "鼠"
    int lunaryear = 2020;
    QString Zodiac = GetYearZodiac(lunaryear);
    //qInfo() << Zodiac;
    assert("鼠" == Zodiac);

    // "猪"
    lunaryear = 2019;
    Zodiac = GetYearZodiac(lunaryear);
    //qInfo() << Zodiac;
    assert("猪" == Zodiac);
}

//QVector<double> get25SolarTermJDs(int year, int start)
TEST_F(test_method_interface, get25SolarTermJDs)
{
    int year = 0;
    int start = 0;
    QVector<double> jds = get25SolarTermJDs(year, start);
    // TODO
    assert(1 == 1);
}

//double GetSolarTermJD(int year, int order)
TEST_F(test_method_interface, GetSolarTermJD)
{
    int year = 2020;
    int order = 0; //春分 3月20日
    double jd = GetSolarTermJD(year, order);
    assert(2.45893e+06 > jd);
}

//bool IsLeapYear(int year)
TEST_F(test_method_interface, IsLeapYear)
{
    int leapYear = 2020; // 闰年
    int notLeapYear = 2019; // 非闰年
    assert(true == IsLeapYear(leapYear) && false == IsLeapYear(notLeapYear));
}

// int GetSolarMonthDays(int year, int month)
TEST_F(test_method_interface, GetSolarMonthDays)
{
    int year = 2020;
    int month = 2;
    int a = GetSolarMonthDays(year, month);
    assert(29 == a);
}

//int GetWeekday(int y, int m, int d)
TEST_F(test_method_interface, GetWeekday)
{
    int y = 2020;
    int m = 2;
    int d = 29;
    int a = GetWeekday(y, m, d);
    assert(6 == a);
}

//double DmsToDegrees(int degrees, int mintues, double seconds)
TEST_F(test_method_interface, DmsToDegrees)
{
    int degrees = 4;
    int mintues = 3;
    double seconds = 7200;
    double degress = DmsToDegrees(degrees, mintues, seconds);
    //qInfo() << degress;
    assert(6.05 == degress);
}

//double DmsToSeconds(int d, int m, double s)
TEST_F(test_method_interface, DmsToSeconds)
{
    int d = 4;
    int m = 3;
    double s = 7200;
    double degress = DmsToSeconds(d, m, s);
    //qInfo() << degress;
    assert(21780 == degress);
}

//double DmsToRadians(int d, int m, int s)
TEST_F(test_method_interface, DmsToRadians)
{
    int d = 4;
    int m = 3;
    double s = 7200;
    double degress = DmsToRadians(d, m, s);
    //qInfo() << degress;
    assert(0.105592 <= degress);
}

//QDateTime GetDateTimeFromJulianDay(double jd)
TEST_F(test_method_interface, GetDateTimeFromJulianDay)
{
    int year = 2020;
    int order = 0; //春分 3月20日
    double jd = GetSolarTermJD(year, order);

    QString strJulianDay = "周五 3月 20 03:49:33 2020 GMT";
    QDateTime julianDay = GetDateTimeFromJulianDay(jd);
    //qInfo() << julianDay.toString();
    assert(strJulianDay.contains(julianDay.toString()));
}

//double GetDeltaT(int year, int month)
TEST_F(test_method_interface, GetDeltaT)
{
    const int count = 15;
    const int year[count] = {-501, 0, 1599, 1699, 1799,
                  1859, 1899, 1919, 1940, 1960,
                  1985, 2004, 2049, 2149, 2150};
    const int mouth = 6;
    double deltaT[15] = {};
    for (int i = 0; i < count; ++i) {
        deltaT[i] = GetDeltaT(year[i], mouth);
    }
    assert(1 == 1);
}

//double JDBeijingTime2UTC(double bjtJD)
TEST_F(test_method_interface, JDBeijingTime2UTC)
{
    double bjtJD = 1.01;
    double utc = JDBeijingTime2UTC(bjtJD);
    qInfo() << "utc" << utc ;
    assert(0.67667 > utc);
}

//QString GetSolarDayFestival(int year, int month, int day)
TEST_F(test_method_interface, GetSolarDayFestival)
{
    int year = 2020;
    // 建军节
    int month = 8;
    int day = 1;
    QString getFesStr = GetSolarDayFestival(year, month, day);
    assert("建军节" == getFesStr);

    // 儿童节
    month = 6;
    day = 1;
    getFesStr = GetSolarDayFestival(year, month, day);
    assert("儿童节" == getFesStr);
}
