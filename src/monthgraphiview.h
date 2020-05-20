#ifndef MONTHGRAPHIVIEW_H
#define MONTHGRAPHIVIEW_H

#include <DGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QDate>
#include <QDrag>
#include <DMenu>

#include <dtkwidget_global.h>
#include "calendardbus.h"
#include "schedulestructs.h"



DWIDGET_USE_NAMESPACE

class CDayGraphicsItem;
class CMonthSchceduleWidgetItem;
class CMonthSchceduleView;

class CMonthGraphiview : public DGraphicsView
{
    Q_OBJECT
public:
    enum PosInItem {LEFT,MIDDLE,RIGHT};
    enum DragStatus {IsCreate,ChangeBegin,ChangeEnd,ChangeWhole,NONE};
    explicit CMonthGraphiview(QWidget *parent = nullptr);
    ~CMonthGraphiview() override;
    void setTheMe(int type = 0);
public:
    void setDate(const QDate date[42]);
    void setFestivalInfo(const QVector<FestivalInfo> &info)
    {
        m_festivallist = info;
    }
    void setLunarInfo(QMap<QDate, CaLunarDayInfo> *lunarCache);
    void setLunarVisible(bool visible);
    void setScheduleInfo(const QVector<ScheduleDateRangeInfo> &info);
    void setSelectSchedule(const ScheduleDtailInfo &scheduleInfo);
private:
    void updateSize();
    void updateLunar();
    void updateInfo();
    char getFestivalInfoByDate(const QDate &date);
    QPointF getItemPos(const QPoint &p, const QRectF &itemRect);
    void DragPressEvent(const QPoint &pos,QGraphicsItem *item);
    ScheduleDtailInfo getScheduleInfo(const QDate &beginDate,const QDate &endDate);
    PosInItem getPosInItem(const QPoint &p,const QRectF &itemRect);
    QDate getPosDate(const QPoint &p);
    void upDateInfoShow(const DragStatus &status = NONE,const ScheduleDtailInfo &info =ScheduleDtailInfo());
    void updateScheduleInfo(const ScheduleDtailInfo &info);
    void DeleteItem(const ScheduleDtailInfo &info);
    void setPressSelectInfo(const ScheduleDtailInfo &info);
protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void changeEvent(QEvent *event) override;
    void wheelEvent(QWheelEvent *) override;
signals:
    void signalFontChange();
    void signalsViewSelectDate(QDate date);
    void slotSchceduleUpdate(const int id =0);
    void signalsSchceduleUpdate(const int id = 0);
    void signalViewtransparentFrame(const int id = 0);
    void signalScheduleShow(const bool isShow, const ScheduleDtailInfo &out = ScheduleDtailInfo());
public slots:
    void slotCreate(const QDate &date);
    void slotCreate();
    void slotdelete(const int id =0);
    void slotDeleteItem();
private:
    QGraphicsScene                      *m_Scene;
    QVector<CDayGraphicsItem *>         m_DayItem;
    QMap<QDate, CaLunarDayInfo>         *m_lunarCache;
    QVector<FestivalInfo>               m_festivallist;
    int                                 m_currentMonth;
    bool                                m_LunarVisible;
    ScheduleDtailInfo                   m_ScheduleInfo;
    QDate                               m_createDate;
    int                                 m_press = false;

    QAction                                     *m_createAction;     // 创建日程
    QAction                                     *m_editAction;
    QAction                                     *m_deleteAction;
    int                                 m_themetype = 0;

    CMonthSchceduleView                 *m_MonthSchceduleView;
    QVector<ScheduleDateRangeInfo>      m_shceludelistdata;
    DMenu                               *m_rightMenu;


    DragStatus                      m_DragStatus =NONE;
    bool                            m_isCreate;
    QDate                           m_PressDate;
    QDate                           m_MoveDate;
    QPoint                          m_PressPos;
    ScheduleDtailInfo               m_DragScheduleInfo;
    QDateTime                       m_InfoBeginTime;
    QDateTime                       m_InfoEndTime;
    QDrag                           *m_Drag = nullptr;
    ScheduleDtailInfo               m_PressScheduleInfo;
    QRectF                          m_PressRect;
};


class CDayGraphicsItem :public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    enum HolidayStatus {H_NONE = 0x00,H_REST = 0x01,H_WORK = 0x02};
    explicit CDayGraphicsItem( QGraphicsItem *parent = nullptr);
    ~CDayGraphicsItem() Q_DECL_OVERRIDE;
    void setData(const QDate &date);
    void setLunar(const QString &lunar);
    void setStatus(const HolidayStatus &status);
    void setCurrentMonth(bool isCurrent)
    {
        m_IsCurrentMonth = isCurrent;
    }
    void setTheMe(int type = 0);
    QDate getDate()const
    {
        return m_Date;
    }
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
private:
    QDate                   m_Date;
    QString                 m_DayLunar;
    HolidayStatus           m_DayStatus;
    QFont                   m_dayNumFont;
    QColor                  m_dayNumColor;
    QColor                  m_dayNumCurrentColor;
    QFont                   m_LunerFont;
    QColor                  m_LunerColor;
    QColor                  m_BorderColor;
    bool                    m_IsCurrentMonth = false;
    QColor                  m_fillColor = Qt::white;
    QColor                  m_banColor = "#FBE9B7";
    QColor                  m_xiuColor = "#D4FFB3";
    int                     m_themetype = 0;
public:
    static bool             m_LunarVisible;
};

#endif // MONTHGRAPHIVIEW_H
