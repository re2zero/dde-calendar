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
#include "graphicsview.h"
#include <QEvent>
#include <QMouseEvent>
#include <QScrollBar>

#include "scheduleitem.h"
#include "schedulecoormanage.h"
#include "schceduledlg.h"
#include "dbmanager.h"
#include <QMenu>
//m_graphicsScene->setSceneRect(0,0,763,1032);
#include "scheduledatamanage.h"
#include <DMessageBox>
#include <DPushButton>
#include <DHiDPIHelper>
#include <DPalette>
#include "schcedulectrldlg.h"
#include "myschceduleview.h"
#include <QShortcut>
DGUI_USE_NAMESPACE
CGraphicsView::CGraphicsView(QWidget *parent)
    : DGraphicsView(parent)
{
    setContentsMargins(0, 0, 0, 0);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_coorManage = new CScheduleCoorManage;

    m_LRPen.setColor(QColor(255, 255, 255));
    m_LRPen.setStyle(Qt::SolidLine);
    m_TBPen.setColor(QColor(255, 255, 255));
    m_TBPen.setStyle(Qt::SolidLine);
    m_LRFlag = true;
    m_TBFlag = true;
    m_margins = QMargins(0, 0, 0, 0);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_graphicsScene = new QGraphicsScene(this);
    m_graphicsScene->setSceneRect(0, 0, width(), height());
    setScene(m_graphicsScene);

    m_dayInterval = width();
    m_timeInterval = height() / 24.0;

    int viewWidth = viewport()->width();
    int viewHeight = viewport()->height();
    // view 根据鼠标下的点作为锚点来定位 scene
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    QPoint newCenter(viewWidth / 2,  viewHeight / 2 - 1000);
    centerOn(mapToScene(newCenter));

    // scene 在 view 的中心点作为锚点
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    scrollBarValueChangedSlot();

    m_editAction = new QAction(tr("Edit"), this);
    m_deleteAction = new QAction(tr("Delete"), this);
    m_createAction = new QAction(tr("Create"), this);
    setMouseTracking(true);
    viewport()->setMouseTracking(true);

    setLineWidth(0);
}

CGraphicsView::~CGraphicsView()
{
    clearSchdule();
}

void CGraphicsView::setMargins(int left, int top, int right, int bottom)
{
    m_margins = QMargins(left, top, right, bottom);
    setViewportMargins(m_margins);
}

void CGraphicsView::setTheMe(int type)
{
    if (type == 0 || type == 1) {
        m_weekcolor = "#00429A";
        m_weekcolor.setAlphaF(0.05);
        QColor linecolor = "#000000";
        linecolor.setAlphaF(0.1);
        m_LRPen.setColor(linecolor);
        m_TBPen.setColor(linecolor);
        m_LRPen.setStyle(Qt::SolidLine);
        m_TBPen.setStyle(Qt::SolidLine);

    } else if (type == 2) {
        m_weekcolor = "#4F9BFF";
        m_weekcolor.setAlphaF(0.1);
        QColor linecolor = "#000000";
        linecolor.setAlphaF(0.1);
        m_LRPen.setColor(linecolor);
        m_TBPen.setColor(linecolor);
    }
    scene()->update();
    update();
}

void CGraphicsView::setRange( int w, int h, QDate begindate, QDate enddate )
{
    m_graphicsScene->setSceneRect(0, 0, w, h);
    m_coorManage->setRange(w, h, begindate, enddate);
    int totalDay = begindate.daysTo(enddate) + 1;
    m_dayInterval = w * 1.0 / totalDay;
    m_timeInterval = h / 24.0;
    m_totalDay = totalDay;
    int viewWidth = viewport()->width();
    int viewHeight = viewport()->height();
    // view 根据鼠标下的点作为锚点来定位 scene
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    QPoint newCenter(viewWidth / 2,  viewHeight / 2 - 2000);
    centerOn(mapToScene(newCenter));

    // scene 在 view 的中心点作为锚点
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    scrollBarValueChangedSlot();
}

void CGraphicsView::addSchduleItem( const ScheduleDtailInfo &info, QDate date, int index, int totalNum, int type, int viewtype, int maxnum)
{
    m_currentItem = NULL;
    CScheduleItem *item = new CScheduleItem(m_coorManage, 0, m_graphicsScene, type);
    item->setData(info, date, index, totalNum, viewtype, maxnum);
    m_vScheduleItem.append(item);

}

