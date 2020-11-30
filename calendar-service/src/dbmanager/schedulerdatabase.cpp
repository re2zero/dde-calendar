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
#include "pinyin/pinyinsearch.h"
#include "src/utils.h"

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
        //数据库包含的都是原始数据所以RecurID默认为0
        obj.insert("RecurID", 0);

        doc.setObject(obj);
        strjson = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
    } else {
        qDebug() << query.lastError();
    }
    return strjson;
}

/**
 * @brief  GetAllOriginJobs 获取所有原始日程
 * @return 返回所有原始日程集合
 */
QList<Job> SchedulerDatabase::GetAllOriginJobs()
{
    QList<Job> jobs;
    QSqlQuery query(m_database);

    QString strsql = QString("select * from jobs;");
    if (query.exec(strsql)) {
        while (query.next()) {
            Job jb;
            jb.ID = query.value("id").toInt();
            jb.Type = query.value("type").toInt();
            jb.Title = query.value("title").toString();
            jb.Description = query.value("description").toString();
            jb.AllDay = query.value("all_day").toBool();
            jb.Start = query.value("start").toDateTime();
            jb.End = query.value("end").toDateTime();
            jb.RRule = query.value("r_rule").toString();
            jb.Remind = query.value("remind").toString();
            jb.Ignore = query.value("ignore").toString();
            jb.Title_pinyin = query.value("title_pinyin").toString();
            jobs.append(jb);
        }
    }

    return jobs;
}

/**
 * @brief  GetAllOriginJobs 获取所有与key相关的job原始数据
 * @param  key 搜索词
 * @return 返回所有原始日程集合
 */
QList<Job> SchedulerDatabase::GetAllOriginJobs(const QString &key)
{
    QList<Job> jobs;
    QSqlQuery query(m_database);
    QString strKey = key.trimmed();
    pinyinsearch *psearch = pinyinsearch::getPinPinSearch();
    QString strsql;
    if (psearch->CanQueryByPinyin(strKey)) {
        //可以按照拼音查询
        QString pinyin = psearch->CreatePinyinQuery(strKey.toLower());
        strsql = QString("select * from jobs where title like '%%1%' or title_pinyin like '%%2%';").arg(key).arg(pinyin);
    } else if (!key.isEmpty()) {
        //按照key查询
        strsql = QString("select * from jobs where title like '%%1%';").arg(key);
    } else {
        //如果没有key，则搜索所有
        strsql = QString("select * from jobs;");
    }

    if (query.exec(strsql)) {
        while (query.next()) {
            Job jb;
            jb.ID = query.value("id").toInt();
            jb.Type = query.value("type").toInt();
            jb.Title = query.value("title").toString();
            jb.Description = query.value("description").toString();
            jb.AllDay = query.value("all_day").toBool();
            jb.Start = query.value("start").toDateTime();
            jb.End = query.value("end").toDateTime();
            jb.RRule = query.value("r_rule").toString();
            jb.Remind = query.value("remind").toString();
            jb.Ignore = query.value("ignore").toString();
            jb.Title_pinyin = query.value("title_pinyin").toString();
            jobs.append(jb);
        }
    }

    return jobs;
}

/**
 * @brief  GetJobsContainRemind 获取包含提醒规则的jos集合
 */
QList<Job> SchedulerDatabase::GetJobsContainRemind()
{
    QList<Job> jobs;
    QSqlQuery query(m_database);
    QString strSql("select * from jobs where remind is not null and remind !=' ';");
    if (query.exec(strSql)) {
        while (query.next()) {
            Job jb;
            jb.ID = query.value("id").toInt();
            jb.Type = query.value("type").toInt();
            jb.Title = query.value("title").toString();
            jb.Description = query.value("description").toString();
            jb.AllDay = query.value("all_day").toBool();
            jb.Start = query.value("start").toDateTime();
            jb.End = query.value("end").toDateTime();
            jb.RRule = query.value("r_rule").toString();
            jb.Remind = query.value("remind").toString();
            jb.Ignore = query.value("ignore").toString();
            jb.Title_pinyin = query.value("title_pinyin").toString();
            jobs.append(jb);
        }
    }
    return jobs;
}

/**
 * @brief  CreateTables 创建日程相关数据表（新用户创建）
 */
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
    if (query.exec(strsql)) {
        if (query.isActive()) {
            query.finish();
        }
        m_database.commit();
    } else {
        qDebug() << __FUNCTION__ << query.lastError();
    }
}

// 执行删除日程类型的数据库SQL命令，以ID为依据
void SchedulerDatabase::DeleteType(qint64 id)
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString strCurTime = currentDateTime.toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString strsql = QString("UPDATE job_types SET deleted_at = '%1' WHERE id = %2").arg(strCurTime).arg(id);
    QSqlQuery query(m_database);
    if (query.exec(strsql)) {
        if (query.isActive()) {
            query.finish();
        }
        m_database.commit();
    } else {
        qDebug() << __FUNCTION__ << query.lastError();
    }
}

