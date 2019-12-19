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
#include "yearschceduleview.h"
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
CYearSchceduleItem::CYearSchceduleItem( QWidget *parent /*= nullptr*/ ): DLabel(parent)
{

}

void CYearSchceduleItem::setBackgroundColor(QColor color1)
{
    m_Backgroundcolor = color1;
}

void CYearSchceduleItem::setStateColor(QColor color1)
{
    m_Statecolor = color1;
}

void CYearSchceduleItem::setText( QColor tcolor, QFont font)
{
    m_ttextcolor = tcolor;
    m_tfont = font;
}

void CYearSchceduleItem::setTimeC(QColor tcolor, QFont font)
{
    m_timecolor = tcolor;
    m_timefont = font;
}

void CYearSchceduleItem::setData( ScheduleDtailInfo vScheduleInfo)
{
    m_ScheduleInfo = vScheduleInfo;
    setToolTip(m_ScheduleInfo.titleName);
    update();
}

void CYearSchceduleItem::paintEvent( QPaintEvent *e )
{
    int labelwidth = width();
    int labelheight = height();

    QPainter painter(this);
    QRect fillRect = QRect(0, 0, labelwidth, labelheight);
    painter.setRenderHints(QPainter::HighQualityAntialiasing);
    QColor bcolor = m_Backgroundcolor;
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    painter.setBrush(QBrush(bcolor));
    painter.setPen(Qt::NoPen);
    painter.drawRect(fillRect);
    painter.restore();
    //圆点
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    painter.setBrush(QBrush(m_Statecolor));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(QRect(10, (labelheight - 8) / 2, 8, 8));
    painter.restore();


    QString str;
    //左边文字
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    painter.setPen(m_ttextcolor);
    painter.setFont(m_tfont);
    QFontMetrics fm = painter.fontMetrics();
    QString tStitlename = m_ScheduleInfo.titleName;
    tStitlename.replace("\n", "");
    str = tStitlename;
    int tilenameW = labelwidth - 90;
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

    painter.drawText(QRect(28, 0, tilenameW, labelheight), Qt::AlignLeft | Qt::AlignVCenter, tstr);
    painter.restore();
    //右边时间
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    painter.setPen(m_timecolor);
    painter.setFont(m_timefont);

    if (m_ScheduleInfo.allday) {
        str = tr("ALL DaY");
    } else {
        str = m_ScheduleInfo.beginDateTime.time().toString("ap h") + tr("h");
    }
    QFontMetrics fm2 = painter.fontMetrics();
    painter.drawText(QRect(labelwidth - 60, 0, 50, labelheight), Qt::AlignRight | Qt::AlignVCenter, str);
    painter.restore();
}

CYearSchceduleView::CYearSchceduleView(QWidget *parent) : DFrame(parent)
{
    setContentsMargins(5, 5, 5, 5);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    m_gradientItemList = new DListWidget(parent);
    // m_gradientItemList->setAlternatingRowColors(true);
    //layout->setContentsMargins(10, 10, 10, 10);
    layout->addWidget(m_gradientItemList, 0, Qt::AlignCenter);
    //m_gradientItemList->setSpacing(1);
    // set default row
    m_gradientItemList->setCurrentRow(0);
    setLayout(layout);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    m_bBackgroundcolor.setAlphaF(0.03);
    m_gradientItemList->setLineWidth(0);
    m_labellist.clear();
    setFrameRounded(true);
    setAttribute(Qt::WA_TranslucentBackground);
}

CYearSchceduleView::~CYearSchceduleView()
{

}

void CYearSchceduleView::setSoloDay(QString soloday)
{
    m_soloDay = soloday;
}

void CYearSchceduleView::setData(QVector<ScheduleDtailInfo> &vListData)
{
    m_vlistData = vListData;
}

void CYearSchceduleView::clearData()
{
    m_vlistData.clear();
    m_soloDay.clear();
    //remove
    for (int i = 0; i < m_gradientItemList->count(); i++) {
        QListWidgetItem *item11 = m_gradientItemList->takeItem(i);
        m_gradientItemList->removeItemWidget(item11);
    }
    m_labellist.clear();
    m_gradientItemList->clear();
}

void CYearSchceduleView::showWindow()
{
    if (m_soloDay.isEmpty() && m_vlistData.isEmpty()) {
        setFixedSize(120, 70);
        m_gradientItemList->setFixedSize(100, 50);
    } else {
        setFixedSize(230, 170);
        m_gradientItemList->setFixedSize(210, 150);
    }
    //show();
    updateDateShow();
}

