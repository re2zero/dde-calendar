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
#include "schcedulectrldlg.h"
#include "myschceduleview.h"
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

void CSchceduleSearchItem::setData( ScheduleDtailInfo vScheduleInfo, QDate date)
{
    m_ScheduleInfo = vScheduleInfo;
    m_date = date;
    setToolTip(m_ScheduleInfo.titleName);
    update();
}
void CSchceduleSearchItem::slotEdit()
{
    emit signalViewtransparentFrame(1);
    CSchceduleDlg dlg(0, this);
    dlg.setData(m_ScheduleInfo);
    if (dlg.exec() == DDialog::Accepted) {
        emit signalsEdit(this);
    }
    emit signalViewtransparentFrame(0);
}

void CSchceduleSearchItem::slotDelete()
{
    emit signalViewtransparentFrame(1);
    int themetype = CScheduleDataManage::getScheduleDataManage()->getTheme();

    if (m_ScheduleInfo.rpeat == 0) {
        CSchceduleCtrlDlg msgBox;

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
            CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->deleteScheduleInfoById(m_ScheduleInfo.id);
        }
    } else {
        if (m_ScheduleInfo.RecurID == 0) {
            CSchceduleCtrlDlg msgBox;

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
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->deleteScheduleInfoById(m_ScheduleInfo.id);
            } else if (msgBox.clickButton() == yesButton) {

                ScheduleDtailInfo newschedule;
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(m_ScheduleInfo.id, newschedule);
                newschedule.ignore.append(m_ScheduleInfo.beginDateTime);
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);
            }
        } else {
            CSchceduleCtrlDlg msgBox;
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
    emit signalViewtransparentFrame(0);
    //ScheduleDbManager::deleteScheduleInfoById(m_ScheduleInfo.id);
}

void CSchceduleSearchItem::slotDoubleEvent(int type)
{
    emit signalsDelete(this);
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
    painter.drawRoundedRect(fillRect, 8, 8);

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

    painter.drawText(QRect(12, 8, 65, labelheight - 16), Qt::AlignLeft, datestr);
    painter.save();
    QPen pen(m_splitlinecolor);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawLine(82, 0, 82, labelheight);
    painter.restore();

    painter.setFont(m_tfont);
    painter.setPen(m_ttextcolor);
    int tilenameW = labelwidth - 91;
    QFontMetrics fm = painter.fontMetrics();
    QString str =  m_ScheduleInfo.titleName;
    QString tstr;
    for (int i = 0; i < str.count(); i++) {
        tstr.append(str.at(i));
        int widthT = fm.width(tstr) + 5;
        if (widthT >= tilenameW) {
            tstr.chop(1);
            break;
        }
    }
    if (tstr != str) {
        tstr = tstr + "...";
    }

    painter.drawText(QRect(91, 6, tilenameW, labelheight), Qt::AlignLeft, tstr);
}
void CSchceduleSearchItem::contextMenuEvent( QContextMenuEvent *event )
{
    DMenu Context(this);
    Context.addAction(m_editAction);
    Context.addAction(m_deleteAction);
    Context.exec(QCursor::pos());
}

void CSchceduleSearchItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit signalViewtransparentFrame(1);
    CMySchceduleView dlg(this);
    dlg.setSchedules(m_ScheduleInfo);
    connect(&dlg, &CMySchceduleView::signalsEditorDelete, this, &CSchceduleSearchItem::slotDoubleEvent);
    dlg.exec();
    disconnect(&dlg, &CMySchceduleView::signalsEditorDelete, this, &CSchceduleSearchItem::slotDoubleEvent);
    emit signalViewtransparentFrame(0);

}

void CSchceduleSearchItem::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit signalSelectDate(m_date);
    }
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
    m_gradientItemList->setSpacing(1);
    // set default row
    m_gradientItemList->setCurrentRow(0);
    setLayout(layout);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    m_bBackgroundcolor.setAlphaF(0.03);
    m_gradientItemList->setLineWidth(0);
}

CSchceduleSearchView::~CSchceduleSearchView()
{

}

