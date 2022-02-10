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
#include "scheduledatainfo.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QTimeZone>
#include <QDebug>

ScheduleDataInfo::ScheduleDataInfo()
    : m_ScheduleID(0)
    , m_ScheduleBeginDateTime()
    , m_ScheduleEndDateTime()
    , m_IgnoreVectorTime()
    , m_ScheduleTitleName("")
    , m_ScheduleDescription("")
    , m_ScheduleIsAllDay(true)
    , m_ScheduleType(1)
    , m_ScheduleRecurID(0)
    , m_ScheduleRRule()
{

}

/**
 * @brief ScheduleID    获取日程ID
 * @return      日程ID
 */
int ScheduleDataInfo::getID() const
{
    return m_ScheduleID;
}

/**
 * @brief setScheduleID             设置日程ID
 * @param scheduleID                日程id
 */
void ScheduleDataInfo::setID(int scheduleID)
{
    m_ScheduleID = scheduleID;
}

/**
 * @brief ScheduleBeginDateTime     获取日程开始时间
 * @return                          日程开始时间
 */
QDateTime &ScheduleDataInfo::getBeginDateTime()
{
    return m_ScheduleBeginDateTime;
}

QDateTime ScheduleDataInfo::getBeginDateTime() const
{
    return m_ScheduleBeginDateTime;
}

/**
 * @brief setScheduleBeginDateTime      设置日程开始时间
 * @param ScheduleBeginDateTime         日程开始时间
 */
void ScheduleDataInfo::setBeginDateTime(const QDateTime &beginDateTime)
{
    m_ScheduleBeginDateTime = beginDateTime;
}

/**
 * @brief ScheduleEndDateTime       获取日程结束时间
 * @return                          日程结束时间
 */
QDateTime &ScheduleDataInfo::getEndDateTime()
{
    return m_ScheduleEndDateTime;
}

QDateTime ScheduleDataInfo::getEndDateTime() const
{
    return m_ScheduleEndDateTime;
}

/**
 * @brief setScheduleEndDateTime    设置日程结束时间
 * @param ScheduleEndDateTime       日程结束时间
 */
void ScheduleDataInfo::setEndDateTime(const QDateTime &endDateTime)
{
    m_ScheduleEndDateTime = endDateTime;
}

QVector<QDateTime> &ScheduleDataInfo::getIgnoreTime()
{
    return m_IgnoreVectorTime;
}

/**
 * @brief IgnoreVectorTime          获取日程重复忽略时间集
 * @return                          日程重复忽略时间集
 */
QVector<QDateTime> ScheduleDataInfo::getIgnoreTime() const
{
    return m_IgnoreVectorTime;
}

/**
 * @brief setIgnoreVectorTime       设置日程重复忽略时间集
 * @param IgnoreVectorTime          日程重复忽略时间集
 */
void ScheduleDataInfo::setIgnoreTime(const QVector<QDateTime> &ignoreVectorTime)
{
    m_IgnoreVectorTime = ignoreVectorTime;
}

/**
 * @brief ScheduleTitleName         获取日程标题
 * @return                          日程标题
 */
QString ScheduleDataInfo::getTitleName() const
{
    return m_ScheduleTitleName;
}

/**
 * @brief setScheduleTitleName      设置日程标题
 * @param ScheduleTitleName         日程标题
 */
void ScheduleDataInfo::setTitleName(const QString &titleName)
{
    m_ScheduleTitleName = titleName;
}

/**
 * @brief ScheduleDescription       获取日程描述信息
 * @return                          日程描述信息
 */
QString ScheduleDataInfo::getDescription() const
{
    return m_ScheduleDescription;
}

/**
 * @brief setScheduleDescription    设置日程描述信息
 * @param ScheduleDescription       日程描述信息
 */
void ScheduleDataInfo::setDescription(const QString &description)
{
    m_ScheduleDescription = description;
}

/**
 * @brief ScheduleIsAllDay      获取日程是否为全天
 * @return      是否为全天
 */
