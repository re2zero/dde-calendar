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
#ifndef ALLDAYEVENTVIEW_H
#define ALLDAYEVENTVIEW_H

#include <DComboBox>
#include <DLabel>
#include <QMouseEvent>
#include <DPushButton>
#include "schedulestructs.h"
#include <DGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <DFontSizeManager>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

#include <DMenu>
#include "SchecduleRemindWidget.h"
DWIDGET_USE_NAMESPACE
class QDrag;
class CAllDayEventWidgetItem;
class CScheduleCoorManage;
class CAllDayEventWeekView : public DGraphicsView
{
    Q_OBJECT
    enum PosInItem {LEFT,MIDDLE,RIGHT};
    enum DragStatus {IsCreate,ChangeBegin,ChangeEnd,ChangeWhole,NONE};
public:
    CAllDayEventWeekView(QWidget *parent = nullptr, int edittype = 0);
    ~CAllDayEventWeekView() Q_DECL_OVERRIDE;
    void setDayData(const QVector<QVector<ScheduleDtailInfo> > &vlistData);
    void setInfo(const QVector<ScheduleDtailInfo> &info);
    QVector<QVector<ScheduleDtailInfo> > &getListData()
    {
        return  m_vlistData;
    }
    QVector<QDate> getDate()
    {
        return  m_vDate;
    }
    void setTheMe(int type = 0);
    void setRange(int w, int h, QDate begindate, QDate enddate, int rightmagin);
    void setRange(QDate begin, QDate end);
    CScheduleCoorManage *getCoorManage()
    {
        return m_coorManage;
    }
    void updateHigh();
    void setSelectSchedule(const ScheduleDtailInfo &info);
    void setMargins(int left, int top, int right, int bottom);
    int upDateInfoShow(const DragStatus &status = NONE,const ScheduleDtailInfo &info =ScheduleDtailInfo());
signals:
    void signalsUpdateShcedule(int id = 0);
    void signalViewtransparentFrame(int type);
    void signalScheduleShow(const bool isShow, const ScheduleDtailInfo &out = ScheduleDtailInfo());
    void signalUpdatePaint(const int topM);
    void signalSceneUpdate();
public slots:
    void slotdeleteitem(CAllDayEventWidgetItem *item);
    void slotedititem(CAllDayEventWidgetItem *item, int type = 0);
    void slotDeleteItem();
    void slotUpdateScene();
private slots:
    void slotCreate();
    void slotDoubleEvent();
protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void paintEvent( QPaintEvent *event ) Q_DECL_OVERRIDE;
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragLeaveEvent(QDragLeaveEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
private:
    void updateDateShow();
    void updateScheduleInfo(const ScheduleDtailInfo &info);
    void DragPressEvent(const QPoint &pos,const CAllDayEventWidgetItem *item);
    void createItemWidget(int index, bool average = false);
    ScheduleDtailInfo getScheduleInfo(const QDate &beginDate,const QDate &endDate);
    PosInItem getPosInItem(const QPoint &p,const QRectF &itemRect);
    void setPressSelectInfo(const ScheduleDtailInfo &info);
    void DeleteItem(const ScheduleDtailInfo &info);
private:
    int                     itemHeight = 22;
    QAction                                     *m_createAction;     // 创建日程
    QAction                                     *m_editAction;
    QAction                                     *m_deleteAction;
    QVector<QVector<ScheduleDtailInfo> >         m_vlistData;
    QVector<ScheduleDtailInfo>                   m_scheduleInfo;
    QVector<CAllDayEventWidgetItem *>            m_baseShowItem;
    QVector<QDate>                               m_vDate;
    int                                          m_editType = 0;
    CScheduleCoorManage                         *m_coorManage;
    QDate                                       m_dianjiDay;
    int                                             m_themetype = 0;
    int m_rightmagin = 0;
    bool                            m_updateDflag  = false;
    QGraphicsScene                              *m_Scene;
    bool                            m_press = false;

    QDate                           m_beginDate;
    QDate                           m_endDate;

    DMenu                           *m_rightMenu;

    DragStatus                      m_DragStatus =NONE;
    bool                            m_isCreate;
    QDate                           m_PressDate;
    QDate                           m_MoveDate;
    QPoint                          m_PressPos;
    ScheduleDtailInfo               m_DragScheduleInfo;
    QDateTime                       m_InfoBeginTime;
    QDateTime                       m_InfoEndTime;
    QDrag                           *m_Drag;
    ScheduleDtailInfo               m_PressScheduleInfo;
    QRectF                           m_PressRect;
};

class CAllDayEventWidgetItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
    Q_PROPERTY(int offset WRITE setOffset)
public:
    explicit CAllDayEventWidgetItem(QRect rect, QGraphicsItem *parent = nullptr, int edittype = 0);
    void setData(const ScheduleDtailInfo  &vScheduleInfo);
    ScheduleDtailInfo getData() const;
    void setCoorManage(CScheduleCoorManage *coor)
    {
        m_coorManage = coor;
    }
    void updateitem();
    void setPressFlag(const bool flag);
    void setFont(DFontSizeManager::SizeType type);
    void setOffset(const int &offset);
    bool hasSelectSchedule(const ScheduleDtailInfo &info);
    void setStartValue(const int value);
    void setEndValue(const int value);
    void startAnimation();
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
private:
    ScheduleDtailInfo                   m_vScheduleInfo;
    QFont                               m_font;
    bool                                m_avgeflag;
    int                                 m_editType = 0;
    QDate                                       m_dianjiDay;
    CScheduleCoorManage                         *m_coorManage;
    bool                                m_vSelectflag;
    bool                                m_vHoverflag;
    bool                                m_vHighflag;
    bool                                m_press = false;
    QRect                               m_rect;
    DFontSizeManager::SizeType          m_sizeType  = DFontSizeManager::T8;
    QPropertyAnimation                  *m_properAnimationFirst;
    QPropertyAnimation                  *m_properANimationSecond;
    QSequentialAnimationGroup           *m_Group;
};
#endif // CSHCEDULEDAYVIEW_H

