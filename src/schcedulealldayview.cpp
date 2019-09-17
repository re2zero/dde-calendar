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
#include "schcedulealldayview.h"
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
DGUI_USE_NAMESPACE
CSchceduleAllWidgetItem::CSchceduleAllWidgetItem( QWidget *parent /*= nullptr*/, int edittype): DPushButton(parent)
{
    m_editType = edittype;
    //setMargin(0);
    m_editAction = new QAction(tr("Edit"), this);
    m_deleteAction = new QAction(tr("Delete"), this);
    connect(m_editAction, SIGNAL(triggered(bool)), this, SLOT(slotEdit()));
    connect(m_deleteAction, SIGNAL(triggered(bool)), this, SLOT(slotDelete()));
    m_item = NULL;
}

void CSchceduleAllWidgetItem::setColor( QColor color1, QColor color2, bool GradientFlag /*= false*/ )
{
    m_color1 = color1;
    m_color2 = color2;
    m_GradientFlag = GradientFlag;
}

void CSchceduleAllWidgetItem::setText( QColor tcolor, QFont font, QPoint pos, bool avgeflag)
{
    m_textcolor = tcolor;
    m_font = font;
    m_pos = pos;
    m_avgeflag = avgeflag;
}

void CSchceduleAllWidgetItem::getColor(QColor &color1, QColor &color2, bool &GradientFlag)
{
    color2 = m_color2;
    color1 = m_color1;
    GradientFlag = m_GradientFlag;
}

void CSchceduleAllWidgetItem::getText(QColor &tcolor, QFont &font, QPoint &pos)
{
    tcolor = m_textcolor;
    font = m_font;
    pos = m_pos;
}

void CSchceduleAllWidgetItem::setData( ScheduleDtailInfo vScheduleInfo )
{
    m_ScheduleInfo = vScheduleInfo;
    setToolTip(m_ScheduleInfo.titleName);
    update();
}
void CSchceduleAllWidgetItem::slotEdit()
{
    CSchceduleDlg dlg(0, this);
    dlg.setData(m_ScheduleInfo);
    if (dlg.exec() == DDialog::Accepted) {

        emit signalsEdit(this, 1);
    }
}

