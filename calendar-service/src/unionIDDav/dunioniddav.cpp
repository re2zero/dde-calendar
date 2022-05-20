// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dunioniddav.h"

DUnionIDDav::DUnionIDDav()
{
}

void DUnionIDDav::setAccount(const DAccount::Ptr &account)
{
}

DAccount::Ptr DUnionIDDav::getAccount() const
{
}

void DUnionIDDav::setScheduleType(const DScheduleType::List &typeList)
{
}

DScheduleType::List DUnionIDDav::getScheduleTypes()
{
}

void DUnionIDDav::uploadSchedule(const DSchedule::Ptr &schedule)
{
}

DSchedule::List DUnionIDDav::downloadSchedule()
{
}

void DUnionIDDav::deleteScheduleByPath(const QStringList &schedulePath)
{
}

void DUnionIDDav::setCalendarGeneralSettings(const DCalendarGeneralSettings &cgSet)
{
}

void DUnionIDDav::createAccountFile(const QString &fileName, const QString &content)
{
}

void DUnionIDDav::createTypeFile(const QString &fileName, const QString &content)
{
}

void DUnionIDDav::createCalendarFile(const QString &fileName, const QString &content)
{
}

void DUnionIDDav::deleteFolder(const QString &path)
{
}

void DUnionIDDav::createFolder(const QString &path)
{
}

bool DUnionIDDav::exists(const QString &path)
{
}
