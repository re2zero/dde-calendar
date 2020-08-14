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
#ifndef DAYWINDOW_H
#define DAYWINDOW_H

#include "calendardbus.h"
#include "schedulestructs.h"

#include <DMainWindow>
#include <DFrame>

#include <QWidget>
#include <QDate>
#include <QLabel>
DWIDGET_USE_NAMESPACE

class CDayMonthView;
class CScheduleView;
class CSchceduleSearchView;
class QHBoxLayout;
class CustomFrame;
class CDayWindow: public QMainWindow
{
    Q_OBJECT
public:
    CDayWindow(QWidget *parent = nullptr);
    ~CDayWindow();
    void setDate(QDate date);
    void setSelectSchedule(const ScheduleDtailInfo &scheduleInfo);
    void setSearchWFlag(bool flag);
    void clearSearch();
    void setSearchText(QString str);
    void setLunarVisible(bool state);
    void setTheMe(int type = 0);
    void setTime(QTime time);
    void updateHigh();
private:
    void initUI();
    void initConnection();
signals:
    void dateSelected(const QDate date, const CaLunarDayInfo &detail) const;
    void signalsWUpdateShcedule(QMainWindow *w, int id = 0);
    void signalsReturnTodayUpdate(QMainWindow *w);
    void signalViewtransparentFrame(int type);
    void signalCurrentDate(QDate date);
public slots:
    void slotupdateSchedule(int id = 0);
    void slotTransitSchedule(int id = 0);
    void slotTransitSearchSchedule(int id = 0);
    void slotReturnTodayUpdate();
    void slotCurrentReturnDay();
    void slotScheduleHide();
protected:
    void resizeEvent(QResizeEvent *event) override;
private slots:
    void slotcurrentDateLunarChanged(QDate date,  CaHuangLiDayInfo detail, int type = 0);
    void slotcurrentDateChanged(QDate date);
    void slotsearchDateSelect(QDate date);
    void slotIsDragging(bool &isDragging);
private:
    CDayMonthView *m_daymonthView = nullptr;
    DFrame *m_contentBackground = nullptr;
    CustomFrame *m_leftground = nullptr;
    DVerticalLine *m_verline = nullptr;
    QDate              m_currentdate;
    QLabel *m_YearLabel = nullptr;
    QLabel *m_LunarLabel = nullptr;
    QLabel *m_SolarDay = nullptr;
    CScheduleView *m_scheduleView = nullptr;
    QString m_searchText;
    bool m_searchfalg = false;
    QHBoxLayout *m_mainLayout = nullptr;
};

#endif // YEARWINDOW_H
