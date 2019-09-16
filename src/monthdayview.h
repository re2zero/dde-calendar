/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
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
#ifndef MONTHDAYVIEW_H
#define MONTHDAYVIEW_H

#include <QObject>
#include <DWidget>
#include <QDate>
DWIDGET_USE_NAMESPACE
class CMonthDayView : public DWidget
{
    Q_OBJECT
public:
    explicit CMonthDayView(QWidget *parent = nullptr);
    ~CMonthDayView();
    void setCurrentDate(const QDate date);
    void setRCurrentDate(const QDate date);
    void setTheMe(int type = 0);
public slots:
    void cellClicked(QWidget *cell);
signals:
    void signalsSelectDate(QDate date);
    void signalsCurrentDate(QDate date);
private:
    void paintCell(QWidget *cell);
    bool eventFilter(QObject *o, QEvent *e);
    void setSelectedCell(int index, int type = 0);
private:
    QList<QWidget *> m_cellList;
    QDate m_selectDate;
    QDate m_days[12];
    bool m_cellSelectable = true;
    int m_selectedCell = 0;
    QFont m_dayNumFont;

    QColor m_defaultTextColor = Qt::black;
    QColor m_backgrounddefaultColor = Qt::white;
    QColor m_currentDayTextColor = Qt::white;
    QColor m_backgroundcurrentDayColor = "#0081FF";

};

#endif // MONTDAYVIEW_H
