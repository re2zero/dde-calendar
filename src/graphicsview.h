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
#include <QMutex>
#include <QDrag>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
DWIDGET_USE_NAMESPACE

typedef struct _tagScheduleclassificationInfo {
    QDateTime begindate;
    QDateTime enddate;
    QVector<ScheduleDtailInfo> vData;
} ScheduleclassificationInfo;

class CScheduleCoorManage;
class CScheduleItem;
class CGraphicsView : public DGraphicsView
{
    Q_OBJECT
    enum PosInItem {TOP,MIDDLE,BOTTOM};
    enum DragStatus {IsCreate,ChangeBegin,ChangeEnd,ChangeWhole,NONE};
public:
    CGraphicsView(QWidget *parent, int viewType = 0);
    ~CGraphicsView() Q_DECL_OVERRIDE;
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
    void updateHigh();
    void setRange(int w, int h, QDate begindate, QDate enddate, int rightmagin);

    CScheduleCoorManage *getCoorManage()
    {
        return m_coorManage;
    }
    void setInfo(const QVector<ScheduleDtailInfo> &info);
    void upDateInfoShow(const DragStatus &status = NONE,const ScheduleDtailInfo &info =ScheduleDtailInfo());

    void  addSchduleItem(const ScheduleDtailInfo &info, QDate date, int index, int totalNum, int type, int viewtype, int maxnum);
    void  deleteSchduleItem(CScheduleItem *item);
    void  setSelectSchedule(const ScheduleDtailInfo &info);
    void  clearSchdule();

    void setMinTime(const int &minTime)
    {
        m_minTime = minTime;
    }
    void setMaxNum(const int maxnum)
    {
        m_sMaxNum = maxnum;
    }

    void scheduleClassificationType(QVector<ScheduleDtailInfo> &scheduleInfolist,
                                    QVector<ScheduleclassificationInfo> &info);
    void updateschedule();
    void setInfoItemNull();

    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
#endif

    /************************************************************************
    Function:       keyPressEvent()
    Description:    键盘按下事件
    Input:          event 键盘事件
    Output:         无
    Return:         无
    Others:         无
    ************************************************************************/
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

    /************************************************************************
    Function:       keyReleaseEvent()
    Description:    键盘释放事件
    Input:          event 键盘事件
    Output:         无
    Return:         无
    Others:         无
    ************************************************************************/
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

    /************************************************************************
    Function:       focusInEvent()
    Description:    焦点进入事件
    Input:          event 焦点事件
    Output:         无
    Return:         无
    Others:         无
    ************************************************************************/
    void focusInEvent(QFocusEvent *event) Q_DECL_OVERRIDE;

    /************************************************************************
    Function:       focusOutEvent()
    Description:    焦点离开事件
    Input:          event 焦点事件
    Output:         无
    Return:         无
    Others:         无
    ************************************************************************/
    void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;

    /************************************************************************
    Function:       enterEvent()
    Description:    进入事件
    Input:          event 事件
    Output:         无
    Return:         无
    Others:         无
    ************************************************************************/
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;

    /************************************************************************
    Function:       leaveEvent()
    Description:    离开事件
    Input:          event 事件
    Output:         无
    Return:         无
    Others:         无
    ************************************************************************/
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;

    /************************************************************************
    Function:       contextMenuEvent()
    Description:    右键菜单事件
    Input:          event 右键菜单事件
    Output:         无
    Return:         无
    Others:         无
    ************************************************************************/
    void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;

    /************************************************************************
    Function:       resizeEvent()
    Description:    窗口大小改变事件
    Input:          event 窗口大小改变事件
    Output:         无
    Return:         无
    Others:         无
    ************************************************************************/
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

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
    void setTime(QTime time);
protected:
    void paintEvent( QPaintEvent *event ) Q_DECL_OVERRIDE;
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragLeaveEvent(QDragLeaveEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
public slots:
    void scrollBarValueChangedSlot();
    void slotDoubleEvent(int type);
    void slotDeleteItem();
    void slotHoverUpdateState(CScheduleItem *item, int state);
    void slotSelectUpdateState(CScheduleItem *item, int state);
    void slotScrollBar();
signals:
    void signalsPosHours(QVector<int> vPos, QVector<int> vHours, int cuttrnttimetype = 0);
    void signalsUpdateShcedule(int id = 0);
    void signalsitem(void *item);
    void signalsCurrentScheduleDate(QDate date);
    void signalViewtransparentFrame(int type);
    void signalScheduleShow(bool isShow, const ScheduleDtailInfo &out = ScheduleDtailInfo());
private:
    int checkDay(int weekday);
    void DragPressEvent(const QPoint &pos,const CScheduleItem *item);
    PosInItem getPosInItem(const QPoint &p,const QRectF &itemRect);
    void TimeRound(QDateTime &dtime);
    ScheduleDtailInfo getScheduleInfo(const QDateTime &beginDate,const QDateTime &endDate);
    void updateScheduleInfo(const ScheduleDtailInfo &info);
    QDateTime TimeRounding(const QDateTime &time);
private:
    QGraphicsScene                  *m_graphicsScene;               //绘制Scene
    CScheduleCoorManage             *m_coorManage;
    QVector<CScheduleItem *>        m_vScheduleItem;
    CScheduleItem                   *m_currentItem;             //当前item
    QMargins                        m_margins;                     //四周空白
    bool                            m_LRFlag;          //水平线
    QPen                            m_LRPen;           //水平线画笔
    bool                            m_TBFlag;          //垂直线
    QPen                            m_TBPen;           //垂直线画笔
    QVector<int>                    m_vLRLarge;        //大刻度像素位置
    QVector<int>                    m_vTBLarge;        //大刻度像素位置
    float                           m_dayInterval;
    float                           m_timeInterval;
    int                             m_firstWeekDay;
    int                             m_totalDay;
    QAction                         *m_editAction;
    QAction                         *m_deleteAction;
    QAction                         *m_createAction;
    QColor                          m_weekcolor = "#4F9BFF";
    QColor                          m_currenttimecolor = "#F74444";
    int                             m_cuttrnttimetype = 0;
    QTimer                          *m_timer;
    QMutex                          m_Mutex;
    int                             m_viewType = 0;
    bool                            m_updateDflag  = false;
    int                             m_rightmagin = 0;
    bool                            m_press = false;
    int                             m_themetype = 0;


    QVector<ScheduleDtailInfo>                      m_scheduleInfo;
    QDate                           m_beginDate;
    QDate                           m_endDate;
    int                             m_minTime;      //最小高度对应的最小时间
    int                             m_sMaxNum = 4;

    DragStatus                      m_DragStatus =NONE;
    bool                            m_isCreate;
    QDateTime                       m_PressDate;
    QDateTime                       m_MoveDate;
    QPoint                          m_PressPos;
    ScheduleDtailInfo               m_DragScheduleInfo;
    QDateTime                       m_InfoBeginTime;
    QDateTime                       m_InfoEndTime;
    QDrag                           *m_Drag;
    ScheduleDtailInfo               m_PressScheduleInfo;
};

#endif // GRAPHICSVIEW_H
