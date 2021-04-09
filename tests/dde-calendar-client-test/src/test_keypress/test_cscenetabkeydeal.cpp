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
#include "test_cscenetabkeydeal.h"

#include "../third-party_stub/stub.h"
#include "gtest/gtest.h"
#include "view/cgraphicsscene.h"
#include "KeyPress/cscenetabkeydeal.h"
#include "view/graphicsItem/cweekdaybackgrounditem.h"
#include "keypressstub.h"

test_CSceneTabKeyDeal::test_CSceneTabKeyDeal(QObject *parent)
    : QObject(parent)
{
}

TEST(SceneTabHandle_test, test_CSceneTabKeyDeal)
{
    KeyPressStub stub;
    CGraphicsScene *scene = new CGraphicsScene();
    CSceneTabKeyDeal tabDeal(scene);
    tabDeal.dealEvent();
    delete scene;
}
