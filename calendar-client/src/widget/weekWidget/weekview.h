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
#ifndef WEEKVIEW_H
#define WEEKVIEW_H

#include <QObject>
#include <QWidget>
#include <QDate>
/**
 * @brief The CWeekView class
 */
class CWeekView : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief CWeekView 构造函数
     * @param parent 父类
     */
    explicit CWeekView(QWidget *parent = nullptr);
    /**
      * @brief ~CWeekView 析构函数
      */
    ~CWeekView();
    /**
     * @brief setFirstWeekDay 设置每周的第一天是周几，并更新
     * @param weekday 周几
     */
    void setFirstWeekDay(int weekday);
    /**
     * @brief setCurrentDate 设置当天时间，并更新
     * @param date 时间
     */
    void setCurrentDate(const QDate date);
    /**
     * @brief setTheMe 根据系统主题类型设置颜色
     * @param type 系统主题类型
     */
    void setTheMe(int type = 0);
    /**
     * @brief setsearchfalg
     * @param flag
     */
    void setsearchfalg(bool flag);
public slots:
    /**
     * @brief cellClicked 选择点击的日期
     * @param cell 周数所在的widget
     */
    void cellClicked(QWidget *cell);
    /**
     * @brief slotprev 选择上一周
     */
    void slotprev();
    /**
     * @brief slotnext 选择下一周
     */
    void slotnext();
signals:
    /**
     * @brief signalsSelectDate 选择日期的信号
     * @param date 选择的日期
     * @param currentDate 今天的时间
     */
    void signalsSelectDate(QDate date, QDate currentDate);
    /**
     * @brief signalIsDragging 判断是否是拖拽状态
     * @param isDragging 是否是拖拽状态
     */
    void signalIsDragging(bool &isDragging);
protected:
    /**
     * @brief resizeEvent 调整窗口大小
     * @param event 窗口大小调整事件
     */
    void resizeEvent(QResizeEvent *event) override;
    /**
     * @brief wheelEvent 鼠标滚轮切换上一周下一周
     * @param event 鼠标滚轮事件
     */
    void wheelEvent(QWheelEvent *event) override;
private:
    /**
     * @brief paintCell 绘制周数
     * @param cell 周数所在的widget
     */
    void paintCell(QWidget *cell);
    /**
     * @brief eventFilter 过滤器
     * @param o 事件对象
     * @param e 事件类型
     * @return  false
     */
    bool eventFilter(QObject *o, QEvent *e) override;
    /**
     * @brief setSelectedCell 设置被选择的周数
     * @param index 周数所在的索引
     */
    void setSelectedCell(int index);
    /**
     * @brief updateDate 更新数据
     */
    void updateDate();
private:
    /**
     * @brief m_cellList list
     *
     * 周数list
     */
    QList<QWidget *> m_cellList;
    //选择的日期
    QDate m_selectDate;
    //周数对应的日期
    QDate m_days[10];
    //是否被选择
    bool m_cellSelectable = true;
    //选择的索引
    int m_selectedCell = 0;
    //字体
    QFont m_dayNumFont;
    //默认字体颜色
    QColor m_defaultTextColor = Qt::black;
    //默认背景色
    QColor m_backgrounddefaultColor = Qt::white;
    //当天的字体颜色
    QColor m_currentDayTextColor = Qt::white;
    //今天的背景色
    QColor m_backgroundcurrentDayColor = "#0081FF";
    //背景色
    QColor m_fillColor = Qt::white;
    //每周起始的周数
    int m_firstWeekDay;
    int m_weekAddDay = 0;
    //系统主题类型
    int m_themetype = 1;
    //选择标志
    bool m_searchfalg = false;
};

#endif // MONTDAYVIEW_H
