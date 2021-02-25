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
#include "cweekdayscenetabkeydeal.h"

#include "graphicsItem/cweekdaybackgrounditem.h"
#include "cgraphicsscene.h"

CWeekDaySceneTabKeyDeal::CWeekDaySceneTabKeyDeal(QGraphicsScene *scene)
    : CSceneTabKeyDeal(scene)
{
}

bool CWeekDaySceneTabKeyDeal::focusItemDeal(CSceneBackgroundItem *item, CGraphicsScene *scene)
{
    CWeekDayBackgroundItem *focusItem = dynamic_cast<CWeekDayBackgroundItem *>(item);
    if (focusItem != nullptr) {
        //如果当前背景是焦点显示则切换到另一个视图
        if (focusItem->getItemFoucs()) {
            focusItem->setItemFocus(false);
            scene->setActiveSwitching(true);
            scene->signalSwitchView(focusItem->getDate());
            return true;
        } else {
            //如果该背景上还有切换显示的日程标签
            if (focusItem->hasNextSubItem() || focusItem->showFocus()) {
                return CSceneTabKeyDeal::focusItemDeal(item, scene);
            } else {
                focusItem->initState();
                scene->setActiveSwitching(true);
                scene->signalSwitchView(focusItem->getDate());
                return true;
            }
        }
    }
    return false;
}
