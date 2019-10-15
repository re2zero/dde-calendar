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
#include "daymonthview.h"
#include <QSpacerItem>
#include "constants.h"
#include "dayhuangliview.h"
#include <DPalette>
#include <DHorizontalLine>
#include <DHiDPIHelper>
DGUI_USE_NAMESPACE
CDayMonthView::CDayMonthView(QWidget *parent) : DWidget(parent)
{
    m_DBusInter = new CalendarDBus("com.deepin.api.LunarCalendar",
                                   "/com/deepin/api/LunarCalendar",
                                   QDBusConnection::sessionBus(), this);
    if (!queue)
        queue = new QQueue<int>;
    if (!lunarCache)
        lunarCache = new QMap<QDate, CaHuangLiDayInfo>;
    if (!emptyCaHuangLiDayInfo)
        emptyCaHuangLiDayInfo = new CaHuangLiDayInfo;

    m_weeklist.append(tr("Monday"));
    m_weeklist.append(tr("Tuesday"));
    m_weeklist.append(tr("Wednesday"));
    m_weeklist.append(tr("Thursday"));
    m_weeklist.append(tr("Friday"));
    m_weeklist.append(tr("Saturday"));
    m_weeklist.append(tr("Sunday"));
    initUI();
    initConnection();

    //setFixedSize(DDEDayCalendar::D_MWindowWidth, DDEDayCalendar::D_MWindowHeight);
}
void CDayMonthView::handleCurrentDateChanged(const QDate date, const CaHuangLiDayInfo &detail)
{
    Q_UNUSED(detail);
    if (date != m_currentDate) {
        setCurrentDate(date);
    }
}

void CDayMonthView::setFirstWeekday(int weekday)
{
    m_firstWeekDay = weekday;
    updateDate();
}
void CDayMonthView::setLunarVisible(bool visible)
{
    m_huanglistate = visible;
    m_yiLabel->setVisible(visible);
    m_jiLabel->setVisible(visible);
    m_currentLuna->setVisible(visible);
    update();
}