void CGraphicsView::deleteSchduleItem( CScheduleItem *item )
{
    int id = item->getData().id;
    for (int i = 0; i < m_vScheduleItem.size(); i++) {
        if (m_vScheduleItem[i]->getData().id == id) {
            m_vScheduleItem.remove(i);
            m_graphicsScene->removeItem(item);
            delete m_vScheduleItem[i];
            m_vScheduleItem[i] = NULL;
            i--;
        }
    }
}

void CGraphicsView::clearSchdule()
{
    for (int i = 0; i < m_vScheduleItem.size(); i++) {
        m_graphicsScene->removeItem(m_vScheduleItem.at(i));
        delete m_vScheduleItem[i];
        m_vScheduleItem[i] = NULL;
    }
    m_vScheduleItem.clear();
}

/************************************************************************
Function:       onViewEvent()
Description:    执行父类事件
Input:          event 事件
Output:         无
Return:         无
Others:         无
************************************************************************/
void CGraphicsView::onViewEvent( QEvent *event )
{
    //防止为空
    if (NULL == event)
        return;
    switch (event->type()) {
    case QEvent::MouseButtonPress: {
        QMouseEvent *t_event = dynamic_cast<QMouseEvent *>(event);
        if (NULL != t_event)
            QGraphicsView::mousePressEvent(t_event);
    }
    break;
    case QEvent::MouseMove: {
        QMouseEvent *t_event = dynamic_cast<QMouseEvent *>(event);
        if (NULL != t_event)
            QGraphicsView::mouseMoveEvent(t_event);
    }
    break;
    case QEvent::MouseButtonRelease: {
        QMouseEvent *t_event = dynamic_cast<QMouseEvent *>(event);
        if (NULL != t_event)
            QGraphicsView::mouseReleaseEvent(t_event);
    }
    break;
    case QEvent::MouseButtonDblClick: {
        QMouseEvent *t_event = dynamic_cast<QMouseEvent *>(event);
        if (NULL != t_event)
            QGraphicsView::mouseDoubleClickEvent(t_event);
    }
    break;
    case QEvent::Wheel: {
        QWheelEvent *t_event = dynamic_cast<QWheelEvent *>(event);
        if (NULL != t_event)
            QGraphicsView::wheelEvent(t_event);
    }
    break;
    case QEvent::KeyPress: {
        QKeyEvent *t_event = dynamic_cast<QKeyEvent *>(event);
        if (NULL != t_event)
            QGraphicsView::keyPressEvent(t_event);
    }
    break;
    case QEvent::KeyRelease: {
        QKeyEvent *t_event = dynamic_cast<QKeyEvent *>(event);
        if (NULL != t_event)
            QGraphicsView::keyReleaseEvent(t_event);
    }
    break;
    case QEvent::FocusIn: {
        QFocusEvent *t_event = dynamic_cast<QFocusEvent *>(event);
        if (NULL != t_event)
            QGraphicsView::focusInEvent(t_event);
    }
    break;
    case QEvent::FocusOut: {
        QFocusEvent *t_event = dynamic_cast<QFocusEvent *>(event);
        if (NULL != t_event)
            QGraphicsView::focusOutEvent(t_event);
    }
    break;
    case QEvent::Enter: {
        QGraphicsView::enterEvent(event);
    }
    break;
    case QEvent::Leave: {
        QGraphicsView::leaveEvent(event);
    }
    break;
    case QEvent::ContextMenu: {
        QContextMenuEvent *t_event = dynamic_cast<QContextMenuEvent *>(event);
        if (NULL != t_event)
            QGraphicsView::contextMenuEvent(t_event);
    }
    break;
    default:
        break;
    }
}

