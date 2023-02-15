/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     chenhaifeng  <chenhaifeng@uniontech.com>
*
* Maintainer: chenhaifeng  <chenhaifeng@uniontech.com>
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
#include "test_calldaykeyleftdeal.h"

#include "../third-party_stub/stub.h"
#include "gtest/gtest.h"
#include "view/cgraphicsscene.h"
#include "KeyPress/calldaykeyleftdeal.h"
#include "view/graphicsItem/cweekdaybackgrounditem.h"
#include "keypressstub.h"

test_CAllDayKeyLeftDeal::test_CAllDayKeyLeftDeal(QObject *parent)
    : QObject(parent)
{
}

TEST(LeftHandle_test, test_CAllDayKeyLeftDeal)
{
    KeyPressStub stub;
    CGraphicsScene *scene = new CGraphicsScene();
    CAllDayKeyLeftDeal alldayLeftDeal(scene);
    alldayLeftDeal.dealEvent();
    delete scene;
}
