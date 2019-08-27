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

void CSchceduleSearchItem::setText( QColor tcolor, QFont font)
{
    m_textcolor = tcolor;
    m_font = font;
}

void CSchceduleSearchItem::setData( ScheduleInfo vScheduleInfo )
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
        ScheduleInfo info = dlg.getData();
        info.id = m_ScheduleInfo.id;
        ScheduleDbManager::updateScheduleInfo(info);
        emit signalsEdit(this);
    }
}

void CSchceduleSearchItem::slotDelete()
{
    emit signalsDelete(this);
    ScheduleDbManager::deleteScheduleInfoById(m_ScheduleInfo.id);
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

    painter.setFont(m_font);
    painter.setPen(m_textcolor);

    QDate begindate  = m_ScheduleInfo.beginDateTime.date();
    QDate enddate = m_ScheduleInfo.endDateTime.date();
    QString datestr;
    if (begindate  == enddate) {
        datestr = begindate.toString("yyyy-MM-d") + " " + m_ScheduleInfo.beginDateTime.toString("hh:mm")
                  + "-" + m_ScheduleInfo.endDateTime.toString("hh:mm");
    } else {
        datestr = begindate.toString("yyyy-MM-d") + " " + m_ScheduleInfo.beginDateTime.toString("hh:mm")
                  + "-" + enddate.toString("yyyy-MM-d") + " " + m_ScheduleInfo.endDateTime.toString("hh:mm");
    }

    painter.drawText(QRect(2, 2, labelwidth, labelheight / 2), Qt::AlignLeft, datestr);

    QFontMetrics fm = painter.fontMetrics();
    QString str =  m_ScheduleInfo.titleName;
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

    painter.drawText(QRect(2, 2 + labelheight / 2, labelwidth, labelheight / 2), Qt::AlignLeft, str);
}
void CSchceduleSearchItem::contextMenuEvent( QContextMenuEvent *event )
{
    QMenu Context(this);
    Context.addAction(m_editAction);
    Context.addAction(m_deleteAction);
    Context.exec(QCursor::pos());
}
CSchceduleSearchView::CSchceduleSearchView(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    m_gradientItemList = new QListWidget(parent);
    m_gradientItemList->setAlternatingRowColors(true);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_gradientItemList);

    // set default row
    m_gradientItemList->setCurrentRow(0);
    setLayout(layout);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

CSchceduleSearchView::~CSchceduleSearchView()
{

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
        CSchceduleSearchItem *gwi = createItemWidget(i, true);
        QListWidgetItem *listItem = new QListWidgetItem;
        listItem->setSizeHint(QSize(m_gradientItemList->width() - 5, 30)); //每次改变Item的高度
        //listItem->setBackgroundColor(Qt::white);
        listItem->setFlags(Qt::ItemIsTristate );
        m_gradientItemList->addItem(listItem);
        m_gradientItemList->setItemWidget(listItem, gwi);
    }
    if (m_gradientItemList->count() == 0) {
        QListWidgetItem *listItem = new QListWidgetItem;
        DLabel *gwi = new DLabel();
        QFont font("PingFangSC-Light");
        font.setPixelSize(10);
        gwi->setAlignment(Qt::AlignCenter);
        QPalette daypa;
        daypa.setColor(QPalette::WindowText,  Qt::black);
        daypa.setColor(QPalette::Window, Qt::white);
        gwi->setPalette(daypa);
        gwi->setFont(font);
        gwi->setText(tr("No search results"));
        gwi->setFixedSize(m_gradientItemList->width() - 5, 80);
        listItem->setSizeHint(QSize(m_gradientItemList->width() - 5, 80)); //每次改变Item的高度
        //listItem->setBackgroundColor(Qt::white);
        listItem->setFlags(Qt::ItemIsTristate );
        m_gradientItemList->addItem(listItem);
        m_gradientItemList->setItemWidget(listItem, gwi);
    }
}

CSchceduleSearchItem *CSchceduleSearchView::createItemWidget(int index, bool average)
{
    const ScheduleInfo &gd = m_vlistData.at(index);
    CSchceduleSearchItem *gwi = new CSchceduleSearchItem();
    QFont font("PingFangSC-Light");
    font.setPixelSize(10);
    gwi->setBackgroundColor(ScheduleDbManager::getTypeColor(gd.infotype));

    gwi->setText(QColor("#000000"), font);
    gwi->setFixedSize(m_gradientItemList->width() - 5, 30);
    gwi->setData(gd);
    connect(gwi, &CSchceduleSearchItem::signalsDelete, this, &CSchceduleSearchView::slotdeleteitem);
    connect(gwi, &CSchceduleSearchItem::signalsEdit, this, &CSchceduleSearchView::slotedititem);
    return gwi;
}

void CSchceduleSearchView::slotdeleteitem( CSchceduleSearchItem *item )
{
    emit signalsUpdateShcedule(item->getData().id);
    for (int i = 0; i < m_vlistData.count(); i++) {
        if (m_vlistData.at(i).id == item->getData().id) {
            m_vlistData.removeAt(i);
            break;
        }
    }
    updateDateShow();
    update();
}

void CSchceduleSearchView::slotedititem(CSchceduleSearchItem *item)
{
    emit signalsUpdateShcedule(item->getData().id);
}

void CSchceduleSearchView::slotsetSearch(QString str)
{
    QVector<ScheduleInfo> vScheduleInfo = ScheduleDbManager::getAllScheduleInfo();
    m_vlistData.clear();
    for (int i = 0; i < vScheduleInfo.count(); i++) {
        if (vScheduleInfo.at(i).titleName.contains(str))
            m_vlistData.append(vScheduleInfo.at(i));
    }
    updateDateShow();
}

