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

#include "weekview.h"
#include <QHBoxLayout>
#include "constants.h"
#include <QPainter>
#include <QBrush>
#include <QEvent>
#include <QMessageBox>
#include <QWheelEvent>
CWeekView::CWeekView(QWidget *parent) : QWidget(parent)
{
    m_dayNumFont.setFamily("Avenir-Light");
    m_dayNumFont.setPixelSize(16);
    m_dayNumFont.setWeight(QFont::Light);


    // cells grid
    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->setMargin(0);
    hboxLayout->setSpacing(0);
    //hboxLayout->setContentsMargins(4, 0, 4, 0);
    for (int c = 0; c != 10; ++c) {
        QWidget *cell = new QWidget;
        cell->setFixedSize(DDEWeekCalendar::WWeekCellWidth, DDEWeekCalendar::WWeekCellHeight);
        cell->installEventFilter(this);
        cell->setFocusPolicy(Qt::ClickFocus);
        hboxLayout->addWidget(cell);
        m_cellList.append(cell);
    }
    setLayout(hboxLayout);
}

CWeekView::~CWeekView()
{
    for (int i = 0; i < 10; i++) {
        m_cellList.at(i)->removeEventFilter(this);
        delete m_cellList.at(i);
    }
    m_cellList.clear();
}

void CWeekView::setFirstWeekDay(int weekday)
{
    m_firstWeekDay = weekday;
    updateDate();
}

void CWeekView::setCurrentDate(const QDate date)
{
    m_selectDate = date;
    m_selectedCell = 0;
    updateDate();
    //setSelectedCell(m_selectDate.month() - 1);
    update();
}

void CWeekView::setTheMe(int type)
{
    if (type == 0 || type == 1) {
        m_defaultTextColor = Qt::black;
        m_backgrounddefaultColor = Qt::white;
        m_currentDayTextColor = Qt::white;
        m_backgroundcurrentDayColor = "#0081FF";
        m_fillColor = "#FFFFFF";
    } else if (type == 2) {
        m_defaultTextColor = "#C0C6D4";
        m_backgrounddefaultColor = "#FFFFFF";
        m_backgrounddefaultColor.setAlphaF(0.05);
        m_currentDayTextColor = "#B8D3FF";
        m_backgroundcurrentDayColor = "#0081FF";
        m_fillColor = "#000000";
        m_fillColor.setAlphaF(0.05);
    }
}
void CWeekView::slotprev()
{
    if (m_selectDate.year() > 1900) {
        QDate date = m_selectDate.addDays(-7);
        setCurrentDate(date);
    } else {
        QMessageBox::information(this, tr("infomation"), tr("Year less than 1900!"));
    }
}

void CWeekView::slotnext()
{
    QDate date  = m_selectDate.addDays(7);;
    setCurrentDate(date);
}
void CWeekView::paintCell(QWidget *cell)
{
    const QRect rect(0, 0, cell->width(), cell->height());

    const int pos = m_cellList.indexOf(cell);
    const bool isCurrentDay = m_days[pos].addDays(m_weekAddDay).weekNumber() == QDate::currentDate().weekNumber();

    const bool isSelectDay = m_days[pos].addDays(m_weekAddDay).weekNumber() == m_selectDate.addDays(m_weekAddDay).weekNumber();


    QPainter painter(cell);
    painter.save();

    painter.setRenderHints(QPainter::HighQualityAntialiasing);
    painter.setBrush(QBrush(m_fillColor));
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect);//画矩形
    painter.restore();
    painter.setPen(Qt::SolidLine);

    const QString dayNum = QString::number(m_days[pos].weekNumber());

    if (isSelectDay) {
        QRect fillRect((cell->width() - 30) / 2, 3, 30, 30);

        painter.setRenderHints(QPainter::HighQualityAntialiasing);
        painter.setBrush(QBrush(m_backgroundcurrentDayColor));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(fillRect);
        painter.setPen(m_currentDayTextColor);
        painter.drawText(QRect(0, 0, cell->width(), cell->height()), Qt::AlignCenter, dayNum);
    } else {
        if (isCurrentDay) {
            painter.setPen(m_backgroundcurrentDayColor);
        } else {
            painter.setPen(m_defaultTextColor);
        }
        painter.drawText(QRect(0, 0, cell->width(), cell->height()), Qt::AlignCenter, dayNum);
    }
    painter.end();
}

bool CWeekView::eventFilter(QObject *o, QEvent *e)
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
void CWeekView::cellClicked(QWidget *cell)
{
    if (!m_cellSelectable)
        return;

    const int pos = m_cellList.indexOf(cell);
    if (pos == -1)
        return;

    setSelectedCell(pos);
    update();
}

void CWeekView::setSelectedCell(int index)
{
    if (m_selectedCell == index)
        return;

    const int prevPos = m_selectedCell;
    m_selectedCell = index;

    m_cellList.at(prevPos)->update();
    m_cellList.at(index)->update();
    m_selectDate = m_days[index];
    emit signalsSelectDate(m_days[index], m_days[index].addDays(m_weekAddDay));
}

void CWeekView::updateDate()
{
    int  weekNum = m_selectDate.weekNumber();
    m_weekAddDay = (m_selectDate.dayOfWeek() + m_firstWeekDay) % 7;
    QDate weekfirstDay = m_selectDate.addDays(-m_weekAddDay);
    m_days[4] = weekfirstDay;
    for (int i(0); i < 4; ++i) {
        m_days[i] = weekfirstDay.addDays(-(4 - i) * 7);
    }
    for (int i(5); i != 10; ++i) {
        m_days[i] = weekfirstDay.addDays((i - 4) * 7);
    }

    setSelectedCell(4);
    update();
}
void CWeekView::resizeEvent(QResizeEvent *event)
{
    int w = width() * 0.1 + 0.5;
    int h = height();
    for (int c = 0; c != 10; ++c) {
        m_cellList[c]->setFixedSize(w, h);
        m_cellList[c]->update();
    }
}
void CWeekView::wheelEvent(QWheelEvent *event)
{
    if (event->delta() < 0) {
        slotnext();
    } else {
        slotprev();
    }
}
