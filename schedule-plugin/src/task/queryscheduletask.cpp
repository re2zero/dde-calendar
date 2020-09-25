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
#include "queryscheduletask.h"
#include "../globaldef.h"

queryScheduleTask::queryScheduleTask(CSchedulesDBus *dbus)
    : scheduleBaseTask(dbus)
{
}

Reply queryScheduleTask::SchedulePress(semanticAnalysisTask &semanticTask)
{
    QueryJsonData *queryJsonData = dynamic_cast<QueryJsonData *>(semanticTask.getJsonData());
    //如果转换失败则返回错误消息
    if (queryJsonData == nullptr)
        return errorMessage();
    //查询日程
    if (queryJsonData->offset() > -1
            && queryJsonData->getPropertyStatus() == JsonData::PRO_NONE) {
        Reply m_reply;
        m_reply.setReplyType(Reply::RT_STRING_TTS | Reply::RT_STRING_DISPLAY);
        m_reply.ttsMessage(CANCEL_ERR_TTS);
        m_reply.displayMessage(CANCEL_ERR_TTS);
        return m_reply;
    }

    QString m_queryTitleName = queryJsonData->TitleName();
    QVector<int> getDayNum = queryJsonData->getRepeatNum();
    viewWidget = new viewschedulewidget();

    QVector<ScheduleDateRangeInfo> showdate;
    QVector<ScheduleDateRangeInfo> schedule = getHalfAYearSchedule(m_queryTitleName);

    setDateTime(queryJsonData);

    switch (queryJsonData->getRepeatStatus()) {
    case QueryJsonData::RepeatStatus::EVED: {
        showdate = getEveryDayOrWorkDaySchedule(queryJsonData, 1);
    }
    break;
    case QueryJsonData::RepeatStatus::WORKD: {
        showdate = getEveryDayOrWorkDaySchedule(queryJsonData, 2);
    }
    break;
    case QueryJsonData::RepeatStatus::RESTD: {
        QVector<int> queryRest;
        queryRest.append(6);
        queryRest.append(7);

        showdate = getRestDaySchedule(queryJsonData, queryRest);
    }
    break;
    case QueryJsonData::RepeatStatus::EVEW: {
        showdate = getEveryWeekSchedule(queryJsonData, getDayNum);
    }
    break;
    case QueryJsonData::RepeatStatus::EVEM: {
        showdate = getEveryMonthSchedule(queryJsonData, getDayNum);
    }
    break;
    case QueryJsonData::RepeatStatus::EVEY: {
        showdate = getEveryYearSchedule(queryJsonData);
    }
    break;
    default: {
        showdate = getNonePropertyStatusSchedule(queryJsonData, schedule);
    }
    break;
    }

    viewWidget->viewScheduleInfoShow(showdate);

    Reply m_reply;

    if (queryJsonData->ShouldEndSession()) {
        //不进行多轮
        if (queryOverDueDate(queryJsonData)) {
            //过期时间
            m_reply.setReplyType(Reply::RT_STRING_TTS | Reply::RT_STRING_DISPLAY);
            m_reply.ttsMessage("抱歉，不能查询过期的提醒");
            m_reply.displayMessage("抱歉，不能查询过期的提醒");
        } else if (queryJsonData->getDateTime().size() > 0
                   && queryJsonData->getDateTime().at(0).datetime > QDateTime::currentDateTime().addMonths(6)) {
            //超过半年的时间
            m_reply.setReplyType(Reply::RT_STRING_TTS | Reply::RT_STRING_DISPLAY);
            m_reply.ttsMessage("只能查询未来半年的日程");
            m_reply.displayMessage("只能查询未来半年的日程");
        } else {
            if (viewWidget->getScheduleNum(showdate) == 0) {
                //没有查询的日程
                m_reply.setReplyType(Reply::RT_STRING_TTS | Reply::RT_STRING_DISPLAY);
                m_reply.ttsMessage("没有找到对应的日程");
                m_reply.displayMessage("没有找到对应的日程");
            } else {
                //查询到日程
                QString str = QString("找到%1个日程").arg(viewWidget->getScheduleNum(showdate));
                m_reply.setReplyType(Reply::RT_INNER_WIDGET | Reply::RT_STRING_TTS | Reply::RT_STRING_DISPLAY);
                m_reply.setReplyWidget(viewWidget);
                m_reply.ttsMessage(str);
                m_reply.displayMessage(str);
            }
        }
    } else {
        //多轮的情况
        m_reply.setReplyType(Reply::RT_STRING_TTS | Reply::RT_STRING_DISPLAY);
        //使用建议回复语
        m_reply.ttsMessage(queryJsonData->SuggestMsg());
        m_reply.displayMessage(queryJsonData->SuggestMsg());
    }

    return m_reply;
}

