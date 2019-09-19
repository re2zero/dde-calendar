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
#include "schcedulesearchview.h"
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
#include "scheduledatamanage.h"
#include <DMessageBox>
#include <DPushButton>
#include <DHiDPIHelper>
#include <DPalette>
DGUI_USE_NAMESPACE
CSchceduleSearchItem::CSchceduleSearchItem( QWidget *parent /*= nullptr*/ ): DLabel(parent)
{
    //setMargin(0);
    m_editAction = new QAction(tr("Edit"), this);
    m_deleteAction = new QAction(tr("Delete"), this);
    connect(m_editAction, SIGNAL(triggered(bool)), this, SLOT(slotEdit()));
    connect(m_deleteAction, SIGNAL(triggered(bool)), this, SLOT(slotDelete()));
    //setFlat(true);

}

void CSchceduleSearchItem::setBackgroundColor(QColor color1)
{
    m_Backgroundcolor = color1;
}

void CSchceduleSearchItem::setSplitLineColor(QColor color1)
{
    m_splitlinecolor = color1;
}

void CSchceduleSearchItem::setText( QColor tcolor, QFont font)
{
    m_ttextcolor = tcolor;
    m_tfont = font;
}

void CSchceduleSearchItem::setTimeC(QColor tcolor, QFont font)
{
    m_timecolor = tcolor;
    m_timefont = font;
}

void CSchceduleSearchItem::setData( ScheduleDtailInfo vScheduleInfo )
{
    m_ScheduleInfo = vScheduleInfo;
    setToolTip(m_ScheduleInfo.titleName);
    update();
}
void CSchceduleSearchItem::slotEdit()
{
    CSchceduleDlg dlg(0, this);
    dlg.setData(m_ScheduleInfo);
    if (dlg.exec() == DDialog::Accepted) {
        emit signalsEdit(this);
    }
}

void CSchceduleSearchItem::slotDelete()
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
    //ScheduleDbManager::deleteScheduleInfoById(m_ScheduleInfo.id);
}

void CSchceduleSearchItem::paintEvent( QPaintEvent *e )
{
    int labelwidth = width();
    int labelheight = height();

    QPainter painter(this);
    QRect fillRect = QRect(0, 0, labelwidth, labelheight);
    painter.setRenderHints(QPainter::HighQualityAntialiasing);
    painter.setBrush(QBrush(m_Backgroundcolor));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(fillRect, 3, 3);

    painter.setFont(m_timefont);
    painter.setPen(m_timecolor);

    QDate begindate  = m_ScheduleInfo.beginDateTime.date();
    QDate enddate = m_ScheduleInfo.endDateTime.date();
    QString datestr;
    if (begindate  == enddate) {
        datestr = m_ScheduleInfo.beginDateTime.toString("hh:mm")
                  + "-" + m_ScheduleInfo.endDateTime.toString("hh:mm");
    } else {
        datestr = m_ScheduleInfo.beginDateTime.toString("hh:mm")
                  + "-" + m_ScheduleInfo.endDateTime.toString("hh:mm");
    }

    painter.drawText(QRect(12, 8, labelwidth * 0.455, labelheight - 16), Qt::AlignLeft, datestr);
    painter.save();
    QPen pen(m_splitlinecolor);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawLine(labelwidth * 0.455, 0, labelwidth * 0.455, labelheight);
    painter.restore();

    painter.setFont(m_tfont);
    painter.setPen(m_ttextcolor);
    int tilenameW = labelwidth * 0.533;
    QFontMetrics fm = painter.fontMetrics();
    QString str =  m_ScheduleInfo.titleName;
    if (fm.width(str) > tilenameW) {
        int widthT = fm.width(str);
        int singlecharw = widthT * 1.0 / str.count();
        int rcharcount = width() * 1.0 / singlecharw;
        QString tstr;
        for (int i = 0; i < rcharcount - 8; i++) {
            tstr.append(str.at(i));
        }
        str = tstr + "...";
    }

    painter.drawText(QRect(labelwidth * 0.455 + 9, 6, tilenameW, labelheight), Qt::AlignLeft, str);
}
void CSchceduleSearchItem::contextMenuEvent( QContextMenuEvent *event )
{
    QMenu Context(this);
    Context.addAction(m_editAction);
    Context.addAction(m_deleteAction);
    Context.exec(QCursor::pos());
}
CSchceduleSearchView::CSchceduleSearchView(QWidget *parent) : DWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    m_gradientItemList = new DListWidget(parent);
    m_gradientItemList->setAlternatingRowColors(true);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_gradientItemList);

    // set default row
    m_gradientItemList->setCurrentRow(0);
    setLayout(layout);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    m_bBackgroundcolor.setAlphaF(0.03);
}

