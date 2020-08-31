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

#include "animationstackedwidget.h"
#include "calendardbus.h"

#include <DImageButton>
#include <DPushButton>
#include <DFrame>
#include <DIconButton>
#include <DLabel>
#include <DMainWindow>

#include <QWidget>
#include <QDate>
#include <QLabel>
#include <QHBoxLayout>

DWIDGET_USE_NAMESPACE

class CYearView;
class CaLunarDayInfo;
class CSchceduleSearchView;
class YearFrame;
class CustomFrame;
class CYearWindow: public QMainWindow
{
    Q_OBJECT
public:
    CYearWindow(QWidget *parent = nullptr);
    ~CYearWindow() override;
    /**
     * @brief setDate 设置年视图当前显示的时间
     * @param date 年视图当前显示的时间
     */
    void setDate(QDate date);
    /**
     * @brief initUI 初始化年视图的界面显示
     */
    void initUI();
    /**
     * @brief initConnection 初始化信号和槽的连接
     */
    void initConnection();
    /**
     * @brief setLunarVisible 设置年视图是否显示阴历信息
     * @param state 是否显示阴历信息
     */
    void setLunarVisible(bool state);
    /**
     * @brief setTheMe 设置系统主题
     * @param type 主题类型
     */
    void setTheMe(int type = 0);
    /**
     * @brief setSearchWFlag 设置是否在进行搜索
     * @param flag 是否所搜的标志
     */
    void setSearchWFlag(bool flag);
    /**
     * @brief clearSearch
     */
    void clearSearch();
    /**
     * @brief setSearchText
     * @param str
     */
    void setSearchText(QString str);
    /**
     * @brief getScheduleInfo 判断一年中每一天是否有日程信息
     */
    void getScheduleInfo();
signals:
    /**
     * @brief dateSelected
     * @param date
     * @param detail
     */
    void dateSelected(const QDate date, const CaLunarDayInfo &detail) const;
    /**
     * @brief signalsReturnTodayUpdate 点击返回今天按钮发出信号
     * @param w 年视图窗口
     */
    void signalsReturnTodayUpdate(QMainWindow *w);
    /**
     * @brief signalsWUpdateShcedule
     * @param w
     * @param id
     */
    void signalsWUpdateShcedule(QMainWindow *w, int id = 0);
    /**
     * @brief signaldoubleclickDate 双击鼠标发出信号
     * @param date 双击的日期
     */
    void signaldoubleclickDate(QDate date);
    /**
     * @brief signalselectMonth 鼠标选择的日期信号
     * @param date 选择的日期
     */
    void signalselectMonth(QDate date);
    /**
     * @brief signalselectWeekwindow 选择周视图的信号
     * @param date 选择的日期
     */
    void signalselectWeekwindow(QDate date);
    /**
     * @brief signalCurrentDate 设置当前时间的信号
     * @param date 当前时间
     */
    void signalCurrentDate(QDate date);
    /**
     * @brief signalupdateschcedule 更新日程的信号
     */
    void signalupdateschcedule();
private slots:
    /**
     * @brief slotprev 通过鼠标点击左箭头切换到上一年，刷新年视图下的所有内容，并隐藏日程展示浮窗。
     */
    void slotprev();
    /**
     * @brief slotnext 通过鼠标点击右箭头切换到下一年，并刷新年视图下的所有内容，并隐藏日程展示浮窗。
     */
    void slotnext();
    /**
     * @brief slottoday 通过鼠标点击返回今天按钮返回到今天，并刷新年视图下的所有内容，并隐藏日程展示浮窗。
     */
    void slottoday();
    /**
     * @brief slotsearchDateSelect 设置选择的时间
     * @param date 时间
     */
    void slotsearchDateSelect(QDate date);
    /**
     * @brief slotTransitSearchSchedule
     * @param id
     */
    void slotTransitSearchSchedule(int id = 0);
public slots:
    /**
     * @brief slotSetSchceduleHide 隐藏日程浮框
     */
    void slotSetSchceduleHide();
    /**
     * @brief slotReturnTodayUpdate
     */
    void slotReturnTodayUpdate();
    /**
     * @brief slotupdateSchedule 更新当前时间
     * @param id
     */
    void slotupdateSchedule(const int id);
    /**
     * @brief setYearData 设置年视图阴历和按钮信息
     */
    void setYearData();
    /**
     * @brief slotUpdateCurrentDate 设置年视图当前时间的信息
     * @param date 时间
     */
    void slotUpdateCurrentDate(const QDate &date);
protected:
    /**
     * @brief wheelEvent 通过鼠标中间的滚轮滚动切换年份，并刷新年视图下的所有内容。
     * @param event 鼠标滚轮事件
     */
    void wheelEvent(QWheelEvent *event) override;
    /**
     * @brief eventFilter 过滤器，过滤返回今天的按钮事件
     * @param watched 事件对象
     * @param event 事件类型
     * @return false
     */
    bool eventFilter(QObject *watched, QEvent *event) override;
    /**
     * @brief mousePressEvent 鼠标单击事件，单击日期区域外，隐藏日程浮框
     * @param event 鼠标事件
     */
    void mousePressEvent(QMouseEvent *event) override;
    /**
     * @brief resizeEvent 窗口大小调整事件，搜索时，调整边框大小
     * @param event 窗口大小调整事件
     */
    void resizeEvent(QResizeEvent *event) override;
private:
    DFrame *m_contentBackground = nullptr;
    DIconButton *m_prevButton = nullptr;
    DIconButton *m_nextButton = nullptr;
    QLabel *m_today = nullptr;
    QDate                       m_currentdate;
    QLabel *m_YearLabel = nullptr;
    QLabel *m_YearLunarLabel = nullptr;
    QLabel *m_YearLunarDayLabel = nullptr;
    YearFrame *m_YearWidget = nullptr;
    YearFrame *YearWidget_First = nullptr;
    YearFrame *YearWidget_Second = nullptr;
    CustomFrame *m_todayframe = nullptr;
    AnimationStackedWidget *m_StackedWidget = nullptr;
    QHBoxLayout *m_tmainLayout = nullptr;
    QString                     m_searchText;
    bool                        m_searchfalg = false;
    DWidget *m_topWidget = nullptr;
};

