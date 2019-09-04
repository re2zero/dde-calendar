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
#ifndef MONTHWINDOW_H
#define MONTHWINDOW_H

#include <QWidget>
#include <DMainWindow>
#include <QDate>
#include <QLabel>
#include <DPushButton>
#include <DLabel>
#include "calendardbus.h"
DWIDGET_USE_NAMESPACE

class CMonthView;
class CMonthDayView;
class QPropertyAnimation;
class CMonthWindow: public QMainWindow
{
    Q_OBJECT
public:
    CMonthWindow(QWidget *parent = 0);
    ~CMonthWindow();
    void setFirstWeekday(int weekday);
    void setDate(QDate date);
    void setLunarVisible(bool state);
public slots:
    void previousMonth();
    void nextMonth();

protected:
    void wheelEvent(QWheelEvent *);
private:
    void initUI();
    void initConnection();
    void initLunar();
    void slideMonth(bool next);
signals:
    void dateSelected(const QDate date, const CaLunarDayInfo &detail) const;
    void signalsWUpdateShcedule(QMainWindow *w, int id = 0);
    void signalsReturnTodayUpdate(QMainWindow *w);
public slots:
    void slotReturnTodayUpdate();
public slots:
    void slotupdateSchedule(int id = 0);
    void slotTransitSchedule(int id = 0);
private slots:
    void slottoday();
    void slotcurrentDateLunarChanged(QDate date,  CaLunarDayInfo detail, int type = 0);
    void slotcurrentDateChanged(QDate date);
    void slotSelectedMonth(QDate date);
private:
    QFrame *m_animationContainer = nullptr;
    QLabel *m_icon;

    CMonthView        *m_monthView;
    CMonthDayView      *m_monthDayView;
    QFrame *m_contentBackground = nullptr;
    DPushButton       *m_today = nullptr;
    QDate              m_currentdate;
    DLabel            *m_YearLabel;
    DLabel            *m_YearLunarLabel;
};

#endif // YEARWINDOW_H
