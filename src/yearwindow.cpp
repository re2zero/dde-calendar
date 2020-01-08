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
#include <DPalette>
#include <DHiDPIHelper>
#include <QMenuBar>
#include "schcedulesearchview.h"
DGUI_USE_NAMESPACE
CYearWindow::CYearWindow(QWidget *parent): QMainWindow (parent)
{
    m_DBusInter = new CalendarDBus("com.deepin.api.LunarCalendar",
                                   "/com/deepin/api/LunarCalendar",
                                   QDBusConnection::sessionBus(), this);
    queue = nullptr;
    lunarCache = nullptr;
    emptyCaLunarDayInfo = nullptr;

    if (!queue)
        queue = new QQueue<QDate>;
    if (!lunarCache)
        lunarCache = new QMap<QDate, CaLunarDayInfo>;
    if (!emptyCaLunarDayInfo)
        emptyCaLunarDayInfo = new CaLunarDayInfo;
    initUI();
    initConnection();
    setWindowFlags(Qt::FramelessWindowHint);//去掉标题
    setContentsMargins(0, 0, 0, 0);
}

CYearWindow::~CYearWindow()
{
    for (int i = 0; i < 12; i++) {
        disconnect(m_monthViewList.at(i), &CYearView::singanleActiveW, this, &CYearWindow::slotActiveW);
        disconnect(m_monthViewList.at(i), &CYearView::signalcurrentDateChanged, this, &CYearWindow::slotcurrentDateChanged);
        disconnect(m_monthViewList.at(i), &CYearView::signaldoubleclickDate, this, &CYearWindow::signaldoubleclickDate);
        disconnect(m_monthViewList.at(i), &CYearView::signalselectMonth, this, &CYearWindow::signalselectMonth);
        delete  m_monthViewList.at(i);
    }
    m_monthViewList.clear();
}

