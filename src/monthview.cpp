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

#include "monthview.h"
#include "calendardbus.h"

#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QEvent>
#include <QDebug>
#include <QMessageBox>
#include <QTime>
#include <QQueue>
#include "schceduledayview.h"
#include "dbmanager.h"
#include "schceduledlg.h"
#include <QMenu>
#include "scheduledatamanage.h"
void CMonthView::setTheMe(int type)
{
    if (type == 0 || type == 1) {
        m_topBorderColor = Qt::red;
        m_backgroundCircleColor = "#2ca7f8";

        m_defaultTextColor = Qt::black;
        m_currentDayTextColor = "#FFFFFF";
        m_weekendsTextColor = Qt::black;
        m_selectedTextColor = Qt::white;
        m_festivalTextColor = Qt::black;
        m_notCurrentTextColor = "#b2b2b2";

        m_defaultLunarColor = "#5E5E5E";
        m_currentDayLunarColor = m_currentDayTextColor;
        m_weekendsLunarColor = m_defaultLunarColor;
        m_selectedLunarColor = Qt::white;
        m_festivalLunarColor = m_defaultLunarColor;
        m_notCurrentLunarColor = "#dfdfdf";
        m_solofestivalLunarColor = "#FF7272";
        m_solofestivalLunarColor.setAlphaF(0.3);
        m_wrectColor = Qt::lightGray;

    } else if (type == 2) {

        m_topBorderColor = Qt::red;
        m_backgroundCircleColor = "#2ca7f8";

        m_defaultTextColor = "#C0C6D4";
        m_currentDayTextColor = "#C0C6D4";
        m_weekendsTextColor = m_defaultTextColor;
        m_selectedTextColor = "#B8D3FF";
        m_festivalTextColor = "#C0C6D4";
        m_notCurrentTextColor = "#b2b2b2";

        m_defaultLunarColor = "#ABDAFF";
        m_currentDayLunarColor = m_currentDayTextColor;
        m_weekendsLunarColor = m_defaultLunarColor;
        m_selectedLunarColor = Qt::white;
        m_festivalLunarColor = m_defaultLunarColor;
        m_notCurrentLunarColor = "#dfdfdf";
        m_solofestivalLunarColor = "#4DFF7272";
        QColor wcolor = Qt::black;
        wcolor.setAlphaF(0.5);
        m_wrectColor = wcolor;
    }
    m_weekIndicator->setTheMe(type);
    for (int i(0); i != 42; ++i) {
        m_cellList.at(i)->update();
        m_cellScheduleList[i]->setTheMe(type);
    }
}

CMonthView::CMonthView(QWidget *parent) : DWidget(parent)
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

    m_dayNumFont.setFamily("Avenir-Light");
    m_dayNumFont.setPixelSize(24);
    m_dayNumFont.setWeight(QFont::Light);

    m_dayLunarFont.setFamily("PingFangSC-Regular");
    m_dayLunarFont.setPixelSize(12);

    m_solofestivalLunarColor.setAlphaF(0.3);
    //setStyleSheet("QWidget { background: rgba(0, 0, 0, 0) }");

    m_weekIndicator = new CMonthWeekView;
    // cells grid
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setMargin(0);
    gridLayout->setSpacing(0);
    for (int r = 0; r != 6; ++r) {
        for (int c = 0; c != 7; ++c) {
            QWidget *cell = new QWidget(this);
            cell->setFixedSize(DDEMonthCalendar::MCellWidth, DDEMonthCalendar::MCellHeight);
            cell->installEventFilter(this);
            cell->setFocusPolicy(Qt::ClickFocus);
            CSchceduleDayView *shceduledayview = new CSchceduleDayView(cell, 1);
            shceduledayview->setFixedSize(108, 46);
            //shceduledayview->setALLDayData(scheduleInfolist);
            shceduledayview->move(5, 27);
            connect(shceduledayview, &CSchceduleDayView::signalsUpdateShcedule, this, &CMonthView::signalsSchceduleUpdate);
            connect(shceduledayview, &CSchceduleDayView::signalsCotrlUpdateShcedule, this, &CMonthView::slotCtrlSchceduleUpdate);
            gridLayout->addWidget(cell, r, c);
            m_cellList.append(cell);
            m_cellScheduleList.append(shceduledayview);
        }
    }

    DFrame *gridWidget = new DFrame;
    gridWidget->setLayout(gridLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_weekIndicator, 0, Qt::AlignHCenter);
    mainLayout->addWidget(gridWidget, 0,  Qt::AlignHCenter);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    setLayout(mainLayout);
    CScheduleDataCtrl  *scheduleDataCtrl = CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl();
    connect(this, &CMonthView::dateSelected, this, &CMonthView::handleCurrentDateChanged);
    m_createAction = new QAction(tr("Create"), this);
    connect(m_createAction, &QAction::triggered, this, &CMonthView::slotCreate);
    connect(scheduleDataCtrl, &CScheduleDataCtrl::signalsupdatescheduleD, this, &CMonthView::slotsupdatescheduleD);
    connect(this, &CMonthView::signalsupdatescheduleD, scheduleDataCtrl, &CScheduleDataCtrl::slotupdatescheduleD);
}

