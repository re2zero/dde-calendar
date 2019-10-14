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
#include "weekwindow.h"
#include "scheduleview.h"
#include "constants.h"
#include "calendardbus.h"
#include "weekheadview.h"
#include "weekview.h"
#include <QMessageBox>
#include <QDate>
#include <QHBoxLayout>
#include <DPalette>
#include <DHiDPIHelper>
DGUI_USE_NAMESPACE
CWeekWindow::CWeekWindow(QWidget *parent): QMainWindow (parent)
{
    setContentsMargins(0, 0, 0, 0);
    initUI();
    initConnection();

}

CWeekWindow::~CWeekWindow()
{

}

void CWeekWindow::setDate(QDate date)
{
    m_currentdate = date;
    m_weekview->setCurrentDate(date);
}

void CWeekWindow::setFirstWeekday(int weekday)
{
    m_weekview->setFirstWeekDay(weekday);
    m_weekHeadView->setFirstWeekday(weekday);
    m_scheduleView->setFirstWeekday(weekday);
}

void CWeekWindow::setLunarVisible(bool state)
{
    m_weekHeadView->setLunarVisible(state);
    m_YearLunarLabel->setVisible(state);
    m_scheduleView->setLunarVisible(state);
}

void CWeekWindow::initUI()
{
    m_contentBackground = new DFrame;
    m_contentBackground->setAutoFillBackground(true);
    DPalette anipa = m_contentBackground->palette();
    anipa.setColor(DPalette::Background, "#F8F8F8");
    m_contentBackground->setPalette(anipa);
    m_contentBackground->setContentsMargins(0, 0, 0, 0);

    m_today = new DPushButton;
    m_today->setText(tr("Return today"));
    m_today->setFixedSize(DDEWeekCalendar::WTodayWindth, DDEWeekCalendar::WTodayHeight);
    DPalette todaypa = m_today->palette();
    todaypa.setColor(DPalette::ButtonText, QColor("#0098FF"));
    todaypa.setColor(DPalette::Dark, Qt::white);
    todaypa.setColor(DPalette::Light, Qt::white);
    QColor sbcolor("#002A57");
    sbcolor.setAlphaF(0.05);
    todaypa.setColor(DPalette::Shadow, sbcolor);
    QFont todayfont;
    todayfont.setFamily("SourceHanSansSC-Medium");
    todayfont.setPixelSize(14);
    m_today->setFont(todayfont);
    m_today->setPalette(todaypa);
    m_prevButton = new DIconButton(DStyle::SP_ArrowLeft, this);
    //m_prevButton->setIconSize(QSize(36, 36));
    //m_prevButton->setIcon(QIcon(DHiDPIHelper::loadNxPixmap(":/resources/icon/previous_normal.svg")));
    //m_prevButton->setArrowDirection(DArrowButton::ArrowLeft);

    m_prevButton->setFixedSize(36, 36);

    m_nextButton = new DIconButton(DStyle::SP_ArrowRight, this);
    //m_nextButton->setIconSize(QSize(36, 36));
    //m_nextButton->setIcon(QIcon(DHiDPIHelper::loadNxPixmap(":/resources/icon/next_normal.svg")));
    m_nextButton->setFixedSize(36, 36);
    //m_nextButton->setFixedSize(DDEWeekCalendar::WWeekCellWidth, DDEWeekCalendar::WWeekCellHeight);

    m_YearLabel = new DLabel();
    m_YearLabel->setFixedHeight(DDEWeekCalendar::W_YLableHeight);
    //m_currentMouth->setStyleSheet("border: 1px solid rgba(0, 0, 0, 0.05);");

    QFont t_labelF;
    t_labelF.setFamily("SourceHanSansSC-Medium");
    t_labelF.setPixelSize(24);
    m_YearLabel->setFont(t_labelF);
    DPalette Lunadpa = m_YearLabel->palette();
    Lunadpa.setColor(DPalette::WindowText, QColor("#3B3B3B"));
    m_YearLabel->setPalette(Lunadpa);
    //m_YearLabel->setStyleSheet("color:#3B3B3B;");

    m_YearLunarLabel = new DLabel(m_contentBackground);
    m_YearLunarLabel->setFixedSize(DDEWeekCalendar::W_YLunatLabelWindth, DDEWeekCalendar::W_YLunatLabelHeight);

    m_weekview  = new CWeekView(this);
    // m_weekview->setFixedHeight(DDEWeekCalendar::W_YLunatLabelHeight + 8);

    m_weekLabel = new DLabel();
    m_weekLabel->setFixedHeight(DDEWeekCalendar::W_YLableHeight);
    QFont weeklabelF;
    weeklabelF.setFamily("PingFangSC-Medium");
    weeklabelF.setPixelSize(14);
    m_weekLabel->setFont(weeklabelF);
    DPalette wpa = m_weekLabel->palette();
    wpa.setColor(DPalette::WindowText, QColor("#717171"));
    m_weekLabel->setPalette(wpa);
    m_weekLabel->setText(tr("Week"));

    QFont ylabelF;
    ylabelF.setFamily("SourceHanSansSC-Medium");
    ylabelF.setPixelSize(14);
    m_YearLunarLabel->setFont(ylabelF);
    DPalette YearLpa = m_YearLunarLabel->palette();
    YearLpa.setColor(DPalette::WindowText, QColor("#8A8A8A"));

    m_YearLunarLabel->setPalette(YearLpa);
    //m_YearLunarLabel->move(116, 27);

    QHBoxLayout *yeartitleLayout = new QHBoxLayout;
    yeartitleLayout->setMargin(0);
    yeartitleLayout->setSpacing(0);
    yeartitleLayout->setContentsMargins(21, 20, 18, 0);
    yeartitleLayout->addWidget(m_YearLabel);

    QHBoxLayout *yeartitleLayout1 = new QHBoxLayout;
    yeartitleLayout1->setMargin(0);
    yeartitleLayout1->setSpacing(0);
    yeartitleLayout1->setContentsMargins(4, 9, 0, 7);
    yeartitleLayout1->addWidget(m_YearLunarLabel);
    yeartitleLayout->addLayout(yeartitleLayout1);



    //QHBoxLayout *yeartitleLayout = new QHBoxLayout;
    //yeartitleLayout->setMargin(0);
    //yeartitleLayout->setSpacing(0);
    //yeartitleLayout->setContentsMargins(21, 10, 20, 20);
    // yeartitleLayout->addWidget(m_YearLabel);
    //yeartitleLayout->addSpacing(14);
    //yeartitleLayout->addWidget(m_YearLunarLabel);
    //yeartitleLayout->addSpacing(30);
    m_spaceitem = new QSpacerItem(30, 36, QSizePolicy::Fixed, QSizePolicy::Fixed);

    yeartitleLayout->addItem(m_spaceitem);
    yeartitleLayout->addWidget(m_prevButton);
    yeartitleLayout->addWidget(m_weekview);
    yeartitleLayout->addWidget(m_nextButton);
    yeartitleLayout->addSpacing(10);
    yeartitleLayout->addWidget(m_weekLabel);
    yeartitleLayout->addStretch();
    yeartitleLayout->addWidget(m_today);

    m_weekHeadView = new CWeekHeadView();
    m_scheduleView = new CScheduleView();
    m_weekHeadView->setFixedWidth(840);
    m_scheduleView->setviewMagin(73, 109, 0, 0);
    m_scheduleView->setRange(763, 1032, QDate(2019, 8, 12), QDate(2019, 8, 18));
    m_scheduleView->setFixedSize(838, 450);

    m_mainhLayout = new QVBoxLayout;
    m_mainhLayout->setMargin(0);
    m_mainhLayout->setSpacing(0);
    m_mainhLayout->setContentsMargins(8, 20, 10, 9);
    m_mainhLayout->addWidget(m_weekHeadView);
    m_mainhLayout->addWidget(m_scheduleView);
    QVBoxLayout *hhLayout = new QVBoxLayout;
    hhLayout->setMargin(0);
    hhLayout->setSpacing(0);
    hhLayout->setContentsMargins(0, 0, 0, 0);
    hhLayout->addLayout(yeartitleLayout);
    hhLayout->addLayout(m_mainhLayout);
    m_contentBackground->setLayout(hhLayout);
    setCentralWidget(m_contentBackground);
}

