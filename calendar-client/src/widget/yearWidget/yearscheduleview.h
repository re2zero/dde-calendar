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
#ifndef YEARSCHEDULEVIEW_H
#define YEARSCHEDULEVIEW_H

#include "schedulestructs.h"

#include <DLabel>
#include <DPushButton>
#include <DListWidget>
#include <DWidget>
#include <DArrowRectangle>

DWIDGET_USE_NAMESPACE
class QVBoxLayout;
class CYearScheduleItem;
class CYearScheduleOutView;
class CYearScheduleView : public DWidget
{
    Q_OBJECT

public:
    /**
     * @brief CYearScheduleView 构造函数
     * @param parent 父类
     */
    explicit CYearScheduleView(QWidget *parent = nullptr);
    /**
      * @brief ~CYearScheduleView 析构函数
      */
    ~CYearScheduleView();
    /**
     * @brief setSoloDay
     * @param soloday
     */
    void setSoloDay(QString soloday);
    /**
     * @brief setData 设置日程信息，并全天日程置于非全天日程之前
     * @param vListData 日程信息
     */
    void setData(QVector<ScheduleDtailInfo> &vListData);
    /**
     * @brief getlistdate 获取日程信息
     * @return  日程信息
     */
    QVector<ScheduleDtailInfo> getlistdate()
    {
        return m_vlistData;
    }
    /**
     * @brief clearData 清除日程信息
     */
    void clearData();
    /**
     * @brief showWindow 设置日程浮框的大小
     * @return 日程浮框的宽度
     */
    int showWindow();
    /**
     * @brief setTheMe 根据系统主题类型设置颜色
     * @param type 系统主题类型
     */
    void setTheMe(int type = 0);
    /**
     * @brief setDtype
     * @param type
     * @param arrowheight
     */
    void setDtype(int type, int arrowheight);
    /**
     * @brief setCurrentDate 设置日程所在当天的日期
     * @param cdate 日期
     */
    void setCurrentDate(QDate cdate);
    /**
     * @brief getCurrentDate 获取日程所在当天的日期
     * @return 日期
     */
    QDate getCurrentDate();
    /**
     * @brief adjustPosition 根据日程浮框左右朝向不同，日程显示位置不同
     * @param ad 是否调整显示位置
     */
    void adjustPosition(bool ad);

private:
    /**
     * @brief updateDateShow 调整最多展示日程为五个，并设置浮框大小
     */
    void updateDateShow();
    /**
     * @brief createItemWidget 设置每条日程的具体信息
     * @param info 日程信息
     * @param type 系统主题类型
     */
    void createItemWidget(ScheduleDtailInfo info, int type = 0);

protected:
    /**
     * @brief paintEvent 绘制日程
     * @param event 绘图事件
     */
    void paintEvent(QPaintEvent *event) override;
    /**
     * @brief paintItem 绘制日程
     * @param info 日程信息
     * @param index 日程的索引
     * @param type 系统主题类型
     */
    void paintItem(ScheduleDtailInfo info, int index, int type = 0);
    /**
     * @brief paintItem
     */
    void paintItem();

private:
    DListWidget *m_gradientItemList = nullptr; //下拉列表窗
    bool m_widgetFlag;
    QVector<ScheduleDtailInfo> m_vlistData;
    QString m_soloDay;
    QVector<DLabel *> m_labellist;
    int m_type;
    QDate m_currentDate;
    QColor m_bBackgroundcolor = "#000000";
    QColor m_btimecolor = "#526A7F";
    QColor m_bttextcolor = "#414D68";
    QColor m_lBackgroundcolor = Qt::white;
    QColor m_ltextcolor = "#001A2E";
    QColor m_solocolor = "#001A2E";
    QColor m_TBcolor = "#001A2E";
    int m_dtype = 3;
    int m_arrowheight = 0;
    QColor m_borderColor = "#000000";
    bool adjustPos = false;
};

class CYearScheduleItem : public DLabel
{
    Q_OBJECT

public:
    /**
     * @brief CYearScheduleItem 构造函数
     * @param parent 父类
     */
    explicit CYearScheduleItem(QWidget *parent = nullptr);
    /**
     * @brief setBackgroundColor 设置背景颜色
     * @param color1 背景颜色
     */
    void setBackgroundColor(QColor color1);
    /**
     * @brief setStateColor 设置圆点的颜色
     * @param color1 圆点颜色
     */
    void setStateColor(QColor color1);
    /**
     * @brief setText 设置文字颜色和字体
     * @param tcolor 文字颜色
     * @param font 文字字体
     */
    void setText(QColor tcolor, QFont font);
    /**
     * @brief setTimeC 设置时间的颜色和字体
     * @param tcolor 时间的颜色
     * @param font 时间的字体
     */
    void setTimeC(QColor tcolor, QFont font);
    /**
     * @brief setData 设置日程信息
     * @param vScheduleInfo 日程信息
     */
    void setData(ScheduleDtailInfo vScheduleInfo);

protected:
    /**
     * @brief paintEvent
     * @param e
     */
    void paintEvent(QPaintEvent *e) override;

private:
    ScheduleDtailInfo m_ScheduleInfo;
    QColor m_Backgroundcolor;
    QColor m_timecolor;
    QColor m_Statecolor;
    QFont m_timefont;
    QColor m_ttextcolor;
    QFont m_tfont;
};

class CYearScheduleOutView : public DArrowRectangle
{
    Q_OBJECT

public:
    /**
     * @brief CYearScheduleOutView 构造函数
     * @param parent 父类
     */
    explicit CYearScheduleOutView(QWidget *parent = nullptr);
    /**
     * @brief setSoloDay
     * @param soloday
     */
    void setSoloDay(QString soloday);
    /**
     * @brief setData 设置日程信息
     * @param vListData 日程信息
     */
    void setData(QVector<ScheduleDtailInfo> &vListData);
    /**
     * @brief clearData 清楚数据
     */
    void clearData();
    /**
     * @brief showWindow 设置日程浮框外边框大小
     */
    void showWindow();
    /**
     * @brief setTheMe 设置系统主题颜色
     * @param type 系统主题
     */
    void setTheMe(int type = 0);
    /**
     * @brief setDtype
     * @param type
     * @param arrowheight
     */
    void setDtype(int type, int arrowheight);
    /**
     * @brief setCurrentDate 设置日程所在当天的日期
     * @param cdate 日期
     */
    void setCurrentDate(QDate cdate);
    /**
     * @brief adjustPosition 根据日程浮框左右朝向不同，日程显示位置不同
     * @param ad 是否调整显示位置
     */
    void adjustPosition(bool ad);
signals:
    /**
     * @brief signalsViewSelectDate 跳转视图信号
     * @param date 当天的时间
     */
    void signalsViewSelectDate(QDate date);
    /**
     * @brief signalupdateschedule 更新日程信息的信号
     */
    void signalupdateschedule();

private:
    CYearScheduleView *yearscheduleview = nullptr;
    QVector<ScheduleDtailInfo> scheduleinfoList;
    QDate currentdate;
    int list_count = 0;

protected:
    /**
     * @brief mousePressEvent 鼠标单击事件，单击非节日日程进行编辑，单击”...”区域跳转到周视图。
     * @param event 鼠标事件
     */
    void mousePressEvent(QMouseEvent *event) override;
};
#endif // YEARSCHEDULEVIEW_H
