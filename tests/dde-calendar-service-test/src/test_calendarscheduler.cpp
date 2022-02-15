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
#include "src/scheduledatainfo.h"

#include <QThread>
#include <QSqlQuery>
#include <QDebug>

//OpenSchedulerDatabase 脱离原数据库地址，在构建机器上依旧需要打桩来构建新数据库以保证取数据的可靠性
void stub_OpenSDDatabase(void *obj, const QString &dbpath)
{
    Q_UNUSED(dbpath);
    SchedulerDatabase *o = reinterpret_cast<SchedulerDatabase *>(obj);
    QString name;
    {
        name = QSqlDatabase::database().connectionName();
    }
    o->m_database.removeDatabase(name);
    o->m_database = QSqlDatabase::addDatabase("QSQLITE");
    o->m_database.setDatabaseName(SD_DATABASE_DIR);
    o->m_database.open();
    if (o->m_database.isOpen()) {
        const QStringList tables = o->m_database.tables();
        if (tables.size() < 1) {
            o->CreateTables();
        } else {
            //初始化数据库表
            QSqlQuery query(o->m_database);
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

            //初始化本地job_types表，保证本地日历的可用性
            QDateTime currentDateTime = QDateTime::currentDateTime();
            QString sTime = Utils::toconvertData(currentDateTime);

            QString strInitJobType = QString("INSERT INTO job_types (created_at, updated_at, name, color) VALUES "
                                             "(\"%1\", \"%1\", \"学习\", \"#FF0000\"),"
                                             "(\"%1\", \"%1\", \"工作\", \"#00FF00\"),"
                                             "(\"%1\", \"%1\", \"其他\", \"#800080\");")
                                     .arg(sTime);
            query.exec(strInitJobType);
            if (query.isActive()) {
                query.finish();
            }

            o->m_database.commit();

            o->CreateTables();
        }
    }
}

ut_calendarscheduler::ut_calendarscheduler()
{
}

void ut_calendarscheduler::SetUp()
{
    Stub stub;
    stub.set(ADDR(SchedulerDatabase, OpenSchedulerDatabase), stub_OpenSDDatabase);
    calScheduler = new CalendarScheduler();
    qRegisterMetaType<Job>("Job");
    qRegisterMetaType<QList<Job>>("QList<Job>");
}

void ut_calendarscheduler::TearDown()
{
    if (calScheduler->threadremind->isRunning()) {
        calScheduler->threadremind->quit();
        calScheduler->threadremind->wait();
    }
    delete calScheduler;
    calScheduler = nullptr;
}

//QString CalendarScheduler::GetType(qint64 id)
TEST_F(ut_calendarscheduler, GetType)
{
    //无type 0
    qint64 id = 0;
    QString type = calScheduler->GetType(id);
    EXPECT_EQ(type, "");

    id = 1;
    type = calScheduler->GetType(id);
    EXPECT_NE(type, "");
}

//QString CalendarScheduler::GetTypes()
TEST_F(ut_calendarscheduler, GetTypes)
{
    QString types_all = calScheduler->GetTypes();
    EXPECT_NE(types_all, "");
}

//void CalendarScheduler::DeleteType(qint64 id)
TEST_F(ut_calendarscheduler, DeleteType)
{
    int id = 1;
    QString type = calScheduler->GetType(id);
    EXPECT_NE(type, "");
    calScheduler->DeleteType(id);
    type = calScheduler->GetType(id);
    EXPECT_NE(type, "");
}

//void CalendarScheduler::UpdateType(const QString &typeInfo)
TEST_F(ut_calendarscheduler, UpdateType)
{
    int id = 1;
    QString getType = calScheduler->GetType(id);
    QString updateTypeJson = QString("{\"ID\":%1,\"Name\":\"test\",\"Color\":\"#CC99AA\"}").arg(id);
    calScheduler->UpdateType(updateTypeJson);
    EXPECT_EQ(getType, calScheduler->GetType(id)) << "updateType:" << updateTypeJson.toStdString()
                                                  << "getType:" << getType.toStdString()
                                                  << calScheduler->GetType(id).toStdString();
}

