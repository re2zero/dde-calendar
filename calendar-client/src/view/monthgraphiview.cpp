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
#include "monthgraphiview.h"
#include "../widget/monthWidget/monthscheduleview.h"
#include "../dialog/scheduledlg.h"
#include "../dialog/schedulectrldlg.h"
#include "../dialog/myscheduleview.h"
#include "../widget/touchgestureoperation.h"
#include "constants.h"

#include <DHiDPIHelper>

#include <QAction>
#include <QShortcut>
#include <QPainterPath>
#include <QMouseEvent>

bool CDayGraphicsItem::m_LunarVisible = false;

CMonthGraphiview::CMonthGraphiview(QWidget *parent)
    : DragInfoGraphicsView(parent)
{
    //设置显示左右下角圆角
    setShowRadius(true, true);
    QShortcut *shortcut = new QShortcut(parent);
    shortcut->setKey(QKeySequence(QLatin1String("Delete")));
    connect(shortcut, &QShortcut::activated, this,  &CMonthGraphiview::slotDeleteItem);
    //设置创建名称
    setBuildName("MONTH VIEW");

    m_MonthScheduleView = new CMonthScheduleView(this, m_Scene);
    connect(this, &CMonthGraphiview::signalFontChange, m_MonthScheduleView, &CMonthScheduleView::slotFontChange);

    for (int i = 0; i < DDEMonthCalendar::ItemSizeofMonthDay; ++i) {
        CDayGraphicsItem *item = new CDayGraphicsItem();
        item->setZValue(-1);
        m_DayItem.append(item);
        m_Scene->addItem(item);
        item->setitemnum(i);
    }
    updateSize();
}

CMonthGraphiview::~CMonthGraphiview()
{
    m_DayItem.clear();
}

void CMonthGraphiview::setTheMe(int type)
{
    m_themetype = type;
    DragInfoGraphicsView::setTheMe(type);

    for (int i = 0; i < m_DayItem.size(); ++i) {
        m_DayItem.at(i)->setTheMe(type);
    }

    m_MonthScheduleView->setTheMe(type);
}

void CMonthGraphiview::setDate(const QVector<QDate> &showDate)
{
    Q_ASSERT(showDate.size() == 42);
    if (showDate.at(0).day() != 1) {
        m_currentMonth = showDate.at(0).addMonths(1).month();
    } else {
        m_currentMonth = showDate.at(0).month();
    }
    for (int i = 0; i < m_DayItem.size(); ++i) {
        m_DayItem.at(i)->setData(showDate.at(i));
        m_DayItem.at(i)->setCurrentMonth(showDate.at(i).month() == m_currentMonth);
    }
    m_shceludelistdata.clear();
    this->scene()->update();
}

void CMonthGraphiview::setFestival(const QMap<QDate, int> &festivalInfo)
{
    m_festivallist = festivalInfo;
    for (int i = 0; i < m_DayItem.size(); ++i) {
        m_DayItem.at(i)->setStatus(static_cast<CDayGraphicsItem::HolidayStatus>(m_festivallist[m_DayItem.at(i)->getDate()]));
    }
    this->scene()->update();
}

void CMonthGraphiview::setLunarInfo(const QMap<QDate, CaHuangLiDayInfo> &lunarCache)
{
    m_lunarCache = lunarCache;
    updateLunar();
}

void CMonthGraphiview::setLunarVisible(bool visible)
{
    CDayGraphicsItem::m_LunarVisible = visible;
}

/**
 * @brief CMonthGraphiview::setScheduleInfo 设置日程信息
 * @param info
 */
void CMonthGraphiview::setScheduleInfo(const QMap<QDate, QVector<ScheduleDataInfo> > &info)
{
    m_shceludelistdata = info;
    updateInfo();
}

/**
 * @brief CMonthGraphiview::setSelectSearchSchedule     设置选择搜索日程
 * @param scheduleInfo
 */
void CMonthGraphiview::setSelectSearchSchedule(const ScheduleDataInfo &scheduleInfo)
{
    DragInfoGraphicsView::setSelectSearchSchedule(scheduleInfo);
    //获取所有的日程item
    QVector<QGraphicsRectItem *> mscheduleShowBtn = m_MonthScheduleView->getScheduleShowItem();

    for (int i = 0; i < mscheduleShowBtn.size(); ++i) {
        CMonthScheduleWidgetItem *item = dynamic_cast<CMonthScheduleWidgetItem *>(mscheduleShowBtn.at(i));

        if (item == nullptr) continue;

        if (scheduleInfo == item->getData()) {
            item->setStartValue(0);
            item->setEndValue(4);
            item->startAnimation();
        }
    }
}