void CDayMonthView::setTheMe(int type)
{
    if (type == 0 || type == 1) {
        DPalette todaypa = m_today->palette();
        todaypa.setColor(DPalette::ButtonText, QColor("#1D81EC"));
        todaypa.setColor(DPalette::Dark, Qt::white);
        todaypa.setColor(DPalette::Light, Qt::white);
        m_today->setPalette(todaypa);

        DPalette prevpa = m_prevButton->palette();
        prevpa.setColor(DPalette::Dark, QColor("#E6E6E6"));
        prevpa.setColor(DPalette::Light, QColor("#E3E3E3"));
        //m_prevButton->setPalette(prevpa);

        DPalette nextvpa = m_nextButton->palette();
        nextvpa.setColor(DPalette::Dark, QColor("#E6E6E6"));
        nextvpa.setColor(DPalette::Light, QColor("#E3E3E3"));
        //m_nextButton->setPalette(nextvpa);

        DPalette pa = m_currentMouth->palette();
        pa.setColor(DPalette::WindowText, QColor("#3B3B3B"));
        m_currentMouth->setPalette(pa);

        DPalette daypa = m_currentDay->palette();
        daypa.setColor(DPalette::WindowText, QColor("#2CA7F8"));
        m_currentDay->setPalette(daypa);


        DPalette wpa = m_currentWeek->palette();
        wpa.setColor(DPalette::WindowText, QColor("#414D68"));
        m_currentWeek->setPalette(wpa);

        m_currentYear->setPalette(wpa);

        DPalette hpa = m_currentLuna->palette();
        hpa.setColor(DPalette::WindowText, QColor("#414D68"));
        m_currentLuna->setPalette(hpa);

        QFont hlabelF;
        hlabelF.setFamily("PingFangSC-Semibold");
        hlabelF.setPixelSize(14);

        QColor yicolor = QColor("#75C18E");
        yicolor.setAlphaF(0.1);
        m_yiLabel->setbackgroundColor(yicolor);
        m_yiLabel->setTextInfo(QColor("#7B7B7B"), hlabelF);
        m_yiLabel->update();

        QColor jicolor = QColor("#C17575");
        jicolor.setAlphaF(0.1);
        m_jiLabel->setbackgroundColor(jicolor);
        m_jiLabel->setTextInfo(QColor("#7B7B7B"), hlabelF);
        m_jiLabel->update();

        m_topBorderColor = Qt::red;
        m_backgroundCircleColor = "#2ca7f8";

        m_defaultTextColor = Qt::black;
        m_currentDayTextColor = "#2ca7f8";
        m_weekendsTextColor = Qt::black;
        m_selectedTextColor = Qt::white;
        m_festivalTextColor = Qt::black;
        m_notCurrentTextColor = "#b2b2b2";


    } else if (type == 2) {
        DPalette todaypa = m_today->palette();
        todaypa.setColor(DPalette::ButtonText, QColor("#0081FF"));
        todaypa.setColor(DPalette::Light, "#484848");
        todaypa.setColor(DPalette::Dark, "#414141");
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

        DPalette pa = m_currentMouth->palette();
        pa.setColor(DPalette::WindowText, QColor("#C0C6D4"));
        m_currentMouth->setPalette(pa);

        DPalette daypa = m_currentDay->palette();
        daypa.setColor(DPalette::WindowText, QColor("#0081FF"));
        m_currentDay->setPalette(daypa);


        DPalette wpa = m_currentWeek->palette();
        wpa.setColor(DPalette::WindowText, QColor("#C0C6D4"));
        m_currentWeek->setPalette(wpa);

        m_currentYear->setPalette(wpa);

        DPalette hpa = m_currentLuna->palette();
        hpa.setColor(DPalette::WindowText, QColor("#C0C6D4"));
        m_currentLuna->setPalette(hpa);

        QFont hlabelF;
        hlabelF.setFamily("PingFangSC-Semibold");
        hlabelF.setPixelSize(14);

        m_yiLabel->setbackgroundColor(QColor("#2F8C4D"));
        m_yiLabel->setTextInfo(QColor("#C0C6D4"), hlabelF);
        m_yiLabel->update();

        m_jiLabel->setbackgroundColor(QColor("#A43B3B"));
        m_jiLabel->setTextInfo(QColor("#C0C6D4"), hlabelF);
        m_jiLabel->update();

        m_topBorderColor = Qt::red;
        m_backgroundCircleColor = "#2ca7f8";

        m_defaultTextColor = "#C0C6D4";
        m_currentDayTextColor = "#0081FF";
        m_weekendsTextColor = Qt::black;
        m_selectedTextColor = "#B8D3FF";
        m_festivalTextColor = Qt::black;
        m_notCurrentTextColor = "#C0C6D4";
    }

    for (int i(0); i != 42; ++i) {
        m_cellList.at(i)->update();
    }
}
void CDayMonthView::setCurrentDate(const QDate date, int type)
{
    qDebug() << "set current date " << date;

    if (date == m_currentDate) {
        return;
    }

    if (date == QDate::currentDate()) {
        m_today->hide();
    } else {
        m_today->show();
    }
    m_currentDate = date;
    // to refresh lunar calendar
    //updateDate();
    emit signalcurrentDateChanged(date);
    updateCurrentLunar(getCaHuangLiDayInfo(getDateIndex(m_currentDate)));
}

void CDayMonthView::setCellSelectable(bool selectable)
{
    if (selectable == m_cellSelectable)
        return;
    m_cellSelectable = selectable;
}

int CDayMonthView::getDateIndex(const QDate &date) const
{
    for (int i = 0; i != 42; ++i)
        if (m_days[i] == date)
            return i;

    return 0;
}

bool CDayMonthView::eventFilter(QObject *o, QEvent *e)
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

void CDayMonthView::updateDate()
{
    const QDate firstDay(m_currentDate.year(), m_currentDate.month(), 1);
    const int day = (firstDay.dayOfWeek() + m_firstWeekDay) % 7;
    const int currentIndex = day + m_currentDate.day() - 1;

    if (currentIndex < 0) {
        return;
    }

    for (int i(0); i != 42; ++i) {
        m_days[i] = firstDay.addDays(i - day);
    }

    setSelectedCell(currentIndex);
    update();
}

