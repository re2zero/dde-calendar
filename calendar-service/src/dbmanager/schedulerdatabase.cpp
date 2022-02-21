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
#include <QFile>
#include <QDir>
#include <QTimeZone>

SchedulerDatabase::SchedulerDatabase(QObject *parent)
    : QObject(parent)
{
    //旧文件路径
    QString oldDbPatch = QStandardPaths::writableLocation(QStandardPaths::HomeLocation).append("/.config/deepin/dde-daemon/calendar");
    QDir dir;
    //如果该路径不存在，则创建该文件夹
    if (!dir.exists(oldDbPatch)) {
        dir.mkpath(oldDbPatch);
    }
    QString dbpath = oldDbPatch.append("/scheduler.db");
    OpenSchedulerDatabase(dbpath);
}

//通过id获取日程信息
QString SchedulerDatabase::GetJob(qint64 id)
{
    QString strjson;
    QSqlQuery query(m_database);
    QString strsql = QString("SELECT id, type, title, description, "
                             "all_day, start, end, r_rule, remind, ignore"
                             " FROM jobs WHERE id = '%1' ")
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
        //调整时间格式，方便前端解析
        obj.insert("Start", Utils::toconvertData(query.value("start").toDateTime()));
        obj.insert("End", Utils::toconvertData(query.value("end").toDateTime()));
        obj.insert("RRule", query.value("r_rule").toString());
        obj.insert("Remind", query.value("remind").toString());
        //将QString类型转换为QJsonArray类型，方便前端解析
        obj.insert("Ignore", QJsonDocument::fromJson(query.value("ignore").toString().toUtf8()).array());
        //数据库包含的都是原始数据所以RecurID默认为0
        obj.insert("RecurID", 0);

        doc.setObject(obj);
        strjson = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
    } else {
        qDebug() << __FUNCTION__ << query.lastError();
    }
    if (query.isActive()) {
        query.finish();
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

    QString strsql = QString("select id,type,title,description,all_day,start,end,r_rule,remind,ignore,title_pinyin "
                             " from jobs ");
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
    if (query.isActive()) {
        query.finish();
    }

    return jobs;
}

/**
 * @brief  GetAllOriginJobs 获取所有与key相关的job原始数据
 * @param  key 搜索词
 * @param  strsort 查询排序条件
 * @return 返回所有原始日程集合
 */
