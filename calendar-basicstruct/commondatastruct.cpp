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
#include "commondatastruct.h"

void CaYearInfo::registerMetaType()
{
    qRegisterMetaType<CaYearInfo>();
    qDBusRegisterMetaType<CaYearInfo>();
}

QDebug operator<<(QDebug argument, const CaYearInfo &what)
{
    argument << what.mYear << what.mMonth << what.mDay;
    return argument;
}

QDBusArgument &operator<<(QDBusArgument &argument, const CaYearInfo &what)
{
    argument.beginStructure();
    argument << what.mYear << what.mMonth << what.mDay;
    argument.endStructure();

    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, CaYearInfo &what)
{
    argument.beginStructure();
    argument >> what.mYear >> what.mMonth >> what.mDay;
    argument.endStructure();

    return argument;
}
