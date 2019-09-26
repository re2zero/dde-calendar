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
#include "alldayschceduleview.h"
#include <QAction>
#include <QMenu>
#include <QListWidget>
#include <QLabel>
#include <QPainter>
#include <QHBoxLayout>
#include <QStylePainter>
#include <QRect>
#include "dbmanager.h"
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
CAllDaySchceduleWidgetItem::CAllDaySchceduleWidgetItem( QWidget *parent /*= nullptr*/, int edittype): DPushButton(parent)
{
    m_editType = edittype;
    //setMargin(0);
    m_editAction = new QAction(tr("Edit"), this);
    m_deleteAction = new QAction(tr("Delete"), this);
    connect(m_editAction, SIGNAL(triggered(bool)), this, SLOT(slotEdit()));
    connect(m_deleteAction, SIGNAL(triggered(bool)), this, SLOT(slotDelete()));
    m_item = NULL;
}

void CAllDaySchceduleWidgetItem::setColor( QColor color1, QColor color2, bool GradientFlag /*= false*/ )
{
    m_color1 = color1;
    m_color2 = color2;
    m_GradientFlag = GradientFlag;
}

void CAllDaySchceduleWidgetItem::setText( QColor tcolor, QFont font, QPoint pos, bool avgeflag)
{
    m_textcolor = tcolor;
    m_font = font;
    m_pos = pos;
    m_avgeflag = avgeflag;
}

void CAllDaySchceduleWidgetItem::getColor(QColor &color1, QColor &color2, bool &GradientFlag)
{
    color2 = m_color2;
    color1 = m_color1;
    GradientFlag = m_GradientFlag;
}

void CAllDaySchceduleWidgetItem::getText(QColor &tcolor, QFont &font, QPoint &pos)
{
    tcolor = m_textcolor;
    font = m_font;
    pos = m_pos;
}

void CAllDaySchceduleWidgetItem::setData( ScheduleDtailInfo vScheduleInfo )
{
    m_ScheduleInfo = vScheduleInfo;
    setToolTip(m_ScheduleInfo.titleName);
    update();
}
void CAllDaySchceduleWidgetItem::slotEdit()
{
    CSchceduleDlg dlg(0, this);
    dlg.setData(m_ScheduleInfo);
    if (dlg.exec() == DDialog::Accepted) {

        emit signalsEdit(this, 1);
    }
}

void CAllDaySchceduleWidgetItem::slotDelete()
{
    int themetype = CScheduleDataManage::getScheduleDataManage()->getTheme();

    if (m_ScheduleInfo.rpeat == 0) {
        CSchceduleCtrlDlg msgBox(this);
        //msgBox.setWindowFlags(Qt::FramelessWindowHint);
        // msgBox.setIconPixmap(DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-logo.svg").scaled(QSize(34, 34) * devicePixelRatioF()));

        msgBox.setText(tr("You are deleted schedule."));
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
            CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->deleteScheduleInfoById(m_ScheduleInfo.id);
        }
    } else {
        if (m_ScheduleInfo.RecurID == 0) {
            CSchceduleCtrlDlg msgBox(this);
            //msgBox.setWindowFlags(Qt::FramelessWindowHint);
            //msgBox.setIconPixmap(DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-logo.svg").scaled(QSize(34, 34) * devicePixelRatioF()));

            msgBox.setText(tr("You are deleted schedule."));
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
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->deleteScheduleInfoById(m_ScheduleInfo.id);
            } else if (msgBox.clickButton() == yesButton) {

                ScheduleDtailInfo newschedule;
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(m_ScheduleInfo.id, newschedule);
                newschedule.ignore.append(m_ScheduleInfo.beginDateTime);
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);
            }
        } else {
            CSchceduleCtrlDlg msgBox(this);
            //msgBox.setWindowFlags(Qt::FramelessWindowHint);
            //msgBox.setIconPixmap(DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-logo.svg").scaled(QSize(34, 34) * devicePixelRatioF()));
            msgBox.setText(tr("You are deleted schedule."));
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
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(m_ScheduleInfo.id, newschedule);
                newschedule.enddata.type = 2;
                newschedule.enddata.date = m_ScheduleInfo.beginDateTime;
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);

            } else if (msgBox.clickButton() == yesButton) {

                ScheduleDtailInfo newschedule;
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(m_ScheduleInfo.id, newschedule);
                newschedule.ignore.append(m_ScheduleInfo.beginDateTime);
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);
            }
        }
    }
    emit signalsDelete(this);
}
//有问题
void CAllDaySchceduleWidgetItem::slotDoubleEvent(int type)
{
    emit signalsEdit(this, 1);
}

