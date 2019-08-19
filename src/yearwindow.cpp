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
#include "yearwindow.h"
#include "yearview.h"
#include "constants.h"
#include "calendardbus.h"
#include <QMessageBox>
CYearWindow::CYearWindow(QWidget *parent): QMainWindow (parent)
{
    initUI();
    initConnection();
}

CYearWindow::~CYearWindow()
{
    for (int i = 0; i < 12; i++) {
        disconnect(m_monthViewList.at(i), &CYearView::singanleActiveW, this, &CYearWindow::slotActiveW);
        disconnect(m_monthViewList.at(i), &CYearView::dateSelected, this, &CYearWindow::dateSelected);
        delete  m_monthViewList.at(i);
    }
    m_monthViewList.clear();
}

void CYearWindow::setDate(QDate date)
{
    m_currentdate = date;
    for (int i = 0; i < 12; i++) {
        m_monthViewList.at(i)->setCurrentDate(QDate(m_currentdate.year(), i + 1, 1));
    }
}

void CYearWindow::initUI()
{
    m_contentBackground = new QFrame;
    m_contentBackground->setObjectName("CalendarBackground");
    m_contentBackground->setStyleSheet("QFrame#CalendarBackground { "
                                       "background:white;"
                                       "}");
    //m_contentBackground->setFixedSize(CalendarWidth + ContentLeftRightPadding * 2,
    //   InfoViewHeight + CalendarHeight);

    m_today = new DBaseButton;
    m_today->setText(tr("Return today"));
    m_today->setFixedHeight(DDEYearCalendar::Y_MLableHeight);
    m_prevButton = new DArrowButton;
    m_prevButton->setFixedSize(DDEYearCalendar::Y_MLableHeight, DDEYearCalendar::Y_MLableHeight);
    m_prevButton->setArrowDirection(DArrowButton::ArrowLeft);
    m_nextButton = new DArrowButton;
    m_nextButton->setFixedSize(DDEYearCalendar::Y_MLableHeight, DDEYearCalendar::Y_MLableHeight);
    m_nextButton->setArrowDirection(DArrowButton::ArrowRight);
    m_YearLabel = new DLabel();
    m_YearLabel->setFixedHeight(DDEYearCalendar::Y_YLableHeight);
    //m_currentMouth->setStyleSheet("border: 1px solid rgba(0, 0, 0, 0.05);");

    QFont t_labelF;
    t_labelF.setFamily("Helvetica");
    t_labelF.setPixelSize(30);
    m_YearLabel->setFont(t_labelF);
    m_YearLabel->setStyleSheet("color:#CF0059;");

    m_YearLunarLabel = new DLabel(m_contentBackground);
    m_YearLunarLabel->setFixedSize(DDEMonthCalendar::M_YLunatLabelWindth, DDEMonthCalendar::M_YLunatLabelHeight);

    QFont ylabelF;
    ylabelF.setFamily("SourceHanSansSC-Medium");
    ylabelF.setPixelSize(14);
    m_YearLunarLabel->setFont(ylabelF);
    m_YearLunarLabel->setStyleSheet("color:#8A8A8A;");
    m_YearLunarLabel->move(116, 27);
    QHBoxLayout *yeartitleLayout = new QHBoxLayout;
    yeartitleLayout->setMargin(0);
    yeartitleLayout->setSpacing(0);
    yeartitleLayout->setContentsMargins(12, 0, 10, 12);
    yeartitleLayout->addWidget(m_YearLabel);
    yeartitleLayout->addWidget(m_YearLunarLabel);
    QSpacerItem *t_spaceitem = new QSpacerItem(30, DDEYearCalendar::Y_MLableHeight, QSizePolicy::Expanding, QSizePolicy::Fixed);
    yeartitleLayout->addSpacerItem(t_spaceitem);
    yeartitleLayout->addWidget(m_prevButton);
    yeartitleLayout->addWidget(m_today);
    yeartitleLayout->addWidget(m_nextButton);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setMargin(0);
    gridLayout->setSpacing(0);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            CYearView *view = new CYearView();
            //view->setCurrentDate(QDate(date.year(),i*j + j+1,1));
            connect(view, &CYearView::singanleActiveW, this, &CYearWindow::slotActiveW);
            connect(view, &CYearView::dateSelected, this, &CYearWindow::dateSelected);
            gridLayout->addWidget(view, i, j);
            m_monthViewList.append(view);
        }
    }
    QWidget *gridWidget = new QWidget;
    gridWidget->setLayout(gridLayout);

    QVBoxLayout *hhLayout = new QVBoxLayout;
    hhLayout->addLayout(yeartitleLayout);
    hhLayout->addWidget(gridWidget);
    m_contentBackground->setLayout(hhLayout);


    setCentralWidget(m_contentBackground);
}

void CYearWindow::initConnection()
{
    connect(m_prevButton, &DArrowButton::mousePress, this, &CYearWindow::slotprev);
    connect(m_today, &DBaseButton::clicked, this, &CYearWindow::slottoday);
    connect(m_nextButton, &DArrowButton::mousePress, this, &CYearWindow::slotnext);
    connect(m_monthViewList.at(0), &CYearView::dateSelected, this, &CYearWindow::slotdateSelected);
    connect(m_monthViewList.at(0), &CYearView::datecurrentDateChanged, this, &CYearWindow::slotdatecurrentDateChanged);

}

void CYearWindow::slotActiveW(CYearView *w)
{
    if (m_activeview == nullptr) {
        m_activeview = w;
    } else if (m_activeview == w) {
        return;
    } else {
        m_activeview = w;
        for (int i = 0; i < 12; i++) {
            if (m_monthViewList.at(i) == w) {
                continue;
            }
            m_monthViewList.at(i)->updateSelectState();
        }
    }
}

void CYearWindow::slotprev()
{
    if (m_currentdate.year() > 1900) {
        m_currentdate = QDate(m_currentdate.year() - 1, m_currentdate.month(), m_currentdate.day());
        setDate(m_currentdate);
    } else {
        QMessageBox::information(this, tr("infomation"), tr("Year less than 1900!"));
    }
}

void CYearWindow::slotnext()
{
    m_currentdate = QDate(m_currentdate.year() + 1, m_currentdate.month(), m_currentdate.day());
    setDate(m_currentdate);
}

void CYearWindow::slottoday()
{
    setDate(QDate::currentDate());
}

void CYearWindow::slotdateSelected(const QDate date, const CaLunarDayInfo &detail) const
{
    if (date != QDate::currentDate()) {
        m_today->setVisible(true);
    } else {
        m_today->setVisible(false);
    }
    m_YearLabel->setText(QString::number(date.year()) + tr("Y"));
    m_YearLunarLabel->setText("-" + detail.mGanZhiYear + detail.mZodiac + "年");
}

void CYearWindow::slotdatecurrentDateChanged(const QDate date, const CaLunarDayInfo &detail) const
{
    if (date != QDate::currentDate()) {
        m_today->setVisible(true);
    } else {
        m_today->setVisible(false);
    }
    m_YearLabel->setText(QString::number(date.year()) + tr("Y"));
    m_YearLunarLabel->setText("-" + detail.mGanZhiYear + detail.mZodiac + "年-");
}
