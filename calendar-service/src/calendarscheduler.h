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
#ifndef CALENDARSCHEDULER_H
#define CALENDARSCHEDULER_H
#include "dbmanager/schedulerdatabase.h"
#include "src/commondatastruct.h"

#include <QObject>
#include <QMutex>

class CalendarScheduler : public QObject
{
    Q_OBJECT
public:
    explicit CalendarScheduler(QObject *parent = nullptr);

    QString GetType(qint64 id);
    QString GetTypes();
    void DeleteJob(qint64 id);
    void DeleteType(qint64 id);
    QString GetJob(qint64 id);
    qint64 CreateJob(const QString &jobInfo);
    void UpdateJob(const QString &jobInfo);
    void UpdateType(const QString &typeInfo);
    QString GetJobs(const QDateTime &start, const QDateTime &end);
    QString QueryJobs(const QString &params);

private:
    static quint32 GetFestivalId(const QString &name);
    void IsFestivalJobEnabled();
    QList<stJobArr> GetJobsBetween(const QDateTime &start, const QDateTime &end, const QList<Job> &joblist, const QString &querykey = QString(), bool bextend = true);
    QList<stJobTime> GetJobTimesBetween(const QDateTime &start, const QDateTime &end, const Job &job);
    void FillFestivalJobs(const QDateTime &start, const QDateTime &end, QList<stJobArr> &listjob);
    QList<QDateTime> GetIgnoreList(const Job &job);
    bool ContainsInIgnoreList(const QList<QDateTime> ignorelist, const QDateTime &time);
    bool OverLap(const QDateTime &start, const QDateTime &end, const QDateTime &jobstart, const QDateTime &jobend);
    QDateTime GetNextJobStartTimeByRule(const stRRuleOptions &options, const QDateTime &datetime);
    stRRuleOptions ParseRRule(const QString &rule);
    QJsonObject JobToObject(const Job &job);
    QString JobArrListToJsonStr(const QList<stJobArr> &jobArrList);

signals:

public slots:

private:
    SchedulerDatabase *m_database;
    bool m_festivalJobEnabled = false; //是否允许节假日日程
    static QMap<QString, quint32> m_festivalIdMap; //节日对应节日Id
    static quint32 nextFestivalJobId; //下一个节日id
    static QMutex m_getJobIdMutex; //节日id获取锁
};

#endif // CALENDARSCHEDULER_H