/************************************************************************
Function:       mousePressEvent()
Description:    鼠标按下事件
Input:          event 鼠标事件
Output:         无
Return:         无
Others:         无
************************************************************************/
void CGraphicsView::mousePressEvent( QMouseEvent *event )
{
    int themetype = CScheduleDataManage::getScheduleDataManage()->getTheme();

    if (event->button() == Qt::RightButton) {
        CScheduleItem *item = dynamic_cast<CScheduleItem *>(itemAt(event->pos()));
        if (item == NULL) {
            DMenu menu(this);
            menu.addAction(m_createAction);
            QAction *action_t = menu.exec(QCursor::pos());
            if (action_t == m_createAction) {
                QPointF senceposs = mapToScene(event->pos());
                CSchceduleDlg dlg(1, this);
                QDateTime tDatatime = m_coorManage->getDate(senceposs);
                dlg.setDate(tDatatime);
                if (dlg.exec() == DDialog::Accepted) {
                    emit signalsUpdateShcedule(0);
                }
            }
        } else {
            if (item->getType() == 1) return;
            DMenu menu(this);
            menu.addAction(m_editAction);
            menu.addAction(m_deleteAction);
            QAction *action_t = menu.exec(QCursor::pos());
            if (action_t == m_editAction) {
                CSchceduleDlg dlg(0, this);
                dlg.setData(item->getData());
                if (dlg.exec() == DDialog::Accepted) {
                    emit signalsUpdateShcedule(0);
                }
            } else if (action_t == m_deleteAction) {
                ScheduleDtailInfo info = item->getData();
                if (info.rpeat == 0) {
                    CSchceduleCtrlDlg msgBox(this);
                    msgBox.setText(tr("You are deleted schedule"));
                    msgBox.setInformativeText(tr("Are you sure you want to delete this schedule?"));
                    DPushButton *noButton = msgBox.addPushButton(tr("Cancel"));
                    DPushButton *yesButton = msgBox.addPushButton(tr("Delete Schedule"), 1);
                    msgBox.updatesize();
                    DPalette pa = yesButton->palette();
                    if (themetype == 0 || themetype == 1) {
                        pa.setColor(DPalette::ButtonText, Qt::red);

                    } else {
                        pa.setColor(DPalette::ButtonText, "#FF5736");

                    }
                    yesButton->setPalette(pa);
                    msgBox.exec();

                    if (msgBox.clickButton() == noButton) {
                        return;
                    } else if (msgBox.clickButton() == yesButton) {
                        CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->deleteScheduleInfoById(info.id);
                    }
                } else {
                    if (info.RecurID == 0) {
                        CSchceduleCtrlDlg msgBox(this);
                        msgBox.setText(tr("You are deleted schedule"));
                        msgBox.setInformativeText(tr("You want to delete all repeat of the schedule, or just delete the selected repeat?"));
                        DPushButton *noButton = msgBox.addPushButton(tr("Cancel"));
                        DPushButton *yesallbutton = msgBox.addPushButton(tr("All Deleted"));
                        DPushButton *yesButton = msgBox.addPushButton(tr("Just Delete Schedule"));
                        msgBox.updatesize();
                        DPalette pa = yesButton->palette();
                        if (themetype == 0 || themetype == 1) {
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
                        msgBox.setText(tr("You are deleted schedule"));
                        msgBox.setInformativeText(tr("You want to delete the schedule of this repetition and all repeat in the future, or just delete all repeat?"));
                        DPushButton *noButton = msgBox.addPushButton(tr("Cancel"));
                        DPushButton *yesallbutton = msgBox.addPushButton(tr("Delete all schedule in the future"));
                        DPushButton *yesButton = msgBox.addPushButton(tr("Just Delete Schedule"));
                        msgBox.updatesize();
                        DPalette pa = yesButton->palette();
                        if (themetype == 0 || themetype == 1) {
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
                            return;
                        } else if (msgBox.clickButton() == yesallbutton) {
                            ScheduleDtailInfo newschedule;
                            CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(info.id, newschedule);
                            newschedule.enddata.type = 2;
                            newschedule.enddata.date = info.beginDateTime;
                            CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);

                        } else if (msgBox.clickButton() == yesButton) {

                            ScheduleDtailInfo newschedule;
                            CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(info.id, newschedule);
                            newschedule.ignore.append(info.beginDateTime);
                            CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);
                        }
                    }
                }
                emit signalsUpdateShcedule(item->getData().id);
            }
        }
    } else if (event->button() == Qt::LeftButton) {
        CScheduleItem *item = dynamic_cast<CScheduleItem *>(itemAt(event->pos()));
        if (item != NULL) {
            m_currentItem = item;
            emit signalsitem(this);
        }
    }
    DGraphicsView::mousePressEvent(event);
}

