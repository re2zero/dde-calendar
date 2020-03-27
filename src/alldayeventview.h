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
#include "SchecduleRemindWidget.h"
DWIDGET_USE_NAMESPACE
class CAllDayEventWidgetItem;
class CSolodayWidgetItem;
class CScheduleCoorManage;
class CAllDayEventWeekView : public DGraphicsView
{
    Q_OBJECT

public:
    CAllDayEventWeekView(QWidget *parent = nullptr, int edittype = 0);
    ~CAllDayEventWeekView() Q_DECL_OVERRIDE;
    void setDayData(const QVector<QVector<ScheduleDtailInfo> > &vlistData, int type = 1);
    void setsolarDayData(QVector<QString> vSolarInfo, QVector<QDate> date);
    QVector<QVector<ScheduleDtailInfo> > &getListData()
    {
        return  m_vlistData;
    }
    QVector<QString> getSolarDay()
    {
        return  m_vSolarDayInfo;
    }
    QVector<QDate> getDate()
    {
        return  m_vDate;
    }
    void setTheMe(int type = 0);
    void setRange(int w, int h, QDate begindate, QDate enddate, int rightmagin);
    CScheduleCoorManage *getCoorManage()
    {
        return m_coorManage;
    }
    void setLunarVisible(bool state);
    void updateHigh();
    void setSelectSchedule(const ScheduleDtailInfo &info);
signals:
    void signalsUpdateShcedule(int id = 0);
    void signalsitem(void *item);
    void signalViewtransparentFrame(int type);
    void signalScheduleShow(const bool isShow, const int scheduleID = 0);
public slots:
    void slotdeleteitem(CAllDayEventWidgetItem *item);
    void slotedititem(CAllDayEventWidgetItem *item, int type = 0);
    void slotupdateItem(CAllDayEventWidgetItem *item);
    void slotDeleteItem();
private slots:
    void slotCreate();
    void slotDoubleEvent();
protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
private:
    void updateDateShow();
    void createItemWidget(int index, bool average = false);
private:
    int                     itemHeight = 22;
    QAction                                     *m_createAction;     // 创建日程
    QAction                       *m_editAction;
    QAction                       *m_deleteAction;
    bool                                         m_widgetFlag;
    QVector<QVector<ScheduleDtailInfo> >         m_vlistData;
    QVector<CAllDayEventWidgetItem *>                       m_baseShowItem;
    int                                          m_type;
    QVector<QString>                             m_vSolarDayInfo;
    QVector<QDate>                               m_vDate;
    int                                          m_editType = 0;
    QColor                                       m_soloColor = "#FF7272";
    CScheduleCoorManage                         *m_coorManage;
    QDate                                       m_dianjiDay;
    bool                                        m_LunarVisible;
    CAllDayEventWidgetItem                      *m_currentitem = nullptr;
    int m_rightmagin = 0;
    bool                         m_updateDflag  = false;
    QGraphicsScene                              *m_Scene;
    bool                            m_press = false;
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
    int getEventByPos(QPoint pos);
    void updateitem();
    void setPressFlag(const bool flag);
    void setFont(DFontSizeManager::SizeType type);
    void setOffset(const int &offset);
    bool hasSelectSchedule(const ScheduleDtailInfo &info);
    void setStartValue(const int value);
    void setEndValue(const int value);
    void startAnimation();
signals:
    void signalsDelete(CAllDayEventWidgetItem *item);
    void signalsEdit(CAllDayEventWidgetItem *item, int type = 0);
    void signalsPress(CAllDayEventWidgetItem *item);
    void signalViewtransparentFrame(int type);
//    void signalScheduleShow(const bool isShow, const int scheduleID = 0);
public slots:
    void slotEdit();
    void slotDelete();
    void slotDoubleEvent(int type = 0);
    void slotCreate();
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
private:
    ScheduleDtailInfo     m_vScheduleInfo;
    QAction              *m_editAction;
    QAction              *m_deleteAction;
    QAction              *m_createAction;     // 创建日程
    QFont                 m_font;
    bool                  m_avgeflag;
    int                   m_editType = 0;
    QDate                                       m_dianjiDay;
    CScheduleCoorManage                         *m_coorManage;
    bool         m_vSelectflag;
    bool         m_vHoverflag;
    bool         m_vHighflag;
    bool            m_press = false;
    bool                  m_pressMove = false;
    QRect       m_rect;
    DFontSizeManager::SizeType      m_sizeType  = DFontSizeManager::T8;
    QPropertyAnimation *m_properAnimationFirst;
    QPropertyAnimation *m_properANimationSecond;
    QSequentialAnimationGroup *m_Group;
};
#endif // CSHCEDULEDAYVIEW_H