CSchceduleSearchView::~CSchceduleSearchView()
{

}

void CSchceduleSearchView::setTheMe(int type)
{
    if (type == 0 || type == 1) {
        m_bBackgroundcolor = "#000000";
        m_bBackgroundcolor.setAlphaF(0.03);
        m_btimecolor = "#526A7F";
        m_bttextcolor = "#414D68";
        m_lBackgroundcolor = Qt::white;
        m_ltextcolor = "#001A2E";
    } else if (type == 2) {
        m_bBackgroundcolor = "#FFFFFF";
        m_bBackgroundcolor.setAlphaF(0.05);
        m_btimecolor = "#6D7C88";
        m_bttextcolor = "#C0C6D4";
        m_lBackgroundcolor = Qt::white;
        m_ltextcolor = "#C0C6D4";
    }
}

void CSchceduleSearchView::updateDateShow()
{
    //remove
    for (int i = 0; i < m_gradientItemList->count(); i++) {
        QListWidgetItem *item11 = m_gradientItemList->takeItem(i);
        m_gradientItemList->removeItemWidget(item11);
    }
    m_gradientItemList->clear();
    for (int i = 0; i < m_vlistData.size(); ++i) {
        createItemWidget(m_vlistData[i].date);
        for (int j = 0; j < m_vlistData.at(i).vData.count(); j++) {
            createItemWidget(m_vlistData.at(i).vData.at(j));
        }
    }
    if (m_gradientItemList->count() == 0) {
        QListWidgetItem *listItem = new QListWidgetItem;
        DLabel *gwi = new DLabel();
        QFont font("SourceHanSansSC-Normal");
        font.setPixelSize(20);
        gwi->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
        DPalette daypa;
        daypa.setColor(DPalette::WindowText, m_ltextcolor);
        daypa.setColor(DPalette::Window, m_lBackgroundcolor);
        gwi->setPalette(daypa);
        gwi->setFont(font);
        gwi->setText(tr("No search results"));
        gwi->setFixedSize(m_gradientItemList->width(), 450);
        listItem->setSizeHint(QSize(m_gradientItemList->width(), 450)); //每次改变Item的高度
        //listItem->setBackgroundColor(Qt::white);
        listItem->setFlags(Qt::ItemIsTristate );
        m_gradientItemList->addItem(listItem);
        m_gradientItemList->setItemWidget(listItem, gwi);
    }
}

void CSchceduleSearchView::createItemWidget(ScheduleDtailInfo info)
{
    ScheduleDtailInfo &gd = info;
    CSchedulesColor gdcolor = CScheduleDataManage::getScheduleDataManage()->getScheduleColorByType(gd.type.ID);

    CSchceduleSearchItem *gwi = new CSchceduleSearchItem();
    QFont font("SourceHanSansSC-Normal");
    font.setPixelSize(14);
    gwi->setBackgroundColor(m_bBackgroundcolor);
    //gwi->setBackgroundColor(Qt::white);
    gwi->setSplitLineColor(gdcolor.Purecolor);
    gwi->setText(m_bttextcolor, font);
    font.setPixelSize(12);
    gwi->setTimeC(m_btimecolor, font);
    gwi->setFixedSize(m_gradientItemList->width() - 20, 35);
    gwi->setData(gd);
    connect(gwi, &CSchceduleSearchItem::signalsDelete, this, &CSchceduleSearchView::slotdeleteitem);
    connect(gwi, &CSchceduleSearchItem::signalsEdit, this, &CSchceduleSearchView::slotedititem);


    QListWidgetItem *listItem = new QListWidgetItem;
    listItem->setSizeHint(QSize(m_gradientItemList->width() - 5, 36)); //每次改变Item的高度
    //listItem->setBackgroundColor(Qt::white);
    listItem->setFlags(Qt::ItemIsTristate );
    m_gradientItemList->addItem(listItem);
    m_gradientItemList->setItemWidget(listItem, gwi);

}

