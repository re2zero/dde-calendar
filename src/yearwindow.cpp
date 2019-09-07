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
}

CYearWindow::~CYearWindow()
{
    for (int i = 0; i < 12; i++) {
        disconnect(m_monthViewList.at(i), &CYearView::singanleActiveW, this, &CYearWindow::slotActiveW);
        disconnect(m_monthViewList.at(i), &CYearView::signalcurrentDateChanged, this, &CYearWindow::slotcurrentDateChanged);
        delete  m_monthViewList.at(i);
    }
    m_monthViewList.clear();
}

void CYearWindow::setDate(QDate date)
{
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
}

void CYearWindow::initUI()
{
    m_contentBackground = new QFrame;
    //m_contentBackground->setObjectName("CalendarBackground");
    // m_contentBackground->setStyleSheet("QFrame#CalendarBackground { "
    //"background:white;"
    //"}");
    //m_contentBackground->setFixedSize(CalendarWidth + ContentLeftRightPadding * 2,
    //   InfoViewHeight + CalendarHeight);
    QPalette anipa;
    anipa.setColor(QPalette::Background, Qt::white);
    m_contentBackground->setAutoFillBackground(true);
    m_contentBackground->setPalette(anipa);
    m_today = new DPushButton(this);
    QFont todayfont("SourceHanSansSC-Medium");
    todayfont.setPixelSize(14);
    m_today->setFont(todayfont);
    // QPalette pal = m_today->palette();
    //pal.setColor(QPalette::Button, QColor("#FFFFFF"));
    //pal.setColor(QPalette::ButtonText, QColor("#1D81EC"));
    //m_today->setPalette(pal);
    //m_today->setAutoFillBackground(true);
    m_today->setText(tr("Return today"));
    m_today->setFixedWidth(100);
    m_today->setFixedHeight(DDEYearCalendar::Y_MLableHeight);
    m_prevButton = new DImageButton(this);
    m_prevButton->setFixedWidth(DDEYearCalendar::Y_MLableHeight);
    m_prevButton->setHoverPic(":/resources/icon/previous_hover.svg");
    m_prevButton->setNormalPic(":/resources/icon/previous_normal.svg");
    m_prevButton->setPressPic(":/resources/icon/previous_press.svg");

    //m_prevButton->setFixedSize(DDEYearCalendar::Y_MLableHeight, DDEYearCalendar::Y_MLableHeight);
    //m_prevButton->setArrowDirection(DArrowButton::ArrowLeft);
    m_nextButton = new DImageButton(this);
    m_nextButton->setFixedWidth(DDEYearCalendar::Y_MLableHeight);
    m_nextButton->setHoverPic(":/resources/icon/next_hover.svg");
    m_nextButton->setNormalPic(":/resources/icon/next_normal.svg");
    m_nextButton->setPressPic(":/resources/icon/next_press.svg");
    //m_nextButton->setFixedSize(DDEYearCalendar::Y_MLableHeight, DDEYearCalendar::Y_MLableHeight);
    //m_nextButton->setArrowDirection(DArrowButton::ArrowRight);
    m_YearLabel = new DLabel();
    m_YearLabel->setFixedHeight(DDEYearCalendar::Y_YLableHeight);
    //m_currentMouth->setStyleSheet("border: 1px solid rgba(0, 0, 0, 0.05);");

    QFont t_labelF;
    t_labelF.setFamily("SourceHanSansSC-Medium");
    t_labelF.setPixelSize(30);
    m_YearLabel->setFont(t_labelF);
    QPalette pa;
    pa.setColor(QPalette::WindowText, QColor("#3B3B3B"));
    m_YearLabel->setPalette(pa);

    m_YearLunarLabel = new DLabel(m_contentBackground);
    m_YearLunarLabel->setFixedSize(DDEMonthCalendar::M_YLunatLabelWindth, DDEMonthCalendar::M_YLunatLabelHeight);

    QFont ylabelF;
    ylabelF.setFamily("SourceHanSansSC-Medium");
    ylabelF.setPixelSize(14);
    m_YearLunarLabel->setFont(ylabelF);
    QPalette Lunapa;
    Lunapa.setColor(QPalette::WindowText, QColor("#8A8A8A"));
    m_YearLunarLabel->setPalette(Lunapa);
    m_YearLunarLabel->move(116, 27);

    m_YearLunarDayLabel = new DLabel(m_contentBackground);
    m_YearLunarDayLabel->setFixedSize(96, DDEMonthCalendar::M_YLunatLabelHeight);

    m_YearLunarDayLabel->setFont(ylabelF);

    m_YearLunarDayLabel->setPalette(Lunapa);


    QHBoxLayout *yeartitleLayout = new QHBoxLayout;
    yeartitleLayout->setMargin(0);
    yeartitleLayout->setSpacing(0);
    yeartitleLayout->setContentsMargins(12, 0, 10, 12);
    yeartitleLayout->addWidget(m_YearLabel);
    yeartitleLayout->addWidget(m_YearLunarLabel);
    QSpacerItem *t_spaceitem = new QSpacerItem(30, DDEYearCalendar::Y_MLableHeight, QSizePolicy::Expanding, QSizePolicy::Fixed);
    yeartitleLayout->addSpacerItem(t_spaceitem);
    yeartitleLayout->addWidget(m_YearLunarDayLabel);
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

            connect(view, &CYearView::signalcurrentDateChanged, this, &CYearWindow::slotcurrentDateChanged);
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
    connect(m_prevButton, &DImageButton::clicked, this, &CYearWindow::slotprev);
    connect(m_today, &DPushButton::clicked, this, &CYearWindow::slottoday);
    connect(m_nextButton, &DImageButton::clicked, this, &CYearWindow::slotnext);
}

void CYearWindow::setLunarVisible(bool state)
{
    m_YearLunarLabel->setVisible(state);
    m_YearLunarDayLabel->setVisible(state);
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
    emit signalsReturnTodayUpdate(this);
    setDate(QDate::currentDate());
}

void CYearWindow::slotcurrentDateChanged(QDate date)
{
    m_currentdate = date;
    CaLunarDayInfo info = getCaLunarDayInfo(m_currentdate);
    m_YearLabel->setText(QString::number(date.year()) + tr("Y"));
    m_YearLunarLabel->setText("-" + info.mGanZhiYear + info.mZodiac + "年-");
    m_YearLunarDayLabel->setText("-" + tr("Lunar") + info.mLunarMonthName + info.mLunarDayName + "-");
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
        m_YearLunarDayLabel->setText("-" + tr("Lunar") + currentDayInfo.mLunarMonthName + currentDayInfo.mLunarDayName + "-");
    }
}
