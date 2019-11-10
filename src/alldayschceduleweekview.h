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
#ifndef ALLDAYSCHCEDULEWEEKVIEW_H
#define ALLDAYSCHCEDULEWEEKVIEW_H

#include <DComboBox>
#include <DLabel>
#include <QMouseEvent>
#include <DPushButton>
#include "schedulestructs.h"
#include <DListWidget>
DWIDGET_USE_NAMESPACE
class CAllDaySchceduleWeekWidgetItem;
class QVBoxLayout;
class CAllSolarDayWeekWidgetItem;
class CScheduleCoorManage;
class CAllDaySchceduleWeekView : public DListWidget
{
    Q_OBJECT

public:
    CAllDaySchceduleWeekView(QWidget *parent = nullptr, int edittype = 0);
    ~CAllDaySchceduleWeekView();
    void setDayData(const QVector<ScheduleDtailInfo> &vlistData, int type = 1);
    void setsolarDayData(QVector<QString> vSolarInfo, QVector<QDate> date);
    QVector<ScheduleDtailInfo> &getListData()
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
signals:
    void signalsUpdateShcedule(int id = 0);
    void signalsitem(void *item);
public slots:
    void slotdeleteitem(CAllDaySchceduleWeekWidgetItem *item);
    void slotedititem(CAllDaySchceduleWeekWidgetItem *item, int type = 0);
    void slotupdateItem(CAllDaySchceduleWeekWidgetItem *item);
    void slotDeleteItem();
private slots:
    void slotCreate();
protected:
    void contextMenuEvent(QContextMenuEvent *event);
private:
    void updateDateShow();
    CAllDaySchceduleWeekWidgetItem *createItemWidget(int index, bool average = false);
    CAllSolarDayWeekWidgetItem *createItemWidget(QVector<QString>   vSolarInfo, QVector<QDate> date, bool average = false);

private:
    QAction                                     *m_createAction;     // 创建日程
    bool                                         m_widgetFlag;
    QVector<ScheduleDtailInfo>                   m_vlistData;
    QVector<DPushButton *>                       m_baseShowItem;
    int                                          m_type;
    QVector<QString>                             m_vSolarDayInfo;
    QVector<QDate>                               m_vDate;
    int                                          m_editType = 0;
    QColor                                       m_soloColor = "#FF7272";
    CScheduleCoorManage                         *m_coorManage;
    QDate                                       m_dianjiDay;
    bool                                        m_LunarVisible;
    CAllDaySchceduleWeekWidgetItem *m_currentitem = NULL;
};

class CAllDaySchceduleWeekWidgetItem : public DPushButton
{
    Q_OBJECT

public:
    explicit CAllDaySchceduleWeekWidgetItem(QWidget *parent = nullptr, int edittype = 0);
    void setColor(QColor color1, QColor color2, bool GradientFlag = false);
    void setText(QColor tcolor, QFont font, QPoint pos, bool avgeflag = false);
    void getColor(QColor &color1, QColor &color2, bool &GradientFlag);
    void getText(QColor &tcolor, QFont &font, QPoint &pos);

    void setItem(QListWidgetItem *_item)
    {
        m_item = _item;
    }
    QListWidgetItem *getItem()
    {
        return m_item;
    }

    void setData(ScheduleDtailInfo  vScheduleInfo);
    void setTransparentB(bool t, QColor tcolor);
    const ScheduleDtailInfo &getData() const
    {
        return m_ScheduleInfo;
    }
    void setCoorManage(CScheduleCoorManage *coor)
    {
        m_coorManage = coor;
    }
signals:
    void signalsDelete(CAllDaySchceduleWeekWidgetItem *item);
    void signalsEdit(CAllDaySchceduleWeekWidgetItem *item, int type = 0);
    void signalsPress(CAllDaySchceduleWeekWidgetItem *item);
public slots:
    void slotEdit();
    void slotDelete();
    void slotDoubleEvent(int type = 0);
    void slotCreate();
protected:
    void paintEvent ( QPaintEvent *e);
    void contextMenuEvent(QContextMenuEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
private:
    ScheduleDtailInfo     m_ScheduleInfo;
    QAction              *m_editAction;
    QAction              *m_deleteAction;
    QAction              *m_createAction;     // 创建日程
    bool                  m_GradientFlag;
    QColor                m_color1;
    QColor                m_color2;
    QColor                m_textcolor;
    QFont                 m_font;
    QPoint                m_pos;
    QListWidgetItem      *m_item;
    bool                  m_avgeflag;
    int                   m_editType = 0;
    QDate                                       m_dianjiDay;
    CScheduleCoorManage                         *m_coorManage;
    QColor                m_transparentcolor;
    bool                  m_transparentf = false;
    bool                  m_selectflag = false;
    bool                  m_hoverflag = false;
};
class CAllSolarDayWeekWidgetItem : public DPushButton
{
    Q_OBJECT

public:
    explicit CAllSolarDayWeekWidgetItem(QWidget *parent = nullptr, int edittype = 0);
    void setColor(QColor color1, QColor color2, bool GradientFlag = false);
    void setText(QColor tcolor, QFont font, QPoint pos, bool avgeflag = false);
    void setData(QVector<QString>   vSolarInfo, QVector<QDate> date);
    void setCoorManage(CScheduleCoorManage *coor)
    {
        m_coorManage = coor;
    }
protected:
    void paintEvent ( QPaintEvent *e);
private:

    QVector<QString>      m_vSolarDayInfo;
    QVector<QDate>        m_vDate;
    bool                  m_GradientFlag;
    QColor                m_color1;
    QColor                m_color2;
    QColor                m_textcolor;
    QFont                 m_font;
    QPoint                m_pos;
    CScheduleCoorManage                         *m_coorManage;
};
#endif // CSHCEDULEDAYVIEW_H