// 执行添加日程的数据库SQL命令，并返回其ID值
qint64 SchedulerDatabase::CreateJob(const Job &job)
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    currentDateTime.setOffsetFromUtc(currentDateTime.offsetFromUtc());
    QSqlQuery query(m_database);
    QString strsql = "INSERT INTO jobs (created_at, updated_at, type, title,"
                     "description, all_day, start, end, r_rule, remind, ignore, title_pinyin)"
                     "values (:created_at, :updated_at, :type, :title, :description,"
                     ":all_day, :start, :end, :r_rule, :remind, :ignore, :title_pinyin)";
    query.prepare(strsql);
    int i = 0;
    query.bindValue(i, currentDateTime);
    query.bindValue(++i, currentDateTime);
    query.bindValue(++i, job.Type);
    query.bindValue(++i, job.Title);
    query.bindValue(++i, job.Description);
    query.bindValue(++i, job.AllDay);
    //修改开始结束时间格式，与前端保持一致
    query.bindValue(++i, Utils::toconvertData(job.Start));
    query.bindValue(++i, Utils::toconvertData(job.End));
    query.bindValue(++i, job.RRule);
    query.bindValue(++i, job.Remind);
    query.bindValue(++i, job.Ignore);
    query.bindValue(++i, job.Title_pinyin);
    if (query.exec()) {
        if (query.isActive()) {
            query.finish();
        }
    } else {
        qDebug() << __FUNCTION__ << query.lastError();
    }

    // 获取最新刚插入日程的ID。由于id为数据库自增，因此插入的日程id一直为最大值。
    qint64 jobID;
    QString returnIdsql = "SELECT MAX(id) FROM jobs";
    if (query.exec(returnIdsql) && query.next()) {
        jobID = query.value(0).toInt();
        if (query.isActive()) {
            query.finish();
        }
        // 共有两次sql语句执行，commit操作需要置于最后
        m_database.commit();
    } else {
        qDebug() << __FUNCTION__ << query.lastError();
        return -1;
    }
    return jobID;
}

// 根据传入的jobInfo中的Id来更新数据库中相应的数据
qint64 SchedulerDatabase::UpdateJob(const QString &jobInfo)
{
    // TODO: 对job数据进行合法性检测
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jobInfo.toLocal8Bit(), &json_error));
    if (json_error.error != QJsonParseError::NoError) {
        return -1;
    }
    QJsonObject rootObj = jsonDoc.object();
    // 此处Ignore参数需要单独解析，后续pinyin参数也可能会单独解析
    QJsonArray subArray = rootObj.value("Ignore").toArray();
    QJsonDocument doc;
    doc.setArray(subArray);

    QDateTime currentDateTime = QDateTime::currentDateTime();
    QSqlQuery query(m_database);
    QString strsql = "UPDATE jobs SET updated_at = ?, type = ?, title = ?, "
                     "description = ?, all_day = ?, start = ?, end = ?, r_rule = ?, "
                     "remind = ?, ignore = ?, title_pinyin = ? WHERE id = ?";
    query.prepare(strsql);
    qint64 id = rootObj.value("ID").toInt();
    int i = 0;
    query.bindValue(i, currentDateTime.toString("yyyy-MM-dd hh:mm:ss.zzz"));
    query.bindValue(++i, rootObj.value("Type").toInt());
    query.bindValue(++i, rootObj.value("Title").toString());
    query.bindValue(++i, rootObj.value("Description").toString());
    query.bindValue(++i, rootObj.value("AllDay").toBool());
    query.bindValue(++i, rootObj.value("Start").toString());
    query.bindValue(++i, rootObj.value("End").toString());
    query.bindValue(++i, rootObj.value("RRule").toString());
    query.bindValue(++i, rootObj.value("Remind").toString());
    query.bindValue(++i, QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
    query.bindValue(++i, pinyinsearch::getPinPinSearch()->CreatePinyin(rootObj.value("Title").toString()));
    query.bindValue(++i, id);
    if (query.exec()) {
        if (query.isActive()) {
            query.finish();
        }
        m_database.commit();
    } else {
        qDebug() << __FUNCTION__ << query.lastError();
    }

    return id;
}

bool SchedulerDatabase::UpdateJobIgnore(const QString &strignore, qint64 id)
{
    QSqlQuery query(m_database);
    QString strsql = QString("UPDATE jobs SET ignore='%1' where id=%2;").arg(strignore).arg(id);
    bool bsuccess = false;
    if (query.exec(strsql)) {
        if (query.isActive()) {
            query.finish();
        }
        bsuccess = m_database.commit();
    }

    return bsuccess;
}

// 根据传入的typeInfo中的Id来更新数据库中相应的数据
void SchedulerDatabase::UpdateType(const QString &typeInfo)
{
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(typeInfo.toLocal8Bit(), &json_error));
    if (json_error.error != QJsonParseError::NoError) {
        return ;
    }
    QJsonObject rootObj = jsonDoc.object();

    QSqlQuery query(m_database);
    QString strsql = "UPDATE job_types SET updated_at = ?, name = ?, color = ? WHERE id = ?";
    query.prepare(strsql);
    QDateTime currentDateTime = QDateTime::currentDateTime();
    int i = 0;
    query.bindValue(i, currentDateTime.toString("yyyy-MM-dd hh:mm:ss.zzz"));
    query.bindValue(++i, rootObj.value("Name").toString());
    query.bindValue(++i, rootObj.value("Color").toString());
    query.bindValue(++i, rootObj.value("ID").toInt());
    if (query.exec()) {
        if (query.isActive()) {
            query.finish();
        }
        m_database.commit();
    } else {
        qDebug() << __FUNCTION__ << query.lastError();
    }
}