void CYearSchceduleView::setTheMe(int type)
{
    if (type == 0 || type == 1) {
        m_bBackgroundcolor = "#FFFFFF";
        m_bBackgroundcolor.setAlphaF(0.2);
        m_btimecolor = "#414D68";
        m_btimecolor.setAlphaF(0.7);
        m_bttextcolor = "#414D68";
        m_lBackgroundcolor = "#EBEBEB";
        m_lBackgroundcolor.setAlphaF(0.3);
        m_ltextcolor = "#001A2E";
        m_solocolor = "#FF7272";
    } else if (type == 2) {
        m_bBackgroundcolor = "#FFFFFF";
        m_bBackgroundcolor.setAlphaF(0.0);
        m_btimecolor = "#C0C6D4";
        m_btimecolor.setAlphaF(0.7);
        m_bttextcolor = "#C0C6D4";
        m_lBackgroundcolor = "#191919";
        m_lBackgroundcolor.setAlphaF(0.05);
        m_ltextcolor = "#C0C6D4";
        m_solocolor = "#FF7272";
        m_solocolor.setAlphaF(0.8);
    }
}

void CYearSchceduleView::updateDateShow()
{
    //remove
    for (int i = 0; i < m_gradientItemList->count(); i++) {
        QListWidgetItem *item11 = m_gradientItemList->takeItem(i);
        m_gradientItemList->removeItemWidget(item11);
    }
    m_gradientItemList->clear();
    m_labellist.clear();

    if (!m_soloDay.isEmpty()) {
        ScheduleDtailInfo info;
        info.titleName = m_soloDay;
        info.allday = true;
        createItemWidget(info, 1);
    }
    int sviewNum = 0;
    if (!m_soloDay.isEmpty()) {
        if (m_vlistData.size() > 5) {
            sviewNum = 5;
        } else {
            sviewNum = m_vlistData.size();
        }
    } else {
        if (m_vlistData.size() > 6) {
            sviewNum = 6;
        } else {
            sviewNum = m_vlistData.size();
        }
    }

    for (int i = 0; i < sviewNum; ++i) {
        createItemWidget(m_vlistData.at(i), 0);
    }

    if (m_gradientItemList->count() == 0) {
        QListWidgetItem *listItem = new QListWidgetItem;
        DLabel *gwi = new DLabel();
        QFont font("SourceHanSansSC-Normal");
        font.setPixelSize(12);
        gwi->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
        DPalette daypa = gwi->palette();
        QColor textcolor = DPalette::ToolTipText;
        int themtype = CScheduleDataManage::getScheduleDataManage()->getTheme();
        if (themtype == 2) {
            textcolor = "#C0C6D4";
            textcolor.setAlphaF(0.4);
        } else {
            textcolor = "#000000";
            textcolor.setAlphaF(0.3);
        }
        daypa.setColor(DPalette::WindowText, textcolor);
        daypa.setColor(DPalette::Window, m_lBackgroundcolor);
        gwi->setPalette(daypa);
        gwi->setForegroundRole(DPalette::WindowText);
        //gwi->setForegroundRole(DPalette::Window);
        gwi->setFont(font);
        gwi->setText(tr("No Schcedule"));
        gwi->setFixedSize(m_gradientItemList->width(), 48);
        listItem->setSizeHint(QSize(m_gradientItemList->width(), 49)); //每次改变Item的高度
        //listItem->setBackgroundColor(Qt::white);
        listItem->setFlags(Qt::ItemIsTristate );
        m_gradientItemList->addItem(listItem);
        m_gradientItemList->setItemWidget(listItem, gwi);
        m_labellist.append(gwi);
    }
}

void CYearSchceduleView::createItemWidget(ScheduleDtailInfo info, int type)
{
    ScheduleDtailInfo &gd = info;
    CSchedulesColor gdcolor = CScheduleDataManage::getScheduleDataManage()->getScheduleColorByType(gd.type.ID);

    CYearSchceduleItem *gwi = new CYearSchceduleItem();
    QFont font("SourceHanSansSC");
    font.setWeight(QFont::Medium);
    font.setPixelSize(14);
    gwi->setBackgroundColor(m_bBackgroundcolor);
    //gwi->setBackgroundColor(Qt::white);
    QColor scolor = gdcolor.Purecolor;
    scolor.setAlphaF(1.0);
    if (type == 0)
        gwi->setStateColor(gdcolor.splitColor);
    else {
        gwi->setStateColor(m_solocolor);
    }
    gwi->setText(m_bttextcolor, font);
    font.setPixelSize(12);
    gwi->setTimeC(m_btimecolor, font);
    gwi->setFixedSize(m_gradientItemList->width() - 3, 28);
    gwi->setData(gd);

    QListWidgetItem *listItem = new QListWidgetItem;
    listItem->setSizeHint(QSize(m_gradientItemList->width() - 2, 29)); //每次改变Item的高度
    //listItem->setBackgroundColor(Qt::white);
    listItem->setFlags(Qt::ItemIsTristate );
    m_gradientItemList->addItem(listItem);
    m_gradientItemList->setItemWidget(listItem, gwi);
    m_labellist.append(gwi);
}
