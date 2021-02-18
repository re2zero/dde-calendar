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
#include "ckeyupdeal.h"

#include "graphicsItem/cscenebackgrounditem.h"
#include "cgraphicsscene.h"

#include <QDebug>

CKeyUpDeal::CKeyUpDeal(QGraphicsScene *scene)
    : CKeyPressDealBase(Qt::Key_Up, scene)
{
}

bool CKeyUpDeal::focusItemDeal(CSceneBackgroundItem *item, CGraphicsScene *scene)
{
    item->initState();
    if (item->getUpItem() != nullptr) {
        scene->setCurrentFocusItem(item->getUpItem());
        item->getUpItem()->setItemFocus(true);
    } else {
        scene->setPrePage(item->getDate().addDays(-7));
    }
    return true;
}
