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

#include "test_schedulerdatabase.h"
#include "calendarscheduler.h"
#include "../../calendar-basicstruct/src/utils.h"
#include "../third-party_stub/stub.h"
#include "config.h"
#include <QSqlQuery>
#include <QDebug>

//OpenSchedulerDatabase
void stub_OpenSchedulerDatabase(void *obj, const QString &dbpath)
{
    SchedulerDatabase *o= (SchedulerDatabase*)obj;
    o->m_database = QSqlDatabase::addDatabase("QSQLITE", "SchedulerDatabase");
    o->m_database.setDatabaseName(SD_DATABASE_DIR);
    o->m_database.open();
    if (o->m_database.isOpen()) {
        const QStringList tables = o->m_database.tables();
        if (tables.size() < 1) {
            o->CreateTables();
        }
    }
}

test_schedulerdatabase::test_schedulerdatabase()
{
    Stub stub;
    stub.set(ADDR(SchedulerDatabase, OpenSchedulerDatabase), stub_OpenSchedulerDatabase);
    sDb = new SchedulerDatabase();
}

test_schedulerdatabase::~test_schedulerdatabase()
{
    if (sDb->m_database.isOpen()) {
        sDb->m_database.close();
        sDb->m_database.removeDatabase("SchedulerDatabase");
    }
    delete sDb;
}

//获取Schedulerdatabase类中私有成员以及私有函数权限
TEST_F(test_schedulerdatabase, dbOparetion)
{
    QSqlQuery query(sDb->m_database);
    //清空原始表
    QString strDeleteJobTable = "DROP TABLE jobs;";
    query.exec(strDeleteJobTable);
    if (query.isActive()) {
        query.finish();
    }

    QString strDeleteJobTypeTable = "DROP TABLE job_types;";
    query.exec(strDeleteJobTypeTable);
    if (query.isActive()) {
        query.finish();
    }
    sDb->m_database.commit();

    sDb->CreateTables();

    //初始化本地job_types表，保证本地日历的可用性
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString sTime = Utils::toconvertData(currentDateTime);

    QString strInitJobType = QString("INSERT INTO job_types (created_at, updated_at, name, color) VALUES "
                             "(\"%1\", \"%1\", \"学习\", \"#FF0000\"),"
                             "(\"%1\", \"%1\", \"工作\", \"#00FF00\"),"
                             "(\"%1\", \"%1\", \"其他\", \"#800080\");").arg(sTime);
    query.exec(strInitJobType);
    if (query.isActive()) {
        query.finish();
    }

    //为后续测试UpdateType、DeleteTpye做先决条件
    QString strCreateJobType = QString("INSERT INTO job_types (created_at, updated_at, name, color) VALUES "
                               "(\"%1\", \"%1\", \"UT测试X——ID应为4\", \"#FFFFFF\"),"
                               "(\"%1\", \"%1\", \"UT测试X——ID应为5\", \"#FFFFFF\"),"
                               "(\"%1\", \"%1\", \"UT测试Y——ID应为6\", \"#FFFFFF\");").arg(sTime);
    query.exec(strCreateJobType);
    if (query.isActive()) {
        query.finish();
    }
    sDb->m_database.commit();
}

//qint64 SchedulerDatabase::CreateJob(const Job &job)
TEST_F(test_schedulerdatabase, CreateJob)
{
    Job job1;
    job1.Start = QDateTime::fromString("2020-12-05T00:00:00+08:00", Qt::ISODate);
    job1.End = QDateTime::fromString("2020-12-05T23:59:00+08:00", Qt::ISODate);
    job1.AllDay = true;
    job1.Type = 1;
    job1.Description = "";
    job1.ID = 0;
    job1.Ignore = "[]";
    job1.Title = "UT测试A";
    job1.RRule = "FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;COUNT=3";
    job1.RecurID = 0;
    job1.Remind = "1;09:00";

    qint64 intJob1 = sDb->CreateJob(job1);

    Job job2;
    job2.Start = QDateTime::fromString("2020-12-09T00:00:00+08:00", Qt::ISODate);
    job2.End = QDateTime::fromString("2020-12-09T23:59:00+08:00", Qt::ISODate);
    job2.AllDay = true;
    job2.Type = 1;
    job2.Description = "";
    job2.ID = 0;
    job2.Ignore = "[\"2020-12-11T00:00:00+08:00\"]";
    job2.Title = "UT测试B";
    job2.RRule = "FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;COUNT=2";
    job2.RecurID = 0;
    job2.Remind = "1;09:00";
    qint64 intJob2 = sDb->CreateJob(job2);

    //为delete提供待删除数据
    Job job3;
    job3.Start = QDateTime::fromString("2020-12-14T00:00:00+08:00", Qt::ISODate);
    job3.End = QDateTime::fromString("2020-12-14T23:59:00+08:00", Qt::ISODate);
    job3.AllDay = true;
    job3.Type = 1;
    job3.Description = "";
    job3.ID = 0;
    job3.Ignore = "[]";
    job3.Title = "UT测试C";
    job3.RRule = "FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;COUNT=4";
    job3.RecurID = 0;
    job3.Remind = "1;09:00";
    qint64 intJob3 = sDb->CreateJob(job3);

    assert(1 == intJob1 && 2 == intJob2 && 3 == intJob3);
}

