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
#include "huanglidatabase.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

HuangLiDataBase::HuangLiDataBase(QObject *parent)
    : QObject(parent)
{
    QString dbpath("/usr/share/dde-api/data/huangli.db");
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName(dbpath);
    m_database.open();
    Q_ASSERT(m_database.isOpen());
}

QList<stFestival> HuangLiDataBase::QueryFestivalList(const QString &table, quint8 month)
{
    QString strsql = QString("SELECT id,month,name,description,rest,list FROM %1 WHERE month = %2").arg(table, month);
    QSqlQuery query(strsql, m_database);
    if (query.exec()) {
    }
    while (query.next()) {
    }
}
