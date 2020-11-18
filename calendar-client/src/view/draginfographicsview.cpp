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
#include "draginfographicsview.h"
#include "scheduledlg.h"
#include "schedulectrldlg.h"
#include "myscheduleview.h"
#include "constants.h"

#include <DMenu>

#include <QMimeData>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>

DragInfoGraphicsView::DragInfoGraphicsView(DWidget *parent)
    : DGraphicsView(parent)
    , m_Scene(new QGraphicsScene(this))
    , m_rightMenu(new DMenu(this))
    , m_MoveDate(QDateTime::currentDateTime())
{
    setFrameShape(QFrame::NoFrame);
    setScene(m_Scene);
    setContentsMargins(0, 0, 0, 0);

    m_editAction = new QAction(tr("Edit"), this);
    m_deleteAction = new QAction(tr("Delete"), this);
    m_createAction = new QAction(tr("New event"), this);
    connect(m_createAction, &QAction::triggered, this,
            static_cast<void (DragInfoGraphicsView::*)()>(&DragInfoGraphicsView::slotCreate));
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    this->setViewportMargins(0, 0, 0, 0);
    setMouseTracking(true);
    viewport()->setMouseTracking(true);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    m_Drag = new QDrag(this);
    //设置接受触摸事件
    setAttribute(Qt::WA_AcceptTouchEvents);
    grabGesture(Qt::TapAndHoldGesture);
    grabGesture(Qt::PanGesture);
    grabGesture(Qt::TapGesture);
    grabGesture(Qt::SwipeGesture);

    m_touchAnimation = new QPropertyAnimation(this, "touchSlidePos");
    //设置动画时间
    m_touchAnimation->setDuration(1000);
    //设置动画曲线
    m_touchAnimation->setEasingCurve(QEasingCurve::OutQuart);
}

DragInfoGraphicsView::~DragInfoGraphicsView()
{

}

void DragInfoGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }
    stopTouchAnimation();
    if (event->source() == Qt::MouseEventSynthesizedByQt) {
        //如果为触摸点击则记录相关状态并改变触摸状态
        DGraphicsView::mousePressEvent(event);
        m_TouchBeginPoint = event->pos();
        m_TouchBeginTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
        m_touchState = TS_PRESS;
        return;
    }
    mousePress(event->pos());
}

void DragInfoGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() ==Qt::RightButton) {
        return;
    }
    if (event->source() == Qt::MouseEventSynthesizedByQt) {
        //如果为触摸点击状态则调用左击事件处理
        if (m_touchState == TS_PRESS) {
            mousePress(m_TouchBeginPoint.toPoint());
        }
        if (m_touchState == TS_SLIDE) {
            stopTouchAnimation();
            const qint64 timeOffset = QDateTime::currentDateTime().toMSecsSinceEpoch() - m_TouchBeginTime;
            //如果为快速滑动则开启滑动动画效果
            if (timeOffset < 150) {
                m_touchAnimation->setStartValue(verticalScrollBar()->sliderPosition());
                m_touchAnimation->setEndValue(verticalScrollBar()->minimum());
                switch (m_touchMovingDir) {
                case touchGestureOperation::T_TOP: {
                    //如果手势往上
                    m_touchAnimation->setStartValue(verticalScrollBar()->sliderPosition());
                    m_touchAnimation->setEndValue(verticalScrollBar()->maximum());
                    m_touchAnimation->start();
                    break;
                }
                case touchGestureOperation::T_BOTTOM: {
                    //如果手势往下
                    m_touchAnimation->setStartValue(verticalScrollBar()->sliderPosition());
                    m_touchAnimation->setEndValue(verticalScrollBar()->minimum());
                    m_touchAnimation->start();
                    break;
                }
                default:

                    break;
                }
            }
        }
    }
    m_touchState = TS_NONE;
    mouseReleaseScheduleUpdate();
}


void DragInfoGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    //移动偏移
    const int lengthOffset = 5;
    if (event->source() == Qt::MouseEventSynthesizedByQt) {
        m_touchMovingDir = touchGestureOperation::T_MOVE_NONE;
        switch (m_touchState) {
        case TS_NONE: {
            break;
        }
        case TS_PRESS: {
            //1 点击
            qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
            qint64 timeOffset = currentTime - m_TouchBeginTime;
            //获取移动距离
            qreal movingLength = QLineF(m_TouchBeginPoint, event->pos()).length();
            //如果移动距离小于5且点击时间大于250毫秒小于900毫秒则为拖拽移动状态
            if (movingLength < lengthOffset && (timeOffset > 250 && timeOffset < 900)) {
                m_touchState = TS_DRAG_MOVE;
                m_touchDragMoveState = 1;
            }
            //如果移动距离大于5则为滑动状态
            if (movingLength > lengthOffset) {
                m_touchState = TS_SLIDE;
            }
            break;
        }
        case TS_DRAG_MOVE: {
            //2 拖拽移动
            qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
            qint64 timeOffset = currentTime - m_TouchBeginTime;
            qreal movingLength = QLineF(m_TouchBeginPoint, event->pos()).length();
            //如果移动距离小于5且点击时间大于900毫秒则为长按状态
            if (movingLength < lengthOffset && (timeOffset > 900)) {
                m_touchState = TS_LONG_PRESS;
            }
            if (movingLength > lengthOffset) {
                if (m_touchDragMoveState == 1) {
                    mousePress(m_TouchBeginPoint.toPoint());
                }
                m_touchDragMoveState = 2;
            }
            break;
        }
        case TS_SLIDE: {
            //3 滑动
            QPointF _currentPoint = event->pos();
            slideEvent(m_TouchBeginPoint, _currentPoint);
            break;
        }
        default:
            break;
        }
    }

    if (m_press) {
        emit signalScheduleShow(false);
        m_press = false;
        DragInfoItem::setPressFlag(false);
    }

    DragInfoItem *item = dynamic_cast<DragInfoItem *>(itemAt(event->pos()));

    if (item != nullptr) {
        if (item->getData().type.ID != DDECalendar::FestivalTypeID) {
            if (m_DragStatus == NONE) {
                switch (getPosInItem(event->pos(),item->rect())) {
                case LEFT:
                case RIGHT:
                    setCursor(Qt::SplitHCursor);
                    break;
                case TOP:
                case BOTTOM:
                    setCursor(Qt::SplitVCursor);
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
    QDateTime gDate =  getPosDate(event->pos());

    switch (m_DragStatus) {
    case IsCreate:
        m_isCreate = JudgeIsCreate(event->pos());
        if (m_isCreate) {
            if (!IsEqualtime(m_MoveDate,gDate)) {
                m_MoveDate = gDate;
                m_DragScheduleInfo = getScheduleInfo(m_PressDate,m_MoveDate);
                upDateInfoShow(IsCreate,m_DragScheduleInfo);
                setPressSelectInfo(m_DragScheduleInfo);
            }
        }
        break;
    case ChangeBegin:
        if (!IsEqualtime(m_MoveDate,gDate)) {
            m_MoveDate = gDate;
            m_DragScheduleInfo.beginDateTime =
                getDragScheduleInfoBeginTime(m_MoveDate);
            m_DragScheduleInfo.endDateTime = m_InfoEndTime;
            upDateInfoShow(ChangeBegin,m_DragScheduleInfo);
        }
        break;
    case ChangeEnd:
        if (!IsEqualtime(m_MoveDate,gDate)) {
            m_MoveDate = gDate;
            m_DragScheduleInfo.endDateTime =
                getDragScheduleInfoEndTime(m_MoveDate);
            m_DragScheduleInfo.beginDateTime =m_InfoBeginTime;
            upDateInfoShow(ChangeEnd,m_DragScheduleInfo);
        }
        break;
    case ChangeWhole: {
        if (!m_PressRect.contains(event->pos())) {
            Qt::DropAction dropaction = m_Drag->exec( Qt::MoveAction);
            Q_UNUSED(dropaction);
            m_Drag = nullptr;
            m_DragStatus = NONE;
            setCursor(Qt::ArrowCursor);
            m_DragScheduleInfo.IsMoveInfo = false;
            setPressSelectInfo(m_DragScheduleInfo);
            emit signalsUpdateShcedule();
        }
    }
    break;
    default:
        break;
    }
    DGraphicsView::mouseMoveEvent(event);
}

void DragInfoGraphicsView::wheelEvent(QWheelEvent *event)
{
    stopTouchAnimation();
    DGraphicsView::wheelEvent(event);
}

void DragInfoGraphicsView::contextMenuEvent(QContextMenuEvent *event)
{
    DGraphicsView::contextMenuEvent(event);

    if (m_DragStatus ==IsCreate) {
        return;
    }
    emit signalScheduleShow(false);
    m_press = false;
    m_DragStatus =NONE;
    QGraphicsItem *listItem = itemAt(event->pos());
    DragInfoItem *infoitem = dynamic_cast<DragInfoItem *>(listItem);

    if (infoitem !=nullptr) {
        if (infoitem->getData().type.ID != DDECalendar::FestivalTypeID) {
            m_rightMenu->clear();
            m_rightMenu->addAction(m_editAction);
            m_rightMenu->addAction(m_deleteAction);
            QAction *action_t = m_rightMenu->exec(QCursor::pos());

            if (action_t == m_editAction) {
                emit signalViewtransparentFrame(1);
                CScheduleDlg dlg(0, this);
                dlg.setData(infoitem->getData());
                if (dlg.exec() == DDialog::Accepted) {
                    emit signalsUpdateShcedule();
                }
                emit signalViewtransparentFrame(0);
            } else if (action_t == m_deleteAction) {
                DeleteItem(infoitem->getData());
            }
        } else {
            emit signalViewtransparentFrame(1);
            CMyScheduleView dlg(infoitem->getData(), this);
            dlg.exec();
            emit signalViewtransparentFrame(0);
        }
    } else {
        RightClickToCreate(listItem,event->pos());
    }
}

void DragInfoGraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("Info")) {
        QJsonParseError json_error;
        QString str = event->mimeData()->data("Info");
        QJsonDocument jsonDoc(QJsonDocument::fromJson(str.toLocal8Bit(), &json_error));

        if (json_error.error != QJsonParseError::NoError) {
            event->ignore();
        }
        QJsonObject rootobj = jsonDoc.object();
        ScheduleDtailInfo info =
            CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->JsonObjectToInfo(rootobj);

        if ((event->source() != this && info.rpeat > 0) || info.type.ID == DDECalendar::FestivalTypeID) {
            event->ignore();
        } else {
            event->accept();
        }
    } else {
        event->ignore();
    }

}

void DragInfoGraphicsView::dragLeaveEvent(QDragLeaveEvent *event)
{
    Q_UNUSED(event);
    upDateInfoShow();
    m_MoveDate = m_MoveDate.addMonths(-2);
}

void DragInfoGraphicsView::dragMoveEvent(QDragMoveEvent *event)
{
    QString str = event->mimeData()->data("Info");
    QDateTime gDate =  getPosDate(event->pos());
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(str.toLocal8Bit(), &json_error));

    if (json_error.error != QJsonParseError::NoError) {
        return;
    }

    if (!IsEqualtime(m_MoveDate,gDate)) {
        m_MoveDate = gDate;
        QJsonObject rootobj = jsonDoc.object();
        m_DragScheduleInfo =
            CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->JsonObjectToInfo(rootobj);

        MoveInfoProcess(m_DragScheduleInfo,event->posF());
        setPressSelectInfo(m_DragScheduleInfo);
    }
}

