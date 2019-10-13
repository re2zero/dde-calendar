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
#include "customcalendarwidget.h"
#include <QLocale>
#include <QPainter>
#include <QTextCharFormat>
#include <QProxyStyle>
#include <QTableView>
#include <QLayout>
#include <QPushButton>
#include <QLabel>
#include "cayearmonthedit.h"
#include "scheduledatamanage.h"
#include <DHiDPIHelper>
class QCustomStyle : public QProxyStyle
{
public:
    QCustomStyle(QWidget *parent)
    {
        setParent(parent);
    };

private:
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget) const
    {
        if (element == PE_FrameFocusRect) {
            return;
        }
        QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
};

CCustomCalendarWidget::CCustomCalendarWidget(QWidget *parent)
    : DCalendarWidget(parent)
{
    initControl();
    //setFixedSize(300, 300);
}

CCustomCalendarWidget::~CCustomCalendarWidget()
{

}

void CCustomCalendarWidget::initControl()
{
    layout()->setSizeConstraint(QLayout::SetMinimumSize);
    setLocale(QLocale(QLocale::Chinese));
    setNavigationBarVisible(false);
    setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    setHorizontalHeaderFormat(QCalendarWidget::SingleLetterDayNames);
    setStyle(new QCustomStyle(this));
    int themetype = CScheduleDataManage::getScheduleDataManage()->getTheme();
    if (themetype == 0 || themetype == 1) {
        m_backgroundCircleColor = "#2ca7f8";

        m_defaultTextColor = Qt::black;
        m_currentDayTextColor = "#2ca7f8";
        m_weekendsTextColor = Qt::black;
        m_selectedTextColor = Qt::white;
        m_notCurrentTextColor = "#b2b2b2";
        m_defaultBColor = Qt::white;
    } else {
        m_backgroundCircleColor = "#0081FF";

        m_defaultTextColor = "#C0C6D4";
        m_currentDayTextColor = "#0081FF";
        m_weekendsTextColor = Qt::black;
        m_selectedTextColor = "#C0C6D4";
        m_notCurrentTextColor = "#919191";
        m_defaultBColor = "#191919";
        m_defaultBColor.setAlphaF(0.8);
    }



    QTextCharFormat format;
    format.setForeground(m_defaultTextColor);
    format.setBackground(m_defaultBColor);
    QTextCharFormat sformat;
    sformat.setForeground(m_backgroundCircleColor);
    sformat.setBackground(m_defaultBColor);
    setHeaderTextFormat(format);
    setWeekdayTextFormat(Qt::Saturday, sformat);
    setWeekdayTextFormat(Qt::Sunday, sformat);
    setWeekdayTextFormat(Qt::Monday, format);
    setWeekdayTextFormat(Qt::Tuesday, format);
    setWeekdayTextFormat(Qt::Wednesday, format);
    setWeekdayTextFormat(Qt::Thursday, format);
    setWeekdayTextFormat(Qt::Friday, format);

    initTopWidget();
    //initBottomWidget();

    connect(this, &QCalendarWidget::currentPageChanged, [this](int year, int month) {
        setDataLabelTimeText(year, month);
    });
    QTableView *view = findChild<QTableView *>("qt_calendar_calendarview");
    if (view) {
        view->setFixedSize(400, 300);
    }
}

void CCustomCalendarWidget::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
{
    if (date == selectedDate()) {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(m_backgroundCircleColor);

        painter->drawRoundedRect(rect.x(), rect.y() + 3, rect.width(), rect.height() - 6, 3, 3);
        painter->setPen(m_selectedTextColor);

        painter->drawText(rect, Qt::AlignCenter, QString::number(date.day()));
        painter->restore();
    } else if (date == QDate::currentDate()) {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(m_backgroundCircleColor);
        painter->drawRoundedRect(rect.x(), rect.y() + 3, rect.width(), rect.height() - 6, 3, 3);
        painter->setBrush(m_defaultBColor);
        painter->drawRoundedRect(rect.x() + 1, rect.y() + 4, rect.width() - 2, rect.height() - 8, 2, 2);
        painter->setPen(m_currentDayTextColor);
        painter->drawText(rect, Qt::AlignCenter, QString::number(date.day()));
        painter->restore();
    } else  {
        //QCalendarWidget::paintCell(painter, rect, date);
        //return;
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(Qt::NoPen);
        painter->setBrush(m_defaultBColor);
        painter->drawRect(rect.x(), rect.y() + 3, rect.width(), rect.height() - 6);
        if (date.month() == m_monthLabel->getNum()) {
            painter->setPen(m_defaultTextColor);
            if (date.dayOfWeek() == 6 || date.dayOfWeek() == 7)
                painter->setPen(m_backgroundCircleColor);
        } else {
            painter->setPen(m_notCurrentTextColor);
        }
        painter->drawText(rect, Qt::AlignCenter, QString::number(date.day()));
        painter->restore();
    }
}

