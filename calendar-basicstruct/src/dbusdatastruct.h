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
#ifndef COMMONDATASTRUCT_H
#define COMMONDATASTRUCT_H

#include <QtCore/QDebug>
#include <QtDBus/QtDBus>

class CaLunarDayInfo
{
public:
    CaLunarDayInfo() {}

    static void registerMetaType();

    friend QDebug operator<<(QDebug argument, const CaLunarDayInfo &what);
    friend QDBusArgument &operator<<(QDBusArgument &argument, const CaLunarDayInfo &what);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument, CaLunarDayInfo &what);

public:
    QString mGanZhiYear;
    QString mGanZhiMonth;
    QString mGanZhiDay;
    QString mLunarMonthName;
    QString mLunarDayName;
    qint32 mLunarLeapMonth;
    QString mZodiac;
    QString mTerm;
    QString mSolarFestival;
    QString mLunarFestival;
    qint32 mWorktime;
};

class CaLunarMonthInfo
{
public:
    CaLunarMonthInfo() {}

    static void registerMetaType();

    friend QDebug operator<<(QDebug argument, const CaLunarMonthInfo &what);
    friend QDBusArgument &operator<<(QDBusArgument &argument, const CaLunarMonthInfo &what);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument, CaLunarMonthInfo &what);

public:
    qint32 mFirstDayWeek;
    qint32 mDays;
    QList<CaLunarDayInfo> mCaLunarDayInfo;
};

class CaHuangLiDayInfo
{
public:
    CaHuangLiDayInfo() {}
    static void registerMetaType();
    friend QDebug operator<<(QDebug argument, const CaHuangLiDayInfo &what);
    friend QDBusArgument &operator<<(QDBusArgument &argument, const CaHuangLiDayInfo &what);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument, CaHuangLiDayInfo &what);

public:
    QString mGanZhiYear;
    QString mGanZhiMonth;
    QString mGanZhiDay;
    QString mLunarMonthName;
    QString mLunarDayName;
    qint32 mLunarLeapMonth;
    QString mZodiac;
    QString mTerm;
    QString mSolarFestival;
    QString mLunarFestival;
    qint32 mWorktime;
    QString mSuit;
    QString mAvoid;
};

class CaHuangLiMonthInfo
{
public:
    CaHuangLiMonthInfo() {}
    static void registerMetaType();

    friend QDebug operator<<(QDebug argument, const CaHuangLiMonthInfo &what);
    friend QDBusArgument &operator<<(QDBusArgument &argument, const CaHuangLiMonthInfo &what);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument, CaHuangLiMonthInfo &what);

public:
    qint32 mFirstDayWeek;
    qint32 mDays;
    QList<CaHuangLiDayInfo> mCaLunarDayInfo;
};

Q_DECLARE_METATYPE(CaLunarDayInfo)
Q_DECLARE_METATYPE(CaLunarMonthInfo)
Q_DECLARE_METATYPE(CaHuangLiDayInfo)
Q_DECLARE_METATYPE(CaHuangLiMonthInfo)

#endif // COMMONDATASTRUCT_H
