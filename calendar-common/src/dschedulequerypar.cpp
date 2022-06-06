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
#include "dschedulequerypar.h"

#include "units.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

DScheduleQueryPar::DScheduleQueryPar()
    : m_key("")
    , m_queryTop(1)
    , m_rruleType(RRule_None)
    , m_queryType(Query_None)
{
}

QDateTime DScheduleQueryPar::dtStart() const
{
    return m_dtStart;
}

void DScheduleQueryPar::setDtStart(const QDateTime &dtStart)
{
    m_dtStart = dtStart;
}

QDateTime DScheduleQueryPar::dtEnd() const
{
    return m_dtEnd;
}

void DScheduleQueryPar::setDtEnd(const QDateTime &dtEnd)
{
    m_dtEnd = dtEnd;
}

QString DScheduleQueryPar::key() const
{
    return m_key;
}

void DScheduleQueryPar::setKey(const QString &key)
{
    m_key = key;
}

DScheduleQueryPar::Ptr DScheduleQueryPar::fromJsonString(const QString &queryStr)
{
    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(queryStr.toLocal8Bit(), &jsonError));
    if (jsonError.error != QJsonParseError::NoError) {
        qWarning() << "error:" << jsonError.errorString();
        return nullptr;
    }
    DScheduleQueryPar::Ptr queryPar = DScheduleQueryPar::Ptr(new DScheduleQueryPar);
    QJsonObject rootObj = jsonDoc.object();
    if (rootObj.contains("key")) {
        queryPar->setKey(rootObj.value("key").toString());
    }
    if (rootObj.contains("dtStart")) {
        queryPar->setDtStart(dtFromString(rootObj.value("dtStart").toString()));
    }
    if (rootObj.contains("dtEnd")) {
        queryPar->setDtEnd(dtFromString(rootObj.value("dtEnd").toString()));
    }
    QueryType qType = Query_None;
    if (rootObj.contains("queryType")) {
        qType = static_cast<QueryType>(rootObj.value("queryType").toInt());
        queryPar->setQueryType(qType);
    }
    switch (qType) {
    case Query_Top: {
        if (rootObj.contains("queryTop")) {
            queryPar->setQueryTop(rootObj.value("queryTop").toInt());
        }
    } break;
    case Query_RRule: {
        if (rootObj.contains("queryRRule")) {
            queryPar->setRruleType(static_cast<RRuleType>(rootObj.value("queryRRule").toInt()));
        }
    }
    default:
        break;
    }
    return queryPar;
}

QString DScheduleQueryPar::toJsonString(const DScheduleQueryPar::Ptr &queryPar)
{
    if (queryPar.isNull()) {
        qWarning() << "hold a reference to a null pointer.";
        return QString();
    }
    QJsonObject jsonObj;
    jsonObj.insert("key", queryPar->key());
    jsonObj.insert("dtStart", dtToString(queryPar->dtStart()));
    jsonObj.insert("dtEnd", dtToString(queryPar->dtEnd()));
    jsonObj.insert("queryType", queryPar->queryType());
    switch (queryPar->queryType()) {
    case Query_Top:
        jsonObj.insert("queryTop", queryPar->queryTop());
        break;
    case Query_RRule:
        jsonObj.insert("queryRRule", queryPar->rruleType());
        break;
    default:
        break;
    }
    QJsonDocument jsonDoc;
    jsonDoc.setObject(jsonObj);
    return QString::fromUtf8(jsonDoc.toJson(QJsonDocument::Compact));
}

DScheduleQueryPar::QueryType DScheduleQueryPar::queryType() const
{
    return m_queryType;
}

void DScheduleQueryPar::setQueryType(const QueryType &queryType)
{
    m_queryType = queryType;
}

int DScheduleQueryPar::queryTop() const
{
    return m_queryTop;
}

void DScheduleQueryPar::setQueryTop(int queryTop)
{
    m_queryTop = queryTop;
}

DScheduleQueryPar::RRuleType DScheduleQueryPar::rruleType() const
{
    return m_rruleType;
}

void DScheduleQueryPar::setRruleType(const RRuleType &rruleType)
{
    m_rruleType = rruleType;
}