void CMonthView::handleCurrentDateChanged(const QDate date, const CaLunarDayInfo &detail)
{
    Q_UNUSED(detail);

    if (date != m_currentDate) {
        setCurrentDate(date);
    }
}

void CMonthView::slotCtrlSchceduleUpdate(QDate date, int type)
{
    setEnabled(false);
    emit signalsupdatescheduleD(this, m_days[0], m_days[41]);
    return;
    for (int i(0); i != 42; ++i) {
        if (m_days[i].month() != m_currentDate.month()) continue;
        if (type == 0 && m_days[i] == date) continue;
        //更新日程
        m_cellScheduleList[i]->setDate(m_days[i]);
    }
}

void CMonthView::slotSchceduleUpdate(int id)
{
    setEnabled(false);
    emit signalsupdatescheduleD(this, m_days[0], m_days[41]);
    return;
    for (int i(0); i != 42; ++i) {
        if (m_days[i].month() != m_currentDate.month()) continue;
        //更新日程
        m_cellScheduleList[i]->setDate(m_days[i]);
    }
}

void CMonthView::slotsupdatescheduleD(QWidget *w, QVector<ScheduleDateRangeInfo> &data)
{
    if (w != this) return;
    setEnabled(true);
    for (int i(0); i != 42; ++i) {
        QVector<ScheduleDtailInfo> vData;
        //更新日程
        m_cellScheduleList[i]->setDayData(m_days[i], vData, 1);
    }
    for (int i(0); i != 42; ++i) {
        if (m_days[i].month() != m_currentDate.month()) continue;

        for (int j = 0; j < data.size(); j++) {
            if (data.at(j).date == m_days[i]) {
                //更新日程
                m_cellScheduleList[i]->setDayData(m_days[i], data.at(j).vData, 1);
                break;
            }
        }
    }
}

void CMonthView::setFirstWeekday(int weekday)
{
    m_firstWeekDay = weekday;

    m_weekIndicator->setList(weekday);

    updateDate();
}

int CMonthView::getDateType(const QDate &date)
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

void CMonthView::setCurrentDate(const QDate date)
{
    qDebug() << "set current date " << date;

    if (date == m_currentDate) {
        return;
    }

    m_currentDate = date;

    // to refresh lunar calendar
    updateCurrentLunar(getCaLunarDayInfo(getDateIndex(m_currentDate)));
    emit currentDateChanged(date.year(), date.month());
    emit signalcurrentDateChanged(m_currentDate);
}

void CMonthView::setLunarVisible(bool visible)
{
    int state = int(m_showState);

    if (visible)
        state |= ShowLunar;
    else
        state &= ~ShowLunar;

    m_showState = ShowState(state);
    update();
}

void CMonthView::setLunarFestivalHighlight(bool highlight)
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

void CMonthView::setCellSelectable(bool selectable)
{
    if (selectable == m_cellSelectable)
        return;
    m_cellSelectable = selectable;

    emit cellSelectableChanged(m_cellSelectable);
}

int CMonthView::getDateIndex(const QDate &date) const
{
    for (int i = 0; i != 42; ++i)
        if (m_days[i] == date)
            return i;

    return 0;
}