class YearFrame : public DFrame
{
    Q_OBJECT
public:
    /**
     * @brief YearFrame 构造函数
     * @param parent 父类
     */
    explicit YearFrame(DWidget *parent = nullptr);
    /**
      *@brief ~YearFrame 析构函数
      */
    ~YearFrame() override;
    /**
     * @brief setDate 设置当前时间
     * @param date 时间
     */
    void setDate(QDate &date);
    /**
     * @brief getInfoAndSetLineFlag 获取日程信息，设置是否有日程的标志
     */
    void getInfoAndSetLineFlag();
    /**
     * @brief setTheMe 根据系统主题类型设置颜色
     * @param type 系统主题类型
     */
    void setTheMe(int type = 0);
    /**
     * @brief setSearchWFlag 设置搜索的标志
     * @param flag 是否进行了搜索
     */
    void setSearchWFlag(bool flag);
    /**
     * @brief getLunarYear 获取阴历年信息
     * @return 阴历年信息字符串
     */
    QString getLunarYear()const
    {
        return m_LunarYear;
    }
    /**
     * @brief getLunarDay 获取阴历日信息
     * @return 阴历日信息字符串
     */
    QString getLunarDay()const
    {
        return m_LunarDay;
    }
    /**
     * @brief getLunarData 更新当天的农历年份数据。
     */
    void getLunarData();
protected:
    /**
     * @brief mousePressEvent 鼠标单击日期区域外隐藏日程浮框
     * @param event 鼠标单击事件
     */
    void mousePressEvent(QMouseEvent *event) override;
signals:
    /**
     * @brief signaldoubleclickDate 鼠标双击的信号
     * @param date 传递日期参数
     */
    void signaldoubleclickDate(QDate date);
    /**
     * @brief signalselectWeekwindow 选择周视图的信号
     * @param date 传递日期参数
     */
    void signalselectWeekwindow(QDate date);
    /**
     * @brief signalselectMonth 选择月视图的信号
     * @param date 传递日期参数
     */
    void signalselectMonth(QDate date);
    /**
     * @brief signalUpdateYearDate 更新年视图的信号
     * @param date 传递日期参数
     */
    void signalUpdateYearDate(const QDate &date);
    /**
     * @brief signalupdateschcedule 更新日程的信号
     */
    void signalupdateschcedule();
public slots:
    /**
     * @brief slotHideInfo 隐藏日程浮框
     */
    void slotHideInfo();
    /**
     * @brief slotSelectInfo 设置是否选择了日期的标志
     * @param flag 是否选择了如期
     */
    void slotSelectInfo(bool flag);
    /**
     * @brief slotupdateSchedule 更新日程
     * @param id
     */
    void slotupdateSchedule(const int id);
    /**
     * @brief slotSetSchceduleHide 隐藏日程浮框
     */
    void slotSetSchceduleHide();
    /**
     * @brief slotcurrentDateChanged 设置当前时间
     * @param date 时间
     */
    void slotcurrentDateChanged(QDate date);
private:
    QList<CYearView *>          m_monthViewList;
    QLabel *m_YearLabel = nullptr;
    QLabel *m_YearLunarLabel = nullptr;
    CalendarDBus *m_DBusInter = nullptr;
    QDate                       m_currentdate;
    bool                        m_searchfalg = false;
    bool                        m_selectFlag = false;
    QString                     m_LunarYear;
    QString                     m_LunarDay;
    DWidget *m_topWidget = nullptr;
};

#endif // YEARWINDOW_H
