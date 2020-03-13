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
#include <QMenu>
#include <QListWidget>
#include <QLabel>
#include <QPainter>
#include <QHBoxLayout>
#include <QStylePainter>
#include <QRect>
#include "schceduledlg.h"
#include "myschceduleview.h"
#include "scheduledatamanage.h"
#include <DMessageBox>
#include <DPushButton>
#include <DHiDPIHelper>
#include <DPalette>
#include <QGraphicsOpacityEffect>
#include "schedulecoormanage.h"
#include "schcedulectrldlg.h"

DGUI_USE_NAMESPACE
CAllDayEventWidgetItem::CAllDayEventWidgetItem( QWidget *parent /*= nullptr*/, int edittype): DPushButton(parent)
{
    m_editType = edittype;
    QFont font("PingFangSC-Light");
    font.setPixelSize(12);
    m_font = font;
    //setMargin(0);
    m_editAction = new QAction(tr("Edit"), this);
    m_deleteAction = new QAction(tr("Delete"), this);
    connect(m_editAction, SIGNAL(triggered(bool)), this, SLOT(slotEdit()));
    connect(m_deleteAction, SIGNAL(triggered(bool)), this, SLOT(slotDelete()));
    m_createAction = new QAction(tr("New event"), this);
    connect(m_createAction, &QAction::triggered, this, &CAllDayEventWidgetItem::slotCreate);
    m_item = nullptr;
    setMouseTracking(true);
    //setAttribute(Qt::WA_TransparentForMouseEvents);
}

void CAllDayEventWidgetItem::setData(QVector<ScheduleDtailInfo> &vScheduleInfo)
{
    m_vScheduleInfo = vScheduleInfo;
    m_vSelectflag.clear();
    m_vHoverflag.clear();
    m_vHighflag.clear();
    m_vSelectflag.resize(m_vScheduleInfo.count());
    m_vHoverflag.resize(m_vScheduleInfo.count());
    m_vHighflag.resize(m_vScheduleInfo.count());
    m_vSelectflag.fill(false);
    m_vHoverflag.fill(false);
    m_vHighflag.fill(false);
    update();
}

int CAllDayEventWidgetItem::getEventByPos(QPoint pos)
{
    int tindex = -1;
    for (int i = 0; i  < m_vScheduleInfo.count(); i++) {
        QRect drawrect = m_coorManage->getAllDayDrawRegion(m_vScheduleInfo.at(i).beginDateTime.date(), m_vScheduleInfo.at(i).endDateTime.date());
        if (drawrect.contains(pos)) {
            tindex = i;
            break;
        }
    }
    return tindex;
}

void CAllDayEventWidgetItem::slotCreate()
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
        emit signalsEdit(this, 1);
    }
    emit signalViewtransparentFrame(0);
}

void CAllDayEventWidgetItem::slotEdit()
{
    if (m_currentIndex == -1) return;
    emit signalViewtransparentFrame(1);
    CSchceduleDlg dlg(0, this);
    dlg.setData(m_vScheduleInfo[m_currentIndex]);
    if (dlg.exec() == DDialog::Accepted) {

        emit signalsEdit(this, 1);
    }
    emit signalViewtransparentFrame(0);
}

