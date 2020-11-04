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
#include <QGestureEvent>

DWIDGET_USE_NAMESPACE

class CYearView;
class CaLunarDayInfo;
class CSchceduleSearchView;
class YearFrame;
class CustomFrame;

struct TouchGestureData {
    enum TouchMovingDirection {T_NONE, T_LEFT, T_TOP, T_RIGHT, T_BOTTOM}; //手势移动状态
    qreal lenght{0};        //手势移动距离
    qreal angle{0};         //手势移动角度
    TouchMovingDirection movingDirection{T_NONE};       // 手势移动方向
};

class CYearWindow: public QMainWindow
{
    Q_OBJECT
public:
    explicit CYearWindow(QWidget *parent = nullptr);
    ~CYearWindow() override;
    void setDate(QDate date);
    void initUI();
    void initConnection();
    void setLunarVisible(bool state);
    void setTheMe(int type = 0);
    void setSearchWFlag(bool flag);
    void clearSearch();
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
    void slotprev();
    void slotnext();
    void slottoday();
    void slotsearchDateSelect(QDate date);
    void slotTransitSearchSchedule(int id = 0);
public slots:
    void slotSetSchceduleHide();
    void slotReturnTodayUpdate();
    void slotupdateSchedule(const int id);
    void setYearData();
    void slotUpdateCurrentDate(const QDate &date);
protected:
    void wheelEvent(QWheelEvent *event) override;
    /**
     * @brief eventFilter 过滤器，过滤返回今天的按钮事件
     * @param watched 事件对象
     * @param event 事件类型
     * @return false
     */
    bool eventFilter(QObject *watched, QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    bool event(QEvent *e) override;
private:
    /**
     * @brief gestureEvent      触摸手势处理
     * @param event     手势事件
     * @return
     */
    bool gestureEvent(QGestureEvent *event);
    /**
     * @brief tapGestureTriggered       轻切手势处理
     * @param tap       轻切手势事件
     */
    void tapGestureTriggered(QTapGesture *tap);
    /**
     * @brief panTriggered      多指滑动手势处理
     * @param pan       多指滑动手势
     */
    void panTriggered(QPanGesture *pan);
    /**
     * @brief calculateAzimuthAngle     计算方位角
     * @param startPoint            起始坐标
     * @param stopPoint            结束坐标
     * @return      触摸手势数据
     */
    TouchGestureData   calculateAzimuthAngle(QPointF &startPoint, QPointF &stopPoint);
private:
    //年视图页面框架
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
    //动画widget
    AnimationStackedWidget *m_StackedWidget = nullptr;
    //年视图布局
    QHBoxLayout *m_tmainLayout = nullptr;
    QString                     m_searchText;
    bool                        m_searchfalg = false;
    DWidget *m_topWidget = nullptr;
    //触摸开始坐标
    QPointF     m_TouchBeginPoint;
    //触摸状态 0：初始状态 1:点击 2：移动
    int         m_TouchState{0};
};

class YearFrame : public DFrame
{
    Q_OBJECT
public:
    explicit YearFrame(DWidget *parent = nullptr);
    ~YearFrame() override;
    void setDate(QDate &date);
    void getInfoAndSetLineFlag();
    void setTheMe(int type = 0);
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
    void getLunarData();
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
    void slotHideInfo();
    void slotSelectInfo(bool flag);
    void slotupdateSchedule(const int id);
    void slotSetSchceduleHide();
    void slotcurrentDateChanged(QDate date);
private:
    //12个月份
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
