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
#ifndef WEEKWINDOW_H
#define WEEKWINDOW_H

#include <QWidget>
#include <DMainWindow>
#include <QDate>
#include <QLabel>
#include <dimagebutton.h>
#include <darrowbutton.h>
#include <dbasebutton.h>
#include <dlabel.h>
#include "calendardbus.h"
DWIDGET_USE_NAMESPACE

class CWeekHeadView;
class CaLunarDayInfo;
class CWeekView;
class CScheduleView;
class CWeekWindow: public QMainWindow
{
    Q_OBJECT
public:
    CWeekWindow(QWidget *parent = 0);
    ~CWeekWindow();
    void setDate(QDate date);
    void setFirstWeekday(int weekday);
    void setLunarVisible(bool state);
    void initUI();
    void initConnection();
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
    void slotprev();
    void slotnext();
    void slottoday();
    void slotCurrentWeek(QDate date, QDate currentDate);
    void slotcurrentDateLunarChanged(QDate date,  CaLunarDayInfo detail, int type = 0);
    void slotcurrentDateChanged(QDate date);
private:
    CWeekHeadView     *m_weekHeadView; //周视图头
    QFrame *m_contentBackground = nullptr;
    DArrowButton      *m_prevButton = nullptr;
    DArrowButton      *m_nextButton = nullptr;
    DBaseButton       *m_today = nullptr;
    QDate              m_currentdate;
    DLabel            *m_YearLabel;
    DLabel            *m_YearLunarLabel;
    DLabel            *m_weekLabel;
    CWeekView         *m_weekview = nullptr;
    CScheduleView     *m_scheduleView;
};

#endif // YEARWINDOW_H