void CAllDayEventWidgetItem::slotDelete()
{
    if (m_currentIndex == -1) return;
    emit signalViewtransparentFrame(1);
    int themetype = CScheduleDataManage::getScheduleDataManage()->getTheme();
    ScheduleDtailInfo tinfo = m_vScheduleInfo[m_currentIndex];
    if (tinfo.rpeat == 0) {
        CSchceduleCtrlDlg msgBox(this);
        //msgBox.setWindowFlags(Qt::FramelessWindowHint);
        // msgBox.setIconPixmap(DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-logo.svg").scaled(QSize(34, 34) * devicePixelRatioF()));

        msgBox.setText(tr("You are deleting an event."));
        msgBox.setInformativeText(tr("Are you sure you want to delete this event?"));
        DPushButton *noButton = msgBox.addPushButton(tr("Cancel"));
        DPushButton *yesButton = msgBox.addPushButton(tr("Delete"), 1);
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
            emit signalViewtransparentFrame(0);
            return;
        } else if (msgBox.clickButton() == yesButton) {
            CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->deleteScheduleInfoById(tinfo.id);
        }
    } else {
        if (tinfo.RecurID == 0) {
            CSchceduleCtrlDlg msgBox(this);
            //msgBox.setWindowFlags(Qt::FramelessWindowHint);
            //msgBox.setIconPixmap(DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-logo.svg").scaled(QSize(34, 34) * devicePixelRatioF()));

            msgBox.setText(tr("You are deleting an event."));
            msgBox.setInformativeText(tr("Do you want to delete all occurrences of this event, or only the selected occurrence?"));
            DPushButton *noButton = msgBox.addPushButton(tr("Cancel"));
            DPushButton *yesallbutton = msgBox.addPushButton(tr("Delete All"));
            DPushButton *yesButton = msgBox.addPushButton(tr("Delete Only This Event"));
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
                emit signalViewtransparentFrame(0);
                return;
            } else if (msgBox.clickButton() == yesallbutton) {
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->deleteScheduleInfoById(tinfo.id);
            } else if (msgBox.clickButton() == yesButton) {

                ScheduleDtailInfo newschedule;
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(tinfo.id, newschedule);
                newschedule.ignore.append(tinfo.beginDateTime);
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);
            }
        } else {
            CSchceduleCtrlDlg msgBox(this);
            //msgBox.setWindowFlags(Qt::FramelessWindowHint);
            //msgBox.setIconPixmap(DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-logo.svg").scaled(QSize(34, 34) * devicePixelRatioF()));
            msgBox.setText(tr("You are deleting an event."));
            msgBox.setInformativeText(tr("Do you want to delete this and all future occurrences of this event, or only the selected occurrence?"));
            DPushButton *noButton = msgBox.addPushButton(tr("Cancel"));
            DPushButton *yesallbutton = msgBox.addPushButton(tr("Delete All Future Events"));
            DPushButton *yesButton = msgBox.addPushButton(tr("Delete Only This Event"));
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
                emit signalViewtransparentFrame(0);
                return;
            } else if (msgBox.clickButton() == yesallbutton) {
                ScheduleDtailInfo newschedule;
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(tinfo.id, newschedule);
                newschedule.enddata.type = 2;
                newschedule.enddata.date = tinfo.beginDateTime.addDays(-1);
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);

            } else if (msgBox.clickButton() == yesButton) {

                ScheduleDtailInfo newschedule;
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(tinfo.id, newschedule);
                newschedule.ignore.append(tinfo.beginDateTime);
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);
            }
        }
    }
    emit signalViewtransparentFrame(0);
    emit signalsDelete(this);
}
//有问题
void CAllDayEventWidgetItem::slotDoubleEvent(int type)
{
    emit signalsEdit(this, 1);
}

void CAllDayEventWidgetItem::paintEvent( QPaintEvent *e )
{
    for (int i = 0; i  < m_vScheduleInfo.count(); i++) {
        paintItem(i);
    }
}
void CAllDayEventWidgetItem::contextMenuEvent( QContextMenuEvent *event )
{
    emit signalScheduleShow(false);
    m_currentIndex = getEventByPos(event->pos());
    if (m_currentIndex != -1) {
        DMenu Context(this);
        Context.addAction(m_editAction);
        Context.addAction(m_deleteAction);
        Context.exec(QCursor::pos());
    } else {
        DMenu Context(this);
        Context.addAction(m_createAction);
        m_dianjiDay = m_coorManage->getsDate(mapFrom(this, event->pos()));
        Context.exec(QCursor::pos());
    }
}

void CAllDayEventWidgetItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit signalScheduleShow(false);
    if (m_editType == 0) return;
    m_currentIndex = getEventByPos(event->pos());
    if (m_currentIndex != -1) {
        emit signalViewtransparentFrame(1);
        CMySchceduleView dlg(this);
        dlg.setSchedules(m_vScheduleInfo[m_currentIndex]);
        connect(&dlg, &CMySchceduleView::signalsEditorDelete, this, &CAllDayEventWidgetItem::slotDoubleEvent);
        dlg.exec();
        emit signalViewtransparentFrame(0);
        disconnect(&dlg, &CMySchceduleView::signalsEditorDelete, this, &CAllDayEventWidgetItem::slotDoubleEvent);
    } else {
        m_dianjiDay = m_coorManage->getsDate(mapFrom(this, event->pos()));
        emit signalViewtransparentFrame(1);
        CSchceduleDlg dlg(1, this);
        QDateTime tDatatime;
        tDatatime.setDate(m_dianjiDay);
        tDatatime.setTime(QTime::currentTime());
        dlg.setDate(tDatatime);
        dlg.setAllDay(true);
        if (dlg.exec() == DDialog::Accepted) {
            emit signalsEdit(this, 1);
        }
        emit signalViewtransparentFrame(0);
    }

}

void CAllDayEventWidgetItem::mousePressEvent(QMouseEvent *event)
{
    if (m_currentIndex == -1) {
        emit signalScheduleShow(false);
        return;
    }

    m_currentIndex = getEventByPos(event->pos());
    QRect drawrect = m_coorManage->getAllDayDrawRegion(m_vScheduleInfo[m_currentIndex].beginDateTime.date(), m_vScheduleInfo[m_currentIndex].endDateTime.date());
    if (drawrect.contains(event->pos())) {

        if (event->button() == Qt::LeftButton) {
            m_vSelectflag[m_currentIndex] = true;
            update();
            emit signalsPress(this);
            m_pressMove = true;
            emit signalScheduleShow(true, m_vScheduleInfo.at(m_currentIndex).id);
        }
    }
}

void CAllDayEventWidgetItem::mouseMoveEvent(QMouseEvent *event)
{
    m_vHoverflag.fill(false);
    m_currentIndex = getEventByPos(event->pos());
//    qDebug() << m_currentIndex;
    if (m_currentIndex == -1) return;
    if (m_pressMove) {
        emit signalScheduleShow(false);
        m_pressMove = false;
    }

    m_vHoverflag[m_currentIndex] = true;
    update();
}

void CAllDayEventWidgetItem::mouseReleaseEvent(QMouseEvent *event)
{
    m_pressMove = false;
    m_currentIndex = getEventByPos(event->pos());
    if (m_currentIndex == -1) return;
    QRect drawrect = m_coorManage->getAllDayDrawRegion(m_vScheduleInfo[m_currentIndex].beginDateTime.date(), m_vScheduleInfo[m_currentIndex].endDateTime.date());
    if (drawrect.contains(event->pos())) {
        if (event->button() == Qt::LeftButton) {
            m_vSelectflag[m_currentIndex] = false;
            update();
        }
    }
}

void CAllDayEventWidgetItem::focusOutEvent(QFocusEvent *event)
{
    //m_highflag = false;
    update();
}

//void CAllDayEventWidgetItem::enterEvent(QEvent *event)
//{
//    m_vHoverflag.fill(false);
//    m_currentIndex = getEventByPos(mapFromGlobal(QCursor::pos()));
//    if (m_currentIndex == -1) return;
//    m_vHoverflag[m_currentIndex] = true;
//}

void CAllDayEventWidgetItem::leaveEvent(QEvent *event)
{
    m_vHoverflag.fill(false);
    update();
}


