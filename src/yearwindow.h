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

#include <QWidget>
#include <DMainWindow>
#include <QDate>
#include <QLabel>
#include <DImageButton>
#include <DPushButton>
#include <DLabel>
#include <DFrame>
#include "calendardbus.h"
#include <DIconButton>
#include <QHBoxLayout>
DWIDGET_USE_NAMESPACE

class CYearView;
class CaLunarDayInfo;
class CSchceduleSearchView;
class CYearWindow: public QMainWindow
{
    Q_OBJECT
public:
    CYearWindow(QWidget *parent = 0);
    ~CYearWindow();
    void setDate(QDate date);
    void initUI();
    void initConnection();
    void setLunarVisible(bool state);
    void setTheMe(int type = 0);
    void setSearchWFlag(bool flag);
    void clearSearch();
    void setSearchText(QString str);
signals:
    void dateSelected(const QDate date, const CaLunarDayInfo &detail) const;
    void signalsReturnTodayUpdate(QMainWindow *w);
    void signalsWUpdateShcedule(QMainWindow *w, int id = 0);
    void signaldoubleclickDate(QDate date);
    void signalselectMonth(QDate date);
public slots:
    void slotReturnTodayUpdate();
private slots:
    void slotActiveW(CYearView *w);
    void slotprev();
    void slotnext();
    void slottoday();
    void slotcurrentDateChanged(QDate date);
    void getDbusData();
private slots:
    void slotsearchDateSelect(QDate date);
    void slotTransitSearchSchedule(int id = 0);
protected:
    void resizeEvent(QResizeEvent *event);
    void wheelEvent(QWheelEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);
private:
    const QString getLunar(QDate date);
    const CaLunarDayInfo getCaLunarDayInfo(QDate date);
private:
    QList<CYearView *> m_monthViewList;
    DFrame *m_contentBackground = nullptr;
    DIconButton      *m_prevButton = nullptr;
    DIconButton      *m_nextButton = nullptr;
    DLabel           *m_today = nullptr;
    QDate              m_currentdate;
    DLabel            *m_YearLabel;
    DLabel            *m_YearLunarLabel;
    DLabel            *m_YearLunarDayLabel;
    CYearView         *m_activeview = nullptr;
    DFrame            *m_gridWidget;
    CalendarDBus *m_DBusInter;
    QQueue<QDate> *queue;
    QMap<QDate, CaLunarDayInfo> *lunarCache;
    CaLunarDayInfo *emptyCaLunarDayInfo;
    //CSchceduleSearchView *m_schceduleSearchView;
    QHBoxLayout *m_tmainLayout;
    QString           m_searchText;
    bool m_searchfalg = false;
};

#endif // YEARWINDOW_H