bool CYearWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_today) {
        if (event->type() == QEvent::MouseButtonPress) {
            slottoday();
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void CYearWindow::setDate(QDate date)
{
    if (!date.isValid()) return;
    m_currentdate = date;
    for (int i = 0; i < 12; i++) {
        QDate tdate(m_currentdate.year(), i + 1, 1);
        m_monthViewList.at(i)->setCurrentDate(tdate, 0);
    }
    for (int i = 0; i < 12; i++) {
        QDate tdate(m_currentdate.year(), i + 1, 1);
        if (date.year() == tdate.year() && date.month() == tdate.month()) {
            m_monthViewList.at(i)->setCurrentDate(date, 1);
            break;
        }
    }
    emit signalCurrentDate(date);
}

void CYearWindow::initUI()
{
    m_contentBackground = new DFrame();
    //m_contentBackground->setObjectName("CalendarBackground");
    // m_contentBackground->setStyleSheet("QFrame#CalendarBackground { "
    //"background:white;"
    //"}");
    //m_contentBackground->setFixedSize(CalendarWidth + ContentLeftRightPadding * 2,
    //   InfoViewHeight + CalendarHeight);
    DPalette anipa = m_contentBackground->palette();
    anipa.setColor(DPalette::Background, "#F8F8F8");
    m_contentBackground->setAutoFillBackground(true);
    m_contentBackground->setPalette(anipa);
    m_today = new QLabel(this);
    m_today->installEventFilter(this);
    // DPalette todaypa = m_today->palette();
    //todaypa.setColor(DPalette::ButtonText, QColor("#000000"));
    //todaypa.setColor(DPalette::Dark, Qt::white);
    //todaypa.setColor(DPalette::Light, Qt::white);

    //m_today->setPalette(todaypa);
    // m_today->setFocusPolicy(Qt::NoFocus);
    // m_today->setFlat(true);

    QFont todayfont("SourceHanSansSC-Normal");
    todayfont.setPixelSize(16);
    m_today->setFont(todayfont);
    m_today->setAlignment(Qt::AlignCenter);
    // DPalette pal = m_today->palette();
    //pal.setColor(DPalette::Button, QColor("#FFFFFF"));
    //pal.setColor(DPalette::ButtonText, QColor("#1D81EC"));
    //m_today->setPalette(pal);
    //m_today->setAutoFillBackground(true);
    m_today->setText(QCoreApplication::translate("today", "Today", "Today"));
    m_today->setFixedWidth(88);
    m_today->setAutoFillBackground(true);
    m_today->setFixedHeight(DDEYearCalendar::Y_MLableHeight - 4);
    m_prevButton = new DIconButton(DStyle::SP_ArrowLeft, this);
    m_prevButton->setFixedWidth(DDEYearCalendar::Y_MLableHeight);
    m_prevButton->setFixedHeight(DDEYearCalendar::Y_MLableHeight);
    //m_prevButton->setIconSize(QSize(36, 36));
    //m_prevButton->setIcon(QIcon(DHiDPIHelper::loadNxPixmap(":/resources/icon/previous_normal.svg")));
    //m_prevButton->setHoverPic(":/resources/icon/previous_hover.svg");
    //m_prevButton->setNormalPic(":/resources/icon/previous_normal.svg");
    //m_prevButton->setPressPic(":/resources/icon/previous_press.svg");
    //m_prevButton->setFrameStyle(QFrame::StyledPanel);




    //DPalette prevpa = m_prevButton->palette();
    //prevpa.setColor(DPalette::Shadow, QColor("#E6E6E6"));
    //prevpa.setColor(DPalette::Light, QColor("#E3E3E3"));
    //m_prevButton->setPalette(prevpa);

    //m_prevButton->setFixedSize(DDEYearCalendar::Y_MLableHeight, DDEYearCalendar::Y_MLableHeight);
    //m_prevButton->setArrowDirection(DArrowButton::ArrowLeft);
    m_nextButton = new DIconButton(DStyle::SP_ArrowRight, this);
    m_nextButton->setFixedWidth(DDEYearCalendar::Y_MLableHeight);
    m_nextButton->setFixedHeight(DDEYearCalendar::Y_MLableHeight);
    // m_nextButton->setIconSize(QSize(36, 36));
    //m_nextButton->setIcon(QIcon(DHiDPIHelper::loadNxPixmap(":/resources/icon/next_normal.svg")));

    //DPalette nextvpa = m_nextButton->palette();
    //nextvpa.setColor(DPalette::Dark, QColor("#E6E6E6"));
    //nextvpa.setColor(DPalette::Light, QColor("#E3E3E3"));
    //m_nextButton->setPalette(nextvpa);

    m_YearLabel = new QLabel();
    m_YearLabel->setFixedHeight(DDEYearCalendar::Y_YLableHeight);
    //m_currentMouth->setStyleSheet("border: 1px solid rgba(0, 0, 0, 0.05);");

    QFont t_labelF;
    t_labelF.setFamily("SourceHanSansSC");
    t_labelF.setWeight(QFont::Medium);
    t_labelF.setPixelSize(24);
    m_YearLabel->setFont(t_labelF);
    DPalette pa = m_YearLabel->palette();
    pa.setColor(DPalette::WindowText, QColor("#3B3B3B"));
    m_YearLabel->setPalette(pa);

    m_YearLunarLabel = new QLabel(m_contentBackground);
    m_YearLunarLabel->setFixedSize(DDEMonthCalendar::M_YLunatLabelWindth, DDEMonthCalendar::M_YLunatLabelHeight);

    QFont ylabelF;
    ylabelF.setFamily("SourceHanSansSC");
    ylabelF.setWeight(QFont::Medium);
    ylabelF.setPixelSize(14);
    m_YearLunarLabel->setFont(ylabelF);
    DPalette Lunapa = m_YearLunarLabel->palette();
    Lunapa.setColor(DPalette::WindowText, QColor("#8A8A8A"));
    m_YearLunarLabel->setPalette(Lunapa);
    //m_YearLunarLabel->move(116, 27);

    m_YearLunarDayLabel = new QLabel(m_contentBackground);
    m_YearLunarDayLabel->setFixedSize(96, DDEMonthCalendar::M_YLunatLabelHeight);

    m_YearLunarDayLabel->setFont(ylabelF);

    m_YearLunarDayLabel->setPalette(Lunapa);


    QHBoxLayout *yeartitleLayout = new QHBoxLayout;
    yeartitleLayout->setMargin(0);
    yeartitleLayout->setSpacing(0);
    //yeartitleLayout->setContentsMargins(2, 10, 2, 0);
    yeartitleLayout->setContentsMargins(11, 12, 8, 10);
    yeartitleLayout->addWidget(m_YearLabel);

    QHBoxLayout *yeartitleLayout1 = new QHBoxLayout;
    yeartitleLayout1->setMargin(0);
    yeartitleLayout1->setSpacing(0);
    //yeartitleLayout1->setContentsMargins(0, 10, 8, 5);
    yeartitleLayout1->setContentsMargins(4, 9, 0, 7);
    yeartitleLayout1->addWidget(m_YearLunarLabel);
    yeartitleLayout1->addSpacing(390);
    yeartitleLayout1->addStretch();
    yeartitleLayout1->addWidget(m_YearLunarDayLabel, 0, Qt::AlignVCenter);
    yeartitleLayout1->addSpacing(10);

    m_todayframe = new CustomFrame;
    m_todayframe->setContentsMargins(0, 0, 0, 0);
    m_todayframe->setRoundState(true, true, true, true);
    m_todayframe->setBColor(Qt::white);
    m_todayframe->setFixedHeight(DDEYearCalendar::Y_MLableHeight);
    m_todayframe->setboreder(1);
    QHBoxLayout *todaylayout = new QHBoxLayout;
    todaylayout->setMargin(0);
    todaylayout->setSpacing(0);
    todaylayout->addWidget(m_prevButton);
    todaylayout->addWidget(m_today, 0, Qt::AlignCenter);
    todaylayout->addWidget(m_nextButton);
    m_todayframe->setLayout(todaylayout);
    yeartitleLayout1->addWidget(m_todayframe);
    yeartitleLayout->addLayout(yeartitleLayout1);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setMargin(0);
    gridLayout->setSpacing(8);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            CYearView *view = new CYearView();
            //view->setCurrentDate(QDate(date.year(),i*j + j+1,1));
            connect(view, &CYearView::singanleActiveW, this, &CYearWindow::slotActiveW);

            connect(view, &CYearView::signalcurrentDateChanged, this, &CYearWindow::slotcurrentDateChanged);
            connect(view, &CYearView::signaldoubleclickDate, this, &CYearWindow::signaldoubleclickDate);
            connect(view, &CYearView::signalselectMonth, this, &CYearWindow::signalselectMonth);
            //view->setFixedSize(202, 159);
            gridLayout->addWidget(view, i, j);
            m_monthViewList.append(view);
        }
    }
    m_gridWidget = new DFrame;
    m_gridWidget->setLayout(gridLayout);

    m_gridWidget->setContentsMargins(0, 0, 0, 10);
    m_gridWidget->setFrameRounded(true);
    QVBoxLayout *hhLayout = new QVBoxLayout;
    hhLayout->setMargin(0);
    hhLayout->setSpacing(0);
    hhLayout->setContentsMargins(0, 0, 0, 0);
    hhLayout->addLayout(yeartitleLayout);
    hhLayout->addWidget(m_gridWidget);

    m_tmainLayout = new QHBoxLayout;
    m_tmainLayout->setMargin(0);
    m_tmainLayout->setSpacing(0);
    m_tmainLayout->setContentsMargins(10, 0, 10, 0);
    m_tmainLayout->addLayout(hhLayout);
    //mainLayout->addStretch(1);

    //m_schceduleSearchView = new CSchceduleSearchView(this);
    //m_schceduleSearchView->setFixedWidth(200);

    //QVBoxLayout *ssLayout = new QVBoxLayout;
    //ssLayout->setMargin(0);
    //ssLayout->setSpacing(0);
    // ssLayout->setContentsMargins(0, 10, 0, 10);
    //ssLayout->addWidget(m_schceduleSearchView);
    // m_tmainLayout->addLayout(ssLayout);
    //m_schceduleSearchView->setVisible(false);

    m_contentBackground->setContentsMargins(0, 0, 0, 0);
    m_contentBackground->setLayout(m_tmainLayout);

    //menuBar()->hide();
    setCentralWidget(m_contentBackground);
}

