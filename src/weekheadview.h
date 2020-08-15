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

#ifndef WEEKHEADVIEW_H
#define WEEKHEADVIEW_H

#include "constants.h"
#include "calendardbus.h"

#include <DLabel>
#include <DFrame>

#include <QWidget>
#include <QList>
#include <QDate>
#include <QStyleOption>
#include <QSignalMapper>

DWIDGET_USE_NAMESPACE

enum CalendarWeekDayType {
    SO_MFestival = QStyleOption::SO_CustomBase + 0x01,
    SO_MWeekends = QStyleOption::SO_CustomBase + 0x02,
    SO_MWeekendsAndFestival = SO_MFestival | SO_MWeekends,
    SO_MNotCurrentMonth = 0x04,
    SO_MNotCurrentMonthFestival = SO_MNotCurrentMonth | SO_MFestival,
    SO_MDefault,
};
class CustomFrame;
class CWeekHeadView: public DFrame
{
    Q_OBJECT
public:
    enum ShowState {
        ShowLunar = 0x01,
        ShowLunarFestivalHighlight = 0x02,
        Normal = ShowLunar | ShowLunarFestivalHighlight,
    };

public:
    explicit CWeekHeadView(QWidget *parent = nullptr);
    ~CWeekHeadView() override;
    void setFirstWeekday(int weekday);
    int getDateType(const QDate &date);
    inline bool cellSelectable() const
    {
        return m_cellSelectable;
    }
    void setTheMe(int type = 0);
    void setWeekDay(QVector<QDate> vDays);
    void setMounthLabelWidth(int w, int rw);
signals:
    void dateSelected(const QDate date, const CaLunarDayInfo &detail) const;
    void signalcurrentLunarDateChanged(QVector<QDate> vdate, QVector<CaLunarDayInfo> vdetail, int type = 0);
    void signalcurrentDateChanged(QDate date);
    void currentDateChanged(const int year, const int month);
    void currentFestivalChanged(const QString &festival);
    void cellSelectableChanged(bool cellSelectable) const;
    void signalsViewSelectDate(QDate date);
    void signaleSchedulHide();
public slots:
    void setCurrentDate(const QDate date);
    void setLunarVisible(bool visible);
    void setLunarFestivalHighlight(bool highlight);
    void setCellSelectable(bool selectable);
    void handleCurrentDateChanged(const QDate date, const CaLunarDayInfo &detail);
private:
    int getDateIndex(const QDate &date) const;
    const QString getCellDayNum(int pos);
    const QDate getCellDate(int pos);
    const QString getLunar(int pos);
    const CaLunarDayInfo getCaLunarDayInfo(int pos);
    void paintCell(QWidget *cell);
    bool eventFilter(QObject *o, QEvent *e) override;
    void updateDate();
    void updateCurrentLunar(const CaLunarDayInfo &info);

    int checkDay(int weekday);
protected:
    void mousePressEvent(QMouseEvent *event) override;
private slots:
    void cellClicked(QWidget *cell);
    void setSelectedCell(int index);
    void getDbusData();
private:
    QList<QWidget *> m_cellList;
    CustomFrame *m_monthLabel = nullptr;
    CalendarDBus *m_DBusInter = nullptr;
    QDate m_days[7];
    QDate m_currentDate;
    QStringList m_monthList;
    ShowState m_showState = Normal;
    int m_selectedCell = 0;
    bool m_cellSelectable = true;

    QFont m_dayNumFont;
    QFont m_monthFont;

    QColor m_backgroundCircleColor = "#2ca7f8";
    QColor m_backgroundShowColor = "#2CA7F8";
    QColor m_defaultTextColor = "#6F6F6F";
    QColor m_currentDayTextColor = "#FFFFFF";
    QColor m_defaultLunarColor = "#898989";
    QColor m_weekendsTextColor = "#0887FF";
    QColor m_currentMonthColor = "#000000";
    QColor m_backgroudColor = "#E6EEF2";
    QColor m_solofestivalLunarColor = "#4DFF7272";
    QQueue<int> *queue = nullptr;
    QMap<QDate, CaLunarDayInfo> *lunarCache = nullptr;
    CaLunarDayInfo *emptyCaLunarDayInfo = nullptr;
    int m_firstWeekDay;
    int m_themetype = 1;
    int m_monthW = 80;
    int m_fixwidth = 200;
    const int m_radius = 8;
};

#endif // MYCALENDARWIDGET_H
