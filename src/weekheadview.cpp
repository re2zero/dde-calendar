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

#include "weekheadview.h"
#include "calendardbus.h"

#include <QHBoxLayout>
#include <QPainter>
#include <QEvent>
#include <QDebug>
#include <QMessageBox>
#include <QTime>
#include <QQueue>
#include <QMenu>
CWeekHeadView::CWeekHeadView(QWidget *parent) : QWidget(parent)
{
    m_DBusInter = new CalendarDBus("com.deepin.api.LunarCalendar",
                                   "/com/deepin/api/LunarCalendar",
                                   QDBusConnection::sessionBus(), this);
    if (!queue)
        queue = new QQueue<int>;
    if (!lunarCache)
        lunarCache = new QMap<QDate, CaLunarDayInfo>;
    if (!emptyCaLunarDayInfo)
        emptyCaLunarDayInfo = new CaLunarDayInfo;

    m_dayNumFont.setFamily("SourceHanSansSC-Medium");
    m_dayNumFont.setPixelSize(16);
    m_monthFont.setFamily("SourceHanSansSC-Medium");
    m_monthFont.setPixelSize(20);

    // cells grid
    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->setMargin(0);
    hboxLayout->setSpacing(0);

    m_monthLabel = new DLabel(this);
    m_monthLabel->setFixedSize(DDEWeekCalendar::WMCellHeadrWidth - 5, DDEWeekCalendar::WCellHeaderItemHeight);
    m_monthLabel->setAlignment(Qt::AlignCenter);
    QPalette monthpa = m_monthLabel->palette();
    QColor textC = monthpa.color(QPalette::Text);
    QColor textbC(230, 238, 242);
    monthpa.setColor(QPalette::WindowText, textC);
    monthpa.setColor(QPalette::Background, textbC);
    m_monthLabel->setAutoFillBackground(true);
    m_monthLabel->setPalette(monthpa);
    QFont mlabelF;
    mlabelF.setFamily("SourceHanSansSC-Medium");
    mlabelF.setPixelSize(20);
    m_monthLabel->setFont(mlabelF);
    // m_monthLabel->setStyleSheet("color:#000000;background: rgb(230,238,242);");
    hboxLayout->addWidget(m_monthLabel);
    for (int c = 0; c != 7; ++c) {
        QWidget *cell = new QWidget(this);
        cell->setFixedSize(DDEWeekCalendar::WCellHeaderItemWidth, DDEWeekCalendar::WCellHeaderItemHeight);
        cell->installEventFilter(this);
        cell->setFocusPolicy(Qt::ClickFocus);
        hboxLayout->addWidget(cell);
        m_cellList.append(cell);
    }

    setLayout(hboxLayout);

    connect(this, &CWeekHeadView::dateSelected, this, &CWeekHeadView::handleCurrentDateChanged);
}

void CWeekHeadView::handleCurrentDateChanged(const QDate date, const CaLunarDayInfo &detail)
{
    Q_UNUSED(detail);

    if (date != m_currentDate) {
        setCurrentDate(date);
    }
}

void CWeekHeadView::setFirstWeekday(int weekday)
{
    m_firstWeekDay = weekday;
    updateDate();
}

int CWeekHeadView::getDateType(const QDate &date)
{
    const int currentIndex = getDateIndex(date);
    const CaLunarDayInfo info = getCaLunarDayInfo(currentIndex);

    const int dayOfWeek = date.dayOfWeek();
    bool weekends = dayOfWeek == 6 || dayOfWeek == 7;
    bool isCurrentMonth = m_currentDate.month() == date.month();
    bool isFestival = !info.mSolarFestival.isEmpty() || !info.mLunarFestival.isEmpty();

    int resultFlag = 0;
    if (!isCurrentMonth)
        resultFlag |= SO_MNotCurrentMonth;
    if (isFestival)
        resultFlag |= SO_MFestival;
    if (weekends)
        resultFlag |= SO_MWeekends;

    return resultFlag;
}

void CWeekHeadView::setWeekDay(QVector<QDate> vDays)
{
    if (vDays.count() != 7) return;
    for (int i = 0; i != 7; ++i)
        m_days[i] = vDays[i];
}

