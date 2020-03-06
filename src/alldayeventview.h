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
#include <DListWidget>
#include "SchecduleRemindWidget.h"
DWIDGET_USE_NAMESPACE
class CAllDayEventWidgetItem;
class QVBoxLayout;
class CSolodayWidgetItem;
class CScheduleCoorManage;
class CAllDayEventWeekView : public DListWidget
{
    Q_OBJECT

public:
    CAllDayEventWeekView(QWidget *parent = nullptr, int edittype = 0);
    ~CAllDayEventWeekView();
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
    void setRange(int w, int h, QDate begindate, QDate enddate);
    CScheduleCoorManage *getCoorManage()
    {
        return m_coorManage;
    }
    void setLunarVisible(bool state);
    void updateHigh();
signals:
    void signalsUpdateShcedule(int id = 0);
    void signalsitem(void *item);
    void signalViewtransparentFrame(int type);
public slots:
    void slotdeleteitem(CAllDayEventWidgetItem *item);
    void slotedititem(CAllDayEventWidgetItem *item, int type = 0);
    void slotupdateItem(CAllDayEventWidgetItem *item);
    void slotDeleteItem();
private slots:
    void slotCreate();
protected:
    void contextMenuEvent(QContextMenuEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
private:
    void updateDateShow();
    CAllDayEventWidgetItem *createItemWidget(int index, bool average = false);
    CSolodayWidgetItem *createItemWidget(QVector<QString>   vSolarInfo, QVector<QDate> date, bool average = false);

private:
    QAction                                     *m_createAction;     // 创建日程
    bool                                         m_widgetFlag;
    QVector<QVector<ScheduleDtailInfo> >         m_vlistData;
    QVector<DPushButton *>                       m_baseShowItem;
    int                                          m_type;
    QVector<QString>                             m_vSolarDayInfo;
    QVector<QDate>                               m_vDate;
    int                                          m_editType = 0;
    QColor                                       m_soloColor = "#FF7272";
    CScheduleCoorManage                         *m_coorManage;
    QDate                                       m_dianjiDay;
    bool                                        m_LunarVisible;
    CAllDayEventWidgetItem *m_currentitem = nullptr;
};

class CAllDayEventWidgetItem : public DPushButton
{
    Q_OBJECT

public:
    explicit CAllDayEventWidgetItem(QWidget *parent = nullptr, int edittype = 0);
    void setItem(QListWidgetItem *_item)
    {
        m_item = _item;
    }
    QListWidgetItem *getItem()
    {
        return m_item;
    }
    void setData(QVector<ScheduleDtailInfo>  &vScheduleInfo);
    void setCoorManage(CScheduleCoorManage *coor)
    {
        m_coorManage = coor;
    }
    int getEventByPos(QPoint pos);
signals:
    void signalsDelete(CAllDayEventWidgetItem *item);
    void signalsEdit(CAllDayEventWidgetItem *item, int type = 0);
    void signalsPress(CAllDayEventWidgetItem *item);
    void signalViewtransparentFrame(int type);
public slots:
    void slotEdit();
    void slotDelete();
    void slotDoubleEvent(int type = 0);
    void slotCreate();
protected:
    void paintEvent ( QPaintEvent *e) Q_DECL_OVERRIDE;
    void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent (QMouseEvent *event ) Q_DECL_OVERRIDE;
    void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
    //void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
private:
    void SchecduleRemindWidgetHide();
private:
    void paintItem(int index);
private:
    QVector<ScheduleDtailInfo>  m_vScheduleInfo;
    QAction              *m_editAction;
    QAction              *m_deleteAction;
    QAction              *m_createAction;     // 创建日程
    QFont                 m_font;
    QListWidgetItem      *m_item;
    bool                  m_avgeflag;
    int                   m_editType = 0;
    QDate                                       m_dianjiDay;
    CScheduleCoorManage                         *m_coorManage;
    int                   m_currentIndex = -1;
    QVector<bool>         m_vSelectflag;
    QVector<bool>         m_vHoverflag;
    QVector<bool>         m_vHighflag;
    bool                  m_pressMove = false;
};
class CSolodayWidgetItem : public DPushButton
{
    Q_OBJECT

public:
    explicit CSolodayWidgetItem(QWidget *parent = nullptr, int edittype = 0);
    void setColor(QColor color1, QColor color2, bool GradientFlag = false);
    void setText(QColor tcolor, QFont font, QPoint pos, bool avgeflag = false);
    void setData(QVector<QString>   vSolarInfo, QVector<QDate> date);
    void setCoorManage(CScheduleCoorManage *coor)
    {
        m_coorManage = coor;
    }
protected:
    void paintEvent ( QPaintEvent *e) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
private:

    QVector<QString>      m_vSolarDayInfo;
    QVector<QDate>        m_vDate;
    QVector<bool>         m_vhover;
    QVector<bool>         m_vselectflag;
    bool                  m_GradientFlag;
    QColor                m_color1;
    QColor                m_color2;
    QColor                m_textcolor;
    QFont                 m_font;
    QPoint                m_pos;
    CScheduleCoorManage                         *m_coorManage;
    QColor                m_transparentcolor = "#000000";
};
#endif // CSHCEDULEDAYVIEW_H