void CSchceduleSearchView::createItemWidget(QDate date)
{
    CSchceduleSearchDateItem *gwi = new CSchceduleSearchDateItem();
    QFont font("SourceHanSansSC-Medium");
    font.setPixelSize(16);
    gwi->setBackgroundColor(m_lBackgroundcolor);

    gwi->setText(m_ltextcolor, font);
    gwi->setFixedSize(m_gradientItemList->width() - 20, 35);
    gwi->setDate(date);
    QListWidgetItem *listItem = new QListWidgetItem;
    listItem->setSizeHint(QSize(m_gradientItemList->width() - 5, 36)); //每次改变Item的高度
    //listItem->setBackgroundColor(Qt::white);
    listItem->setFlags(Qt::ItemIsTristate );
    m_gradientItemList->addItem(listItem);
    m_gradientItemList->setItemWidget(listItem, gwi);
}

void CSchceduleSearchView::slotdeleteitem( CSchceduleSearchItem *item )
{
    emit signalsUpdateShcedule(item->getData().id);
    updateDateShow();
    update();
}

void CSchceduleSearchView::slotedititem(CSchceduleSearchItem *item)
{
    emit signalsUpdateShcedule(item->getData().id);
}

void CSchceduleSearchView::slotsetSearch(QString str)
{
    QDate date = QDate::currentDate();

    QDate bdate = date.addMonths(-6);
    QDate edate = date.addMonths(6);
    QVector<ScheduleDateRangeInfo> vScheduleInfo;
    CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfo(bdate, edate, vScheduleInfo);
    m_vlistData.clear();
    for (int i = 0; i < vScheduleInfo.count(); i++) {
        QVector<ScheduleDtailInfo> vData;
        for (int j = 0; j < vScheduleInfo.at(i).vData.count(); j++) {
            if (vScheduleInfo.at(i).vData.at(j).titleName.contains(str))
                vData.append(vScheduleInfo.at(i).vData.at(j));
        }
        if (!vData.isEmpty()) {
            ScheduleDateRangeInfo sinfo;
            sinfo.date = vScheduleInfo.at(i).date;
            sinfo.vData = vData;
            m_vlistData.append(sinfo);
        }
    }
    updateDateShow();
}


CSchceduleSearchDateItem::CSchceduleSearchDateItem(QWidget *parent)
{

}

void CSchceduleSearchDateItem::setBackgroundColor(QColor color1)
{
    m_Backgroundcolor = color1;
}

void CSchceduleSearchDateItem::setText(QColor tcolor, QFont font)
{
    m_textcolor = tcolor;
    m_font = font;
}

void CSchceduleSearchDateItem::setDate(QDate date)
{
    m_date = date;
}

void CSchceduleSearchDateItem::paintEvent(QPaintEvent *e)
{
    int labelwidth = width();
    int labelheight = height();

    QPainter painter(this);
    QRect fillRect = QRect(0, 0, labelwidth, labelheight);
    painter.setRenderHints(QPainter::HighQualityAntialiasing);
    painter.setBrush(QBrush(m_Backgroundcolor));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(fillRect, 3, 3);

    painter.setFont(m_font);
    painter.setPen(m_textcolor);

    QString datestr = QString::number(m_date.year()) + tr("Y") + QString::number(m_date.month()) + tr("M") + QString::number(m_date.day()) + tr("D");

    painter.drawText(QRect(12, 8, labelwidth, labelheight - 8), Qt::AlignLeft, datestr);
}
