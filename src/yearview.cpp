
/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
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

#include "calendardbus.h"

#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QEvent>
#include <QDebug>
#include <QMessageBox>
#include <QTime>
#include <QQueue>
#include "yearview.h"
#include <QSpacerItem>
#include <DPalette>
#include <QLocale>
DGUI_USE_NAMESPACE
CYearView::CYearView(QWidget *parent) : DFrame(parent)
{
    m_dayNumFont.setFamily("Helvetica");
    m_dayNumFont.setPixelSize(12);
    //m_dayNumFont.setWeight(QFont::Light);

    //setStyleSheet("QWidget { background: rgba(0, 0, 0, 0) }");

    //add separator line
    m_currentMouth = new DLabel();
    m_currentMouth->setFixedHeight(24);
    //m_currentMouth->setStyleSheet("border: 1px solid rgba(0, 0, 0, 0.05);");

    m_momthFont.setFamily("SourceHanSansSC");
    m_momthFont.setWeight(QFont::Medium);
    m_momthFont.setPixelSize(16);
    m_currentMouth->setFont(m_momthFont);
    DPalette Lunadpa = m_currentMouth->palette();
    Lunadpa.setColor(DPalette::WindowText, QColor("#CF0059"));
    m_currentMouth->setPalette(Lunadpa);
    QHBoxLayout *separatorLineLayout = new QHBoxLayout;
    separatorLineLayout->setMargin(0);
    separatorLineLayout->setSpacing(0);
    separatorLineLayout->setContentsMargins(0, 0, 0, 0);
    //separatorLineLayout->addSpacing(13);
    separatorLineLayout->addWidget(m_currentMouth);
    separatorLineLayout->addStretch();
    // QSpacerItem *t_spaceitem = new QSpacerItem(30, 32, QSizePolicy::Expanding, QSizePolicy::Fixed);
    //separatorLineLayout->addSpacerItem(t_spaceitem);

    // cells grid
    m_gridLayout = new QGridLayout;
    m_gridLayout->setMargin(0);
    m_gridLayout->setSpacing(0);
    m_gridLayout->setHorizontalSpacing(6);
    m_gridLayout->setVerticalSpacing(3);
    for (int r = 0; r != 6; ++r) {
        for (int c = 0; c != 7; ++c) {
            QWidget *cell = new QWidget;
            cell->setFixedSize(cellwidth, cellheight);
            cell->installEventFilter(this);
            cell->setFocusPolicy(Qt::ClickFocus);
            m_gridLayout->addWidget(cell, r, c);
            m_cellList.append(cell);
        }
    }
    m_hhLayout = new QVBoxLayout;
    m_hhLayout->addLayout(separatorLineLayout);
    m_hhLayout->addLayout(m_gridLayout);
    m_hhLayout->setMargin(0);
    m_hhLayout->setSpacing(0);
    m_hhLayout->setContentsMargins(13, 5, 10, 10);
    m_gridWidget = new DFrame(this);
    m_gridWidget->setContentsMargins(0, 0, 0, 0);
    m_gridWidget->setLayout(m_hhLayout);
    //m_gridWidget->setFixedSize(176, 113);
    DPalette anipa = m_gridWidget->palette();
    anipa.setColor(DPalette::Background, Qt::white);
    m_gridWidget->setAutoFillBackground(true);
    m_gridWidget->setPalette(anipa);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    // mainLayout->addWidget(m_weekIndicator, 0, Qt::AlignHCenter);
    mainLayout->addWidget(m_gridWidget, 0,  Qt::AlignHCenter);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    setLayout(mainLayout);
    setAutoFillBackground(true);
    connect(this, &CYearView::dateSelected, this, &CYearView::handleCurrentDateChanged);
    //setFixedSize(DDEYearCalendar::Y_MWindowWidth, DDEYearCalendar::Y_MWindowHeight);
    setFrameRounded(true);
}
void CYearView::handleCurrentDateChanged(const QDate date, const CaLunarDayInfo &detail)
{
    return;
    Q_UNUSED(detail);


    if (date != m_currentDate) {
        setCurrentDate(date);
    }
}

void CYearView::setFirstWeekday(int weekday)
{
    m_firstWeekDay = weekday;
    updateDate();
}


