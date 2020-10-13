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
#include "jsondata.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonDocument>

#include "../globaldef.h"

JsonData::JsonData()
{
}

JsonData::~JsonData()
{
}

void JsonData::JosnResolve(const QJsonObject &jsObj)
{
    setDefaultValue();
    if (jsObj.contains("slots") && jsObj["slots"].isArray()) {
        auto slotsObjArr = jsObj["slots"].toArray();
        for (int j = 0; j < slotsObjArr.size(); ++j) {
            auto slotsObj = slotsObjArr[j].toObject();
            if (slotsObj.contains(JSON_NAME) && slotsObj[JSON_NAME].isString()) {
                if (slotsObj[JSON_NAME] == JSON_CONTENT) {
                    contentJsonResolve(slotsObj);
                    continue;
                }
                if (slotsObj[JSON_NAME] == JSON_REPEAT) {
                    repeatJsonResolve(slotsObj);
                    continue;
                }
                if (slotsObj[JSON_NAME] == JSON_DATETIME) {
                    datetimeJsonResolve(slotsObj);
                    continue;
                }
                if (slotsObj[JSON_NAME] == JSON_PROPERTY) {
                    propertyJsonResolve(slotsObj);
                    continue;
                }
                if (slotsObj[JSON_NAME] == JSON_POSRANK_OFFSET) {
                    posRankOffsetResolve(slotsObj);
                    continue;
                }
                jsonObjResolve(slotsObj);
            }
        }
    }
}

QString JsonData::SuggestMsg() const
{
    return m_SuggestMsg;
}

void JsonData::setSuggestMsg(const QString &SuggestMsg)
{
    m_SuggestMsg = SuggestMsg;
}

bool JsonData::ShouldEndSession() const
{
    return m_ShouldEndSession;
}

void JsonData::setShouldEndSession(bool ShouldEndSession)
{
    m_ShouldEndSession = ShouldEndSession;
}

void JsonData::setRepeatStatus(const JsonData::RepeatStatus &RepeatStatus)
{
    m_RepeatStatus = RepeatStatus;
}

QString JsonData::TitleName() const
{
    return m_TitleName;
}

void JsonData::setTitleName(const QString &TitleName)
{
    m_TitleName = TitleName;
}

void JsonData::setDateTimeStatus(const JsonData::DateTimeStatus &DateTimeStatus)
{
    m_DateTimeStatus = DateTimeStatus;
}

JsonData::RepeatStatus JsonData::getRepeatStatus() const
{
    return m_RepeatStatus;
}

QVector<int> JsonData::getRepeatNum() const
{
    return m_RepeatNum;
}

void JsonData::setRepeatNum(const QVector<int> &RepeatNum)
{
    m_RepeatNum = RepeatNum;
}

SemanticsDateTime JsonData::getDateTime() const
{
    return m_DateTime;
}

void JsonData::setDateTime(const SemanticsDateTime &DateTime)
{
    m_DateTime = DateTime;
}

void JsonData::contentJsonResolve(const QJsonObject &jsobj)
{
    setTitleName(jsobj[JSON_VALUE].toString());
}

void JsonData::repeatJsonResolve(const QJsonObject &jsobj)
{
    QString repeatValue = jsobj[JSON_VALUE].toString();
    if (repeatValue.contains("EVERYEAR")) {
        setRepeatStatus(EVEY);
        return;
    }
    if (repeatValue.contains("EVERYDAY")) {
        setRepeatStatus(EVED);
        return;
    }
    if (repeatValue.contains("WORKDAY")) {
        setRepeatStatus(WORKD);
        return;
    }
    if (repeatValue.contains("RESTDAY")) {
        setRepeatStatus(RESTD);
        return;
    }
    if (repeatValue.contains("WEEKEND")) {
        setRepeatStatus(RESTD);
        return;
    }

    QRegExp rxlen("([W,M])([0-9]{0,2})");
    int pos = 0;
    QVector<int> repeatnum {};
    repeatnum.clear();
    while ((pos = rxlen.indexIn(repeatValue, pos)) != -1) {
        if (rxlen.cap(1).contains("M")) {
            setRepeatStatus(EVEM);
        }
        if (rxlen.cap(1).contains("W")) {
            setRepeatStatus(EVEW);
        }
        if (rxlen.cap(0).size() > 0 && rxlen.cap(2) != "") {
            repeatnum.append(rxlen.cap(2).toInt());
        }
        pos += rxlen.matchedLength();
    }
    setRepeatNum(repeatnum);
}

void JsonData::datetimeJsonResolve(const QJsonObject &jsobj)
{
    setDateTime(suggestDatetimeResolve(jsobj));
}

