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
#include "constants.h"
#include <QWidget>
#include <QList>
#include <QDate>
#include <QStyleOption>
#include <QSignalMapper>
#include <QAction>
#include "calendardbus.h"
class CSchceduleDayView;
enum CalendarMonthDayType {
    SO_MFestival = QStyleOption::SO_CustomBase + 0x01,
    SO_MWeekends = QStyleOption::SO_CustomBase + 0x02,
    SO_MWeekendsAndFestival = SO_MFestival | SO_MWeekends,
    SO_MNotCurrentMonth = 0x04,
    SO_MNotCurrentMonthFestival = SO_MNotCurrentMonth | SO_MFestival,
    SO_MDefault,
};

class CMonthView: public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor backgroundCircleColor MEMBER m_backgroundCircleColor DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY(QColor defaultTextColor MEMBER m_defaultTextColor DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY(QColor defaultLunarColor MEMBER m_defaultLunarColor DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY(QColor festivalLunarColor MEMBER m_festivalLunarColor DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY(QColor weekendsTextColor MEMBER m_weekendsTextColor DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY(QColor weekendsLunarColor MEMBER m_weekendsLunarColor DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY(QColor topBorderColor MEMBER m_topBorderColor DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY(bool cellSelectable READ cellSelectable WRITE setCellSelectable NOTIFY cellSelectableChanged)

public:
    enum ShowState {
        ShowLunar = 0x01,
        ShowLunarFestivalHighlight = 0x02,
        Normal = ShowLunar | ShowLunarFestivalHighlight,
    };

public:
    explicit CMonthView(QWidget *parent = 0);
    void setFirstWeekday(int weekday);
    int getDateType(const QDate &date);
    inline bool cellSelectable() const
    {
        return m_cellSelectable;
    }

signals:
    void dateSelected(const QDate date, const CaLunarDayInfo &detail) const;
    void signalcurrentLunarDateChanged(QDate date,  CaLunarDayInfo detail, int type = 0);
    void signalcurrentDateChanged(QDate date);
    void currentDateChanged(const int year, const int month);
    void currentFestivalChanged(const QString &festival);
    void cellSelectableChanged(bool cellSelectable) const;
    void signalsSchceduleUpdate(int id = 0);
public slots:
    void setCurrentDate(const QDate date);
    void setLunarVisible(bool visible);
    void setLunarFestivalHighlight(bool highlight);
    void setCellSelectable(bool selectable);
    void handleCurrentDateChanged(const QDate date, const CaLunarDayInfo &detail);
    void slotCtrlSchceduleUpdate(QDate date, int type = 0);
    void slotSchceduleUpdate(int id = 0);
private:
    int getDateIndex(const QDate &date) const;
    const QString getCellDayNum(int pos);
    const QDate getCellDate(int pos);
    const QString getLunar(int pos);
    const CaLunarDayInfo getCaLunarDayInfo(int pos);
    void paintCell(QWidget *cell);
    bool eventFilter(QObject *o, QEvent *e);
    void updateDate();
    void updateCurrentLunar(const CaLunarDayInfo &info);

private slots:
    void cellClicked(QWidget *cell);
    void setSelectedCell(int index);
    void getDbusData();

    void slotCreate();
private:
    QList<QWidget *> m_cellList;
    QList<CSchceduleDayView *> m_cellScheduleList;
    CalendarDBus *m_DBusInter;
    QDate m_days[42];
    QDate m_currentDate;

    ShowState m_showState = Normal;
    int m_selectedCell = 0;
    bool m_cellSelectable = true;

    QFont m_dayNumFont;
    QFont m_dayLunarFont;

    QColor m_topBorderColor = Qt::red;
    QColor m_backgroundCircleColor = "#2ca7f8";

    QColor m_defaultTextColor = Qt::black;
    QColor m_currentDayTextColor = "#2ca7f8";
    QColor m_weekendsTextColor = Qt::black;
    QColor m_selectedTextColor = Qt::white;
    QColor m_festivalTextColor = Qt::black;
    QColor m_notCurrentTextColor = "#b2b2b2";

    QColor m_defaultLunarColor = "#5E5E5E";
    QColor m_currentDayLunarColor = m_currentDayTextColor;
    QColor m_weekendsLunarColor = m_defaultLunarColor;
    QColor m_selectedLunarColor = Qt::white;
    QColor m_festivalLunarColor = m_defaultLunarColor;
    QColor m_notCurrentLunarColor = "#dfdfdf";
    QColor m_solofestivalLunarColor = "#4DFF7272";

    QQueue<int> *queue = nullptr;
    QMap<QDate, CaLunarDayInfo> *lunarCache = nullptr;
    CaLunarDayInfo *emptyCaLunarDayInfo = nullptr;

    CMonthWeekView *m_weekIndicator;
    int m_firstWeekDay;
    QAction          *m_createAction;     // 创建日程
};

#endif // MYCALENDARWIDGET_H
