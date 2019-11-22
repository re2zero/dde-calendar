/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     kirigaya <kirigaya@mkacg.com>
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
#ifndef SCHEDULEITEM_H
#define SCHEDULEITEM_H

#include <QGraphicsItem>
#include <QDateTime>
#include <QRectF>
#include <QPainterPath>
#include "schedulestructs.h"
class CScheduleCoorManage;
class CScheduleItem : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    CScheduleItem(CScheduleCoorManage *coor, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0, int type = 0);
    ~CScheduleItem();
    void setData(const ScheduleDtailInfo &info, QDate date, int index, int totalNum, int viewtype, int maxnum);
    int getType()
    {
        return  m_type;
    }
    ScheduleDtailInfo getData()
    {
        return m_scheduleInfo;
    }
    QDate getdate()
    {
        return  m_date;
    }
    int getviewtype()
    {
        return  m_viewtype;
    }
    int getindex()
    {
        return m_index;
    }
    int gettotalNum()
    {
        return  m_totalNum;
    }
    int getmaxNum()
    {
        return m_sMaxNum;
    }
    /*****************************************************************************
    Function:       boundingRect()
    Description:     获取边界矩形
    Input:              无
    Output:             无
    Return:             边界矩形
    Others:             无
    *******************************************************************************/
    QRectF boundingRect() const;
    QPainterPath shape () const;
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent ( QGraphicsSceneMouseEvent *event );
    void focusOutEvent ( QFocusEvent *event );
private:
    /*****************************************************************************
    Function:       paint()
    Description:    绘制
    Input:              painter 绘制工具，option 绘制参数，widget 窗口
    Output:             无
    Return:             无
    Others:             无
    *******************************************************************************/
    void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 );
    void splitText(QFont font, int w, int h, QString str, QStringList &liststr);
private:
    ScheduleDtailInfo                     m_scheduleInfo;
    CScheduleCoorManage             *m_coorManage;
    QDate                            m_date;
    QColor                           m_color;
    int                              m_type = 0;
    int                              m_viewtype;
    int                              m_index;
    int                              m_totalNum;
    int                              m_sMaxNum;
    bool                  m_selectflag = false;
    bool                  m_hoverflag = false;
    bool                  m_highflag = false;
    QColor                m_transparentcolor;
};

#endif // SCHEDULEITEM_H
