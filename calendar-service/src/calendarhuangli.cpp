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
#include "dbmanager/huanglidatabase.h"
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
    return "";
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
    }
    qDebug() << monthinfo;
    return monthinfo.toJson();
}