void CDayMonthView::initUI()
{
    m_dayNumFont.setFamily("Helvetica");
    m_dayNumFont.setPixelSize(12);
    m_dayNumFont.setWeight(QFont::Light);

    m_today = new DPushButton;
    m_today->setText(tr("Return today"));
    m_today->setFixedSize(100, DDEDayCalendar::D_MLableHeight);
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
    m_prevButton->setFixedSize(36, 36);
    // m_prevButton->setIconSize(QSize(36, 36));
    // m_prevButton->setIcon(QIcon(DHiDPIHelper::loadNxPixmap(":/resources/icon/previous_normal.svg")));

    m_nextButton = new DIconButton(DStyle::SP_ArrowRight, this);
    //m_nextButton->setFixedSize(DDEDayCalendar::D_MLableHeight, DDEDayCalendar::D_MLableHeight);
    m_nextButton->setFixedSize(36, 36);
    //m_nextButton->setIconSize(QSize(36, 36));
    //m_nextButton->setIcon(QIcon(DHiDPIHelper::loadNxPixmap(":/resources/icon/next_normal.svg")));


    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->setMargin(0);
    titleLayout->setSpacing(0);
    titleLayout->setContentsMargins(0, 0, 0, 3);
    //add separator line
    m_currentMouth = new DLabel(this);
    m_currentMouth->setFixedSize(74, DDEDayCalendar::D_MLableHeight);
    QFont mlabelF;
    mlabelF.setFamily("SourceHanSansSC-Medium");
    mlabelF.setPixelSize(24);
    DPalette pa = m_currentMouth->palette();
    pa.setColor(DPalette::WindowText, QColor("#3B3B3B"));
    m_currentMouth->setPalette(pa);
    m_currentMouth->setFont(mlabelF);
    m_currentMouth->setAlignment(Qt::AlignCenter);
    titleLayout->addWidget(m_prevButton);
    titleLayout->addWidget(m_currentMouth);
    titleLayout->addWidget(m_nextButton);
    titleLayout->addStretch(1);
    titleLayout->addWidget(m_today);
    // cells grid
    m_gridLayout = new QGridLayout;
    m_gridLayout->setMargin(0);
    m_gridLayout->setSpacing(0);
    for (int r = 0; r != 6; ++r) {
        for (int c = 0; c != 7; ++c) {
            QWidget *cell = new QWidget;
            cell->setFixedSize(DDEDayCalendar::DCellWidth, DDEDayCalendar::DCellHeight);
            cell->installEventFilter(this);
            cell->setFocusPolicy(Qt::ClickFocus);
            m_gridLayout->addWidget(cell, r, c);
            m_cellList.append(cell);
        }
    }
    //上半部分
    m_upLayout = new QVBoxLayout;
    m_upLayout->setMargin(0);
    m_upLayout->setSpacing(0);
    m_upLayout->setContentsMargins(22, 9, 9, 7);
    m_upLayout->addLayout(titleLayout);
    m_upLayout->addLayout(m_gridLayout);


    //中间部分
    QVBoxLayout *midLayout = new QVBoxLayout;
    midLayout->setMargin(0);
    midLayout->setSpacing(0);
    midLayout->setContentsMargins(0, 0, 0, 20);
    m_currentDay = new DLabel(this);
    m_currentDay->setFixedHeight(DDEDayCalendar::DDLableHeight);
    m_currentDay->setAlignment(Qt::AlignCenter);
    QFont daylabelF;
    daylabelF.setFamily("DINAlternate-Bold");
    daylabelF.setPixelSize(100);
    DPalette daypa = m_currentDay->palette();
    daypa.setColor(DPalette::WindowText, QColor("#2CA7F8"));
    m_currentDay->setPalette(daypa);
    m_currentDay->setFont(daylabelF);
    midLayout->addWidget(m_currentDay);


    m_currentWeek = new DLabel(this);
    m_currentWeek->setFixedHeight(DDEDayCalendar::DWLableHeight);
    m_currentWeek->setAlignment(Qt::AlignCenter);
    QFont wlabelF;
    wlabelF.setFamily("PingFangSC-Semibold");
    wlabelF.setPixelSize(16);
    DPalette wpa = m_currentWeek->palette();
    wpa.setColor(DPalette::WindowText, QColor("#414D68"));
    m_currentWeek->setPalette(wpa);
    m_currentWeek->setFont(wlabelF);
    midLayout->addWidget(m_currentWeek);

    m_currentYear = new DLabel(this);
    m_currentYear->setFixedHeight(DDEDayCalendar::DWLableHeight);
    m_currentYear->setAlignment(Qt::AlignCenter);
    m_currentYear->setFont(wlabelF);
    m_currentYear->setPalette(wpa);
    midLayout->addWidget(m_currentYear);

    m_currentLuna = new DLabel(this);
    m_currentLuna->setFixedHeight(DDEDayCalendar::DHualiInfoLableHeight);
    m_currentLuna->setAlignment(Qt::AlignCenter);
    QFont hlabelF;
    hlabelF.setFamily("PingFangSC-Semibold");
    hlabelF.setPixelSize(12);
    DPalette hpa = m_currentLuna->palette();
    hpa.setColor(DPalette::WindowText, QColor("#414D68"));
    m_currentLuna->setPalette(hpa);
    m_currentLuna->setFont(hlabelF);
    midLayout->addWidget(m_currentLuna);


    m_yidownLayout = new QVBoxLayout;
    m_yidownLayout->setMargin(0);
    m_yidownLayout->setSpacing(0);
    m_yidownLayout->setContentsMargins(10, 5, 10, 0);
    hlabelF.setPixelSize(14);
    m_yiLabel = new CDayHuangLiLabel(this);
    m_yiLabel->setbackgroundColor(QColor("#75C18E"));
    m_yiLabel->setTextInfo(QColor("#7B7B7B "), hlabelF);
    m_yiLabel->setFixedSize(DDEDayCalendar::DHualiDtailLableWidth, DDEDayCalendar::DHualiDtailLableHeight);
    m_yidownLayout->addWidget(m_yiLabel);

    m_jidownLayout = new QVBoxLayout;
    m_jidownLayout->setMargin(0);
    m_jidownLayout->setSpacing(0);
    m_jidownLayout->setContentsMargins(10, 10, 10, 10);

    m_jiLabel = new CDayHuangLiLabel(this);
    m_jiLabel->setbackgroundColor(QColor("#C17575"));
    m_jiLabel->setTextInfo(QColor("#7B7B7B "), hlabelF);
    m_jiLabel->setFixedSize(DDEDayCalendar::DHualiDtailLableWidth, DDEDayCalendar::DHualiDtailLableHeight);
    m_jidownLayout->addWidget(m_jiLabel);

    m_hhLayout = new QVBoxLayout;
    // mainLayout->addWidget(m_weekIndicator, 0, Qt::AlignHCenter);
    m_hhLayout->setMargin(0);
    m_hhLayout->setSpacing(0);
    m_hhLayout->addLayout(m_upLayout);
    m_hhLayout->addLayout(midLayout);

    m_splitline = new DHorizontalLine;

    //QFrame *frame = new QFrame(this);
    //frame->setFrameShape(QFrame::HLine);
    //frame->setFrameShadow(QFrame::Plain);
    //frame->setLineWidth(2);
    //frame->setFixedSize(241, 2);
    m_splitline->setFixedSize(241, 2);
    QHBoxLayout *hlineLayout = new QHBoxLayout;
    hlineLayout->setMargin(0);
    hlineLayout->setSpacing(0);
    hlineLayout->setContentsMargins(0, 0, 0, 3);
    hlineLayout->addStretch(1);
    hlineLayout->addWidget(m_splitline);
    hlineLayout->addStretch(1);
    m_hhLayout->addLayout(hlineLayout);
    m_hhLayout->addLayout(m_yidownLayout);
    m_hhLayout->addLayout(m_jidownLayout);
    m_hhLayout->addStretch();

    setLayout(m_hhLayout);
}

