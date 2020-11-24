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
#include "jobremindmanager.h"
#include "src/utils.h"
#include "dbus/dbusuiopenschedule.h"
#include "dbus/dbusnotify.h"

#include <QTimer>
#include <QDebug>

#define Millisecond 1
#define Second 1000 * Millisecond
#define Minute 60 * Second
#define Hour 60 * Minute

static QString notifyActKeyClose("close");
static QString notifyActKeyRemindLater("later");
static QString notifyActKeyRemind1DayBefore("one-day-before");
static QString notifyActKeyRemindTomorrow("tomorrow");
static QString layoutHM("15:04");
static int notifyCloseReasonDismissedByUser = 2;

JobRemindManager::JobRemindManager(QObject *parent)
    : QObject(parent)
{
    m_dbusuiopen = new DbusUIOpenSchedule("com.deepin.Calendar",
                                          "/com/deepin/Calendar",
                                          QDBusConnection::sessionBus(),
                                          this);
    m_dbusnotify = new DBusNotify("com.deepin.dde.Notification",
                                  "/com/deepin/dde/Notification",
                                  QDBusConnection::sessionBus(),
                                  this);
    initConnections();
}

void JobRemindManager::initConnections()
{
    connect(m_dbusnotify, &DBusNotify::NotificationClosed, this, &JobRemindManager::NotifyClosed);
    connect(m_dbusnotify, &DBusNotify::ActionInvoked, this, &JobRemindManager::ActionInvoked);
}

/**
 * @brief  CallUiOpenSchedule 激活日历前端使之跳转到日视图并弹出编辑框
 * @param job 日程信息
 */
void JobRemindManager::CallUiOpenSchedule(const Job &job)
{
    QJsonObject obj = Utils::JobToObject(job);
    QJsonDocument doc;
    doc.setObject(obj);
    QString strjson(doc.toJson(QJsonDocument::Compact));
    m_dbusuiopen->OpenSchedule(strjson);
}

/**
 * @brief  RemindJob 日程提醒
 * 日程相关信息获取调用dbus服务弹出提醒窗口，并将返回id作为key,job作为value
 * 插入m_notifymap，等待后续提醒弹窗被点击后操作
 * @param job 日程信息
 */
void JobRemindManager::RemindJob(const Job &job)
{
    if (!job.Remind.isEmpty()) {
        int nDays = GetRemindAdvanceDays(job.Remind);
        if (-1 != nDays) {
            qint64 duration = 0;
            bool bmax = GetRemindLaterDuration(job.RemindLaterCount, duration);
            QStringList actionlist;
            if (nDays >= 3 && job.RemindLaterCount == 1) {
                actionlist << notifyActKeyRemind1DayBefore << tr("One day before start") << notifyActKeyClose << tr("Close");
            } else if ((nDays == 1 || nDays == 2) && bmax) {
                actionlist << notifyActKeyRemindTomorrow << tr("Remind me tomorrow") << notifyActKeyClose << tr("Close");
            } else {
                QDateTime tm = QDateTime::currentDateTime();
                tm = tm.addMSecs(duration);
                if (tm < job.Start) {
                    actionlist << notifyActKeyRemindLater << tr("Remind me later") << notifyActKeyClose << tr("Close");
                } else {
                    actionlist << notifyActKeyClose << tr("Close");
                }
            }
            QString title(tr("Schedule Reminder"));
            QString body = GetRemindBody(job, QDateTime::currentDateTime());
            QString appicon("dde-calendar");
            QString appname("dde-calendar");
            quint32 replaces_id = 0;
            qint32 timeout = 0;
            QList<QVariant> argumentList;
            QVariantMap hits;
            argumentList << appname << replaces_id << appicon << title << body << actionlist << hits << timeout;
            qDebug() << __FUNCTION__ << QString("remind now: %1, title:"
                                                " %2, body: %3")
                                            .arg(QDateTime::currentDateTime().toString())
                                            .arg(title)
                                            .arg(body);
            int notifyid = m_dbusnotify->Notify(argumentList);
            m_notifymap.insert(notifyid, job);
        } else {
            qDebug() << __FUNCTION__ << QString("remind job failed id=%1").arg(job.ID);
        }
    }
}

/**
 * @brief  GetRemindAdvanceDays 根据提醒规则获取提前提醒天数
 * @param remind 提醒规则
 * @return 返回天数值
 */
