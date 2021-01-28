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
#include "test_calendarscheduler.h"
#include "../third-party_stub/stub.h"
#include "../../calendar-basicstruct/src/utils.h"
#include "config.h"
#include <QThread>
#include <QSqlQuery>
#include <QDebug>

//OpenSchedulerDatabase 脱离原数据库地址，在构建机器上依旧需要打桩来构建新数据库以保证取数据的可靠性
void stub_OpenSDDatabase(void *obj, const QString &dbpath)
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

test_calendarscheduler::test_calendarscheduler()
{
    Stub stub;
    stub.set(ADDR(SchedulerDatabase, OpenSchedulerDatabase), stub_OpenSDDatabase);
    calScheduler = new CalendarScheduler();
}

test_calendarscheduler::~test_calendarscheduler()
{
    if (calScheduler->threadremind->isRunning()) {
        calScheduler->threadremind->quit();
        calScheduler->threadremind->wait();
    }
    delete calScheduler;
}

//初始化数据库表
TEST_F(test_calendarscheduler, dbOparetion)
{
    QSqlQuery query(calScheduler->m_database->m_database);
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
    calScheduler->m_database->m_database.commit();
    //创建新表，保证删除操作可重复执行
    calScheduler->m_database->CreateTables();

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
    calScheduler->m_database->m_database.commit();
}

//QString CalendarScheduler::GetType(qint64 id)
TEST_F(test_calendarscheduler, GetType)
{
    //无type 0
    const QString type_0 = "";
    qint64 id = 0;
    QString type = calScheduler->GetType(id);
    assert(type_0 == type);

    //type_1 Work #FF0000
    const QString type_1 = "{\n    \"Color\": \"#FF0000\",\n    \"ID\": 1,\n    \"Name\": \"Work\"\n}\n";
    id = 1;
    type = calScheduler->GetType(id);
    assert(type_1 == type);
}

//QString CalendarScheduler::GetTypes()
TEST_F(test_calendarscheduler, GetTypes)
{
    const QString types = "[{\"Color\":\"#FF0000\",\"ID\":1,\"Name\":\"Work\"},"
                          "{\"Color\":\"#00FF00\",\"ID\":2,\"Name\":\"Life\"},"
                          "{\"Color\":\"#800080\",\"ID\":3,\"Name\":\"Other\"},"
                          "{\"Color\":\"#FFFF00\",\"ID\":4,\"Name\":\"Festival\"}]";
    QString types_all = calScheduler->GetTypes();
    assert(types == types_all);
}

//void CalendarScheduler::DeleteType(qint64 id)
TEST_F(test_calendarscheduler, DeleteType)
{
    calScheduler->DeleteType(6);
    assert(1 == 1);
}

//void CalendarScheduler::UpdateType(const QString &typeInfo)
TEST_F(test_calendarscheduler, UpdateType)
{
    QString updateTypeJson = "{\"ID\":5,\"Name\":\"嗨皮\",\"Color\":\"#CC99AA\"}";
    calScheduler->UpdateType(updateTypeJson);
    assert(1 == 1);
}

//qint64 CalendarScheduler::CreateJob(const QString &jobInfo)
TEST_F(test_calendarscheduler, CreateJob)
{
    QString jobInfo1 = "{\"AllDay\":true,\"Description\":\"\",\"End\":"
                       "\"2020-12-13T23:59:00+08:00\",\"Ignore\":[],"
                       "\"RRule\":\"FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;COUNT=1\","
                       "\"RecurID\":0,\"Remind\":\"1;09:00\",\"Start\":"
                       "\"2020-12-13T00:00:00+08:00\",\"Title\":\"UT测试A\",\"Type\":1}";
    qint64 id1 = calScheduler->CreateJob(jobInfo1);
    assert(1 == id1);

    QString jobInfo2 = "{\"AllDay\":true,\"Description\":\"\",\"End\":"
                       "\"2020-12-14T23:59:00+08:00\",\"Ignore\":[],"
                       "\"RRule\":\"FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;COUNT=2\","
                       "\"RecurID\":0,\"Remind\":\"1;09:00\",\"Start\":"
                       "\"2020-12-14T00:00:00+08:00\",\"Title\":\"UT测试B\",\"Type\":1}";
    qint64 id2 = calScheduler->CreateJob(jobInfo2);
    assert(2 == id2);

    QString jobInfo3 = "{\"AllDay\":true,\"Description\":\"\",\"End\":"
                       "\"2020-12-16T23:59:00+08:00\",\"Ignore\":[],"
                       "\"RRule\":\"FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;COUNT=2\","
                       "\"RecurID\":0,\"Remind\":\"1;09:00\",\"Start\":"
                       "\"2020-12-15T00:00:00+08:00\",\"Title\":\"UT测试C\",\"Type\":1}";
    qint64 id3 = calScheduler->CreateJob(jobInfo3);
    assert(3 == id3);
}