bool CMonthView::eventFilter(QObject *o, QEvent *e)
{
    QWidget *cell = qobject_cast<QWidget *>(o);

    if (cell && m_cellList.contains(cell)) {
        if (e->type() == QEvent::Paint) {
            paintCell(cell);
        } else if (e->type() == QEvent::MouseButtonPress) {
            QMouseEvent *rightevent = dynamic_cast<QMouseEvent *>(e);
            if (rightevent->button() == Qt::RightButton)
                m_updateflag = false;
            cellClicked(cell);
        } else if (e->type() == QEvent::ContextMenu) {
            DMenu Context(this);
            Context.addAction(m_createAction);
            Context.exec(QCursor::pos());
        } else if (e->type() == QEvent::MouseButtonRelease) {
            m_updateflag = true;
        }
    }

    return false;
}
void CMonthView::slotCreate()
{
    CSchceduleDlg dlg(1, this);
    QDateTime tDatatime;
    tDatatime.setDate(m_currentDate);
    tDatatime.setTime(QTime::currentTime());
    dlg.setDate(tDatatime);
    if (dlg.exec() == DDialog::Accepted) {
        slotSchceduleUpdate();
    }
}
void CMonthView::updateDate()
{
    const QDate firstDay(m_currentDate.year(), m_currentDate.month(), 1);
    const int day = (firstDay.dayOfWeek() + m_firstWeekDay) % 7;
    const int currentIndex = day + m_currentDate.day() - 1;

    if (currentIndex < 0) {
        return;
    }

    for (int i(0); i != 42; ++i) {
        m_days[i] = firstDay.addDays(i - day);
        if (m_days[i].month() != m_currentDate.month()) continue;
        //更新日程
        //m_cellScheduleList[i]->setDate(m_days[i]);
    }
    if (m_updateflag) {
        setEnabled(false);
        emit signalsupdatescheduleD(this, m_days[0], m_days[41]);
    }
    setSelectedCell(currentIndex);
    update();
}

void CMonthView::updateCurrentLunar(const CaLunarDayInfo &info)
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

const QString CMonthView::getCellDayNum(int pos)
{
    if (m_days[pos].day() ==  1) {
        return QString::number(m_days[pos].month()) + "/" + QString::number(m_days[pos].day());
    }
    return QString::number(m_days[pos].day());
}

const QDate CMonthView::getCellDate(int pos)
{
    return m_days[pos];
}

const QString CMonthView::getLunar(int pos)
{
    CaLunarDayInfo info = getCaLunarDayInfo(pos);

    if (info.mLunarDayName == "初一") {
        info.mLunarDayName = info.mLunarMonthName + info.mLunarDayName;
    }

    if (info.mTerm.isEmpty())
        return info.mLunarDayName;

    return info.mTerm;
}

const CaLunarDayInfo CMonthView::getCaLunarDayInfo(int pos)
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

void CMonthView::getDbusData()
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
        QDBusReply<CaLunarDayInfo> replydd = m_DBusInter->GetLunarInfoBySolar(date.year(), date.month(), date.day(), o1);

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