void CWeekHeadView::setCurrentDate(const QDate date)
{
    qDebug() << "set current date " << date;

    if (date == m_currentDate) {
        return;
    }

    //m_selectedCell = 0;
    m_currentDate = date;

    // to refresh lunar calendar
    updateCurrentLunar(getCaLunarDayInfo(getDateIndex(m_currentDate)));
    m_monthLabel->setText(QString::number(date.month()) + tr("Mon"));

    emit signalcurrentDateChanged(m_currentDate);
}

void CWeekHeadView::setLunarVisible(bool visible)
{
    int state = int(m_showState);

    if (visible)
        state |= ShowLunar;
    else
        state &= ~ShowLunar;

    m_showState = ShowState(state);
    update();
}

void CWeekHeadView::setLunarFestivalHighlight(bool highlight)
{
    highlight = true;
    int state = int(m_showState);
    //

    if (highlight)
        state |= ShowLunarFestivalHighlight;
    else
        state &= ~ShowLunarFestivalHighlight;

    m_showState = ShowState(state);
    update();
}

void CWeekHeadView::setCellSelectable(bool selectable)
{
    if (selectable == m_cellSelectable)
        return;
    m_cellSelectable = selectable;

    emit cellSelectableChanged(m_cellSelectable);
}

int CWeekHeadView::getDateIndex(const QDate &date) const
{
    for (int i = 0; i != 7; ++i)
        if (m_days[i] == date)
            return i;

    return 0;
}

bool CWeekHeadView::eventFilter(QObject *o, QEvent *e)
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

void CWeekHeadView::updateDate()
{
    const int currentIndex = getDateIndex(m_currentDate);
    setSelectedCell(currentIndex);
    update();
}

void CWeekHeadView::updateCurrentLunar(const CaLunarDayInfo &info)
{
    if (!info.mLunarFestival.isEmpty()) {
        emit currentFestivalChanged(info.mLunarFestival);
    } else if (!info.mTerm.isEmpty()) {
        emit currentFestivalChanged(info.mTerm);
    } else if (!info.mSolarFestival.isEmpty()) {
        QStringList tmpFestival = info.mSolarFestival.split(" ");

        if (tmpFestival.length() >= 3) {
            emit currentFestivalChanged(QString("%1 %2").arg(tmpFestival[0]).arg(tmpFestival[1]));
        } else {
            emit currentFestivalChanged(info.mSolarFestival);
        }
    } else {
        emit currentFestivalChanged("");
    }
    emit signalcurrentLunarDateChanged(m_currentDate, getCaLunarDayInfo(getDateIndex(m_currentDate)), 1);
    updateDate();
}

const QString CWeekHeadView::getCellDayNum(int pos)
{
    //if (m_days[pos].day() ==  1) {
    //  return QString::number(m_days[pos].month()) + "/" + QString::number(m_days[pos].day());
    //}
    return QString::number(m_days[pos].day());
}

const QDate CWeekHeadView::getCellDate(int pos)
{
    return m_days[pos];
}

const QString CWeekHeadView::getLunar(int pos)
{
    CaLunarDayInfo info = getCaLunarDayInfo(pos);

    if (info.mLunarDayName == "初一") {
        info.mLunarDayName =  info.mLunarDayName;
    }

    if (info.mTerm.isEmpty())
        return info.mLunarDayName;

    return info.mTerm;
}

const CaLunarDayInfo CWeekHeadView::getCaLunarDayInfo(int pos)
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

    return *emptyCaLunarDayInfo;
}

