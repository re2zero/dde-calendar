#ifndef YEARVIEW_H
#define YEARVIEW_H
#include "weekindicator.h"
#include "constants.h"
#include <DWidget>
#include <QList>
#include <QDate>
#include <QStyleOption>
#include <QLabel>
#include <QSignalMapper>
#include "calendardbus.h"
#include <DFrame>
#include "customframe.h"
DWIDGET_USE_NAMESPACE
class CalendarDBus;
class CaLunarDayInfo;
class QLabel;
class CYearSchceduleView;
enum CalendarYearDayType {
    SO_YFestival = QStyleOption::SO_CustomBase + 0x01,
    SO_YWeekends = QStyleOption::SO_CustomBase + 0x02,
    SO_YWeekendsAndFestival = SO_YFestival | SO_YWeekends,
    SO_YNotCurrentMonth = 0x04,
    SO_YNotCurrentMonthFestival = SO_YNotCurrentMonth | SO_YFestival,
    SO_YDefault,
};
class CYearView: public CustomFrame
{
    Q_OBJECT

    Q_PROPERTY(QColor backgroundCircleColor MEMBER m_backgroundCircleColor DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY(QColor defaultTextColor MEMBER m_defaultTextColor DESIGNABLE true SCRIPTABLE true)
    // Q_PROPERTY(QColor defaultLunarColor MEMBER m_defaultLunarColor DESIGNABLE true SCRIPTABLE true)
    //Q_PROPERTY(QColor festivalLunarColor MEMBER m_festivalLunarColor DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY(QColor weekendsTextColor MEMBER m_weekendsTextColor DESIGNABLE true SCRIPTABLE true)
    //Q_PROPERTY(QColor weekendsLunarColor MEMBER m_weekendsLunarColor DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY(QColor topBorderColor MEMBER m_topBorderColor DESIGNABLE true SCRIPTABLE true)
    Q_PROPERTY(bool cellSelectable READ cellSelectable WRITE setCellSelectable NOTIFY cellSelectableChanged)
public:
    explicit CYearView(QWidget *parent = 0);
    void setFirstWeekday(int weekday);
    int getDateType(const QDate &date);
    inline bool cellSelectable() const
    {
        return m_cellSelectable;
    }
    void updateSelectState();
    void setTheMe(int type = 0);
    void updateHigh();
signals:
    void dateSelected(const QDate date, const CaLunarDayInfo &detail) const;
    void signalcurrentDateChanged(QDate date);
    void cellSelectableChanged(bool cellSelectable) const;
    void singanleActiveW(CYearView *w);

    void signaldoubleclickDate(QDate date);
    void signalselectMonth(QDate date);
    void signalHideInfo();
    void signalSelectInfo(bool flag);
public slots:
    void setCurrentDate(const QDate date, int type = 0);
    void setCellSelectable(bool selectable);
    void handleCurrentDateChanged(const QDate date, const CaLunarDayInfo &detail);
    void updateInfoWIndow(bool flag);
private:
    int getDateIndex(const QDate &date) const;
    const QString getCellDayNum(int pos);
    const QDate getCellDate(int pos);
    void paintCell(QWidget *cell);
    bool eventFilter(QObject *o, QEvent *e);
    void updateDate();

private slots:
    void cellClicked(QWidget *cell);
    void setSelectedCell(int index);
protected:
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
protected:
    void paintEvent ( QPaintEvent *e);
private:
    CustomFrame *m_currentMouth;
    QList<QWidget *> m_cellList;
    QDate m_days[42];
    QDate m_currentDate;
    QStringList m_monthList;
    int m_selectedCell = -1;
    bool m_cellSelectable = true;

    QFont m_dayNumFont, m_momthFont;

    QColor m_topBorderColor = Qt::red;
    QColor m_backgroundCircleColor = "#2ca7f8";

    QColor m_defaultTextColor = Qt::black;
    QColor m_currentDayTextColor = "#2ca7f8";
    QColor m_weekendsTextColor = Qt::black;
    QColor m_selectedTextColor = Qt::white;
    QColor m_festivalTextColor = Qt::black;
    QColor m_notCurrentTextColor = "#b2b2b2";
    int m_firstWeekDay = 0;
    QGridLayout *m_gridLayout;
    int cellwidth = 20;
    int cellheight = 20;
    QVBoxLayout *m_hhLayout;
    int                   m_themetype  = 1;
    QColor m_highColor = "#0081FF";
    QColor m_highTextColor = "#FFFFFF";
    QColor m_ceventColor = "#FB2525";
    QFont m_hightFont;
    QColor                   m_bnormalColor = "#FFFFFF";
    int                      m_radius = 8;
    int                      m_borderframew = 0;
    CYearSchceduleView      *m_Scheduleview = nullptr;
    bool                     m_selectFlag = false;
    QDate                    m_selectDate;

    QVector<bool>            m_vlineflag; //节假日和日程标识
};

#endif // YEARVIEW_H
