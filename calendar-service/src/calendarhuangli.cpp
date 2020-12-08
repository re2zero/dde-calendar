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
#include "calendarhuangli.h"
#include "lunarandfestival/lunarmanager.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

CalendarHuangLi::CalendarHuangLi(QObject *parent)
    : QObject(parent)
    , m_database(new HuangLiDataBase(this))
{
}

//获取指定公历月的假日信息
QString CalendarHuangLi::GetFestivalMonth(quint32 year, quint32 month)
{
    return m_database->QueryFestivalList(year, month);
}

QString CalendarHuangLi::GetHuangLiDay(quint32 year, quint32 month, quint32 day)
{
    stDay viewday{static_cast<int>(year), static_cast<int>(month), static_cast<int>(day)};
    QList<stDay> viewdate{viewday};
    CaHuangLiDayInfo hldayinfo;
    //获取阴历信息
    stLunarDayInfo lunardayinfo = SolarToLunar(static_cast<qint32>(year), static_cast<qint32>(month), static_cast<qint32>(day));
    //获取宜忌信息
    QList<stHuangLi> hllist = m_database->QueryHuangLiByDays(viewdate);
    //将黄历信息保存到CaHuangLiDayInfo
    //数据库中的宜忌信息是从2008年开始的，搜索之前年份的会导致hllist为空，所以需要判断hllist的size
    if (hllist.size() > 0) {
        //对宜忌赋值
        hldayinfo.mSuit = hllist.begin()->Suit;
        hldayinfo.mAvoid = hllist.begin()->Avoid;
    } else {
        //如果没有，设置为空
        hldayinfo.mSuit = "";
        hldayinfo.mAvoid = "";
    }
    hldayinfo.mGanZhiYear = lunardayinfo.GanZhiYear;
    hldayinfo.mGanZhiMonth = lunardayinfo.GanZhiMonth;
    hldayinfo.mGanZhiDay = lunardayinfo.GanZhiDay;
    hldayinfo.mLunarDayName = lunardayinfo.LunarDayName;
    hldayinfo.mLunarFestival = lunardayinfo.LunarFestival;
    hldayinfo.mLunarLeapMonth = lunardayinfo.LunarLeapMonth;
    hldayinfo.mLunarMonthName = lunardayinfo.LunarMonthName;
    hldayinfo.mSolarFestival = lunardayinfo.SolarFestival;
    hldayinfo.mTerm = lunardayinfo.Term;
    hldayinfo.mZodiac = lunardayinfo.Zodiac;
    hldayinfo.mWorktime = lunardayinfo.Worktime;
    //返回json
    return hldayinfo.toJson();
}

QString CalendarHuangLi::GetHuangLiMonth(quint32 year, quint32 month, bool fill)
{
    CaHuangLiMonthInfo monthinfo;
    SolarMonthInfo solarmonth = GetSolarMonthCalendar(year, month, fill);
    LunarMonthInfo lunarmonth = GetLunarMonthCalendar(year, month, fill);
    monthinfo.mFirstDayWeek = lunarmonth.FirstDayWeek;
    monthinfo.mDays = lunarmonth.Days;
    QList<stHuangLi> hllist = m_database->QueryHuangLiByDays(solarmonth.Datas);
    for (int i = 0; i < lunarmonth.Datas.size(); ++i) {
        CaHuangLiDayInfo hldayinfo;
        hldayinfo.mAvoid = hllist.at(i).Avoid;
        hldayinfo.mSuit = hllist.at(i).Suit;
        stLunarDayInfo lunardayinfo = lunarmonth.Datas.at(i);
        hldayinfo.mGanZhiYear = lunardayinfo.GanZhiYear;
        hldayinfo.mGanZhiMonth = lunardayinfo.GanZhiMonth;
        hldayinfo.mGanZhiDay = lunardayinfo.GanZhiDay;
        hldayinfo.mLunarDayName = lunardayinfo.LunarDayName;
        hldayinfo.mLunarFestival = lunardayinfo.LunarFestival;
        hldayinfo.mLunarLeapMonth = lunardayinfo.LunarLeapMonth;
        hldayinfo.mLunarMonthName = lunardayinfo.LunarMonthName;
        hldayinfo.mSolarFestival = lunardayinfo.SolarFestival;
        hldayinfo.mTerm = lunardayinfo.Term;
        hldayinfo.mZodiac = lunardayinfo.Zodiac;
        hldayinfo.mWorktime = lunardayinfo.Worktime;
        monthinfo.mCaLunarDayInfo.append(hldayinfo);
//        qDebug() << hldayinfo.mGanZhiYear << hldayinfo.mGanZhiMonth << hldayinfo.mGanZhiDay
//                 << hldayinfo.mLunarDayName << hldayinfo.mLunarFestival << hldayinfo.mLunarFestival
//                 << hldayinfo.mLunarLeapMonth << hldayinfo.mLunarMonthName << hldayinfo.mSolarFestival
//                 << hldayinfo.mTerm << hldayinfo.mZodiac << hldayinfo.mWorktime;
    }
    return monthinfo.toJson();
}