/**
 * @brief CMonthGraphiview::setSearchScheduleInfo       设置搜索日程信息
 * @param searchScheduleInfo
 */
void CMonthGraphiview::setSearchScheduleInfo(const QVector<ScheduleDataInfo> &searchScheduleInfo)
{
    DragInfoItem::setSearchScheduleInfo(searchScheduleInfo);
    this->scene()->update();
}

void CMonthGraphiview::updateSize()
{
    m_Scene->setSceneRect(this->viewport()->rect());
    qreal w = m_Scene->width() / DDEMonthCalendar::AFewDaysofWeek;
    qreal h = m_Scene->height() / DDEMonthCalendar::LinesNumofMonth;
    QRectF rect ;
    int w_offset = 0;
    int h_offset = 0;

    for (int i = 0 ; i < m_DayItem.size(); ++i) {
        h_offset = i / DDEMonthCalendar::AFewDaysofWeek;
        w_offset = i % DDEMonthCalendar::AFewDaysofWeek;
        rect.setRect(w * w_offset,
                     h * h_offset,
                     w,
                     h);
        m_DayItem.at(i)->setRect(rect);
    }
}

void CMonthGraphiview::updateLunar()
{
    QDate date;
    CaHuangLiDayInfo info;
    QString lunarStr("");

    for (int i = 0 ; i < m_DayItem.size(); ++i) {
        date = m_DayItem.at(i)->getDate();
        if (m_lunarCache.contains(date)) {
            info = m_lunarCache.value(date);

            if (info.mLunarDayName == "初一") {
                info.mLunarDayName = info.mLunarMonthName + info.mLunarDayName;
            }

            if (info.mTerm.isEmpty()) {
                lunarStr = info.mLunarDayName;
            } else {
                lunarStr = info.mTerm;
            }
        } else {
            lunarStr = "";
        }
        m_DayItem.at(i)->setLunar(lunarStr);
    }
}

/**
 * @brief CMonthGraphiview::updateInfo      更新日程数据显示
 */
void CMonthGraphiview::updateInfo()
{
    DragInfoGraphicsView::updateInfo();
    int h = m_MonthScheduleView->getScheduleHeight();
    m_MonthScheduleView->setallsize(this->viewport()->width(),
                                    this->viewport()->height(),
                                    0, 0, 0, h);
    m_MonthScheduleView->setData(m_shceludelistdata, 1);

    switch (m_DragStatus) {
    case IsCreate:
        upDateInfoShow(IsCreate, m_DragScheduleInfo);
        break;
    case ChangeWhole:
        upDateInfoShow(ChangeWhole, m_DragScheduleInfo);
        break;
    default:

        break;
    }
    viewport()->update();
    update();
}


QPointF CMonthGraphiview::getItemPos(const QPoint &p, const QRectF &itemRect)
{
    QPointF scenePos = this->mapToScene(p);
    return  QPointF(scenePos.x() - itemRect.x(),
                    scenePos.y() - itemRect.y());
}

CMonthGraphiview::PosInItem CMonthGraphiview::getPosInItem(const QPoint &p, const QRectF &itemRect)
{
    QPointF scenePos = this->mapToScene(p);
    QPointF itemPos = QPointF(scenePos.x() - itemRect.x(),
                              scenePos.y() - itemRect.y());
    qreal bottomy = itemRect.width() - itemPos.x();

    if (itemPos.x() < 5) {
        return LEFT;
    }
    if (bottomy < 5) {
        return RIGHT;
    }
    return MIDDLE;
}

QDateTime CMonthGraphiview::getPosDate(const QPoint &p)
{
    if (!this->sceneRect().contains(p))
        return m_MoveDate;
    QRectF rect = this->sceneRect();
    qreal x = 0;
    qreal y = 0;

    if (p.x() < 0) {
        x = 0;
    } else if (p.x() > (rect.width() - 10)) {
        x = rect.width() - 10;
    } else {
        x = p.x();
    }

    if (p.y() < 0) {
        y = 0;
    } else if (p.y() > (rect.height() - 10)) {
        y = rect.height() - 10;
    } else {
        y = p.y();
    }

    int xoffset = qFloor(x / (rect.width() / DDEMonthCalendar::AFewDaysofWeek)) % DDEMonthCalendar::AFewDaysofWeek;
    int yoffset = qFloor(y / (rect.height() / DDEMonthCalendar::LinesNumofMonth)) % DDEMonthCalendar::LinesNumofMonth;

    return  QDateTime(m_DayItem[xoffset + yoffset * 7]->getDate(),
                      QTime(0, 0, 0));
}

