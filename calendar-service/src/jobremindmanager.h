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
    void CallUiOpenSchedule(const Job &job);

    int GetRemindAdvanceDays(const QString &remind);
    bool GetRemindLaterDuration(int count, qint64 &duration);
    QString GetRemindBody(const Job &job, const QDateTime &tm);
    void RemindJobLater(const Job &job);
    void SetJobRemindOneDayBefore(const Job &job);
    void SetJobRemindTomorrow(const Job &job);
    QString GetBodyTimePart(const QDateTime &nowtime, const QDateTime &jobtime, bool allday, bool isstart);

signals:
    void ModifyJobRemind(const Job &job, const QString &remind);
    void saveNotifyID(const Job &job,int notifyid);
public slots:
    void UpdateRemindJobs(const QList<Job> &jobs);
    void NotifyJobsChanged(const QList<Job> &jobs);
    void RemindJob(const Job &job);
    /**
     * @brief notifyMsgHanding      通知提示框交互处理
     * @param job                   日程信息
     * @param operationNum          操作编号 1：打开日历，2：稍后提醒 3： 明天提醒 4： 提前1天提醒
     */
    void notifyMsgHanding(const Job &job, const int operationNum);

    /**
     * @brief closeNotification     关闭桌面顶部通知
     * @param notifyID              通知ID
     */
    void closeNotification(quint32 notifyID);
private:
    DbusUIOpenSchedule *m_dbusuiopen; //打开日历前端dbus操作相关
    DBusNotify *m_dbusnotify; //日程提醒dbus操作相关
};

#endif // JOBREMINDMANAGER_H
