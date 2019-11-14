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

#ifndef CALENDARMAINWINDOW_H
#define CALENDARMAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <DMainWindow>
#include <dstackwidget.h>
#include <DPushButton>
#include <dsearchedit.h>
#include <QButtonGroup>
#include <DTitlebar>
#include <DLabel>
#include <QStackedWidget>
#include <DSegmentedControl>
#include <DButtonBox>
#include <QStackedLayout>
#include "scheduledatamanage.h"
#include <DFrame>
DWIDGET_USE_NAMESPACE
class CYearWindow;
class CMonthWindow;
class CWeekWindow;
class CDayWindow;
class __Scheduler;
class CSchceduleSearchView;
class Calendarmainwindow : public DMainWindow
{
    Q_OBJECT
public:
    Calendarmainwindow(QWidget *w = NULL);
    //void Invoke(const QString &mothodName, const QString &content);
    bool analysisCreate(const QString &content, ScheduleDtailInfo &info);
    void viewWindow(int type, QDateTime datetime);
    void UpdateJob();
public slots:
    void slotTheme(int type);
    void OpenSchedule(QString job);
    void ActiveWindow();
    void onViewShortcut();
    void slotmaxminViewShortcut();
private:
    void initUI();
    void initConnection();
    void initLunar();
    void createview();
    DPushButton *createButon(QString name);
protected:
    void resizeEvent(QResizeEvent *event);
private slots:
    void slotstackWClicked(QAbstractButton *bt);
    void slotWUpdateShcedule(QMainWindow *w, int id = 0);
    void slotReturnTodyUpdate(QMainWindow *w);
    void slotSreturnPressed();
    void slotStextChanged();
    void slotJobsUpdated(const QList<qlonglong> &Ids);
    void slotSearchEdit();
    void slotTransitSearchSchedule(int id = 0);
    void slotsearchDateSelect(QDate date);

    void slotdoubleclickDate(QDate date);
    void slotselectMonth(QDate date);
private:
    DLabel                    *m_icon;
    QStackedLayout              *m_stackWidget;
    //DSegmentedControl         *m_segmentedControl;
    DButtonBox                *m_buttonBox;
    DButtonBoxButton          *m_yearButton;
    DButtonBoxButton          *m_monthButton;
    DButtonBoxButton          *m_weekButton;
    DButtonBoxButton          *m_dayButton;
    DSearchEdit               *m_searchEdit;
    CYearWindow               *m_yearwindow;
    CMonthWindow              *m_monthWindow;
    CWeekWindow               *m_weekWindow;
    CDayWindow                *m_DayWindow;
    __Scheduler               *m_dbus;
    bool                      m_searchflag = false;
    CSchceduleSearchView *m_schceduleSearchView;
    DFrame *m_contentBackground = nullptr;
    int                       m_priindex = 3; //默认打开日视图双击
};

#endif // CALENDARMAINWINDOW_H
