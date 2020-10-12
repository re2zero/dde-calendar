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
#ifndef WEEKWINDOW_H
#define WEEKWINDOW_H

#include "customframe.h"
#include "schedulestructs.h"
#include "calendardbus.h"

#include <DFrame>
#include <DIconButton>
#include <DImageButton>
#include <DPushButton>
#include <DMainWindow>

#include <QWidget>
#include <QDate>
#include <QLabel>
#include <QLabel>
#include <QHBoxLayout>

DWIDGET_USE_NAMESPACE

class CWeekHeadView;
class CaLunarDayInfo;
class CWeekView;
class CScheduleView;
class CSchceduleSearchView;
class CTodyButton;
/**
 * @brief The CWeekWindow class
 */
class CWeekWindow: public QMainWindow
{
    Q_OBJECT
public:
    /**
     * @brief CWeekWindow 构造函数
     * @param parent 父类
     */
    CWeekWindow(QWidget *parent = nullptr);
    /**
      *@brief ~CWeekWindow 析构函数
      */
    ~CWeekWindow() override;
    /**
     * @brief setDate 设置今天的日期
     * @param date 日期
     */
    void setDate(QDate date);
    /**
     * @brief setSelectSchedule 设置选择的日程
     * @param scheduleInfo 日程信息
     */
    void setSelectSchedule(const ScheduleDtailInfo &scheduleInfo);
    /**
     * @brief setFirstWeekday 设置每周以周几开始
     * @param weekday 周几
     */
    void setFirstWeekday(int weekday);
    /**
     * @brief setLunarVisible 设置是否显示阴历信息
     * @param state 是否显示阴历信息
     */
    void setLunarVisible(bool state);
    /**
     * @brief initUI 初始化界面设置
     */
    void initUI();
    /**
     * @brief initConnection 初始化信号和槽的连接
     */
    void initConnection();
    /**
     * @brief setTheMe 根据系统主题类型设置颜色
     * @param type 系统主题类型
     */
    void setTheMe(int type = 0);
    /**
     * @brief setTime 设置CScheduleView的时间
     * @param time 时间
     */
    void setTime(QTime time);
    /**
     * @brief setSearchWFlag 设置搜索标志
     * @param flag 是否进行了搜索
     */
    void setSearchWFlag(bool flag);
    /**
     * @brief clearSearch
     */
    void clearSearch();
    /**
     * @brief updateHigh CScheduleView updateHigh
     */
    void updateHigh();
signals:
    /**
     * @brief dateSelected
     * @param date
     * @param detail
     */
    void dateSelected(const QDate date, const CaLunarDayInfo &detail) const;
    /**
     * @brief signalsWUpdateShcedule 更新日程信号
     * @param w
     * @param id
     */
    void signalsWUpdateShcedule(QMainWindow *w, int id = 0);
    /**
     * @brief signalsReturnTodayUpdate 返回今天的信号
     * @param w 视图窗口
     */
    void signalsReturnTodayUpdate(QMainWindow *w);
    /**
     * @brief signalsCurrentScheduleDate 切换窗口的信号
     * @param date 时间
     */
    void signalsCurrentScheduleDate(QDate date);
    /**
     * @brief signalViewtransparentFrame 窗口转换信号
     * @param type
     */
    void signalViewtransparentFrame(int type);
    /**
     * @brief signalCurrentDate 设置时间信号
     * @param date 时间
     */
    void signalCurrentDate(QDate date);
    /**
     * @brief signalsViewSelectDate 选择时间的信号
     * @param date 时间
     */
    void signalsViewSelectDate(QDate date);
public slots:
    /**
     * @brief slotReturnTodayUpdate 返回今天按钮，设置当前时间
     */
    void slotReturnTodayUpdate();
    /**
     * @brief slotIsDragging CScheduleView IsDragging,判断是否可以拖拽
     * @param isDragging 是否可以拖拽
     */
    void slotIsDragging(bool &isDragging);
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
     * @brief slotTransitSearchSchedule 更新日程并发送信号
     * @param id
     */
    void slotTransitSearchSchedule(int id = 0);
    /**
     * @brief slotScheduleHide 隐藏日程浮框
     */
    void slotScheduleHide();
private slots:
    /**
     * @brief slotprev 切换到上一周，隐藏日程浮框
     */
    void slotprev();
    /**
     * @brief slotnext 切换到下一周，隐藏日程浮框
     */
    void slotnext();
    /**
     * @brief slottoday 返回到今天，隐藏日程浮框
     */
    void slottoday();
    /**
     * @brief slotCurrentWeek 设置每周开始的日期和今天的日期
     * @param date 每周开始的日期
     * @param currentDate 今天的日期
     */
    void slotCurrentWeek(QDate date, QDate currentDate);
    /**
     * @brief slotcurrentDateLunarChanged 切换显示当天的阴历信息
     * @param vdate 一周的时间
     * @param vdetail 阴历信息
     * @param type 是否显示阴历信息
     */
    void slotcurrentDateLunarChanged(QVector<QDate> vdate, QVector<CaLunarDayInfo> vdetail, int type = 0);
    /**
     * @brief slotcurrentDateChanged 切换显示当天的时间，并改变返回今天按钮显示的文字
     * @param date 时间
     */
    void slotcurrentDateChanged(QDate date);
    /**
     * @brief slotsearchDateSelect 选择选中的日期
     * @param date 日期
     */
    void slotsearchDateSelect(QDate date);
protected:
    /**
     * @brief resizeEvent 调整周视图窗口
     * @param event 窗口大小调整事件
     */
    void resizeEvent(QResizeEvent *event) override;
    /**
     * @brief mousePressEvent 鼠标单击隐藏日程浮框
     * @param event 鼠标事件
     */
    void mousePressEvent(QMouseEvent *event) override;
private:
    //周视图头weekheadview
    CWeekHeadView *m_weekHeadView = nullptr;
    //背景
    DFrame *m_contentBackground = nullptr;
    //上一周按钮
    DIconButton *m_prevButton = nullptr;
    //下一周按钮
    DIconButton *m_nextButton = nullptr;
    //返回今天按钮
    CTodyButton       *m_today = nullptr;
    //当前日期
    QDate              m_currentdate;
    //年份label
    QLabel *m_YearLabel = nullptr;
    //阴历年份label
    QLabel *m_YearLunarLabel = nullptr;
    //周lable
    QLabel *m_weekLabel = nullptr;
    //weekview
    CWeekView         *m_weekview = nullptr;
    //title中间的背景
    CustomFrame *m_todayframe = nullptr;
    //日程view
    CScheduleView *m_scheduleView = nullptr;
    QSpacerItem *m_spaceitem = nullptr;
    //周视图布局
    QVBoxLayout *m_mainhLayout = nullptr;
    //搜索文字
    QString m_searchText;
    //总体布局
    QHBoxLayout *m_tmainLayout = nullptr;
    //搜索标志
    bool m_searchfalg = false;
};

#endif // YEARWINDOW_H
