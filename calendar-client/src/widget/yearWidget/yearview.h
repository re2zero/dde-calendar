#ifndef YEARVIEW_H
#define YEARVIEW_H

#include "schedulestructs.h"
#include "customframe.h"

#include <DWidget>
#include <DFrame>

#include <QList>
#include <QDate>
#include <QStyleOption>
#include <QLabel>
#include <QSignalMapper>
#include <QVBoxLayout>

DWIDGET_USE_NAMESPACE
class CalendarDBus;
class CaLunarDayInfo;
class QLabel;
class CYearScheduleView;
class CYearScheduleOutView;
class WeekIndicator;
/**
 * @brief The CalendarYearDayType enum
 */
enum CalendarYearDayType {
    SO_YFestival = QStyleOption::SO_CustomBase + 0x01,
    SO_YWeekends = QStyleOption::SO_CustomBase + 0x02,
    SO_YWeekendsAndFestival = SO_YFestival | SO_YWeekends,
    SO_YNotCurrentMonth = 0x04,
    SO_YNotCurrentMonthFestival = SO_YNotCurrentMonth | SO_YFestival,
    SO_YDefault,
};
//#define M_FLAG 0;

class CYearMonthView;
class CYearView: public CustomFrame
{
    Q_OBJECT

    Q_PROPERTY(QColor backgroundCircleColor MEMBER m_backgroundCircleColor DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY(QColor weekendsTextColor MEMBER m_weekendsTextColor DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY(QColor topBorderColor MEMBER m_topBorderColor DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY(bool cellSelectable READ cellSelectable WRITE setCellSelectable NOTIFY cellSelectableChanged)
public:
    /**
     * @brief CYearView 构造函数
     * @param parent 父类
     */
    explicit CYearView(QWidget *parent = nullptr);
    /**
     * @brief setFirstWeekday 设置日历每周是以周几开始的
     * @param weekday 星期几
     */
    void setFirstWeekday(int weekday);
    /**
     * @brief cellSelectable
     * @return
     */
    inline bool cellSelectable() const
    {
        return m_cellSelectable;
    }
    /**
     * @brief setTheMe 根据系统主题类型，设置颜色
     * @param type 系统主题类型
     */
    void setTheMe(int type = 0);
    /**
     * @brief ScheduleViewHide 隐藏日程浮框
     */
    static void ScheduleViewHide();
    /**
     * @brief getInfoAndSetLineFlag 获取日程信息，设置是否有日程的标志
     */
    void getInfoAndSetLineFlag();
signals:
    /**
     * @brief signalcurrentDateChanged 当前现实日期改变的信号
     * @param date 传递日期参数
     */
    void signalcurrentDateChanged(QDate date);
    /**
     * @brief cellSelectableChanged
     * @param cellSelectable
     */
    void cellSelectableChanged(bool cellSelectable) const;
    /**
     * @brief signaldoubleclickDate 鼠标双击日期的信号
     * @param date 传递日期参数
     */
    void signaldoubleclickDate(QDate date);
    /**
     * @brief signalselectMonth 鼠标双击月份的信号
     * @param date 传递日期参数
     */
    void signalselectMonth(QDate date);
    /**
     * @brief signalselectWeekwindow 选择周视图的信号
     * @param date 传递日期参数
     */
    void signalselectWeekwindow(QDate date);
    /**
     * @brief signalupdateschedule 更新日程的信号
     */
    void signalupdateschedule();
    /**
     * @brief signalHideInfo 隐藏日程浮框的信号
     */
    void signalHideInfo();
    /**
     * @brief signalSelectInfo 设置选择是否选择日期的标志
     * @param flag 是否选择日期
     */
    void signalSelectInfo(bool flag);
public slots:
    /**
     * @brief setCurrentDate 设置当前的日期
     * @param date 日期
     * @param type
     */
    void setCurrentDate(const QDate date, int type = 0);
    /**
     * @brief setCellSelectable
     * @param selectable
     */
    void setCellSelectable(bool selectable);
    /**
     * @brief slotupdateSchedule 设置当前时间
     * @param id
     */
    void slotupdateSchedule(const int id);
    /**
     * @brief slotDoubleClickDate 隐藏日程浮框，并发出鼠标双击的信号
     * @param date 传递时间参数
     */
    void slotDoubleClickDate(const QDate &date);
    /**
     * @brief slotPressClickDate 鼠标单击日期，显示日程浮框
     * @param date 传递日程参数
     */
    void slotPressClickDate(const QDate &date);
private:
    /**
     * @brief getDateIndex 根据日期找到在一个月中的位置索引
     * @param date 比较的日期
     * @return 索引
     */
    int getDateIndex(const QDate &date) const;
    /**
     * @brief eventFilter 过滤器，双击年视图下的月份跳转到月视图。
     * @param o 事件对象
     * @param e 事件类型
     * @return false
     */
    bool eventFilter(QObject *o, QEvent *e) override;
    /**
     * @brief updateDate 更新日期
     */
    void updateDate();
    /**
     * @brief createYearScheduleView 创建日程浮框对象，并连接信号和槽
     * @param parent 父类
     */
    void createYearScheduleView(QWidget *parent = nullptr);

protected:
    /**
     * @brief resizeEvent 更新月份框的高度
     * @param event 窗口大小调整事件
     */
    void resizeEvent(QResizeEvent *event) override;
protected:
    /**
     * @brief paintEvent 绘制每个月的背景
     * @param e 绘图事件
     */
    void paintEvent(QPaintEvent *e) override;
private:
    CustomFrame *m_currentMouth = nullptr;
    QDate m_days[42];
    QDate m_currentDate;
    QStringList m_monthList;
    int m_selectedCell = -1;
    bool m_cellSelectable = true;

    QFont  m_momthFont;
    QColor m_topBorderColor = Qt::red;
    QColor m_backgroundCircleColor = "#2ca7f8";
    QColor m_currentDayTextColor = "#2ca7f8";
    QColor m_weekendsTextColor = Qt::black;
    QColor m_festivalTextColor = Qt::black;
    QColor m_cellBackgroundColor = "#FFFFFF" ;

    int m_firstWeekDay = 0;
    QVBoxLayout *m_hhLayout = nullptr;
    int m_themetype = 1;
    QColor m_bnormalColor = "#FFFFFF";
    const int m_radius = 8;
    const int m_borderframew = 0;
    static CYearScheduleOutView *m_Scheduleview;
    bool m_selectFlag = false;
    QDate m_selectDate;
    QVector<bool> m_vlineflag; //节假日和日程标识
    CYearMonthView *m_monthView = nullptr;
    QVector<ScheduleDateRangeInfo> m_DateRangeInfo;
};

class CMonthDayRect;
class CYearMonthView : public DWidget
{
    Q_OBJECT
public:
    /**
     * @brief CYearMonthView 构造函数
     * @param parent 父类
     */
    explicit CYearMonthView(DWidget *parent = nullptr);
    /**
      *@brief ~CYearMonthView 析构函数
      */
    ~CYearMonthView() override;
    /**
     * @brief setDate 设置每个月的日期
     * @param date 日期
     */
    void setDate(const QDate date[42]);
    /**
     * @brief setTheMe 根据系统主题设置颜色
     * @param type 系统主题
     */
    void setTheMe(int type = 0);
    /**
     * @brief setLintFlag 设置是否有日程的标志
     * @param lineFlag 是否有日程的标志
     */
    void setLintFlag(const QVector<bool> &lineFlag);
private:
    /**
     * @brief updateSize 设置每天的日期所在矩形框的大小
     */
    void updateSize();
    /**
     * @brief getMousePosItem 获取点击日期的索引
     * @param pos 点击的点
     * @return 索引值
     */
    int getMousePosItem(const QPointF &pos);
protected:
    /**
     * @brief resizeEvent 设置每天的日期所在矩形框的大小
     * @param event 窗口大小调整函数
     */
    void resizeEvent(QResizeEvent *event) override;
    /**
     * @brief mousePressEvent 鼠标单击事件，单击日期显示当天的日程和节日。
     * @param event 鼠标事件
     */
    void mousePressEvent(QMouseEvent *event) override;
    /**
     * @brief mouseDoubleClickEvent  鼠标双击事件，双击跳转到上次打开的视图
     * @param event 鼠标事件
     */
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    /**
     * @brief mouseReleaseEvent 鼠标释放事件
     * @param event 鼠标事件
     */
    void mouseReleaseEvent(QMouseEvent *event) override;
    /**
     * @brief mouseMoveEvent 鼠标移动事件，设置hover状态
     * @param event 鼠标事件
     */
    void mouseMoveEvent(QMouseEvent *event) override;
    /**
     * @brief paintEvent 绘制日期以及当天状态
     * @param event 绘图事件
     */
    void paintEvent(QPaintEvent *event) override;
    /**
     * @brief leaveEvent 离开事件，设置当前选中的日期为空
     * @param event QT事件
     */
    void leaveEvent(QEvent *event) override;
private:
    /**
     * @brief mousePress 鼠标点击触发事件
     * @param QPoint 鼠标触发坐标
     */
    void mousePress(const QPoint &point);
signals:
    /**
     * @brief signalPressDate 鼠标点击日期的信号
     * @param date 传递时间参数
     */
    void signalPressDate(const QDate &date);
    /**
     * @brief signalDoubleClickDate 鼠标双击日期的信号
     * @param date 传递时间参数
     */
    void signalDoubleClickDate(const QDate &date);
private:
    QVector<CMonthDayRect *>        m_DayItem;
    int m_currentMonth = 1;
    QVector<bool> m_vlineflag; //节假日和日程标识
    bool                            m_press = false;
    int m_pressIndex = 0;
    //触摸状态 0：原始  1：点击  2：移动
    int         m_touchState{0};
    //触摸点击坐标
    QPoint      m_touchBeginPoint;
};


class CMonthDayRect
{
public:
    /**
     * @brief The CellEventType enum
     */
    enum CellEventType {
        CellNormal = 0,
        Cellhover = 1,
        CellPress = 2
    };
    /**
     * @brief The CellColor struct
     */
    struct CellColor {
        QColor normalColor;
        QColor hoverColor;
        QColor pressColor;
    };
    /**
     * @brief CMonthDayRect 构造函数
     */
    CMonthDayRect();
    /**
      * @brief ~CMonthDayRect 析构函数
      */
    ~CMonthDayRect();
    /**
     * @brief setDate 设置某一天的时间
     * @param date 时间
     */
    void setDate(const QDate &date);
    /**
     * @brief getDate 获取某一天的时间
     * @return 时间
     */
    QDate getDate()const;
    /**
     * @brief setCellEvent 设置某一天的状态，CellEventType
     * @param type CellEventType类型
     */
    void setCellEvent(const CellEventType &type);
    /**
     * @brief setIsCurrentMonth 设置是否是当前月
     * @param isCurrMonth 是否是当前月
     */
    void setIsCurrentMonth(const bool isCurrMonth);
    /**
     * @brief rect
     * @return 当天日期所在的矩形
     */
    QRectF rect() const;
    /**
     * @brief setRect 设置当天日期的矩形
     * @param rect 矩形
     */
    void setRect(const QRectF &rect);
    /**
     * @brief setRect 设置当天日期的矩形
     * @param x 设置矩形左上角的x坐标
     * @param y 设置矩形左上角的y坐标
     * @param w 设置矩形的宽度
     * @param h 设置矩形的高度
     */
    inline void setRect(qreal x, qreal y, qreal w, qreal h);
    /**
     * @brief paintItem 绘制年视图一天的所有状态并展示出来。
     * @param painter QPainter绘画
     * @param rect 年视图中每一天所在的矩形
     */
    void paintItem(QPainter *painter, const QRectF &rect);
    /**
     * @brief setLineFlag 设置是否有日程的标志
     * @param flag 标志
     */
    void setLineFlag(const bool flag);
    /**
     * @brief setTheMe 根据系统主题类型设置颜色
     * @param type 系统主题类型
     */
    static void setTheMe(int type = 0);
    /**
     * @brief setDevicePixelRatio
     * @param pixel
     */
    static void setDevicePixelRatio(const qreal pixel);
    /**
     * @brief setCurrentRect 设置当前天
     * @param currrect 当前天
     */
    static void setCurrentRect(CMonthDayRect *currrect);
private:
    QFont                   m_dayNumFont;
    QFont                   m_hightFont;
    int                     m_themetype = 0;
    CellEventType           m_cellEventType {CellNormal};
    QColor                  m_highColor = "#0081FF";
    QColor                  m_highTextColor = "#FFFFFF";
    bool                    m_vlineflag = false;
    QColor                  m_currentDayTextColor = "#2ca7f8";

    static QColor                   m_defaultTextColor;
    static QColor                   m_selectedTextColor;
    static QColor                   m_notCurrentTextColor;
    static CellColor                m_currentColor;
    static QColor                   m_ceventColor;
    static CMonthDayRect            *m_CurrentRect;
    static qreal                    m_DevicePixelRatio;

    QRectF                  m_rect;
    QDate                   m_date;
    bool                    m_isCurrentMonth = false;
};

#endif // YEARVIEW_H
