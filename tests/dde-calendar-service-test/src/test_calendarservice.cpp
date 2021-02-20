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
#include "test_calendarservice.h"
#include "../third-party_stub/stub.h"
#include "config.h"
#include "../calendar-basicstruct/src/utils.h"
#include <QSqlQuery>
#include <QDebug>

bool stub_OpenHuangliDatabase_service(void *obj, const QString &dbpath)
{
    Q_UNUSED(dbpath);
    HuangLiDataBase *o = reinterpret_cast<HuangLiDataBase *>(obj);
    o->m_database = QSqlDatabase::addDatabase("QSQLITE");
    o->m_database.setDatabaseName(HL_DATABASE_DIR);
    return o->m_database.open();
}

void stub_OpenSchedulerDatabase_service(void *obj, const QString &dbpath)
{
    Q_UNUSED(dbpath);
    SchedulerDatabase *o = reinterpret_cast<SchedulerDatabase *>(obj);
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

test_calendarservice::test_calendarservice()
{
    Stub stub;
    stub.set(ADDR(HuangLiDataBase, OpenHuangliDatabase), stub_OpenHuangliDatabase_service);
    stub.set(ADDR(SchedulerDatabase, OpenSchedulerDatabase), stub_OpenSchedulerDatabase_service);
    service = new CalendarService();
}

test_calendarservice::~test_calendarservice()
{
    delete service;
}

TEST_F(test_calendarservice, dbOparetion)
{
    QSqlQuery query(service->m_scheduler->m_database->m_database);
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
    service->m_scheduler->m_database->m_database.commit();

    service->m_scheduler->m_database->CreateTables();

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
    service->m_scheduler->m_database->m_database.commit();
}

//QString CalendarService::GetFestivalMonth(quint32 year, quint32 month)
TEST_F(test_calendarservice, GetFestivalMonth)
{
    const QString fesTenMonth = "[{\"description\":\"10月1日至10月8日放假8天，9月27日，10月10日上班\","
                                "\"id\":\"2020100110\",\"list\":[{\"date\":\"2020-10-1\",\"status\":1},"
                                "{\"date\":\"2020-10-2\",\"status\":1},"
                                "{\"date\":\"2020-10-3\",\"status\":1},"
                                "{\"date\":\"2020-10-4\",\"status\":1},"
                                "{\"date\":\"2020-10-5\",\"status\":1},"
                                "{\"date\":\"2020-10-6\",\"status\":1},"
                                "{\"date\":\"2020-10-7\",\"status\":1},"
                                "{\"date\":\"2020-10-8\",\"status\":1},"
                                "{\"date\":\"2020-9-27\",\"status\":2},"
                                "{\"date\":\"2020-10-10\",\"status\":2}],"
                                "\"month\":10,\"name\":\"中秋节\",\"rest\":\"10月9日至10月10日请假2天，与周末连休可拼11天长假。\"}]";
    QString festivalMonth = service->GetFestivalMonth(2020, 10);
    assert(fesTenMonth == festivalMonth);
}

//QString CalendarService::GetHuangLiDay(quint32 year, quint32 month, quint32 day)
TEST_F(test_calendarservice, GetHuangLiDay)
{
    //2020年12月13日黄历信息
    const QString huangli_20201213 = "{\"Avoid\":\"嫁娶.祈福.出火.移徙.入宅.\",\"GanZhiDay\":\"庚寅\",\"GanZhiMonth\":\"戊子\","
                                     "\"GanZhiYear\":\"庚子\",\"LunarDayName\":\"廿九\",\"LunarFestival\":\"\",\"LunarLeapMonth\":0,"
                                     "\"LunarMonthName\":\"十月\",\"SolarFestival\":\"南京大屠杀死难者国家公祭日\","
                                     "\"Suit\":\"纳财.开市.交易.立券.会亲友.进人口.经络.祭祀.祈福.安香.出火.求医.治病.修造.动土.拆卸.扫舍.安床."
                                     "栽种.牧养.开生坟.合寿木.入殓.安葬.启攒.\",\"Term\":\"\",\"Worktime\":0,\"Zodiac\":\"鼠\"}";
    quint32 year = 2020;
    quint32 month = 12;
    quint32 day = 13;
    QString gethuangli = service->GetHuangLiDay(year, month, day);
    assert(huangli_20201213 == gethuangli);

    //2020年12月14日黄历信息
    const QString huangli_20201214 = "{\"Avoid\":\"入宅.修造.动土.破土.安门.上梁.\",\"GanZhiDay\":\"辛卯\",\"GanZhiMonth\":\"戊子\","
                                     "\"GanZhiYear\":\"庚子\",\"LunarDayName\":\"三十\",\"LunarFestival\":\"\",\"LunarLeapMonth\":0,"
                                     "\"LunarMonthName\":\"十月\",\"SolarFestival\":\"\",\"Suit\":\"祭祀.入殓.移柩.余事勿取.\",\"Term\":"
                                     "\"\",\"Worktime\":0,\"Zodiac\":\"鼠\"}";
    day = 14;
    gethuangli = service->GetHuangLiDay(year, month, day);
    assert(huangli_20201214 == gethuangli);
}

//QString CalendarService::GetHuangLiMonth(quint32 year, quint32 month, bool fill)
TEST_F(test_calendarservice, GetHuangLiMonth)
{
    quint32 year = 2020;
    quint32 month = 12;
    bool fill = false;
    service->GetHuangLiMonth(year, month, fill);

    fill = true;
    service->GetHuangLiMonth(year, month, fill);
}

//CaLunarDayInfo CalendarService::GetLunarInfoBySolar(quint32 year, quint32 month, quint32 day)
TEST_F(test_calendarservice, GetLunarInfoBySolar)
{
    quint32 year = 2020;
    quint32 month = 12;
    quint32 day = 13;
    CaLunarDayInfo caLunarDay = service->GetLunarInfoBySolar(year, month, day);
    qInfo() << caLunarDay;
}

//CaLunarMonthInfo CalendarService::GetLunarMonthCalendar(quint32 year, quint32 month, bool fill)
TEST_F(test_calendarservice, GetLunarCalendarMonth)
{
    quint32 year = 2020;
    quint32 month = 12;
    bool fill = false;
    service->GetLunarMonthCalendar(year, month, fill);

    fill = true;
    service->GetLunarMonthCalendar(year, month, fill);
}

//QString CalendarService::GetType(qint64 id)
TEST_F(test_calendarservice, GetType)
{
    //无type 0
    const QString type_0 = "";
    qint64 id = 0;
    QString type = service->GetType(id);
    assert(type_0 == type);

    //type_1 Work #FF0000
    const QString type_1 = "{\n    \"Color\": \"#FF0000\",\n    \"ID\": 1,\n    \"Name\": \"Work\"\n}\n";
    id = 1;
    type = service->GetType(id);
    assert(type_1 == type);
}

//qint64 CalendarService::CreateJob(const QString &jobInfo)
TEST_F(test_calendarservice, CreateJob)
{
    QString jobInfo1 = "{\"AllDay\":true,\"Description\":\"\",\"End\":"
                       "\"2020-12-13T23:59:00+08:00\",\"Ignore\":[],"
                       "\"RRule\":\"FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;COUNT=1\","
                       "\"RecurID\":0,\"Remind\":\"1;09:00\",\"Start\":"
                       "\"2020-12-13T00:00:00+08:00\",\"Title\":\"UT测试A\",\"Type\":1}";
    qint64 id1 = service->CreateJob(jobInfo1);
    assert(1 == id1);

    QString jobInfo2 = "{\"AllDay\":true,\"Description\":\"\",\"End\":"
                       "\"2020-12-14T23:59:00+08:00\",\"Ignore\":[],"
                       "\"RRule\":\"FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;COUNT=2\","
                       "\"RecurID\":0,\"Remind\":\"1;09:00\",\"Start\":"
                       "\"2020-12-14T00:00:00+08:00\",\"Title\":\"UT测试B\",\"Type\":1}";
    qint64 id2 = service->CreateJob(jobInfo2);
    assert(2 == id2);

    QString jobInfo3 = "{\"AllDay\":true,\"Description\":\"\",\"End\":"
                       "\"2020-12-16T23:59:00+08:00\",\"Ignore\":[],"
                       "\"RRule\":\"FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;COUNT=2\","
                       "\"RecurID\":0,\"Remind\":\"1;09:00\",\"Start\":"
                       "\"2020-12-15T00:00:00+08:00\",\"Title\":\"UT测试C\",\"Type\":1}";
    qint64 id3 = service->CreateJob(jobInfo3);
    assert(3 == id3);
}

//qint64 CalendarService::CreateType(const QString &typeInfo)
TEST_F(test_calendarservice, CreateType)
{
    const QString typeInfo = "";
    service->CreateType(typeInfo);
}

//void CalendarService::DeleteJob(qint64 id)
TEST_F(test_calendarservice, DeleteJob)
{
    qint64 id = 1;
    service->DeleteJob(id);
}

//void CalendarService::DeleteType(qint64 id)
TEST_F(test_calendarservice, DeleteType)
{
    qint64 id = 4;
    service->DeleteType(id);
}

//QString CalendarService::GetTypes()
TEST_F(test_calendarservice, GetTypes)
{
    const QString types = "";
    QString getTypes = service->GetTypes();
    qInfo() << "types = " << getTypes;
}

//QString CalendarService::GetJob(qint64 id)
TEST_F(test_calendarservice, GetJob)
{
    qint64 id = 2;
    QString getJob = service->GetJob(id);
    qInfo() << "getJob = " << getJob;
}

//QString CalendarService::GetJobs(quint32 startYear, quint32 startMonth, quint32 startDay, quint32 endYear, quint32 endMonth, quint32 endDay)
TEST_F(test_calendarservice, GetJobs)
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

    QString jobs = service->GetJobs(2020, 12, 16, 2020, 12, 17);
    assert(getjobs == jobs);
}

