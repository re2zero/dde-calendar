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
DWIDGET_USE_NAMESPACE
class CYearWindow;
class CMonthWindow;
class CWeekWindow;
class CDayWindow;
class Calendarmainwindow : public DMainWindow
{
    Q_OBJECT
public:
    Calendarmainwindow();
private:
    void initUI();
    void initConnection();
    void initLunar();
    void createview();
    DPushButton *createButon(QString name);
private slots:
    void slotstackWClicked(int index);
    void slotWUpdateShcedule(QMainWindow *w, int id = 0);
    void slotReturnTodyUpdate(QMainWindow *w);
    void slotSreturnPressed();
    void slotStextChanged();
private:
    DLabel                    *m_icon;
    QStackedWidget              *m_stackWidget;
    DSegmentedControl         *m_segmentedControl;
    DSearchEdit               *m_searchEdit;
    CYearWindow               *m_yearwindow;
    CMonthWindow              *m_monthWindow;
    CWeekWindow               *m_weekWindow;
    CDayWindow                *m_DayWindow;
};

#endif // CALENDARMAINWINDOW_H