bool ScheduleDataInfo::getAllDay() const
{
    return m_ScheduleIsAllDay;
}

/**
 * @brief setScheduleIsAllDay   设置日程是否为全天
 * @param ScheduleIsAllDay      是否为全天
 */
void ScheduleDataInfo::setAllDay(const bool isAllDay)
{
    m_ScheduleIsAllDay = isAllDay;
}

/**
 * @brief ScheduleType  获取日程类型
 * @return      日程类型
 */
int ScheduleDataInfo::getType() const
{
    return m_ScheduleType;
}

/**
 * @brief setScheduleType   设置日程类型
 * @param ScheduleType      日程类型
 */
void ScheduleDataInfo::setType(int scheduleType)
{
    m_ScheduleType = scheduleType;
}

/**
 * @brief ScheduleRecurID   获取日程重复id类型
 * @return                  重复id类型
 */
int ScheduleDataInfo::getRecurID() const
{
    return m_ScheduleRecurID;
}

/**
 * @brief setRecurID            设置日程重复id类型
 * @param ScheduleRecurID       重复id类型
 */
void ScheduleDataInfo::setRecurID(int recurID)
{
    m_ScheduleRecurID = recurID;
}

/**
 * @brief setScheduleRemindData     设置日程提醒规则
 * @param remindData                提醒规则
 */
void ScheduleDataInfo::setRemindData(const RemindData &remindData)
{
    m_ScheduleRemind = remindData;
}

/**
 * @brief getScheduleRemindData     获取提醒规则
 * @return
 */
RemindData ScheduleDataInfo::getRemindData() const
{
    return m_ScheduleRemind;
}

/**
 * @brief ScheduleRemind    设置日程提醒规则
 * @return                  提醒规则
 */
QString ScheduleDataInfo::getScheduleRemind() const
{
    return RemindData::RemindDataToRemindString(m_ScheduleRemind, this->getAllDay());
}

/**
 * @brief setScheduleRemind     设置日程提醒规则
 * @param ScheduleRemind        提醒规则
 */
void ScheduleDataInfo::setScheduleRemind(const QString &scheduleRemind)
{
    m_ScheduleRemind = RemindData::RemindStringToRemindData(scheduleRemind);
}

/**
 * @brief ScheduleRRule     获取日程重复规则
 * @return                  日程重复规则
 */
QString ScheduleDataInfo::getScheduleRRule() const
{
    return m_ScheduleRRule.RuleDataToRuleString(m_ScheduleRRule);;
}

/**
 * @brief setScheduleRRule  设置日程重复规则
 * @param ScheduleRRule 日程重复规则
 */
void ScheduleDataInfo::setScheduleRRule(const QString &scheduleRRule)
{
    m_ScheduleRRule = RepetitionRule::RuleStringToRuleData(scheduleRRule);
}

void ScheduleDataInfo::registerMetaType()
{
    qRegisterMetaType<ScheduleDataInfo>("ScheduleDataInfo");
}

/**
 * @brief setRepetitionRule     设置重复规则
 * @param rule
 */
void ScheduleDataInfo::setRepetitionRule(const RepetitionRule &rule)
{
    m_ScheduleRRule = rule;
}

/**
 * @brief ScheduleDataInfo::getRepetitionRule       获取重复规则
 * @return
 */
RepetitionRule &ScheduleDataInfo::getRepetitionRule()
{
    return m_ScheduleRRule;
}

/**
 * @brief getRepetitionRule         获取重复规则
 * @return
 */
RepetitionRule ScheduleDataInfo::getRepetitionRule() const
{
    return m_ScheduleRRule;
}

/**
 * @brief ScheduleDataInfo::isValid 日程是否有效
 * @return
 */
bool ScheduleDataInfo::isValid() const
{
    if (!getBeginDateTime().isValid()) {
        return  false;
    }
    if (!getEndDateTime().isValid()) {
        return  false;
    }
    if (getType() < 1 || getType() > 3) {
        return  false;
    }
    return true;
}

/**
 * @brief ScheduleDataInfo::setIsMoveInfo       设置是否为拖拽移动日程
 * @param isMoveInfo
 */
