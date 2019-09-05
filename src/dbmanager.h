/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     kirigaya <kirigaya@mkacg.com>
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
#ifndef SCHEDULEDBMANAGER_H
#define SCHEDULEDBMANAGER_H
#include "schedulestructs.h"
#include <QString>
#include <QSqlDatabase>

class ScheduleDbManager
{
public:
    static int addSchedule(const ScheduleInfo &scheduledate);
    static QVector<ScheduleInfo> getAllScheduleInfo();
    static ScheduleInfo getScheduleInfoById(int id);
    //type  0 day 1 week 2 month 3 year
    static QVector<ScheduleInfo> getScheduleInfo(const QDate date, int type);
    static QVector<ScheduleInfo> getALLDayScheduleInfo(const QDate date);

    static QVector<ScheduleRemindInfo> getScheduleRemind(const QDate date);
    static QColor getTypeColor(int type);
    static bool updateScheduleInfo(const ScheduleInfo &scheduledate);
    static bool deleteScheduleInfoById(int id);
    static int initDataBase();
    static void close();
private:
    ScheduleDbManager();
private:
    static QString          m_dbName;
    static QString          m_dbPath;
    static QSqlDatabase     m_dbSqlDatabase;
    static bool             m_initFlag;
};


#endif // DBMANAGER_H
