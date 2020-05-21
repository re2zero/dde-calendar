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
#include "alldayeventview.h"
#include <QAction>
#include <QListWidget>
#include <QLabel>
#include <QPainter>
#include <QHBoxLayout>
#include <QStylePainter>
#include <QRect>
#include <QMimeData>
#include "schceduledlg.h"
#include "myschceduleview.h"
#include "scheduledatamanage.h"
#include <DMessageBox>
#include <DPushButton>
#include <DHiDPIHelper>
#include <DPalette>
#include <QDrag>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>

#include <QGraphicsOpacityEffect>
#include "schedulecoormanage.h"
#include "schcedulectrldlg.h"

DGUI_USE_NAMESPACE

CAllDayEventWidgetItem::CAllDayEventWidgetItem(QRect rect, QGraphicsItem *parent /*= nullptr*/, int edittype)
    : QGraphicsRectItem(parent),
      m_rect(rect)
{
    setZValue(0);
    setAcceptHoverEvents(true);
    setRect(rect);
    m_editType = edittype;
//    m_font.setFamily("PingFangSC-Light");
    const int duration = 200;
    m_properAnimationFirst = new  QPropertyAnimation(this, "offset", this);
    m_properANimationSecond  = new QPropertyAnimation(this, "offset", this);
    m_properAnimationFirst->setDuration(duration);
    m_properANimationSecond->setDuration(duration);
    m_properAnimationFirst->setEasingCurve(QEasingCurve::InOutQuad);
    m_properANimationSecond->setEasingCurve(QEasingCurve::InOutQuad);
    m_Group = new QSequentialAnimationGroup(this);
    m_Group->addAnimation(m_properAnimationFirst);
    m_Group->addAnimation(m_properANimationSecond);
}

void CAllDayEventWidgetItem::setData(const ScheduleDtailInfo &vScheduleInfo)
{
    m_vScheduleInfo = vScheduleInfo;
    m_vSelectflag = false;
    m_vHoverflag = false;
    m_vHighflag = false;
    update();
}

ScheduleDtailInfo CAllDayEventWidgetItem::getData() const
{
    return m_vScheduleInfo;
}

void CAllDayEventWidgetItem::updateitem()
{

}

void CAllDayEventWidgetItem::setPressFlag(const bool flag)
{
    m_press = flag;
    update();
}

void CAllDayEventWidgetItem::setFont(DFontSizeManager::SizeType type)
{
    m_sizeType = type;
}

void CAllDayEventWidgetItem::setOffset(const int &offset)
{
    setRect(QRect(m_rect.x() - offset,
                  m_rect.y() - offset / 2,
                  m_rect.width() + offset * 2,
                  m_rect.height() + offset));
    setZValue(offset);
    update();
}

bool CAllDayEventWidgetItem::hasSelectSchedule(const ScheduleDtailInfo &info)
{
    return info == m_vScheduleInfo;
}

void CAllDayEventWidgetItem::setStartValue(const int value)
{
    m_properAnimationFirst->setStartValue(value);
    m_properANimationSecond->setEndValue(value);
}

void CAllDayEventWidgetItem::setEndValue(const int value)
{
    m_properAnimationFirst->setEndValue(value);
    m_properANimationSecond->setStartValue(value);
}

void CAllDayEventWidgetItem::startAnimation()
{
    if (m_Group->state() != QAnimationGroup::Running) {
        m_Group->start();
    }
}


void CAllDayEventWidgetItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_vHoverflag = true;
    update();
}

void CAllDayEventWidgetItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_vHoverflag = false;
    m_press = false;
    update();
}

void CAllDayEventWidgetItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    m_font = DFontSizeManager::instance()->get(m_sizeType, m_font);

    painter->setRenderHints(QPainter::Antialiasing);
    CSchedulesColor gdcolor = CScheduleDataManage::getScheduleDataManage()->getScheduleColorByType(m_vScheduleInfo.type.ID);
    QRectF drawrect = this->rect();
    QLinearGradient linearGradient(drawrect.topLeft().x(), 0, drawrect.topRight().x(), 0);

    QColor color1 = gdcolor.gradientFromC;
    QColor color2 = gdcolor.gradientToC;
    QColor textcolor = gdcolor.textColor;

    m_vHighflag = CScheduleDataManage::getScheduleDataManage()->getSearchResult(m_vScheduleInfo);


    if (CScheduleDataManage::getScheduleDataManage()->getPressSelectInfo() == m_vScheduleInfo ) {
        if (m_vScheduleInfo.IsMoveInfo ==
                CScheduleDataManage::getScheduleDataManage()->getPressSelectInfo().IsMoveInfo) {
            m_vHighflag = true;
        } else {
            painter->setOpacity(0.4);
            textcolor.setAlphaF(0.4);
        }
    }
    int themetype = CScheduleDataManage::getScheduleDataManage()->getTheme();





    if (m_vHoverflag) {
        color1 = gdcolor.hovergradientFromC;
        color2 = gdcolor.hovergradientToC;
    } else if (m_vHighflag) {
        color1 = gdcolor.hightlightgradientFromC;
        color2 = gdcolor.hightlightgradientToC;
    }
    if (m_vSelectflag || m_press) {
        color1 = gdcolor.pressgradientFromC;
        color2 = gdcolor.pressgradientToC;
        textcolor.setAlphaF(0.6);
    }
    linearGradient.setColorAt(0, color1);
    linearGradient.setColorAt(1, color2);
    QRectF fillRect = drawrect;
    //将直线开始点设为0，终点设为1，然后分段设置颜色
    painter->setBrush(linearGradient);
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(fillRect, 8, 8);

    painter->setFont(m_font);
    painter->setPen(textcolor);
    QFontMetrics fm = painter->fontMetrics();
    QString tStitlename = m_vScheduleInfo.titleName;
    tStitlename.replace("\n", "");
    QString str = tStitlename;

    QString tstr;
    for (int i = 0; i < str.count(); i++) {
        tstr.append(str.at(i));
        int widthT = fm.width(tstr) + 5;
        if (widthT >= fillRect.width() - 13) {
            tstr.chop(2);
            break;
        }
    }
    if (tstr != str) {
        tstr = tstr + "...";
    }

    painter->drawText(QRect(fillRect.topLeft().x() + 13, fillRect.y(), fillRect.width(), fillRect.height()),
                      Qt::AlignLeft | Qt::AlignVCenter, tstr);
    if (m_vHoverflag && !m_vSelectflag) {
        QRectF trect = QRectF(fillRect.x() + 0.5, fillRect.y() + 0.5, fillRect.width() - 1, fillRect.height() - 1);
        painter->save();

        QPen pen;
        QColor selcolor;

        if (themetype == 2) {
            selcolor = "#FFFFFF";
        } else {
            selcolor = "#000000";
        }
        selcolor.setAlphaF(0.08);

        pen.setColor(selcolor);
        pen.setWidthF(1);
        pen.setStyle(Qt::SolidLine);
        painter->setBrush(Qt::NoBrush);
        painter->setPen(pen);
        painter->drawRoundedRect(trect, 8, 8);
        painter->restore();
    }
    if (m_vSelectflag) {
        QColor selcolor = "#000000";
        selcolor.setAlphaF(0.05);
        painter->setBrush(selcolor);
        painter->setPen(Qt::NoPen);
        painter->drawRoundedRect(fillRect, 8, 8);
    }
}


void CAllDayEventWeekView::setTheMe(int type)
{
    m_themetype=type;
    updateDateShow();
}

void CAllDayEventWeekView::setRange(int w, int h, QDate begindate, QDate enddate, int rightmagin)
{
    m_beginDate = begindate;
    m_endDate = enddate;
    m_coorManage->setRange(w, h, begindate, enddate, rightmagin);
    m_Scene->setSceneRect(0, 0, w, h);
    m_rightmagin = rightmagin;
    updateDateShow();

}

void CAllDayEventWeekView::setRange(QDate begin, QDate end)
{
    m_beginDate = begin;
    m_endDate = end;
    getCoorManage()->setDateRange(begin, end);
}

void CAllDayEventWeekView::updateHigh()
{
    for (int i = 0; i < m_baseShowItem.count(); i++) {
        m_baseShowItem.at(i)->update();
    }
}

void CAllDayEventWeekView::setSelectSchedule(const ScheduleDtailInfo &info)
{
    for (int i = 0; i < m_baseShowItem.size(); ++i) {
        CAllDayEventWidgetItem *item = m_baseShowItem.at(i);
        if (item->hasSelectSchedule(info)) {
            QRectF rect = item->rect();
            centerOn(0, rect.y());
            setTransformationAnchor(QGraphicsView::AnchorViewCenter);
            item->setStartValue(0);
            item->setEndValue(4);
            item->startAnimation();
        }
    }
}

void CAllDayEventWeekView::setMargins(int left, int top, int right, int bottom)
{
    setViewportMargins(QMargins(left, top, right, bottom));
}