void CAllDayEventWidgetItem::paintItem(int index)
{
    CSchedulesColor gdcolor = CScheduleDataManage::getScheduleDataManage()->getScheduleColorByType(m_vScheduleInfo[index].type.ID);
    m_vHighflag[index] = CScheduleDataManage::getScheduleDataManage()->getSearchResult(m_vScheduleInfo[index]);
    int themetype = CScheduleDataManage::getScheduleDataManage()->getTheme();

    QRect drawrect = m_coorManage->getAllDayDrawRegion(m_vScheduleInfo[index].beginDateTime.date(), m_vScheduleInfo[index].endDateTime.date());
    QPainter painter(this);


    QLinearGradient linearGradient(drawrect.topLeft().x(), 0, drawrect.topRight().x(), 0);

    QColor color1 = gdcolor.gradientFromC;
    QColor color2 = gdcolor.gradientToC;
    QColor textcolor = gdcolor.textColor;
    if (m_vHoverflag[index]) {
        color1 = gdcolor.hovergradientFromC;
        color2 = gdcolor.hovergradientToC;
    } else if (m_vHighflag[index]) {
        color1 = gdcolor.hightlightgradientFromC;
        color2 = gdcolor.hightlightgradientToC;
    }
    if (m_vSelectflag[index]) {
        color1 = gdcolor.pressgradientFromC;
        color2 = gdcolor.pressgradientToC;
        textcolor.setAlphaF(0.6);
    }
    linearGradient.setColorAt(0, color1);
    linearGradient.setColorAt(1, color2);
    QRect fillRect = drawrect;
    //将直线开始点设为0，终点设为1，然后分段设置颜色
    painter.setRenderHints(QPainter::HighQualityAntialiasing);
    painter.setBrush(linearGradient);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(fillRect, 8, 8);

    painter.setFont(m_font);
    painter.setPen(textcolor);
    QFontMetrics fm = painter.fontMetrics();
    QString tStitlename = m_vScheduleInfo[index].titleName;
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

    painter.drawText(QRect(fillRect.topLeft().x() + 13, 2, fillRect.width(), fillRect.height()), Qt::AlignLeft, tstr);
    if (m_vHoverflag[index] && !m_vSelectflag[index]) {
        QRectF trect = QRectF(fillRect.x() + 0.5, fillRect.y() + 0.5, fillRect.width() - 1, fillRect.height() - 1);
        painter.save();
        painter.setRenderHints(QPainter::Antialiasing);
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
        painter.setBrush(Qt::NoBrush);
        painter.setPen(pen);
        painter.drawRoundedRect(trect, 8, 8);
        painter.restore();
    }
    if (m_vSelectflag[index]) {
        QColor selcolor = "#000000";
        selcolor.setAlphaF(0.05);
        painter.setBrush(selcolor);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(fillRect, 8, 8);
    }
}


void CAllDayEventWeekView::setTheMe(int type)
{
    updateDateShow();
}

void CAllDayEventWeekView::setRange(int w, int h, QDate begindate, QDate enddate, int rightmagin)
{
    m_coorManage->setRange(w, h, begindate, enddate, rightmagin);
    m_rightmagin = rightmagin;
    updateDateShow();
}

void CAllDayEventWeekView::setLunarVisible(bool state)
{
    m_LunarVisible = state;
}

void CAllDayEventWeekView::updateHigh()
{
    for (int i = 0; i < m_baseShowItem.count(); i++) {
        m_baseShowItem.at(i)->update();
    }
}

CAllDayEventWeekView::CAllDayEventWeekView(QWidget *parent, int edittype) : DListWidget (parent)
{


    //setWindowFlags(Qt::FramelessWindowHint);
    //setAttribute(Qt::WA_TranslucentBackground, true);
    m_editType = edittype;
    m_widgetFlag = false;
    setContentsMargins(0, 0, 0, 0);
    //setSpacing(1);
    //DPalette mainpa = this->palette();
    // this->setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Light, QColor(0, 0, 0, 0));
    pal.setColor(QPalette::Dark, QColor(0, 0, 0, 0));
    pal.setColor(QPalette::Base, QColor(0, 0, 0, 0));
    //pal.setBrush(backgroundRole(), QColor(0, 0, 0, 0));
    setPalette(pal);

    //setWindowFlags(Qt::FramelessWindowHint);
    //setAttribute(Qt::WA_TranslucentBackground, true);
    setFrameShape(QListWidget::NoFrame);
    //setWindowOpacity(0);
    setMouseTracking(true);
    setFocusPolicy(Qt::NoFocus);

    m_coorManage = new CScheduleCoorManage;

    m_createAction = new QAction(tr("New event"), this);
    connect(m_createAction, &QAction::triggered, this, &CAllDayEventWeekView::slotCreate);
    //setStyleSheet("background-color:transparent");
    setSelectionMode(QAbstractItemView::NoSelection);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setSelectionModel(nullptr);
}