void CAllDaySchceduleWidgetItem::paintEvent( QPaintEvent *e )
{
    int labelwidth = width();
    int labelheight = height();
    float avge = 1;
    if (m_avgeflag) {
        avge = 0.5;
    }
    QRect drawrect = m_coorManage->getAllDayDrawRegion(m_ScheduleInfo.beginDateTime.date(), m_ScheduleInfo.endDateTime.date());
    QPainter painter(this);
    if (m_GradientFlag) {

        QLinearGradient linearGradient(0, 0, labelwidth, 0);
        linearGradient.setColorAt(0, m_color1);
        linearGradient.setColorAt(1, m_color2);
        QRect fillRect = drawrect;
        //将直线开始点设为0，终点设为1，然后分段设置颜色
        painter.setRenderHints(QPainter::HighQualityAntialiasing);
        painter.setBrush(linearGradient);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(fillRect, 3, 3 * avge);

        painter.setFont(m_font);
        painter.setPen(m_textcolor);
        QFontMetrics fm = painter.fontMetrics();
        QString str = "-" + m_ScheduleInfo.titleName;
        if (fm.width(str) > fillRect.width() - 13) {
            int widthT = fm.width(str);
            int singlecharw = widthT * 1.0 / str.count();
            int rcharcount = fillRect.width() * 1.0 / singlecharw;
            QString tstr;
            for (int i = 0; i < rcharcount - 8; i++) {
                tstr.append(str.at(i));
            }
            str = tstr + "...";
        }
        painter.drawText(QRect(fillRect.topLeft().x() + 13, 0, fillRect.width(), fillRect.height()), Qt::AlignLeft, str);
    } else {
        QRect fillRect = QRect(2, 2 * avge, labelwidth - 2, labelheight - 2 * avge);
        //将直线开始点设为0，终点设为1，然后分段设置颜色
        painter.setRenderHints(QPainter::HighQualityAntialiasing);
        painter.setBrush(m_color1);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(fillRect, 3, 3);

        painter.setFont(m_font);
        painter.setPen(m_textcolor);
        QFontMetrics fm = painter.fontMetrics();
        QString str = m_ScheduleInfo.titleName;
        if (fm.width(str) > width()) {
            int widthT = fm.width(str);
            int singlecharw = widthT * 1.0 / str.count();
            int rcharcount = width() * 1.0 / singlecharw;
            QString tstr;
            for (int i = 0; i < rcharcount - 8; i++) {
                tstr.append(str.at(i));
            }
            str = tstr + "...";
        }
        painter.drawText(QRect(m_pos.x(), m_pos.y(), labelwidth - m_pos.x(), labelheight), Qt::AlignLeft, str);
    }
}
void CAllDaySchceduleWidgetItem::contextMenuEvent( QContextMenuEvent *event )
{
    DMenu Context(this);
    Context.addAction(m_editAction);
    Context.addAction(m_deleteAction);
    Context.exec(QCursor::pos());
}

void CAllDaySchceduleWidgetItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (m_editType == 0) return;
    CMySchceduleView dlg(this);
    dlg.setSchedules(m_ScheduleInfo);
    connect(&dlg, &CMySchceduleView::signalsEditorDelete, this, &CAllDaySchceduleWidgetItem::slotDoubleEvent);
    dlg.exec();
    disconnect(&dlg, &CMySchceduleView::signalsEditorDelete, this, &CAllDaySchceduleWidgetItem::slotDoubleEvent);
}

void CAllDaySchceduleView::setDayData(QDate date, const QVector<ScheduleDtailInfo> &vlistData, int type)
{
    m_currentDate = date;
    m_vlistData = vlistData;
    m_type = type;
    m_widgetFlag = false;
    updateDateShow();
}

void CAllDaySchceduleView::setsolarDayData(QString solarDay)
{
    m_solarDay = solarDay;
    m_widgetFlag = false;
    updateDateShow();
}

void CAllDaySchceduleView::setDate(QDate date, int type)
{
    m_currentDate = date;
    if (type) {
        //m_vlistData = ScheduleDbManager::getScheduleInfo(date, 0);
    } else {
        // m_vlistData = ScheduleDbManager::getALLDayScheduleInfo(date);
    }
    m_type = type;
    updateDateShow();
}

void CAllDaySchceduleView::setTheMe(int type)
{
    updateDateShow();
}

void CAllDaySchceduleView::setRange(int w, int h, QDate begindate, QDate enddate)
{
    m_coorManage->setRange(w, h, begindate, enddate);
    updateDateShow();
}

CAllDaySchceduleView::CAllDaySchceduleView(QWidget *parent, int edittype) : DListWidget (parent)
{


    //setWindowFlags(Qt::FramelessWindowHint);
    //setAttribute(Qt::WA_TranslucentBackground, true);
    m_editType = edittype;
    m_widgetFlag = false;
    setContentsMargins(0, 0, 0, 0);
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
    setFocusPolicy(Qt::StrongFocus);

    m_coorManage = new CScheduleCoorManage;

    m_createAction = new QAction(tr("Create"), this);
    connect(m_createAction, &QAction::triggered, this, &CAllDaySchceduleView::slotCreate);
    //setStyleSheet("background-color:transparent");
    setSelectionMode(QAbstractItemView::NoSelection);
}