void CMonthGraphiview::upDateInfoShow(const CMonthGraphiview::DragStatus &status, const ScheduleDataInfo &info)
{
    switch (status) {
    case NONE:
        Q_UNUSED(info);
        break;
    case ChangeBegin:
    case ChangeEnd: {
        m_MonthScheduleView->changeDate(info);
    }
    break;
    case ChangeWhole: {
    }
    break;
    case IsCreate:
        m_MonthScheduleView->updateDate(info);
        break;
    }
}

void CMonthGraphiview::slideEvent(QPointF &startPoint, QPointF &stopPort)
{
    qreal _movingLine {0};
    touchGestureOperation::TouchMovingDirection _touchMovingDir =
        touchGestureOperation::getTouchMovingDir(startPoint, stopPort, _movingLine);
    //切换月份 0 不切换  1 下一个  -1 上个月
    int delta {0};
    //移动偏移 25则切换月份
    const int moveOffset = 25;
    switch (_touchMovingDir) {
    case touchGestureOperation::T_TOP: {
        if (_movingLine > moveOffset) {
            delta = -1;
            startPoint = stopPort;
        }
        break;
    }
    case touchGestureOperation::T_BOTTOM: {
        if (_movingLine > moveOffset) {
            delta = 1;
            startPoint = stopPort;
        }
        break;
    }
    default:
        break;
    }
    if (delta != 0) {
        emit signalAngleDelta(delta);
    }
}

void CMonthGraphiview::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }

    QGraphicsItem *listItem = itemAt(event->pos());
    CMonthScheduleNumButton *item = dynamic_cast<CMonthScheduleNumButton *>(listItem);

    if (item != nullptr) {
        //双击切换视图
        if (item->getDate().year() > DDECalendar::QueryEarliestYear) {
            emit signalsViewSelectDate(item->getDate());
        }
        return;
    }

    CMonthScheduleWidgetItem *infoitem = dynamic_cast<CMonthScheduleWidgetItem *>(listItem);

    if (infoitem != nullptr) {
        CMyScheduleView dlg(infoitem->getData(), this);
        connect(&dlg, &CMyScheduleView::signalsEditorDelete, this, &CMonthGraphiview::signalsUpdateShcedule);
        connect(&dlg, &CMyScheduleView::signalViewtransparentFrame,
                this, &CMonthGraphiview::signalViewtransparentFrame);
        dlg.exec();
        return;
    }

    CDayGraphicsItem *Dayitem = dynamic_cast<CDayGraphicsItem *>(listItem);

    if (Dayitem != nullptr) {
        QPointF point = getItemPos(event->pos(), Dayitem->rect());
        if (point.y() < 38) {
            //双击切换视图
            if (Dayitem->getDate().year() > DDECalendar::QueryEarliestYear) {
                emit signalsViewSelectDate(Dayitem->getDate());
            }
        } else {
            //双击新建日程
            slotCreate(QDateTime(Dayitem->getDate(), QTime(0, 0, 0)));
        }
    }
}

void CMonthGraphiview::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    updateSize();
    updateInfo();
}
void CMonthGraphiview::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::FontChange) {
        emit signalFontChange();
    }
}

void CMonthGraphiview::wheelEvent(QWheelEvent *e)
{
    //如果滚动为上下则发送信号
    if (e->orientation() == Qt::Orientation::Vertical) {
        emit signalAngleDelta(e->angleDelta().y());
    }
}

void CMonthGraphiview::setDragPixmap(QDrag *drag, DragInfoItem *item)
{
    CMonthScheduleWidgetItem *infoitem = dynamic_cast<CMonthScheduleWidgetItem *>(item);
    drag->setPixmap(infoitem->getPixmap());
}

bool CMonthGraphiview::MeetCreationConditions(const QDateTime &date)
{
    return  qAbs(date.daysTo(m_PressDate) < 43);
}

bool CMonthGraphiview::IsEqualtime(const QDateTime &timeFirst, const QDateTime &timeSecond)
{
    return timeFirst.date() == timeSecond.date();
}

bool CMonthGraphiview::JudgeIsCreate(const QPointF &pos)
{
    return qAbs(pos.x() - m_PressPos.x()) > 20 || qAbs(m_PressDate.daysTo(getPosDate(pos.toPoint()))) > 0;
}

void CMonthGraphiview::RightClickToCreate(QGraphicsItem *listItem, const QPoint &pos)
{
    Q_UNUSED(pos);
    CDayGraphicsItem *Dayitem = dynamic_cast<CDayGraphicsItem *>(listItem);

    if (Dayitem != nullptr) {
        m_rightMenu->clear();
        m_rightMenu->addAction(m_createAction);
        m_createDate.setDate(Dayitem->getDate());
        m_rightMenu->exec(QCursor::pos());
    }
}

