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
#include "calendarscheduler.h"
#include "src/commondatastruct.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

CalendarScheduler::CalendarScheduler(QObject *parent)
    : QObject(parent)
    , m_database(new SchedulerDatabase(this))
{
}

QString CalendarScheduler::GetType(qint64 id)
{
    QString strres;
    foreach (auto var, globalPredefinedTypes) {
        if (var.ID == id) {
            //tojson
            QJsonObject obj;
            obj.insert("ID", static_cast<int>(var.ID));
            obj.insert("Name", var.Name);
            obj.insert("Color", var.Color);
            QJsonDocument doc;
            doc.setObject(obj);
            strres = QString::fromUtf8(doc.toJson());
            break;
        }
    }
    return strres;
}

QString CalendarScheduler::GetTypes()
{
    QString strres;
    QJsonArray jsonArray;
    QJsonDocument doc;
    foreach (auto var, globalPredefinedTypes) {
        QJsonObject obj;
        obj.insert("ID", static_cast<int>(var.ID));
        obj.insert("Name", var.Name);
        obj.insert("Color", var.Color);
        jsonArray.append(obj);
    }
    doc.setArray(jsonArray);
    strres = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
    return strres;
}

void CalendarScheduler::DeleteJob(qint64 id)
{
    m_database->DeleteJob(id);
}

void CalendarScheduler::DeleteType(qint64 id)
{
    m_database->DeleteType(id);
}


QString CalendarScheduler::GetJob(qint64 id)
{
    return m_database->GetJob(id);
}

// 给定日程Json信息，解析为job类型传入数据库
qint64 CalendarScheduler::CreateJob(const QString &jobInfo)
{
    // 现将给的Json信息转为Job类型
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jobInfo.toLocal8Bit(), &json_error));

    if (json_error.error != QJsonParseError::NoError) {
        return false;
    }

    QJsonObject rootObj = jsonDoc.object();
    Job job;
    if(rootObj.contains("ID")) {
        job.ID = rootObj.value("ID").toInt();
    }
    if(rootObj.contains("Type")) {
        job.Type = rootObj.value("Type").toInt();
    }
    if (rootObj.contains("Title")) {
        job.Title = rootObj.value("Title").toString();
    }
    if (rootObj.contains("Description")) {
        job.Description = rootObj.value("Description").toString();
    }
    if (rootObj.contains("AllDay")) {
        job.AllDay = rootObj.value("AllDay").toBool();
    }
    if (rootObj.contains("Start")) {
        QStringList liststr = rootObj.value("Start").toString().split("+", QString::SkipEmptyParts);
        // 此处时间转换为与client同样式
        job.Start = QDateTime::fromString(liststr.at(0), "yyyy-MM-ddThh:mm:ss");
    }
    if (rootObj.contains("End")) {
        QStringList liststr = rootObj.value("End").toString().split("+", QString::SkipEmptyParts);
        job.End = QDateTime::fromString(liststr.at(0), "yyyy-MM-ddThh:mm:ss");
    }
    if (rootObj.contains("RRule")) {
        job.RRule = rootObj.value("RRule").toString();
    }
    if (rootObj.contains("Remind")) {
        job.Remind = rootObj.value("Remind").toString();
    }
    if (rootObj.contains("Ignore")) {
        QJsonArray subArray = rootObj.value("Ignore").toArray();
        QJsonDocument doc;
        doc.setArray(subArray);
        job.Ignore = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
    }
    // 暂不使用此字段，挂起
    job.Title_pinyin = "";

    return m_database->CreateJob(job);
}

// 可将要改动的日程信息直接传入数据库操作层中
void CalendarScheduler::UpdateJob(const QString &jobInfo)
{
    m_database->UpdateJob(jobInfo);
}

// 可将要改动的日程类型信息直接传入数据库操作层中
void CalendarScheduler::UpdateType(const QString &typeInfo)
{
    m_database->UpdateType(typeInfo);
}
