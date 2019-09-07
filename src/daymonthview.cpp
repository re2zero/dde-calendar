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
CDayMonthView::CDayMonthView(QWidget *parent) : QWidget(parent)
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

    setFixedSize(DDEDayCalendar::D_MWindowWidth, DDEDayCalendar::D_MWindowHeight);
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
    update();
}
void CDayMonthView::setCurrentDate(const QDate date, int type)
{
    qDebug() << "set current date " << date;

    if (date == m_currentDate) {
        return;
    }

    m_currentDate = date;
    // to refresh lunar calendar
    //updateDate();
    updateCurrentLunar(getCaHuangLiDayInfo(getDateIndex(m_currentDate)));
    emit signalcurrentDateChanged(date);
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
    m_prevButton = new DImageButton(this);
    m_prevButton->setFixedWidth(DDEDayCalendar::D_MLableHeight);
    m_prevButton->setHoverPic(":/resources/icon/previous_hover.svg");
    m_prevButton->setNormalPic(":/resources/icon/previous_normal.svg");
    m_prevButton->setPressPic(":/resources/icon/previous_press.svg");
    //m_prevButton->setFixedSize(DDEDayCalendar::D_MLableHeight, DDEDayCalendar::D_MLableHeight);
    //m_prevButton->setArrowDirection(DArrowButton::ArrowLeft);
    m_nextButton = new DImageButton(this);
    //m_nextButton->setFixedSize(DDEDayCalendar::D_MLableHeight, DDEDayCalendar::D_MLableHeight);
    m_nextButton->setFixedWidth(DDEDayCalendar::D_MLableHeight);
    m_nextButton->setHoverPic(":/resources/icon/next_hover.svg");
    m_nextButton->setNormalPic(":/resources/icon/next_normal.svg");
    m_nextButton->setPressPic(":/resources/icon/next_press.svg");
    //m_nextButton->setArrowDirection(DArrowButton::ArrowRight);

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
    QPalette pa;
    pa.setColor(QPalette::WindowText, QColor("#3B3B3B"));
    m_currentMouth->setPalette(pa);
    m_currentMouth->setFont(mlabelF);
    m_currentMouth->setAlignment(Qt::AlignCenter);
    titleLayout->addWidget(m_prevButton);
    titleLayout->addWidget(m_currentMouth);
    titleLayout->addWidget(m_nextButton);
    titleLayout->addStretch(1);
    titleLayout->addWidget(m_today);
    // cells grid
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setMargin(0);
    gridLayout->setSpacing(0);
    for (int r = 0; r != 6; ++r) {
        for (int c = 0; c != 7; ++c) {
            QWidget *cell = new QWidget;
            cell->setFixedSize(DDEDayCalendar::DCellWidth, DDEDayCalendar::DCellHeight);
            cell->installEventFilter(this);
            cell->setFocusPolicy(Qt::ClickFocus);
            gridLayout->addWidget(cell, r, c);
            m_cellList.append(cell);
        }
    }
    //上半部分
    QVBoxLayout *upLayout = new QVBoxLayout;
    upLayout->setMargin(0);
    upLayout->setSpacing(0);
    upLayout->setContentsMargins(22, 9, 9, 7);
    upLayout->addLayout(titleLayout);
    upLayout->addLayout(gridLayout);


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
    QPalette daypa;
    daypa.setColor(QPalette::WindowText, QColor("#2CA7F8"));
    m_currentDay->setPalette(daypa);
    m_currentDay->setFont(daylabelF);
    midLayout->addWidget(m_currentDay);


    m_currentWeek = new DLabel(this);
    m_currentWeek->setFixedHeight(DDEDayCalendar::DWLableHeight);
    m_currentWeek->setAlignment(Qt::AlignCenter);
    QFont wlabelF;
    wlabelF.setFamily("PingFangSC-Semibold");
    wlabelF.setPixelSize(16);
    QPalette wpa;
    wpa.setColor(QPalette::WindowText, QColor("#414D68"));
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
    QPalette hpa;
    hpa.setColor(QPalette::WindowText, QColor("#414D68"));
    m_currentLuna->setPalette(hpa);
    m_currentLuna->setFont(hlabelF);
    midLayout->addWidget(m_currentLuna);


    QVBoxLayout *yidownLayout = new QVBoxLayout;
    yidownLayout->setMargin(0);
    yidownLayout->setSpacing(0);
    yidownLayout->setContentsMargins(10, 5, 10, 5);
    hlabelF.setPixelSize(14);
    m_yiLabel = new CDayHuangLiLabel(this);
    m_yiLabel->setbackgroundColor(QColor("#75C18E"));
    m_yiLabel->setTextInfo(QColor("#7B7B7B "), hlabelF);
    m_yiLabel->setFixedSize(DDEDayCalendar::DHualiDtailLableWidth, DDEDayCalendar::DHualiDtailLableHeight);
    yidownLayout->addWidget(m_yiLabel);

    QVBoxLayout *jidownLayout = new QVBoxLayout;
    jidownLayout->setMargin(0);
    jidownLayout->setSpacing(0);
    jidownLayout->setContentsMargins(10, 5, 10, 10);

    m_jiLabel = new CDayHuangLiLabel(this);
    m_jiLabel->setbackgroundColor(QColor("#C17575"));
    m_jiLabel->setTextInfo(QColor("#7B7B7B "), hlabelF);
    m_jiLabel->setFixedSize(DDEDayCalendar::DHualiDtailLableWidth, DDEDayCalendar::DHualiDtailLableHeight);
    jidownLayout->addWidget(m_jiLabel);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    // mainLayout->addWidget(m_weekIndicator, 0, Qt::AlignHCenter);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addLayout(upLayout);
    mainLayout->addLayout(midLayout);
    QFrame *frame = new QFrame(this);
    frame->setFrameShape(QFrame::HLine);
    frame->setFrameShadow(QFrame::Plain);
    frame->setLineWidth(2);
    frame->setFixedSize(241, 2);
    QHBoxLayout *hlineLayout = new QHBoxLayout;
    hlineLayout->setMargin(0);
    hlineLayout->setSpacing(0);
    hlineLayout->setContentsMargins(0, 0, 0, 3);
    hlineLayout->addStretch(1);
    hlineLayout->addWidget(frame);
    hlineLayout->addStretch(1);
    mainLayout->addLayout(hlineLayout);
    mainLayout->addLayout(yidownLayout);
    mainLayout->addLayout(jidownLayout);

    setLayout(mainLayout);
}