//qint64 CalendarScheduler::CreateJob(const QString &jobInfo)
TEST_F(ut_calendarscheduler, CreateJob)
{
    QDateTime currentTime = QDateTime::currentDateTime();

    ScheduleDataInfo info;
    info.setTitleName("test ut");
    info.setType(1);
    info.setAllDay(false);
    info.setBeginDateTime(currentTime);
    info.setEndDateTime(currentTime.addSecs(60 * 60));

    qint64 intJob1 = calScheduler->CreateJob(ScheduleDataInfo::ScheduleToJsonStr(info));

    ScheduleDataInfo info_1;
    info_1.setTitleName("test ut1");
    info_1.setType(1);
    info_1.setAllDay(false);
    info_1.setBeginDateTime(currentTime.addSecs(60 * 60));
    info_1.setEndDateTime(currentTime.addSecs(60 * 60 * 2));

    qint64 intJob2 = calScheduler->CreateJob(ScheduleDataInfo::ScheduleToJsonStr(info_1));

    ScheduleDataInfo info_2;
    info_2.setTitleName("test ut2");
    info_2.setType(2);
    info_2.setAllDay(false);
    info_2.setBeginDateTime(currentTime.addSecs(60 * 60));
    info_2.setEndDateTime(currentTime.addSecs(60 * 60 * 2));

    qint64 intJob3 = calScheduler->CreateJob(ScheduleDataInfo::ScheduleToJsonStr(info_2));

    QString jobs1 = calScheduler->GetJob(intJob1);
    QString jobs2 = calScheduler->GetJob(intJob2);
    QString jobs3 = calScheduler->GetJob(intJob3);

    ScheduleDataInfo scheduleInfo1 = ScheduleDataInfo::JsonStrToSchedule(jobs1);
    EXPECT_TRUE(scheduleInfo1.getTitleName() == info.getTitleName());
    EXPECT_EQ(scheduleInfo1.getAllDay(), info.getAllDay());
    EXPECT_EQ(scheduleInfo1.getType(), info.getType());
    EXPECT_EQ(scheduleInfo1.getDescription(), info.getDescription());

    ScheduleDataInfo scheduleInfo2 = ScheduleDataInfo::JsonStrToSchedule(jobs2);
    EXPECT_TRUE(scheduleInfo2.getTitleName() == info_1.getTitleName());
    EXPECT_EQ(scheduleInfo2.getAllDay(), info_1.getAllDay());
    EXPECT_EQ(scheduleInfo2.getType(), info_1.getType());
    EXPECT_EQ(scheduleInfo2.getDescription(), info_1.getDescription());

    ScheduleDataInfo scheduleInfo3 = ScheduleDataInfo::JsonStrToSchedule(jobs3);
    EXPECT_TRUE(scheduleInfo3.getTitleName() == info_2.getTitleName());
    EXPECT_EQ(scheduleInfo3.getAllDay(), info_2.getAllDay());
    EXPECT_EQ(scheduleInfo3.getType(), info_2.getType());
    EXPECT_EQ(scheduleInfo3.getDescription(), info_2.getDescription());
}

//void CalendarScheduler::DeleteJob(qint64 id)
TEST_F(ut_calendarscheduler, DeleteJob)
{
    QDateTime currentTime = QDateTime::currentDateTime();

    ScheduleDataInfo info;
    info.setTitleName("test ut");
    info.setType(1);
    info.setAllDay(false);
    info.setBeginDateTime(currentTime);
    info.setEndDateTime(currentTime.addSecs(60 * 60));

    qint64 intJob1 = calScheduler->CreateJob(ScheduleDataInfo::ScheduleToJsonStr(info));
    EXPECT_GT(intJob1, 0);
    calScheduler->DeleteJob(intJob1);
    EXPECT_EQ(calScheduler->GetJob(intJob1), "");
}

//void UpdateJob(const QString &jobInfo);
TEST_F(ut_calendarscheduler, UpdateJob)
{
    QDateTime currentTime = QDateTime::currentDateTime();

    ScheduleDataInfo info;
    info.setTitleName("test ut");
    info.setType(1);
    info.setAllDay(false);
    info.setBeginDateTime(currentTime);
    info.setEndDateTime(currentTime.addSecs(60 * 60));

    qint64 intJob1 = calScheduler->CreateJob(ScheduleDataInfo::ScheduleToJsonStr(info));
    EXPECT_GT(intJob1, 0);

    QString getInfoStr = calScheduler->GetJob(intJob1);
    ScheduleDataInfo getInfo = ScheduleDataInfo::JsonStrToSchedule(getInfoStr);
    getInfo.setTitleName("test update");
    calScheduler->UpdateJob(ScheduleDataInfo::ScheduleToJsonStr(getInfo));
    QString getChangetInfoStr = calScheduler->GetJob(intJob1);
    ScheduleDataInfo changetInfo = ScheduleDataInfo::JsonStrToSchedule(getChangetInfoStr);
    EXPECT_EQ(changetInfo.getTitleName(), getInfo.getTitleName());
}


