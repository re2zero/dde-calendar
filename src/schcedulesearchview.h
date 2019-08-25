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

#include <QLabel>
#include <QPushButton>
#include "schedulestructs.h"
#include <QListWidget>
class QVBoxLayout;
class CSchceduleSearchItem;
class CSchceduleSearchView : public QWidget
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
    CSchceduleSearchItem *createItemWidget(int index, bool average = false);
private:
    QListWidget                                 *m_gradientItemList; //下拉列表窗
    bool                                         m_widgetFlag;
    QVector<ScheduleInfo>                        m_vlistData;
    int                                          m_type;
    QDate                                        m_currentDate;
};

class CSchceduleSearchItem : public QPushButton
{
    Q_OBJECT

public:
    explicit CSchceduleSearchItem(QWidget *parent = nullptr);
    void setBackgroundColor(QColor color1);
    void setText(QColor tcolor, QFont font);

    void setData(ScheduleInfo  vScheduleInfo);
    const ScheduleInfo &getData() const
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
    ScheduleInfo          m_ScheduleInfo;
    QAction              *m_editAction;
    QAction              *m_deleteAction;
    QColor                m_Backgroundcolor;
    QColor                m_textcolor;
    QFont                 m_font;
};

#endif // CSHCEDULEDAYVIEW_H