void ScheduleDataInfo::setIsMoveInfo(const bool &isMoveInfo)
{
    m_moveInfo = isMoveInfo;
}

/**
 * @brief ScheduleDataInfo::getIsMoveInfo       获取是否为拖拽移动日程
 * @return
 */
bool ScheduleDataInfo::getIsMoveInfo() const
{
    return  m_moveInfo;
}

/**
 * @brief ScheduleToJson        日程数据转换为json形式的字符串
 * @param scheduleJsonData      日程数据
 * @return          json形式的字符串
 */
QString ScheduleDataInfo::ScheduleToJsonStr(const ScheduleDataInfo &scheduleJsonData)
{
    QJsonObject _scheduleJsonObject;
    _scheduleJsonObject.insert("ID", scheduleJsonData.getID());
    _scheduleJsonObject.insert("AllDay", scheduleJsonData.getAllDay());
    _scheduleJsonObject.insert("Remind", scheduleJsonData.getScheduleRemind());
    _scheduleJsonObject.insert("RRule", scheduleJsonData.getScheduleRRule());
    _scheduleJsonObject.insert("Title", scheduleJsonData.getTitleName());
    _scheduleJsonObject.insert("Description", scheduleJsonData.getDescription());
    _scheduleJsonObject.insert("Type", scheduleJsonData.getType());
    _scheduleJsonObject.insert("Start", DateTimeToStringDate(scheduleJsonData.getBeginDateTime()));
    _scheduleJsonObject.insert("End", DateTimeToStringDate(scheduleJsonData.getEndDateTime()));
    _scheduleJsonObject.insert("RecurID", scheduleJsonData.getRecurID());
    QJsonArray _ignoreJsonArray;
    for (int i = 0; i < scheduleJsonData.getIgnoreTime().count(); i++) {
        _ignoreJsonArray.append(DateTimeToStringDate(scheduleJsonData.getIgnoreTime().at(i)));
    }
    _scheduleJsonObject.insert("Ignore", _ignoreJsonArray);
    // 构建 JSON 文档
    QJsonDocument _scheduleJsonDocument;
    _scheduleJsonDocument.setObject(_scheduleJsonObject);
    QByteArray _scheduleByteArray = _scheduleJsonDocument.toJson(QJsonDocument::Compact);
    QString _resultJsonStr(_scheduleByteArray);
    return _resultJsonStr;
}

/**
 * @brief JsonToSchedule            Json对象转换到日程数据
 * @param ScheduleJsonObject        json对象
 * @return      日程数据
 */
ScheduleDataInfo ScheduleDataInfo::JsonToSchedule(const QJsonObject &scheduleJsonObject)
{
    ScheduleDataInfo _resultSchedule;
    //因为是预先定义好的JSON数据格式，所以这里可以这样读取
    //日程id
    if (scheduleJsonObject.contains("ID")) {
        _resultSchedule.setID(scheduleJsonObject.value("ID").toInt());
    }
    //日程是否为全天
    if (scheduleJsonObject.contains("AllDay")) {
        _resultSchedule.setAllDay(scheduleJsonObject.value("AllDay").toBool());
    }
    //日程提醒规则
    if (scheduleJsonObject.contains("Remind")) {
        _resultSchedule.setScheduleRemind(scheduleJsonObject.value("Remind").toString());
    }
    //日程标题
    if (scheduleJsonObject.contains("Title")) {
        _resultSchedule.setTitleName(scheduleJsonObject.value("Title").toString());
    }
    //日程描述
    if (scheduleJsonObject.contains("Description")) {
        _resultSchedule.setDescription(scheduleJsonObject.value("Description").toString());
    }
    //日程类型
    if (scheduleJsonObject.contains("Type")) {
        _resultSchedule.setType(scheduleJsonObject.value("Type").toInt());
    }
    //日程开始时间
    if (scheduleJsonObject.contains("Start")) {
        _resultSchedule.setBeginDateTime(StringDateToDateTime(scheduleJsonObject.value("Start").toString()));
    }
    //日程结束时间
    if (scheduleJsonObject.contains("End")) {
        _resultSchedule.setEndDateTime(StringDateToDateTime(scheduleJsonObject.value("End").toString()));
    }
    //日程重复ID
    if (scheduleJsonObject.contains("RecurID")) {
        _resultSchedule.setRecurID(scheduleJsonObject.value("RecurID").toInt());
    }
    //日程重复规则
    if (scheduleJsonObject.contains("RRule")) {
        _resultSchedule.setScheduleRRule(scheduleJsonObject.value("RRule").toString());
    }
    //重复日程忽略日期集
    if (scheduleJsonObject.contains("Ignore")) {
        QJsonArray subArray = scheduleJsonObject.value("Ignore").toArray();
        QVector<QDateTime> _ignoreDateVector;
        for (int i = 0; i < subArray.size(); i++) {
            QString subObj = subArray.at(i).toString();
            _ignoreDateVector.append(StringDateToDateTime(subObj));
        }
        _resultSchedule.setIgnoreTime(_ignoreDateVector);
    }
    return _resultSchedule;
}