void CSchceduleAllWidgetItem::slotDelete()
{
    if (m_ScheduleInfo.rpeat == 0) {
        DMessageBox msgBox;
        msgBox.setWindowFlags(Qt::FramelessWindowHint);
        msgBox.setIconPixmap(DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-logo.svg").scaled(QSize(34, 34) * devicePixelRatioF()));

        msgBox.setText(tr("You are deleted schedule."));
        msgBox.setInformativeText(tr("Are you sure you want to delete this schedule?"));
        DPushButton *noButton = msgBox.addButton(tr("Cancel"), DMessageBox::NoRole);
        DPushButton *yesButton = msgBox.addButton(tr("Delete Schedule"), DMessageBox::YesRole);
        DPalette pa = yesButton->palette();
        pa.setColor(DPalette::ButtonText, Qt::red);
        yesButton->setPalette(pa);
        msgBox.exec();

        if (msgBox.clickedButton() == noButton) {
            return;
        } else if (msgBox.clickedButton() == yesButton) {
            CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->deleteScheduleInfoById(m_ScheduleInfo.id);
        }
    } else {
        if (m_ScheduleInfo.RecurID == 0) {
            DMessageBox msgBox;
            msgBox.setWindowFlags(Qt::FramelessWindowHint);
            msgBox.setIconPixmap(DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-logo.svg").scaled(QSize(34, 34) * devicePixelRatioF()));

            msgBox.setText(tr("You are deleted schedule."));
            msgBox.setInformativeText(tr("You want to delete all repeat of the schedule, or just delete the selected repeat?"));
            DPushButton *noButton = msgBox.addButton(tr("Cancel"), DMessageBox::NoRole);
            DPushButton *yesallbutton = msgBox.addButton(tr("All Deleted"), DMessageBox::YesRole);
            DPushButton *yesButton = msgBox.addButton(tr("Just Delete Schedule"), DMessageBox::YesRole);
            DPalette pa = yesButton->palette();
            pa.setColor(DPalette::ButtonText, Qt::white);
            pa.setColor(DPalette::Dark, QColor("#0098FF"));
            pa.setColor(DPalette::Light, QColor("#0098FF"));
            yesButton->setPalette(pa);
            msgBox.exec();

            if (msgBox.clickedButton() == noButton) {
                return;
            } else if (msgBox.clickedButton() == yesallbutton) {
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->deleteScheduleInfoById(m_ScheduleInfo.id);
            } else if (msgBox.clickedButton() == yesButton) {

                ScheduleDtailInfo newschedule;
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(m_ScheduleInfo.id, newschedule);
                newschedule.ignore.append(m_ScheduleInfo.beginDateTime);
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);
            }
        } else {
            DMessageBox msgBox;
            msgBox.setWindowFlags(Qt::FramelessWindowHint);
            msgBox.setIconPixmap(DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-logo.svg").scaled(QSize(34, 34) * devicePixelRatioF()));
            msgBox.setText(tr("You are deleted schedule."));
            msgBox.setInformativeText(tr("You want to delete the schedule of this repetition and all repeat in the future, or just delete all repeat?"));
            DPushButton *noButton = msgBox.addButton(tr("Cancel"), DMessageBox::NoRole);
            DPushButton *yesallbutton = msgBox.addButton(tr("Delete all schedule in the future"), DMessageBox::YesRole);
            DPushButton *yesButton = msgBox.addButton(tr("Just Delete Schedule"), DMessageBox::YesRole);
            DPalette pa = yesButton->palette();
            pa.setColor(DPalette::ButtonText, Qt::white);
            pa.setColor(DPalette::Dark, QColor("#0098FF"));
            pa.setColor(DPalette::Light, QColor("#0098FF"));
            yesButton->setPalette(pa);
            msgBox.exec();

            if (msgBox.clickedButton() == noButton) {
                return;
            } else if (msgBox.clickedButton() == yesallbutton) {
                ScheduleDtailInfo newschedule;
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(m_ScheduleInfo.id, newschedule);
                newschedule.enddata.type = 2;
                newschedule.enddata.date = m_ScheduleInfo.beginDateTime;
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);

            } else if (msgBox.clickedButton() == yesButton) {

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
void CSchceduleAllWidgetItem::slotDoubleEvent(int type)
{
    emit signalsEdit(this, 1);
}

void CSchceduleAllWidgetItem::paintEvent( QPaintEvent *e )
{
    int labelwidth = width();
    int labelheight = height();
    float avge = 1;
    if (m_avgeflag) {
        avge = 0.5;
    }
    QPainter painter(this);
    if (m_GradientFlag) {
        QLinearGradient linearGradient(0, 0, labelwidth, 0);
        linearGradient.setColorAt(0, m_color1);
        linearGradient.setColorAt(1, m_color2);
        QRect fillRect = QRect(2, 2 * avge, labelwidth - 2, labelheight - 2 * avge);
        //将直线开始点设为0，终点设为1，然后分段设置颜色
        painter.setRenderHints(QPainter::HighQualityAntialiasing);
        painter.setBrush(linearGradient);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(fillRect, 3, 3 * avge);

        painter.setFont(m_font);
        painter.setPen(m_textcolor);
        QFontMetrics fm = painter.fontMetrics();
        QString str = "-" + m_ScheduleInfo.titleName;
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
void CSchceduleAllWidgetItem::contextMenuEvent( QContextMenuEvent *event )
{
    DMenu Context(this);
    Context.addAction(m_editAction);
    Context.addAction(m_deleteAction);
    Context.exec(QCursor::pos());
}

void CSchceduleAllWidgetItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (m_editType == 0) return;
    CMySchceduleView dlg(this);
    dlg.setSchedules(m_ScheduleInfo);
    connect(&dlg, &CMySchceduleView::signalsEditorDelete, this, &CSchceduleAllWidgetItem::slotDoubleEvent);
    dlg.exec();
    disconnect(&dlg, &CMySchceduleView::signalsEditorDelete, this, &CSchceduleAllWidgetItem::slotDoubleEvent);
}
CSchceduleAllNumButton::CSchceduleAllNumButton(QWidget *parent /*= nullptr*/): DPushButton(parent)
{

}

CSchceduleAllNumButton::~CSchceduleAllNumButton()
{

}

void CSchceduleAllNumButton::setColor( QColor color1, QColor color2, bool GradientFlag /*= false*/ )
{
    m_color1 = color1;
    m_color2 = color2;
    m_GradientFlag = GradientFlag;
}

void CSchceduleAllNumButton::setText( QColor tcolor, QFont font, QPoint pos)
{
    m_textcolor = tcolor;
    m_font = font;
    m_pos = pos;
}

void CSchceduleAllNumButton::paintEvent(QPaintEvent *e)
{
    int labelwidth = width();
    int labelheight = height();

    QPainter painter(this);
    if (m_GradientFlag) {
        QLinearGradient linearGradient(0, 0, labelwidth, 0);
        linearGradient.setColorAt(0, m_color1);
        linearGradient.setColorAt(1, m_color2);
        QRect fillRect = QRect(2, 1, labelwidth - 2, labelheight - 1);
        //将直线开始点设为0，终点设为1，然后分段设置颜色
        painter.setRenderHints(QPainter::HighQualityAntialiasing);
        painter.setBrush(linearGradient);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(fillRect, 3, 2);

        painter.setFont(m_font);
        painter.setPen(m_textcolor);
        painter.drawText(QRect(m_pos.x(), m_pos.y(), labelwidth - m_pos.x(), labelheight), Qt::AlignLeft, "-" + QString(tr("There is %1 schedule")).arg(m_num));
    } else {
        QRect fillRect = QRect(2, 1, labelwidth - 2, labelheight - 1);
        //将直线开始点设为0，终点设为1，然后分段设置颜色
        painter.setRenderHints(QPainter::HighQualityAntialiasing);
        painter.setBrush(m_color1);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(fillRect, 3, 2);

        painter.setFont(m_font);
        painter.setPen(m_textcolor);
        painter.drawText(QRect(m_pos.x(), m_pos.y(), labelwidth - m_pos.x(), labelheight), Qt::AlignLeft, QString(tr("There is %1 schedule")).arg(m_num));
    }
}

void CSchceduleAllDayView::setDayData(QDate date, const QVector<ScheduleDtailInfo> &vlistData, int type)
{
    m_currentDate = date;
    m_vlistData = vlistData;
    m_type = type;
    m_widt->hide();
    m_widgetFlag = false;
    updateDateShow();
}

void CSchceduleAllDayView::setsolarDayData(QString solarDay)
{
    m_solarDay = solarDay;
    m_widt->hide();
    m_widgetFlag = false;
    updateDateShow();
}

void CSchceduleAllDayView::setDate(QDate date, int type)
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

void CSchceduleAllDayView::setTheMe(int type)
{
    updateDateShow();
}

CSchceduleAllDayView::CSchceduleAllDayView(QWidget *parent, int edittype) : DWidget(parent)
{
    m_editType = edittype;
    m_widt = new DWidget(parent);

    m_widt->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    m_widt->setAttribute(Qt::WA_TranslucentBackground);

    QVBoxLayout *layout = new QVBoxLayout;
    m_widgetFlag = false;

    m_layout = new QVBoxLayout;
    m_layout->setContentsMargins(0, 0, 0, 0);

    m_gradientItemList = new DListWidget(parent);
    m_gradientItemList->setAlternatingRowColors(true);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_gradientItemList);

    m_widt->setLayout(layout);
    m_widt->hide();

    m_numButton = new CSchceduleAllNumButton(this);
    m_numButton->setColor(Qt::white, Qt::white, false);

    m_numButton->hide();
    m_layout->addWidget(m_numButton);
    connect(m_numButton, SIGNAL(clicked()), this, SLOT(slothidelistw()));
    // set default row
    m_gradientItemList->setCurrentRow(0);
    setLayout(m_layout);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    m_createAction = new QAction(tr("Create"), this);
    connect(m_createAction, &QAction::triggered, this, &CSchceduleAllDayView::slotCreate);
}

CSchceduleAllDayView::~CSchceduleAllDayView()
{

}
void CSchceduleAllDayView::slothidelistw()
{
    m_widgetFlag = !m_widgetFlag;

    if (m_widgetFlag) {
        int px = x();
        int py = y();
        QPoint pos = mapToGlobal(QPoint(px, py));
        int heighty = height() / 2 * m_gradientItemList->count();
        //if (heighty > window()->height() - this->y())
        // heighty = this->y();
        m_widt->resize(this->width(), heighty);
        m_widt->move(pos.x() - px, pos.y() + height() - py);
        m_widt->show();
    } else {
        m_widt->hide();
    }
}

void CSchceduleAllDayView::slotCreate()
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

void CSchceduleAllDayView::contextMenuEvent(QContextMenuEvent *event)
{
    if (m_vlistData.isEmpty()) {
        QMenu Context(this);
        Context.addAction(m_createAction);
        Context.exec(QCursor::pos());
    }
}

void CSchceduleAllDayView::updateDateShow()
{
    //remove
    for (int i = 0; i < m_gradientItemList->count(); i++) {
        QListWidgetItem *item11 = m_gradientItemList->takeItem(i);
        m_gradientItemList->removeItemWidget(item11);
    }
    m_gradientItemList->clear();
    for (int i = 0; i < m_baseShowItem.count(); i++) {
        //m_baseShowItem[i]->setParent(NULL);
        m_layout->removeWidget(m_baseShowItem[i]);
        m_baseShowItem[i]->deleteLater();
    }
    m_baseShowItem.clear();
    m_gradientItemList->clear();
    m_numButton->hide();
    if (m_solarDay.isEmpty()) {
        if (m_vlistData.count() == 1) {
            m_numButton->hide();
            CSchceduleAllWidgetItem *gwi = createItemWidget(0);
            m_layout->insertWidget(0, gwi);
            m_baseShowItem.append(gwi);
        } else if (m_vlistData.count() == 2) {
            m_numButton->hide();
            m_widt->hide();
            CSchceduleAllWidgetItem *gwiup = createItemWidget(0);
            CSchceduleAllWidgetItem *gwidown = createItemWidget(1);
            m_layout->insertWidget(0, gwiup);
            m_baseShowItem.append(gwiup);
            m_layout->insertWidget(0, gwidown);
            m_baseShowItem.append(gwidown);
        } else if (m_vlistData.count() >= 3) {
            CSchceduleAllWidgetItem *gwiup = createItemWidget(0);
            m_layout->insertWidget(0, gwiup);
            m_baseShowItem.append(gwiup);
            m_numButton->show();
            CSchceduleAllWidgetItem *firstgwi = NULL;
            for (int i = 1; i < m_vlistData.size(); ++i) {
                CSchceduleAllWidgetItem *gwi = createItemWidget(i);
                QListWidgetItem *listItem = new QListWidgetItem;
                m_gradientItemList->addItem(listItem);
                m_gradientItemList->setItemWidget(listItem, gwi);
                gwi->setItem(listItem);
                if (i == 1) firstgwi = gwi;
            }
            QColor color1;
            QColor color2;
            bool GradientFlag;
            QColor tcolor;
            QFont font;
            QPoint pos;
            firstgwi->getColor(color1, color2, GradientFlag);
            firstgwi->getText(tcolor, font, pos);
            m_numButton->setColor(color1, color2, GradientFlag);
            m_numButton->setText(tcolor, font, pos);
            m_numButton->setData(m_vlistData.count() - 1);
            m_numButton->update();
        }
    } else {
        m_numButton->hide();
        CSolarDayAllWidgetItem *solargwi = createItemWidget(m_solarDay);
        m_layout->insertWidget(0, solargwi);
        m_baseShowItem.append(solargwi);
        if (m_vlistData.count() == 1) {
            CSchceduleAllWidgetItem *gwi = createItemWidget(0);
            m_layout->insertWidget(1, gwi);
            m_baseShowItem.append(gwi);
        } else if (m_vlistData.count() >= 2) {
            m_numButton->show();
            CSchceduleAllWidgetItem *firstgwi = NULL;
            for (int i = 0; i < m_vlistData.size(); ++i) {
                CSchceduleAllWidgetItem *gwi = createItemWidget(i, false);
                QListWidgetItem *listItem = new QListWidgetItem;
                m_gradientItemList->addItem(listItem);
                m_gradientItemList->setItemWidget(listItem, gwi);
                gwi->setItem(listItem);
                if (i == 0) firstgwi = gwi;
            }
            QColor color1;
            QColor color2;
            bool GradientFlag;
            QColor tcolor;
            QFont font;
            QPoint pos;
            firstgwi->getColor(color1, color2, GradientFlag);
            firstgwi->getText(tcolor, font, pos);
            m_numButton->setColor(color1, color2, GradientFlag);
            m_numButton->setText(tcolor, font, pos);
            m_numButton->setData(m_vlistData.count());
            m_numButton->update();
        }
    }
}

CSchceduleAllWidgetItem *CSchceduleAllDayView::createItemWidget(int index, bool average)
{
    const ScheduleDtailInfo &gd = m_vlistData.at(index);
    CSchedulesColor gdcolor = CScheduleDataManage::getScheduleDataManage()->getScheduleColorByType(gd.type.ID);
    CSchceduleAllWidgetItem *gwi = new CSchceduleAllWidgetItem(nullptr, m_editType);
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
    connect(gwi, &CSchceduleAllWidgetItem::signalsDelete, this, &CSchceduleAllDayView::slotdeleteitem);
    connect(gwi, &CSchceduleAllWidgetItem::signalsEdit, this, &CSchceduleAllDayView::slotedititem);
    return gwi;
}

CSolarDayAllWidgetItem *CSchceduleAllDayView::createItemWidget(QString solarDay, bool average)
{
    CSolarDayAllWidgetItem *gwi = new CSolarDayAllWidgetItem(nullptr, m_editType);
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
        gwi->setData(solarDay);
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
        gwi->setData(solarDay);
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

void CSchceduleAllDayView::slotdeleteitem( CSchceduleAllWidgetItem *item)
{
    emit signalsUpdateShcedule(item->getData().id);
    emit signalsCotrlUpdateShcedule(m_currentDate, 1);
    for (int i = 0; i < m_vlistData.count(); i++) {
        if (m_vlistData.at(i).id == item->getData().id) {
            m_vlistData.removeAt(i);
            break;
        }
    }
    if (item != NULL) {
        int row = m_gradientItemList->row(item->getItem());
        QListWidgetItem *item11 = m_gradientItemList->takeItem(row);
        m_gradientItemList->removeItemWidget(item11);
    }
    updateDateShow();
    update();
}

void CSchceduleAllDayView::slotedititem(CSchceduleAllWidgetItem *item, int type)
{

    emit signalsUpdateShcedule(item->getData().id);
    emit signalsCotrlUpdateShcedule(m_currentDate, type);
    if (type == 0) return;
    for (int i = 0; i < m_vlistData.count(); i++) {
        if (m_vlistData.at(i).id == item->getData().id) {
            m_vlistData.removeAt(i);
            break;
        }
    }
    if (item != NULL) {
        int row = m_gradientItemList->row(item->getItem());
        QListWidgetItem *item11 = m_gradientItemList->takeItem(row);
        m_gradientItemList->removeItemWidget(item11);
    }
    updateDateShow();
    update();
}


CSolarDayAllWidgetItem::CSolarDayAllWidgetItem(QWidget *parent, int edittype)
{

}

void CSolarDayAllWidgetItem::setColor(QColor color1, QColor color2, bool GradientFlag)
{
    m_color1 = color1;
    m_color2 = color2;
    m_GradientFlag = GradientFlag;
}

void CSolarDayAllWidgetItem::setText(QColor tcolor, QFont font, QPoint pos, bool avgeflag)
{
    m_textcolor = tcolor;
    m_font = font;
    m_pos = pos;
}

void CSolarDayAllWidgetItem::setData(QString vSolarInfo)
{
    m_SolarDayInfo = vSolarInfo;
}

void CSolarDayAllWidgetItem::paintEvent(QPaintEvent *e)
{
    int labelwidth = width();
    int labelheight = height();

    QPainter painter(this);
    if (m_GradientFlag) {
        QLinearGradient linearGradient(0, 0, labelwidth, 0);
        linearGradient.setColorAt(0, m_color1);
        linearGradient.setColorAt(1, m_color2);
        QRect fillRect = QRect(2, 1, labelwidth - 2, labelheight - 1);
        //将直线开始点设为0，终点设为1，然后分段设置颜色
        painter.setRenderHints(QPainter::HighQualityAntialiasing);
        painter.setBrush(linearGradient);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(fillRect, 3, 2);

        painter.setFont(m_font);
        painter.setPen(m_textcolor);
        painter.drawText(QRect(m_pos.x(), m_pos.y(), labelwidth - m_pos.x(), labelheight), Qt::AlignLeft, m_SolarDayInfo);
    } else {
        QRect fillRect = QRect(2, 1, labelwidth - 2, labelheight - 1);
        //将直线开始点设为0，终点设为1，然后分段设置颜色
        painter.setRenderHints(QPainter::HighQualityAntialiasing);
        painter.setBrush(m_color1);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(fillRect, 3, 2);

        painter.setFont(m_font);
        painter.setPen(m_textcolor);
        painter.drawText(QRect(m_pos.x(), m_pos.y(), labelwidth - m_pos.x(), labelheight), Qt::AlignLeft, m_SolarDayInfo);
    }
}
