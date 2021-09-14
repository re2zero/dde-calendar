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
#ifndef JOBREMINDMANAGER_H
#define JOBREMINDMANAGER_H
#include "src/commondatastruct.h"

#include <QObject>
#include <QMap>

class QTimer;
class DbusUIOpenSchedule;
class DBusNotify;

class JobRemindManager : public QObject
{
    Q_OBJECT
public:
    explicit JobRemindManager(QObject *parent = nullptr);

private:
    void initConnections();
    void CallUiOpenSchedule(const Job &job);
    void RemindJob(const Job &job);
    int GetRemindAdvanceDays(const QString &remind);
    bool GetRemindLaterDuration(int count, qint64 &duration);
    QString GetRemindBody(const Job &job, const QDateTime &tm);
    void RemindJobLater(const Job &job);
    void SetJobRemindOneDayBefore(const Job &job);
    void SetJobRemindTomorrow(const Job &job);
    void BindToRemindWorkTimeOut(QTimer *timer);
    QString GetBodyTimePart(const QDateTime &nowtime, const QDateTime &jobtime, bool allday, bool isstart);

signals:
    void ModifyJobRemind(const Job &job, const QString &remind);

public slots:
    void RemindWorkTimeOut();
    void UpdateRemindJobs(const QList<Job> &jobs);
    void NotifyJobsChanged(const QList<qlonglong> &Ids);
    //通知关闭响应（包含关闭，稍后提示等等）
    void NotifyClosed(quint32 id, quint32 reason);
    void ActionInvoked(quint32 id, const QString &actionKey);

private:
    DbusUIOpenSchedule *m_dbusuiopen; //打开日历前端dbus操作相关
    DBusNotify *m_dbusnotify; //日程提醒dbus操作相关
    QMap<int, Job> m_notifymap; // key is notification id
    QMap<QTimer *, Job> m_timejobmap; //key is timer*
    QMap<QTimer *, Job> m_remindlatertimersmap; //稍后提醒队列,该队列的任务除非被关闭或者达到提醒次数，否则一直存在
};

#endif // JOBREMINDMANAGER_H
