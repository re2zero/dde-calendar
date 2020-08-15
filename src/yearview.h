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
class CYearSchceduleView;
class CYearSchceduleOutView;
class WeekIndicator;
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
    explicit CYearView(QWidget *parent = nullptr);
    void setFirstWeekday(int weekday);
    int getDateType(const QDate &date);
    inline bool cellSelectable() const
    {
        return m_cellSelectable;
    }
    void setTheMe(int type = 0);
    static void SchceduleViewHide();
    void getInfoAndSetLineFlag();
signals:
    void signalcurrentDateChanged(QDate date);
    void cellSelectableChanged(bool cellSelectable) const;
    void signaldoubleclickDate(QDate date);
    void signalselectMonth(QDate date);
    void signalselectWeekwindow(QDate date);
    void signalupdateschcedule();
    void signalHideInfo();
    void signalSelectInfo(bool flag);
public slots:
    void setCurrentDate(const QDate date, int type = 0);
    void setCellSelectable(bool selectable);
    void updateInfoWIndow(bool flag);
    void slotupdateSchedule(const int id);
    void slotDoubleClickDate(const QDate &date);
    void slotPressClickDate(const QDate &date);
private:
    int getDateIndex(const QDate &date) const;
    bool eventFilter(QObject *o, QEvent *e) override;
    void updateDate();
    void createYearSchceduleView(QWidget *parent = nullptr);
protected:
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

protected:
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
    static CYearSchceduleOutView *m_Scheduleview;
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
    CYearMonthView(DWidget *parent = nullptr);
    ~CYearMonthView() override;
    void setDate(const QDate date[42]);
    void setTheMe(int type = 0);
    void setLintFlag(const QVector<bool> &lineFlag);
private:
    void updateSize();
    int getMousePosItem(const QPointF &pos);
protected:
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void leaveEvent(QEvent *event) override;
signals:
    void signalPressDate(const QDate &date);
    void signalDoubleClickDate(const QDate &date);
private:
    QVector<CMonthDayRect *>        m_DayItem;
    int                             m_currentMonth =1;
    QVector<bool>                   m_vlineflag; //节假日和日程标识
    bool                            m_press = false;
    int                             m_pressIndex =0;
};


class CMonthDayRect
{
public:
    enum CellEventType {
        CellNormal = 0,
        Cellhover = 1,
        CellPress = 2
    };
    struct CellColor {
        QColor normalColor;
        QColor hoverColor;
        QColor pressColor;
    };
    CMonthDayRect();
    ~CMonthDayRect();
    void setDate(const QDate &date);
    QDate getDate()const;
    void setCellEvent(const CellEventType &type);
    void setIsCurrentMonth(const bool isCurrMonth);
    QRectF rect() const;
    void setRect(const QRectF &rect);
    inline void setRect(qreal x, qreal y, qreal w, qreal h);
    void paintItem(QPainter *painter, const QRectF &rect);
    void setLineFlag(const bool flag);

    static void setTheMe(int type = 0);
    static void setDevicePixelRatio(const qreal pixel);
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