void queryScheduleTask::setDateTime(QueryJsonData *queryJsonData)
{
    switch (queryJsonData->getDateTime().size()) {
    case 1: {
        m_BeginDateTime = queryJsonData->getDateTime().at(0).datetime;
        m_EndDateTime = m_BeginDateTime;
        //时间处理
        if (!queryJsonData->getDateTime().at(0).hasTime) {
            if (m_BeginDateTime.date() == QDate::currentDate()) {
                m_BeginDateTime.setTime(QTime::currentTime());
            } else {
                m_BeginDateTime.setTime(QTime(0, 0, 0));
            }
            m_EndDateTime.setTime(QTime(23, 59, 59));
        }
    }
    break;
    case 2: {
        m_BeginDateTime = queryJsonData->getDateTime().at(0).datetime;
        m_EndDateTime = queryJsonData->getDateTime().at(1).datetime;
        //日期处理
        if (queryJsonData->getDateTime().at(0).datetime.date() < QDate::currentDate()) {
            m_BeginDateTime.setDate(QDate::currentDate());
        }
        if (queryJsonData->getDateTime().at(1).datetime.date() > QDate::currentDate().addMonths(6)) {
            m_EndDateTime.setDate(QDate::currentDate().addMonths(6));
        }
        if (queryJsonData->getDateTime().at(1).datetime.date() < QDate::currentDate()) {
            break;
        }
        //时间处理
        if (queryJsonData->getDateTime().at(0).hasTime) {
            if (queryJsonData->getDateTime().at(0).datetime.time() <= QTime::currentTime())
                m_BeginDateTime.setTime(QTime::currentTime());
        } else {
            if (queryJsonData->getDateTime().at(0).datetime.date() > QDate::currentDate()) {
                m_BeginDateTime.setTime(QTime(0, 0, 0));
            } else {
                m_BeginDateTime.setTime(QTime::currentTime());
            }
        }
        if (!queryJsonData->getDateTime().at(1).hasTime) {
            m_EndDateTime.setTime(QTime(23, 59, 59));
        }
    }
    break;
    default: {
        //如果没有时间，设置开始结束时间为无效时间
        m_BeginDateTime.setDate(QDate(0,0,0));
        m_BeginDateTime.setTime(QTime(0,0,0));
        m_EndDateTime.setDate(QDate(0,0,0));
        m_EndDateTime.setTime(QTime(0,0,0));
    }
    break;
    }
}

QVector<ScheduleDateRangeInfo> queryScheduleTask::getHalfAYearSchedule(QString titleName)
{
    QVector<ScheduleDateRangeInfo> schedule;
    TIME_FRAME_IN_THE_NEXT_SIX_MONTHT
    m_dbus->QueryJobs(titleName, beginTime, endTime, schedule);
    viewWidget->setScheduleDateRangeInfo(schedule);
    return viewWidget->getAllScheduleInfo();
}

QVector<ScheduleDateRangeInfo> queryScheduleTask::getEveryDayOrWorkDaySchedule(QueryJsonData *queryJsonData, int repeat)
{
    QTime m_BTime;
    QTime m_ETime;
    QVector<ScheduleDateRangeInfo> schedule;
    schedule = viewWidget->getAllRpeatScheduleInfo(repeat);
    if (queryJsonData->getDateTime().at(0).hasTime) {
        m_BTime = m_BeginDateTime.time();
        m_ETime = m_EndDateTime.time();
        schedule = viewWidget->queryScheduleWithTime(schedule, m_BTime, m_ETime);
    }
    return schedule;
}