void CDayMonthView::initConnection()
{
    connect(m_prevButton, &DImageButton::clicked, this, &CDayMonthView::slotprev);
    connect(m_today, &DPushButton::clicked, this, &CDayMonthView::slottoday);
    connect(m_nextButton, &DImageButton::clicked, this, &CDayMonthView::slotnext);
    connect(this, &CDayMonthView::dateSelected, this, &CDayMonthView::handleCurrentDateChanged);
}

void CDayMonthView::updateDateShow()
{
    m_currentMouth->setText(QString::number(m_currentDate.month()) + tr("mon"));
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
    const QRect rect((cell->width() - DDEYearCalendar::YHeaderItemWidth) / 2,
                     (cell->height() - DDEYearCalendar::YHeaderItemHeight) / 2,
                     DDEYearCalendar::YCellHighlightWidth,
                     DDEYearCalendar::YCellHighlightHeight);

    const int pos = m_cellList.indexOf(cell);
    const bool isSelectedCell = pos == m_selectedCell;
    const bool isCurrentDay = getCellDate(pos) == QDate::currentDate();

    QPainter painter(cell);

//    painter.drawRoundedRect(cell->rect(), 4, 4);

    // draw selected cell background circle
    if (isSelectedCell) {
        QRect fillRect((cell->width() - DDEYearCalendar::YHeaderItemWidth) / 2 + 4,
                       (cell->height() - DDEYearCalendar::YHeaderItemHeight) / 2 + 1,
                       18, 18);
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
    emit signalcurrentLunarDateChanged(m_currentDate, getCaHuangLiDayInfo(getDateIndex(m_currentDate)), 1);
    updateDate();
    updateDateShow();
    updateDateLunarDay();
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

    if (lunarCache->size() > 300)
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
        bool o1 = true;

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

    } else {
        currentDayInfo = lunarCache->value(date);
    }

    m_cellList.at(pos)->update();

    // refresh   lunar info
    if (date == m_currentDate) {
        updateCurrentLunar(currentDayInfo);
    }
}
void CDayMonthView::slotprev()
{
    if (m_currentDate.year() > 1900) {
        m_currentDate = m_currentDate.addMonths(-1);
        updateCurrentLunar(getCaHuangLiDayInfo(getDateIndex(m_currentDate)));
        emit signalcurrentDateChanged(m_currentDate);
    } else {
        QMessageBox::information(this, tr("infomation"), tr("Year less than 1900!"));
    }
}

void CDayMonthView::slotnext()
{
    m_currentDate = m_currentDate.addMonths(1);
    updateCurrentLunar(getCaHuangLiDayInfo(getDateIndex(m_currentDate)));
    emit signalcurrentDateChanged(m_currentDate);
}

void CDayMonthView::slottoday()
{
    emit signalsReturnDay();
    setCurrentDate(QDate::currentDate());
}
