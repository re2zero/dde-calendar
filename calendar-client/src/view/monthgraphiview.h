/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     uniontech  <uniontech@uniontech.com>
*
* Maintainer: uniontech  <chenhaifeng@uniontech.com>
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
#ifndef MONTHGRAPHIVIEW_H
#define MONTHGRAPHIVIEW_H

#include "src/scheduledatainfo.h"
#include "src/lunardatastruct.h"
#include "src/dbusdatastruct.h"
#include "draginfographicsview.h"

#include <DGraphicsView>
#include <DMenu>

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QDate>
#include <QDrag>

#include <dtkwidget_global.h>

DWIDGET_USE_NAMESPACE

class CDayGraphicsItem;
class CMonthScheduleWidgetItem;
class CMonthScheduleView;

class CMonthGraphiview : public DragInfoGraphicsView
{
    Q_OBJECT
public:
    explicit CMonthGraphiview(QWidget *parent = nullptr);
    ~CMonthGraphiview() override;
    void setTheMe(int type = 0) override;
public:
    void setDate(const QVector<QDate> &showDate);
    //设置班休信息
    void setFestival(const QMap<QDate, int> &festivalInfo);
    //设置农历信息
    void setLunarInfo(const QMap<QDate, CaHuangLiDayInfo> &lunarCache);
    //设置是否显示农历信息
    void setLunarVisible(bool visible);
    //设置日程信息
    void setScheduleInfo(const QMap<QDate, QVector<ScheduleDataInfo> > &info);
    void setSelectSchedule(const ScheduleDataInfo &scheduleInfo);
    //设置搜索日程信息
    void setSearchScheduleInfo(const QVector<ScheduleDataInfo> &searchScheduleInfo);
private:
    void updateSize();
    void updateLunar();
    //更新日程数据显示
    void updateInfo() override;
    QPointF getItemPos(const QPoint &p, const QRectF &itemRect);
protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void changeEvent(QEvent *event) override;
    void wheelEvent(QWheelEvent *) override;
private:
    void setDragPixmap(QDrag *drag, DragInfoItem *item) override;
    bool MeetCreationConditions(const QDateTime &date) override;
    //判断时间是否相等
    bool IsEqualtime(const QDateTime &timeFirst, const QDateTime &timeSecond) override;
    //根据鼠标移动的距离判断是否创建日程
    bool JudgeIsCreate(const QPointF &pos)  override;
    void RightClickToCreate(QGraphicsItem *listItem, const QPoint &pos) override;
    void MoveInfoProcess(ScheduleDataInfo &info, const QPointF &pos) override;
    QDateTime getDragScheduleInfoBeginTime(const QDateTime &moveDateTime) override;
    QDateTime getDragScheduleInfoEndTime(const QDateTime &moveDateTime) override;
    PosInItem getPosInItem(const QPoint &p, const QRectF &itemRect)override;
    QDateTime getPosDate(const QPoint &p)override;
    void upDateInfoShow(const DragStatus &status = NONE, const ScheduleDataInfo &info = ScheduleDataInfo())override;
    /**
     * @brief slideEvent            触摸滑动事件处理
     * @param startPoint            触摸开始坐标
     * @param stopPort              触摸结束坐标
     */
    void slideEvent(QPointF &startPoint, QPointF &stopPort) override;
signals:
    void signalFontChange();
    void signalsViewSelectDate(QDate date);
    void signalsScheduleUpdate(const int id = 0);
public slots:
    void slotCreate(const QDateTime &date) override;
    void slotdelete(const int id = 0);
private:
    QVector<CDayGraphicsItem *>         m_DayItem;
    QMap<QDate, CaHuangLiDayInfo>         m_lunarCache;
    QMap<QDate, int>                    m_festivallist;
    int                                 m_currentMonth;
    CMonthScheduleView                  *m_MonthScheduleView = nullptr;
    QMap<QDate, QVector<ScheduleDataInfo> >      m_shceludelistdata;
};

class CDayGraphicsItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    enum HolidayStatus {H_NONE = 0x00, H_REST = 0x01, H_WORK = 0x02};
    explicit CDayGraphicsItem(QGraphicsItem *parent = nullptr);
    ~CDayGraphicsItem() override;
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
    void setitemnum(int num);
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
    QColor                  m_currentColor;
    int m_itemnum = 0;
    const int m_radius = 18;

public:
    static bool             m_LunarVisible;
};

#endif // MONTHGRAPHIVIEW_H
