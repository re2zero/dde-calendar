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
#ifndef SCHCEDULESEARCHVIEW_H
#define SCHCEDULESEARCHVIEW_H

#include <DLabel>
#include <DPushButton>
#include "schedulestructs.h"
#include <DListWidget>
DWIDGET_USE_NAMESPACE
class QVBoxLayout;
class CSchceduleSearchItem;
class CSchceduleSearchDateItem;
class CSchceduleSearchView : public DWidget
{
    Q_OBJECT

public:
    CSchceduleSearchView(QWidget *parent = nullptr);
    ~CSchceduleSearchView();
    void setTheMe(int type = 0);
    void clearSearch();
signals:
    void signalsUpdateShcedule(int id = 0);
    void signalDate(QDate date);
public slots:
    void slotdeleteitem(CSchceduleSearchItem *item);
    void slotedititem(CSchceduleSearchItem *item);
    void slotsetSearch(QString str);
    void slotSelectDate(QDate date);
protected:
    void resizeEvent(QResizeEvent *event);
private:
    void updateDateShow();
    void createItemWidget(ScheduleDtailInfo info, QDate date);
    QListWidgetItem *createItemWidget(QDate date);
private:
    DListWidget                                 *m_gradientItemList; //下拉列表窗
    bool                                         m_widgetFlag;
    QVector<ScheduleDateRangeInfo>               m_vlistData;
    QVector<DLabel *>                             m_labellist;
    int                                          m_type;
    QDate                                        m_currentDate;
    QColor                m_bBackgroundcolor = "#000000";
    QColor                m_btimecolor = "#526A7F";
    QColor                m_bttextcolor = "#414D68";
    QColor                m_lBackgroundcolor = Qt::white;
    QColor                m_ltextcolor = "#001A2E";
    QListWidgetItem      *m_currentItem;
};

class CSchceduleSearchItem : public DLabel
{
    Q_OBJECT

public:
    explicit CSchceduleSearchItem(QWidget *parent = nullptr);
    void setBackgroundColor(QColor color1);
    void setSplitLineColor(QColor color1);
    void setText(QColor tcolor, QFont font);
    void setTimeC(QColor tcolor, QFont font);
    void setData(ScheduleDtailInfo  vScheduleInfo, QDate date);
    const ScheduleDtailInfo &getData() const
    {
        return m_ScheduleInfo;
    }
signals:
    void signalsDelete(CSchceduleSearchItem *item);
    void signalsEdit(CSchceduleSearchItem *item);
    void signalSelectDate(QDate date);
public slots:
    void slotEdit();
    void slotDelete();
    void slotDoubleEvent(int type = 0);
protected:
    void paintEvent ( QPaintEvent *e);
    void contextMenuEvent(QContextMenuEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
private:
    ScheduleDtailInfo          m_ScheduleInfo;
    QAction              *m_editAction;
    QAction              *m_deleteAction;
    QColor                m_Backgroundcolor;
    QColor                m_timecolor;
    QColor                m_splitlinecolor;
    QFont                 m_timefont;
    QColor                m_ttextcolor;
    QFont                 m_tfont;
    QDate                 m_date;
};
class CSchceduleSearchDateItem : public DLabel
{
    Q_OBJECT

public:
    explicit CSchceduleSearchDateItem(QWidget *parent = nullptr);
    void setBackgroundColor(QColor color1);
    void setText(QColor tcolor, QFont font);
    void setDate(QDate  date);
protected:
    void paintEvent ( QPaintEvent *e);
private:
    QColor                m_Backgroundcolor;
    QColor                m_textcolor;
    QFont                 m_font;
    QDate                 m_date;
};
#endif // CSHCEDULEDAYVIEW_H
