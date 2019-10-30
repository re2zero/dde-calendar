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

#include "monthweekview.h"

#include <QLabel>
#include <QDebug>
#include <QDate>
#include <DPalette>
#include <DApplicationHelper>
#include <DPalette>
DGUI_USE_NAMESPACE
CMonthWeekView::CMonthWeekView(QWidget *parent) : DWidget(parent)
{
    m_mainLayout = new QHBoxLayout;
    m_mainLayout->setMargin(0);
    m_mainLayout->setSpacing(0);
    setLayout(m_mainLayout);
}

void CMonthWeekView::setList(int weekday)
{
    QLayoutItem *child;
    while ((child = m_mainLayout->takeAt(0)) != 0) {
        if (child->widget() != 0) {
            delete child->widget();
        }
        delete child;
    }
    m_weekData.clear();
    QLocale locale;
    for (int i = 0; i != 7; ++i) {

        int d = checkDay(i - weekday);

        DLabel *label = new DLabel(locale.dayName(d ? d : 7, QLocale::ShortFormat));
        label->setContentsMargins(0, 0, 0, 0);
        QFont weekfont;
        weekfont.setFamily("SourceHanSansSC-Medium");
        weekfont.setPixelSize(16);
        label->setFont(weekfont);
        if (d == 7 || d == 6) {
            DPalette monthpa = label->palette();
            QColor textC = "#0887FF";
            QColor textbC(0, 66, 154);
            textbC.setAlphaF(0.05);
            monthpa.setColor(DPalette::WindowText, textC);
            monthpa.setColor(DPalette::Background, textbC);
            label->setAutoFillBackground(true);
            label->setPalette(monthpa);
            m_weekData.append(qMakePair(label, 1));
            //label->setStyleSheet("color:#0887FF;background: rgba(0,66,154,0.05);");
        } else {
            DPalette monthpa = label->palette();
            QColor textC = Qt::black;
            QColor textbC(0, 66, 154);
            textbC.setAlphaF(0.05);
            monthpa.setColor(DPalette::WindowText, textC);
            monthpa.setColor(DPalette::Background, textbC);
            label->setAutoFillBackground(true);
            label->setPalette(monthpa);
            m_weekData.append(qMakePair(label, 0));
            //label->setStyleSheet("color:black;background: rgba(0,66,154,0.05);");
        }
        if ((i == weekday - 1 && weekday != 0) || i == weekday || (weekday == 0 && i == 6)) {
            label->setObjectName("MonthHeaderWeekend");
        } else {
            label->setObjectName("MonthHeaderWeekday");
        }

        label->setAlignment(Qt::AlignCenter);
        if (i == 0 || i == 6) {
            label->setFixedSize(DDEMonthCalendar::MWeekCellWidth - 1, DDEMonthCalendar::MWeekCellHeight);
        } else {
            label->setFixedSize(DDEMonthCalendar::MWeekCellWidth - 3, DDEMonthCalendar::MWeekCellHeight);
        }
        m_mainLayout->addWidget(label, 0, Qt::AlignCenter);
    }
}

void CMonthWeekView::setTheMe(int type)
{
    if (type == 0 || type == 1) {
        for (int i = 0; i < m_weekData.count(); i++) {
            if (m_weekData.at(i).second == 1) {
                DPalette monthpa = m_weekData.at(i).first->palette();
                QColor textC = "#0887FF";
                QColor textbC("#75C18E");
                textbC.setAlphaF(0.1);
                monthpa.setColor(DPalette::WindowText, textC);
                monthpa.setColor(DPalette::Background, textbC);
                m_weekData.at(i).first->setPalette(monthpa);
                m_weekData.at(i).first->setForegroundRole(DPalette::WindowText);
                m_weekData.at(i).first->setBackgroundRole(DPalette::Background);
            } else {
                DPalette monthpa = m_weekData.at(i).first->palette();
                QColor textC = "#6F6F6F";
                QColor textbC("#75C18E");
                textbC.setAlphaF(0.1);
                monthpa.setColor(DPalette::WindowText, textC);
                monthpa.setColor(DPalette::Background, textbC);
                m_weekData.at(i).first->setPalette(monthpa);
                m_weekData.at(i).first->setForegroundRole(DPalette::WindowText);
                m_weekData.at(i).first->setBackgroundRole(DPalette::Background);
            }
        }

    } else if (type == 2) {
        for (int i = 0; i < m_weekData.count(); i++) {
            if (m_weekData.at(i).second == 1) {
                DPalette monthpa = m_weekData.at(i).first->palette();
                QColor textC = "#0887FF";
                QColor textbC = "#82AEC1";
                textbC.setAlphaF(0.00);
                monthpa.setColor(DPalette::WindowText, textC);
                monthpa.setColor(DPalette::Background, textbC);
                m_weekData.at(i).first->setPalette(monthpa);
                m_weekData.at(i).first->setForegroundRole(DPalette::WindowText);
                m_weekData.at(i).first->setBackgroundRole(DPalette::Background);
            } else {
                DPalette monthpa = m_weekData.at(i).first->palette();
                QColor textC = "#C0C6D4";
                QColor textbC = "#82AEC1";
                textbC.setAlphaF(0.00);
                monthpa.setColor(DPalette::WindowText, textC);
                monthpa.setColor(DPalette::Background, textbC);
                m_weekData.at(i).first->setPalette(monthpa);
                m_weekData.at(i).first->setForegroundRole(DPalette::WindowText);
                m_weekData.at(i).first->setBackgroundRole(DPalette::Background);
            }
        }
    }
}

int CMonthWeekView::checkDay(int weekday)
{

    // check the week, calculate the correct order in the custom.

    if (weekday <= 0)
        return weekday += 7;

    if (weekday > 7)
        return weekday -= 7;

    return weekday;
}

void CMonthWeekView::resizeEvent(QResizeEvent *event)
{
    int tw = width() / 7.0 + 0.5;
    int th = height();
    for (int i = 0; i < m_weekData.count(); i++) {
        if (m_weekData.at(i).second == 1) {

            m_weekData.at(i).first->setFixedSize(tw, th);
        } else {
            m_weekData.at(i).first->setFixedSize(tw, th);
        }
    }
    DWidget::resizeEvent(event);
}