CAllDayEventWeekView::~CAllDayEventWeekView()
{

}

void CAllDayEventWeekView::setDayData(const QVector<QVector<ScheduleDtailInfo> > &vlistData, int type)
{
    m_vlistData = vlistData;
    m_type = type;
    m_widgetFlag = false;
    updateDateShow();
}

void CAllDayEventWeekView::setsolarDayData(QVector<QString> vSolarInfo, QVector<QDate> date)
{
    m_vSolarDayInfo = vSolarInfo;
    m_widgetFlag = false;
    m_vDate = date;
    updateDateShow();
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

void CAllDayEventWeekView::contextMenuEvent(QContextMenuEvent *event)
{
    //if (m_vlistData.isEmpty()) {
    QMenu Context(this);
    Context.addAction(m_createAction);
    m_dianjiDay = m_coorManage->getsDate(mapFrom(this, event->pos()));
    Context.exec(QCursor::pos());
    //}
}

void CAllDayEventWeekView::mouseDoubleClickEvent(QMouseEvent *event)
{
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
}

void CAllDayEventWeekView::wheelEvent(QWheelEvent *event)
{
    DListWidget::wheelEvent(event);
    emit signalScheduleShow(false, 0);
}


void CAllDayEventWeekView::updateDateShow()
{
    m_currentitem = nullptr;
    //remove
    for (int i = 0; i < this->count(); i++) {
        QListWidgetItem *item11 = this->takeItem(i);
        this->removeItemWidget(item11);
    }
    this->clear();
    for (int i = 0; i < m_baseShowItem.count(); i++) {
        m_baseShowItem[i]->deleteLater();
    }
    m_baseShowItem.clear();
    if (m_vSolarDayInfo.isEmpty() || !m_LunarVisible) {
        for (int i = 0; i < m_vlistData.size(); ++i) {
            CAllDayEventWidgetItem *gwi = createItemWidget(i);
            QListWidgetItem *listItem = new QListWidgetItem;
            listItem->setSizeHint(QSize(gwi->width(), 23)); //每次改变Item的高度
            //listItem->setBackgroundColor(Qt::white);
            listItem->setFlags(Qt::ItemIsTristate );
            addItem(listItem);
            setItemWidget(listItem, gwi);
            gwi->setItem(listItem);
        }

    } else {
        CSolodayWidgetItem *solargwi = createItemWidget(m_vSolarDayInfo, m_vDate);
        QListWidgetItem *solarlistItem = new QListWidgetItem;
        addItem(solarlistItem);
        setItemWidget(solarlistItem, solargwi);
        m_baseShowItem.append(solargwi);
        for (int i = 0; i < m_vlistData.size(); ++i) {
            CAllDayEventWidgetItem *gwi = createItemWidget(i, false);
            QListWidgetItem *listItem = new QListWidgetItem;
            listItem->setSizeHint(QSize(gwi->width(), 23)); //每次改变Item的高度
            //listItem->setBackgroundColor(Qt::white);
            listItem->setFlags(Qt::ItemIsTristate );
            addItem(listItem);
            setItemWidget(listItem, gwi);
            gwi->setItem(listItem);
        }

    }
}

CAllDayEventWidgetItem *CAllDayEventWeekView::createItemWidget(int index, bool average)
{
    CAllDayEventWidgetItem *gwi = new CAllDayEventWidgetItem(this, m_editType);
    gwi->setCoorManage(m_coorManage);
    if (m_type == 0) {
        gwi->setData(m_vlistData[index]);
        gwi->setFixedSize(width() - m_rightmagin, 22);
        gwi->setItem(nullptr);
    }
    connect(gwi, &CAllDayEventWidgetItem::signalsDelete, this, &CAllDayEventWeekView::slotdeleteitem);
    connect(gwi, &CAllDayEventWidgetItem::signalsEdit, this, &CAllDayEventWeekView::slotedititem);
    connect(gwi, &CAllDayEventWidgetItem::signalsPress, this, &CAllDayEventWeekView::slotupdateItem);
    connect(gwi, &CAllDayEventWidgetItem::signalViewtransparentFrame, this, &CAllDayEventWeekView::signalViewtransparentFrame);
    connect(gwi, &CAllDayEventWidgetItem::signalScheduleShow, this, &CAllDayEventWeekView::signalScheduleShow);
    return gwi;
}

CSolodayWidgetItem *CAllDayEventWeekView::createItemWidget(QVector<QString> vSolarInfo, QVector<QDate> date, bool average)
{
    CSolodayWidgetItem *gwi = new CSolodayWidgetItem(this, m_editType);
    gwi->setCoorManage(m_coorManage);
    QColor color1 = m_soloColor;
    color1.setAlphaF(0.3);
    CSchedulesColor gdcolor = CScheduleDataManage::getScheduleDataManage()->getScheduleColorByType(1);
    if (m_type == 0) {
        gwi->setColor(color1, color1, true);
        QFont font("PingFangSC-Light");
        if (average) {
            font.setPixelSize(6);
        } else {
            font.setPixelSize(12);
        }
        gwi->setData(vSolarInfo, date);
        int w = width();
        if (average) {
            gwi->setFixedSize(width() - m_rightmagin, 22);
            gwi->setText(gdcolor.textColor, font, QPoint(13, 2), average);
        } else {
            gwi->setFixedSize(width() - m_rightmagin, 22);
            gwi->setText(gdcolor.textColor, font, QPoint(13, 2), average);
        }
    } else {
        gwi->setColor(color1, color1, true);
        QFont font("PingFangSC-Light");
        if (average) {
            font.setPixelSize(6);
        } else {
            font.setPixelSize(12);
        }
        gwi->setData(vSolarInfo, date);
        if (average) {
            gwi->setFixedSize(width() - m_rightmagin, 22);
            gwi->setText(gdcolor.textColor, font, QPoint(13, 2), average);
        } else {
            gwi->setFixedSize(width() - m_rightmagin, 22);
            gwi->setText(gdcolor.textColor, font, QPoint(13, 2), average);
        }
    }
    return gwi;
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

void CAllDayEventWeekView::slotupdateItem(CAllDayEventWidgetItem *item)
{
    m_currentitem = item;
    emit signalsitem(this);
}

void CAllDayEventWeekView::slotDeleteItem()
{
    if (m_currentitem != nullptr) {
        m_currentitem->slotDelete();
    }
}


CSolodayWidgetItem::CSolodayWidgetItem(QWidget *parent, int edittype)
{

}

void CSolodayWidgetItem::setColor(QColor color1, QColor color2, bool GradientFlag)
{
    m_color1 = color1;
    m_color2 = color2;
    m_GradientFlag = GradientFlag;
}

void CSolodayWidgetItem::setText(QColor tcolor, QFont font, QPoint pos, bool avgeflag)
{
    m_textcolor = tcolor;
    m_font = font;
    m_pos = pos;
}

void CSolodayWidgetItem::setData(QVector<QString> vSolarInfo, QVector<QDate> date)
{
    m_vhover.clear();
    m_vselectflag.clear();
    m_vSolarDayInfo = vSolarInfo;
    m_vDate = date;
    m_vhover.resize(m_vDate.count());
    m_vselectflag.resize(m_vDate.count());
    m_vhover.fill(false);
    m_vselectflag.fill(false);
}
void CSolodayWidgetItem::paintEvent(QPaintEvent *e)
{
    int labelwidth = width();
    int labelheight = height();
    QPainter painter(this);
    for (int i = 0; i < m_vDate.count(); i++) {
        QRect drawrect = m_coorManage->getAllDayDrawRegion(m_vDate.at(i), m_vDate.at(i));
        if (drawrect.width() < 0) return;

        if (m_GradientFlag) {
            QRect fillRect = QRect(2, 1, labelwidth - 2, labelheight - 1);
            //将直线开始点设为0，终点设为1，然后分段设置颜色
            painter.save();
            painter.setRenderHints(QPainter::HighQualityAntialiasing);
            painter.setBrush(QColor(0, 0, 0, 0));
            painter.setPen(Qt::NoPen);
            painter.drawRoundedRect(fillRect, 8, 8);
            painter.restore();
            painter.save();
            QLinearGradient linearGradient(0, 0, labelwidth, 0);
            QColor color1 = m_color1;
            QColor color2 = m_color2;
            QColor textcolor = m_textcolor;

            if (m_vhover.at(i)) {
                color1.setAlphaF(0.4);
                color2.setAlphaF(0.4);
                //textcolor.setAlphaF(textcolor.alphaF() * 0.94);
            }
            linearGradient.setColorAt(0, color1);
            linearGradient.setColorAt(1, color2);
            QRect drawrect2 = drawrect;
            //将直线开始点设为0，终点设为1，然后分段设置颜色
            painter.setRenderHints(QPainter::HighQualityAntialiasing);
            painter.setBrush(linearGradient);
            painter.setPen(Qt::NoPen);
            painter.drawRoundedRect(drawrect2, 8, 8);
            painter.restore();
            painter.setFont(m_font);
            QFont solofont = m_font;
            QFontMetrics fm = painter.fontMetrics();
            QString str =  m_vSolarDayInfo.at(i);
            QString tstr;
            for (int i = 0; i < str.count(); i++) {
                tstr.append(str.at(i));
                int widthT = fm.width(tstr);
                if (widthT >= drawrect.width() - 6) {
                    tstr.chop(2);
                    break;
                }
            }
            if (tstr != str) {
                tstr = tstr + "...";
            }
            painter.setPen(textcolor);
            painter.drawText(QRect(drawrect.topLeft().x() + 13, drawrect.topLeft().y() + (drawrect.height() - fm.height()) / 2, drawrect.width(), drawrect.height()), Qt::AlignLeft, tstr);
        }
    }
}
void CSolodayWidgetItem::mousePressEvent(QMouseEvent *event)
{
    emit signalScheduleShow(false);
    m_vselectflag.fill(false);
    /*for (int i = 0; i < m_vDate.count(); i++) {
        QRect drawrect = m_coorManage->getAllDayDrawRegion(m_vDate.at(i), m_vDate.at(i));
        if (drawrect.contains(mapFromGlobal(QCursor::pos()))) {
            m_vhover[i] = true;
        }
    }*/
    update();
}

void CSolodayWidgetItem::focusOutEvent(QFocusEvent *event)
{
    m_vselectflag.fill(false);
    update();
}

void CSolodayWidgetItem::enterEvent(QEvent *event)
{
    m_vhover.fill(false);
    /*for (int i = 0; i < m_vDate.count(); i++) {
        QRect drawrect = m_coorManage->getAllDayDrawRegion(m_vDate.at(i), m_vDate.at(i));
        if (drawrect.contains(mapFromGlobal(QCursor::pos()))) {
            m_vhover[i] = true;
        }
    }*/
    update();
}

void CSolodayWidgetItem::leaveEvent(QEvent *event)
{
    m_vhover.fill(false);
    update();
}
