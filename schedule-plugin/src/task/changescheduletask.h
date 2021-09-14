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
#ifndef CHANGESCHEDULETASK_H
#define CHANGESCHEDULETASK_H

#include "schedulebasetask.h"

class CLocalData;
class changeScheduleTask : public scheduleBaseTask
{
    Q_OBJECT
public:
    explicit changeScheduleTask(CSchedulesDBus *dbus);
    Reply getFeedbackByQuerySchedule(const QVector<ScheduleDtailInfo> &infoVector) override;
    Reply getReplyBySelectSchedule(const ScheduleDtailInfo &info) override;
    Reply InitState(const JsonData *jsonData, bool isUpdateState = false) override;
    Reply repeatScheduleHandle(const ScheduleDtailInfo &info, bool isOnlyOne) override;
    Reply confirwScheduleHandle(const ScheduleDtailInfo &info) override;
    Reply confirmInfo(bool isOK) override;

public slots:
    void slotSelectScheduleIndex(int index);
    void slotButtonCheckNum(int index, const QString &text, const int buttonCount);

private:
    scheduleState *getCurrentState();
    QWidget *createRepeatWidget(const ScheduleDtailInfo &info);
    QWidget *createConfirmWidget(const ScheduleDtailInfo &info);
    QWidget *createInquiryWidget(const ScheduleDtailInfo &info);

    Reply getListScheduleReply(const QVector<ScheduleDtailInfo> &infoVector);
    /**
     * @brief getNextStateBySelectScheduleInfo      根据选择的日程获取下一个修改状态
     * @param info                                  选择的日程信息
     * @param localData                             当前状态的存储数据
     * @param reply                                 修改的回复
     * @return                                      下一个状态
     */
    scheduleState *getNextStateBySelectScheduleInfo(const ScheduleDtailInfo &info, CLocalData *localData, Reply &reply);
    /**
     * @brief getNewInfo        根据修改信息获取新的日程信息
     * @return                  在时间范围内返回true
     */
    bool getNewInfo();

    void changeRepeatSchedule(const ScheduleDtailInfo &info, bool isOnlyOne);
    void changeOnlyInfo(const ScheduleDtailInfo &info);
    void changeAllInfo(const ScheduleDtailInfo &info);
    void changeOrdinarySchedule(const ScheduleDtailInfo &info);
    /**
     * @brief changeDateTimeIsInRange   判断修改的日期在正确的时间范围内
     * @param info          修改过的日程
     * @return              在正常范围内则返回true
     */
    bool changeDateTimeIsInNormalRange(const ScheduleDtailInfo &info);

private:
    QVector<ScheduleDtailInfo> m_scheduleInfo;
};

#endif // CHANGESCHEDULETASK_H
