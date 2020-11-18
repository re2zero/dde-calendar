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
    /**
     * @brief setTheMe 根据系统主题类型设置颜色
     * @param type  系统主题类型
     */
    void setTheMe(int type = 0);
    /**
     * @brief isDragging 是否可以拖拽
     * @return 是否可以拖拽
     */
    bool isDragging()const
    {
        return m_MonthGraphicsView->getDragStatus() != 4;
    }
public:
    /**
     * @brief CMonthView 构造函数
     * @param parent 父类
     */
    explicit CMonthView(QWidget *parent = nullptr);
    /**
      * @brief ~CMonthView 析构函数
      */
    ~CMonthView() override;
    /**
     * @brief setFirstWeekday 设置每周的第一天是周几
     * @param weekday 周几
     */
    void setFirstWeekday(int weekday);
signals:
    /**
     * @brief signalcurrentLunarDateChanged 当前时间改变的信号
     * @param date 改变的日期
     * @param detail 阴历信息
     * @param type 是否显示阴历信息
     */
    void signalcurrentLunarDateChanged(QDate date,  CaLunarDayInfo detail, int type = 0);
    /**
     * @brief signalcurrentDateChanged 当前日期改变的信号
     * @param date 日期
     */
    void signalcurrentDateChanged(QDate date);
    /**
     * @brief signalsScheduleUpdate 更新日程的信号
     * @param id
     */
    void signalsScheduleUpdate(int id = 0);
    /**
     * @brief signalsCurrentScheduleDate
     * @param date
     */
    void signalsCurrentScheduleDate(QDate date);
    /**
     * @brief signalViewtransparentFrame 设置窗口的焦点
     * @param type
     */
    void signalViewtransparentFrame(int type);
    /**
     * @brief signalsViewSelectDate 切换视图
     * @param date 日期
     */
    void signalsViewSelectDate(QDate date);
public slots:
    /**
     * @brief setCurrentDate 设置当前的日程
     * @param date 日期
     */
    void setCurrentDate(const QDate date);
    /**
     * @brief setLunarVisible 设置是否显示阴历信息
     * @param visible 是否显示阴历信息
     */
    void setLunarVisible(bool visible);
    /**
     * @brief slotScheduleUpdate 更新日程信息
     */
    void slotScheduleUpdate();
    /**
     * @brief setSelectSchedule 设置选择的日程
     * @param scheduleInfo 日程信息
     */
    void setSelectSchedule(const ScheduleDtailInfo &scheduleInfo);
public slots:
    /**
     * @brief slotsupdatescheduleD 更新日程
     * @param w 判断是否是当前的widget
     * @param data 日程信息
     */
    void slotsupdatescheduleD(QWidget *w, QVector<ScheduleDateRangeInfo> &data);
    /**
     * @brief slotdelete 发送更新日程的信号
     * @param id
     */
    void slotdelete(int id);
    /**
     * @brief slotScheduleRemindWidget 日程浮框
     * @param isShow 是否显示日程浮框
     * @param out 日程信息
     */
    void slotScheduleRemindWidget(const bool isShow, const ScheduleDtailInfo &out = ScheduleDtailInfo());
signals:
    /**
     * @brief signalsupdatescheduleD  更新日程的信号
     * @param w widget
     * @param begin 开始日期
     * @param end 结束日期
     */
    void signalsupdatescheduleD(QWidget *w, QDate begin, QDate end);
    /**
     * @brief signalAngleDelta      发送滚动信号滚动相对量
     * @param delta     滚动相对量
     */
    void signalAngleDelta(int delta);
protected:
    /**
     * @brief resizeEvent 窗口大小调整
     * @param event 窗口大小调整事件
     */
    void resizeEvent(QResizeEvent *event) override;
    /**
     * @brief mousePressEvent 鼠标单击
     * @param event 鼠标事件
     */
    void mousePressEvent(QMouseEvent *event) override;
private:
    /**
     * @brief getDateIndex 获取日期的索引值
     * @param date 日期
     * @return 索引值
     */
    int getDateIndex(const QDate &date) const;

    const QDate getCellDate(int pos);
    const CaLunarDayInfo getCaLunarDayInfo(const QDate &date);
    void updateDate();
    void updateCurrentLunar(const CaLunarDayInfo &info);
    ScheduleDtailInfo getScheduleInfo(const QDate &beginDate, const QDate &endDate);
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