int CAllDayEventWeekView::upDateInfoShow(const DragStatus &status, const ScheduleDtailInfo &info)
{
    QVector<ScheduleDtailInfo> vListData;
    vListData = m_scheduleInfo;
    switch (status) {
    case NONE:
        Q_UNUSED(info);
        break;
    case ChangeBegin:
    case ChangeEnd: {
        int index = vListData.indexOf(info);
        vListData[index] = info;
    }
    break;
    case ChangeWhole:
        vListData.append(info);
        break;
    case IsCreate:
        vListData.append(info);
        break;
    }


    qSort(vListData.begin(), vListData.end());

    QVector<MScheduleDateRangeInfo> vMDaySchedule;
    for (int i = 0; i < vListData.count(); i++) {
        QDate tbegindate = vListData.at(i).beginDateTime.date();
        QDate tenddate = vListData.at(i).endDateTime.date();
        if (tbegindate < m_beginDate)
            tbegindate = m_beginDate;
        if (tenddate > m_endDate)
            tenddate = m_endDate;
        MScheduleDateRangeInfo sinfo;
        sinfo.bdate = tbegindate;
        sinfo.edate = tenddate;
        sinfo.tData = vListData.at(i);
        sinfo.state = false;
        vMDaySchedule.append(sinfo);
    }
    QVector<QVector<int> > vCfillSchedule;
    vCfillSchedule.resize(vListData.count());
    int tNum = m_beginDate.daysTo(m_endDate) + 1;
    for (int i = 0; i < vListData.count(); i++) {
        vCfillSchedule[i].resize(tNum);
        vCfillSchedule[i].fill(-1);
    }
    //首先填充跨天日程
    for (int i = 0; i < vMDaySchedule.count(); i++) {
        if (vMDaySchedule[i].state)
            continue;
        int bindex = m_beginDate.daysTo(vMDaySchedule[i].bdate);
        int eindex = m_beginDate.daysTo(vMDaySchedule[i].edate);
        int c = -1;
        for (int k = 0; k < vListData.count(); k++) {
            int t = 0;
            for (t = bindex; t <= eindex; t++) {
                if (vCfillSchedule[k][t] != -1) {
                    break;
                }
            }
            if (t == eindex + 1) {
                c = k;
                break;
            }
        }
        if (c == -1)
            continue;

        bool flag = false;
        for (int sd = bindex; sd <= eindex; sd++) {
            if (vCfillSchedule[c][sd] != -1)
                continue;
            vCfillSchedule[c][sd] = i;
            flag = true;
        }
        if (flag)
            vMDaySchedule[i].state = true;
    }
    QVector<QVector<ScheduleDtailInfo> > vResultData;
    for (int i = 0; i < vListData.count(); i++) {
        QVector<int> vId;
        for (int j = 0; j < tNum; j++) {
            if (vCfillSchedule[i][j] != -1) {
                int k = 0;
                for (; k < vId.count(); k++) {
                    if (vId[k] == vCfillSchedule[i][j])
                        break;
                }
                if (k == vId.count())
                    vId.append(vCfillSchedule[i][j]);
            }
        }
        QVector<ScheduleDtailInfo> tData;
        for (int j = 0; j < vId.count(); j++) {
            tData.append(vMDaySchedule[vId[j]].tData);
        }
        if (!tData.isEmpty())
            vResultData.append(tData);
    }

    int m_topMagin;
    if (vResultData.count() < 2) {
        m_topMagin = 31;
    } else if (vResultData.count()  < 6) {
        m_topMagin = 31 + (vResultData.count()  - 1) * (itemHeight+1);

    } else {
        m_topMagin = 123;
    }
    setFixedHeight(m_topMagin - 3);
    setDayData(vResultData);
    update();
    emit signalUpdatePaint(m_topMagin);
    return m_topMagin;
}

CAllDayEventWeekView::CAllDayEventWeekView(QWidget *parent, int edittype)
    : DGraphicsView (parent),
      m_Scene(new QGraphicsScene(this)),
      m_rightMenu(new DMenu(this))
{
    setScene(m_Scene);
    m_editType = edittype;
    m_widgetFlag = false;
    setContentsMargins(0, 0, 0, 0);
    QPalette pal = palette();
    pal.setColor(QPalette::Light, QColor(0, 0, 0, 0));
    pal.setColor(QPalette::Dark, QColor(0, 0, 0, 0));
    pal.setColor(QPalette::Base, QColor(0, 0, 0, 0));
    setPalette(pal);

    m_coorManage = new CScheduleCoorManage;

    m_editAction = new QAction(tr("Edit"), this);
    m_deleteAction = new QAction(tr("Delete"), this);
    m_createAction = new QAction(tr("New event"), this);
    connect(m_createAction, &QAction::triggered, this, &CAllDayEventWeekView::slotCreate);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setViewportMargins(0, 0, 0, 0);
    setMouseTracking(true);
    viewport()->setMouseTracking(true);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    m_Drag = new QDrag(this);
}

