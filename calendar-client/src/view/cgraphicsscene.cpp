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
#include "cgraphicsscene.h"

#include "graphicsItem/cscenebackgrounditem.h"
#include "cscenetabkeydeal.h"

#include <QEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QShortcut>

CGraphicsScene::CGraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
    , firstfocusItem(nullptr)
    , currentFocusItem(nullptr)
    , m_keyPrxy(nullptr)
{
}

CGraphicsScene::~CGraphicsScene()
{
    if (m_keyPrxy != nullptr)
        delete m_keyPrxy;
}

void CGraphicsScene::setFirstFocusItem(QGraphicsItem *item)
{
    firstfocusItem = item;
}

QGraphicsItem *CGraphicsScene::getFirstFocusItem() const
{
    return firstfocusItem;
}

void CGraphicsScene::setCurrentFocusItem(QGraphicsItem *item)
{
    currentFocusItem = item;
}

QGraphicsItem *CGraphicsScene::getCurrentFocusItem() const
{
    return currentFocusItem;
}

void CGraphicsScene::setKeyPressPrxy(CKeyPressPrxy *keyPrxy)
{
    m_keyPrxy = keyPrxy;
}

void CGraphicsScene::currentFocusItemUpdate()
{
    if (currentFocusItem != nullptr) {
        CFocusItem *item = dynamic_cast<CFocusItem *>(currentFocusItem);
        item->setItemFocus(true);
    }
}

void CGraphicsScene::setPrePage(const QDate &focusDate)
{
    emit signalSwitchPrePage(focusDate);
}

void CGraphicsScene::setNextPage(const QDate &focusDate)
{
    emit signalSwitchNextPage(focusDate);
}

bool CGraphicsScene::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = dynamic_cast<QKeyEvent *>(event);
        if (m_keyPrxy != nullptr && m_keyPrxy->keyPressDeal(keyEvent)) {
            return true;
        }
    }
    return QGraphicsScene::event(event);
}

void CGraphicsScene::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    if (firstfocusItem != nullptr && Qt::TabFocusReason == event->reason()) {
        currentFocusItem = firstfocusItem;
        CFocusItem *item = dynamic_cast<CFocusItem *>(currentFocusItem);
        item->setItemFocus(true);
    }
    if (currentFocusItem != nullptr && Qt::ActiveWindowFocusReason == event->reason()) {
        CFocusItem *item = dynamic_cast<CFocusItem *>(currentFocusItem);
        item->setItemFocus(true);
    }
}

void CGraphicsScene::focusOutEvent(QFocusEvent *event)
{
    if (currentFocusItem != nullptr) {
        CSceneBackgroundItem *item = dynamic_cast<CSceneBackgroundItem *>(currentFocusItem);
        if (Qt::ActiveWindowFocusReason == event->reason()) {
            item->setItemFocus(false);
        } else {
            currentFocusItem = nullptr;
            item->initState();
        }
    }
}