void CYearWindow::initConnection()
{
    connect(m_prevButton, &DIconButton::clicked, this, &CYearWindow::slotprev);
    //connect(m_today, &DPushButton::clicked, this, &CYearWindow::slottoday);
    connect(m_nextButton, &DIconButton::clicked, this, &CYearWindow::slotnext);
    //connect(m_schceduleSearchView, &CSchceduleSearchView::signalsUpdateShcedule, this, &CYearWindow::slotTransitSearchSchedule);
    // connect(m_schceduleSearchView, &CSchceduleSearchView::signalDate, this, &CYearWindow::slotsearchDateSelect);
}

void CYearWindow::setLunarVisible(bool state)
{
    m_YearLunarLabel->setVisible(state);
    m_YearLunarDayLabel->setVisible(state);
}

void CYearWindow::setTheMe(int type)
{
    if (type == 0 || type == 1) {
        DPalette anipa = m_contentBackground->palette();
        anipa.setColor(DPalette::Background, "#F8F8F8");
        m_contentBackground->setPalette(anipa);
        m_contentBackground->setBackgroundRole(DPalette::Background);

        DPalette todaypa = m_today->palette();
        todaypa.setColor(DPalette::WindowText, QColor("#000000"));
        todaypa.setColor(DPalette::Background, Qt::white);
        //todaypa.setColor(DPalette::Light, Qt::white);
        m_today->setPalette(todaypa);
        m_today->setForegroundRole(DPalette::WindowText);
        m_today->setBackgroundRole(DPalette::Background);

        m_todayframe->setBColor(Qt::white);

        //DPalette prevpa = m_prevButton->palette();
        //prevpa.setColor(DPalette::Dark, QColor("#E6E6E6"));
        //prevpa.setColor(DPalette::Light, QColor("#E3E3E3"));
        //m_prevButton->setPalette(prevpa);

        //DPalette nextvpa = m_nextButton->palette();
        //nextvpa.setColor(DPalette::Dark, QColor("#E6E6E6"));
        //nextvpa.setColor(DPalette::Light, QColor("#E3E3E3"));
        //m_nextButton->setPalette(nextvpa);

        DPalette pa = m_YearLabel->palette();
        pa.setColor(DPalette::WindowText, QColor("#3B3B3B"));
        m_YearLabel->setPalette(pa);
        m_YearLabel->setForegroundRole(DPalette::WindowText);

        DPalette Lunapa = m_YearLunarLabel->palette();
        Lunapa.setColor(DPalette::WindowText, QColor("#8A8A8A"));
        m_YearLunarLabel->setPalette(Lunapa);
        m_YearLunarLabel->setForegroundRole(DPalette::WindowText);

        m_YearLunarDayLabel->setPalette(Lunapa);
        m_YearLunarDayLabel->setForegroundRole(DPalette::WindowText);
        DPalette gpa = m_gridWidget->palette();
        gpa.setColor(DPalette::Background, "#F8F8F8");
        m_gridWidget->setPalette(gpa);
        m_gridWidget->setBackgroundRole(DPalette::Background);

    } else if (type == 2) {
        DPalette anipa = m_contentBackground->palette();
        anipa.setColor(DPalette::Background, "#252525");
        m_contentBackground->setPalette(anipa);
        m_contentBackground->setBackgroundRole(DPalette::Background);

        DPalette todaypa = m_today->palette();
        todaypa.setColor(DPalette::WindowText, QColor("#C0C6D4"));
        //todaypa.setColor(DPalette::Dark, "#414141");
        QColor tbcolor = "#414141";
        tbcolor.setAlphaF(0.0);
        todaypa.setColor(DPalette::Background, tbcolor);
        m_today->setPalette(todaypa);
        m_today->setForegroundRole(DPalette::WindowText);
        m_today->setBackgroundRole(DPalette::Background);
        QColor tbcolor2 = "#414141";
        tbcolor2.setAlphaF(0.3);
        m_todayframe->setBColor(tbcolor2);
        //DPalette prevpa = m_prevButton->palette();
        //prevpa.setColor(DPalette::Dark, QColor("#484848"));
        //prevpa.setColor(DPalette::Light, QColor("#414141"));
        //prevpa.setColor(DPalette::Dark, Qt::black);
        //prevpa.setColor(DPalette::Light, Qt::black);
        // m_prevButton->setPalette(prevpa);

        //DPalette nextvpa = m_nextButton->palette();
        //nextvpa.setColor(DPalette::Dark, QColor("#484848"));
        //nextvpa.setColor(DPalette::Light, QColor("#414141"));
        //m_nextButton->setPalette(nextvpa);

        DPalette pa = m_YearLabel->palette();
        pa.setColor(DPalette::WindowText, QColor("#C0C6D4"));
        m_YearLabel->setPalette(pa);
        m_YearLabel->setForegroundRole(DPalette::WindowText);
        DPalette Lunapa = m_YearLunarLabel->palette();
        Lunapa.setColor(DPalette::WindowText, QColor("#798BA8"));
        m_YearLunarLabel->setPalette(Lunapa);
        m_YearLunarLabel->setForegroundRole(DPalette::WindowText);
        m_YearLunarDayLabel->setPalette(Lunapa);
        m_YearLunarDayLabel->setForegroundRole(DPalette::WindowText);
        DPalette gpa = m_gridWidget->palette();
        gpa.setColor(DPalette::Background, "#252525");
        m_gridWidget->setPalette(gpa);
        m_gridWidget->setBackgroundRole(DPalette::Background);
    }
    for (int i = 0; i < 12; i++) {
        m_monthViewList.at(i)->setTheMe(type);
    }
}
void CYearWindow::setSearchWFlag(bool flag)
{
    m_searchfalg = flag;
    //m_schceduleSearchView->setVisible(flag);
}

