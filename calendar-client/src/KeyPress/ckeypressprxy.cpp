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
#include "ckeypressprxy.h"

#include <QDebug>

CKeyPressPrxy::CKeyPressPrxy()
{
}

CKeyPressPrxy::~CKeyPressPrxy()
{
    QMap<int, CKeyPressDealBase *>::iterator iter = m_keyEventMap.begin();
    for (; iter != m_keyEventMap.end(); ++iter) {
        delete iter.value();
    }
    m_keyEventMap.clear();
}

/**
 * @brief CKeyPressPrxy::keyPressDeal   键盘事件处理
 * @param event
 * @return
 */
bool CKeyPressPrxy::keyPressDeal(int key)
{
    bool result = m_keyEventMap.contains(key);
    if (result) {
        //如果有注册对应的key事件 开始处理
        result = m_keyEventMap[key]->dealEvent();
    }
    return result;
}

/**
 * @brief CKeyPressPrxy::addkeyPressDeal    添加需要处理的键盘事件
 * @param deal
 */
void CKeyPressPrxy::addkeyPressDeal(CKeyPressDealBase *deal)
{
    if (deal != nullptr)
        m_keyEventMap[deal->getKey()] = deal;
}

/**
 * @brief CKeyPressPrxy::removeDeal     移除添加的键盘事件
 * @param deal
 */
void CKeyPressPrxy::removeDeal(CKeyPressDealBase *deal)
{
    if (m_keyEventMap.contains(deal->getKey())) {
        delete m_keyEventMap[deal->getKey()];
        m_keyEventMap.remove(deal->getKey());
    }
}