CAllDayEventWeekView::~CAllDayEventWeekView()
{
    delete m_coorManage;
    m_coorManage = nullptr;
}

void CAllDayEventWeekView::setDayData(const QVector<QVector<ScheduleDtailInfo> > &vlistData)
{
    m_vlistData = vlistData;
    m_widgetFlag = false;
    updateDateShow();
}

void CAllDayEventWeekView::setInfo(const QVector<ScheduleDtailInfo> &info)
{
    m_scheduleInfo = info;

}

void CAllDayEventWeekView::slotCreate()
{
    emit signalViewtransparentFrame(1);
    CSchceduleDlg dlg(1, this);
    QDateTime tDatatime;
    tDatatime.setDate(m_dianjiDay);
    if (m_dianjiDay == QDate::currentDate()) {
        tDatatime.setTime(QTime::currentTime());
    } else {
        tDatatime.setTime(QTime(8, 0));
    }
    dlg.setDate(tDatatime);
    dlg.setAllDay(true);
    if (dlg.exec() == DDialog::Accepted) {
        emit signalsUpdateShcedule(0);
    }
    emit signalViewtransparentFrame(0);
}

void CAllDayEventWeekView::slotDoubleEvent()
{
    m_updateDflag  = true;
    emit signalsUpdateShcedule(0);
}


void CAllDayEventWeekView::mousePressEvent(QMouseEvent *event)
{
    setPressSelectInfo(ScheduleDtailInfo());
    CAllDayEventWidgetItem *item = dynamic_cast<CAllDayEventWidgetItem *>(itemAt(event->pos()));
    if (event->button() == Qt::RightButton) {
        emit signalScheduleShow(false);
        m_press = false;
        m_DragStatus =NONE;
        if (item == nullptr) {
            m_rightMenu->clear();
            m_rightMenu->addAction(m_createAction);
            m_dianjiDay = m_coorManage->getsDate(mapFrom(this, event->pos()));
            m_rightMenu->exec(QCursor::pos());

        } else {
//            setPressSelectInfo(item->getData());
            if (item->getData().type.ID == 4)
                return;
            m_rightMenu->clear();
            m_rightMenu->addAction(m_editAction);
            m_rightMenu->addAction(m_deleteAction);
            QAction *action_t = m_rightMenu->exec(QCursor::pos());
            if (action_t == m_editAction) {
                emit signalViewtransparentFrame(1);
                CSchceduleDlg dlg(0, this);
                dlg.setData(item->getData());
                if (dlg.exec() == DDialog::Accepted) {
                    emit signalsUpdateShcedule(0);
                }
                emit signalViewtransparentFrame(0);
            } else if (action_t == m_deleteAction) {
                DeleteItem(item->getData());
            }
        }
    } else if (event->button() == Qt::LeftButton) {
        if (item != nullptr) {
            setPressSelectInfo(item->getData());
            m_press = true;
            item->setPressFlag(true);
            emit signalScheduleShow(true, item->getData());
        } else {
            emit signalScheduleShow(false);
        }
        DragPressEvent(event->pos(),item);
    }
}

void CAllDayEventWeekView::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    setCursor(Qt::ArrowCursor);
    m_press = false;
    switch (m_DragStatus) {
    case IsCreate:
        if (qAbs(m_MoveDate.daysTo(m_PressDate)<7)) {
            CSchceduleDlg dlg(1, this);
            dlg.setData(m_DragScheduleInfo);
            if (dlg.exec() == DDialog::Accepted) {
            }
            emit signalsUpdateShcedule(0);
        }
        break;
    case ChangeBegin:
        if (m_MoveDate != m_InfoBeginTime.date()) {
            updateScheduleInfo(m_DragScheduleInfo);
        }
        break;
    case ChangeEnd:
        if (m_MoveDate != m_InfoEndTime.date()) {
            updateScheduleInfo(m_DragScheduleInfo);
        }
        break;
    default:
        break;
    }
    m_DragStatus = NONE;
    update();
    emit signalSceneUpdate();
}

