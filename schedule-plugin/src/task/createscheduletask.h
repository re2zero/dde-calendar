/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     uniontech  <uniontech@uniontech.com>
*
* Maintainer: uniontech  <chenhaifeng@uniontech.com>
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
#ifndef CREATESCHEDULETASK_H
#define CREATESCHEDULETASK_H

#include "schedulebasetask.h"
#include "../data/createjsondata.h"
#include "../widget/createschedulewidget.h"

class createScheduleTask : public scheduleBaseTask
{
    Q_OBJECT
public:
    createScheduleTask(CSchedulesDBus *dbus);
    Reply SchedulePress(semanticAnalysisTask &semanticTask) override;

private:
    /**
     * @brief setDateTime 设置查询的开始结束时间
     * @param queryJsonData jsondata句柄
     */
    void setDateTime(CreateJsonData *createJsonData);
    /**
     * @brief setScheduleTitleName 设置日程的titlename
     * @param createJsonData jsondata句柄
     */
    void setScheduleTitleName(CreateJsonData *createJsonData);
    /**
     * @brief setNotRepeatDay 获取非重复的日程
     * @return 日程信息
     */
    QVector<ScheduleDtailInfo> getNotRepeatDaySchedule();
    /**
     * @brief getEveryDaySchedule 获取每天重复的日程
     * @return 日程信息
     */
    QVector<ScheduleDtailInfo> getEveryDaySchedule();
    /**
     * @brief getEveryWeekSchedule 获取每周重复的日程
     * @param dateRange 开始和结束的周数
     * @return 日程信息
     */
    QVector<ScheduleDtailInfo> getEveryWeekSchedule(QVector<int> dateRange);
    /**
     * @brief getEveryMonthSchedule 获取每月重复的日程
     * @param dateRange 开始和结束的日期数
     * @return 日程信息
     */
    QVector<ScheduleDtailInfo> getEveryMonthSchedule(QVector<int> dateRange);
    /**
     * @brief getEveryDYearSchedule 获取每年重复的日程
     * @return 日程信息
     */
    QVector<ScheduleDtailInfo> getEveryDYearSchedule();
    /**
     * @brief getEveryWorkDaySchedule 获取工作日的日程
     * @return 日程信息
     */
    QVector<ScheduleDtailInfo> getEveryWorkDaySchedule();
    /**
     * @brief getEveryRestDaySchedule 获取休息日的日程
     * @return 日程信息
     */
    QVector<ScheduleDtailInfo> getEveryRestDaySchedule();
    /**
     * @brief getFirstSchedule 获取第一个日程
     * @param scheduleInfo 所有日程信息的容器
     * @return 第一个日程的日程信息
     */
    ScheduleDtailInfo getFirstSchedule(QVector<ScheduleDtailInfo> scheduleInfo);
    /**
     * @brief getCreatesDays 获取新建日程的天数
     * @param firstDay 开始日期
     * @param secondday 结束日期
     * @param containsToday 是否包含今天
     * @return 总天数
     */
    int getCreatesDays(int firstDay, int secondday, bool containsToday);
    /**
     * @brief getNoneWeekNumDate 获取没有周数的重复日程的日期
     * @return 日期容器
     */
    QVector<QDateTime> getNoneWeekNumDate();
    /**
     * @brief getOneWeekNumDate 获取有一个周数的重复日程的日期
     * @param firstWeekNum 这个周数是周几
     * @return 日期容器
     */
    QVector<QDateTime> getOneWeekNumDate(int firstWeekNum);
    /**
     * @brief getTwoWeekNumDate 获取有两个周数的重复日程的日期
     * @param firstWeekNum 第一个周数是周几
     * @param secondWeekNum 第二个周数是周几
     * @return 日期容器
     */
    QVector<QDateTime> getTwoWeekNumDate(int firstWeekNum, int secondWeekNum);
    /**
     * @brief firstWeekNumLessThanSecond 获取第一个周数小于第二个周数的日程的日期
     * @param firstWeekNum 第一个周数
     * @param secondWeekNum 第二个周数
     * @return 日期容器
     */
    QVector<QDateTime> firstWeekNumLessThanSecond(int firstWeekNum, int secondWeekNum);
    /**
     * @brief firstWeekNumGreaterThanSecondButEveryDay 获取第一个周数大于第二个周数但不包含全天日程的日程
     * @param firstWeekNum 第一个周数
     * @param secondWeekNum 第二个周数
     * @return 日期容器
     */
    QVector<QDateTime> firstWeekNumGreaterThanSecondButEveryDay(int firstWeekNum, int secondWeekNum);
    /**
     * @brief getWeekAllDateTime 获取所有的周重复日程的时间
     * @param BeginDate 日程开始的时间
     * @param firstDayofWeek 获取到的第一个周数
     * @param secondDayofWeek 获取到的第二个周数
     * @return DateTime容器
     */
    QVector<QDateTime> getWeekAllDateTime(QDate BeginDate, int firstWeekNum, int secondWeekNum);
    /**
     * @brief getWeekFrontPartDateTime 获取当前时间之前的日程时间
     * @param BeginDate 日程开始的时间
     * @param firstDate 获取到的第一个周数
     * @param containsToday 是否包含今天
     * @return DateTime容器
     */
    QVector<QDateTime> getWeekFrontPartDateTime(QDate BeginDate, int firstWeekNum, bool containsToday);
    /**
     * @brief getWeekBackPartDateTime 获取当前时间之后的日程时间
     * @param BeginDate 日程开始时间
     * @param secondDayofWeek 获取到的第二个周数
     * @param containsToday 是否包含今天
     * @return DateTime容器
     */
    QVector<QDateTime> getWeekBackPartDateTime(QDate BeginDate, int secondWeekNum, bool containsToday);
    /**
     * @brief analysisEveryWeekDate 解析每周重复日程的日期
     * @param dateRange 周数范围
     * @return 日期容器
     */
    QVector<QDateTime> analysisEveryWeekDate(QVector<int> dateRange);
    /**
     * @brief analysisEveryMonthDate 解析每月重复日程的日期
     * @param dateRange 日期范围
     * @return 日期容器
     */
    QVector<QDateTime> analysisEveryMonthDate(QVector<int> dateRange);
    /**
     * @brief getNoneMonthNumDate 获取没有日期的每月重复日程的日期
     * @return 日期容器
     */
    QVector<QDateTime> getNoneMonthNumDate();
    /**
     * @brief getOneMonthNumDate 获取有一个日期的每月重复日程的日期
     * @param firstMonthNum 第一个日期是几号
     * @return 日期容器
     */
    QVector<QDateTime> getOneMonthNumDate(int firstMonthNum);
    /**
     * @brief getTwoMonthNumDate 获取有两个日期的每月重复日程的日程
     * @param firstMonthNum 第一个日期是几号
     * @param secondMonthNum 第二个日期是几号
     * @return 日期容器
     */
    QVector<QDateTime> getTwoMonthNumDate(int firstMonthNum, int secondMonthNum);
    /**
     * @brief getMonthAllDateTime 获取每月重复日程的日期
     * @param BeginDate 开始时间
     * @param firstMonthNum 第一个日期
     * @param secondMonthNum 第二个日期
     * @return 日期容器
     */
    QVector<QDateTime> getMonthAllDateTime(QDate BeginDate, int firstMonthNum, int secondMonthNum);
    /**
     * @brief getMonthFrontPartDateTime 获取当前时间之前的日程日期
     * @param BeginDate 开始时间
     * @param firstMonthNum 第一个日期
     * @param containsToday 是否包含今天
     * @return 日期容器
     */
    QVector<QDateTime> getMonthFrontPartDateTime(QDate BeginDate, int firstMonthNum, bool containsToday);
    /**
     * @brief getMonthBackPartDateTime 获取当前时间之后的日程日期
     * @param BeginDate 开始时间
     * @param secondMonthNum 第二个日期
     * @param containsToday 是否包含今天
     * @return 日期容器
     */
    QVector<QDateTime> getMonthBackPartDateTime(QDate BeginDate, int secondMonthNum, bool containsToday);
    /**
     * @brief firstMonthNumLessThanSecond 第一个日期小于第二个日期的日程的日期
     * @param firstMonthNum 第一个日期
     * @param secondWMonthNum 第二个日期
     * @return 日期容器
     */
    QVector<QDateTime> firstMonthNumLessThanSecond(int firstMonthNum, int secondWMonthNum);
    /**
     * @brief firstMonthNumGreaterThanSecondButEveryDay 第一个日期大于第二个日期但不包含每天的日程的日期
     * @param firstMonthNum 第一个日期
     * @param secondMonthNum 第二个日期
     * @return 日期容器
     */
    QVector<QDateTime> firstMonthNumGreaterThanSecondButEveryDay(int firstMonthNum, int secondMonthNum);
    /**
     * @brief analysisRestDayDate 解析休息日的日程的日期
     * @return 日期容器
     */
    QVector<QDateTime> analysisRestDayDate();
    /**
     * @brief setDateTimeAndGetSchedule 设置日程开始结束时间，并获取日程信息
     * @param beginDateTime 日程开始时间
     * @param endDateTime 日程结束时间
     * @return 日程信息
     */
    ScheduleDtailInfo setDateTimeAndGetSchedule(QDateTime beginDateTime, QDateTime endDateTime);
private:
    QDateTime       m_begintime;
    QDateTime       m_endtime;
    bool            everyDayState = false;
    createSchedulewidget        *m_widget = nullptr;
};

#endif // CREATESCHEDULETASK_H
