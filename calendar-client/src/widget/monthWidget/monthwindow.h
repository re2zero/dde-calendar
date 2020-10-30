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
#ifndef MONTHWINDOW_H
#define MONTHWINDOW_H

#include "SchecduleRemindWidget.h"

#include <DPushButton>
#include <DFrame>
#include <DMainWindow>

#include <QWidget>
#include <QDate>
#include <QLabel>
#include <QSpacerItem>

DWIDGET_USE_NAMESPACE
class CSchceduleSearchView;
class CMonthView;
class CMonthDayView;
class QHBoxLayout;
class CTodyButton;
class CMonthWindow: public QMainWindow
{
    Q_OBJECT
public:
    /**
     * @brief CMonthWindow 构造函数
     * @param parent 父类
     */
    explicit CMonthWindow(QWidget *parent = nullptr);
    /**
      * @brief ~CMonthWindow 析构函数
      */
    ~CMonthWindow() override;
    /**
     * @brief setFirstWeekday 设置每周第一天是周几
     * @param weekday 周几
     */
    void setFirstWeekday(int weekday);
    /**
     * @brief setDate 设置时间
     * @param date 日期
     */
    void setDate(QDate date);
    /**
     * @brief setLunarVisible 设置是否显示阴历信息
     * @param state 是否显示阴历信息
     */
    void setLunarVisible(bool state);
    /**
     * @brief setTheMe 根据系统主题类型设置颜色
     * @param type 系统主题类型
     */
    void setTheMe(int type = 0);
    /**
     * @brief setSearchWFlag 设置是否选择的标志
     * @param flag 是否选择的标志
     */
    void setSearchWFlag(bool flag);
    /**
     * @brief clearSearch
     */
    void clearSearch();
public slots:
    /**
     * @brief previousMonth 选择上一个月份
     */
    void previousMonth();
    /**
     * @brief nextMonth 选择下一个月份
     */
    void nextMonth();
    /**
     * @brief slotsearchDateSelect 选择搜索到的日期
     * @param date 日期
     */
    void slotsearchDateSelect(QDate date);
private:
    /**
     * @brief initUI 初始化界面
     */
    void initUI();
    /**
     * @brief initConnection 初始化信号和槽的连接
     */
    void initConnection();
    /**
     * @brief initLunar 初始化阴历信息
     */
    void initLunar();
    /**
     * @brief slideMonth 切换月份，并更新信息
     * @param next 是否切换到下一个月
     */
    void slideMonth(bool next);
signals:
    /**
     * @brief dateSelected
     * @param date
     * @param detail
     */
    void dateSelected(const QDate date, const CaLunarDayInfo &detail) const;
    /**
     * @brief signalsWUpdateShcedule 更新日程的信号
     * @param w unused
     * @param id unused
     */
    void signalsWUpdateShcedule(QMainWindow *w, int id = 0);
    /**
     * @brief signalsReturnTodayUpdate 返回今天的按钮的信号
     * @param w 视图
     */
    void signalsReturnTodayUpdate(QMainWindow *w);
    /**
     * @brief signalsCurrentScheduleDate
     * @param date
     */
    void signalsCurrentScheduleDate(QDate date);
    /**
     * @brief signalsViewSelectDate 切换视图的信号
     * @param date 日期
     */
    void signalsViewSelectDate(QDate date);
    /**
     * @brief signalViewtransparentFrame 获取视图的焦点
     * @param type
     */
    void signalViewtransparentFrame(int type);
    /**
     * @brief signalCurrentDate 设置时间的信号
     * @param date 日期
     */
    void signalCurrentDate(QDate date);
public slots:
    /**
     * @brief slotReturnTodayUpdate 返回今天
     */
    void slotReturnTodayUpdate();
    /**
     * @brief slotScheduleHide 隐藏日程浮框
     */
    void slotScheduleHide();
    /**
     * @brief slotAngleDelta    接受滚动事件滚动相对量
     * @param delta     滚动相对量
     */
    void slotAngleDelta(int delta);
public slots:
    /**
     * @brief slotupdateSchedule 更新日程
     * @param id
     */
    void slotupdateSchedule(int id = 0);
    /**
     * @brief slotTransitSchedule 发送更新日程的信号
     * @param id
     */
    void slotTransitSchedule(int id = 0);
    /**
     * @brief setSelectSchedule 设置选择的日程
     * @param scheduleInfo 选择日程的信息
     */
    void setSelectSchedule(const ScheduleDtailInfo &scheduleInfo);
protected:
    /**
     * @brief resizeEvent 窗口大小调整
     * @param event 窗口大小调整事件
     */
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *e) override;
private slots:
    /**
     * @brief slottoday 返回今天
     */
    void slottoday();
    /**
     * @brief slotcurrentDateLunarChanged 当前时间改变，更新信息
     * @param date 时间
     * @param detail 阴历信息
     * @param type 是否显示阴历信息
     */
    void slotcurrentDateLunarChanged(QDate date,  CaLunarDayInfo detail, int type = 0);
    /**
     * @brief slotcurrentDateChanged 根据时间变化，返回今天按钮状态变化
     * @param date 时间
     */
    void slotcurrentDateChanged(QDate date);
    /**
     * @brief slotSelectedMonth 设置选择的月份
     * @param date 日期
     */
    void slotSelectedMonth(QDate date);
private:
    //一个月的视图
    CMonthView *m_monthView = nullptr;
    //12个月份视图
    CMonthDayView *m_monthDayView = nullptr;
    //背景frame
    DFrame *m_contentBackground = nullptr;
    //返回今天的按钮
    CTodyButton *m_today = nullptr;
    //当天日期
    QDate                   m_currentdate;
    //年份label
    QLabel *m_YearLabel = nullptr;
    //阴历年份label
    QLabel *m_YearLunarLabel = nullptr;
    QSpacerItem *m_spaceitem = nullptr;
    /**
     * @brief m_gridWidget 月widget
     *
     * 将月view放入widget内进行显示
     */
    DWidget *m_gridWidget = nullptr;
    //搜索标志
    bool                    m_searchfalg = false;
    //月视图最终的窗口布局
    QHBoxLayout *m_tmainLayout = nullptr;
};

#endif // YEARWINDOW_H