CAllDaySchceduleView::~CAllDaySchceduleView()
{

}
void CAllDaySchceduleView::slotCreate()
{
    CSchceduleDlg dlg(1, this);
    QDateTime tDatatime;
    tDatatime.setDate(m_currentDate);
    tDatatime.setTime(QTime::currentTime());
    dlg.setDate(tDatatime);
    if (dlg.exec() == DDialog::Accepted) {
        emit signalsCotrlUpdateShcedule(m_currentDate, 1);
    }
}

void CAllDaySchceduleView::contextMenuEvent(QContextMenuEvent *event)
{
    if (m_vlistData.isEmpty()) {
        QMenu Context(this);
        Context.addAction(m_createAction);
        Context.exec(QCursor::pos());
    }
}

void CAllDaySchceduleView::updateDateShow()
{
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
    if (m_solarDay.isEmpty()) {
        for (int i = 0; i < m_vlistData.size(); ++i) {
            CAllDaySchceduleWidgetItem *gwi = createItemWidget(i);
            QListWidgetItem *listItem = new QListWidgetItem;
            addItem(listItem);
            setItemWidget(listItem, gwi);
            gwi->setItem(listItem);
        }

    } else {
        CAllSolarDayWidgetItem *solargwi = createItemWidget(m_solarDay);
        QListWidgetItem *solarlistItem = new QListWidgetItem;
        addItem(solarlistItem);
        setItemWidget(solarlistItem, solargwi);
        m_baseShowItem.append(solargwi);
        for (int i = 0; i < m_vlistData.size(); ++i) {
            CAllDaySchceduleWidgetItem *gwi = createItemWidget(i, false);
            QListWidgetItem *listItem = new QListWidgetItem;
            addItem(listItem);
            setItemWidget(listItem, gwi);
            gwi->setItem(listItem);
        }

    }
}

CAllDaySchceduleWidgetItem *CAllDaySchceduleView::createItemWidget(int index, bool average)
{
    const ScheduleDtailInfo &gd = m_vlistData.at(index);
    CSchedulesColor gdcolor = CScheduleDataManage::getScheduleDataManage()->getScheduleColorByType(gd.type.ID);
    CAllDaySchceduleWidgetItem *gwi = new CAllDaySchceduleWidgetItem(this, m_editType);
    gwi->setCoorManage(m_coorManage);
    if (m_type == 0) {
        gwi->setColor(gdcolor.gradientFromC, gdcolor.gradientToC, true);
        QFont font("PingFangSC-Light");
        if (average) {
            font.setPixelSize(6);
        } else {
            font.setPixelSize(12);
        }
        gwi->setData(gd);
        if (average) {
            gwi->setFixedSize(width(), 22);
            gwi->setText(gdcolor.textColor, font, QPoint(23, 2), average);
        } else {
            gwi->setFixedSize(width(), 22);
            gwi->setText(gdcolor.textColor, font, QPoint(23, 2), average);
        }
        gwi->setItem(NULL);
    } else {
        gwi->setColor(gdcolor.gradientFromC, gdcolor.gradientToC, true);
        QFont font("PingFangSC-Light");
        if (average) {
            font.setPixelSize(6);
        } else {
            font.setPixelSize(12);
        }
        if (average) {
            gwi->setFixedSize(width(), 22);
            gwi->setText(gdcolor.textColor, font, QPoint(8, 2), average);
        } else {
            gwi->setFixedSize(width(), 22);
            gwi->setText(gdcolor.textColor, font, QPoint(8, 2), average);
        }
        gwi->setData(gd);
        gwi->setItem(NULL);
    }
    connect(gwi, &CAllDaySchceduleWidgetItem::signalsDelete, this, &CAllDaySchceduleView::slotdeleteitem);
    connect(gwi, &CAllDaySchceduleWidgetItem::signalsEdit, this, &CAllDaySchceduleView::slotedititem);
    return gwi;
}

