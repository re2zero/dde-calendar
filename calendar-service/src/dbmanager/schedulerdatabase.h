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
#ifndef SCHEDULERDATABASE_H
#define SCHEDULERDATABASE_H
#include "src/commondatastruct.h"
#include "gtest/gtest_prod.h"

#include <QObject>
#include <QSqlDatabase>
#include <QDateTime>

class SchedulerDatabase : public QObject
{
    Q_OBJECT
public:
    explicit SchedulerDatabase(QObject *parent = nullptr);
    void DeleteJob(qint64 id);
    void DeleteType(qint64 id);
    QString GetJob(qint64 id);
    qint64 CreateJob(const Job &job);
    qint64 UpdateJob(const QString &jobInfo);
    bool UpdateJobIgnore(const QString &strignore, qint64 id);
    void UpdateType(const QString &typeInfo);
    QList<Job> GetAllOriginJobs();
    QList<Job> GetAllOriginJobs(const QString &key, const QString &strsort = QString());
    QList<Job> GetAllOriginJobsWithRule(const QString &key, const QString &rules);
    QList<Job> GetJobsContainRemind();
    /**
     * @brief saveRemindJob     存储提醒日程的相关信息
     * @param job               提醒日程
     */
    void saveRemindJob(const Job & job);

    /**
     * @brief updateRemindJob       更新对应的稍后提醒日程
     * @param job
     */
    void updateRemindJob(const Job & job);

    void deleteRemindJob(const QList<qlonglong> &Ids);

    QList<Job> getRemindJobs(const QList<qlonglong> &Ids);
    /**
     * @brief getValidRemindJob     获取未提醒的稍后提醒日程
     * @return
     */
    QList<Job> getValidRemindJob();

    /**
     * @brief clearRemindJobDatabase    清空稍后提醒表
     */
    void clearRemindJobDatabase();

    /**
     * @brief getRemindJob
     * @param id
     * @return
     */
    Job getRemindJob(qint64 id);
private:
    void CreateTables();
    void OpenSchedulerDatabase(const QString &dbpath);
    /**
     * @brief dateTimeToString      将时间转换为string格式
     * @param dateTime
     * @return
     */
    QString dateTimeToString(const QDateTime &dateTime);

signals:

public slots:

private:
    QSqlDatabase m_database;
    FRIEND_TEST(test_schedulerdatabase, dbOparetion);
};

#endif // SCHEDULERDATABASE_H
