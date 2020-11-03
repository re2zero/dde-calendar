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
#include "schedulerdatabase.h"
#include "src/commondef.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

SchedulerDatabase::SchedulerDatabase(QObject *parent)
    : QObject(parent)
{
    QString dbpath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append("/.config/deepin/dde-daemon/calendar/scheduler.db");
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName(dbpath);
    m_database.open();
    if (m_database.isOpen()) {
        const QStringList tables = m_database.tables();
        if (tables.size() < 1) {
            CreateTables();
        }
    } else {
        qDebug() << __FUNCTION__ << m_database.lastError();
    }
}

void SchedulerDatabase::CreateTables()
{
    QSqlQuery query(m_database);
    //table job_types
    qDebug() << query.exec("CREATE TABLE \"job_types\" (\"id\" integer primary key autoincrement,\"created_at\""
                           " datetime,\"updated_at\" datetime,\"deleted_at\" datetime,\"name\" varchar(255),\"color\" varchar(255) )")
             << query.lastError();
    qDebug() << query.exec("CREATE INDEX idx_job_types_deleted_at ON \"job_types\"(deleted_at)") << query.lastError();

    //table jobs
    qDebug() << query.exec("CREATE TABLE \"jobs\" (\"id\" integer primary key autoincrement,"
                           "\"created_at\" datetime,\"updated_at\" datetime,\"deleted_at\" datetime,"
                           "\"type\" integer,\"title\" varchar(255),\"description\" varchar(255),"
                           "\"all_day\" bool,\"start\" datetime,\"end\" datetime,\"r_rule\" varchar(255),"
                           "\"remind\" varchar(255),\"ignore\" varchar(255) , \"title_pinyin\" varchar(255))")
             << query.lastError();
    qDebug() << query.exec("CREATE INDEX idx_jobs_deleted_at ON \"jobs\"(deleted_at)") << query.lastError();
    if (query.isActive()) {
        query.finish();
    }
    m_database.commit();
}