void DragInfoGraphicsView::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("Info")) {
        if (event->source()!=this || m_MoveDate !=m_PressDate) {
            updateScheduleInfo(m_DragScheduleInfo);
        } else {
            emit signalsUpdateShcedule();
        }
        m_DragStatus = NONE;
        m_MoveDate = m_MoveDate.addMonths(-2);
    }
}

bool DragInfoGraphicsView::event(QEvent *e)
{
    if (e->type() ==QEvent::Leave) {
        if (m_DragStatus ==IsCreate ||
                m_DragStatus == ChangeBegin ||
                m_DragStatus == ChangeEnd)
            mouseReleaseScheduleUpdate();
    }
    return DGraphicsView::event(e);
}

void DragInfoGraphicsView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left || event->key() ==Qt::Key_Right)
        return;
    DGraphicsView::keyPressEvent(event);
}

void DragInfoGraphicsView::slotCreate()
{
    slotCreate(m_createDate);
}

void DragInfoGraphicsView::setPressSelectInfo(const ScheduleDtailInfo &info)
{
    CScheduleDataManage::getScheduleDataManage()->setPressSelectInfo(info);
}

void DragInfoGraphicsView::updateScheduleInfo(const ScheduleDtailInfo &info)
{
    emit signalViewtransparentFrame(1);

    if (info.rpeat >0) {
        CScheduleDlg::ChangeRecurInfo(this, info,
                                      m_PressScheduleInfo, m_themetype);
    } else {
        CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(
            info);
    }
    emit signalViewtransparentFrame(0);
}