void CAllDayEventWeekView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        return;
    }
    emit signalScheduleShow(false);
    DGraphicsView::mouseDoubleClickEvent(event);
    CAllDayEventWidgetItem *item = dynamic_cast<CAllDayEventWidgetItem *>(itemAt(event->pos()));
    if (item == nullptr) {
        m_dianjiDay = m_coorManage->getsDate(mapFrom(this, event->pos()));
        emit signalViewtransparentFrame(1);
        CSchceduleDlg dlg(1, this);
        QDateTime tDatatime;
        tDatatime.setDate(m_dianjiDay);
        tDatatime.setTime(QTime::currentTime());
        dlg.setDate(tDatatime);
        dlg.setAllDay(true);
        if (dlg.exec() == DDialog::Accepted) {
            emit signalsUpdateShcedule(0);
        }
        emit signalViewtransparentFrame(0);
    } else {
        emit signalViewtransparentFrame(1);
        m_updateDflag  = false;
        CMySchceduleView dlg(item->getData(), this);
//        dlg.setSchedules(item->getData());
        connect(&dlg, &CMySchceduleView::signalsEditorDelete, this, &CAllDayEventWeekView::slotDoubleEvent);
        dlg.exec();
        emit signalViewtransparentFrame(0);
        disconnect(&dlg, &CMySchceduleView::signalsEditorDelete, this, &CAllDayEventWeekView::slotDoubleEvent);
        if (item == nullptr) {
            return;
        }
        if (!m_updateDflag)
            item->updateitem();
    }
}

void CAllDayEventWeekView::mouseMoveEvent(QMouseEvent *event)
{
    DGraphicsView::mouseMoveEvent(event);
    if (m_press) {
        emit signalScheduleShow(false);
        m_press = false;
    }

    CAllDayEventWidgetItem *item = dynamic_cast<CAllDayEventWidgetItem *>(itemAt(event->pos()));
    if (item != nullptr) {
        if (item->getData().type.ID != 4) {
            if (m_DragStatus == NONE) {
                switch (getPosInItem(event->pos(),item->rect())) {
                case LEFT:
                case RIGHT:
                    setCursor(Qt::SplitHCursor);
                    break;
                default:
                    setCursor(Qt::ArrowCursor);
                    break;
                }
            }
        }

    } else {
        if (m_DragStatus == NONE) {
            setCursor(Qt::ArrowCursor);
        }
    }

    QDate gDate =  m_coorManage->getsDate(mapFrom(this, event->pos()));
    switch (m_DragStatus) {
    case IsCreate:
        if (qAbs(event->pos().x()-m_PressPos.x())>20 ||qAbs(m_PressDate.daysTo(gDate))>0) {
            m_isCreate = true;
        }
        if (m_isCreate) {
            if (m_MoveDate !=gDate) {
                m_MoveDate = gDate;
                m_DragScheduleInfo = getScheduleInfo(m_PressDate,m_MoveDate);
                upDateInfoShow(IsCreate,getScheduleInfo(m_PressDate,m_MoveDate));
                setPressSelectInfo(m_DragScheduleInfo);
            }
        }
        break;
    case ChangeBegin:
        if (m_MoveDate !=gDate) {
            m_MoveDate = gDate;
            if (m_MoveDate.daysTo(m_InfoEndTime.date())<0) {
                m_DragScheduleInfo.beginDateTime = QDateTime(m_InfoEndTime.date(),QTime(0,0,0));
            } else {
                m_DragScheduleInfo.beginDateTime = QDateTime(m_MoveDate,QTime(0,0,0));
            }
            m_DragScheduleInfo.endDateTime = m_InfoEndTime;
            upDateInfoShow(ChangeBegin,m_DragScheduleInfo);
        }
        break;
    case ChangeEnd:
        if (m_MoveDate !=gDate) {
            m_MoveDate = gDate;
            if (m_InfoBeginTime.date().daysTo(m_MoveDate)<0) {
                m_DragScheduleInfo.endDateTime = QDateTime(m_InfoBeginTime.date(),QTime(23,59,0));
            } else {
                m_DragScheduleInfo.endDateTime = QDateTime(m_MoveDate,QTime(23,59,0));
            }
            m_DragScheduleInfo.beginDateTime =m_InfoBeginTime;
            upDateInfoShow(ChangeEnd,m_DragScheduleInfo);
        }
        break;
    case ChangeWhole: {
        if (!m_PressRect.contains(event->pos())) {
            m_DragScheduleInfo.IsMoveInfo = true;
            setPressSelectInfo(m_DragScheduleInfo);
            upDateInfoShow();
            Qt::DropAction dropAciton = m_Drag->exec( Qt::MoveAction);
            Q_UNUSED(dropAciton);
            m_Drag = nullptr;
            m_DragStatus = NONE;
            m_DragScheduleInfo.IsMoveInfo = false;
            setPressSelectInfo(m_DragScheduleInfo);
            emit signalsUpdateShcedule(0);
        }
    }
    break;

    default:
        break;
    }


}