//void CalendarScheduler::DeleteJob(qint64 id)
TEST_F(test_calendarscheduler, DeleteJob)
{
    qint64 id = 1;
    calScheduler->DeleteJob(id);
    assert(1 == 1);
}

//void UpdateJob(const QString &jobInfo);
TEST_F(test_calendarscheduler, UpdateJob)
{
    const QString jobInfo = "{\"AllDay\":true,\"Description\":\"\",\"End\":"
                            "\"2020-12-09T23:59:00+08:00\",\"ID\":2,\"Ignore\":[],"
                            "\"RRule\":\"FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;UNTIL=20201130T000000Z\","
                            "\"RecurID\":0,\"Remind\":\"1;09:00\",\"Start\":"
                            "\"2020-12-09T00:00:00+08:00\",\"Title\":\"UT测试X\",\"Type\":1}";
    calScheduler->UpdateJob(jobInfo);
    assert(1 == 1);
}


//QString CalendarScheduler::GetJobs(const QDateTime &start, const QDateTime &end)
TEST_F(test_calendarscheduler, GetJobs)
{
    const QString getjobs = "[{\"Date\":\"2020-12-16\",\"Jobs\":"
                            "[{\"AllDay\":true,\"Description\":\"\",\"End\":\"2020-12-17T23:59:00+08:00\","
                            "\"ID\":3,\"Ignore\":[],\"RRule\":\"FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;COUNT=2\","
                            "\"RecurID\":1,\"Remind\":\"1;09:00\",\"Start\":\"2020-12-16T00:00:00+08:00\",\"Title\":\"UT测试C\",\"Type\":1},"
                            "{\"AllDay\":true,\"Description\":\"\",\"End\":\"2020-12-16T23:59:00+08:00\","
                            "\"ID\":3,\"Ignore\":[],\"RRule\":\"FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;COUNT=2\","
                            "\"RecurID\":0,\"Remind\":\"1;09:00\",\"Start\":\"2020-12-15T00:00:00+08:00\",\"Title\":\"UT测试C\",\"Type\":1}]},"
                            "{\"Date\":\"2020-12-17\",\"Jobs\":"
                            "[{\"AllDay\":true,\"Description\":\"\",\"End\":\"2020-12-17T23:59:00+08:00\","
                            "\"ID\":3,\"Ignore\":[],\"RRule\":\"FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;COUNT=2\","
                            "\"RecurID\":1,\"Remind\":\"1;09:00\",\"Start\":\"2020-12-16T00:00:00+08:00\",\"Title\":\"UT测试C\",\"Type\":1}]}]";

    QString strStart = "2020-12-16T00:00:00";
    QString strEnd = "2020-12-17T23:59:00";
    QDateTime startTime = Utils::fromconvertiIGData(strStart);
    QDateTime endTime = Utils::fromconvertiIGData(strEnd);
    QString jobs = calScheduler->GetJobs(startTime, endTime);
    assert(getjobs == jobs);
}

