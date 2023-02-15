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
#ifndef CFOCUSITEM_H
#define CFOCUSITEM_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QPen>
#include <QDate>

class CFocusItem : public QObject
    , public QGraphicsRectItem
{
    Q_OBJECT
public:
    enum CItemType {
        CBACK,
        CITEM,
        COTHER
    };

public:
    explicit CFocusItem(QGraphicsItem *parent = nullptr);
    //设置下一个FocusItem
    void setNextFocusItem(CFocusItem *nextFocusItem);
    //设置item是否获取focus
    virtual void setItemFocus(bool isFocus);
    //获取该item是否focus
    bool getItemFocus() const;
    //设置item类型
    void setItemType(CItemType itemType);
    //获取item类型
    CItemType getItemType() const;
    //设置下一个item focus状态并获取下一个Item
    virtual CFocusItem *setNextItemFocusAndGetNextItem();
    //获取系统活动色
    QColor getSystemActiveColor();
    //设置显示日期
    void setDate(const QDate &date);
    //获取显示日期
    QDate getDate() const
    {
        return m_Date;
    }

protected:
    QDate m_Date;

private:
    CFocusItem *m_NextFocusItem;
    CItemType m_itemType;
    bool m_isFocus;
};

#endif // CFOCUSITEM_H
