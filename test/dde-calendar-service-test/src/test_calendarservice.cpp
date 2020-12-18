/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     hejinghai <hejinghai@uniontech.com>
*
* Maintainer: hejinghai <hejinghai@uniontech.com>
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
#include "test_calendarservice.h"

test_calendarservice::test_calendarservice()
{
    //service = new CalendarService();
}

test_calendarservice::~test_calendarservice()
{
    delete service;
}

////QString CalendarService::GetFestivalMonth(quint32 year, quint32 month)
//TEST_F(test_calendarservice, GetFestivalMonth)
//{
//    QString festivalMonth = service->GetFestivalMonth(2020, 10);
//    qInfo() << festivalMonth;
//    assert(1==1);
//}

////QString CalendarService::GetType(qint64 id)
//TEST_F(test_calendarservice, GetType)
//{
//    QString Type = service->GetType(1);
//    qInfo() << Type;
//    assert(1==1);
//}