//QString CalendarService::QueryJobs(const QString &params);
TEST_F(test_calendarservice, QueryJobs)
{
    const QString jobs = "[{\"Date\":\"2020-12-16\",\"Jobs\":"
                         "[{\"AllDay\":true,\"Description\":\"\",\"End\":\"2020-12-17T23:59:00+08:00\","
                         "\"ID\":3,\"Ignore\":[],\"RRule\":\"FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;COUNT=2\","
                         "\"RecurID\":1,\"Remind\":\"1;09:00\",\"Start\":\"2020-12-16T00:00:00+08:00\",\"Title\":\"UT测试C\",\"Type\":1},"
                         "{\"AllDay\":true,\"Description\":\"\",\"End\":\"2020-12-16T23:59:00+08:00\","
                         "\"ID\":3,\"Ignore\":[],\"RRule\":\"FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;COUNT=2\","
                         "\"RecurID\":0,\"Remind\":\"1;09:00\",\"Start\":\"2020-12-15T00:00:00+08:00\",\"Title\":\"UT测试C\",\"Type\":1}]}]";
    const QString params = "{\"key\":\"ce\",\"Start\":\"2020-12-16T00:00:00\",\"End\":\"2020-12-16T23:59:00\"}";
    QString qJobs = service->QueryJobs(params);
    assert(jobs == qJobs);
}