QVector<ScheduleDateRangeInfo> queryScheduleTask::getRestDaySchedule(QueryJsonData *queryJsonData, QVector<int> queryWeek)
{
    QVector<ScheduleDateRangeInfo> schedule;
    QTime m_BTime;
    QTime m_ETime;
    //查询所有周重复的日程
    schedule = viewWidget->getAllRpeatScheduleInfo(3);
    //查询周几-周几的重复日程
    schedule = viewWidget->queryScheduleWithWeek(schedule, queryWeek);
    //如果有时间，则再按照时间进行过滤
    if (queryJsonData->getDateTime().at(0).hasTime) {
        m_BTime = queryJsonData->getDateTime().at(0).datetime.time();
        m_ETime = m_BTime;
        schedule = viewWidget->queryScheduleWithTime(schedule, m_BTime, m_ETime);
    }
    return  schedule;
}

QVector<ScheduleDateRangeInfo> queryScheduleTask::getEveryWeekSchedule(QueryJsonData *queryJsonData, QVector<int> repeatNum)
{
    QTime m_BTime;
    QTime m_ETime;
    QVector<ScheduleDateRangeInfo> schedule;
    //查询所有周重复的日程
    schedule = viewWidget->getAllRpeatScheduleInfo(3);

    if (repeatNum.size() == 1) {
        QVector<int> queryWeek;
        queryWeek.append(repeatNum[0]);

        schedule = viewWidget->queryScheduleWithWeek(schedule, queryWeek);
        if (queryJsonData->getDateTime().at(0).hasTime) {
            m_BTime = queryJsonData->getDateTime().at(0).datetime.time();
            m_ETime = m_BTime;
            schedule = viewWidget->queryScheduleWithTime(schedule, m_BTime, m_ETime);
        }
    } else if (repeatNum.size() == 2) {
        int start = repeatNum[0];
        int end = repeatNum[1];
        QVector<int> queryWeek;

        if (start == end) {
            return viewWidget->getAllRpeatScheduleInfo(1);
        } else if (start < end) {
            if (start == 1 && end == 5) {
                return viewWidget->getAllRpeatScheduleInfo(2);
            } else if (end - start == 6) {
                return viewWidget->getAllRpeatScheduleInfo(1);
            } else {
                for (int i = start; i <= end; i++) {
                    queryWeek.append(i);
                }
            }
        } else {
            if (start - end == 1) {
                return viewWidget->getAllRpeatScheduleInfo(1);
            } else {
                for (int i = 0; i <= 7 - start; i++) {
                    queryWeek.append(i + start);
                }
                for (int i = 0; i < end; i++) {
                    queryWeek.append(i + 1);
                }
            }
        }
        //查询周几-周几的重复日程
        schedule = viewWidget->queryScheduleWithWeek(schedule, queryWeek);
        if (queryJsonData->getDateTime().size() == 1) {
            if (queryJsonData->getDateTime().at(0).hasTime) {
                m_BTime = QTime(0, 0, 0);
                m_ETime = queryJsonData->getDateTime().at(0).datetime.time();
                schedule = viewWidget->queryScheduleWithWeek(schedule, queryWeek, end, m_BTime, m_ETime);
            }
        }
    }
    return schedule;
}

QVector<ScheduleDateRangeInfo> queryScheduleTask::getEveryMonthSchedule(QueryJsonData *queryJsonData, QVector<int> repeatNum)
{
    QTime m_BTime;
    QTime m_ETime;
    QVector<ScheduleDateRangeInfo> schedule;
    //查询所有月重复的日程
    schedule = viewWidget->getAllRpeatScheduleInfo(4);

    if (repeatNum.size() == 1) {
        QVector<int> queryMonth;
        queryMonth.append(repeatNum[0]);

        schedule = viewWidget->queryScheduleWithMonth(schedule, queryMonth);
        if (queryJsonData->getDateTime().size() == 1) {
            if (queryJsonData->getDateTime().at(0).hasTime) {
                m_BTime = queryJsonData->getDateTime().at(0).datetime.time();
                m_ETime = m_BTime;
                schedule = viewWidget->queryScheduleWithTime(schedule, m_BTime, m_ETime);
            }
        } else if (queryJsonData->getDateTime().size() == 2) {
            m_BTime = queryJsonData->getDateTime().at(0).datetime.time();
            m_ETime = queryJsonData->getDateTime().at(1).datetime.time();
            schedule = viewWidget->queryScheduleWithTime(schedule, m_BTime, m_ETime);
        }
    } else if (repeatNum.size() == 2) {
        int start = repeatNum[0];
        int end = repeatNum[1];
        QVector<int> queryMonth;

        if (start == end) {
            return viewWidget->getAllRpeatScheduleInfo(1);
        } else if (start < end) {
            for (int i = start; i <= end; i++) {
                queryMonth.append(i);
            }
        }

        schedule = viewWidget->queryScheduleWithMonth(schedule, queryMonth);
    } else {
        if (queryJsonData->getDateTime().at(0).hasTime) {
            m_BTime = queryJsonData->getDateTime().at(0).datetime.time();
            m_ETime = m_BTime;
            schedule = viewWidget->queryScheduleWithTime(schedule, m_BTime, m_ETime);
        }
    }
    return schedule;
}