void CAllDayEventWeekView::wheelEvent(QWheelEvent *event)
{
    emit signalScheduleShow(false);
    DGraphicsView::wheelEvent(event);
}

void CAllDayEventWeekView::paintEvent(QPaintEvent *event)
{
    DGraphicsView::paintEvent(event);
}

void CAllDayEventWeekView::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("Info")) {
        if (event->source() !=this) {
            QJsonParseError json_error;
            QString str = event->mimeData()->data("Info");
            QJsonDocument jsonDoc(QJsonDocument::fromJson(str.toLocal8Bit(), &json_error));

            if (json_error.error != QJsonParseError::NoError) {
                event->ignore();
            }
            QJsonObject rootobj = jsonDoc.object();
            ScheduleDtailInfo info =
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->JsonObjectToInfo(rootobj);
            if (info.rpeat>0) {
                event->ignore();
            } else {
                event->accept();
            }
        } else {
            event->accept();
        }

    } else {
        event->ignore();
    }
}

void CAllDayEventWeekView::dragLeaveEvent(QDragLeaveEvent *event)
{
    upDateInfoShow();
    m_MoveDate = m_MoveDate.addMonths(2);
}

void CAllDayEventWeekView::dragMoveEvent(QDragMoveEvent *event)
{
    QString str = event->mimeData()->data("Info");
    QDate gDate =  m_coorManage->getsDate(mapFrom(this, event->pos()));
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(str.toLocal8Bit(), &json_error));

    if (json_error.error != QJsonParseError::NoError) {
        return;
    }

    if (m_MoveDate !=gDate) {
        m_MoveDate = gDate;
        QJsonObject rootobj = jsonDoc.object();
        m_DragScheduleInfo =
            CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->JsonObjectToInfo(rootobj);
        if (m_DragScheduleInfo.allday) {
            qint64 offset = m_PressDate.daysTo(m_MoveDate);
            m_DragScheduleInfo.beginDateTime = m_DragScheduleInfo.beginDateTime.addDays(offset);
            m_DragScheduleInfo.endDateTime    = m_DragScheduleInfo.endDateTime.addDays(offset);
        } else {
            qint64 offset = m_DragScheduleInfo.beginDateTime.daysTo(m_DragScheduleInfo.endDateTime);
            m_DragScheduleInfo.allday = true;
            m_DragScheduleInfo.remind = true;
//            if (m_DragScheduleInfo.remind) {
            m_DragScheduleInfo.remindData.time = QTime(9, 0);
            m_DragScheduleInfo.remindData.n = 1;
//            }
//            m_DragScheduleInfo.remind = false;
            m_DragScheduleInfo.beginDateTime = QDateTime(m_MoveDate,QTime(0,0,0));
            m_DragScheduleInfo.endDateTime = QDateTime(m_MoveDate.addDays(offset),QTime(23,59,59));
        }
        m_DragScheduleInfo.IsMoveInfo = true;
        upDateInfoShow(ChangeWhole,m_DragScheduleInfo);
        setPressSelectInfo(m_DragScheduleInfo);

    }
}

void CAllDayEventWeekView::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("Info")) {
        if (event->source()!=this || m_MoveDate !=m_PressDate) {
            updateScheduleInfo(m_DragScheduleInfo);
        } else {
            emit signalsUpdateShcedule(0);
        }
        m_DragStatus = NONE;
        m_MoveDate = m_MoveDate.addMonths(2);
    }
}

void CAllDayEventWeekView::updateDateShow()
{
    m_Scene->setSceneRect(0,
                          0,
                          m_Scene->width(),
                          (itemHeight + 1)*m_vlistData.size());

    for (int i = 0; i < m_baseShowItem.count(); i++) {
        delete  m_baseShowItem[i];
    }
    m_baseShowItem.clear();
    for (int i = 0; i < m_vlistData.size(); ++i) {
        createItemWidget(i);
    }
}

void CAllDayEventWeekView::updateScheduleInfo(const ScheduleDtailInfo &info)
{
    if (info.rpeat >0) {
        CSchceduleDlg::ChangeRecurInfo(this,info,
                                       m_PressScheduleInfo,m_themetype);
    } else {
        CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(
            info);
    }
    emit signalsUpdateShcedule(0);
}

