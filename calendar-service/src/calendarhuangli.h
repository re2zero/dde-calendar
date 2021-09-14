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
#ifndef CALENDARHUANGLI_H
#define CALENDARHUANGLI_H

#include "dbmanager/huanglidatabase.h"

#include <QObject>

class HuangLiDataBase;

class CalendarHuangLi : public QObject
{
    Q_OBJECT
public:
    explicit CalendarHuangLi(QObject *parent = nullptr);

    QString GetFestivalMonth(quint32 year, quint32 month);
    QString GetHuangLiDay(quint32 year, quint32 month, quint32 day);
    QString GetHuangLiMonth(quint32 year, quint32 month, bool fill);
    CaLunarDayInfo GetLunarInfoBySolar(quint32 year, quint32 month, quint32 day);
    CaLunarMonthInfo GetLunarCalendarMonth(quint32 year, quint32 month, bool fill);

private:
signals:

public slots:

private:
    HuangLiDataBase *m_database;
};

#endif // CALENDARHUANGLI_H