void CGraphicsView::mouseReleaseEvent( QMouseEvent *event )
{
    DGraphicsView::mouseReleaseEvent(event);
}


void CGraphicsView::mouseDoubleClickEvent( QMouseEvent *event )
{
    DGraphicsView::mouseDoubleClickEvent(event);
    CScheduleItem *item = dynamic_cast<CScheduleItem *>(itemAt(event->pos()));
    if (item == NULL) {
        return;
    }
    if (item->getType() == 1) return;
    CMySchceduleView dlg(this);
    dlg.setSchedules(item->getData());
    connect(&dlg, &CMySchceduleView::signalsEditorDelete, this, &CGraphicsView::slotDoubleEvent);
    dlg.exec();
    disconnect(&dlg, &CMySchceduleView::signalsEditorDelete, this, &CGraphicsView::slotDoubleEvent);
}
void CGraphicsView::slotDoubleEvent(int type)
{
    emit signalsUpdateShcedule(0);
}

void CGraphicsView::slotDeleteItem()
{
    int themetype = CScheduleDataManage::getScheduleDataManage()->getTheme();
    if (m_currentItem == NULL) return;

    ScheduleDtailInfo info = m_currentItem->getData();
    if (info.rpeat == 0) {
        CSchceduleCtrlDlg msgBox(this);
        msgBox.setText(tr("You are deleted schedule"));
        msgBox.setInformativeText(tr("Are you sure you want to delete this schedule?"));
        DPushButton *noButton = msgBox.addPushButton(tr("Cancel"));
        DPushButton *yesButton = msgBox.addPushButton(tr("Delete Schedule"));
        msgBox.updatesize();
        DPalette pa = yesButton->palette();
        if (themetype == 0 || themetype == 1) {
            pa.setColor(DPalette::ButtonText, Qt::red);

        } else {
            pa.setColor(DPalette::ButtonText, "#FF5736");

        }
        yesButton->setPalette(pa);
        msgBox.exec();

        if (msgBox.clickButton() == noButton) {
            return;
        } else if (msgBox.clickButton() == yesButton) {
            CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->deleteScheduleInfoById(info.id);
        }
    } else {
        if (info.RecurID == 0) {
            CSchceduleCtrlDlg msgBox(this);
            msgBox.setText(tr("You are deleted schedule"));
            msgBox.setInformativeText(tr("You want to delete all repeat of the schedule, or just delete the selected repeat?"));
            DPushButton *noButton = msgBox.addPushButton(tr("Cancel"));
            DPushButton *yesallbutton = msgBox.addPushButton(tr("All Deleted"));
            DPushButton *yesButton = msgBox.addPushButton(tr("Just Delete Schedule"));
            msgBox.updatesize();
            DPalette pa = yesButton->palette();
            if (themetype == 0 || themetype == 1) {
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
            msgBox.setText(tr("You are deleted schedule"));
            msgBox.setInformativeText(tr("You want to delete the schedule of this repetition and all repeat in the future, or just delete all repeat?"));
            DPushButton *noButton = msgBox.addPushButton(tr("Cancel"));
            DPushButton *yesallbutton = msgBox.addPushButton(tr("Delete all schedule in the future"));
            DPushButton *yesButton = msgBox.addPushButton(tr("Just Delete Schedule"));
            msgBox.updatesize();
            DPalette pa = yesButton->palette();
            if (themetype == 0 || themetype == 1) {
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
                return;
            } else if (msgBox.clickButton() == yesallbutton) {
                ScheduleDtailInfo newschedule;
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(info.id, newschedule);
                newschedule.enddata.type = 2;
                newschedule.enddata.date = info.beginDateTime;
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);

            } else if (msgBox.clickButton() == yesButton) {

                ScheduleDtailInfo newschedule;
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(info.id, newschedule);
                newschedule.ignore.append(info.beginDateTime);
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);
            }
        }
    }
    emit signalsUpdateShcedule(m_currentItem->getData().id);
}
void CGraphicsView::mouseMoveEvent( QMouseEvent *event )
{
    DGraphicsView::mouseMoveEvent(event);
}

