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
signals:
    void signalsUpdateShcedule(int id = 0);
public slots:
    void slotdeleteitem(CSchceduleSearchItem *item);
    void slotedititem(CSchceduleSearchItem *item);
    void slotsetSearch(QString str);
private:
    void updateDateShow();
    void createItemWidget(ScheduleDtailInfo info);
    void createItemWidget(QDate date);
private:
    DListWidget                                 *m_gradientItemList; //下拉列表窗
    bool                                         m_widgetFlag;
    QVector<ScheduleDateRangeInfo>               m_vlistData;
    int                                          m_type;
    QDate                                        m_currentDate;
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
    void setData(ScheduleDtailInfo  vScheduleInfo);
    const ScheduleDtailInfo &getData() const
    {
        return m_ScheduleInfo;
    }
signals:
    void signalsDelete(CSchceduleSearchItem *item);
    void signalsEdit(CSchceduleSearchItem *item);
public slots:
    void slotEdit();
    void slotDelete();
protected:
    void paintEvent ( QPaintEvent *e);
    void contextMenuEvent(QContextMenuEvent *event);
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
