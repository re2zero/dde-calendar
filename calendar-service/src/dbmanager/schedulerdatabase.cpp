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
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QSqlQuery>

SchedulerDatabase::SchedulerDatabase(QObject *parent)
    : QObject(parent)
{
    QString dbpath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append("/.config/deepin/dde-daemon/calendar/scheduler.db");
    // 重复调用QSQLITE会导致数据库连接覆盖导致失败，需指定每部分的连接名称
    m_database = QSqlDatabase::addDatabase("QSQLITE", "SchedulerDatabase");
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

//通过id获取日程信息
QString SchedulerDatabase::GetJob(qint64 id)
{
    QString strjson;
    QSqlQuery query(m_database);
    QString strsql = QString("SELECT id, type, title, description, "
                             "all_day, start, end, r_rule, remind, ignore"
                             " FROM jobs WHERE id = '%1';")
                         .arg(id);
    //id唯一因此此处最多只有一条数据
    if (query.exec(strsql) && query.next()) {
        QJsonDocument doc;
        QJsonObject obj;
        obj.insert("ID", query.value("id").toInt());
        obj.insert("Type", query.value("type").toInt());
        obj.insert("Title", query.value("title").toString());
        obj.insert("Description", query.value("description").toString());
        obj.insert("AllDay", query.value("all_day").toBool());
        obj.insert("Start", query.value("start").toString());
        obj.insert("End", query.value("end").toString());
        obj.insert("RRule", query.value("r_rule").toString());
        obj.insert("Remind", query.value("remind").toString());
        obj.insert("Ignore", query.value("ignore").toString());
        //RecurID数据库不包含该字段，当前先默认处理
        obj.insert("RecurID", 0);

        doc.setObject(obj);
        strjson = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
    } else {
        qDebug() << query.lastError();
    }
    return strjson;
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

// 执行删除日程的数据库SQL命令，以ID为依据
void SchedulerDatabase::DeleteJob(qint64 id)
{
    QString strsql = QString("DELETE FROM jobs WHERE id = %1").arg(id);
    QSqlQuery query(m_database);
    if (!query.exec(strsql)) {
        qDebug() << __FUNCTION__ << query.lastError();
    }
    if (query.isActive()) {
        query.finish();
    }
    m_database.commit();
}

// 执行添加日程的数据库SQL命令，并返回其ID值
qint64 SchedulerDatabase::CreateJob(const Job &job)
{
    QDateTime currentDateTime =QDateTime::currentDateTime();

    QSqlQuery query(m_database);
    QString strsql = "INSERT INTO jobs (created_at, updated_at, type, title,"
                     "description, all_day, start, end, r_rule, remind, ignore, title_pinyin)"
                     "values (:created_at, :updated_at, :type, :title, :description,"
                     ":all_day, :start, :end, :r_rule, :remind, :ignore, :title_pinyin)";
    query.prepare(strsql);
    query.bindValue(0,currentDateTime.toString("yyyy-MM-dd hh:mm:ss.zzz"));
    query.bindValue(1,currentDateTime.toString("yyyy-MM-dd hh:mm:ss.zzz"));
    query.bindValue(2,job.Type);
    query.bindValue(3,job.Title);
    query.bindValue(4,job.Description);
    query.bindValue(5,job.AllDay);
    query.bindValue(6,job.Start);
    query.bindValue(7,job.End);
    query.bindValue(8,job.RRule);
    query.bindValue(9,job.Remind);
    query.bindValue(10,job.Ignore);
    query.bindValue(11,job.Title_pinyin);
    if (!query.exec()) {
        qDebug() << __FUNCTION__ << query.lastError();
    }
    if (query.isActive()) {
        query.finish();
    }

    // 获取最新刚插入日程的ID。由于id为数据库自增，因此插入的日程id一直为最大值。
    qint64 jobID;
    QString returnIdsql = "SELECT MAX(id) FROM jobs";
    if (!query.exec(returnIdsql)) {
        qDebug() << __FUNCTION__ << query.lastError();
    }
    if (query.next()) {
        jobID = query.value(0).toInt();
    } else {
        qDebug() << __FUNCTION__ << query.lastError();
        return -1;
    }
    if (query.isActive()) {
        query.finish();
    }
    // 共有两次sql语句执行，commit操作需要置于最后
    m_database.commit();
    return jobID;
}