/**
 * @brief ScheduleDataInfo::JsonStrToSchedule   将json转换为日程
 * @param jsonStr                               json数据
 * @return
 */
ScheduleDataInfo ScheduleDataInfo::JsonStrToSchedule(const QString &jsonStr)
{
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonStr.toLocal8Bit(), &json_error));

    if (json_error.error != QJsonParseError::NoError) {
        qWarning() << json_error.errorString();
    }
    QJsonObject ssubObj = jsonDoc.object();
    return ScheduleDataInfo::JsonToSchedule(ssubObj);
}

/**
 * @brief StringDateToDateTime      string类型的datetime数据转换为QDateTime
 * @param dateStr                   string类型的datetime数据
 * @return                          QDateTime
 */
QDateTime ScheduleDataInfo::StringDateToDateTime(const QString &dateStr)
{
    return QDateTime::fromString(dateStr, Qt::ISODate);
}

/**
 * @brief DateTimeToStringDate      QDateTime转换为string类型的datetime数据
 * @param dateTime                  需要转换的时间
 * @return                          string类型的datetime数据
 */
QString ScheduleDataInfo::DateTimeToStringDate(const QDateTime &dateTime)
{
    QTime _offsetTime = QTime(0, 0).addSecs(dateTime.timeZone().offsetFromUtc(dateTime));
    return QString("%1+%2").arg(dateTime.toString("yyyy-MM-ddThh:mm:ss")).arg(_offsetTime.toString("hh:mm"));
}

/**
 * @brief JsonArrayDataToScheduleMapData        将JsonArray数据转换为QMap存储Schedule数据
 * @param jsonArrData                           JsonArray数据
 * @return                                      QMap存储Schedule数据
 */
QMap<QDate, QVector<ScheduleDataInfo> > ScheduleDataInfo::JsonArrayDataToScheduleMapData(const QJsonArray &jsonArrData)
{
    QMap<QDate, QVector<ScheduleDataInfo> > _resultMap;
    QJsonObject _jsonObjScheduleVector;
    QVector<ScheduleDataInfo> _scheduleVector;
    QDate _date;
    for (int i = 0; i < jsonArrData.size(); i++) {
        _jsonObjScheduleVector = jsonArrData.at(i).toObject();
        _scheduleVector.clear();
        //因为是预先定义好的JSON数据格式，所以这里可以这样读取
        if (_jsonObjScheduleVector.contains("Date")) {
            _date = QDate::fromString(_jsonObjScheduleVector.value("Date").toString(), "yyyy-MM-dd");
        }
        if (_jsonObjScheduleVector.contains("Jobs")) {
            QJsonArray subArray = _jsonObjScheduleVector.value("Jobs").toArray();
            for (int j = 0; j < subArray.size(); j++) {
                QJsonObject _jsonObjectSchedule = subArray.at(j).toObject();
                _scheduleVector.append(JsonToSchedule(_jsonObjectSchedule));
            }
        }
        _resultMap[_date] = _scheduleVector;
    }
    return _resultMap;
}

