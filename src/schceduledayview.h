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
#ifndef SCHCEDULEDAYVIEW_H
#define SCHCEDULEDAYVIEW_H

#include <QComboBox>
#include <QLabel>
#include <QMouseEvent>
#include <QPushButton>
#include "schedulestructs.h"
#include <QListWidget>
class CSchceduleWidgetItem;
class QVBoxLayout;
class CSchceduleNumButton;
class CSchceduleDayView : public QWidget
{
    Q_OBJECT

public:
    CSchceduleDayView(QWidget *parent = nullptr);
    ~CSchceduleDayView();
    void setALLDayData(QVector<ScheduleInfo> &vlistData);
    void setDayData(QVector<ScheduleInfo> &vlistData);
    void setDate(QDate date, int type = 1);
signals:
    void signalsUpdateShcedule(int id = 0);
public slots:
    void slotdeleteitem(CSchceduleWidgetItem *item);
    void slotedititem(CSchceduleWidgetItem *item);
private slots:
    void slothidelistw();
    void slotCreate();
protected:
    void contextMenuEvent(QContextMenuEvent *event);
private:
    void updateDateShow();
    CSchceduleWidgetItem *createItemWidget(int index, bool average = false);
private:
    QAction                                     *m_createAction;     // 创建日程
    QListWidget                                 *m_gradientItemList; //下拉列表窗口
    QWidget                                     *m_widt;             //主窗口
    CSchceduleNumButton                         *m_numButton;
    QVBoxLayout                                 *m_layout;
    bool                                         m_widgetFlag;
    QVector<ScheduleInfo>                          m_vlistData;
    QVector<CSchceduleWidgetItem *>              m_baseShowItem;
    int                                          m_type;
    QDate                                        m_currentDate;
};

class CSchceduleNumButton : public QPushButton
{
    Q_OBJECT

public:
    CSchceduleNumButton(QWidget *parent = nullptr);
    ~CSchceduleNumButton();
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

class CSchceduleWidgetItem : public QPushButton
{
    Q_OBJECT

public:
    explicit CSchceduleWidgetItem(QWidget *parent = nullptr);
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

    void setData(ScheduleInfo  vScheduleInfo);
    const ScheduleInfo &getData() const
    {
        return m_ScheduleInfo;
    }
signals:
    void signalsDelete(CSchceduleWidgetItem *item);
    void signalsEdit(CSchceduleWidgetItem *item);
public slots:
    void slotEdit();
    void slotDelete();
protected:
    void paintEvent ( QPaintEvent *e);
    void contextMenuEvent(QContextMenuEvent *event);
private:
    ScheduleInfo          m_ScheduleInfo;
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
};

#endif // CSHCEDULEDAYVIEW_H
