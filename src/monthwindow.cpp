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
#include "monthwindow.h"
#include "monthview.h"
#include "monthdayview.h"
#include "constants.h"
#include "calendardbus.h"
#include <QMessageBox>
#include <QPainter>
CMonthWindow::CMonthWindow(QWidget *parent): QMainWindow (parent)
{
    initUI();
    initConnection();
    initLunar();
}

CMonthWindow::~CMonthWindow()
{
    disconnect(m_today, &DPushButton::clicked, this, &CMonthWindow::slottoday);
    delete  m_monthDayView;
    m_monthDayView = nullptr;
    delete  m_monthView;
    m_monthView = nullptr;
}

void CMonthWindow::setFirstWeekday(int weekday)
{
    m_monthView->setFirstWeekday(weekday);
}

void CMonthWindow::setDate(QDate date)
{
    m_currentdate = date;
    m_YearLabel->setText(QString::number(date.year()) + tr("Y"));
    m_monthDayView->setCurrentDate(date);
}

void CMonthWindow::setLunarVisible(bool state)
{
    m_monthView->setLunarVisible(state);
}

void CMonthWindow::previousMonth()
{
    slideMonth(false);
}

void CMonthWindow::nextMonth()
{
    slideMonth(true);
}

void CMonthWindow::wheelEvent(QWheelEvent *e)
{
    if (e->delta() < 0) {
        nextMonth();
    } else {
        previousMonth();
    }
}

void CMonthWindow::initUI()
{
    m_contentBackground = new QFrame;
    m_contentBackground->setObjectName("CalendarBackground");
    m_contentBackground->setStyleSheet("QFrame#CalendarBackground { "
                                       "background:white;"
                                       "}");
    //m_contentBackground->setFixedSize(CalendarWidth + ContentLeftRightPadding * 2,
    //   InfoViewHeight + CalendarHeight);

    m_today = new DPushButton;
    m_today->setText(tr("Return today"));
    m_today->setFixedSize(DDEMonthCalendar::MTodayWindth, DDEMonthCalendar::MTodayHeight);
    m_YearLabel = new DLabel();
    m_YearLabel->setFixedHeight(DDEMonthCalendar::M_YLableHeight);
    m_YearLunarLabel = new DLabel();
    m_YearLunarLabel->setFixedSize(DDEMonthCalendar::M_YLunatLabelWindth, DDEMonthCalendar::M_YLunatLabelHeight);

    QFont ylabelF;
    ylabelF.setFamily("SourceHanSansSC-Medium");
    ylabelF.setPixelSize(24);
    m_YearLabel->setFont(ylabelF);
    QPalette pa;
    pa.setColor(QPalette::WindowText, QColor("#3B3B3B"));
    m_YearLabel->setPalette(pa);
    //m_YearLabel->setStyleSheet("color:#3B3B3B;");

    ylabelF.setPixelSize(14);
    m_YearLunarLabel->setFont(ylabelF);
    QPalette Lunarpa;
    Lunarpa.setColor(QPalette::WindowText, QColor("#8A8A8A"));
    m_YearLunarLabel->setPalette(Lunarpa);

    m_monthDayView = new CMonthDayView();

    QHBoxLayout *yeartitleLayout = new QHBoxLayout;
    yeartitleLayout->setMargin(0);
    yeartitleLayout->setSpacing(0);
    yeartitleLayout->setContentsMargins(21, 18, 18, 20);
    yeartitleLayout->addWidget(m_YearLabel);
    yeartitleLayout->addWidget(m_YearLunarLabel);
    QSpacerItem *t_spaceitem = new QSpacerItem(30, DDEMonthCalendar::M_YLableHeight, QSizePolicy::Expanding, QSizePolicy::Fixed);
    yeartitleLayout->addSpacerItem(t_spaceitem);
    yeartitleLayout->addWidget(m_monthDayView);
    QSpacerItem *t_spaceitemr = new QSpacerItem(30, DDEMonthCalendar::M_YLableHeight, QSizePolicy::Expanding, QSizePolicy::Fixed);
    yeartitleLayout->addSpacerItem(t_spaceitemr);
    yeartitleLayout->addWidget(m_today);

    m_monthView = new CMonthView(this);
    QVBoxLayout *mhLayout = new QVBoxLayout;
    mhLayout->setContentsMargins(10, 0, 10, 10);
    mhLayout->addWidget(m_monthView);
    QVBoxLayout *hhLayout = new QVBoxLayout;
    hhLayout->addLayout(yeartitleLayout);
    hhLayout->addLayout(mhLayout);
    m_contentBackground->setLayout(hhLayout);

    m_animationContainer = new QFrame(m_contentBackground);
    m_animationContainer->setStyleSheet("QFrame { background: rgba(0, 0, 0, 0) }");
    m_animationContainer->setFixedSize(m_monthView->width(),
                                       m_monthView->height() - DDEMonthCalendar::MDayCellHeight);
    m_animationContainer->move(10, 130);
    m_animationContainer->hide();

    setCentralWidget(m_contentBackground);
}

