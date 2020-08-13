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
#ifndef YEARWINDOW_H
#define YEARWINDOW_H

#include "animationstackedwidget.h"
#include "calendardbus.h"

#include <DImageButton>
#include <DPushButton>
#include <DFrame>
#include <DIconButton>
#include <DLabel>
#include <DMainWindow>

#include <QWidget>
#include <QDate>
#include <QLabel>
#include <QHBoxLayout>

DWIDGET_USE_NAMESPACE

class CYearView;
class CaLunarDayInfo;
class CSchceduleSearchView;
class YearFrame;
class CustomFrame;
class CYearWindow: public QMainWindow
{
    Q_OBJECT
public:
    CYearWindow(QWidget *parent = nullptr);
    ~CYearWindow() Q_DECL_OVERRIDE;
    void setDate(QDate date);
    void initUI();
    void initConnection();
    void setLunarVisible(bool state);
    void setTheMe(int type = 0);
    void setSearchWFlag(bool flag);
    void clearSearch();
    void setSearchText(QString str);
    void getScheduleInfo();
signals:
    void dateSelected(const QDate date, const CaLunarDayInfo &detail) const;
    void signalsReturnTodayUpdate(QMainWindow *w);
    void signalsWUpdateShcedule(QMainWindow *w, int id = 0);
    void signaldoubleclickDate(QDate date);
    void signalselectMonth(QDate date);
    void signalselectWeekwindow(QDate date);
    void signalCurrentDate(QDate date);
    void signalupdateschcedule();
private slots:
    void slotprev();
    void slotnext();
    void slottoday();
    void slotsearchDateSelect(QDate date);
    void slotTransitSearchSchedule(int id = 0);
public slots:
    void slotSetSchceduleHide();
    void slotReturnTodayUpdate();
    void slotupdateSchedule(const int id);
    void setYearData();
    void slotUpdateCurrentDate(const QDate &date);
protected:
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
private:
    DFrame *m_contentBackground = nullptr;
    DIconButton *m_prevButton = nullptr;
    DIconButton *m_nextButton = nullptr;
    QLabel *m_today = nullptr;
    QDate                       m_currentdate;
    QLabel *m_YearLabel = nullptr;
    QLabel *m_YearLunarLabel = nullptr;
    QLabel *m_YearLunarDayLabel = nullptr;
    YearFrame *m_YearWidget = nullptr;
    YearFrame *YearWidget_First = nullptr;
    YearFrame *YearWidget_Second = nullptr;
    CustomFrame *m_todayframe = nullptr;
    AnimationStackedWidget *m_StackedWidget = nullptr;
    QHBoxLayout *m_tmainLayout = nullptr;
    QString                     m_searchText;
    bool                        m_searchfalg = false;
    DWidget *m_topWidget = nullptr;
};

class YearFrame : public DFrame
{
    Q_OBJECT
public:
    explicit YearFrame(DWidget *parent = nullptr);
    ~YearFrame() Q_DECL_OVERRIDE;
    void setDate(QDate &date);
    void getInfoAndSetLineFlag();
    void setTheMe(int type = 0);
    void setSearchWFlag(bool flag);
    QString getLunarYear()const
    {
        return m_LunarYear;
    }
    QString getLunarDay()const
    {
        return m_LunarDay;
    }
    void getLunarData();
protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
signals:
    void signaldoubleclickDate(QDate date);
    void signalselectWeekwindow(QDate date);
    void signalselectMonth(QDate date);
    void signalUpdateYearDate(const QDate &date);
    void signalupdateschcedule();
public slots:
    void slotHideInfo();
    void slotSelectInfo(bool flag);
    void slotupdateSchedule(const int id);
    void slotSetSchceduleHide();
    void slotcurrentDateChanged(QDate date);
private:
    QList<CYearView *>          m_monthViewList;
    QLabel *m_YearLabel = nullptr;
    QLabel *m_YearLunarLabel = nullptr;
    CalendarDBus *m_DBusInter = nullptr;
    QDate                       m_currentdate;
    bool                        m_searchfalg = false;
    bool                        m_selectFlag = false;
    QString                     m_LunarYear;
    QString                     m_LunarDay;
    DWidget *m_topWidget = nullptr;
};

#endif // YEARWINDOW_H