void CMonthGraphiview::MoveInfoProcess(ScheduleDataInfo &info, const QPointF &pos)
{
    qint64 offset       = m_PressDate.daysTo(m_MoveDate);
    info.setBeginDateTime(info.getBeginDateTime().addDays(offset));
    info.setEndDateTime(info.getEndDateTime().addDays(offset));
    qreal y = 0;
    QRectF rect = this->sceneRect();

    if (pos.y() < 0) {
        y = 0;
    } else if (pos.y() > rect.height()) {
        y = rect.height();
    } else {
        y = pos.y();
    }

    int yoffset = qFloor(y / (rect.height() / DDEMonthCalendar::LinesNumofMonth)) % DDEMonthCalendar::LinesNumofMonth;
    m_MonthScheduleView->updateDate(yoffset, info);
}

/**
 * @brief CMonthGraphiview::getDragScheduleInfoBeginTime        获取移动开始时间
 * @param moveDateTime
 * @return
 */
QDateTime CMonthGraphiview::getDragScheduleInfoBeginTime(const QDateTime &moveDateTime)
{
    //获取移动开始时间
    QDateTime _beginTime = moveDateTime.daysTo(m_InfoEndTime) < 0 ?
                           QDateTime(m_InfoEndTime.date(), m_InfoBeginTime.time()) :
                           QDateTime(moveDateTime.date(), m_InfoBeginTime.time());
    //如果开始时间晚与结束时间则减少一天
    _beginTime = _beginTime > m_InfoEndTime ? _beginTime.addDays(-1) : _beginTime;
    return _beginTime;
}

/**
 * @brief CMonthGraphiview::getDragScheduleInfoEndTime      获取结束时间
 * @param moveDateTime
 * @return
 */
QDateTime CMonthGraphiview::getDragScheduleInfoEndTime(const QDateTime &moveDateTime)
{
    //获取结束时间
    QDateTime _endTime = m_InfoBeginTime.daysTo(moveDateTime) < 0 ?
                         QDateTime(m_InfoBeginTime.date(), m_InfoEndTime.time()) :
                         QDateTime(moveDateTime.date(), m_InfoEndTime.time());
    //如果结束时间小于开始时间则添加一天
    _endTime = _endTime < m_InfoBeginTime ? _endTime.addDays(1)  : _endTime;
    return _endTime;
}

void CMonthGraphiview::slotCreate(const QDateTime &date)
{
    emit signalViewtransparentFrame(1);
    CScheduleDlg dlg(1, this);
    QDateTime tDatatime;
    tDatatime.setDate(date.date());

    if (date.date() == QDate::currentDate()) {
        tDatatime.setTime(QTime::currentTime());
    } else {
        tDatatime.setTime(QTime(8, 0));
    }

    dlg.setDate(tDatatime);
    dlg.setAllDay(true);

    if (dlg.exec() == DDialog::Accepted) {
        emit signalsUpdateShcedule();
        emit signalsScheduleUpdate(0);
    }
    emit signalViewtransparentFrame(0);
}

