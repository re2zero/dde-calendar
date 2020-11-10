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

// 直接给DB传入id执行删除命令 暂无返回值
void CalendarScheduler::DeleteJob(qint64 id)
{
    m_database->DeleteJob(id);
}