void CAllDayEventWeekView::DragPressEvent(const QPoint &pos, const CAllDayEventWidgetItem *item)
{
    m_PressPos = pos;
    m_PressDate = m_coorManage->getsDate(mapFrom(this, pos));
    m_MoveDate = m_PressDate.addMonths(-2);
    if (item != nullptr) {
        if (item->getData().type.ID == 4)
            return;
        m_DragScheduleInfo = item->getData();
        m_PressScheduleInfo = item->getData();
        m_InfoBeginTime = m_DragScheduleInfo.beginDateTime;
        m_InfoEndTime = m_DragScheduleInfo.endDateTime;
        m_PressRect = item->rect();
        switch (getPosInItem(pos,item->rect())) {
        case LEFT:
            m_DragStatus = ChangeBegin;
            setCursor(Qt::SplitHCursor);
            break;
        case RIGHT:
            m_DragStatus = ChangeEnd;
            setCursor(Qt::SplitHCursor);
            break;
        default:
            m_DragStatus = ChangeWhole;
            QMimeData *mimeData = new QMimeData();
            mimeData->setText(m_DragScheduleInfo.titleName);
            mimeData->setData("Info",
                              CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->InfoToJson(m_DragScheduleInfo).toUtf8());


            if (m_Drag ==nullptr) {
                m_Drag = new QDrag(this);
            }
            m_Drag->setMimeData(mimeData);
            QPointF itemPos = QPointF(pos.x()-item->rect().x(),
                                      pos.y()-item->rect().y());
            m_Drag->setHotSpot(itemPos.toPoint());
            break;
        }
    } else {
        m_DragStatus = IsCreate;
        m_isCreate = false;
    }
}

void CAllDayEventWeekView::createItemWidget(int index, bool average)
{
    Q_UNUSED(average)
    for (int i = 0; i < m_vlistData[index].size(); ++i) {
        const ScheduleDtailInfo &info = m_vlistData[index].at(i);

        QRect drawrect = m_coorManage->getAllDayDrawRegion(info.beginDateTime.date(), info.endDateTime.date());
        drawrect.setY(3 + (itemHeight + 1)*index);
        drawrect.setHeight(itemHeight);

        CAllDayEventWidgetItem *gwi = new CAllDayEventWidgetItem(drawrect, nullptr, m_editType);
        gwi->setCoorManage(m_coorManage);
        gwi->setData(m_vlistData[index].at(i));
        m_Scene->addItem(gwi);
        m_baseShowItem.append(gwi);
    }
}

ScheduleDtailInfo CAllDayEventWeekView::getScheduleInfo(const QDate &beginDate, const QDate &endDate)
{
    ScheduleDtailInfo info;
    if (beginDate.daysTo(endDate)>0) {
        info.beginDateTime = QDateTime(beginDate,QTime(0,0,0));
        info.endDateTime = QDateTime(endDate,QTime(23,59,59));
    } else {
        info.beginDateTime = QDateTime(endDate,QTime(0,0,0));
        info.endDateTime = QDateTime(beginDate,QTime(23,59,00));
    }
    info.titleName = tr("New Event");
    info.allday = true;
    info.remind = true;
    info.id = 0;
    info.remindData.n = 1;
    info.remindData.time = QTime(9, 0);
    info.RecurID = 0;
    CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->GetType(
        1, info.type);
    info.rpeat = 0;

    return info;
}

CAllDayEventWeekView::PosInItem CAllDayEventWeekView::getPosInItem(const QPoint &p, const QRectF &itemRect)
{
    QPointF scenePos = this->mapToScene(p);
    QPointF itemPos = QPointF(scenePos.x()-itemRect.x(),
                              scenePos.y()-itemRect.y());
    int bottomy = itemRect.width()- itemPos.x();
    if (itemPos.x()<5) {
        return LEFT;
    }
    if (bottomy <5) {
        return RIGHT;
    }
    return MIDDLE;
}

void CAllDayEventWeekView::setPressSelectInfo(const ScheduleDtailInfo &info)
{
    CScheduleDataManage::getScheduleDataManage()->setPressSelectInfo(info);
}