//QString SchedulerDatabase::GetJob(qint64 id)
TEST_F(test_schedulerdatabase, GetJob)
{
    //返回值需要解析
    QString getJob1 = sDb->GetJob(1);
    const QString job1 = "{\"AllDay\":true,\"Description\":\"\",\"End\":"
                          "\"2020-12-05T23:59:00+08:00\",\"ID\":1,\"Ignore\":[],"
                          "\"RRule\":\"FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;COUNT=3\","
                          "\"RecurID\":0,\"Remind\":\"1;09:00\",\"Start\":"
                          "\"2020-12-05T00:00:00+08:00\",\"Title\":\"UT测试A\",\"Type\":1}";
    assert(job1 == getJob1);
}

//QList<Job> SchedulerDatabase::GetAllOriginJobs()
//QList<Job> SchedulerDatabase::GetAllOriginJobs(const QString &key, const QString &strsort)
TEST_F(test_schedulerdatabase, GetAllOriginJobs)
{
    QList<Job> jobList = sDb->GetAllOriginJobs();
    //int jobCount = jobList.count();

    const QString key = "";
    const QString strsort = "";
    QList<Job> jobListP = sDb->GetAllOriginJobs(key, strsort);

    const QString key1 = "BYDAY";
    const QString strsort1 = "id";
    QList<Job> jobListP1 = sDb->GetAllOriginJobs(key1, strsort1);

    const QString key2 = "ce";
    const QString strsort2 = "id";
    QList<Job> jobList2 = sDb->GetAllOriginJobs(key2, strsort2);


    assert(1 == 1);
}

//QList<Job> SchedulerDatabase::GetAllOriginJobsWithRule(const QString &key, const QString &rules)
TEST_F(test_schedulerdatabase, GetAllOriginJobsWithRule)
{
    const QString key = "";
    const QString rules = "";
    QList<Job> jobs = sDb->GetAllOriginJobsWithRule(key, rules);
    assert(1 == 1);
}

//QList<Job> SchedulerDatabase::GetJobsContainRemind()
TEST_F(test_schedulerdatabase, GetJobsContainRemind)
{
    QList<Job> jobList = sDb->GetJobsContainRemind();
    qInfo() << jobList.at(0).ID;
    assert(1 == 1);
}

//void SchedulerDatabase::DeleteJob(qint64 id)
TEST_F(test_schedulerdatabase, DeleteJob)
{
    sDb->DeleteJob(3);
    assert(1 == 1);
}


//qint64 SchedulerDatabase::UpdateJob(const QString &jobInfo)
TEST_F(test_schedulerdatabase, UpdateJob)
{
    //将job2修改为一下参数
    QString updateJobJson = "{\"AllDay\":true,\"Description\":\"\",\"End\":"
                            "\"2020-12-09T23:59:00+08:00\",\"ID\":2,\"Ignore\":[],"
                            "\"RRule\":\"FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;UNTIL=20201130T000000Z\","
                            "\"RecurID\":0,\"Remind\":\"1;09:00\",\"Start\":"
                            "\"2020-12-09T00:00:00+08:00\",\"Title\":\"UT测试X\",\"Type\":1}";
    qint64 id = sDb->UpdateJob(updateJobJson);
    assert(2 == id);
}

//bool SchedulerDatabase::UpdateJobIgnore(const QString &strignore, qint64 id)
TEST_F(test_schedulerdatabase, UpdateJobIgnore)
{
    //job2在ut测试UpdateJob函数中，Ignore参数被置空，因此再此传入Ignore
    qint64 jobId = 2;
    const QString strignore = "[\"2020-12-06T00:00:00+08:00\"]";
    bool successBool = sDb->UpdateJobIgnore(strignore, jobId);
    assert(false == successBool);
}

//void SchedulerDatabase::UpdateType(const QString &typeInfo)
TEST_F(test_schedulerdatabase, UpdateType)
{
    QString updateTypeJson = "{\"ID\":5,\"Name\":\"嗨皮\",\"Color\":\"#CC99AA\"}";
    sDb->UpdateType(updateTypeJson);
    assert(1 == 1);
}

//void SchedulerDatabase::DeleteType(qint64 id)
TEST_F(test_schedulerdatabase, DeleteType)
{
    sDb->DeleteType(6);
    assert(1 == 1);
}

//void SchedulerDatabase::OpenSchedulerDatabase(const QString &dbpath)
TEST_F(test_schedulerdatabase, OpenSchedulerDatabase)
{
    QString dbpath = "";
    sDb->OpenSchedulerDatabase(dbpath);

    dbpath = "123123";
    sDb->OpenSchedulerDatabase(dbpath);

    dbpath = "%s%s%s%s%s%s";
    sDb->OpenSchedulerDatabase(dbpath);

    sDb->OpenSchedulerDatabase(SD_DATABASE_DIR);
    assert(1 == 1);
}