void CYearView::updateSelectState()
{
    m_selectedCell = -1;
    for (int i(0); i != 42; ++i) {
        m_cellList.at(i)->update();
    }
}

void CYearView::setTheMe(int type)
{
    if (type == 0 || type == 1) {
        DPalette bpa = m_gridWidget->palette();
        bpa.setColor(DPalette::Background, Qt::white);
        setPalette(bpa);
        setBackgroundRole(DPalette::Background);

        DPalette anipa = m_gridWidget->palette();
        anipa.setColor(DPalette::Background, Qt::white);
        m_gridWidget->setPalette(anipa);
        m_gridWidget->setBackgroundRole(DPalette::Background);

        DPalette Lunadpa = m_currentMouth->palette();
        Lunadpa.setColor(DPalette::WindowText, QColor("#CF0059"));
        m_currentMouth->setPalette(Lunadpa);
        m_currentMouth->setForegroundRole(DPalette::WindowText);

        m_topBorderColor = Qt::red;
        m_backgroundCircleColor = "#0081FF";

        m_defaultTextColor = Qt::black;
        m_currentDayTextColor = "#0081FF";
        m_weekendsTextColor = Qt::black;
        m_selectedTextColor = Qt::white;
        m_festivalTextColor = Qt::black;
        m_notCurrentTextColor = "#b2b2b2";

    } else if (type == 2) {
        DPalette anipa = m_gridWidget->palette();
        QColor framecolor("#FFFFFF");
        //framecolor.setAlphaF(0.15);
        framecolor = "#414141";
        framecolor.setAlphaF(0.3);
        anipa.setColor(DPalette::Background, framecolor);
        m_gridWidget->setPalette(anipa);
        m_gridWidget->setBackgroundRole(DPalette::Background);

        DPalette bpa = m_gridWidget->palette();
        bpa.setColor(DPalette::Background, framecolor);
        setPalette(bpa);
        setBackgroundRole(DPalette::Background);

        DPalette Lunadpa = m_currentMouth->palette();
        Lunadpa.setColor(DPalette::WindowText, QColor("#BF1D63"));
        m_currentMouth->setPalette(Lunadpa);
        m_currentMouth->setForegroundRole(DPalette::WindowText);

        m_topBorderColor = Qt::red;
        m_backgroundCircleColor = "#0059D2";

        m_defaultTextColor = "#C0C6D4";
        m_currentDayTextColor = "#0059D2";
        m_weekendsTextColor = Qt::black;
        m_selectedTextColor = Qt::white;
        m_festivalTextColor = Qt::black;
        m_notCurrentTextColor = "#C0C6D4";
        m_notCurrentTextColor.setAlphaF(0.5);
    }
}

void CYearView::setCurrentDate(const QDate date, int type)
{
    qDebug() << "set current date " << date;

    //if (date == m_currentDate) {
    //   return;
    // }

    m_currentDate = date;
    QLocale locale;
    //QString monthName(int month, QLocale::FormatType type = LongFormat)
    m_currentMouth->setText(locale.monthName(date.month(), QLocale::ShortFormat));
    //m_currentMouth->setText(QString::number(date.month()) + tr("Mon"));
    updateDate();
    if (type == 1)
        setSelectedCell(getDateIndex(date));
    emit signalcurrentDateChanged(date);
}

void CYearView::setCellSelectable(bool selectable)
{
    if (selectable == m_cellSelectable)
        return;
    m_cellSelectable = selectable;
}

int CYearView::getDateIndex(const QDate &date) const
{
    for (int i = 0; i != 42; ++i)
        if (m_days[i] == date)
            return i;

    return 0;
}

bool CYearView::eventFilter(QObject *o, QEvent *e)
{
    QWidget *cell = qobject_cast<QWidget *>(o);

    if (cell && m_cellList.contains(cell)) {
        if (e->type() == QEvent::Paint) {
            paintCell(cell);
        } else if (e->type() == QEvent::MouseButtonPress) {
            cellClicked(cell);
        }
    }

    return false;
}