void CMonthWindow::initConnection()
{
    connect(m_today, &DPushButton::clicked, this, &CMonthWindow::slottoday);
    connect(m_monthView, &CMonthView::signalcurrentLunarDateChanged, this, &CMonthWindow::slotcurrentDateLunarChanged);
    connect(m_monthView, &CMonthView::signalcurrentDateChanged, this, &CMonthWindow::slotcurrentDateChanged);
    connect(m_monthDayView, &CMonthDayView::signalsSelectDate, this, &CMonthWindow::slotSelectedMonth);
    connect(m_monthView, &CMonthView::signalsSchceduleUpdate, this, &CMonthWindow::slotTransitSchedule);
    connect(m_monthDayView, &CMonthDayView::signalsCurrentDate, this, &CMonthWindow::slotSelectedMonth);
}

void CMonthWindow::initLunar()
{
    m_monthView->setLunarVisible(true);
}

void CMonthWindow::slideMonth(bool next)
{
    m_animationContainer->show();
    m_animationContainer->raise();

    if (next) {
        if (m_currentdate.month() != 1) {
            m_currentdate = m_currentdate.addMonths(-1);
        }
    } else {
        if (m_currentdate.month() != 12) {
            m_currentdate = m_currentdate.addMonths(1);
        }
    }
    setDate(m_currentdate);
}

void CMonthWindow::slotReturnTodayUpdate()
{
    setDate(QDate::currentDate());
}

void CMonthWindow::slotupdateSchedule(int id)
{
    m_monthView->slotSchceduleUpdate(id);
}

void CMonthWindow::slotTransitSchedule(int id)
{
    emit signalsWUpdateShcedule(this, id);
}

void CMonthWindow::slottoday()
{
    emit signalsReturnTodayUpdate(this);
    setDate(QDate::currentDate());
}

void CMonthWindow::slotcurrentDateLunarChanged(QDate date, CaLunarDayInfo detail, int type)
{
    QDate currentdate = m_currentdate;
    m_currentdate = date;
    if (type == 1) {
        m_YearLabel->setText(QString::number(date.year()) + tr("Y"));
        m_YearLunarLabel->setText("-" + detail.mGanZhiYear + detail.mZodiac + "年-");
        m_animationContainer->hide();

    } else if (type == 0) {
        if (date.month() != currentdate.month()
                && date.year() == currentdate.year()) {
            m_monthDayView->setRCurrentDate(date);
        }
    }
    //m_monthDayView->setRCurrentDate(date);
}

void CMonthWindow::slotcurrentDateChanged(QDate date)
{
    m_currentdate = date;
    if (date != QDate::currentDate()) {
        m_today->setVisible(true);
    } else {
        m_today->setVisible(false);
    }
}

void CMonthWindow::slotSelectedMonth(QDate date)
{
    m_currentdate = date;
    m_monthView->setCurrentDate(date);
}