void DragInfoGraphicsView::DragPressEvent(const QPoint &pos, DragInfoItem *item)
{
    m_PressPos = pos;
    m_PressDate = getPosDate(pos);
    m_MoveDate = m_PressDate.addMonths(-2);

    if (item != nullptr) {
        PosInItem mpressstatus = getPosInItem(pos,item->boundingRect());
        if (mpressstatus != MIDDLE && item->getData().type.ID == 4) {
            return;
        }
        m_DragScheduleInfo = item->getData();
        m_PressScheduleInfo = item->getData();
        m_InfoBeginTime = m_DragScheduleInfo.beginDateTime;
        m_InfoEndTime = m_DragScheduleInfo.endDateTime;
        switch (mpressstatus) {
        case TOP:
            m_DragStatus = ChangeBegin;
            setCursor(Qt::SplitVCursor);
            break;
        case BOTTOM:
            m_DragStatus = ChangeEnd;
            setCursor(Qt::SplitVCursor);
            break;
        case LEFT:
            m_DragStatus = ChangeBegin;
            setCursor(Qt::SplitHCursor);
            break;
        case RIGHT:
            m_DragStatus = ChangeEnd;
            setCursor(Qt::SplitHCursor);
            break;
        default:
            ShowSchedule(item);
            m_DragStatus = ChangeWhole;
            QMimeData *mimeData = new QMimeData();
            mimeData->setText(m_DragScheduleInfo.titleName);
            mimeData->setData("Info",
                              CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->InfoToJson(m_DragScheduleInfo).toUtf8());

            if (m_Drag ==nullptr) {
                m_Drag = new QDrag(this);
            }
            m_Drag->setMimeData(mimeData);
            QPointF itemPos = QPointF(pos.x()-item->boundingRect().x(),
                                      pos.y()-item->boundingRect().y());
            m_Drag->setHotSpot(itemPos.toPoint());
            setDragPixmap(m_Drag,item);
            break;
        }
    } else {
        m_DragStatus = IsCreate;
        m_isCreate = false;
    }
}