void CYearWindow::clearSearch()
{
    //m_schceduleSearchView->clearSearch();
}

void CYearWindow::setSearchText(QString str)
{
    m_searchText = str;
    // m_schceduleSearchView->slotsetSearch(str);
}

void CYearWindow::updateHigh()
{
    for (int i = 0; i < 12; i++) {
        m_monthViewList.at(i)->updateHigh();
    }
}
void CYearWindow::slotTransitSearchSchedule(int id)
{
    // m_schceduleSearchView->slotsetSearch(m_searchText);
    emit signalsWUpdateShcedule(this, id);
}
void CYearWindow::slotReturnTodayUpdate()
{
    setDate(QDate::currentDate());
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
    if (m_currentdate.year() == 1900) return;
    QDate tcurrent = QDate(m_currentdate.year() - 1, m_currentdate.month(), m_currentdate.day());
    if (!tcurrent.isValid()) {
        m_currentdate = QDate(m_currentdate.year() - 1, m_currentdate.month(), 1);
    } else {
        m_currentdate = tcurrent;
    }
    if (m_currentdate.year() >= 1900) {
        //m_currentdate = QDate(m_currentdate.year() - 1, m_currentdate.month(), m_currentdate.day());
        setDate(m_currentdate);
    } else {
        //QMessageBox::information(this, tr("infomation"), tr("Year less than 1900!"));
    }
}

