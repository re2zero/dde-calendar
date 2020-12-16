/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     chenhaifeng  <chenhaifeng@uniontech.com>
*
* Maintainer: chenhaifeng  <chenhaifeng@uniontech.com>
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
#ifndef CSCHEDULEMANAGE_H
#define CSCHEDULEMANAGE_H

#include "calendardatedatamanage.h"
#include "scheduletask.h"

#include <com_deepin_daemon_timedate.h>

#include <QObject>

typedef  com::deepin::daemon::Timedate DaemonTimeDate;
class CScheduleBaseWidget;
/**
 * @brief The CalendarManage class
 *  日历数据管理类
 */
class CalendarManager : public QObject
{
    Q_OBJECT
public:
    static CalendarManager *getInstance();
    static void releaseInstance();
    //添加显示界面
    void addShowWidget(CScheduleBaseWidget *showWidget);
    //移除显示界面
    void removeShowWidget(CScheduleBaseWidget *showWidget);
    //根据编号获取显示界面
    CScheduleBaseWidget *getShowWidget(const int index);
    //获取显示窗口的数目
    int  getShowWidgetSize();
    //获取日历时间数据管理
    CalendarDateDataManager *getCalendarDateDataManage() const;
    //获取日程任务
    CScheduleTask *getScheduleTask() const;
    //获取是否显示农历信息
    bool getShowLunar() const;
private:
    explicit CalendarManager(QObject *parent = nullptr);
    ~CalendarManager();
private:
    //初始化数据
    void initData();
    //初始化关联
    void initConnection();
    //设置每周首日
    void setWeekBegins(const int value);
    //更新日程信息
    void updateJobs();
public slots:
    //关联dbus信号，每周首日改变事触发
    void WeekBeginsChanged(int  value);
    //日程更新成功刷新界面
    void slotGetScheduleSuccess();
    //农历更新成功刷新界面
    void slotGetLunarSuccess();
    //接收dbus信号更新日程日程信息
    void slotJobsUpdated();
    //
    void slotUpdateSearchSchedule();
private:
    static CalendarManager      *m_scheduleManager;
    CalendarDateDataManager     *m_dateManage;
    DaemonTimeDate              *m_timeDateDbus;
    CScheduleTask               *m_scheduleTask;
    QList<CScheduleBaseWidget *> m_showWidget;
    bool                        m_showLunar;
};

#endif // CSCHEDULEMANAGE_H
