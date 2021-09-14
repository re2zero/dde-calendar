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
    enum ViewPosition {
        WeekPos //周视图
        ,
        DayPos //日视图
    };
    enum ViewType {
        ALLDayView //全天
        ,
        PartTimeView //非全天
    };

public:
    explicit CWeekDayGraphicsview(QWidget *parent = nullptr, ViewPosition viewPos = WeekPos, ViewType viewtype = ALLDayView);
    ~CWeekDayGraphicsview() override;
    void setRange(int w, int h, QDate begindate, QDate enddate, int rightmagin);
    void setRange(QDate begin, QDate end);
    void setTheMe(int type = 0) override;
    CScheduleCoorManage *getCoorManage() const;
    //设置当前焦点背景item
    void setCurrentFocusItem(const QDate &focusDate, bool setItemFocus = false);

protected:
    //设置场景的矩阵
    void setSceneRect(qreal x, qreal y, qreal w, qreal h);
    //创建背景显示项
    void createBackgroundItem();
    void setSceneCurrentItemFocus(const QDate &focusDate) override;
    //更新背景上显示的item
    void updateBackgroundShowItem() override;

private:
    //设置背景时间
    void setBackgroundDate();
signals:
    void signaleSwitchToView(const QDate &focusDate, ViewType type, bool setItemFocus);
    void signalViewFocusInit();
public slots:
    void slotSwitchView(const QDate &focusDate, bool setItemFocus);
    void slotViewInit();
    void slotPosOnView(const qreal y);

protected:
    ViewPosition m_viewPos;
    ViewType m_viewType;
    QDate m_beginDate;
    QDate m_endDate;
    CScheduleCoorManage *m_coorManage;
    int m_rightmagin;
    QVector<CWeekDayBackgroundItem *> m_backgroundItem;
};

#endif // CWEEKDAYGRAPHICSVIEW_H
