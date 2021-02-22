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
#ifndef CWEEKDAYBACKGROUNDITEM_H
#define CWEEKDAYBACKGROUNDITEM_H

#include "cscenebackgrounditem.h"

#include <QDate>
/**
 * @brief The CWeekDayBackgroundItem class
 * 周和天的全天/非全天背景绘制
 */
class CWeekDayBackgroundItem : public CSceneBackgroundItem
{
    Q_OBJECT
public:
    explicit CWeekDayBackgroundItem(QGraphicsItem *parent = nullptr);
    //设置主题
    void setTheMe(int type = 0);
    bool drawDividingLine() const;
    void setDrawDividingLine(bool drawDividingLine);
    bool showFocus() const;
    void setShowFocus(bool showFocus);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
signals:

public slots:
private:
    bool m_drawDividingLine; //是否绘制分割线
    bool m_showFocus; //背景是否显示焦点效果
    QColor m_weekColor; //周六周日背景色
};

#endif // CWEEKDAYBACKGROUNDITEM_H
