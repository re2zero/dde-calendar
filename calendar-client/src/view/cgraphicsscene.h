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
#ifndef CGRAPHICSSCENE_H
#define CGRAPHICSSCENE_H

#include "ckeypressprxy.h"

#include <QGraphicsScene>
/**
 * @brief The CGraphicsScene class
 *
 */
class CGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CGraphicsScene(QObject *parent = nullptr);
    ~CGraphicsScene() override;
    //设置Scene的第一个focus item
    void setFirstFocusItem(QGraphicsItem *item);
    //获取Scene的第一个focus item
    QGraphicsItem *getFirstFocusItem() const;
    //设置Scene当前focus item
    void setCurrentFocusItem(QGraphicsItem *item);
    //获取Scene当前focus item
    QGraphicsItem *getCurrentFocusItem() const;
    //设置keypress处理
    void setKeyPressPrxy(CKeyPressPrxy *keyPrxy);
    void currentFocusItemUpdate();
    void setPrePage(const QDate &focusDate);
    void setNextPage(const QDate &focusDate);

protected:
    bool event(QEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
signals:
    void signalSwitchPrePage(const QDate &focusDate);
    void signalSwitchNextPage(const QDate &focusDate);

private:
    QGraphicsItem *firstfocusItem;
    QGraphicsItem *currentFocusItem;
    CKeyPressPrxy *m_keyPrxy;
};

#endif // CGRAPHICSSCENE_H
