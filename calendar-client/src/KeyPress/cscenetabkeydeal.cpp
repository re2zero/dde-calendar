/*
   * Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
   *
   * Author:     chenhaifeng <chenhaifeng@uniontech.com>
   *
   * Maintainer: chenhaifeng <chenhaifeng@uniontech.com>
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
#include "cscenetabkeydeal.h"

#include "graphicsItem/cscenebackgrounditem.h"
#include "cgraphicsscene.h"

#include <QDebug>
#include <QGraphicsView>

CSceneTabKeyDeal::CSceneTabKeyDeal(QGraphicsScene *scene)
    : CKeyPressDealBase(Qt::Key_Tab, scene)
{
}

bool CSceneTabKeyDeal::focusItemDeal(CSceneBackgroundItem *item, CGraphicsScene *scene)
{
    CSceneBackgroundItem *nextItem = qobject_cast<CSceneBackgroundItem *>(item->setNextItemFocusAndGetNextItem());
    if (nextItem == nullptr) {
        scene->setCurrentFocusItem(nullptr);
        item->setItemFocus(false);
        return false;
    } else {
        CFocusItem *focusItem = nextItem->getFocusItem();
        //如果当前焦点显示不为背景则定位到当前焦点item位置
        if (focusItem->getItemType() != CFocusItem::CBACK) {
            QGraphicsView *view = scene->views().at(0);
            QPointF point(scene->width() / 2, focusItem->rect().y());
            view->centerOn(point);
        }
        scene->setCurrentFocusItem(nextItem);
        return true;
    }
}