void DragInfoGraphicsView::mouseReleaseScheduleUpdate()
{
    setCursor(Qt::ArrowCursor);
    m_press = false;
    DragInfoItem::setPressFlag(false);
    bool isUpdateInfo = false;

    switch (m_DragStatus) {
    case IsCreate:
        if (MeetCreationConditions(m_MoveDate)) {
            //如果不添加会进入leaveEvent事件内的条件
            m_DragStatus = NONE;

            emit signalViewtransparentFrame(1);
            CScheduleDlg dlg(1, this);
            dlg.setData(m_DragScheduleInfo);
            if (dlg.exec() == DDialog::Accepted) {
            } else {
                setPressSelectInfo(ScheduleDtailInfo());
            }
            emit signalViewtransparentFrame(0);
            isUpdateInfo = true;
        }
        break;
    case ChangeBegin:
        if (!IsEqualtime(m_MoveDate,m_InfoBeginTime)) {
            //如果不添加会进入leaveEvent事件内的条件
            m_DragStatus = NONE;
            updateScheduleInfo(m_DragScheduleInfo);
            isUpdateInfo = true;
        }
        break;
    case ChangeEnd:
        if (!IsEqualtime(m_MoveDate,m_InfoEndTime)) {
            //如果不添加会进入leaveEvent事件内的条件
            m_DragStatus = NONE;
            updateScheduleInfo(m_DragScheduleInfo);
            isUpdateInfo = true;
        }
        break;
    default:
        break;
    }
    m_DragStatus = NONE;

    if (isUpdateInfo) {
        emit signalsUpdateShcedule();
    }
    update();
}

void DragInfoGraphicsView::mousePress(const QPoint &point)
{
    setPressSelectInfo(ScheduleDtailInfo());
    QGraphicsItem *listItem = itemAt(point);
    DragInfoItem *infoitem = dynamic_cast<DragInfoItem *>(listItem);

    if (infoitem != nullptr) {
        setPressSelectInfo(infoitem->getData());
        m_press = true;
        DragInfoItem::setPressFlag(true);
    } else {
        emit signalScheduleShow(false);
    }
    DragPressEvent(point, infoitem);
    update();
}

int DragInfoGraphicsView::getSlidePos() const
{
    return m_touchSlidePos;
}

void DragInfoGraphicsView::setSlidePos(int pos)
{
    m_touchSlidePos = pos;
    verticalScrollBar()->setValue(m_touchSlidePos);
}

void DragInfoGraphicsView::stopTouchAnimation()
{
    m_touchAnimation->stop();
}

void DragInfoGraphicsView::DeleteItem(const ScheduleDtailInfo &info)
{
    emit signalViewtransparentFrame(1);

    if (info.rpeat == 0) {
        CScheduleCtrlDlg msgBox(this);
        msgBox.setText(tr("You are deleting an event."));
        msgBox.setInformativeText(tr("Are you sure you want to delete this event?"));
        msgBox.addPushButton(tr("Cancel"), true);
        msgBox.addWaringButton(tr("Delete"), true);
        msgBox.exec();

        if (msgBox.clickButton() == 0) {
            emit signalViewtransparentFrame(0);
            return;
        } else if (msgBox.clickButton() == 1) {
            CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->deleteScheduleInfoById(info.id);
        }
    } else {
        if (info.RecurID == 0) {
            CScheduleCtrlDlg msgBox(this);
            msgBox.setText(tr("You are deleting an event."));
            msgBox.setInformativeText(tr("Do you want to delete all occurrences of this event, or only the selected occurrence?"));
            msgBox.addPushButton(tr("Cancel"));
            msgBox.addPushButton(tr("Delete All"));
            msgBox.addWaringButton(tr("Delete Only This Event"));
            msgBox.exec();

            if (msgBox.clickButton() == 0) {
                emit signalViewtransparentFrame(0);
                return;
            } else if (msgBox.clickButton() == 1) {
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->deleteScheduleInfoById(info.id);
            } else if (msgBox.clickButton() == 2) {

                ScheduleDtailInfo newschedule;
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(info.id, newschedule);
                newschedule.ignore.append(info.beginDateTime);
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);
            }
        } else {
            CScheduleCtrlDlg msgBox(this);
            msgBox.setText(tr("You are deleting an event."));
            msgBox.setInformativeText(tr("Do you want to delete this and all future occurrences of this event, or only the selected occurrence?"));
            msgBox.addPushButton(tr("Cancel"));
            msgBox.addPushButton(tr("Delete All Future Events"));
            msgBox.addWaringButton(tr("Delete Only This Event"));
            msgBox.exec();

            if (msgBox.clickButton() == 0) {
                emit signalViewtransparentFrame(0);
                return;
            } else if (msgBox.clickButton() == 1) {
                ScheduleDtailInfo newschedule;
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(info.id, newschedule);
                newschedule.enddata.type = 2;
                newschedule.enddata.date = info.beginDateTime.addDays(-1);
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);
            } else if (msgBox.clickButton() == 2) {
                ScheduleDtailInfo newschedule;
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(info.id, newschedule);
                newschedule.ignore.append(info.beginDateTime);
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);
            }
        }
    }
    emit signalsUpdateShcedule();
    emit signalViewtransparentFrame(0);
}

