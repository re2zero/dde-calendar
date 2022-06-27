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
#include "utils.h"

#include <QJsonDocument>
#include <QJsonArray>

Utils::Utils()
{
}

QString Utils::toconvertData(QDateTime date)
{
    QDateTime datetimeutc = QDateTime::fromTime_t(0);
    QString str = date.toString("yyyy-MM-ddThh:mm:ss") + "+" + datetimeutc.toString("hh:mm");
    return str;
}

QDateTime Utils::fromconvertData(QString str)
{
    QStringList liststr = str.split("+", QString::SkipEmptyParts);

    return QDateTime::fromString(liststr.at(0), "yyyy-MM-ddThh:mm:ss");
}

QDateTime Utils::fromconvertiIGData(QString str)
{
    QStringList liststr = str.split("Z", QString::SkipEmptyParts);
    return QDateTime::fromString(liststr.at(0), "yyyy-MM-ddThh:mm:ss");
}

/**
 * @brief  JobToObject 将Job转换成QJsonObject
 * @param job Job结构体
 * @return QJsonObject
 */
QJsonObject Utils::JobToObject(const Job &job)
{
    QJsonObject obj;
    obj.insert("ID", job.ID);
    obj.insert("Type", job.Type);
    obj.insert("Title", job.Title);
    obj.insert("Description", job.Description);
    obj.insert("AllDay", job.AllDay);
    obj.insert("Start", Utils::toconvertData(job.Start));
    obj.insert("End", Utils::toconvertData(job.End));
    obj.insert("RRule", job.RRule);
    obj.insert("Remind", job.Remind);
    //将QString类型转换为QJsonArray类型，方便前端解析
    obj.insert("Ignore", QJsonDocument::fromJson(job.Ignore.toUtf8()).array());
    obj.insert("RecurID", job.RecurID);
    obj.insert("IsLunar", job.IsLunar);

    return obj;
}
/**
 * @brief  StringToObject 将JSON格式字符串转换成QJsonObject
 * @param  str JSON格式字符
 * @param  ssubObj 出参，QJsonObject
 * @return 是否正确
 */
bool Utils::StringToObject(const QString &jsonStr, QJsonArray &ja)
{
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonStr.toLocal8Bit(), &json_error));

    if (json_error.error != QJsonParseError::NoError) {
        return false;
    }
    ja = jsonDoc.array();
    return true;
}

