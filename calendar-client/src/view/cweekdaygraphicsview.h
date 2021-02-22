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
#ifndef CWEEKDAYGRAPHICSVIEW_H
#define CWEEKDAYGRAPHICSVIEW_H

#include "draginfographicsview.h"
#include "graphicsItem/cweekdaybackgrounditem.h"

/**
 * @brief The CWeekDayGraphicsview class
 *  周/日全天和非全天视图
 */
class CScheduleCoorManage;
class CWeekDayGraphicsview : public DragInfoGraphicsView
{
    Q_OBJECT
public:
    enum ViewType {
        WeekView //周视图
        ,
        DayView //日视图
    };

public:
    explicit CWeekDayGraphicsview(QWidget *parent = nullptr, ViewType edittype = WeekView);
    ~CWeekDayGraphicsview() override;
    void setRange(int w, int h, QDate begindate, QDate enddate, int rightmagin);
    void setRange(QDate begin, QDate end);
    void setTheMe(int type = 0) override;
    CScheduleCoorManage *getCoorManage() const;

protected:
    //设置场景的矩阵
    void setSceneRect(qreal x, qreal y, qreal w, qreal h);
    //创建背景显示项
    void createBackgroundItem();
signals:

public slots:
protected:
    ViewType m_viewType;
    QDate m_beginDate;
    QDate m_endDate;
    CScheduleCoorManage *m_coorManage;
    int m_rightmagin;
    QVector<CWeekDayBackgroundItem *> m_backgroundItem;
};

#endif // CWEEKDAYGRAPHICSVIEW_H