QVector<ScheduleDateRangeInfo> queryScheduleTask::getEveryYearSchedule(QueryJsonData *queryJsonData)
{
    Q_UNUSED(queryJsonData)
    QVector<ScheduleDateRangeInfo> schedule;

    if (queryJsonData->getDateTime().size() == 1) {
        //查询所有年重复的日程
        schedule = viewWidget->getAllRpeatScheduleInfo(5);
        //设置查询的开始结束日期
        QDate m_beginD = queryJsonData->getDateTime().at(0).datetime.date();
        QDate m_endD = m_beginD;
        //按照日期查询日程
        schedule = viewWidget->queryScheduleWithDate(schedule, m_beginD, m_endD);
        //如果有时间，则按照时间查询
        if (queryJsonData->getDateTime().at(0).hasTime) {
            QTime m_beginT = queryJsonData->getDateTime().at(0).datetime.time();
            QTime m_endT = m_beginT;
            schedule = viewWidget->queryScheduleWithTime(schedule, m_beginT, m_endT);
        }
    }

    return schedule;
}

QVector<ScheduleDateRangeInfo> queryScheduleTask::getNonePropertyStatusSchedule(QueryJsonData *queryJsonData, QVector<ScheduleDateRangeInfo> schedule)
{
    QVector<ScheduleDateRangeInfo> scheduleInfo;
    scheduleInfo.clear();

    switch (queryJsonData->getPropertyStatus()) {
    case QueryJsonData::PropertyStatus::ALL: {
        //查询所有的日程，因此不需要对已查询到的日程进行过滤操作
        return schedule;
    }
    case QueryJsonData::PropertyStatus::NEXT: {
        if (schedule.isEmpty()) {
            //如果半年内没有日程，返回空容器
            return scheduleInfo;
        } else {
            schedule.clear();
            scheduleInfo = viewWidget->getNextScheduleInfo();
        }
        //返回下一个日程信息
        return scheduleInfo;
    }
    case QueryJsonData::PropertyStatus::LAST:
        break;
    default: {
        if (m_BeginDateTime.isValid()) {
            qDebug() << m_BeginDateTime << m_EndDateTime;
            //按照处理过的日期进行查询
            scheduleInfo = viewWidget->queryScheduleWithDate(schedule, m_BeginDateTime.date(), m_EndDateTime.date());
            //按照处理过的时间进行查询
            scheduleInfo = viewWidget->queryScheduleWithTime(scheduleInfo, m_BeginDateTime.time(), m_EndDateTime.time());
            //返回过滤的日程
            return scheduleInfo;
        }
    }
    }
    //如果以上情况都没有返回半年所有日程
    return schedule;
}

bool queryScheduleTask::queryOverDueDate(QueryJsonData *queryJsonData)
{
    bool overduedate = false;
    int datenum;
    if (queryJsonData->getDateTime().size() > 0) {
        if (queryJsonData->getDateTime().size() == 1)
            datenum = 0;
        else
            datenum = 1;

        if (queryJsonData->getDateTime().at(datenum).datetime.date() < QDate::currentDate()) {
            overduedate = true;
        } else if (queryJsonData->getDateTime().at(datenum).datetime.date() == QDate::currentDate()
                   && queryJsonData->getDateTime().at(datenum).hasTime
                   && queryJsonData->getDateTime().at(datenum).datetime.time() < QTime::currentTime()) {
            overduedate = true;
        } else {
            overduedate = false;
        }
    } else {
        overduedate = false;
    }
    return overduedate;
}

#ifdef LOG_PRINT
void queryScheduleTask::logPrint(const JsonData *jsonData)
{
    Q_UNUSED(jsonData)
}
#endif
