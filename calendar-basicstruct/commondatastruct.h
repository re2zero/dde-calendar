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
#ifndef COMMONDATASTRUCT_H
#define COMMONDATASTRUCT_H

#include <QtCore/QDebug>
#include <QtDBus/QtDBus>

class CaYearInfo
{
public:
    CaYearInfo() {}

    inline static void registerMetaType();

    friend QDebug operator<<(QDebug argument, const CaYearInfo &what);
    friend QDBusArgument &operator<<(QDBusArgument &argument, const CaYearInfo &what);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument, CaYearInfo &what);

public:
    qint32 mYear;
    qint32 mMonth;
    qint32 mDay;
};

Q_DECLARE_METATYPE(CaYearInfo)

#endif // COMMONDATASTRUCT_H
