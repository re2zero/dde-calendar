/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     leilong  <leilong@uniontech.com>
*
* Maintainer: leilong  <leilong@uniontech.com>
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
#ifndef TEST_SCHEDULERDATABASE_H
#define TEST_SCHEDULERDATABASE_H

#include "schedulerdatabase.h"
#include "gtest/gtest.h"
#include "../third-party_stub/stub.h"
#include <QObject>
#include <QVariant>
#include <QSqlQuery>

static bool schedulerdatabase_next = false;

static bool schedulerdatabase_stub_exec(const QString&)
{
    schedulerdatabase_next = false;
    return true;
};

static bool schedulerdatabase_stub_next()
{
    static int count = 0;
    if (!schedulerdatabase_next) {
        schedulerdatabase_next = true;
        count = 0;
    }
    count++;
    if (count >= 10) {
        schedulerdatabase_next = false;
        count = 0;
    }
    return schedulerdatabase_next;
};

static QVariant schedulerdatabase_stub_value(const QString&)
{
    return QVariant();
};

class test_schedulerdatabase: public::testing::Test
{
public:
    test_schedulerdatabase();

    virtual void SetUp()
    {
        Stub stub;
        stub.set((bool(QSqlQuery::*)(const QString&))ADDR(QSqlQuery, exec), schedulerdatabase_stub_exec);
        stub.set((QVariant(QSqlQuery::*)(const QString&)const)ADDR(QSqlQuery, value), schedulerdatabase_stub_value);
        stub.set(ADDR(QSqlQuery, next), schedulerdatabase_stub_next);
        mBase = new SchedulerDatabase();
    }

    virtual void TearDown()
    {
        delete mBase;
        mBase = nullptr;
    }
protected:
    SchedulerDatabase *mBase = nullptr;
};

#endif // TEST_SCHEDULERDATABASE_H