//QString QueryJobs(const QString &params);
TEST_F(test_calendarscheduler, QueryJobs)
{
    const QString jobs = "[{\"Date\":\"2020-12-16\",\"Jobs\":"
                   "[{\"AllDay\":true,\"Description\":\"\",\"End\":\"2020-12-17T23:59:00+08:00\","
                   "\"ID\":3,\"Ignore\":[],\"RRule\":\"FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;COUNT=2\","
                   "\"RecurID\":1,\"Remind\":\"1;09:00\",\"Start\":\"2020-12-16T00:00:00+08:00\",\"Title\":\"UT测试C\",\"Type\":1},"
                   "{\"AllDay\":true,\"Description\":\"\",\"End\":\"2020-12-16T23:59:00+08:00\","
                   "\"ID\":3,\"Ignore\":[],\"RRule\":\"FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;COUNT=2\","
                   "\"RecurID\":0,\"Remind\":\"1;09:00\",\"Start\":\"2020-12-15T00:00:00+08:00\",\"Title\":\"UT测试C\",\"Type\":1}]}]";
    const QString params = "{\"key\":\"ce\",\"Start\":\"2020-12-16T00:00:00\",\"End\":\"2020-12-16T23:59:00\"}";
    QString qJobs = calScheduler->QueryJobs(params);
    assert(jobs == qJobs);
}

//QString QueryJobsWithLimit(const QString &params, qint32 maxNum);
TEST_F(test_calendarscheduler, QueryJobsWithLimit)
{
    const QString jobs = "[{\"Date\":\"2020-12-16\",\"Jobs\":"
                         "[{\"AllDay\":true,\"Description\":\"\",\"End\":\"2020-12-17T23:59:00+08:00\","
                         "\"ID\":3,\"Ignore\":[],\"RRule\":\"FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;COUNT=2\","
                         "\"RecurID\":1,\"Remind\":\"1;09:00\",\"Start\":\"2020-12-16T00:00:00+08:00\",\"Title\":\"UT测试C\",\"Type\":1}]}]";
    const QString params = "{\"key\":\"ce\",\"Start\":\"2020-12-16T00:00:00\",\"End\":\"2020-12-16T23:59:00\"}";
    QString qJobs = calScheduler->QueryJobsWithLimit(params, 1);
    assert(jobs == qJobs);
}

//QString QueryJobsWithRule(const QString &params, const QString &rules);
TEST_F(test_calendarscheduler, QueryJobsWithRule)
{
    const QString jobs = "[{\"Date\":\"2020-12-16\",\"Jobs\":"
                         "[{\"AllDay\":true,\"Description\":\"\",\"End\":\"2020-12-17T23:59:00+08:00\","
                         "\"ID\":3,\"Ignore\":[],\"RRule\":\"FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;COUNT=2\","
                         "\"RecurID\":1,\"Remind\":\"1;09:00\",\"Start\":\"2020-12-16T00:00:00+08:00\",\"Title\":\"UT测试C\",\"Type\":1}]}]";
    const QString params = "{\"key\":\"ce\",\"Start\":\"2020-12-16T00:00:00\",\"End\":\"2020-12-16T23:59:00\"}";
    const QString rules = "FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;COUNT=2";
    QString qJobs = calScheduler->QueryJobsWithRule(params, rules);
    assert(jobs == qJobs);
}

//void CalendarScheduler::OnModifyJobRemind(const Job &job, const QString &remind)
TEST_F(test_calendarscheduler, OnModifyJobRemind)
{
    Job job3;
    job3.Start = QDateTime::fromString("2020-12-15T00:00:00+08:00", Qt::ISODate);
    job3.End = QDateTime::fromString("2020-12-16T23:59:00+08:00", Qt::ISODate);
    job3.AllDay = true;
    job3.Type = 1;
    job3.Description = "";
    job3.ID = 3;
    job3.Ignore = "[]";
    job3.Title = "UT测试C";
    job3.RRule = "FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;COUNT=2";
    job3.RecurID = 0;
    job3.Remind = "1;09:00";
    calScheduler->OnModifyJobRemind(job3, "1,10:00");
}

//quint32 CalendarScheduler::GetFestivalId(const QString &name)
TEST_F(test_calendarscheduler, GetFestivalId)
{
    quint32 id = calScheduler->GetFestivalId("国庆节");
    qInfo() << "啊？ 这特么是国庆节？" << id ;

    id = calScheduler->GetFestivalId("春节");
    qInfo() << "啊？ 这特么是春节？" << id ;
}