QList<Job> SchedulerDatabase::GetAllOriginJobs(const QString &key, const QString &strsort)
{
    QList<Job> jobs;
    QSqlQuery query(m_database);
    QString strKey = key.trimmed();
    pinyinsearch *psearch = pinyinsearch::getPinPinSearch();
    QString strsql;
    if (psearch->CanQueryByPinyin(strKey)) {
        //可以按照拼音查询
        QString pinyin = psearch->CreatePinyinQuery(strKey.toLower());
        strsql = QString("select id,type,title,description,all_day,start,end,r_rule,remind,ignore,title_pinyin "
                         " from jobs where instr(UPPER(title), UPPER('%1')) OR title_pinyin LIKE '%2'").arg(key).arg(pinyin);
    } else if (!key.isEmpty()) {
        //按照key查询
        strsql = QString("select id,type,title,description,all_day,start,end,r_rule,remind,ignore,title_pinyin "
                         " from jobs where instr(UPPER(title), UPPER('%1'))").arg(key);
    } else {
        //如果没有key，则搜索所有
        strsql = QString("select id,type,title,description,all_day,start,end,r_rule,remind,ignore,title_pinyin "
                         " from jobs ");
    }

    //排序条件不为空
    if (!strsort.isEmpty()) {
        strsql.append(QString("order by %1").arg(strsort));
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
    if (query.isActive()) {
        query.finish();
    }

    return jobs;
}

QList<Job> SchedulerDatabase::GetAllOriginJobsWithRule(const QString &key, const QString &rules)
{
    QList<Job> jobs;
    QSqlQuery query(m_database);
    QString strKey = key.trimmed();
    QString strrule;
    if (!rules.contains("BYDAY") && rules.contains("DAILY")) {
        // 每日
        strrule = QString("r_rule LIKE '%%1%' AND r_rule NOT LIKE '%BYDAY%' ").arg(rules);
    } else {
        // 工作日 每周 每月 每年
        strrule = QString("r_rule LIKE '%%1%'").arg(rules);
    }

    pinyinsearch *psearch = pinyinsearch::getPinPinSearch();
    QString strsql;
    if (psearch->CanQueryByPinyin(strKey)) {
        //可以按照拼音查询
        QString pinyin = psearch->CreatePinyinQuery(strKey.toLower());
        strsql = QString("select id from jobs where title like '%%1%' or title_pinyin like '%%2%' ").arg(key).arg(pinyin);
    } else if (!key.isEmpty()) {
        //按照key查询
        strsql = QString("select id from jobs where title like '%%1%' ").arg(key);
    } else {
        //如果没有key，则搜索所有
        strsql = QString("select id from jobs ");
    }

    strsql = QString("select id,type,title,description,all_day,start,end,r_rule,remind,ignore,title_pinyin "
                     " from jobs where id in(%1) and %2").arg(strsql).arg(strrule);

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
    if (query.isActive()) {
        query.finish();
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
    QString strSql("select id,type,title,description,all_day,start,end,r_rule,remind,ignore,title_pinyin "
                   "from jobs where remind is not null and remind !=' ' ");
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
    if (query.isActive()) {
        query.finish();
    }
    return jobs;
}

//存储提醒日程的相关信息
void SchedulerDatabase::saveRemindJob(const Job &job)
{
    QSqlQuery query(m_database);
    QString strsql = "INSERT INTO jobsReminder (jobid,recurid,remindCount,notifyid, remindTime,jobStartTime,jobEndTime)"
                     "values (:jobid,:recurid,:remindCount,:notifyid,:remindTime,:jobStartTime,:jobEndTime)";
    query.prepare(strsql);
    int i = 0;
    query.bindValue(i, job.ID);
    query.bindValue(++i, job.RecurID);
    query.bindValue(++i, job.RemindLaterCount);
    //通知提醒id默认为-1,表示未提醒
    query.bindValue(++i, -1);
    query.bindValue(++i, job.RemidTime);
    query.bindValue(++i, job.Start);
    query.bindValue(++i, job.End);
    if (query.exec()) {
        if (query.isActive()) {
            query.finish();
        }
    } else {
        qDebug() << __FUNCTION__ << query.lastError();
    }
}

void SchedulerDatabase::updateRemindJob(const Job &job)
{
    //点击稍后提醒后，更新信息并设置通知提醒为-1
    QString strsql = QString("UPDATE jobsReminder SET remindCount = '%1' , remindTime = '%2' WHERE jobid = %3 and recurid = %4 and notifyid = -1")
        .arg(job.RemindLaterCount)
        .arg(dateTimeToString(job.RemidTime))
        .arg(job.ID)
        .arg(job.RecurID);
    QSqlQuery query(m_database);
    if (query.exec(strsql)) {
        if (query.isActive()) {
            query.finish();
        }
        m_database.commit();
    } else {
        qWarning() << __FUNCTION__ << query.lastError();
    }
}

void SchedulerDatabase::deleteRemindJobs(const QList<qlonglong> &Ids)
{
    if (Ids.size() ==0)
        return;
    QStringList idList;
    for (int i = 0 ; i < Ids.size();++i) {
        idList.append(QString::number(Ids.at(i)));
    }
    QSqlQuery query(m_database);
    QString sql = QString("delete from jobsReminder where  jobsReminder.jobid in ( %1)").arg(idList.join(","));
    if (query.exec(sql)) {
        if (query.isActive()) {
            query.finish();
        }
    } else {
        qWarning() << __FUNCTION__ << query.lastError();
    }
}

void SchedulerDatabase::deleteRemindJobs(const qlonglong &jobID, const qint64 recurid)
{
    QSqlQuery query(m_database);
    QString sql = QString("delete from jobsReminder where  jobsReminder.jobid = %1 and jobsReminder.recurid = %2")
                      .arg(jobID).arg(recurid);
    if (query.exec(sql)) {
        if (query.isActive()) {
            query.finish();
        }
    } else {
        qWarning() << __FUNCTION__ << query.lastError();
    }
}

QList<Job> SchedulerDatabase::getValidRemindJob()
{
    QList<Job> jobs{};
    QSqlQuery query(m_database);
    QString sql("select jobs.id, jobs.all_day,jobs.type,jobs.title,jobs.description,jobsReminder.jobStartTime as start,"
                "jobsReminder.jobEndTime as end,jobs.r_rule,jobs.remind,jobs.ignore,jobs.title_pinyin,jobsReminder.remindCount,"
                "jobsReminder.remindTime , jobsReminder.recurid from jobs left join jobsReminder on jobs.id = jobsReminder.jobid "
                "where jobsReminder.remindCount > 0 and jobsReminder.remindTime > ");
    sql +=QString(" '%1'").arg(dateTimeToString(QDateTime::currentDateTime()));
    if (query.exec(sql)) {
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
            jb.RemindLaterCount = query.value("remindCount").toInt();
            jb.RemidTime = query.value("remindTime").toDateTime();
            jb.RecurID = query.value("recurid").toInt();
            jobs.append(jb);
        }
    }
    if (query.isActive()) {
        query.finish();
    }
    return jobs;
}

void SchedulerDatabase::clearRemindJobDatabase()
{
    QSqlQuery query(m_database);
    QString sql("delete from jobsReminder");
    if (query.exec(sql)) {
        if (query.isActive()) {
            query.finish();
        }
    } else {
        qWarning() << __FUNCTION__ << query.lastError();
    }

}

Job SchedulerDatabase::getRemindJob(const qint64 id, const qint64 recurid)
{
    QSqlQuery query(m_database);
    QString sql = QString("select jobs.id, jobs.all_day,jobs.type,jobs.title,jobs.description,"
                          "jobsReminder.jobStartTime as start,jobsReminder.jobEndTime as end,jobs.r_rule,jobs.remind,jobs.ignore,jobs.title_pinyin,"
                          "jobsReminder.remindCount,jobsReminder.remindTime , jobsReminder.recurid from jobs inner join jobsReminder "
                          "on jobs.id = jobsReminder.jobid   where jobsReminder.jobid = %1 and jobsReminder.recurid = %2")
                  .arg(id).arg(recurid);

    //id唯一因此此处最多只有一条数据
    Job jb;
    if (query.exec(sql) && query.next()) {
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
        jb.RemindLaterCount = query.value("remindCount").toInt();
        jb.RemidTime = query.value("remindTime").toDateTime();
        jb.RecurID = query.value("recurid").toInt();
    } else {
        qWarning() << query.lastError();
    }
    if (query.isActive()) {
        query.finish();
    }
    return jb;
}

QList<Job> SchedulerDatabase::getRemindJob(const qint64 id)
{
    QSqlQuery query(m_database);
    QString sql = QString("select jobs.id, jobs.all_day,jobs.type,jobs.title,jobs.description,"
                          "jobsReminder.jobStartTime as start,jobsReminder.jobEndTime as end,jobs.r_rule,jobs.remind,jobs.ignore,jobs.title_pinyin,"
                          "jobsReminder.remindCount,jobsReminder.remindTime , jobsReminder.recurid from jobs inner join jobsReminder "
                          "on jobs.id = jobsReminder.jobid   where jobsReminder.jobid = %1")
                  .arg(id);

    //id唯一因此此处最多只有一条数据
    QList<Job> jbList;
    if (query.exec(sql) && query.next()) {
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
        jb.RemindLaterCount = query.value("remindCount").toInt();
        jb.RemidTime = query.value("remindTime").toDateTime();
        jb.RecurID = query.value("recurid").toInt();
        jbList.append(jb);
    } else {
        qWarning() << query.lastError();
    }
    if (query.isActive()) {
        query.finish();
    }
    return jbList;
}

//获取桌面顶部通知ID
QVector<int> SchedulerDatabase::getNotifyID(const qint64 id)
{
    QVector<int> notifyid;
    QSqlQuery query(m_database);
    QString sql("select distinct jobsReminder.notifyid from jobsReminder where jobsReminder.jobid = ");
    sql += QString::number(id);
    if (query.exec(sql) && query.next()) {
        notifyid.append(query.value("notifyid").toInt());
    }
    if (query.isActive()) {
        query.finish();
    }
    return notifyid;
}

int SchedulerDatabase::getNotifyID(const qint64 jobID, const qint64 recurid)
{
    int notifyid = -1;
    QSqlQuery query(m_database);
    QString sql = QString("select distinct jobsReminder.notifyid from jobsReminder where jobsReminder.jobid = %1 and jobsReminder.recurid = %2")
        .arg(jobID).arg(recurid);
    if (query.exec(sql) && query.next()) {
        notifyid = query.value("notifyid").toInt();
    }
    if (query.isActive()) {
        query.finish();
    }
    return notifyid;
}

//更新桌面顶部通知ID
void SchedulerDatabase::updateNotifyID(const Job &job, int notifyid)
{
    QString strsql =
        QString("UPDATE jobsReminder SET notifyid = '%1'  WHERE jobid = %2 and recurid = %3")
        .arg(notifyid)
        .arg(job.ID)
        .arg(job.RecurID);
    QSqlQuery query(m_database);
    if (query.exec(strsql)) {
        if (query.isActive()) {
            query.finish();
        }
        m_database.commit();
    } else {
        qWarning() << __FUNCTION__ << query.lastError();
    }
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

void SchedulerDatabase::OpenSchedulerDatabase(const QString &dbpath)
{
    // 重复调用QSQLITE会导致数据库连接覆盖导致失败，需指定每部分的连接名称
    m_database = QSqlDatabase::addDatabase("QSQLITE", "SchedulerDatabase");
    m_database.setDatabaseName(dbpath);
    //这里用QFile来修改日历数据库文件的权限
    QFile file(dbpath);
    //如果不存在该文件则创建
    if (!file.exists()) {
        m_database.open();
        m_database.close();
    }
    //将权限修改为600（对文件的所有者可以读写，其他用户不可读不可写）
    if (!file.setPermissions(QFile::WriteOwner | QFile::ReadOwner)) {
        qWarning() << "权限设置失败，错误:" << file.errorString();
    }
    if (m_database.open()) {
        const QStringList tables = m_database.tables();
        if (tables.size() < 1) {
            CreateTables();
        }
        //如果不存在日程提醒相关的数据库则创建一个（因为需要将程序改成按需运行的程序，所以需要存储相关数据）
        if(!tables.contains("jobsReminder")){
            QSqlQuery query(m_database);
            query.exec("CREATE TABLE \"jobsReminder\" (\"id\" integer primary key autoincrement,"
                       "\"jobid\" integer,\"recurid\" integer,\"remindCount\" integer ,\"notifyid\" integer ,"
                       "\"remindTime\" datetime ,\"jobStartTime\" datetime ,\"jobEndTime\" datetime) ");
            if (query.isActive()) {
                query.finish();
            }
            m_database.commit();
        }
    } else {
        qDebug() << __FUNCTION__ << m_database.lastError();
    }
}

QString SchedulerDatabase::dateTimeToString(const QDateTime &dateTime)
{
    QTime _offsetTime = QTime(0, 0).addSecs(dateTime.timeZone().offsetFromUtc(dateTime));
    return QString("%1.000+%2").arg(dateTime.toString("yyyy-MM-ddThh:mm:ss")).arg(_offsetTime.toString("hh:mm"));
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
    currentDateTime.setOffsetFromUtc(currentDateTime.offsetFromUtc());
    QSqlQuery query(m_database);
    QString strsql = "UPDATE jobs SET updated_at = ?, type = ?, title = ?, "
                     "description = ?, all_day = ?, start = ?, end = ?, r_rule = ?, "
                     "remind = ?, ignore = ?, title_pinyin = ? WHERE id = ?";
    query.prepare(strsql);
    qint64 id = rootObj.value("ID").toInt();
    int i = 0;
    //修改updatetime时间格式
    query.bindValue(i, currentDateTime);
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
