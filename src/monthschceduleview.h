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
#ifndef MONTHSCHCEDULEVIEW_H
#define MONTHSCHCEDULEVIEW_H

#include <DLabel>
#include <QMouseEvent>
#include <DPushButton>
#include "schedulestructs.h"
#include <DListWidget>
DWIDGET_USE_NAMESPACE
class CMonthSchceduleWidgetItem;
class QVBoxLayout;
class CMonthSchceduleNumButton;
class CMonthSchceduleView : public QObject
{
    Q_OBJECT

public:
    CMonthSchceduleView(QWidget *parent);
    ~CMonthSchceduleView();
    void setData(QVector<ScheduleDateRangeInfo> &data);
    void setTheMe(int type = 0);
signals:
    void signalsUpdateShcedule(int id = 0);
public slots:
    void slotdeleteitem(CMonthSchceduleWidgetItem *item);
    void slotedititem(CMonthSchceduleWidgetItem *item, int type = 0);
private:
    void updateData();
    void updateDateShow();
    CMonthSchceduleWidgetItem *createItemWidget(int index, bool average = false);
private:
    QVector<CMonthSchceduleWidgetItem *>         m_scheduleShowItem;
    QVector<CMonthSchceduleNumButton *>          m_DayShowItem;
    QVector<ScheduleDateRangeInfo>               m_data;
    int                                          m_cNum = 2;//日程层数
    QWidget                                     *m_parernt;
};

class CMonthSchceduleNumButton : public DPushButton
{
    Q_OBJECT

public:
    CMonthSchceduleNumButton(QWidget *parent = nullptr);
    ~CMonthSchceduleNumButton();
    void setColor(QColor color1, QColor color2, bool GradientFlag = false);
    void setText(QColor tcolor, QFont font, QPoint pos);
    void setData(int  num)
    {
        m_num = num;
    };
protected:
    void paintEvent(QPaintEvent *e);
private:
    bool                  m_GradientFlag;
    QColor                m_color1;
    QColor                m_color2;
    QColor                m_textcolor;
    QFont                 m_font;
    QPoint                m_pos;
    int                   m_num;
};

class CMonthSchceduleWidgetItem : public DPushButton
{
    Q_OBJECT

public:
    explicit CMonthSchceduleWidgetItem(QWidget *parent = nullptr, int edittype = 0);
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
    const ScheduleDtailInfo &getData() const
    {
        return m_ScheduleInfo;
    }
signals:
    void signalsDelete(CMonthSchceduleWidgetItem *item);
    void signalsEdit(CMonthSchceduleWidgetItem *item, int type = 0);
public slots:
    void slotEdit();
    void slotDelete();
    void slotDoubleEvent(int type = 0);
protected:
    void paintEvent ( QPaintEvent *e);
    void contextMenuEvent(QContextMenuEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
private:
    ScheduleDtailInfo     m_ScheduleInfo;
    QAction              *m_editAction;
    QAction              *m_deleteAction;
    bool                  m_GradientFlag;
    QColor                m_color1;
    QColor                m_color2;
    QColor                m_textcolor;
    QFont                 m_font;
    QPoint                m_pos;
    QListWidgetItem      *m_item;
    bool                  m_avgeflag;
    int                   m_editType = 0;
};

#endif // CSHCEDULEDAYVIEW_H