void CWeekHeadView::getDbusData()
{
    if (queue->isEmpty())
        return;

    const int pos = queue->head();
    queue->pop_front();
    const QDate date = m_days[pos];
    if (!date.isValid()) {
        return;
    }

    CaLunarDayInfo currentDayInfo;
    if (!lunarCache->contains(date)) {
        bool o1 = true;
        QDBusReply<CaLunarMonthInfo> reply = m_DBusInter->GetLunarMonthCalendar(date.year(), date.month(), false, o1);

        QDate cacheDate;
        cacheDate.setDate(date.year(), date.month(), 1);
        foreach (const CaLunarDayInfo &dayInfo, reply.value().mCaLunarDayInfo) {
            lunarCache->insert(cacheDate, dayInfo);
            if (date == m_currentDate) {
                currentDayInfo = dayInfo;
            }
            cacheDate = cacheDate.addDays(1);
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

void CWeekHeadView::paintCell(QWidget *cell)
{
    const QRect rect(0, 0, cell->width(), cell->height());

    const int pos = m_cellList.indexOf(cell);
    const int type = getDateType(m_days[pos]);
    const bool isSelectedCell = pos == m_selectedCell;
    const bool isCurrentDay = getCellDate(pos) == QDate::currentDate();

    int d = checkDay(pos - m_firstWeekDay);

    QPainter painter(cell);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(m_backgroudColor));
    painter.drawRect(rect);//画矩形
    if (d == 6 || d == 7)  painter.drawRect(rect); //画矩形


    if (isSelectedCell) {
        QRect fillRect(1, 12, 26, 26);
        painter.setRenderHints(QPainter::HighQualityAntialiasing);
        painter.setBrush(QBrush(m_weekendsTextColor));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(fillRect);
    }

    QLocale locale;
    const QString dayNum = getCellDayNum(pos);
    const QString dayLunar = getLunar(pos);
    const QString dayWeek = locale.dayName(d ? d : 7, QLocale::ShortFormat);

    painter.save();
    painter.setPen(Qt::SolidLine);
    // draw text of day
    if (isSelectedCell) {
        painter.setPen(m_currentDayTextColor);
    } else if (isCurrentDay) {
        painter.setPen(m_weekendsTextColor);
    } else {
        if (d == 6 || d == 7)
            painter.setPen(m_weekendsTextColor);
        else
            painter.setPen(m_defaultTextColor);
    }

//    painter.drawRect(rect);
    painter.setFont(m_dayNumFont);
    if (m_showState & ShowLunar) {
        painter.drawText(QRect(5, 14, 27, 25), Qt::AlignLeft, dayNum);
        if (d == 6 || d == 7)
            painter.setPen(m_weekendsTextColor);
        else
            painter.setPen(m_defaultTextColor);
        painter.drawText(QRect(28, 14, 30, 25), Qt::AlignCenter, dayWeek);

    } else {
        painter.drawText(QRect(0, 0, cell->width() / 2, cell->height() / 2), Qt::AlignCenter, dayNum);
        if (d == 6 || d == 7)
            painter.setPen(m_weekendsTextColor);
        else
            painter.setPen(m_defaultTextColor);
        painter.drawText(QRect(cell->width() / 2, 0, cell->width() / 2, cell->height() / 2), Qt::AlignCenter, dayWeek);
    }

    // draw text of day type
    if (m_showState & ShowLunar) {
        if (d == 6 || d == 7)
            painter.setPen(m_weekendsTextColor);
        else
            painter.setPen(m_defaultLunarColor);

        painter.drawText(QRect(cell->width() / 2 + 10, 14, 50, 25), Qt::AlignLeft, dayLunar);
        CaLunarDayInfo dayInfo = getCaLunarDayInfo(pos);
        if (!dayInfo.mSolarFestival.isEmpty()) {
            QRect fillRect = QRect(2, 39, 106, 15);
            painter.setPen(Qt::red);
            QFont solofont = m_dayNumFont;
            solofont.setPixelSize(10);
            painter.setFont(solofont);
            QFontMetrics fm = painter.fontMetrics();
            while (fm.width(dayInfo.mSolarFestival) > 108) {
                solofont.setPixelSize(solofont.pixelSize() - 1);
                painter.setFont(solofont);
                fm = painter.fontMetrics();
            }
            painter.drawText(fillRect, Qt::AlignRight, dayInfo.mSolarFestival);
        }

    }
    painter.restore();
    painter.end();
}

void CWeekHeadView::cellClicked(QWidget *cell)
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

void CWeekHeadView::setSelectedCell(int index)
{
    if (m_selectedCell == index)
        return;

    const int prevPos = m_selectedCell;
    m_selectedCell = index;

    m_cellList.at(prevPos)->update();
    m_cellList.at(index)->update();
    emit dateSelected(m_days[index], getCaLunarDayInfo(index));
    emit signalcurrentLunarDateChanged(m_days[index], getCaLunarDayInfo(index), 0);
}
int CWeekHeadView::checkDay(int weekday)
{

    // check the week, calculate the correct order in the custom.

    if (weekday <= 0)
        return weekday += 7;

    if (weekday > 7)
        return weekday -= 7;

    return weekday;
}