void DragInfoGraphicsView::setDragPixmap(QDrag *drag, DragInfoItem *item)
{
    Q_UNUSED(item);
    //设置一个1*1的透明图片，要不然关闭窗口特效会有一个小黑点
    QPixmap pixmap(1,1);
    pixmap.fill(Qt::transparent);
    drag->setPixmap(pixmap);
    //设置图标位置为鼠标位置
    drag->setHotSpot(QPoint(0,0));
}

void DragInfoGraphicsView::slotCreate(const QDateTime &date)
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
    }
    emit signalViewtransparentFrame(0);
}

ScheduleDtailInfo DragInfoGraphicsView::getScheduleInfo(const QDateTime &beginDate, const QDateTime &endDate)
{
    ScheduleDtailInfo info;

    if (beginDate.daysTo(endDate)>0) {
        info.beginDateTime = QDateTime(beginDate.date(),QTime(0,0,0));
        info.endDateTime = QDateTime(endDate.date(),QTime(23,59,59));
    } else {
        info.beginDateTime = QDateTime(endDate.date(),QTime(0,0,0));
        info.endDateTime = QDateTime(beginDate.date(),QTime(23,59,00));
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

void DragInfoGraphicsView::ShowSchedule(DragInfoItem *infoitem)
{
    if (infoitem ==nullptr)
        return;
    emit signalScheduleShow(true, infoitem->getData());

}

void DragInfoGraphicsView::slideEvent(QPointF &startPoint, QPointF &stopPort)
{
    qreal _movingLine {0};
    //获取滑动方向
    touchGestureOperation::TouchMovingDirection _touchMovingDir =
        touchGestureOperation::getTouchMovingDir(startPoint, stopPort, _movingLine);
    m_touchMovingDir = _touchMovingDir;
    //切换标志 0 不切换  1 下一页  -1 上一页
    int delta {0};
    //移动偏移 25则切换
    const int moveOffset = 25;
    switch (_touchMovingDir) {
    case touchGestureOperation::T_TOP:
    case touchGestureOperation::T_BOTTOM: {
        const int pos_Diff_Y = qFloor(stopPort.y() - startPoint.y());
        verticalScrollBar()->setValue(verticalScrollBar()->sliderPosition() - pos_Diff_Y);
        startPoint = stopPort;
        break;
    }
    case touchGestureOperation::T_LEFT: {
        if (_movingLine > moveOffset) {
            delta = 1;
            startPoint = stopPort;
        }
        break;
    }
    case touchGestureOperation::T_RIGHT: {
        if (_movingLine > moveOffset) {
            delta = -1;
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

int DragInfoGraphicsView::getDragStatus() const
{
    return m_DragStatus;
}