void CSchceduleSearchView::setTheMe(int type)
{
    if (type == 0 || type == 1) {
        m_bBackgroundcolor = "#000000";
        m_bBackgroundcolor.setAlphaF(0.05);
        m_btimecolor = "#526A7F";
        m_bttextcolor = "#414D68";
        m_lBackgroundcolor = Qt::white;
        m_ltextcolor = "#001A2E";
    } else if (type == 2) {
        m_bBackgroundcolor = "#FFFFFF";
        m_bBackgroundcolor.setAlphaF(0.05);
        m_btimecolor = "#6D7C88";
        m_bttextcolor = "#C0C6D4";
        m_lBackgroundcolor = "#FFFFFF";
        m_lBackgroundcolor.setAlphaF(0.05);
        m_ltextcolor = "#C0C6D4";
    }
    updateDateShow();
}

void CSchceduleSearchView::clearSearch()
{
    m_vlistData.clear();
    //remove
    for (int i = 0; i < m_gradientItemList->count(); i++) {
        QListWidgetItem *item11 = m_gradientItemList->takeItem(i);
        m_gradientItemList->removeItemWidget(item11);
    }
    m_gradientItemList->clear();
}

void CSchceduleSearchView::updateDateShow()
{
    m_currentItem = NULL;
    //remove
    for (int i = 0; i < m_gradientItemList->count(); i++) {
        QListWidgetItem *item11 = m_gradientItemList->takeItem(i);
        m_gradientItemList->removeItemWidget(item11);
    }
    m_gradientItemList->clear();
    m_labellist.clear();
    //找最近日程
    bool flag = false;
    QDate tcurrentdata = QDate::currentDate();
    for (int i = 0; i < m_vlistData.size(); ++i) {
        if (m_vlistData[i].date == tcurrentdata) {
            if (!m_vlistData.at(i).vData.isEmpty()) {
                flag = true;
                break;
            }
        }
    }
    if (!flag && !m_vlistData.isEmpty()) {
        QDate topdate = tcurrentdata;
        QDate mindate = topdate;
        for (int i = 0; i < m_vlistData.size(); ++i) {
            if (m_vlistData[i].date > mindate) {
                mindate = m_vlistData[i].date;
            }
        }
        while (!flag) {
            topdate = topdate.addDays(-1);
            for (int i = 0; i < m_vlistData.size(); ++i) {
                if (m_vlistData[i].date == topdate) {
                    if (!m_vlistData.at(i).vData.isEmpty()) {
                        flag = true;
                        break;
                    }
                }
            }
            if (topdate < mindate) break;
        }
        tcurrentdata = topdate;
    }

    for (int i = 0; i < m_vlistData.size(); ++i) {
        QListWidgetItem *titem = createItemWidget(m_vlistData[i].date);
        if (m_vlistData[i].date == tcurrentdata) {
            m_currentItem = titem;
        }
        for (int j = 0; j < m_vlistData.at(i).vData.count(); j++) {
            createItemWidget(m_vlistData.at(i).vData.at(j), m_vlistData[i].date);
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
        m_labellist.append(gwi);
    }
    if (m_currentItem != NULL) {
        m_gradientItemList->scrollToItem(m_currentItem, QAbstractItemView::PositionAtTop);
    }
}

void CSchceduleSearchView::createItemWidget(ScheduleDtailInfo info, QDate date)
{
    ScheduleDtailInfo &gd = info;
    CSchedulesColor gdcolor = CScheduleDataManage::getScheduleDataManage()->getScheduleColorByType(gd.type.ID);

    CSchceduleSearchItem *gwi = new CSchceduleSearchItem();
    QFont font("SourceHanSansSC-Normal");
    font.setPixelSize(14);
    gwi->setBackgroundColor(m_bBackgroundcolor);
    //gwi->setBackgroundColor(Qt::white);
    QColor scolor = gdcolor.Purecolor;
    scolor.setAlphaF(1.0);
    gwi->setSplitLineColor(scolor);
    gwi->setText(m_bttextcolor, font);
    font.setPixelSize(12);
    gwi->setTimeC(m_btimecolor, font);
    gwi->setFixedSize(m_gradientItemList->width() - 20, 35);
    gwi->setData(gd, date);
    connect(gwi, &CSchceduleSearchItem::signalsDelete, this, &CSchceduleSearchView::slotdeleteitem);
    connect(gwi, &CSchceduleSearchItem::signalsEdit, this, &CSchceduleSearchView::slotedititem);
    connect(gwi, &CSchceduleSearchItem::signalSelectDate, this, &CSchceduleSearchView::slotSelectDate);
    connect(gwi, &CSchceduleSearchItem::signalViewtransparentFrame, this, &CSchceduleSearchView::signalViewtransparentFrame);

    //connect(gwi, SIGNAL(signalsDelete(QDate )), this, SIGNAL(signalDate(QDate )));

    QListWidgetItem *listItem = new QListWidgetItem;
    listItem->setSizeHint(QSize(m_gradientItemList->width() - 5, 36)); //每次改变Item的高度
    //listItem->setBackgroundColor(Qt::white);
    listItem->setFlags(Qt::ItemIsTristate );
    m_gradientItemList->addItem(listItem);
    m_gradientItemList->setItemWidget(listItem, gwi);
    m_labellist.append(gwi);
}

QListWidgetItem *CSchceduleSearchView::createItemWidget(QDate date)
{
    CSchceduleSearchDateItem *gwi = new CSchceduleSearchDateItem();
    QFont font("SourceHanSansSC");
    font.setWeight(QFont::Medium);
    font.setPixelSize(16);
    gwi->setBackgroundColor(m_lBackgroundcolor);
    gwi->setText(m_ltextcolor, font);
    if (date == QDate::currentDate()) {
        gwi->setText("#F85566", font);
    }
    gwi->setFixedSize(m_gradientItemList->width() - 20, 35);
    gwi->setDate(date);
    QListWidgetItem *listItem = new QListWidgetItem;
    listItem->setSizeHint(QSize(m_gradientItemList->width() - 5, 36)); //每次改变Item的高度
    //listItem->setBackgroundColor(Qt::white);
    listItem->setFlags(Qt::ItemIsTristate );
    m_gradientItemList->addItem(listItem);
    m_gradientItemList->setItemWidget(listItem, gwi);
    m_labellist.append(gwi);
    return listItem;
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
    QDateTime date = QDateTime::currentDateTime();

    QDateTime bdate = date.addMonths(-6);
    QDateTime edate = date.addMonths(6);
    QVector<ScheduleDateRangeInfo> vScheduleInfo;
    m_vlistData.clear();
#if 0
    CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfo(bdate.date(), edate.date(), vScheduleInfo);
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
#else
    CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->queryScheduleInfo(str, bdate, edate, vScheduleInfo);
    m_vlistData = vScheduleInfo;
#endif
    updateDateShow();
}

void CSchceduleSearchView::slotSelectDate(QDate date)
{
    emit signalDate(date);
}

void CSchceduleSearchView::resizeEvent(QResizeEvent *event)
{
    for (int i = 0; i < m_gradientItemList->count(); i++) {
        QListWidgetItem *item11 = m_gradientItemList->item(i);
        item11->setSizeHint(QSize(m_gradientItemList->width() - 5, 36)); //每次改变Item的高度
    }
    //remove
    for (int i = 0; i < m_labellist.count(); i++) {
        m_labellist.at(i)->setFixedSize(m_gradientItemList->width() - 20, 35);
        m_labellist.at(i)->update();
    }
    if (m_gradientItemList->count() == 1) {
        QListWidgetItem *item11 = m_gradientItemList->item(0);
        item11->setSizeHint(QSize(m_gradientItemList->width(), height() * 0.7978)); //每次改变Item的高度
        m_labellist.at(0)->setFixedSize(m_gradientItemList->width(), height() * 0.7978);
        m_labellist.at(0)->update();
    }
    DWidget::resizeEvent(event);
}


CSchceduleSearchDateItem::CSchceduleSearchDateItem(QWidget *parent): DLabel(parent)
{
    setAutoFillBackground(true);
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