void CDayMonthView::initConnection()
{
    connect(m_prevButton, &DIconButton::clicked, this, &CDayMonthView::slotprev);
    connect(m_today, &DPushButton::clicked, this, &CDayMonthView::slottoday);
    connect(m_nextButton, &DIconButton::clicked, this, &CDayMonthView::slotnext);
    connect(this, &CDayMonthView::dateSelected, this, &CDayMonthView::handleCurrentDateChanged);
}

void CDayMonthView::updateDateShow()
{
    QLocale locale;
    //QString monthName(int month, QLocale::FormatType type = LongFormat)
    m_currentMouth->setText(locale.monthName(m_currentDate.month(), QLocale::ShortFormat));
    // m_currentMouth->setText(QString::number(m_currentDate.month()) + tr("mon"));
    m_currentDay->setText(QString::number(m_currentDate.day()));
    if (m_currentDate.dayOfWeek() > 0)
        m_currentWeek->setText(m_weeklist.at(m_currentDate.dayOfWeek() - 1));
    m_currentYear->setText(m_currentDate.toString("yyyy/M"));
}

void CDayMonthView::updateDateLunarDay()
{
    if (!m_huanglistate) return;
    CaHuangLiDayInfo info = getCaHuangLiDayInfo(getDateIndex(m_currentDate));
    m_currentLuna->setText(info.mGanZhiYear + tr("Y ") + tr("【") + info.mZodiac + tr("Y】") + info.mGanZhiMonth + tr("M ") + info.mGanZhiDay + tr("D "));
    QStringList yilist = info.mSuit.split(".", QString::SkipEmptyParts);
    QStringList jilist = info.mAvoid.split(".", QString::SkipEmptyParts);
    m_yiLabel->setHuangLiText(yilist);
    m_jiLabel->setHuangLiText(jilist, 1);
}
const QString CDayMonthView::getCellDayNum(int pos)
{
    return QString::number(m_days[pos].day());
}

