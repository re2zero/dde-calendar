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
#include "calendartimeeidt.h"
#include "verticalscrolltext.h"
#include "verticalscroll.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
CCalendarTimeEidt::CCalendarTimeEidt(QWidget *parent)
    : QWidget(parent)
{
    initUI();
    initConnection();
    resize(200, 300);
    //setFocus(Qt::PopupFocusReason);
    //setFocusPolicy(Qt::WheelFocus);
}

CCalendarTimeEidt::~CCalendarTimeEidt()
{
}

void CCalendarTimeEidt::setTime(QTime time)
{
    if (m_time == time) return;
    m_time = time;
    int hour = time.hour();
    int mints = time.minute();
    if (hour < 12) {
        m_AmorPmW->setCurrValue(0);
        m_HourScroll->setCurrValue(hour);
    } else {
        m_AmorPmW->setCurrValue(1);
        m_HourScroll->setCurrValue(hour - 12);
    }
    m_MinScroll->setCurrValue(mints);
}

QString CCalendarTimeEidt::getCrrentAMorPm()
{
    return m_AmorPmW->getText();
}

int CCalendarTimeEidt::getHour()
{
    return m_HourScroll->readValue();
}

int CCalendarTimeEidt::getMin()
{
    return m_MinScroll->readValue();
}

void CCalendarTimeEidt::focusOutEvent(QFocusEvent *event)
{
    // hide();
    QWidget::focusOutEvent(event);
}

void CCalendarTimeEidt::initUI()
{
    QVBoxLayout *vboxlayout = new QVBoxLayout();
    vboxlayout->setSpacing(0);
    vboxlayout->setMargin(0);
    vboxlayout->addStretch(1);
    m_AmorPmW = new VerticalScrollText(NULL);
    m_AmorPmW->setFixedSize(90, 150);
    QVector<QString> AMPM;
    AMPM.append(tr("AM"));
    AMPM.append(tr("PM"));

    m_AmorPmW->setRange(AMPM);
    vboxlayout->addWidget(m_AmorPmW);
    vboxlayout->addStretch(1);
    QHBoxLayout *vhboxlayout = new QHBoxLayout();

    m_HourScroll = new VerticalScroll(NULL);
    m_HourScroll->setRange(0, 12);
    m_HourScroll->setFixedSize(55, 300);

    m_MinScroll = new VerticalScroll(NULL);
    m_MinScroll->setRange(0, 59);
    m_MinScroll->setFixedSize(55, 300);
    vhboxlayout->setSpacing(0);
    vhboxlayout->setMargin(0);
    vhboxlayout->addWidget(m_HourScroll);
    vhboxlayout->addWidget(m_MinScroll);
    QHBoxLayout *mainlayout = new QHBoxLayout(this);
    mainlayout->setSpacing(0);
    mainlayout->setMargin(0);
    mainlayout->addLayout(vboxlayout);
    mainlayout->addLayout(vhboxlayout);
    setLayout(mainlayout);
}

void CCalendarTimeEidt::initConnection()
{
    connect(m_AmorPmW, &VerticalScrollText::currentValueChanged, this, &CCalendarTimeEidt::currentTValueChanged);
    connect(m_HourScroll, &VerticalScroll::currentValueChanged, this, &CCalendarTimeEidt::currentHValueChanged);
    connect(m_MinScroll, &VerticalScroll::currentValueChanged, this, &CCalendarTimeEidt::currentMValueChanged);
}