void CYearView::updateDate()
{
    const QDate firstDay(m_currentDate.year(), m_currentDate.month(), 1);
    const int day = (firstDay.dayOfWeek() + m_firstWeekDay) % 7;
    const int currentIndex = day + m_currentDate.day() - 1;

    if (currentIndex < 0) {
        return;
    }

    for (int i(0); i != 42; ++i) {
        m_days[i] = firstDay.addDays(i - day);
        m_cellList.at(i)->update();
    }

    //setSelectedCell(currentIndex);
    update();
}
const QString CYearView::getCellDayNum(int pos)
{
    return QString::number(m_days[pos].day());
}

const QDate CYearView::getCellDate(int pos)
{
    return m_days[pos];
}
void CYearView::paintCell(QWidget *cell)
{
    const QRect rect(0, 0, cell->width(), cell->height());

    const int pos = m_cellList.indexOf(cell);
    const bool isSelectedCell = pos == m_selectedCell;
    const bool isCurrentDay = getCellDate(pos) == QDate::currentDate() && getCellDate(pos).month() == m_currentDate.month();

    QPainter painter(cell);

//    painter.drawRoundedRect(cell->rect(), 4, 4);

    // draw selected cell background circle
    if (isSelectedCell) {
        int hh = 0;
        QRect fillRect;
        if (cell->width() > cell->height()) {
            hh = cell->height();
            fillRect = QRect((cell->width() - hh) / 2.0, 0, hh, hh);
        } else {
            hh = cell->width();
            fillRect = QRect(0, (cell->height() - hh) / 2, hh, hh);
        }

        painter.setRenderHints(QPainter::HighQualityAntialiasing);
        painter.setBrush(QBrush(m_backgroundCircleColor));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(fillRect);
    }

    painter.setPen(Qt::SolidLine);

    const QString dayNum = getCellDayNum(pos);

    // draw text of day
    if (isSelectedCell) {
        painter.setPen(m_selectedTextColor);
    } else if (isCurrentDay) {
        painter.setPen(m_currentDayTextColor);
    } else {
        if (m_currentDate.month() == getCellDate(pos).month())
            painter.setPen(m_defaultTextColor);
        else
            painter.setPen(m_notCurrentTextColor);
    }

//    painter.drawRect(rect);
    QRect test;
    painter.setFont(m_dayNumFont);

    painter.drawText(rect, Qt::AlignCenter, dayNum, &test);

    painter.end();
}

void CYearView::cellClicked(QWidget *cell)
{
    if (!m_cellSelectable)
        return;

    const int pos = m_cellList.indexOf(cell);
    if (pos == -1)
        return;

    setSelectedCell(pos);

    // my gift eggs
    static int gift = 0;
    if (m_days[pos] == QDate(1993, 7, 28))
        if (++gift == 10)
            QMessageBox::about(this, "LinuxDeepin", "by shibowen <sbw@sbw.so> :P");
}

void CYearView::setSelectedCell(int index)
{
    if (m_selectedCell == index)
        return;

    const int prevPos = m_selectedCell;
    m_selectedCell = index;

    updateDate();
    //m_cellList.at(prevPos)->update();
    //m_cellList.at(index)->update();
    emit singanleActiveW(this);
    emit signalcurrentDateChanged(m_days[index]);
}

void CYearView::resizeEvent(QResizeEvent *event)
{
    cellwidth = width() * 0.099 + 0.5;
    cellheight = height() * 0.1257 + 0.5;
    m_gridLayout->setHorizontalSpacing(width() * 0.0297 + 0.5);
    m_gridLayout->setVerticalSpacing(height() * 0.0034 + 0.5);
    int leftmagin = width() * 0.06435 + 0.5;
    int rightmagin = leftmagin;
    int topmagin = height() * 0.02955 + 0.5;
    int buttonmagin = height() * 0.044 + 0.5;
    m_hhLayout->setContentsMargins(leftmagin, topmagin, rightmagin, buttonmagin);
    m_dayNumFont.setPixelSize(12 + (height() - 159) / 22.33);
    m_momthFont.setPixelSize(16 + (height() - 159) / 16.75);
    m_currentMouth->setFixedHeight(24 + (height() - 159) / 12);
    for (int i(0); i != 42; ++i) {
        m_cellList.at(i)->setFixedSize(cellwidth, cellheight);
    }
    QWidget::resizeEvent(event);
}
