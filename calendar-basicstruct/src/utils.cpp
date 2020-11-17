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

Utils::Utils()
{
}

QString Utils::toconvertData(QDateTime date)
{
    QDateTime datetimeutc11 = date;
    datetimeutc11.setTimeSpec(Qt::UTC);
    QString strss = datetimeutc11.toString(Qt::ISODate);
    datetimeutc11.setTimeSpec(Qt::OffsetFromUTC);
    strss = datetimeutc11.toString(Qt::ISODateWithMs);
    QDateTime datetimeutc = QDateTime::fromTime_t(0);
    QString str = date.toString("yyyy-MM-ddThh:mm:ss") + "+" + datetimeutc.toString("hh:mm");

    return str;
}

QDateTime Utils::fromconvertData(QString str)
{
    QStringList liststr = str.split("+", QString::SkipEmptyParts);

    return QDateTime::fromString(liststr.at(0), "yyyy-MM-ddThh:mm:ss");
}

QString Utils::toconvertIGData(QDateTime date)
{
    QDateTime datetimeutc11 = date;
    datetimeutc11.setTimeSpec(Qt::UTC);
    QString strss = datetimeutc11.toString(Qt::ISODate);
    datetimeutc11.setTimeSpec(Qt::OffsetFromUTC);
    strss = datetimeutc11.toString(Qt::ISODateWithMs);
    QDateTime datetimeutc = QDateTime::fromTime_t(0);
    QString str = date.toString("yyyy-MM-ddThh:mm:ss") + "Z" + datetimeutc.toString("hh:mm");

    return str;
}

QDateTime Utils::fromconvertiIGData(QString str)
{
    QStringList liststr = str.split("Z", QString::SkipEmptyParts);
    return QDateTime::fromString(liststr.at(0), "yyyy-MM-ddThh:mm:ss");
}
