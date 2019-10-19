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
#include "monthschceduleview.h"
#include <DHiDPIHelper>
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
        m_defaultLunarColor.setAlphaF(0.5);
        m_currentDayLunarColor = m_currentDayTextColor;
        m_weekendsLunarColor = m_defaultLunarColor;
        m_selectedLunarColor = Qt::white;
        m_festivalLunarColor = m_defaultLunarColor;
        m_notCurrentLunarColor = "#dfdfdf";
        m_solofestivalLunarColor = "#FF7272";
        m_solofestivalLunarColor.setAlphaF(0.3);
        m_wrectColor = "#000000";
        m_wrectColor.setAlphaF(0.05);
        m_fillColor = Qt::white;
        m_banColor = "#FF7171";
        m_banColor.setAlphaF(0.1);
        m_xiuColor = "#ADFF71";
        m_xiuColor.setAlphaF(0.1);

        m_pressColor = "#000000";
        m_pressColor.setAlphaF(0.2);

        m_hoverColor = "#000000";
        m_hoverColor.setAlphaF(0.1);

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
        m_fillColor = "#000000";
        m_fillColor.setAlphaF(0.05);
        m_banColor = "#FF7171";
        m_banColor.setAlphaF(0.1);
        m_xiuColor = "#ADFF71";
        m_xiuColor.setAlphaF(0.1);

        m_pressColor = "#FFFFFF";
        m_pressColor.setAlphaF(0.2);

        m_hoverColor = "#FFFFFF";
        m_hoverColor.setAlphaF(0.1);
    }
    m_weekIndicator->setTheMe(type);
    m_MonthSchceduleView->setTheMe(type);
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
    m_MonthSchceduleView = new CMonthSchceduleView(this);

    // cells grid
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setMargin(0);
    gridLayout->setSpacing(0);
    for (int r = 0; r != 6; ++r) {
        for (int c = 0; c != 7; ++c) {
            QWidget *cell = new QWidget(this);
            cell->setFixedSize(cellwidth, cellheight);
            cell->installEventFilter(this);
            cell->setFocusPolicy(Qt::ClickFocus);

            gridLayout->addWidget(cell, r, c);
            m_cellList.append(cell);

        }
    }
    connect(m_MonthSchceduleView, &CMonthSchceduleView::signalsUpdateShcedule, this, &CMonthView::slotSchceduleUpdate);

    DFrame *gridWidget = new DFrame;
    gridWidget->setLayout(gridLayout);

    m_mainLayout = new QVBoxLayout;
    m_mainLayout->setMargin(0);
    m_mainLayout->setSpacing(0);
    m_mainLayout->setContentsMargins(10, 0, 10, 10);
    m_mainLayout->addWidget(m_weekIndicator);
    m_mainLayout->addWidget(gridWidget);
    for (int i = 0; i < 42; i++) {
        m_cellhoverflag[i] = false;
    }
    m_banColor = "#F85566";
    m_banColor.setAlphaF(0.2);
    m_xiuColor = "#6FFF00";
    m_xiuColor.setAlphaF(0.2);
    setLayout(m_mainLayout);
    CScheduleDataCtrl  *scheduleDataCtrl = CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl();
    connect(this, &CMonthView::dateSelected, this, &CMonthView::handleCurrentDateChanged);
    m_createAction = new QAction(tr("Create"), this);
    connect(m_createAction, &QAction::triggered, this, &CMonthView::slotCreate);
    connect(scheduleDataCtrl, &CScheduleDataCtrl::signalsupdatescheduleD, this, &CMonthView::slotsupdatescheduleD);
    connect(this, &CMonthView::signalsupdatescheduleD, scheduleDataCtrl, &CScheduleDataCtrl::slotupdatescheduleD);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
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
}

void CMonthView::slotSchceduleUpdate(int id)
{
    setEnabled(false);
    emit signalsupdatescheduleD(this, m_days[0], m_days[41]);
}