int JobRemindManager::GetRemindAdvanceDays(const QString &remind)
{
    int nDays = 0, min = 0;
    QRegExp reg("\\d;\\d\\d:\\d\\d");
    // QRegExp reg("\\d;\\d+?:\\d+?");
    if (reg.exactMatch(remind)) {
        int hour;
        int ret = sscanf(remind.toStdString().c_str(), "%d;%d:%d", &nDays, &hour, &min);
        if (-1 == ret) {
            nDays = -1;
        }
    } else {
        bool bok = false;
        min = remind.toInt(&bok);
        if (bok) {
            nDays = int(float(min) / float(24 * 60));
        } else {
            nDays = -1;
        }
    }

    return nDays;
}

/**
 * @brief  GetRemindLaterDuration 根据提醒次数获取下一次提醒距现在的时间间隔
 * @param count 该日程已经提醒的次数
 * @param duration 下一次提醒距离现在的时间间隔单位毫秒
 * @return bool 是否超过一小时
 */
bool JobRemindManager::GetRemindLaterDuration(int count, qint64 &duration)
{
    bool bmax = false;
    duration = (10 + ((count - 1) * 5)) * Minute; //下一次提醒距离现在的时间间隔，单位毫秒
    if (duration >= Hour) {
        bmax = true;
        duration = Hour;
    }
    return bmax;
}

/**
 * @brief  GetRemindBody 获取日程提醒内容
 * @param job 日程信息结构体
 * @param tm  查询时间
 * @return QString 提醒内容
 */
QString JobRemindManager::GetRemindBody(const Job &job, const QDateTime &tm)
{
    QString msgStart;
    QString msgEnd;
    msgStart = GetBodyTimePart(tm, job.Start, job.AllDay, true);
    msgEnd = GetBodyTimePart(tm, job.End, job.AllDay, false);
    quint32 startdayofyear = job.Start.date().dayOfYear();
    quint32 enddayofyear = job.End.date().dayOfYear();
    QString prefix;
    if (job.AllDay) {
        if (startdayofyear == enddayofyear) {
            prefix = msgStart;
        } else {
            prefix = QString(tr("%1 to %2")).arg(msgStart).arg(msgEnd);
        }
    } else {
        if (startdayofyear == enddayofyear) {
            msgEnd = job.End.toString(Qt::ISODate);
        }
        prefix = QString(tr("%1 to %2")).arg(msgStart).arg(msgEnd);
    }
    QString strBody = QString("%1 %2").arg(prefix).arg(job.Title);

    return strBody;
}

/**
 * @brief  RemindJobLater 稍后提醒
 * @param job 日程信息结构体
 */
void JobRemindManager::RemindJobLater(const Job &job)
{
    qint64 duration = 0;
    GetRemindLaterDuration(job.RemindLaterCount, duration);
    QTimer *timer = new QTimer(this);
    timer->setInterval(duration);
    m_remindlatertimersmap.insert(timer, job);
    BindToRemindWorkTimeOut(timer);
    timer->start();
}

/**
 * @brief  SetJobRemindOneDayBefore 设置提醒为日程开始一天前
 * @param job 日程信息结构体
 */
void JobRemindManager::SetJobRemindOneDayBefore(const Job &job)
{
    Job jj = job;
    // 非全天，提醒改成24小时前
    // 全天，提醒改成一天前的09:00。
    QString remind = "1440";
    if (jj.AllDay) {
        remind = "1;09:00";
    }
    emit ModifyJobRemind(job, remind);
}

/**
 * @brief  SetJobRemindOneDayBefore 设置提醒为明天
 * @param job 日程信息结构体
 */
void JobRemindManager::SetJobRemindTomorrow(const Job &job)
{
    Job jj = job;
    // 非全天，提醒改成1小时前；
    // 全天，提醒改成当天09:00。
    QString remind = "60";
    if (jj.AllDay) {
        remind = "0;09:00";
    }
    emit ModifyJobRemind(job, remind);
}

//定时器绑定超时处理槽函数
void JobRemindManager::BindToRemindWorkTimeOut(QTimer *timer)
{
    connect(timer, &QTimer::timeout, this, &JobRemindManager::RemindWorkTimeOut);
}

QString JobRemindManager::GetBodyTimePart(const QDateTime &nowtime, const QDateTime &jobtime, bool allday, bool isstart)
{
    //ToDo 需确认规则，需根据isstart确认是否为开始时间单独处理
    QString strmsg;
    int diff = nowtime.daysTo(jobtime); //jobtime只可能大于等于当前remind任务执行的当前时间
    if (allday) {
        if (diff == 0) {
            strmsg = tr("today");
        } else if (diff == 1) {
            strmsg = tr("tomorrow");
        }
    } else {
        //strmsg = cFormatTime("%x %H:%M", t)
        if (diff == 0) {
            strmsg = tr("today");
        } else if (diff == 1) {
            strmsg = tr("tomorrow");
        }
        strmsg.append(QString(" %1").arg(jobtime.toString("HH:mm")));
    }

    return strmsg;
}

