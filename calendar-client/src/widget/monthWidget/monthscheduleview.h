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
#ifndef MONTHSCHEDULEVIEW_H
#define MONTHSCHEDULEVIEW_H

#include "schedulestructs.h"
#include "draginfoitem.h"

#include <DLabel>
#include <DPushButton>
#include <DListWidget>
#include <DFontSizeManager>

#include <QMouseEvent>
#include <QGraphicsRectItem>
#include <QGraphicsScene>

DWIDGET_USE_NAMESPACE
class CMonthScheduleWidgetItem;
class QVBoxLayout;
class CMonthScheduleNumButton;
class SchecduleRemindWidget;
class QPropertyAnimation;
class QSequentialAnimationGroup;
class CWeekScheduleView;
class CScheduleDataManage;
class CMonthScheduleView : public QObject
{
    Q_OBJECT

public:
    CMonthScheduleView(QWidget *parent, QGraphicsScene *scene);
    ~CMonthScheduleView() override;
    void setallsize(int w, int h, int left, int top, int buttom, int itemHeight = 22);
    void setData(QVector<ScheduleDateRangeInfo> &data, int currentMonth);
    void setTheMe(int type = 0);
    void updateData();
    void updateHigh();
    QVector<QGraphicsRectItem *> getScheduleShowItem() const;
    int getScheduleHeight() const
    {
        return m_ItemHeight;
    }
    void updateDate(const ScheduleDtailInfo &info);
    void changeDate(const ScheduleDtailInfo &info);
    void updateDate(const int row, const ScheduleDtailInfo &info);
signals:
    void signalsUpdateShcedule(int id = 0);
    void signalsCurrentScheduleDate(QDate date);
    void signalViewtransparentFrame(int type);
    void signalUpdateUI(int type);
    void signalPressScheduleShow(const bool isShow, const ScheduleDtailInfo &out = ScheduleDtailInfo());
public slots:
    void slotdeleteitem(CMonthScheduleWidgetItem *item);
    void slotedititem(CMonthScheduleWidgetItem *item, int type = 0);
    void slotFontChange();

private:
    void updateDateShow(QVector<QVector<MScheduleDateRangeInfo>> &vCMDaySchedule, QVector<QGraphicsRectItem *> &schudeleShowItem);
    void createScheduleItemWidget(MScheduleDateRangeInfo info, int cnum, QVector<QGraphicsRectItem *> &schudeleShowItem);
    void createScheduleNumWidget(MScheduleDateRangeInfo info, int cnum, QVector<QGraphicsRectItem *> &schudeleShowItem);
    void computePos(int cnum, QDate bgeindate, QDate enddate, QPoint &pos, int &fw, int &fh);

private:
    QVector<ScheduleDateRangeInfo> m_data;
    int m_cNum = 2; //日程层数
    QWidget *m_parernt = nullptr;
    int m_currentMonth = 0;
    QDate m_beginDate;
    QDate m_endDate;
    int m_width = 0;
    int m_height = 0;
    int m_leftMagin = 0;
    int m_topMagin = 0;
    int m_buttommagin = 0;
    QVector<CWeekScheduleView *> m_weekSchedule;
    int m_ItemHeight = 22;
    QGraphicsScene *m_Scene = nullptr;
};

class CWeekScheduleView : public QObject
{
    Q_OBJECT
    typedef QVector<MScheduleDateRangeInfo> RowScheduleInfo;

public:
    CWeekScheduleView(QObject *parent = nullptr);
    ~CWeekScheduleView() override;

public:
    void setData(QVector<ScheduleDateRangeInfo> &data, const int position, const int count = 7);
    bool addData(const ScheduleDtailInfo &info);
    void changeDate(const ScheduleDtailInfo &info);
    void setHeight(const int ScheduleHeight, const int DayHeigth);
    QVector<RowScheduleInfo> getMScheduleInfo() const
    {
        return m_MScheduleInfo;
    }
    QVector<QGraphicsRectItem *> &getScheduleShowItem()
    {
        return m_scheduleShowItem;
    }
    void updateSchedule(const bool isNormalDisplay, const ScheduleDtailInfo &info = ScheduleDtailInfo());
    void clearItem();

private:
    void setMaxNum();
    void mScheduleClear();
    void sortAndFilter(QVector<MScheduleDateRangeInfo> &vMDaySchedule);
    /**
     * @brief addShowSchedule       添加显示的日程
     * @param startPos              其实位置
     * @param endPos                结束位置
     * @param addRow                需要添加的行
     * @param addInfo               添加的日程
     */
    void addShowSchedule(const int &startPos, const int &endPos, const int &addRow, const ScheduleDtailInfo &addInfo);

private:
    QVector<QGraphicsRectItem *> m_scheduleShowItem;
    QVector<RowScheduleInfo> m_MScheduleInfo;
    QVector<ScheduleDtailInfo> m_ScheduleInfo;
    QVector<int> m_ColumnScheduleCount;
    int m_ScheduleHeight = 0;
    int m_DayHeight = 0;
    int m_MaxNum = 0;
    QDate beginDate;
    QDate endDate;
    int m_colum = 0;
};

class CMonthScheduleNumButton : public QObject
    , public QGraphicsRectItem
{
    Q_OBJECT

public:
    CMonthScheduleNumButton(QGraphicsItem *parent = nullptr);
    ~CMonthScheduleNumButton() override;
    void setColor(QColor color1, QColor color2, bool GradientFlag = false);
    void setText(QColor tcolor, QFont font, QPoint pos);
    void setSizeType(DFontSizeManager::SizeType sizeType);
    void setTransparentB(bool t, QColor tcolor);
    void setDate(QDate date)
    {
        m_date = date;
    }
    QDate getDate() const
    {
        return m_date;
    }
    void setData(int num)
    {
        m_num = num;
    }
signals:
    void signalsCurrentScheduleDate(QDate date);
    void signalPressScheduleShow(const bool isShow, const ScheduleDtailInfo &out = ScheduleDtailInfo());

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    bool m_GradientFlag;
    QColor m_color1;
    QColor m_color2;
    QColor m_textcolor;
    QFont m_font;
    QPoint m_pos;
    int m_num;
    QColor m_transparentcolor;
    bool m_transparentf = false;
    QDate m_date;
    DFontSizeManager::SizeType m_SizeType = DFontSizeManager::T8;
};

class CMonthScheduleWidgetItem : public DragInfoItem
{
    Q_OBJECT
public:
    explicit CMonthScheduleWidgetItem(QRect rect, QGraphicsItem *parent = nullptr, int edittype = 0);
    ~CMonthScheduleWidgetItem() override;
    QPixmap getPixmap();

private:
    void paintBackground(QPainter *painter, const QRectF &rect, const int isPixMap = false) override;

private:
    QPoint m_pos;
};

#endif // MONTHSCHEDULEVIEW_H