void CCustomCalendarWidget::resizeEvent(QResizeEvent *e)
{
    QCalendarWidget::resizeEvent(e);
}

void CCustomCalendarWidget::initTopWidget()
{
    QWidget *topWidget = new QWidget(this);
    topWidget->setObjectName("CalendarTopWidget");
    topWidget->setFixedHeight(40);
    topWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->setContentsMargins(12, 0, 12, 0);
    hboxLayout->setSpacing(4);

    m_leftMonthBtn = new DIconButton(DStyle::SP_ArrowLeft, this);
    m_leftMonthBtn->setFixedSize(36, 36);
    // m_leftMonthBtn->setIconSize(QSize(36, 36));
    //m_leftMonthBtn->setIcon(QIcon(DHiDPIHelper::loadNxPixmap(":/resources/icon/previous_normal.svg")));




    m_rightMonthBtn = new DIconButton(DStyle::SP_ArrowRight, this);
    m_rightMonthBtn->setFixedSize(36, 36);
    //m_rightMonthBtn->setIconSize(QSize(36, 36));
    //m_rightMonthBtn->setIcon(QIcon(DHiDPIHelper::loadNxPixmap(":/resources/icon/next_normal.svg")));


    //m_dataLabel = new QLabel(this);
    m_monthLabel = new CCaYearMonthEdit(this);
    m_yearLabel = new CCaYearMonthEdit(this);
    m_monthLabel->setRange(1, 12);
    m_yearLabel->setRange(1900, 9999);
    m_monthLabel->setMinimumWidth(100);
    m_monthLabel->setPadStr(tr("M"));
    m_yearLabel->setPadStr(tr("Y"));
    m_yearLabel->setMinimumWidth(120);

    m_leftMonthBtn->setObjectName("CalendarLeftMonthBtn");
    m_rightMonthBtn->setObjectName("CalendarRightMonthBtn");
    hboxLayout->addWidget(m_leftMonthBtn);
    hboxLayout->addStretch();
    hboxLayout->addWidget(m_monthLabel);
    hboxLayout->addWidget(m_yearLabel);
    hboxLayout->addStretch();
    hboxLayout->addWidget(m_rightMonthBtn);
    topWidget->setLayout(hboxLayout);

    //这里见下图1
    QVBoxLayout *vBodyLayout = qobject_cast<QVBoxLayout *>(layout());
    vBodyLayout->insertWidget(0, topWidget);

    //vBodyLayout->setSizeConstraint(QLayout::SetMinimumSize);
    //connect(m_leftMonthBtn, SIGNAL(clicked()), this, SLOT(onbtnClicked()));
    //connect(m_rightMonthBtn, SIGNAL(clicked()), this, SLOT(onbtnClicked()));
    connect(m_leftMonthBtn, &DIconButton::clicked, this, &CCustomCalendarWidget::slotABtPrivMonth);
    connect(m_rightMonthBtn, &DIconButton::clicked, this, &CCustomCalendarWidget::slotABtNextMonth);

    connect(m_monthLabel, &CCaYearMonthEdit::signalsCurrentValueChange, this, &CCustomCalendarWidget::slotmonthChange);
    connect(m_yearLabel, &CCaYearMonthEdit::signalsCurrentValueChange, this, &CCustomCalendarWidget::slotyearChange);

    setDataLabelTimeText(selectedDate().year(), selectedDate().month());
}

void CCustomCalendarWidget::initBottomWidget()
{

}

void CCustomCalendarWidget::setDataLabelTimeText(int year, int month)
{
    m_monthLabel->setCurrentNum(month);
    m_yearLabel->setCurrentNum(year);
    //m_dataLabel->setText(QStringLiteral("%1年%2月").arg(year).arg(month));
}

void CCustomCalendarWidget::onbtnClicked()
{
    //  QPushButton *senderBtn = qobject_cast<QPushButton *>(sender());
    //if (senderBtn == m_leftMonthBtn) {
    //    showPreviousMonth();
    //} else if (senderBtn == m_rightMonthBtn) {
    //    showNextMonth();
    //}
}

void CCustomCalendarWidget::slotmonthChange(int value)
{
    QDate date = selectedDate();
    setSelectedDate(QDate(date.year(), value, date.day()));
}

void CCustomCalendarWidget::slotyearChange(int value)
{
    QDate date = selectedDate();
    setSelectedDate(QDate( value, date.month(), date.day()));
}

void CCustomCalendarWidget::slotABtPrivMonth()
{
    showPreviousMonth();
}

void CCustomCalendarWidget::slotABtNextMonth()
{
    showNextMonth();
}