void CMonthView::slotsupdatescheduleD(QWidget *w, QVector<ScheduleDateRangeInfo> &data)
{
    if (w != this) return;
    setEnabled(true);
    m_shceludelistdata = data;
    m_MonthSchceduleView->setallsize(width(), height(), m_leftmaagin, m_weekIndicator->height() + m_topmagin, m_topmagin);
    m_MonthSchceduleView->setData(data, m_currentDate.month());
    for (int i(0); i != 42; ++i) {
        m_cellList.at(i)->update();
    }
}

void CMonthView::resizeEvent(QResizeEvent *event)
{
    cellwidth = width() * 0.1395 + 0.5;
    cellheight = height() * 0.1428 + 0.5;


    int leftmagin = width() * 0.0116 + 0.5;
    int rightmagin = leftmagin;
    int topmagin = height() * 0.0193 + 0.5;
    int buttonmagin = topmagin;
    m_leftmaagin = leftmagin;
    m_topmagin = topmagin;
    m_mainLayout->setContentsMargins(leftmagin, topmagin, rightmagin, buttonmagin);
    m_weekIndicator->setFixedSize(width() - leftmagin * 2, height() * 0.1042 + 0.5);
    for (int i(0); i != 42; ++i) {
        m_cellList.at(i)->setFixedSize(cellwidth, cellheight);
        m_cellList.at(i)->update();
    }
    m_MonthSchceduleView->setallsize(width(), height(), leftmagin, m_weekIndicator->height() + topmagin, topmagin);
    m_MonthSchceduleView->updateData();
    DWidget::resizeEvent(event);
}

void CMonthView::focusOutEvent(QFocusEvent *event)
{
    m_fouceFlag = false;
    DWidget::focusOutEvent(event);
}