CDayGraphicsItem::CDayGraphicsItem(QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
    , m_Date(QDate::currentDate())
    , m_DayLunar("")
    , m_DayStatus(H_NONE)
{
    m_dayNumFont.setPixelSize(DDECalendar::FontSizeTwentyfour);
    m_dayNumFont.setWeight(QFont::Light);

    m_LunerFont.setPixelSize(DDECalendar::FontSizeTwelve);
    m_LunerFont.setWeight(QFont::Normal);
}

CDayGraphicsItem::~CDayGraphicsItem()
{
}

void CDayGraphicsItem::setData(const QDate &date)
{
    m_Date = date;
//    setLunar("");
}

void CDayGraphicsItem::setLunar(const QString &lunar)
{
    m_DayLunar = lunar;
}

void CDayGraphicsItem::setStatus(const CDayGraphicsItem::HolidayStatus &status)
{
    m_DayStatus = status;
}

void CDayGraphicsItem::setTheMe(int type)
{
    m_themetype = type;

    if (type == 0 || type == 1) {
        m_dayNumColor = "#000000";
        m_dayNumCurrentColor = "#FFFFFF";

        m_LunerColor = "#5E5E5E";
        m_LunerColor.setAlphaF(0.5);

        m_fillColor = Qt::white;
        m_banColor = "#FF7171";
        m_banColor.setAlphaF(0.1);
        m_xiuColor = "#ADFF71";
        m_xiuColor.setAlphaF(0.1);

        m_BorderColor = "#000000";
        m_BorderColor.setAlphaF(0.05);
    } else if (type == 2) {
        m_dayNumColor = "#C0C6D4";
        m_dayNumCurrentColor = "#B8D3FF";

        m_LunerColor = "#ABDAFF";
        m_LunerColor.setAlphaF(0.5);

        m_fillColor = "#000000";
        m_fillColor.setAlphaF(0.05);
        m_banColor = "#FF7171";
        m_banColor.setAlphaF(0.1);
        m_xiuColor = "#ADFF71";
        m_xiuColor.setAlphaF(0.1);

        m_BorderColor = "#000000";
        m_BorderColor.setAlphaF(0.05);
    }
    update();
}

void CDayGraphicsItem::setitemnum(int num)
{
    m_itemnum = num;
}

void CDayGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    const int hh = 36;
    m_currentColor = CScheduleDataManage::getScheduleDataManage()->getSystemActiveColor();
    painter->setRenderHints(QPainter::Antialiasing);
    //绘制背景
    if (m_LunarVisible) {
        switch (m_DayStatus) {
        case H_WORK:
            painter->setBrush(QBrush(m_banColor));
            break;
        case H_REST:
            painter->setBrush(QBrush(m_xiuColor));
            break;
        default:
            painter->setBrush(QBrush(m_fillColor));
            break;
        }
    } else {
        painter->setBrush(QBrush(m_fillColor));
    }

    if (!m_IsCurrentMonth) {
        painter->setOpacity(0.4);
    }
    QPen pen;
    pen.setWidth(1);
    pen.setColor(m_BorderColor);
    painter->setPen(pen);
    painter->drawRect(this->rect());
    painter->save();
    //绘制日期
    painter->setFont(m_dayNumFont);
    QRectF fillRect;
    if (m_LunarVisible) {
        fillRect.setRect(this->rect().x() + 3, this->rect().y() + 4, hh, hh);
    } else {
        fillRect.setRect(this->rect().x(), this->rect().y() + 4, this->rect().width(), hh);
    }
    //如果为当前时间
    if (m_Date == QDate::currentDate()) {
        //设置不透明度为1
        painter->setOpacity(1);
        QFont tfont = m_dayNumFont;
        tfont.setPixelSize(DDECalendar::FontSizeTwenty);
        painter->setFont(tfont);
        painter->setPen(m_dayNumCurrentColor);
        painter->save();
        painter->setBrush(QBrush(m_currentColor));
        painter->setPen(Qt::NoPen);
        if (m_LunarVisible)
            painter->drawEllipse(QRectF(this->rect().x() + 6, this->rect().y() + 4, hh - 8, hh - 8));
        else
            painter->drawEllipse(QRectF((this->rect().width() - hh + 8) / 2 + this->rect().x(), this->rect().y() + 4, hh - 8, hh - 8));
        painter->restore();
    } else {
        painter->setPen(m_dayNumColor);
    }

    fillRect.setY(fillRect.y() - 10);
    fillRect.setX(fillRect.x() - 1);
    painter->drawText(fillRect,
                      Qt::AlignCenter,
                      QString::number(m_Date.day()));
    painter->restore();
    //绘制农历
    if (m_LunarVisible) {
        QFontMetrics metrics(m_LunerFont);
        int Lunarwidth = metrics.width(m_DayLunar);
        qreal filleRectX = this->rect().width() - 12 - 3 - (58 + Lunarwidth) / 2;
        QRectF fillRect(this->rect().x() + filleRectX,
                        this->rect().y() + 9,
                        12,
                        12);

        if (filleRectX > hh) {
            painter->setRenderHint(QPainter::Antialiasing);
            painter->setRenderHint(QPainter::HighQualityAntialiasing);
            painter->setRenderHint(QPainter::SmoothPixmapTransform);
            switch (m_DayStatus) {
            case H_WORK: {
                QPixmap  pixmap = DHiDPIHelper::loadNxPixmap(":/resources/icon/ban.svg");
                painter->drawPixmap(fillRect.toRect(), pixmap);
            }
            break;
            case H_REST: {
                QPixmap pixmap = DHiDPIHelper::loadNxPixmap(":/resources/icon/xiu.svg");
                painter->drawPixmap(fillRect.toRect(), pixmap);
            }
            break;
            default:
                break;
            }
        }
        painter->setFont(m_LunerFont);
        painter->setPen(m_LunerColor);
        painter->drawText(QRectF(this->rect().x() + this->rect().width() - 58,
                                 this->rect().y() + 6, 58, 18), Qt::AlignCenter, m_DayLunar);
    }
}
