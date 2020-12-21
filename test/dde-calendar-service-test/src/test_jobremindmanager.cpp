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
#include "test_jobremindmanager.h"
#include <QDebug>

test_jobremindmanager::test_jobremindmanager()
{
    jobRemindManager = new JobRemindManager();
}

test_jobremindmanager::~test_jobremindmanager()
{
    delete jobRemindManager;
}

Job createJobs(uint i)
{
    Job job1;
    job1.Start = QDateTime::fromString("2020-12-21T00:00:00+08:00", Qt::ISODate);
    job1.End = QDateTime::fromString("2020-12-21T23:59:00+08:00", Qt::ISODate);
    job1.AllDay = true;
    job1.Type = 1;
    job1.Description = "";
    job1.ID = 0;
    job1.Ignore = "[]";
    job1.Title = "UT测试A";
    job1.RRule = "FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;COUNT=3";
    job1.RecurID = 0;
    job1.Remind = "1;09:00";
    if (1 == i) {
        return job1;
    }

    Job job2;
    job2.Start = QDateTime::fromString("2020-12-22T00:00:00+08:00", Qt::ISODate);
    job2.End = QDateTime::fromString("2020-12-22T23:59:00+08:00", Qt::ISODate);
    job2.AllDay = true;
    job2.Type = 1;
    job2.Description = "";
    job2.ID = 0;
    job2.Ignore = "[\"2020-12-11T00:00:00+08:00\"]";
    job2.Title = "UT测试B";
    job2.RRule = "FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;COUNT=2";
    job2.RecurID = 0;
    job2.Remind = "1;09:00";
    if (2 == i) {
        return job2;
    }

    Job job3;
    job3.Start = QDateTime::fromString("2020-12-24T00:00:00+08:00", Qt::ISODate);
    job3.End = QDateTime::fromString("2020-12-24T23:59:00+08:00", Qt::ISODate);
    job3.AllDay = true;
    job3.Type = 1;
    job3.Description = "";
    job3.ID = 0;
    job3.Ignore = "[]";
    job3.Title = "UT测试C";
    job3.RRule = "FREQ=DAILY;BYDAY=MO,TU,WE,TH,FR;COUNT=4";
    job3.RecurID = 0;
    job3.Remind = "1;09:00";
    if (3 == i) {
        return job3;
    }
    qInfo() << "number is error";
    return Job{};
}

//void JobRemindManager::CallUiOpenSchedule(const Job &job)
TEST_F(test_jobremindmanager, CallUiOpenSchedule)
{
    //jobRemindManager->CallUiOpenSchedule(createJobs(1));
}

//void JobRemindManager::RemindJob(const Job &job)
TEST_F(test_jobremindmanager, RemindJob)
{
    //jobRemindManager->RemindJob(createJobs(1));
}

//int JobRemindManager::GetRemindAdvanceDays(const QString &remind)
TEST_F(test_jobremindmanager, GetRemindAdvanceDays)
{
    const QString remind = "1,09:00";
    //int dayCount = jobRemindManager->GetRemindAdvanceDays(remind);
}

//bool JobRemindManager::GetRemindLaterDuration(int count, qint64 &duration)
TEST_F(test_jobremindmanager, GetRemindLaterDuration)
{

}

//QString JobRemindManager::GetRemindBody(const Job &job, const QDateTime &tm)
TEST_F(test_jobremindmanager, GetRemindBody)
{

}

//void JobRemindManager::RemindJobLater(const Job &job)
TEST_F(test_jobremindmanager, RemindJobLater)
{

}

//void JobRemindManager::SetJobRemindOneDayBefore(const Job &job)
TEST_F(test_jobremindmanager, SetJobRemindOneDayBefore)
{

}

//void JobRemindManager::SetJobRemindTomorrow(const Job &job)
TEST_F(test_jobremindmanager, SetJobRemindTomorrow)
{

}

//void JobRemindManager::BindToRemindWorkTimeOut(QTimer *timer)
TEST_F(test_jobremindmanager, BindToRemindWorkTimeOut)
{

}

//QString JobRemindManager::GetBodyTimePart(const QDateTime &nowtime, const QDateTime &jobtime, bool allday, bool isstart)
TEST_F(test_jobremindmanager, GetBodyTimePart)
{

}

//void JobRemindManager::RemindWorkTimeOut()
TEST_F(test_jobremindmanager, RemindWorkTimeOut)
{

}

//void JobRemindManager::UpdateRemindJobs(const QList<Job> &jobs)
TEST_F(test_jobremindmanager, UpdateRemindJobs)
{

}

//void JobRemindManager::NotifyClosed(quint32 id, quint32 reason)
TEST_F(test_jobremindmanager, NotifyClosed)
{

}

//void JobRemindManager::ActionInvoked(quint32 id, const QString &actionKey)
TEST_F(test_jobremindmanager, ActionInvoked)
{

}
