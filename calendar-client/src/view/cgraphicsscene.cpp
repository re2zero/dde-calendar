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

#include <QEvent>
#include <QKeyEvent>
#include <QDebug>
#include <QShortcut>

CGraphicsScene::CGraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
    , firstfocusItem(nullptr)
    , currentFocusItem(nullptr)
    , m_keyPrxy(nullptr)
    , m_activeSwitching(false)
    , m_isContextMenu(false)
    , m_isShowCurrentItem(false)
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

/**
 * @brief CGraphicsScene::setPrePage        切换上一页
 * @param focusDate                         切换焦点时间
 * @param isSwitchView                      是否切换视图
 */
void CGraphicsScene::setPrePage(const QDate &focusDate, bool isSwitchView)
{
    emit signalSwitchPrePage(focusDate, isSwitchView);
}

/**
 * @brief CGraphicsScene::setPrePage        切换下一页
 * @param focusDate                         切换焦点时间
 * @param isSwitchView                      是否切换视图
 */
void CGraphicsScene::setNextPage(const QDate &focusDate, bool isSwitchView)
{
    emit signalSwitchNextPage(focusDate, isSwitchView);
}

bool CGraphicsScene::event(QEvent *event)
{
    bool dealResult = false;
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = dynamic_cast<QKeyEvent *>(event);
        if (m_keyPrxy != nullptr && m_keyPrxy->keyPressDeal(keyEvent->key())) {
            dealResult = true;
        }
        //如果为左右键处理则设置为true
        if (dealResult == false && (keyEvent->key() == Qt::Key_Left || keyEvent->key() == Qt::Key_Right)) {
            dealResult = true;
        }
        if (keyEvent->modifiers() == Qt::ALT && keyEvent->key() == Qt::Key_M) {
            CSceneBackgroundItem *item = dynamic_cast<CSceneBackgroundItem *>(currentFocusItem);
            if (item != nullptr && item->getFocusItem()->getItemType() == CFocusItem::CITEM) {
                dealResult = true;
                emit signalContextMenu(item->getFocusItem());
            }
        }
    }

    if (event->type() == QEvent::FocusIn) {
        dealResult = focusInDeal(event);
    }

    if (event->type() == QEvent::FocusOut) {
        dealResult = focusOutDeal(event);
    }
    return dealResult ? true : QGraphicsScene::event(event);
}

bool CGraphicsScene::focusInDeal(QEvent *event)
{
    bool dealResult = true;
    QFocusEvent *focusEvent = dynamic_cast<QFocusEvent *>(event);
    if (firstfocusItem != nullptr && (Qt::TabFocusReason == focusEvent->reason() || Qt::BacktabFocusReason == focusEvent->reason())) {
        if (m_isShowCurrentItem || currentFocusItem == nullptr) {
            if (currentFocusItem == nullptr) {
                currentFocusItem = firstfocusItem;
            }
            CFocusItem *item = dynamic_cast<CFocusItem *>(currentFocusItem);
            item->setItemFocus(true);
        } else {
            dealResult = m_keyPrxy->keyPressDeal(Qt::Key_Tab);
        }
    }
    if (currentFocusItem != nullptr && Qt::ActiveWindowFocusReason == focusEvent->reason()) {
        CFocusItem *item = dynamic_cast<CFocusItem *>(currentFocusItem);
        item->setItemFocus(true);
    }
    return dealResult;
}

bool CGraphicsScene::focusOutDeal(QEvent *event)
{
    QFocusEvent *focusEvent = dynamic_cast<QFocusEvent *>(event);
    if (currentFocusItem != nullptr) {
        CSceneBackgroundItem *item = dynamic_cast<CSceneBackgroundItem *>(currentFocusItem);
        if (Qt::ActiveWindowFocusReason == focusEvent->reason()) {
            item->setItemFocus(false);
        } else {
            //如果为右击菜单则更新焦点显示效果
            if (m_isContextMenu) {
                item->setItemFocus(false);
            } else {
                item->initState();
                //如果为被动切换焦点则初始化当前焦点item
                if (getActiveSwitching() == false) {
                    currentFocusItem = nullptr;
                    //通知另外一个视图初始化状态,因为全天和非全天之间tab切换保存了当前item信息
                    emit signalViewFocusInit();
                } else {
                    setActiveSwitching(false);
                }
            }
        }
    }
    return true;
}

void CGraphicsScene::setIsContextMenu(bool isContextMenu)
{
    m_isContextMenu = isContextMenu;
}

bool CGraphicsScene::getIsShowCurrentItem() const
{
    return m_isShowCurrentItem;
}

void CGraphicsScene::setIsShowCurrentItem(bool isShowCurrentItem)
{
    m_isShowCurrentItem = isShowCurrentItem;
}

void CGraphicsScene::currentItemInit()
{
    if (currentFocusItem != nullptr) {
        CSceneBackgroundItem *item = dynamic_cast<CSceneBackgroundItem *>(currentFocusItem);
        if (item != nullptr) {
            item->initState();
        }
        currentFocusItem = nullptr;
    }
}

bool CGraphicsScene::getActiveSwitching() const
{
    return m_activeSwitching;
}

void CGraphicsScene::setActiveSwitching(bool activeSwitching)
{
    m_activeSwitching = activeSwitching;
}
