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
#include "ckeypressdealbase.h"

#include "cgraphicsscene.h"
#include "graphicsItem/cscenebackgrounditem.h"

CKeyPressDealBase::CKeyPressDealBase(Qt::Key key, QGraphicsScene *scene)
    : m_key(key)
    , m_scene(scene)
{
}

CKeyPressDealBase::~CKeyPressDealBase()
{
}

/**
 * @brief CKeyPressDealBase::getKey 获取注册的key
 * @return
 */
Qt::Key CKeyPressDealBase::getKey() const
{
    return m_key;
}

bool CKeyPressDealBase::dealEvent()
{
    CGraphicsScene *scene = qobject_cast<CGraphicsScene *>(m_scene);
    if (scene != nullptr) {
        CSceneBackgroundItem *item = dynamic_cast<CSceneBackgroundItem *>(scene->getCurrentFocusItem());
        if (item != nullptr) {
            return focusItemDeal(item, scene);
        } else {
            return false;
        }
    }
    return false;
}