CAllSolarDayWidgetItem *CAllDaySchceduleView::createItemWidget(QString solarDay, bool average)
{
    CAllSolarDayWidgetItem *gwi = new CAllSolarDayWidgetItem(this, m_editType);
    gwi->setCoorManage(m_coorManage);
    QColor color1 = m_soloColor;
    color1.setAlphaF(0.3);

    if (m_type == 0) {
        gwi->setColor(color1, color1, true);
        QFont font("PingFangSC-Light");
        if (average) {
            font.setPixelSize(6);
        } else {
            font.setPixelSize(12);
        }
        gwi->setData(solarDay, m_currentDate);
        int w = width();
        if (average) {
            gwi->setFixedSize(width(), 22);
            gwi->setText(Qt::black, font, QPoint(13, 2), average);
        } else {
            gwi->setFixedSize(width(), 22);
            gwi->setText(Qt::black, font, QPoint(13, 2), average);
        }
    } else {
        gwi->setColor(color1, color1, true);
        QFont font("PingFangSC-Light");
        if (average) {
            font.setPixelSize(6);
        } else {
            font.setPixelSize(12);
        }
        gwi->setData(solarDay, m_currentDate);
        if (average) {
            gwi->setFixedSize(width(), 22);
            gwi->setText(Qt::black, font, QPoint(13, 2), average);
        } else {
            gwi->setFixedSize(width(), 22);
            gwi->setText(Qt::black, font, QPoint(13, 2), average);
        }
    }
    return gwi;
}

void CAllDaySchceduleView::slotdeleteitem( CAllDaySchceduleWidgetItem *item)
{
    emit signalsUpdateShcedule(item->getData().id);
    emit signalsCotrlUpdateShcedule(m_currentDate, 1);
    updateDateShow();
    update();
}

void CAllDaySchceduleView::slotedititem(CAllDaySchceduleWidgetItem *item, int type)
{

    emit signalsUpdateShcedule(item->getData().id);
    emit signalsCotrlUpdateShcedule(m_currentDate, type);
    updateDateShow();
    update();
}


CAllSolarDayWidgetItem::CAllSolarDayWidgetItem(QWidget *parent, int edittype)
{

}

void CAllSolarDayWidgetItem::setColor(QColor color1, QColor color2, bool GradientFlag)
{
    m_color1 = color1;
    m_color2 = color2;
    m_GradientFlag = GradientFlag;
}

void CAllSolarDayWidgetItem::setText(QColor tcolor, QFont font, QPoint pos, bool avgeflag)
{
    m_textcolor = tcolor;
    m_font = font;
    m_pos = pos;
}

void CAllSolarDayWidgetItem::setData(QString vSolarInfo, QDate date)
{
    m_SolarDayInfo = vSolarInfo;
    m_date = date;
}

void CAllSolarDayWidgetItem::paintEvent(QPaintEvent *e)
{
    int labelwidth = width();
    int labelheight = height();

    QRect drawrect = m_coorManage->getAllDayDrawRegion(m_date, m_date);
    QPainter painter(this);
    if (m_GradientFlag) {
        QRect fillRect = QRect(2, 1, labelwidth - 2, labelheight - 1);
        //将直线开始点设为0，终点设为1，然后分段设置颜色
        painter.setRenderHints(QPainter::HighQualityAntialiasing);
        painter.setBrush(QColor(0, 0, 0, 0));
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(fillRect, 3, 2);

        QLinearGradient linearGradient(0, 0, labelwidth, 0);
        linearGradient.setColorAt(0, m_color1);
        linearGradient.setColorAt(1, m_color2);
        QRect drawrect2 = drawrect;
        //将直线开始点设为0，终点设为1，然后分段设置颜色
        painter.setRenderHints(QPainter::HighQualityAntialiasing);
        painter.setBrush(linearGradient);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(drawrect2, 3, 2);

        painter.setFont(m_font);
        painter.setFont(m_font);
        QFont solofont = m_font;
        QFontMetrics fm = painter.fontMetrics();
        while (fm.width(m_SolarDayInfo) > drawrect.width() - 6) {
            solofont.setPixelSize(solofont.pixelSize() - 1);
            painter.setFont(solofont);
            fm = painter.fontMetrics();
        }
        painter.setPen(m_textcolor);
        painter.drawText(QRect(drawrect.topLeft().x() + 2, drawrect.topLeft().y() + (drawrect.height() - fm.height()) / 2, drawrect.width(), drawrect.height()), Qt::AlignLeft, m_SolarDayInfo);
    } else {
        QRect fillRect = QRect(2, 1, labelwidth - 2, labelheight - 1);
        //将直线开始点设为0，终点设为1，然后分段设置颜色
        painter.setRenderHints(QPainter::HighQualityAntialiasing);
        painter.setBrush(m_color1);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(fillRect, 3, 2);

        painter.setFont(m_font);
        QFont solofont = m_font;
        QFontMetrics fm = painter.fontMetrics();
        while (fm.width(m_SolarDayInfo) > labelwidth - m_pos.x() - 3) {
            solofont.setPixelSize(solofont.pixelSize() - 1);
            painter.setFont(solofont);
            fm = painter.fontMetrics();
        }
        painter.setPen(m_textcolor);
        painter.drawText(QRect(m_pos.x(), m_pos.y(), labelwidth - m_pos.x(), labelheight), Qt::AlignLeft, m_SolarDayInfo);
    }
}
