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
#include "SchecduleRemindWidget.h"
#include "monthgraphiview.h"
#include "src/scheduledatainfo.h"

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
    //根据系统主题类型设置颜色
    void setTheMe(int type = 0);
    /**
     * @brief isDragging 是否可以拖拽
     * @return 是否可以拖拽
     */
    bool isDragging()const
    {
        return m_monthGraphicsView->getDragStatus() != 4;
    }
public:
    explicit CMonthView(QWidget *parent = nullptr);
    ~CMonthView() override;
    //设置每周首日
    void setFirstWeekday(Qt::DayOfWeek weekday);
    //设置显示日期
    void setShowDate(const QVector<QDate> &showDate);
    //设置黄历信息
    void setHuangliInfo(const QMap<QDate, CaHuangLiDayInfo> &huangliInfo);
    //设置班休信息
    void setFestival(const QMap<QDate, int> &festivalInfo);
    //设置显示日程
    void setScheduleInfo(const QMap<QDate, QVector<ScheduleDataInfo> > &scheduleInfo);
    //设置搜索日程
    void setSearchScheduleInfo(const QVector<ScheduleDataInfo> &searchScheduleInfo);
signals:
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
     * @brief setLunarVisible 设置是否显示阴历信息
     * @param visible 是否显示阴历信息
     */
    void setLunarVisible(bool visible);
    /**
     * @brief setSelectSchedule 设置选择的日程
     * @param scheduleInfo 日程信息
     */
    void setSelectSchedule(const ScheduleDataInfo &scheduleInfo);
public slots:
    /**
     * @brief slotScheduleRemindWidget 日程浮框
     * @param isShow 是否显示日程浮框
     * @param out 日程信息
     */
    void slotScheduleRemindWidget(const bool isShow, const ScheduleDataInfo &out = ScheduleDataInfo());
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
    ScheduleDataInfo getScheduleInfo(const QDate &beginDate, const QDate &endDate);
private:
    CMonthGraphiview                *m_monthGraphicsView = nullptr;
    QVector<QDate>                  m_showDate;
    QDate                           m_currentDate;
    QDate                           m_createDate;
    CMonthWeekView                  *m_weekIndicator = nullptr;
    int                             m_firstWeekDay = 0;
    QAction                         *m_createAction = nullptr; // 创建日程
    QVBoxLayout                     *m_mainLayout = nullptr;
    int                             m_leftmaagin = 0;
    int                             m_topmagin = 0;
    bool                            m_sflag = true;
    SchecduleRemindWidget           *m_remindWidget = nullptr;

    QPoint                  m_PressPoint;
    bool                    isCreate;
    QDate                   m_PressDate;
    QDate                   m_MoveDate;
};

#endif // MYCALENDARWIDGET_H
