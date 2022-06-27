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
#ifndef TEST_CALENDARSERVICE_H
#define TEST_CALENDARSERVICE_H

#include "calendarservice.h"
#include <QObject>
#include <gtest/gtest.h>

class test_calendarservice: public::testing::Test
{
public:
    test_calendarservice();

    virtual void SetUp()
    {
        mService = new CalendarService();
    }

    virtual void TearDown()
    {
        delete mService;
        mService = nullptr;
    }
protected:
    CalendarService *mService = nullptr;
};

#endif // TEST_CALENDARSERVICE_H