void CWeekWindow::initConnection()
{
    connect(m_prevButton, &DIconButton::clicked, this, &CWeekWindow::slotprev);
    connect(m_today, &DPushButton::clicked, this, &CWeekWindow::slottoday);
    connect(m_nextButton, &DIconButton::clicked, this, &CWeekWindow::slotnext);
    connect(m_weekview, &CWeekView::signalsSelectDate, this, &CWeekWindow::slotCurrentWeek);
    connect(m_weekHeadView, &CWeekHeadView::signalcurrentLunarDateChanged, this, &CWeekWindow::slotcurrentDateLunarChanged);
    connect(m_weekHeadView, &CWeekHeadView::signalcurrentDateChanged, this, &CWeekWindow::slotcurrentDateChanged);
    connect(m_scheduleView, &CScheduleView::signalsUpdateShcedule, this, &CWeekWindow::slotTransitSchedule);
}

void CWeekWindow::setTheMe(int type)
{
    if (type == 0 || type == 1) {
        DPalette anipa = m_contentBackground->palette();
        anipa.setColor(DPalette::Background, "#F8F8F8");
        m_contentBackground->setPalette(anipa);

        DPalette todaypa = m_today->palette();
        todaypa.setColor(DPalette::ButtonText, QColor("#1D81EC"));
        todaypa.setColor(DPalette::Dark, Qt::white);
        todaypa.setColor(DPalette::Light, Qt::white);
        m_today->setPalette(todaypa);

        DPalette prevpa = m_prevButton->palette();
        prevpa.setColor(DPalette::Dark, QColor("#E6E6E6"));
        prevpa.setColor(DPalette::Light, QColor("#E3E3E3"));
        // m_prevButton->setPalette(prevpa);

        DPalette nextvpa = m_nextButton->palette();
        nextvpa.setColor(DPalette::Dark, QColor("#E6E6E6"));
        nextvpa.setColor(DPalette::Light, QColor("#E3E3E3"));
        //m_nextButton->setPalette(nextvpa);

        DPalette pa = m_YearLabel->palette();
        pa.setColor(DPalette::WindowText, QColor("#3B3B3B"));
        m_YearLabel->setPalette(pa);

        DPalette Lunapa = m_YearLunarLabel->palette();
        Lunapa.setColor(DPalette::WindowText, QColor("#8A8A8A"));
        m_YearLunarLabel->setPalette(Lunapa);


        DPalette wpa = m_weekLabel->palette();
        wpa.setColor(DPalette::WindowText, QColor("#717171"));
        m_weekLabel->setPalette(wpa);
    } else if (type == 2) {
        DPalette anipa = m_contentBackground->palette();
        anipa.setColor(DPalette::Background, "#252525");
        m_contentBackground->setPalette(anipa);

        DPalette todaypa = m_today->palette();
        todaypa.setColor(DPalette::ButtonText, QColor("#0081FF"));
        todaypa.setColor(DPalette::Dark, "#414141");
        todaypa.setColor(DPalette::Light, "#484848");
        m_today->setPalette(todaypa);

        DPalette prevpa = m_prevButton->palette();
        prevpa.setColor(DPalette::Dark, QColor("#484848"));
        prevpa.setColor(DPalette::Light, QColor("#414141"));
        //prevpa.setColor(DPalette::Dark, Qt::black);
        //prevpa.setColor(DPalette::Light, Qt::black);
        //m_prevButton->setPalette(prevpa);

        DPalette nextvpa = m_nextButton->palette();
        nextvpa.setColor(DPalette::Dark, QColor("#484848"));
        nextvpa.setColor(DPalette::Light, QColor("#414141"));
        //m_nextButton->setPalette(nextvpa);

        DPalette pa = m_YearLabel->palette();
        pa.setColor(DPalette::WindowText, QColor("#C0C6D4"));
        m_YearLabel->setPalette(pa);

        DPalette Lunapa = m_YearLunarLabel->palette();
        Lunapa.setColor(DPalette::WindowText, QColor("#798BA8"));
        m_YearLunarLabel->setPalette(Lunapa);

        DPalette wpa = m_weekLabel->palette();
        wpa.setColor(DPalette::WindowText, QColor("#717171"));
        m_weekLabel->setPalette(wpa);
    }
    m_weekview->setTheMe(type);
    m_weekHeadView->setTheMe(type);
    m_scheduleView->setTheMe(type);
}

