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
#ifndef SCHEDULEJSONDATA_H
#define SCHEDULEJSONDATA_H

#include "repetitionrule.h"
#include "reminddata.h"

#include <QObject>
#include <QDateTime>
#include <QVector>
#include <QMap>
#include <QJsonObject>


class QDebug;
/**
 * @brief The ScheduleJsonData class
 * 日程数据结构与Json对应
 */
class ScheduleDataInfo
{
public:
    explicit ScheduleDataInfo();
    //获取日程ID
    int getID() const;
    //设置日程ID
    void setID(int getID);
    //获取开始时间
    QDateTime &getBeginDateTime();
    QDateTime getBeginDateTime() const;
    //设置开始时间
    void setBeginDateTime(const QDateTime &beginDateTime);
    //获取结束时间
    QDateTime &getEndDateTime();
    QDateTime getEndDateTime() const;
    //设置结束时间
    void setEndDateTime(const QDateTime &endDateTime);
    //获取日程忽略时间集
    QVector<QDateTime> &getIgnoreTime();
    QVector<QDateTime> getIgnoreTime() const;
    //获取日程忽略时间集
    void setIgnoreTime(const QVector<QDateTime> &ignoreVectorTime);
    //获取日程标题
    QString getTitleName() const;
    //设置日程标题
    void setTitleName(const QString &titleName);
    //获取日程描述信息
    QString getDescription() const;
    //设置日程描述信息
    void setDescription(const QString &description);
    //获取日程是否为全天
    bool getAllDay() const;
    //设置日程是否为全天
    void setAllDay(const bool isAllDay);
    //获取日程类型
    int getType() const;
    //设置日程类型
    void setType(int scheudleType);
    //获取日程重复id类型
    int getRecurID() const;
    //设置日程重复id类型
    void setRecurID(int recurID);
    //设置日程提醒规则
    void setRemindData(const RemindData &remindData);
    //获取提醒规则
    RemindData getRemindData() const;
    //设置重复规则
    void setRepetitionRule(const RepetitionRule &rule);
    //获取重复规则
    RepetitionRule &getRepetitionRule();
    //获取重复规则
    RepetitionRule getRepetitionRule() const;
    //该日程是否有效
    bool isValid() const;
    //设置是否为拖拽移动日程
    void setIsMoveInfo(const bool &isMoveInfo);
    //获取是否为拖拽移动日程
    bool getIsMoveInfo()const;
    //日程数据转换为json形式的字符串
    static QString ScheduleToJsonStr(const ScheduleDataInfo &scheduleJsonData);
    //Json对象转换到日程数据
    static ScheduleDataInfo JsonToSchedule(const QJsonObject &scheudleJsonObject);
    //将json转换为日程
    static ScheduleDataInfo JsonStrToSchedule(const QString &jsonStr);
    //string类型的datetime数据转换为QDateTime
    static QDateTime StringDateToDateTime(const QString &dateStr);
    //QDateTime转换为string类型的datetime数据
    static QString DateTimeToStringDate(const QDateTime &dateTime);
    //将JsonArray数据转换为QMap存储Schedule数据
    static QMap<QDate, QVector<ScheduleDataInfo> > JsonArrayDataToScheduleMapData(const QJsonArray &jsonArrData);
    static QMap<QDate, QVector<ScheduleDataInfo> > StrJsonToRangeInfo(const QString &jsonStr);
    //判断日程是否相等
    bool operator ==(const ScheduleDataInfo &info) const;
    //判断日程大小
    bool operator <(const ScheduleDataInfo &info) const;
private:
    //获取日程提醒规则
    QString getScheudleRemind() const;
    //设置日程提醒规则
    void setScheudleRemind(const QString &scheudleRemind);
    //获取日程重复规则
    QString getScheduleRRule() const;
    //设置日程重复规则
    void setScheduleRRule(const QString &scheduleRRule);
public:
    static void registerMetaType();
    //日程信息调试打印
    friend QDebug operator<<(QDebug debug, const ScheduleDataInfo &scheduleJsonData);
private:
    /**
     * @brief m_ScheduleID  日程ID
     */
    int         m_ScheduleID;
    /**
     * @brief m_ScheduleBeginDateTime   日常开始时间
     */
    QDateTime       m_ScheduleBeginDateTime;
    /**
     * @brief m_ScheduleEndDateTime 日程结束时间
     */
    QDateTime       m_ScheduleEndDateTime;
    /**
     * @brief m_Ignore_VectorTime   重复日程忽略时间集
     */
    QVector<QDateTime>      m_IgnoreVectorTime;
    /**
     * @brief m_ScheudleTitleName   日程标题
     */
    QString     m_ScheudleTitleName;
    /**
     * @brief m_ScheudleDescription 日程描述
     */
    QString     m_ScheudleDescription;
    /**
     *@brief m_ScheduleIsAllDay     是否为全天日程
     * true  表示全天
     * false    表示非全天
     */
    bool        m_ScheduleIsAllDay;  //1全天
    /**
     * @brief m_ScheudleType    日程类型
     * 1 工作
     * 2 生活
     * 3 其他
     * 4 节日信息
     */
    int         m_ScheudleType;    //1工作 2 生活 3其他
    /**
     * @brief m_ScheudleRecurID     日程重复id类型
     * 0 原始数据
     * >0   克隆数据
     */
    int         m_ScheudleRecurID;
    /**
     * @brief m_ScheduleRemind      提醒规则
     */
    RemindData      m_ScheduleRemind;
    /**
     * 日程重复规则
     * 具体规则可参考资料          https://www.rfc-editor.org/rfc/rfc5545.html#page-38
     *                          3.3.10.  Recurrence Rule
     * @brief m_Rule         重复规则
     */
    RepetitionRule  m_ScheduleRRule;
    //客户端使用判断是否为拖拽移动日程
    bool            m_moveInfo{false};
};
Q_DECLARE_METATYPE(ScheduleDataInfo);
#endif // SCHEDULEJSONDATA_H