QMap<QDate, QVector<ScheduleDataInfo> > ScheduleDataInfo::StrJsonToRangeInfo(const QString &jsonStr)
{
    QMap<QDate, QVector<ScheduleDataInfo> >_resultInfo {};
    QVector<ScheduleDataInfo>  _scheduleVector;
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonStr.toLocal8Bit(), &json_error));
    if (json_error.error != QJsonParseError::NoError) {
        qWarning() << json_error.errorString();
        return _resultInfo;
    }
    QJsonArray rootarry = jsonDoc.array();
    _resultInfo = JsonArrayDataToScheduleMapData(rootarry);
    return _resultInfo;
}

/**
 * @brief ScheduleDataInfo::operator == 判断日程是否相等
 * @param info
 * @return
 */
bool ScheduleDataInfo::operator ==(const ScheduleDataInfo &info) const
{
    if (info.getType() == 4) {
        //由于后端每次启动设置的节日日程id不一定相同，所以不比较日程id
        return this->getTitleName() == info.getTitleName() &&
               this->getBeginDateTime() == info.getBeginDateTime();
    } else {
        return this->getID() == info.getID() && this->getRecurID() == info.getRecurID() &&
               this->getTitleName() == info.getTitleName() /*&& this->getEndDateTime() == info.getEndDateTime()
               && this->getBeginDateTime() == info.getBeginDateTime()*/;
    }
}

/**
 * @brief ScheduleDataInfo::operator <      对比日程大小
 * @param info
 * @return
 */
bool ScheduleDataInfo::operator <(const ScheduleDataInfo &info) const
{
    if (getBeginDateTime().date() != getEndDateTime().date() &&
            info.getBeginDateTime().date() == info.getEndDateTime().date()) {
        return true;
    } else if (getBeginDateTime().date() == getEndDateTime().date() &&
               info.getBeginDateTime().date() != info.getEndDateTime().date()) {
        return false;
    } else if (getBeginDateTime().date() != getEndDateTime().date() &&
               info.getBeginDateTime().date() != info.getEndDateTime().date()) {
        if (getBeginDateTime().date() == info.getBeginDateTime().date()) {
            return getBeginDateTime().daysTo(getEndDateTime()) > info.getBeginDateTime().daysTo(info.getEndDateTime());
        }
        return getBeginDateTime().date() < info.getBeginDateTime().date();
    } else {
        if (getType() == 4) return true;
        if (info.getType() == 4) return false;
        if (getBeginDateTime() == info.getBeginDateTime()) {
            if (getTitleName() == info.getTitleName()) {
                return getID() < info.getID();
            }
            return getTitleName() < info.getTitleName();
        } else {
            return getBeginDateTime() < info.getBeginDateTime();
        }
    }
}

/**
 * @brief operator <<           日程信息调试打印
 * @param debug
 * @param scheduleJsonData
 * @return
 */
QDebug operator<<(QDebug debug, const ScheduleDataInfo &scheduleJsonData)
{
    debug << "("
          << QString("ID:") << scheduleJsonData.getID() << QString(",")
          << QString("IsAllDay:") << scheduleJsonData.getAllDay() << QString(",")
          << QString("TitleName:") << scheduleJsonData.getTitleName() << QString(",")
          << QString("BeginDateTime:") << scheduleJsonData.getBeginDateTime() << QString(",")
          << QString("EndDateTime:") << scheduleJsonData.getEndDateTime() << QString(",")
          << QString("Description:") << scheduleJsonData.getDescription() << QString(",")
          << QString("Type:") << scheduleJsonData.getType() << QString(",")
          << QString("RRule:") << scheduleJsonData.getScheduleRRule() << QString(",")
          << QString("Remind:") << scheduleJsonData.getScheduleRemind() << QString(",")
          << QString("RecurID:") << scheduleJsonData.getRecurID()
          << ")";
    return debug;
}