void CWeekWindow::slotReturnTodayUpdate()
{
    setDate(QDate::currentDate());
}

void CWeekWindow::slotupdateSchedule(int id)
{
    m_scheduleView->slotupdateSchedule(id);
}

void CWeekWindow::slotTransitSchedule(int id)
{
    emit signalsWUpdateShcedule(this, id);
}

void CWeekWindow::slotprev()
{
    if (m_currentdate.year() > 1900) {
        m_currentdate = m_currentdate.addDays(-7);
        setDate(m_currentdate);
    } else {
        QMessageBox::information(this, tr("infomation"), tr("Year less than 1900!"));
    }
}

void CWeekWindow::slotnext()
{
    m_currentdate = m_currentdate.addDays(7);;
    setDate(m_currentdate);
}

void CWeekWindow::slottoday()
{
    emit signalsReturnTodayUpdate(this);
    setDate(QDate::currentDate());
}

void CWeekWindow::slotCurrentWeek(QDate date, QDate currentDate)
{
    QVector<QDate> vDate;
    for (int i = 0; i < 7; i++)
        vDate.append(date.addDays(i));
    m_currentdate = currentDate;
    m_weekHeadView->setWeekDay(vDate);
    m_weekHeadView->setCurrentDate(m_currentdate);
    m_scheduleView->setRange(date, date.addDays(6));
    //m_scheduleView->setDate(m_currentdate);
}