#ifndef QT_NO_WHEELEVENT
/************************************************************************
Function:       wheelEvent()
Description:    鼠标滚轮事件
Input:          event 滚轮事件
Output:         无
Return:         无
Others:         无
************************************************************************/
void CGraphicsView::wheelEvent( QWheelEvent *event )
{
    int test = event -> delta();
    int viewWidth = viewport()->width();
    int viewHeight = viewport()->height();
    // view 根据鼠标下的点作为锚点来定位 scene
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    QPoint newCenter(viewWidth / 2,  viewHeight / 2 - test);
    QPointF centerpos = mapToScene(newCenter);
    centerOn(centerpos.x(), centerpos.y() + 2);

    // scene 在 view 的中心点作为锚点
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);

    scrollBarValueChangedSlot();
    DGraphicsView::wheelEvent(event);
}
#endif

/************************************************************************
Function:       keyPressEvent()
Description:    键盘按下事件
Input:          event 键盘事件
Output:         无
Return:         无
Others:         无
************************************************************************/
void CGraphicsView::keyPressEvent( QKeyEvent *event )
{
    DGraphicsView::keyPressEvent(event);
}

/************************************************************************
Function:       keyReleaseEvent()
Description:    键盘释放事件
Input:          event 键盘事件
Output:         无
Return:         无
Others:         无
************************************************************************/
void CGraphicsView::keyReleaseEvent( QKeyEvent *event )
{
    DGraphicsView::keyReleaseEvent(event);
}

/************************************************************************
Function:       focusInEvent()
Description:    焦点进入事件
Input:          event 焦点事件
Output:         无
Return:         无
Others:         无
************************************************************************/
void CGraphicsView::focusInEvent( QFocusEvent *event )
{
    DGraphicsView::focusInEvent(event);
}

/************************************************************************
Function:       focusOutEvent()
Description:    焦点离开事件
Input:          event 焦点事件
Output:         无
Return:         无
Others:         无
************************************************************************/
void CGraphicsView::focusOutEvent( QFocusEvent *event )
{
    DGraphicsView::focusOutEvent(event);
}

/************************************************************************
Function:       enterEvent()
Description:    进入事件
Input:          event 事件
Output:         无
Return:         无
Others:         无
************************************************************************/
void CGraphicsView::enterEvent( QEvent *event )
{
    DGraphicsView::enterEvent(event);
}

/************************************************************************
Function:       leaveEvent()
Description:    离开事件
Input:          event 事件
Output:         无
Return:         无
Others:         无
************************************************************************/
void CGraphicsView::leaveEvent( QEvent *event )
{
    DGraphicsView::leaveEvent(event);
}

/************************************************************************
Function:       contextMenuEvent()
Description:    右键菜单事件
Input:          event 右键菜单事件
Output:         无
Return:         无
Others:         无
************************************************************************/
void CGraphicsView::contextMenuEvent( QContextMenuEvent *event )
{
    DGraphicsView::contextMenuEvent(event);
}

/************************************************************************
Function:       resizeEvent()
Description:    窗口大小改变事件
Input:          event 窗口大小改变事件
Output:         无
Return:         无
Others:         无
************************************************************************/
void CGraphicsView::resizeEvent( QResizeEvent *event )
{
    scrollBarValueChangedSlot();
    QGraphicsView::resizeEvent(event);
}
/************************************************************************
Function:       setLargeScale()
Description:    设置大刻度
Input:          vLRLarge 左右刻度，vTBLarge 上下刻度
Output:         无
Return:         无
Others:         无
************************************************************************/
void CGraphicsView::setLargeScale(const QVector<int> &vLRLarge, const QVector<int> &vTBLarge)
{
    m_vLRLarge = vLRLarge;
    m_vTBLarge = vTBLarge;
    update();
}

