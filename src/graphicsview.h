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
#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <DGraphicsView>
#include <QGraphicsScene>
#include "schedulestructs.h"
#include <QAction>
DWIDGET_USE_NAMESPACE
class CScheduleCoorManage;
class CScheduleItem;
class CGraphicsView : public DGraphicsView
{
    Q_OBJECT

public:
    CGraphicsView(QWidget *parent);
    ~CGraphicsView();
    void setMargins(int left, int top, int right, int bottom);
    QMargins getMargins()
    {
        return m_margins;
    }
    QGraphicsScene *getSence()
    {
        return m_graphicsScene;
    }
    void setTheMe(int type = 0);
    void setRange(int w, int h, QDate begindate, QDate enddate);

    CScheduleCoorManage *getCoorManage()
    {
        return m_coorManage;
    }
    void  addSchduleItem(const ScheduleDtailInfo &info, int index, int totalNum, int type);
    void  deleteSchduleItem(CScheduleItem *item);
    void  clearSchdule();
    /************************************************************************
    Function:       onViewEvent()
    Description:    执行父类事件
    Input:          event 事件
    Output:         无
    Return:         无
    Others:         无
    ************************************************************************/
    void onViewEvent(QEvent *event);

    /************************************************************************
    Function:       mousePressEvent()
    Description:    鼠标按下事件
    Input:          event 鼠标事件
    Output:         无
    Return:         无
    Others:         无
    ************************************************************************/
    void mousePressEvent(QMouseEvent *event);

    /************************************************************************
    Function:       mouseReleaseEvent()
    Description:    鼠标释放事件
    Input:          event 鼠标事件
    Output:         无
    Return:         无
    Others:         无
    ************************************************************************/
    void mouseReleaseEvent(QMouseEvent *event);

    /************************************************************************
    Function:       mouseDoubleClickEvent()
    Description:    鼠标双击事件
    Input:          event 鼠标事件
    Output:         无
    Return:         无
    Others:         无
    ************************************************************************/
    void mouseDoubleClickEvent(QMouseEvent *event);

    /************************************************************************
    Function:       mouseMoveEvent()
    Description:    鼠标移动事件
    Input:          event 鼠标事件
    Output:         无
    Return:         无
    Others:         无
    ************************************************************************/
    void mouseMoveEvent(QMouseEvent *event);

#ifndef QT_NO_WHEELEVENT
    /************************************************************************
    Function:       wheelEvent()
    Description:    鼠标滚轮事件
    Input:          event 滚轮事件
    Output:         无
    Return:         无
    Others:         无
    ************************************************************************/
    void wheelEvent(QWheelEvent *event);
#endif

    /************************************************************************
    Function:       keyPressEvent()
    Description:    键盘按下事件
    Input:          event 键盘事件
    Output:         无
    Return:         无
    Others:         无
    ************************************************************************/
    void keyPressEvent(QKeyEvent *event);

    /************************************************************************
    Function:       keyReleaseEvent()
    Description:    键盘释放事件
    Input:          event 键盘事件
    Output:         无
    Return:         无
    Others:         无
    ************************************************************************/
    void keyReleaseEvent(QKeyEvent *event);

    /************************************************************************
    Function:       focusInEvent()
    Description:    焦点进入事件
    Input:          event 焦点事件
    Output:         无
    Return:         无
    Others:         无
    ************************************************************************/
    void focusInEvent(QFocusEvent *event);

    /************************************************************************
    Function:       focusOutEvent()
    Description:    焦点离开事件
    Input:          event 焦点事件
    Output:         无
    Return:         无
    Others:         无
    ************************************************************************/
    void focusOutEvent(QFocusEvent *event);

    /************************************************************************
    Function:       enterEvent()
    Description:    进入事件
    Input:          event 事件
    Output:         无
    Return:         无
    Others:         无
    ************************************************************************/
    void enterEvent(QEvent *event);

    /************************************************************************
    Function:       leaveEvent()
    Description:    离开事件
    Input:          event 事件
    Output:         无
    Return:         无
    Others:         无
    ************************************************************************/
    void leaveEvent(QEvent *event);

    /************************************************************************
    Function:       contextMenuEvent()
    Description:    右键菜单事件
    Input:          event 右键菜单事件
    Output:         无
    Return:         无
    Others:         无
    ************************************************************************/
    void contextMenuEvent(QContextMenuEvent *event);

    /************************************************************************
    Function:       resizeEvent()
    Description:    窗口大小改变事件
    Input:          event 窗口大小改变事件
    Output:         无
    Return:         无
    Others:         无
    ************************************************************************/
    void resizeEvent(QResizeEvent *event);

    /************************************************************************
    Function:       setLargeScale()
    Description:    设置大刻度
    Input:          vLRLarge 左右刻度，vTBLarge 上下刻度
    Output:         无
    Return:         无
    Others:         无
    ************************************************************************/
    void setLargeScale(const QVector<int> &vLRLarge, const QVector<int> &vTBLarge);

    /************************************************************************
    Function:       setLargeScaleInfo()
    Description:    设置大刻度显示
    Input:          LRFlag 水平刻度，TBFlag 垂直刻度
    Output:         无
    Return:         无
    Others:         无
    ************************************************************************/
    void setLargeScaleFlag(const bool &LRFlag, const bool &TBFlag);

    /************************************************************************
    Function:       getLargeScaleFlag()
    Description:    获取大刻度显示
    Input:          LRFlag 水平刻度，TBFlag 垂直刻度
    Output:         无
    Return:         无
    Others:         无
    ************************************************************************/
    void getLargeScaleFlag(bool &LRFlag, bool &TBFlag);

    /************************************************************************
    Function:       setLargeScaleInfo()
    Description:    设置大刻度画笔
    Input:          LRPen 水平画笔，TBPen 垂直画笔
    Output:         无
    Return:         无
    Others:         无
    ************************************************************************/
    void setLargeScalePen(const QPen &LRPen, const QPen &TBPen);
    void setFirstWeekday(int weekday);

protected:
    void paintEvent( QPaintEvent *event );
public slots:
    void scrollBarValueChangedSlot();
signals:
    void signalsPosHours(QVector<int> vPos, QVector<int> vHours);
    void signalsUpdateShcedule(int id = 0);
private:
    int checkDay(int weekday);

private:
    QGraphicsScene                *m_graphicsScene;               //绘制Scene
    CScheduleCoorManage           *m_coorManage;
    QVector<CScheduleItem *>        m_vScheduleItem;
    QMargins                       m_margins;                     //四周空白
    bool                           m_LRFlag;          //水平线
    QPen                           m_LRPen;           //水平线画笔
    bool                           m_TBFlag;          //垂直线
    QPen                           m_TBPen;           //垂直线画笔
    QVector<int>                   m_vLRLarge;        //大刻度像素位置
    QVector<int>                   m_vTBLarge;        //大刻度像素位置
    int                            m_dayInterval;
    int                            m_timeInterval;
    int                            m_firstWeekDay;
    int                            m_totalDay;
    QAction                       *m_editAction;
    QAction                       *m_deleteAction;
    QAction                       *m_createAction;
    QColor                        m_weekcolor = "#4F9BFF";
};

#endif // GRAPHICSVIEW_H