void CWeekWindow::slotcurrentDateLunarChanged(QDate date,  CaLunarDayInfo detail, int type)
{
    if (date != QDate::currentDate()) {
        m_today->setVisible(true);
    } else {
        m_today->setVisible(false);
    }
    if (detail.mLunarFestival.isEmpty()) {
        m_scheduleView->setDate(m_currentdate, detail.mSolarFestival);
    } else {
        m_scheduleView->setDate(m_currentdate, detail.mLunarFestival + "    " + detail.mSolarFestival);
    }
    m_currentdate = date;
    if (type == 1) {
        m_YearLabel->setText(QString::number(date.year()) + tr("Y"));
        m_YearLunarLabel->setText("-" + detail.mGanZhiYear + detail.mZodiac + "年-");
    }
}

void CWeekWindow::slotcurrentDateChanged(QDate date)
{
    if (date != QDate::currentDate()) {
        m_today->setVisible(true);
    } else {
        m_today->setVisible(false);
    }
    m_currentdate = date;
    m_scheduleView->setDate(date);
}

void CWeekWindow::resizeEvent(QResizeEvent *event)
{
    int sleftMagin = 0.093 * width() + 0.5;
    int stopMagin = 0.1797 * height() + 0.5;

    int headh = height() * 0.0924 + 0.5;
    int sh = height() - headh - 78;
    int topmagin = height() * 0.0193 + 0.5;
    int buttonmagin = topmagin;

    int dw = width() * 0.4186 + 0.5;
    int dh = 36;
    int space = (width() - dw) / 2 - 220;

    int sw = (width() -  width() * 0.9802 + 0.5) / 2;

    m_mainhLayout->setContentsMargins(sw, 20, width() -  width() * 0.9802 + 0.5 - sw, 10);

    m_spaceitem->changeSize(space, 36, QSizePolicy::Fixed, QSizePolicy::Fixed);


    m_weekview->setFixedSize(dw, dh);

    m_weekHeadView->setFixedSize(width() * 0.9802 + 0.5, headh);

    m_scheduleView->setviewMagin(sleftMagin, stopMagin, 0, 0);

    m_scheduleView->setFixedSize(width() * 0.9802 + 0.5, sh);
    QMainWindow::resizeEvent(event);
}