void JsonData::propertyJsonResolve(const QJsonObject &jsobj)
{
    QString propertyValue = jsobj[JSON_VALUE].toString();
    if (propertyValue == JSON_NEXT) {
        setPropertyStatus(NEXT);
        return;
    }
    if (propertyValue == JSON_LAST) {
        setPropertyStatus(LAST);
        return;
    }
    if (propertyValue == JSON_ALL) {
        setPropertyStatus(ALL);
        return;
    }
    if (propertyValue == JSON_THIS) {
        setPropertyStatus(PRO_THIS);
        return;
    }
}

void JsonData::posRankOffsetResolve(const QJsonObject &jsobj)
{
    setOffset(jsobj[JSON_VALUE].toString().toInt());
}

SemanticsDateTime JsonData::suggestDatetimeResolve(const QJsonObject &jsobj)
{
    SemanticsDateTime semdatetime {};
    semdatetime.clear();
    QString DateTimeJson = jsobj[JSON_NORMVALUE].toString();
    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(DateTimeJson.toUtf8(), &jsonError);
    //建议时间
    QString sugdateTimeStr{""};
    //模糊时间
    QString dateTimeStr{""};

    if (!doc.isNull() && (jsonError.error == QJsonParseError::NoError)) {
        auto rootObject = doc.object();
        sugdateTimeStr = (rootObject["suggestDatetime"].toString());
        dateTimeStr = rootObject["datetime"].toString();
    }
    //建议时间解析赋值
    if (sugdateTimeStr != "") {
        if (sugdateTimeStr.contains("/")) {
            QStringList dateTimeList = sugdateTimeStr.split("/");
            for (int i = 0; i < dateTimeList.size(); ++i) {
                //如果数据不为空，添加解析数据
                if(!dateTimeList.at(i).isEmpty()){
                    semdatetime.suggestDatetime.append(resolveNormValue(dateTimeList.at(i)));
                }
            }
        } else {
            semdatetime.suggestDatetime.append(resolveNormValue(sugdateTimeStr));
        }
    }
    //模糊时间解析赋值
    if(!dateTimeStr.isEmpty()){
        if(dateTimeStr.contains("/")){
            QStringList dateTimeList = dateTimeStr.split("/");
            for (int i = 0; i < dateTimeList.size(); ++i) {
                //如果数据不为空，添加解析数据
                if(!dateTimeList.at(i).isEmpty()){
                    semdatetime.dateTime.append(resolveDateTimeValeu(dateTimeList.at(i)));
                }
            }
        } else {
            semdatetime.dateTime.append(resolveDateTimeValeu(dateTimeStr));
        }
    }
    return semdatetime;
}

SuggestDatetimeInfo JsonData::resolveNormValue(const QString &str)
{
    SuggestDatetimeInfo datetimeInfo;
    datetimeInfo.hasTime = str.contains(JSON_DATETIME_DELIMITER);
    datetimeInfo.datetime = QDateTime::fromString(str, Qt::ISODate);
    return datetimeInfo;
}

DateTimeInfo JsonData::resolveDateTimeValeu(const QString &dateTimeStr)
{
    DateTimeInfo dateTimeInfo{};
    //根据关键符合“T”分割字符
    QStringList dateList = dateTimeStr.split(JSON_DATETIME_DELIMITER);
    //如果只有一个表示没有时间信息
    if(dateList.size() ==1){
        dateTimeInfo.m_Date = QDate::fromString(dateList.at(0),DATEFORMAT);
        dateTimeInfo.hasDate = true;
        dateTimeInfo.hasTime = false;
    }else if (dateList.size()>1) {
        //如果第一个数据为空表示没有日期信息
        if(dateList.at(0).isEmpty()){
            dateTimeInfo.hasDate = false;
        }else {
            dateTimeInfo.m_Date = QDate::fromString(dateList.at(0),DATEFORMAT);
            dateTimeInfo.hasDate = true;
        }
        dateTimeInfo.m_Time = QTime::fromString(dateList.at(1),TIMEFORMAT);
        dateTimeInfo.hasTime = true;
    }
    return dateTimeInfo;
}

void JsonData::setDefaultValue()
{
    setTitleName("");
    setDateTimeStatus(NODATE);
    setRepeatStatus(NONE);
    setPropertyStatus(PRO_NONE);
    m_RepeatNum.clear();
    m_DateTime.clear();
    setOffset(-1);
}

void JsonData::jsonObjResolve(const QJsonObject &jsobj)
{
    Q_UNUSED(jsobj);
}

JsonData::PropertyStatus JsonData::getPropertyStatus() const
{
    return m_propertyStatus;
}

void JsonData::setPropertyStatus(const PropertyStatus &propertyStatus)
{
    m_propertyStatus = propertyStatus;
}

int JsonData::offset() const
{
    return m_offset;
}

void JsonData::setOffset(int offset)
{
    m_offset = offset;
}

bool JsonData::isVaild() const
{
    return TitleName() == "" && getDateTime().suggestDatetime.size() == 0
           && m_offset == -1 && m_RepeatStatus == NONE && m_propertyStatus == PRO_NONE;
}