void CMonthView::focusInEvent(QFocusEvent *event)
{
    m_fouceFlag = true;
    DWidget::focusInEvent(event);
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
    bool flag = false;
    if (date.month() != m_currentDate.month()) {
        m_festivallist.clear();
        m_DBusInter->GetFestivalMonth(date.year(), date.month(), m_festivallist);
        flag = true;
    }
    m_currentDate = date;

    // to refresh lunar calendar
    updateCurrentLunar(getCaLunarDayInfo(getDateIndex(m_currentDate)));
    if (flag)
        slotSchceduleUpdate();
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
            if (rightevent->button() == Qt::LeftButton)
                cellClicked(cell);
        } else if (e->type() == QEvent::ContextMenu) {
            DMenu Context(this);
            Context.addAction(m_createAction);
            const int pos = m_cellList.indexOf(cell);
            m_createDate = m_days[pos];
            Context.exec(QCursor::pos());
        } else if (e->type() == QEvent::MouseButtonRelease) {
            m_updateflag = true;
        } else if (e->type() == QEvent::Leave) {
            const int pos = m_cellList.indexOf(cell);
            m_cellhoverflag[pos] = false;
            m_cellList[pos]->update();
        } else if (e->type() == QEvent::Enter) {
            const int pos = m_cellList.indexOf(cell);
            m_cellhoverflag[pos] = true;
            m_cellList[pos]->update();
        }
    }

    return false;
}
void CMonthView::slotCreate()
{
    CSchceduleDlg dlg(1, this);
    QDateTime tDatatime;
    tDatatime.setDate(m_createDate);
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

char CMonthView::getFestivalInfoByDate(const QDate &date)
{
    for (int i = 0; i < m_festivallist.count(); i++) {
        if (date.year() == m_festivallist[i].year && date.month() == m_festivallist[i].month) {
            for (int j = 0; j < m_festivallist[i].listHoliday.count(); j++) {

                if (m_festivallist[i].listHoliday[j].date == date) {
                    return m_festivallist[i].listHoliday[j].status;
                }
            }
        }
    }
    return 0;
}

bool CMonthView::getShowSolarDayByDate(const QDate &date)
{
    bool tflag = true;
    for (int i = 0; i < m_shceludelistdata.count(); i++) {
        if (date == m_shceludelistdata.at(i).date) {
            tflag = m_shceludelistdata.at(i).vData.isEmpty();
            break;
        }
    }
    return tflag;
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
    const QRect rect(0, 0, cellwidth, cellheight);

    const int pos = m_cellList.indexOf(cell);
    const int type = getDateType(m_days[pos]);
    const bool isSelectedCell = pos == m_selectedCell;
    const bool isCurrentDay = getCellDate(pos) == QDate::currentDate();

    QPainter painter(cell);
    painter.save();
    if (m_showState & ShowLunar) {
        painter.setRenderHints(QPainter::HighQualityAntialiasing);
        int ftype = getFestivalInfoByDate(m_days[pos]);
        if (ftype == 2) {
            painter.setBrush(QBrush(m_banColor));
        } else if (ftype == 1) {
            painter.setBrush(QBrush(m_xiuColor));
        } else {
            painter.setBrush(QBrush(m_fillColor));
        }
        painter.setPen(Qt::NoPen);
        painter.drawRect(rect);//画矩形
        if (getShowSolarDayByDate(m_days[pos])) {
            QRect fillRect(8, cell->height() - 17, 15, 15);
            painter.setRenderHints(QPainter::HighQualityAntialiasing);
            if (ftype == 2) {
                QColor banc = m_banColor;
                banc.setAlphaF(1.0);
                painter.setBrush(QBrush(banc));
                //QPixmap  pixmap = DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-ban.svg").scaled(30, 30);
                //painter.drawPixmap(0, cell->height() - 30, pixmap);
            } else if (ftype == 1) {
                QColor banc = m_xiuColor;
                banc.setAlphaF(1.0);
                painter.setBrush(QBrush(banc));
                //QPixmap pixmap = DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-xiu.svg").scaled(30, 30);
                //painter.drawPixmap(0, cell->height() - 30, pixmap);
            }
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(fillRect);
            QFont tbxfont;
            tbxfont.setFamily("Avenir-Light");
            tbxfont.setPixelSize(11);
            painter.setFont(tbxfont);
            painter.setPen(m_currentDayTextColor);
            if (ftype == 2) {
                painter.drawText(fillRect, Qt::AlignCenter, "班");
            } else if (ftype == 1) {
                painter.drawText(fillRect, Qt::AlignCenter, "休");
            }
        }

    } else {
        painter.setRenderHints(QPainter::HighQualityAntialiasing);
        painter.setBrush(QBrush(m_fillColor));
        painter.setPen(Qt::NoPen);
        painter.drawRect(rect);//画矩形
    }
    painter.restore();


    painter.setPen(Qt::SolidLine);
    painter.setPen(m_wrectColor);
    //painter.setPen(Qt::SolidLine);
    int rectindex = pos % 7;
    if (rectindex == 0) {
        painter.drawLine(0, 0, cellwidth, 0); //画矩形
        painter.drawLine(0, cellheight, cellwidth, cellheight); //画矩形
    } else {
        painter.drawRect(rect);//画矩形
    }



//    painter.drawRoundedRect(cell->rect(), 4, 4);

    // draw selected cell background circle
    if (isSelectedCell && m_fouceFlag) {
        QRect fillRect = QRect(2, 2, cellwidth - 3, cellheight - 3);

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
    if (!isSelectedCell) {
        if (isCurrentDay) {
            if (m_showState & ShowLunar) {
                QRect fillRect(4, 2, 30, 30);
                painter.setRenderHints(QPainter::HighQualityAntialiasing);
                painter.setBrush(QBrush(m_backgroundCircleColor));
                painter.setPen(Qt::NoPen);
                painter.drawEllipse(fillRect);
            } else {
                QRect fillRect((cellwidth - 30) / 2, 0, 30, 30);
                painter.setRenderHints(QPainter::HighQualityAntialiasing);
                painter.setBrush(QBrush(m_backgroundCircleColor));
                painter.setPen(Qt::NoPen);
                painter.drawEllipse(fillRect);
            }
        }
    } else {
        if (isCurrentDay) {
            if (m_showState & ShowLunar) {
                QRect fillRect(4, 3, 28, 28);
                painter.setRenderHints(QPainter::HighQualityAntialiasing);
                painter.setBrush(QBrush(m_backgroundCircleColor));
                painter.setPen(Qt::NoPen);
                painter.drawEllipse(fillRect);
            } else {
                QRect fillRect((cellwidth - 28) / 2, 2, 28, 28);
                painter.setRenderHints(QPainter::HighQualityAntialiasing);
                painter.setBrush(QBrush(m_backgroundCircleColor));
                painter.setPen(Qt::NoPen);
                painter.drawEllipse(fillRect);
            }
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

    if (isCurrentDay) {
        if (isSelectedCell) {
            QFont tfont = m_dayNumFont;
            if (m_days[pos].day() == 1) {
                tfont.setPixelSize(11);
            } else {
                tfont.setPixelSize(19);
            }
            painter.setFont(tfont);
            if (m_showState & ShowLunar) {
                painter.drawText(QRect(4, 3, 28, 28), Qt::AlignCenter, dayNum, &test);
            } else {
                painter.drawText(QRect(0, 2, cell->width(), 31), Qt::AlignCenter, dayNum, &test);
            }
        } else {
            QFont tfont = m_dayNumFont;
            if (m_days[pos].day() == 1) {
                tfont.setPixelSize(12);
            } else {
                tfont.setPixelSize(20);
            }
            painter.setFont(tfont);
            if (m_showState & ShowLunar) {
                painter.drawText(QRect(4, 2, 30, 30), Qt::AlignCenter, dayNum, &test);
            } else {
                painter.drawText(QRect(0, 0, cell->width(), 33), Qt::AlignCenter, dayNum, &test);
            }
        }
    } else {
        if (m_showState & ShowLunar) {
            painter.drawText(QRect(8, 0, cell->width() / 2, 33), Qt::AlignLeft, dayNum);
        } else {
            painter.drawText(QRect(0, 0, cell->width(), 33), Qt::AlignCenter, dayNum, &test);
        }
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
        painter.drawText(QRect(cell->width() - 50, 6, 50, 18), Qt::AlignCenter, dayLunar);
        if (getShowSolarDayByDate(m_days[pos])) {
            CaLunarDayInfo dayInfo = getCaLunarDayInfo(pos);
            if (!dayInfo.mSolarFestival.isEmpty()) {
                QRect fillRect = QRect(6, 34, cell->width() - 12, 22);
                painter.setRenderHints(QPainter::HighQualityAntialiasing);
                painter.setBrush(QBrush(m_solofestivalLunarColor));
                painter.setPen(Qt::NoPen);
                painter.drawRoundedRect(fillRect, 8, 8);
                painter.setPen(m_defaultTextColor);
                QFont solofont = m_dayLunarFont;
                painter.setFont(solofont);
                //QFontMetrics fm = painter.fontMetrics();
                //while (fm.width(dayInfo.mSolarFestival) > cell->width() - 12) {
                // solofont.setPixelSize(solofont.pixelSize() - 1);
                // painter.setFont(solofont);
                //fm = painter.fontMetrics();
                //}
                QFontMetrics fm = painter.fontMetrics();
                QString str =  dayInfo.mSolarFestival;
                QString tstr;
                for (int i = 0; i < str.count(); i++) {
                    tstr.append(str.at(i));
                    int widthT = fm.width(tstr) + 5;
                    if (widthT >= cell->width() - 12) {
                        tstr.chop(2);
                        break;
                    }
                }
                if (tstr != str) {
                    tstr = tstr + "...";
                }
                painter.drawText(QRect(6 + (fillRect.width() - fm.width(tstr)) / 2,
                                       34 + (fillRect.height() - fm.height()) / 2,
                                       fm.width(tstr),
                                       fm.height()),
                                 Qt::AlignLeft, tstr);
            }
        }
    }
    if (isSelectedCell) {
        QRect fillRect = QRect(0, 0, cellwidth, cellheight);
        painter.setBrush(m_pressColor);
        painter.setPen(Qt::NoPen);
        painter.drawRect(fillRect);
    }
    if (m_cellhoverflag[pos]) {
        QRect fillRect = QRect(0, 0, cellwidth, cellheight);
        painter.setBrush(m_hoverColor);
        painter.setPen(Qt::NoPen);
        painter.drawRect(fillRect);
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