//QString CalendarScheduler::GetJobs(const QDateTime &start, const QDateTime &end)
TEST_F(ut_calendarscheduler, GetJobs)
{
    QDateTime currentTime = QDateTime::currentDateTime();

    ScheduleDataInfo info;
    info.setTitleName("test ut");
    info.setType(1);
    info.setAllDay(false);
    info.setBeginDateTime(currentTime);
    info.setEndDateTime(currentTime.addSecs(60 * 60));

    qint64 intJob1 = calScheduler->CreateJob(ScheduleDataInfo::ScheduleToJsonStr(info));
    EXPECT_GT(intJob1, 0);
    QString jobs = calScheduler->GetJobs(info.getBeginDateTime(), info.getEndDateTime());
    QMap<QDate, QVector<ScheduleDataInfo>> infoMap = ScheduleDataInfo::StrJsonToRangeInfo(jobs);
    ASSERT_GT(infoMap.size(), 0);
    EXPECT_EQ(infoMap.begin().key(), currentTime.date());
    ASSERT_GT(infoMap.begin().value().size(), 0);
    //一天中可能有多个日程
    int num = 0;
    foreach(auto job , infoMap.begin().value()){
        if(job.getTitleName() == info.getTitleName()){
            ++num;
        }
    }
    EXPECT_EQ(num,1);
}

//QString QueryJobs(const QString &params);
TEST_F(ut_calendarscheduler, QueryJobs)
{
    QDateTime currentTime = QDateTime::currentDateTime();

    ScheduleDataInfo info;
    info.setTitleName("test ut");
    info.setType(1);
    info.setAllDay(false);
    info.setBeginDateTime(currentTime);
    info.setEndDateTime(currentTime.addSecs(60 * 60));

    qint64 intJob1 = calScheduler->CreateJob(ScheduleDataInfo::ScheduleToJsonStr(info));
    EXPECT_GT(intJob1, 0);

    const QString params = QString("{\"key\":\"%1\",\"Start\":\"%2\",\"End\":\"%2\"}").arg("").arg(info.getBeginDateTime().toString("yyyy-MM-ddTHH:mm:ss")).arg(info.getEndDateTime().toString("yyyy-MM-ddTHH:mm:ss"));
    QString qJobs = calScheduler->QueryJobs(params);
    QMap<QDate, QVector<ScheduleDataInfo>> infoMap = ScheduleDataInfo::StrJsonToRangeInfo(qJobs);
    ASSERT_GT(infoMap.size(), 0);
    EXPECT_EQ(infoMap.begin().key(), currentTime.date());
    ASSERT_GT(infoMap.begin().value().size(), 0);
    //一天中可能有多个日程
    int num = 0;
    foreach(auto job , infoMap.begin().value()){
        if(job.getTitleName() == info.getTitleName()){
            ++num;
        }
    }
    EXPECT_EQ(num,1);
}

//QString QueryJobsWithLimit(const QString &params, qint32 maxNum);
TEST_F(ut_calendarscheduler, QueryJobsWithLimit_01)
{
    QDateTime currentTime = QDateTime::currentDateTime();

    ScheduleDataInfo info;
    info.setTitleName("test ut");
    info.setType(1);
    info.setAllDay(false);
    info.setBeginDateTime(currentTime);
    info.setEndDateTime(currentTime.addSecs(60 * 60));

    qint64 intJob1 = calScheduler->CreateJob(ScheduleDataInfo::ScheduleToJsonStr(info));
    EXPECT_GT(intJob1, 0);
    const QString params = QString("{\"key\":\"%1\",\"Start\":\"%2\",\"End\":\"%3\"}").arg("").arg(info.getBeginDateTime().toString("yyyy-MM-ddTHH:mm:ss")).arg(info.getEndDateTime().toString("yyyy-MM-ddTHH:mm:ss"));
    QString qJobs = calScheduler->QueryJobsWithLimit(params, 1);
    QMap<QDate, QVector<ScheduleDataInfo>> infoMap = ScheduleDataInfo::StrJsonToRangeInfo(qJobs);
    ASSERT_GT(infoMap.size(), 0);
    EXPECT_EQ(infoMap.begin().key(), currentTime.date());
    ASSERT_GT(infoMap.begin().value().size(), 0);
    EXPECT_EQ(infoMap.begin().value().at(0).getTitleName(), info.getTitleName());
}