//所有内部定时器超时处理槽函数
void JobRemindManager::RemindWorkTimeOut()
{
    qDebug() << __FUNCTION__;
    QTimer *timer = qobject_cast<QTimer *>(sender());
    auto it = m_timejobmap.find(timer);
    if (it != m_timejobmap.end()) {
        RemindJob(it.value());
        it.key()->stop();
        m_timejobmap.remove(it.key());
        it.key()->deleteLater();
    }

    it = m_remindlatertimersmap.find(timer);
    if (it != m_remindlatertimersmap.end()) {
        RemindJob(it.value());
        it.key()->stop();
        m_remindlatertimersmap.remove(it.key());
        it.key()->deleteLater();
    }
}

/**
 * @brief  UpdateRemindJobs 提醒日程更新槽
 * @param jobs 待提醒日程集合
 */
void JobRemindManager::UpdateRemindJobs(const QList<Job> &jobs)
{
    qDebug() << __FUNCTION__ << jobs.size();
    auto it = m_timejobmap.begin();
    for (; it != m_timejobmap.end(); ++it) {
        it.key()->stop();
        it.key()->deleteLater();
        qDebug() << __FUNCTION__ << "******stop job id=" << it.value().ID;
    }
    foreach (Job jb, jobs) {
        //计算当前距离提醒任务还剩多长时间，单位：毫秒
        qDebug() << __FUNCTION__ << "RemindTime=" << jb.RemidTime;
        qint64 msec = QDateTime::currentDateTime().msecsTo(jb.RemidTime);
        QTimer *timer = new QTimer(this);
        timer->setInterval(msec);
        m_timejobmap.insert(timer, jb);
        BindToRemindWorkTimeOut(timer);
        timer->start();
        qDebug() << __FUNCTION__ << "******start job id=" << jb.ID << "timeout=" << msec;
    }
}

//Job被改变删除稍后提醒队列里对应id的Job
void JobRemindManager::NotifyJobsChanged(const QList<qlonglong> &Ids)
{
    foreach (qlonglong id, Ids) {
        for (auto it = m_remindlatertimersmap.begin(); it != m_remindlatertimersmap.end(); ++it) {
            if (it.value().ID == id) {
                it.key()->stop();
                it.key()->deleteLater();
                m_remindlatertimersmap.remove(it.key());
                break; //一定要跳出
            }
        }
    }
}

/**
 * @brief  NotifyClosed 关闭消息提醒通知处理函数
 * @param id 该提醒对应的id
 * @param reason 提醒通知关闭的处理方式：关闭/稍后提醒/明天提醒等
 */
void JobRemindManager::NotifyClosed(quint32 id, quint32 reason)
{
    qDebug() << __FUNCTION__ << QString("*********------id=%1 reason=%2").arg(id).arg(reason);

    //点击消息提示框reason==2激活日历窗口，否则直接返回
    if (reason != notifyCloseReasonDismissedByUser) {
        return;
    }
    auto it = m_notifymap.find(id);
    if (it != m_notifymap.end()) {
        Job job = it.value();
        CallUiOpenSchedule(job);
        m_notifymap.remove(it.key());
    }
}

/**
 * @brief  ActionInvoked 关闭消息提醒通知动作响应处理函数
 * @param id 该提醒对应的id
 * @param actionKey 对应动作 close/later/tommorrow/one-day-before
 */
void JobRemindManager::ActionInvoked(quint32 id, const QString &actionKey)
{
    qDebug() << __FUNCTION__ << QString("*********+++++++++id=%1 actionKey=%2").arg(id).arg(actionKey);
    auto it = m_notifymap.find(id);
    if (it != m_notifymap.end()) {
        Job job = it.value();
        if (0 == actionKey.compare(notifyActKeyRemindLater)) {
            job.RemindLaterCount++;
            RemindJobLater(job);
        } else if (0 == actionKey.compare(notifyActKeyRemind1DayBefore)) {
            SetJobRemindOneDayBefore(job);
        } else if (0 == actionKey.compare(notifyActKeyRemindTomorrow)) {
            SetJobRemindTomorrow(job);
        }
    }
}
