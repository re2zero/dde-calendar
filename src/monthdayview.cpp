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

#include "monthdayview.h"
#include <QHBoxLayout>
#include "constants.h"
#include <QPainter>
#include <QBrush>
#include <QEvent>
CMonthDayView::CMonthDayView(QWidget *parent) : QWidget(parent)
{
    m_dayNumFont.setFamily("Avenir-Light");
    m_dayNumFont.setPixelSize(16);
    m_dayNumFont.setWeight(QFont::Light);


    // cells grid
    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->setMargin(0);
    hboxLayout->setSpacing(0);
    hboxLayout->setContentsMargins(4, 0, 4, 0);
    for (int c = 0; c != 12; ++c) {
        QWidget *cell = new QWidget;
        cell->setFixedSize(DDEMonthCalendar::MDayCellWidth, DDEMonthCalendar::MDayCellHeight);
        cell->installEventFilter(this);
        cell->setFocusPolicy(Qt::ClickFocus);
        hboxLayout->addWidget(cell);
        m_cellList.append(cell);
    }
    setLayout(hboxLayout);
}

CMonthDayView::~CMonthDayView()
{
    for (int i = 0; i < 12; i++) {
        m_cellList.at(i)->removeEventFilter(this);
        delete m_cellList.at(i);
    }
    m_cellList.clear();
}

void CMonthDayView::setCurrentDate(const QDate date)
{
    m_selectDate = date;
    for (int i = 0; i < 12; i++) {
        m_days[i] = QDate(date.year(), i + 1, date.day());
    }
    setSelectedCell(m_selectDate.month() - 1);
    update();
}

void CMonthDayView::paintCell(QWidget *cell)
{
    const QRect rect(0, 0, DDEMonthCalendar::MDayCellWidth, DDEMonthCalendar::MDayCellHeight);

    const int pos = m_cellList.indexOf(cell);
    const bool isCurrentDay = m_days[pos].month() == QDate::currentDate().month();

    const bool isSelectDay = m_days[pos].month() == m_selectDate.month();


    QPainter painter(cell);
    painter.setPen(Qt::SolidLine);

    const QString dayNum = QString::number(m_days[pos].month());

    if (isSelectDay) {
        QRect fillRect(3, 3, 30, 30);

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

bool CMonthDayView::eventFilter(QObject *o, QEvent *e)
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
void CMonthDayView::cellClicked(QWidget *cell)
{
    if (!m_cellSelectable)
        return;

    const int pos = m_cellList.indexOf(cell);
    if (pos == -1)
        return;

    setSelectedCell(pos);
    update();
}

void CMonthDayView::setSelectedCell(int index)
{
    if (m_selectedCell == index)
        return;

    const int prevPos = m_selectedCell;
    m_selectedCell = index;

    m_cellList.at(prevPos)->update();
    m_cellList.at(index)->update();
    m_selectDate = m_days[index];
    emit signalsSelectDate(m_days[index]);
}