/************************************************************************
Function:       paintEvent()
Description:    绘制事件
Input:          event 事件
Output:         无
Return:         无
Others:         无
************************************************************************/
void CGraphicsView::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);

    QPainter t_painter(viewport());
    //t_painter.setCompositionMode(QPainter::CompositionMode_Difference  ); //设置混合模式
    int t_width = viewport()->width();
    int t_height = viewport()->height();
    //绘制垂直线
    if (m_TBFlag) {
        t_painter.save();
        t_painter.setPen(m_TBPen);
        for (int i = 0; i < m_vTBLarge.size(); ++i)
            t_painter.drawLine(QPoint(m_vTBLarge[i] - 1, 0), QPoint(m_vTBLarge[i] - 1, t_height));
        t_painter.restore();
        if (m_totalDay == 7) {
            t_painter.save();
            for (int i = 0; i != 7; ++i) {

                int d = checkDay(i - m_firstWeekDay);

                t_painter.setBrush(QBrush(m_weekcolor));
                t_painter.setPen(Qt::NoPen);
                if (d == 6 || d == 7) {
                    t_painter.drawRect(QRect(0 + i * m_dayInterval, 0, m_dayInterval, t_height));
                }
            }
            t_painter.restore();
        }
    }
    //绘制水平线
    if (m_LRFlag) {
        t_painter.save();
        t_painter.setPen(m_LRPen);
        for (int i = 0; i < m_vLRLarge.size(); ++i)
            t_painter.drawLine(QPoint(0, m_vLRLarge[i] - 1), QPoint(t_width, m_vLRLarge[i] - 1));
        t_painter.restore();
    }
}

void CGraphicsView::scrollBarValueChangedSlot()
{
    int viewWidth = viewport()->width();
    int viewHeight = viewport()->height();
    m_vLRLarge.clear();
    m_vTBLarge.clear();
    QPointF leftToprealPos = mapToScene(QPoint(0, 0));
    QPointF leftBttomrealPos = mapToScene(QPoint(0, viewHeight));
    QPointF rightToprealPos = mapToScene(QPoint(viewWidth, 0));

    for (float i = m_dayInterval; i < scene()->width(); i = i + m_dayInterval) {
        m_vTBLarge.append(i);
    }
    float beginpos = (int)(leftToprealPos.y() / m_timeInterval) * m_timeInterval;
    if (beginpos < leftToprealPos.y()) {
        beginpos = (beginpos / m_timeInterval + 1) * m_timeInterval ;
    }
    QVector<int> vHours;
    for (float i = beginpos; i < leftBttomrealPos.y(); i = i + m_timeInterval) {
        QPoint point = mapFromScene(leftBttomrealPos.x(), i);
        m_vLRLarge.append(point.y());
        vHours.append(i / m_timeInterval + 0.5);
    }
    emit signalsPosHours(m_vLRLarge, vHours);
    scene()->update();
}

int CGraphicsView::checkDay(int weekday)
{
    if (weekday <= 0)
        return weekday += 7;

    if (weekday > 7)
        return weekday -= 7;

    return weekday;
}

/************************************************************************
Function:       setLargeScaleInfo()
Description:    设置大刻度显示
Input:          LRFlag 水平刻度，TBFlag 垂直刻度
Output:         无
Return:         无
Others:         无
************************************************************************/
void CGraphicsView::setLargeScaleFlag(const bool &LRFlag, const bool &TBFlag)
{
    m_LRFlag = LRFlag;
    m_TBFlag = TBFlag;
    scene()->update();
}

void CGraphicsView::getLargeScaleFlag(bool &LRFlag, bool &TBFlag)
{
    LRFlag = m_LRFlag;
    TBFlag = m_TBFlag;
}

void CGraphicsView::setLargeScalePen(const QPen &LRPen, const QPen &TBPen)
{
    m_LRPen = LRPen;
    m_TBPen = TBPen;
    update();
}

void CGraphicsView::setFirstWeekday(int weekday)
{
    m_firstWeekDay = weekday;
}

void CGraphicsView::setTime(QTime time)
{
    int viewWidth = viewport()->width();
    int viewHeight = viewport()->height();
    // view 根据鼠标下的点作为锚点来定位 scene
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    QPoint newCenter(viewWidth / 2,  viewHeight / 2);
    QPointF centerpos = mapToScene(newCenter);
    centerpos = QPointF(centerpos.x(), m_coorManage->getHeight(time));
    centerOn(centerpos.x(), centerpos.y());

    // scene 在 view 的中心点作为锚点
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);

    scrollBarValueChangedSlot();
}