void CYearWindow::slotnext()
{
    QDate tcurrent = QDate(m_currentdate.year() + 1, m_currentdate.month(), m_currentdate.day());
    if (!tcurrent.isValid()) {
        m_currentdate = QDate(m_currentdate.year() + 1, m_currentdate.month(), 1);
    } else {
        m_currentdate = tcurrent;
    }
    setDate(m_currentdate);
}

void CYearWindow::slottoday()
{
    emit signalsReturnTodayUpdate(this);
    setDate(QDate::currentDate());
}

void CYearWindow::slotcurrentDateChanged(QDate date)
{
    m_currentdate = date;
    if (m_currentdate == QDate::currentDate()) {
        m_today->setText(QCoreApplication::translate("today", "Today", "Today"));
    } else {
        m_today->setText(QCoreApplication::translate("Return", "Today", "Return"));
    }
    CaLunarDayInfo info = getCaLunarDayInfo(m_currentdate);

    QLocale locale;
    if (locale.language() == QLocale::Chinese) {
        m_YearLabel->setText(QString::number(date.year()) + tr("Y"));
    } else {
        m_YearLabel->setText(QString::number(date.year()));
    }
    m_YearLunarLabel->setText("-" + info.mGanZhiYear + info.mZodiac + "年-");
    m_YearLunarDayLabel->setText("-农历" + info.mLunarMonthName + info.mLunarDayName + "-");
}