//void CalendarService::UpdateJob(const QString &jobInfo);
TEST_F(test_calendarservice, UpdateJob)
{
    const QString jobInfo = "{\"AllDay\":true,\"Description\":\"\",\"End\":"
                            "\"2020-12-09T23:59:00+08:00\",\"ID\":2,\"Ignore\":[],"
                            "\"RRule\":\"FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;UNTIL=20201130T000000Z\","
                            "\"RecurID\":0,\"Remind\":\"1;09:00\",\"Start\":"
                            "\"2020-12-09T00:00:00+08:00\",\"Title\":\"UT测试X\",\"Type\":1}";
    service->UpdateJob(jobInfo);
}

//QString CalendarService::QueryJobsWithLimit(const QString &params, qint32 maxNum);
TEST_F(test_calendarservice, QueryJobsWithLimit)
{
    const QString jobs = "[{\"Date\":\"2020-12-16\",\"Jobs\":"
                         "[{\"AllDay\":true,\"Description\":\"\",\"End\":\"2020-12-17T23:59:00+08:00\","
                         "\"ID\":3,\"Ignore\":[],\"RRule\":\"FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;COUNT=2\","
                         "\"RecurID\":1,\"Remind\":\"1;09:00\",\"Start\":\"2020-12-16T00:00:00+08:00\",\"Title\":\"UT测试C\",\"Type\":1}]}]";
    const QString params = "{\"key\":\"ce\",\"Start\":\"2020-12-16T00:00:00\",\"End\":\"2020-12-16T23:59:00\"}";
    QString qJobs = service->QueryJobsWithLimit(params, 1);
    assert(jobs == qJobs);
}

//QString CalendarService::QueryJobsWithRule(const QString &params, const QString &rules);
TEST_F(test_calendarservice, QueryJobsWithRule)
{
    const QString jobs = "[{\"Date\":\"2020-12-16\",\"Jobs\":"
                         "[{\"AllDay\":true,\"Description\":\"\",\"End\":\"2020-12-17T23:59:00+08:00\","
                         "\"ID\":3,\"Ignore\":[],\"RRule\":\"FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;COUNT=2\","
                         "\"RecurID\":1,\"Remind\":\"1;09:00\",\"Start\":\"2020-12-16T00:00:00+08:00\",\"Title\":\"UT测试C\",\"Type\":1}]}]";
    const QString params = "{\"key\":\"ce\",\"Start\":\"2020-12-16T00:00:00\",\"End\":\"2020-12-16T23:59:00\"}";
    const QString rules = "FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;COUNT=2";
    QString qJobs = service->QueryJobsWithRule(params, rules);
    assert(jobs == qJobs);
}

//void CalendarService::UpdateType(const QString &typeInfo)
TEST_F(test_calendarservice, UpdateType)
{
    QString updateTypeJson = "{\"ID\":5,\"Name\":\"嗨皮\",\"Color\":\"#CC99AA\"}";
    service->UpdateType(updateTypeJson);
}
