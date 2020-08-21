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

#ifndef MONTHVIEW_H
#define MONTHVIEW_H

#include "monthweekview.h"
#include "calendardbus.h"
#include "SchecduleRemindWidget.h"
#include "monthgraphiview.h"

#include <DWidget>

#include <QList>
#include <QDate>
#include <QStyleOption>
#include <QSignalMapper>
#include <QAction>
#include <QGraphicsView>

DWIDGET_USE_NAMESPACE

class CMonthView: public DWidget
{
    Q_OBJECT
public:
    void setTheMe(int type = 0);
    bool isDragging()const
    {
        return m_MonthGraphicsView->getDragStatus() !=4;
    }
public:
    explicit CMonthView(QWidget *parent = nullptr);
    ~CMonthView() override;
    void setFirstWeekday(int weekday);
signals:
    void signalcurrentLunarDateChanged(QDate date,  CaLunarDayInfo detail, int type = 0);
    void signalcurrentDateChanged(QDate date);
    void signalsSchceduleUpdate(int id = 0);
    void signalsCurrentScheduleDate(QDate date);
    void signalViewtransparentFrame(int type);
    void signalsViewSelectDate(QDate date);
public slots:
    void setCurrentDate(const QDate date);
    void setLunarVisible(bool visible);
    void slotSchceduleUpdate();
    void slotUpdateUI(int type);
    void setSelectSchedule(const ScheduleDtailInfo &scheduleInfo);
public slots:
    void slotsupdatescheduleD(QWidget *w, QVector<ScheduleDateRangeInfo> &data);
    void slotdelete(int id);
    void slotScheduleRemindWidget(const bool isShow, const ScheduleDtailInfo &out = ScheduleDtailInfo());
signals:
    void signalsupdatescheduleD(QWidget *w, QDate begin, QDate end);
protected:
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
private:
    int getDateIndex(const QDate &date) const;
    const QDate getCellDate(int pos);
    const CaLunarDayInfo getCaLunarDayInfo(const QDate &date);
    void updateDate();
    void updateCurrentLunar(const CaLunarDayInfo &info);
    ScheduleDtailInfo getScheduleInfo(const QDate &beginDate,const QDate &endDate);
private slots:
    void getDbusData();
private:
    CMonthGraphiview *m_MonthGraphicsView = nullptr;
    CalendarDBus *m_DBusInter = nullptr;
    QDate                   m_days[42];
    QDate                   m_currentDate;

    QDate m_createDate;
    QMap<QDate, CaLunarDayInfo> *lunarCache = nullptr;
    QVector<FestivalInfo> m_festivallist;

    CMonthWeekView *m_weekIndicator = nullptr;
    int m_firstWeekDay = 0;
    QAction *m_createAction = nullptr; // 创建日程
    QVBoxLayout *m_mainLayout = nullptr;
    int                     m_leftmaagin = 0;
    int                     m_topmagin = 0;
    bool                    m_sflag = true;
    SchecduleRemindWidget *m_RemindWidget = nullptr;

    QPoint                  m_PressPoint;
    bool                    isCreate;
    QDate                   m_PressDate;
    QDate                   m_MoveDate;
};

#endif // MYCALENDARWIDGET_H