void CMonthView::paintCell(QWidget *cell)
{
    const QRect rect((cell->width() - DDEMonthCalendar::MCellHighlightWidth) / 2,
                     (cell->height() - DDEMonthCalendar::MCellHighlightHeight) / 2,
                     DDEMonthCalendar::MCellHighlightWidth,
                     DDEMonthCalendar::MCellHighlightHeight);

    const int pos = m_cellList.indexOf(cell);
    const int type = getDateType(m_days[pos]);
    const bool isSelectedCell = pos == m_selectedCell;
    const bool isCurrentDay = getCellDate(pos) == QDate::currentDate();

    QPainter painter(cell);
    painter.setPen(Qt::SolidLine);
    painter.setPen(m_wrectColor);
    //painter.setPen(Qt::SolidLine);
    painter.drawRect(rect);//画矩形

//    painter.drawRoundedRect(cell->rect(), 4, 4);

    // draw selected cell background circle
    if (isSelectedCell) {
        QRect fillRect = QRect(2, 2, DDEMonthCalendar::MCellHighlightWidth - 6, DDEMonthCalendar::MCellHighlightHeight - 7);

        painter.setRenderHints(QPainter::HighQualityAntialiasing);
        //painter.setBrush(QBrush(m_backgroundCircleColor));
        QPen pen;
        pen.setColor(m_backgroundCircleColor);
        pen.setWidth(2);
        painter.setPen(pen);
        painter.drawRoundedRect(fillRect, 3, 3);
    }

    painter.setPen(Qt::SolidLine);

    const QString dayNum = getCellDayNum(pos);
    const QString dayLunar = getLunar(pos);
    if (isCurrentDay) {
        if (m_showState & ShowLunar) {
            QRect fillRect(6, 3, 30, 30);
            painter.setRenderHints(QPainter::HighQualityAntialiasing);
            painter.setBrush(QBrush(m_backgroundCircleColor));
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(fillRect);
        } else {
            QRect fillRect(45, 0, 30, 30);
            painter.setRenderHints(QPainter::HighQualityAntialiasing);
            painter.setBrush(QBrush(m_backgroundCircleColor));
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(fillRect);
        }
    }
    // draw text of day
    //if (isSelectedCell) {
    //   painter.setPen(m_selectedTextColor);
    if (isCurrentDay) {
        painter.setPen(m_currentDayTextColor);
    } else {
        const int tType = type & 0xff;
        if (tType & SO_MNotCurrentMonth)
            painter.setPen(m_notCurrentTextColor);
        else if (type == SO_MWeekends)
            painter.setPen(m_weekendsTextColor);
        else
            painter.setPen(m_defaultTextColor);
    }


//    painter.drawRect(rect);
    QRect test;
    painter.setFont(m_dayNumFont);
    if (m_showState & ShowLunar) {
        painter.drawText(QRect(8, 0, cell->width() / 2, cell->height() / 2), Qt::AlignLeft, dayNum);
    } else {
        painter.drawText(QRect(0, 0, 120, 33), Qt::AlignCenter, dayNum, &test);
    }

    // draw text of day type
    if (m_showState & ShowLunar) {
        //if (isCurrentDay) {
        // painter.setPen(m_currentDayLunarColor);
        if (m_showState & ShowLunarFestivalHighlight) {
            const int tType = type & 0xff;
            if (tType & SO_MNotCurrentMonth)
                painter.setPen(m_notCurrentLunarColor);
            else if (tType & SO_MFestival)
                painter.setPen(m_festivalLunarColor);
            else if (tType & SO_MWeekends)
                painter.setPen(m_weekendsLunarColor);
            else
                painter.setPen(m_defaultLunarColor);
        }
        painter.setFont(m_dayLunarFont);
        painter.drawText(QRect(cell->width() / 2 + 10, 8, cell->width() / 2 - 10, cell->height() / 2 - 6), Qt::AlignLeft, dayLunar);
        CaLunarDayInfo dayInfo = getCaLunarDayInfo(pos);
        if (!dayInfo.mSolarFestival.isEmpty()) {
            QRect fillRect = QRect(6, 34, 108, 22);
            painter.setRenderHints(QPainter::HighQualityAntialiasing);
            painter.setBrush(QBrush(m_solofestivalLunarColor));
            painter.setPen(Qt::NoPen);
            painter.drawRoundedRect(fillRect, 2, 2);
            painter.setPen(m_defaultTextColor);
            QFont solofont = m_dayLunarFont;
            painter.setFont(solofont);
            QFontMetrics fm = painter.fontMetrics();
            while (fm.width(dayInfo.mSolarFestival) > 108) {
                solofont.setPixelSize(solofont.pixelSize() - 1);
                painter.setFont(solofont);
                fm = painter.fontMetrics();
            }
            painter.drawText(QRect(6 + (fillRect.width() - fm.width(dayInfo.mSolarFestival)) / 2,
                                   34 + (fillRect.height() - fm.height()) / 2,
                                   fm.width(dayInfo.mSolarFestival),
                                   fm.height()),
                             Qt::AlignLeft, dayInfo.mSolarFestival);
        }

    }
    painter.end();
}

void CMonthView::cellClicked(QWidget *cell)
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

void CMonthView::setSelectedCell(int index)
{
    if (m_selectedCell == index)
        return;

    const int prevPos = m_selectedCell;
    m_selectedCell = index;

    m_cellList.at(prevPos)->update();
    m_cellList.at(index)->update();
    emit signalcurrentLunarDateChanged(m_days[index], getCaLunarDayInfo(getDateIndex(m_days[index])), 0);
    emit dateSelected(m_days[index], getCaLunarDayInfo(index));
}