const QString CYearWindow::getLunar(QDate date)
{
    CaLunarDayInfo info = getCaLunarDayInfo(date);

    if (info.mLunarDayName == "初一") {
        info.mLunarDayName = info.mLunarMonthName;
    }

    if (info.mTerm.isEmpty())
        return info.mLunarDayName;

    return info.mTerm;
}

const CaLunarDayInfo CYearWindow::getCaLunarDayInfo(QDate date)
{
    const QDate tdate = date;

    if (lunarCache->contains(tdate)) {
        return lunarCache->value(tdate);
    }

    if (lunarCache->size() > 300)
        lunarCache->clear();

//    QTimer::singleShot(500, [this, pos] {getDbusData(pos);});
    queue->push_back(tdate);

    QTimer::singleShot(300, this, SLOT(getDbusData()));

    return *emptyCaLunarDayInfo;
}
void CYearWindow::getDbusData()
{
    if (queue->isEmpty())
        return;

    const QDate tdate = queue->head();
    queue->pop_front();
    const QDate date = tdate;
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
            if (date == m_currentdate) {
                currentDayInfo = dayInfo;
            }
            cacheDate = cacheDate.addDays(1);
        }
    } else {
        currentDayInfo = lunarCache->value(date);
    }
    // refresh   lunar info
    if (date == m_currentdate) {
        //更新
        m_YearLunarLabel->setText("-" + currentDayInfo.mGanZhiYear + currentDayInfo.mZodiac + "年-");
        m_YearLunarDayLabel->setText("-农历" + currentDayInfo.mLunarMonthName + currentDayInfo.mLunarDayName + "-");
    }
}

void CYearWindow::slotsearchDateSelect(QDate date)
{
    setDate(date);
}

void CYearWindow::resizeEvent(QResizeEvent *event)
{
    int tw = width() * 0.237 + 0.5;
    int th = height() * 0.272 + 0.5;
    if (m_searchfalg) {
        m_tmainLayout->setContentsMargins(10, 0, 0, 0);
    } else {
        m_tmainLayout->setContentsMargins(10, 0, 10, 0);
    }
    // m_schceduleSearchView->setFixedWidth(0.2325 * width() + 0.5);
    for (int i = 0; i < m_monthViewList.count(); i++) {
        //m_monthViewList.at(i)->setFixedSize(tw, th);
    }

    QMainWindow::resizeEvent(event);
}

void CYearWindow::wheelEvent(QWheelEvent *event)
{
    if (event->delta() < 0) {
        slotnext();
    } else {
        slotprev();
    }
}
