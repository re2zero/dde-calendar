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
#ifndef HUANGLIDATABASE_H
#define HUANGLIDATABASE_H
#include "src/lunardatastruct.h"

#include <QObject>
#include <QSqlDatabase>

class HuangLiDataBase : public QObject
{
    Q_OBJECT
public:
    explicit HuangLiDataBase(QObject *parent = nullptr);
    QList<stFestival> QueryFestivalList(const QString &table, quint8 month);

signals:

public slots:

private:
    QSqlDatabase m_database;
};

#endif // HUANGLIDATABASE_H
