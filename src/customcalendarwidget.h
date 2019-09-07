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
#pragma once

#include <DCalendarWidget>
#include <DImageButton>
DWIDGET_USE_NAMESPACE
class QPushButton;
class QLabel;
class CCaYearMonthEdit;
class CCustomCalendarWidget : public DCalendarWidget
{
    Q_OBJECT

public:
    CCustomCalendarWidget(QWidget *parent);
    ~CCustomCalendarWidget();

private:
    void initControl();
    void initTopWidget();
    void initBottomWidget();
    void setDataLabelTimeText(int year, int month);

signals:
    void signalSetCalendarTime(const QDate &data);

private slots:
    void onbtnClicked();
public slots:
    void slotmonthChange(int value);
    void slotyearChange(int value);
    void slotABtPrivMonth();
    void slotABtNextMonth();
protected:
    void paintCell(QPainter *painter, const QRect &rect, const QDate &date) const;
    void resizeEvent(QResizeEvent *e);
private:
    DImageButton *m_leftMonthBtn;
    DImageButton *m_rightMonthBtn;
    CCaYearMonthEdit *m_monthLabel;
    CCaYearMonthEdit *m_yearLabel;
    QColor m_backgroundCircleColor = "#2ca7f8";

    QColor m_defaultTextColor = Qt::black;
    QColor m_currentDayTextColor = "#2ca7f8";
    QColor m_weekendsTextColor = Qt::black;
    QColor m_selectedTextColor = Qt::white;
    QColor m_notCurrentTextColor = "#b2b2b2";
};
