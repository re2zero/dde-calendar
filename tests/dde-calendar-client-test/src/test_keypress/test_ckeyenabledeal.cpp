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
#include "test_ckeyenabledeal.h"

#include "../third-party_stub/stub.h"

#include "view/graphicsItem/cweekdaybackgrounditem.h"
#include "../dialog_stub.h"
#include "view/graphicsItem/scheduleitem.h"

#include <QGraphicsView>

test_CKeyEnableDeal::test_CKeyEnableDeal()
{
}

void test_CKeyEnableDeal::SetUp()
{
    delete SceneCurrentItem;
    SceneCurrentItem = new CSceneBackgroundItem(CSceneBackgroundItem::OnMonthView);
    calendarDDialogExecStub(stub.getStub());
    scene = QSharedPointer<CGraphicsScene>(new CGraphicsScene());
    view = QSharedPointer<QGraphicsView>(new QGraphicsView());
    view->setScene(scene.get());
    enableDeal = QSharedPointer<CKeyEnableDeal>(new CKeyEnableDeal(scene.get()));
}

void test_CKeyEnableDeal::TearDown()
{
}

TEST_F(test_CKeyEnableDeal, focusItemDeal_Back)
{
    enableDeal->dealEvent();
}

TEST_F(test_CKeyEnableDeal, focusItemDeal_Back_addDay)
{
    itemDate = QDate::currentDate().addDays(1);
    enableDeal->dealEvent();
}

CFocusItem *focusItem = nullptr;

CFocusItem *getFocusItem_stub()
{
    return focusItem;
}

TEST_F(test_CKeyEnableDeal, focusItemDeal_Item)
{
    QRectF rect(0, 0, 100, 100);
    focusItem = new CScheduleItem(rect);
    stub.getStub().set(ADDR(CSceneBackgroundItem, getFocusItem), getFocusItem_stub);
    focusItemType = CFocusItem::CITEM;
    enableDeal->dealEvent();
    delete focusItem;
    focusItem = nullptr;
}

TEST_F(test_CKeyEnableDeal, focusItemDeal_Other)
{
    focusItemType = CFocusItem::COTHER;
    enableDeal->dealEvent();
}
