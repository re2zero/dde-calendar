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
#ifndef CKEYPRESSPRXY_H
#define CKEYPRESSPRXY_H

#include "ckeypressdealbase.h"

#include <QMap>
#include <QKeyEvent>

class CKeyPressPrxy
{
public:
    explicit CKeyPressPrxy();
    ~CKeyPressPrxy();
    //键盘事件处理
    bool keyPressDeal(QKeyEvent *event);
    //添加需要处理的键盘事件
    void addkeyPressDeal(CKeyPressDealBase *deal);
    //移除添加的键盘事件
    void removeDeal(CKeyPressDealBase *deal);

private:
    QMap<int, CKeyPressDealBase *> m_keyEventMap {};
};

#endif // CKEYPRESSPRXY_H