TEST_F(ut_calendarscheduler, QueryJobsWithLimit_02)
{
    QDateTime currentTime = QDateTime::currentDateTime();

    ScheduleDataInfo info;
    info.setTitleName("test ut");
    info.setType(1);
    info.setAllDay(false);
    info.setBeginDateTime(currentTime);
    info.setEndDateTime(currentTime.addSecs(60 * 60));

    qint64 intJob1 = calScheduler->CreateJob(ScheduleDataInfo::ScheduleToJsonStr(info));
    EXPECT_GT(intJob1, 0);
    const QString params = QString("{\"key\":\"%1\",\"Start\":\"%2\",\"End\":\"%3\"}").arg("changet").arg(info.getBeginDateTime().toString("yyyy-MM-ddTHH:mm:ss")).arg(info.getEndDateTime().toString("yyyy-MM-ddTHH:mm:ss"));
    QString qJobs = calScheduler->QueryJobsWithLimit(params, 1);
    QMap<QDate, QVector<ScheduleDataInfo>> infoMap = ScheduleDataInfo::StrJsonToRangeInfo(qJobs);
    EXPECT_EQ(infoMap.size(), 0);
}

//QString QueryJobsWithRule(const QString &params, const QString &rules);
TEST_F(ut_calendarscheduler, QueryJobsWithRule_01)
{
    QDateTime currentTime = QDateTime::currentDateTime();

    ScheduleDataInfo info;
    info.setTitleName("test ut");
    info.setType(1);
    info.setAllDay(false);
    info.setBeginDateTime(currentTime);
    info.setEndDateTime(currentTime.addSecs(60 * 60));

    qint64 intJob1 = calScheduler->CreateJob(ScheduleDataInfo::ScheduleToJsonStr(info));
    EXPECT_GT(intJob1, 0);
    const QString params = QString("{\"key\":\"%1\",\"Start\":\"%2\",\"End\":\"%3\"}").arg("").arg(info.getBeginDateTime().toString("yyyy-MM-ddTHH:mm:ss")).arg(info.getEndDateTime().toString("yyyy-MM-ddTHH:mm:ss"));
    const QString rules = "FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;COUNT=2";
    QString qJobs = calScheduler->QueryJobsWithRule(params, rules);
    QMap<QDate, QVector<ScheduleDataInfo>> infoMap = ScheduleDataInfo::StrJsonToRangeInfo(qJobs);
    ASSERT_EQ(infoMap.size(), 0);
}

TEST_F(ut_calendarscheduler, QueryJobsWithRule_02)
{
    QDateTime currentTime = QDateTime::currentDateTime();

    ScheduleDataInfo info;
    info.setTitleName("test ut");
    info.setType(1);
    info.setAllDay(false);
    info.setBeginDateTime(currentTime);
    info.setEndDateTime(currentTime.addSecs(60 * 60));

    qint64 intJob1 = calScheduler->CreateJob(ScheduleDataInfo::ScheduleToJsonStr(info));
    EXPECT_GT(intJob1, 0);
    const QString params = QString("{\"key\":\"%1\",\"Start\":\"%2\",\"End\":\"%3\"}").arg("").arg(info.getBeginDateTime().toString("yyyy-MM-ddTHH:mm:ss")).arg(info.getEndDateTime().toString("yyyy-MM-ddTHH:mm:ss"));
    const QString rules = "";
    QString qJobs = calScheduler->QueryJobsWithRule(params, rules);
    QMap<QDate, QVector<ScheduleDataInfo>> infoMap = ScheduleDataInfo::StrJsonToRangeInfo(qJobs);
    ASSERT_GT(infoMap.size(), 0);
    EXPECT_EQ(infoMap.begin().key(), currentTime.date());
    ASSERT_GT(infoMap.begin().value().size(), 0);
    EXPECT_EQ(infoMap.begin().value().at(0).getTitleName(), info.getTitleName());
}

//void CalendarScheduler::OnModifyJobRemind(const Job &job, const QString &remind)
TEST_F(ut_calendarscheduler, OnModifyJobRemind)
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
    QObject::connect(calScheduler, &CalendarScheduler::JobsUpdated, [ = ](const QList<qlonglong> &Ids) {
        EXPECT_GT(Ids.size(), 0);
    });
    //    calScheduler->get
}

//quint32 CalendarScheduler::GetFestivalId(const QString &name)
TEST_F(ut_calendarscheduler, GetFestivalId)
{
    quint32 id = calScheduler->GetFestivalId("国庆节");
    EXPECT_GT(id, 0);
    id = calScheduler->GetFestivalId("春节");
    EXPECT_GT(id, 0);
}