CaLunarDayInfo CalendarHuangLi::GetLunarInfoBySolar(quint32 year, quint32 month, quint32 day)
{
    CaLunarDayInfo lunardayinfo;
    //获取阴历信息
    stLunarDayInfo m_lunardayinfo = SolarToLunar(static_cast<qint32>(year), static_cast<qint32>(month), static_cast<qint32>(day));
    //将阴历信息保存到CaLunarDayInfo
    lunardayinfo.mGanZhiYear = m_lunardayinfo.GanZhiYear;
    lunardayinfo.mGanZhiMonth = m_lunardayinfo.GanZhiMonth;
    lunardayinfo.mGanZhiDay = m_lunardayinfo.GanZhiDay;
    lunardayinfo.mLunarDayName = m_lunardayinfo.LunarDayName;
    lunardayinfo.mLunarFestival = m_lunardayinfo.LunarFestival;
    lunardayinfo.mLunarLeapMonth = m_lunardayinfo.LunarLeapMonth;
    lunardayinfo.mLunarMonthName = m_lunardayinfo.LunarMonthName;
    lunardayinfo.mSolarFestival = m_lunardayinfo.SolarFestival;
    lunardayinfo.mTerm = m_lunardayinfo.Term;
    lunardayinfo.mZodiac = m_lunardayinfo.Zodiac;
    lunardayinfo.mWorktime = m_lunardayinfo.Worktime;
    //返回CaLunarDayInfo
    return lunardayinfo;
}

CaLunarMonthInfo CalendarHuangLi::GetLunarCalendarMonth(quint32 year, quint32 month, bool fill)
{
    CaLunarMonthInfo lunarmonthinfo;
    //获取阴历月信息
    LunarMonthInfo lunarmonth = GetLunarMonthCalendar(static_cast<qint32>(year), static_cast<qint32>(month), fill);
    //将阴历月信息保存到CaLunarMonthInfo
    lunarmonthinfo.mDays = lunarmonth.Days;
    lunarmonthinfo.mFirstDayWeek = lunarmonth.FirstDayWeek;
    for (int i = 0; i < lunarmonth.Datas.size(); ++i) {
        CaLunarDayInfo m_lunardayinfo;
        stLunarDayInfo lunardayinfo = lunarmonth.Datas.at(i);
        m_lunardayinfo.mGanZhiYear = lunardayinfo.GanZhiYear;
        m_lunardayinfo.mGanZhiMonth = lunardayinfo.GanZhiMonth;
        m_lunardayinfo.mGanZhiDay = lunardayinfo.GanZhiDay;
        m_lunardayinfo.mLunarDayName = lunardayinfo.LunarDayName;
        m_lunardayinfo.mLunarFestival = lunardayinfo.LunarFestival;
        m_lunardayinfo.mLunarLeapMonth = lunardayinfo.LunarLeapMonth;
        m_lunardayinfo.mLunarMonthName = lunardayinfo.LunarMonthName;
        m_lunardayinfo.mSolarFestival = lunardayinfo.SolarFestival;
        m_lunardayinfo.mTerm = lunardayinfo.Term;
        m_lunardayinfo.mZodiac = lunardayinfo.Zodiac;
        m_lunardayinfo.mWorktime = lunardayinfo.Worktime;
        lunarmonthinfo.mCaLunarDayInfo.append(m_lunardayinfo);
    }
    //返回CaLunarMonthInfo
    return lunarmonthinfo;
}