void CAllDayEventWeekView::DeleteItem(const ScheduleDtailInfo &info)
{
    emit signalViewtransparentFrame(1);
    if (info.rpeat == 0) {
        CSchceduleCtrlDlg msgBox(this);
        msgBox.setText(tr("You are deleting an event."));
        msgBox.setInformativeText(tr("Are you sure you want to delete this event?"));
        DPushButton *noButton = msgBox.addPushButton(tr("Cancel"));
        DPushButton *yesButton = msgBox.addPushButton(tr("Delete"), 1);
        msgBox.updatesize();
        DPalette pa = yesButton->palette();
        if (m_themetype == 0 || m_themetype == 1) {
            pa.setColor(DPalette::ButtonText, Qt::red);

        } else {
            pa.setColor(DPalette::ButtonText, "#FF5736");

        }
        yesButton->setPalette(pa);
        msgBox.exec();

        if (msgBox.clickButton() == noButton) {
            emit signalViewtransparentFrame(0);
            return;
        } else if (msgBox.clickButton() == yesButton) {
            CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->deleteScheduleInfoById(info.id);
        }
    } else {
        if (info.RecurID == 0) {
            CSchceduleCtrlDlg msgBox(this);
            msgBox.setText(tr("You are deleting an event."));
            msgBox.setInformativeText(tr("Do you want to delete all occurrences of this event, or only the selected occurrence?"));
            DPushButton *noButton = msgBox.addPushButton(tr("Cancel"));
            DPushButton *yesallbutton = msgBox.addPushButton(tr("Delete All"));
            DPushButton *yesButton = msgBox.addPushButton(tr("Delete Only This Event"));
            msgBox.updatesize();
            DPalette pa = yesButton->palette();
            if (m_themetype == 0 || m_themetype == 1) {
                pa.setColor(DPalette::ButtonText, Qt::white);
                pa.setColor(DPalette::Dark, QColor("#25B7FF"));
                pa.setColor(DPalette::Light, QColor("#0098FF"));
            } else {
                pa.setColor(DPalette::ButtonText, "#B8D3FF");
                pa.setColor(DPalette::Dark, QColor("#0056C1"));
                pa.setColor(DPalette::Light, QColor("#004C9C"));
            }
            yesButton->setPalette(pa);
            msgBox.exec();

            if (msgBox.clickButton() == noButton) {
                emit signalViewtransparentFrame(0);
                return;
            } else if (msgBox.clickButton() == yesallbutton) {
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->deleteScheduleInfoById(info.id);
            } else if (msgBox.clickButton() == yesButton) {

                ScheduleDtailInfo newschedule;
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(info.id, newschedule);
                newschedule.ignore.append(info.beginDateTime);
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);
            }
        } else {
            CSchceduleCtrlDlg msgBox(this);
            msgBox.setText(tr("You are deleting an event."));
            msgBox.setInformativeText(tr("Do you want to delete this and all future occurrences of this event, or only the selected occurrence?"));
            DPushButton *noButton = msgBox.addPushButton(tr("Cancel"));
            DPushButton *yesallbutton = msgBox.addPushButton(tr("Delete All Future Events"));
            DPushButton *yesButton = msgBox.addPushButton(tr("Delete Only This Event"));
            msgBox.updatesize();
            DPalette pa = yesButton->palette();
            if (m_themetype == 0 || m_themetype == 1) {
                pa.setColor(DPalette::ButtonText, Qt::white);
                pa.setColor(DPalette::Dark, QColor("#25B7FF"));
                pa.setColor(DPalette::Light, QColor("#0098FF"));
            } else {
                pa.setColor(DPalette::ButtonText, "#B8D3FF");
                pa.setColor(DPalette::Dark, QColor("#0056C1"));
                pa.setColor(DPalette::Light, QColor("#004C9C"));
            }
            yesButton->setPalette(pa);
            msgBox.exec();

            if (msgBox.clickButton() == noButton) {
                emit signalViewtransparentFrame(0);
                return;
            } else if (msgBox.clickButton() == yesallbutton) {
                ScheduleDtailInfo newschedule;
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(info.id, newschedule);
                newschedule.enddata.type = 2;
                newschedule.enddata.date = info.beginDateTime.addDays(-1);
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);

            } else if (msgBox.clickButton() == yesButton) {

                ScheduleDtailInfo newschedule;
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(info.id, newschedule);
                newschedule.ignore.append(info.beginDateTime);
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);
            }
        }
    }
    emit signalsUpdateShcedule(info.id);
    emit signalViewtransparentFrame(0);
}


void CAllDayEventWeekView::slotdeleteitem( CAllDayEventWidgetItem *item)
{
    emit signalsUpdateShcedule(0);
    updateDateShow();
    update();
}

void CAllDayEventWeekView::slotedititem(CAllDayEventWidgetItem *item, int type)
{
    emit signalsUpdateShcedule(0);
    updateDateShow();
    update();
}


void CAllDayEventWeekView::slotDeleteItem()
{
    if (CScheduleDataManage::getScheduleDataManage()->getPressSelectInfo().type.ID <0) {
        return;
    }
    if (CScheduleDataManage::getScheduleDataManage()->getPressSelectInfo().type.ID !=4) {
        DeleteItem(CScheduleDataManage::getScheduleDataManage()->getPressSelectInfo());
    }
    CScheduleDataManage::getScheduleDataManage()->setPressSelectInfo(ScheduleDtailInfo());
}

void CAllDayEventWeekView::slotUpdateScene()
{
    this->scene()->update();
}