const QDate CDayMonthView::getCellDate(int pos)
{
    return m_days[pos];
}
void CDayMonthView::paintCell(QWidget *cell)
{
    const QRect rect(0, 0, cellwidth, cellheight);

    const int pos = m_cellList.indexOf(cell);
    const bool isSelectedCell = pos == m_selectedCell;
    const bool isCurrentDay = getCellDate(pos) == QDate::currentDate();

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

void CDayMonthView::cellClicked(QWidget *cell)
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

void CDayMonthView::setSelectedCell(int index)
{
    if (m_selectedCell == index)
        return;

    const int prevPos = m_selectedCell;
    m_selectedCell = index;

    updateDate();
    m_cellList.at(prevPos)->update();
    m_cellList.at(index)->update();
    emit signalcurrentDateChanged(m_days[index]);
    emit dateSelected(m_days[index], getCaHuangLiDayInfo(getDateIndex(m_currentDate)));
}
void CDayMonthView::updateCurrentLunar(const CaHuangLiDayInfo &info)
{
    updateDate();
    updateDateShow();
    updateDateLunarDay();
    emit signalcurrentLunarDateChanged(m_currentDate, getCaHuangLiDayInfo(getDateIndex(m_currentDate)), 1);
}
const QString CDayMonthView::getLunar(int pos)
{
    CaHuangLiDayInfo info = getCaHuangLiDayInfo(pos);

    if (info.mLunarDayName == "初一") {
        info.mLunarDayName = info.mLunarMonthName + info.mLunarDayName;
    }

    if (info.mTerm.isEmpty())
        return info.mLunarDayName;

    return info.mTerm;
}

const CaHuangLiDayInfo CDayMonthView::getCaHuangLiDayInfo(int pos)
{
    const QDate date = m_days[pos];

    if (lunarCache->contains(date)) {
        return lunarCache->value(date);
    }

    if (lunarCache->size() > 40)
        lunarCache->clear();

//    QTimer::singleShot(500, [this, pos] {getDbusData(pos);});
    queue->push_back(pos);

    QTimer::singleShot(300, this, SLOT(getDbusData()));

    return *emptyCaHuangLiDayInfo;
}

void CDayMonthView::getDbusData()
{
    if (queue->isEmpty())
        return;

    const int pos = queue->head();
    queue->pop_front();
    const QDate date = m_days[pos];
    if (!date.isValid()) {
        return;
    }

    CaHuangLiDayInfo currentDayInfo;
    if (!lunarCache->contains(date)) {
        bool o1 = false;
#if 0
        CaHuangLiMonthInfo currentMonth;
        if (m_DBusInter->GetHuangLiMonthCalendar(date.year(), date.month(), o1, currentMonth)) {
            QDate cacheDate;
            cacheDate.setDate(date.year(), date.month(), 1);
            foreach (const CaHuangLiDayInfo &dayInfo, currentMonth.mCaLunarDayInfo) {
                lunarCache->insert(cacheDate, dayInfo);
                if (date == m_currentDate) {
                    currentDayInfo = dayInfo;
                }
                cacheDate = cacheDate.addDays(1);
            }
        }
#else
        CaHuangLiDayInfo scurrentDayinfo;
        if (m_DBusInter->GetHuangLiDayCalendar(date.year(), date.month(), date.day(), scurrentDayinfo)) {
            lunarCache->insert(date, scurrentDayinfo);
            currentDayInfo = scurrentDayinfo;
        }
#endif


    } else {
        currentDayInfo = lunarCache->value(date);
    }

    m_cellList.at(pos)->update();

    // refresh   lunar info
    if (date == m_currentDate) {
        updateCurrentLunar(currentDayInfo);
    }
}

void CDayMonthView::resizeEvent(QResizeEvent *event)
{
    cellwidth = width() * 0.1005 + 0.5;
    cellheight = height() * 0.0496 + 0.5;
    m_gridLayout->setHorizontalSpacing(width() * 0.0287 + 0.5);
    m_gridLayout->setVerticalSpacing(0);
    int leftmagin = width() * 0.0632 + 0.5;
    int rightmagin = leftmagin;
    int topmagin = height() * 0.0164 + 0.5;
    int buttonmagin = topmagin;
    m_upLayout->setContentsMargins(leftmagin, topmagin, rightmagin, buttonmagin);
    m_dayNumFont.setPixelSize(12 + (width() - 348) / 71.66);
    for (int i(0); i != 42; ++i) {
        m_cellList.at(i)->setFixedSize(cellwidth, cellheight);
        m_cellList.at(i)->update();
    }
    m_splitline->setFixedWidth(0.6925 * width() + 0.5);

    int hleftmagin = width() * 0.026 + 0.5;
    int hrightmagin = hleftmagin;
    int htopmagin = height() * 0.01773 + 0.5;
    int hbuttonmagin = htopmagin;
    int lw = width() - hleftmagin * 2;
    int lh = height() * 0.0992;
    m_yiLabel->setFixedSize(lw, lh);
    m_yidownLayout->setContentsMargins(hleftmagin, htopmagin * 0.5, hrightmagin, 0);
    m_jiLabel->setFixedSize(lw, lh);
    m_jidownLayout->setContentsMargins(hleftmagin, htopmagin, hrightmagin, hbuttonmagin);
}
void CDayMonthView::slotprev()
{
    if (m_currentDate.year() > 1900) {
        m_currentDate = m_currentDate.addMonths(-1);
        emit signalcurrentDateChanged(m_currentDate);
        updateCurrentLunar(getCaHuangLiDayInfo(getDateIndex(m_currentDate)));
    } else {
        QMessageBox::information(this, tr("infomation"), tr("Year less than 1900!"));
    }
}

void CDayMonthView::slotnext()
{
    m_currentDate = m_currentDate.addMonths(1);
    emit signalcurrentDateChanged(m_currentDate);
    updateCurrentLunar(getCaHuangLiDayInfo(getDateIndex(m_currentDate)));
}

void CDayMonthView::slottoday()
{
    emit signalsReturnDay();
    setCurrentDate(QDate::currentDate());
}
