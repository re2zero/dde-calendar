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
#ifndef KEYPRESSSTUB_H
#define KEYPRESSSTUB_H

#include "../third-party_stub/stub.h"
#include "view/graphicsItem/cfocusitem.h"

#include <QObject>

class QGraphicsItem;
class KeyPressStub
{
public:
    KeyPressStub();
    ~KeyPressStub();
    Stub &getStub();

private:
    Stub stub;
};

extern bool itemFocus;
extern bool scene_activeSwitching;
extern bool WeekDayBackgroundItem_hasNextItem;
extern bool WeekDayBackgroundItem_showFocus;
extern QGraphicsItem *SceneCurrentItem;
extern CFocusItem::CItemType focusItemType;
extern QDate itemDate;

#endif // KEYPRESSSTUB_H
