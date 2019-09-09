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
#include "daywindow.h"
#include "daymonthview.h"
#include "constants.h"
#include "scheduleview.h"
#include "schcedulesearchview.h"
#include <QMessageBox>
#include <QHBoxLayout>
#include <DPalette>
DGUI_USE_NAMESPACE
CDayWindow::CDayWindow(QWidget *parent): QMainWindow (parent)
{
    initUI();
    initConnection();
}

CDayWindow::~CDayWindow()
{

}

void CDayWindow::setDate(QDate date)
{
    m_currentdate = date;
    m_daymonthView->setFirstWeekday(0);
    m_daymonthView->setCurrentDate(date);
    int w = m_scheduleView->width() - 72;
    m_scheduleView->setRange(w, 1032, m_currentdate, m_currentdate);
}

void CDayWindow::setSearchWFlag(bool flag)
{
    m_schceduleSearchView->setVisible(flag);
}

void CDayWindow::setSearchText(QString str)
{
    m_searchText = str;
    m_schceduleSearchView->slotsetSearch(str);
}

void CDayWindow::setLunarVisible(bool state)
{
    m_LunarLabel->setVisible(state);
    m_SolarDay->setVisible(state);
    m_daymonthView->setVisible(state);
}

void CDayWindow::initUI()
{
    m_contentBackground = new QFrame;
    m_contentBackground->setObjectName("CalendarBackground");
    DPalette anipa = m_contentBackground->palette();
    anipa.setColor(DPalette::Background, Qt::white);
    m_contentBackground->setAutoFillBackground(true);
    m_contentBackground->setPalette(anipa);

    //m_contentBackground->setFixedSize(CalendarWidth + ContentLeftRightPadding * 2,
    //   InfoViewHeight + CalendarHeight);

    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->setMargin(0);
    titleLayout->setSpacing(0);
    titleLayout->setContentsMargins(0, 0, 0, 3);

    m_YearLabel = new DLabel();
    m_YearLabel->setFixedSize(168, DDEDayCalendar::D_YLableHeight);
    QFont labelF;
    labelF.setFamily("SourceHanSansSC-Medium");
    labelF.setPixelSize(24);
    m_YearLabel->setFont(labelF);
    DPalette ypa = m_YearLabel->palette();
    ypa.setColor(DPalette::WindowText, QColor("#3B3B3B"));
    m_YearLabel->setPalette(ypa);
    titleLayout->addWidget(m_YearLabel);
    //titleLayout->addStretch(1);
    m_LunarLabel = new DLabel();
    m_LunarLabel->setFixedHeight(DDEDayCalendar::D_YLableHeight);
    labelF.setPixelSize(14);
    m_LunarLabel->setFont(labelF);
    m_LunarLabel->setAlignment(Qt::AlignCenter);
    DPalette lpa = m_LunarLabel->palette();
    lpa.setColor(DPalette::WindowText, QColor("#8A8A8A"));
    m_LunarLabel->setPalette(lpa);
    titleLayout->addWidget(m_LunarLabel);
    //titleLayout->addStretch(1);
    m_SolarDay = new DLabel();
    labelF.setPixelSize(10);
    m_SolarDay->setFixedHeight(DDEDayCalendar::D_YLableHeight);
    m_SolarDay->setFont(labelF);
    m_SolarDay->setAlignment(Qt::AlignCenter);
    DPalette spa = m_SolarDay->palette();
    spa.setColor(DPalette::WindowText, Qt::red);
    m_SolarDay->setPalette(spa);
    titleLayout->addWidget(m_SolarDay);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->setMargin(0);
    leftLayout->setSpacing(0);
    m_scheduleView = new CScheduleView(this);
    //leftLayout->setContentsMargins(12, 0, 10, 12);
    m_scheduleView->setviewMagin(72, 57, 0, 0);
    m_scheduleView->setRange(423, 1032, QDate(2019, 8, 12), QDate(2019, 8, 12));
    //m_scheduleView->setFixedSize(513, 450);
    leftLayout->addLayout(titleLayout);
    leftLayout->addWidget(m_scheduleView);

    m_daymonthView = new CDayMonthView(this);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->addLayout(leftLayout);
    mainLayout->addWidget(m_daymonthView);
    //mainLayout->addStretch(1);
    m_schceduleSearchView = new CSchceduleSearchView(this);
    m_schceduleSearchView->setFixedWidth(200);
    mainLayout->addWidget(m_schceduleSearchView);
    m_schceduleSearchView->setVisible(false);


    m_contentBackground->setLayout(mainLayout);


    setCentralWidget(m_contentBackground);
}

void CDayWindow::initConnection()
{
    connect(m_daymonthView, &CDayMonthView::signalcurrentLunarDateChanged, this, &CDayWindow::slotcurrentDateLunarChanged);
    connect(m_daymonthView, &CDayMonthView::signalcurrentDateChanged, this, &CDayWindow::slotcurrentDateChanged);
    connect(m_daymonthView, &CDayMonthView::signalsReturnDay, this, &CDayWindow::slotCurrentReturnDay);

    connect(m_scheduleView, &CScheduleView::signalsUpdateShcedule, this, &CDayWindow::slotTransitSchedule);
    connect(m_schceduleSearchView, &CSchceduleSearchView::signalsUpdateShcedule, this, &CDayWindow::slotTransitSearchSchedule);
}


void CDayWindow::slotupdateSchedule(int id)
{
    m_scheduleView->setDate(m_currentdate);
}

void CDayWindow::slotTransitSchedule(int id)
{
    m_schceduleSearchView->slotsetSearch(m_searchText);
    emit signalsWUpdateShcedule(this, id);
}

void CDayWindow::slotTransitSearchSchedule(int id)
{
    m_scheduleView->slotupdateSchedule();
    m_schceduleSearchView->slotsetSearch(m_searchText);
    emit signalsWUpdateShcedule(this, id);
}

void CDayWindow::slotReturnTodayUpdate()
{
    m_daymonthView->slottoday();
}

void CDayWindow::slotCurrentReturnDay()
{
    emit signalsReturnTodayUpdate(this);
}

void CDayWindow::slotcurrentDateLunarChanged(QDate date, CaHuangLiDayInfo detail, int type)
{
    m_currentdate = date;
    if (type == 1) {
        m_YearLabel->setText(QString::number(date.year()) + tr("Y") + QString::number(date.month()) + tr("M") + QString::number(date.day()) + tr("D"));
        m_LunarLabel->setText(detail.mLunarDayName);
        m_SolarDay->setText(detail.mSolarFestival);
    }
    m_scheduleView->setRange(m_currentdate, m_currentdate);
    m_scheduleView->setDate(m_currentdate);
}

void CDayWindow::slotcurrentDateChanged(QDate date)
{
    m_currentdate = date;
    m_YearLabel->setText(QString::number(date.year()) + tr("Y") + QString::number(date.month()) + tr("Y") + QString::number(date.day()) + tr("D"));
    m_scheduleView->setRange(m_currentdate, m_currentdate);
    m_scheduleView->setDate(m_currentdate);
}


