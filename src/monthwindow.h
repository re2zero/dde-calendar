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

#include "SchecduleRemindWidget.h"

#include <DPushButton>
#include <DFrame>
#include <DMainWindow>

#include <QWidget>
#include <QDate>
#include <QLabel>
#include <QSpacerItem>

DWIDGET_USE_NAMESPACE
class CSchceduleSearchView;
class CMonthView;
class CMonthDayView;
class QHBoxLayout;
class CTodyButton;
class CMonthWindow: public QMainWindow
{
    Q_OBJECT
public:
    CMonthWindow(QWidget *parent = nullptr);
    ~CMonthWindow() override;
    void setFirstWeekday(int weekday);
    void setDate(QDate date);
    void setLunarVisible(bool state);
    void setTheMe(int type = 0);
    void setSearchWFlag(bool flag);
    void clearSearch();
public slots:
    void previousMonth();
    void nextMonth();
    void slotsearchDateSelect(QDate date);
protected:
    void wheelEvent(QWheelEvent *) override;
private:
    void initUI();
    void initConnection();
    void initLunar();
    void slideMonth(bool next);
signals:
    void dateSelected(const QDate date, const CaLunarDayInfo &detail) const;
    void signalsWUpdateShcedule(QMainWindow *w, int id = 0);
    void signalsReturnTodayUpdate(QMainWindow *w);
    void signalsCurrentScheduleDate(QDate date);
    void signalsViewSelectDate(QDate date);
    void signalViewtransparentFrame(int type);
    void signalCurrentDate(QDate date);
public slots:
    void slotReturnTodayUpdate();
    void slotScheduleHide();
public slots:
    void slotupdateSchedule(int id = 0);
    void slotTransitSchedule(int id = 0);
    void setSelectSchedule(const ScheduleDtailInfo &scheduleInfo);
protected:
    void resizeEvent(QResizeEvent *event) override;
private slots:
    void slottoday();
    void slotcurrentDateLunarChanged(QDate date,  CaLunarDayInfo detail, int type = 0);
    void slotcurrentDateChanged(QDate date);
    void slotSelectedMonth(QDate date);
private:
    CMonthView *m_monthView = nullptr;
    CMonthDayView *m_monthDayView = nullptr;
    DFrame *m_contentBackground = nullptr;
    CTodyButton *m_today = nullptr;
    QDate                   m_currentdate;
    QLabel *m_YearLabel = nullptr;
    QLabel *m_YearLunarLabel = nullptr;
    QSpacerItem *m_spaceitem = nullptr;
    DWidget *m_gridWidget = nullptr;
    bool                    m_searchfalg = false;
    QHBoxLayout *m_tmainLayout = nullptr;
};

#endif // YEARWINDOW_H
