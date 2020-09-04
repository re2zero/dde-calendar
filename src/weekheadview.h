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
    /**
     * @brief CWeekHeadView 构造函数
     * @param parent 父类
     */
    explicit CWeekHeadView(QWidget *parent = nullptr);
    /**
      * @brief CWeekHeadView 析构函数
      */
    ~CWeekHeadView() override;
    /**
     * @brief setFirstWeekday 设置每周的第一天是周几
     * @param weekday 周几
     */
    void setFirstWeekday(int weekday);
    /**
     * @brief getDateType 获取所给日期的类型CalendarWeekDayType
     * @param date 日期
     * @return 类型
     */
    int getDateType(const QDate &date);
    /**
     * @brief cellSelectable
     * @return
     */
    inline bool cellSelectable() const
    {
        return m_cellSelectable;
    }
    /**
     * @brief setTheMe 根据系统主题类型设置颜色
     * @param type 系统主题类型
     */
    void setTheMe(int type = 0);
    /**
     * @brief setWeekDay 设置一周的时间
     * @param vDays 一周代表的时间
     */
    void setWeekDay(QVector<QDate> vDays);
    /**
     * @brief setMounthLabelWidth  设置
     * @param w
     * @param rw
     */
    void setMounthLabelWidth(int w, int rw);
signals:
    /**
     * @brief dateSelected 选择时间的信号
     * @param date 时间
     * @param detail 时间的阴历信息
     */
    void dateSelected(const QDate date, const CaLunarDayInfo &detail) const;
    /**
     * @brief signalcurrentLunarDateChanged 当前周的阴历日期改变的信号
     * @param vdate 一周的日期
     * @param vdetail 一周的阴历信息
     * @param type 是否显示信息
     */
    void signalcurrentLunarDateChanged(QVector<QDate> vdate, QVector<CaLunarDayInfo> vdetail, int type = 0);
    /**
     * @brief signalcurrentDateChanged 当前时间改变的信号
     * @param date 当前时间
     */
    void signalcurrentDateChanged(QDate date);
    /**
     * @brief currentDateChanged
     * @param year
     * @param month
     */
    void currentDateChanged(const int year, const int month);
    /**
     * @brief currentFestivalChanged 当前的节日信息改变的信号
     * @param festival 节日信息
     */
    void currentFestivalChanged(const QString &festival);
    /**
     * @brief cellSelectableChanged
     * @param cellSelectable
     */
    void cellSelectableChanged(bool cellSelectable) const;
    /**
     * @brief signalsViewSelectDate 选择日期的信号
     * @param date 日期
     */
    void signalsViewSelectDate(QDate date);
    /**
     * @brief signaleSchedulHide 隐藏日程浮框的信号
     */
    void signaleSchedulHide();
public slots:
    /**
     * @brief setCurrentDate 设置当前的时间
     * @param date 日期
     */
    void setCurrentDate(const QDate date);
    /**
     * @brief setLunarVisible 设置是否显示阴历信息
     * @param visible 是否显示阴历信息
     */
    void setLunarVisible(bool visible);
    /**
     * @brief setLunarFestivalHighlight
     * @param highlight
     */
    void setLunarFestivalHighlight(bool highlight);
    /**
     * @brief setCellSelectable
     * @param selectable
     */
    void setCellSelectable(bool selectable);
    /**
     * @brief handleCurrentDateChanged 当前日期改变的信号
     * @param date 日期
     * @param detail unused
     */
    void handleCurrentDateChanged(const QDate date, const CaLunarDayInfo &detail);
private:
    /**
     * @brief getDateIndex 根据日期返回当天的索引
     * @param date 日期
     * @return 日期所在的索引
     */
    int getDateIndex(const QDate &date) const;
    /**
     * @brief getCellDayNum 根据索引值获取当天是在一个月中的第几天
     * @param pos 索引
     * @return 日期的字符串
     */
    const QString getCellDayNum(int pos);
    /**
     * @brief getCellDate 根据索引获取当天的日期
     * @param pos 索引值
     * @return 日期
     */
    const QDate getCellDate(int pos);
    /**
     * @brief getLunar 根据索引值获取当天的阴历信息
     * @param pos 索引值
     * @return 阴历信息字符串
     */
    const QString getLunar(int pos);
    /**
     * @brief getCaLunarDayInfo 根据索引值获取阴历信息
     * @param pos 索引值
     * @return 阴历信息
     */
    const CaLunarDayInfo getCaLunarDayInfo(int pos);
    /**
     * @brief paintCell 绘制周信息
     * @param cell 每天所在的widget
     */
    void paintCell(QWidget *cell);
    /**
     * @brief eventFilter 过滤器
     * @param o 事件对象
     * @param e 时间类型
     * @return false
     */
    bool eventFilter(QObject *o, QEvent *e) override;
    /**
     * @brief updateDate 更新数据
     */
    void updateDate();
    /**
     * @brief updateCurrentLunar 更新当前的阴历信息
     * @param info 阴历信息
     */
    void updateCurrentLunar(const CaLunarDayInfo &info);
    /**
     * @brief checkDay 检查周数，在定制中计算正确的顺序。
     * @param weekday 周数
     * @return 周数
     */
    int checkDay(int weekday);
protected:
    /**
     * @brief mousePressEvent 鼠标单击事件
     * @param event 鼠标事件
     */
    void mousePressEvent(QMouseEvent *event) override;
private slots:
    /**
     * @brief cellClicked
     * @param cell
     */
    void cellClicked(QWidget *cell);
    /**
     * @brief setSelectedCell 设置选择的时间
     * @param index 索引值
     */
    void setSelectedCell(int index);
    /**
     * @brief getDbusData 获取dbus日期
     */
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
    /**
     * @brief m_Background_Weekend_Color 周六周日背景色
     */
    QColor m_Background_Weekend_Color = "#00429A";
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
