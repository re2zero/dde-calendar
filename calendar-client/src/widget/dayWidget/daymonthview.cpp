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

#include "todaybutton.h"
#include "scheduledatamanage.h"
#include "daymonthview.h"
#include "constants.h"
#include "dayhuangliview.h"

#include <DPalette>
#include <DHorizontalLine>
#include <DHiDPIHelper>

#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QEvent>
#include <QDebug>
#include <QMessageBox>
#include <QTime>
#include <QQueue>
#include <QSpacerItem>
#include <QPainterPath>
#include <QMouseEvent>
#include <QtGlobal>

DGUI_USE_NAMESPACE
CDayMonthView::CDayMonthView(QWidget *parent)
    : CustomFrame(parent)
{
    m_weeklist.append(tr("Monday"));
    m_weeklist.append(tr("Tuesday"));
    m_weeklist.append(tr("Wednesday"));
    m_weeklist.append(tr("Thursday"));
    m_weeklist.append(tr("Friday"));
    m_weeklist.append(tr("Saturday"));
    m_weeklist.append(tr("Sunday"));
    initUI();
    initConnection();
}

CDayMonthView::~CDayMonthView()
{
}

void CDayMonthView::setShowDate(const QVector<QDate> &showDate, const QDate &selectDate, const QDate &currentDate)
{
    m_selectDate = selectDate;
    m_currentDate = currentDate;
    m_dayMonthWidget->setShowDate(showDate, selectDate, currentDate);
    updateDateShow();
}

void CDayMonthView::setLunarVisible(bool visible)
{
    m_huanglistate = visible;
    m_yiLabel->setVisible(visible);
    m_jiLabel->setVisible(visible);
    m_currentLuna->setVisible(visible);
    m_splitline->setVisible(visible);
    update();
}

void CDayMonthView::setTheMe(int type)
{
    m_dayMonthWidget->setTheMe(type);
    if (type == 0 || type == 1) {
        DPalette anipa = this->palette();
        QColor tbcolor = "#FFFFFF";
        anipa.setColor(DPalette::Background, tbcolor);
        setPalette(anipa);
        setBackgroundRole(DPalette::Background);
        setBColor(tbcolor);

        DPalette todaypa = m_today->palette();
        QColor todayColor = CScheduleDataManage::getScheduleDataManage()->getSystemActiveColor();
        todaypa.setColor(DPalette::ButtonText, todayColor);
        todaypa.setColor(DPalette::Dark, Qt::white);
        todaypa.setColor(DPalette::Light, Qt::white);
        QColor sbcolor("#002A57");
        sbcolor.setAlphaF(0.05);
        todaypa.setColor(DPalette::Shadow, sbcolor);
        m_today->setPalette(todaypa);
        QColor todayhover = "#000000";
        todayhover.setAlphaF(0.1);
        QColor todaypress = "#000000";
        todaypress.setAlphaF(0.2);
        m_today->setBColor("#FFFFFF", todayhover, todaypress, "#FFFFFF", todayhover, todaypress);
        m_today->setTColor(todayColor, "#001A2E", "#0081FF");
        m_today->setshadowColor(sbcolor);
        DPalette prevpa = m_prevButton->palette();
        prevpa.setColor(DPalette::Dark, QColor("#E6E6E6"));
        prevpa.setColor(DPalette::Light, QColor("#E3E3E3"));

        DPalette nextvpa = m_nextButton->palette();
        nextvpa.setColor(DPalette::Dark, QColor("#E6E6E6"));
        nextvpa.setColor(DPalette::Light, QColor("#E3E3E3"));

        m_currentMouth->setTextColor(QColor("#3B3B3B"));

        m_currentDay->setTextColor(todayColor);

        m_currentWeek->setTextColor(QColor("#414D68"));

        m_currentLuna->setTextColor(QColor("#414D68"));

        m_currentYear->setTextColor(QColor("#414D68"));

        QFont hlabelF;
        hlabelF.setPixelSize(DDECalendar::FontSizeFourteen);

        QColor yicolor = QColor("#75C18E");
        yicolor.setAlphaF(0.1);
        m_yiLabel->setbackgroundColor(yicolor);
        m_yiLabel->setTextInfo(QColor("#7B7B7B"), hlabelF);

        QColor jicolor = QColor("#C17575");
        jicolor.setAlphaF(0.1);
        m_jiLabel->setbackgroundColor(jicolor);
        m_jiLabel->setTextInfo(QColor("#7B7B7B"), hlabelF);

        m_topBorderColor = Qt::red;
        m_backgroundCircleColor = "#0081FF";
        m_weekendsTextColor = Qt::black;
        m_festivalTextColor = Qt::black;

    } else if (type == 2) {
        DPalette anipa = this->palette();
        QColor tbcolor = "#282828";
        anipa.setColor(DPalette::Background, tbcolor);
        setPalette(anipa);
        setBackgroundRole(DPalette::Background);
        setBColor(tbcolor);

        DPalette todaypa = m_today->palette();
        QColor todayColor = CScheduleDataManage::getScheduleDataManage()->getSystemActiveColor();
        todaypa.setColor(DPalette::ButtonText, todayColor);
        todaypa.setColor(DPalette::Light, "#484848");
        todaypa.setColor(DPalette::Dark, "#414141");
        QColor sbcolor("#000000");
        sbcolor.setAlphaF(0.05);
        todaypa.setColor(DPalette::Shadow, sbcolor);
        m_today->setPalette(todaypa);
        m_today->setBColor("#484848", "#727272", "#242424", "#414141", "#535353", "#282828");
        m_today->setTColor(todayColor, "#FFFFFF", "#0081FF");
        m_today->setshadowColor(sbcolor);
        DPalette prevpa = m_prevButton->palette();
        prevpa.setColor(DPalette::Dark, QColor("#484848"));
        prevpa.setColor(DPalette::Light, QColor("#414141"));
        DPalette nextvpa = m_nextButton->palette();
        nextvpa.setColor(DPalette::Dark, QColor("#484848"));
        nextvpa.setColor(DPalette::Light, QColor("#414141"));

        m_currentMouth->setTextColor(QColor("#C0C6D4"));

        m_currentDay->setTextColor(todayColor);

        m_currentWeek->setTextColor(QColor("#C0C6D4"));

        m_currentLuna->setTextColor(QColor("#C0C6D4"));

        m_currentYear->setTextColor(QColor("#C0C6D4"));
        QFont hlabelF;
        hlabelF.setPixelSize(DDECalendar::FontSizeFourteen);

        QColor yicolor = QColor("#2F8C4D");
        yicolor.setAlphaF(0.2);
        m_yiLabel->setbackgroundColor(yicolor);
        m_yiLabel->setTextInfo(QColor("#C0C6D4"), hlabelF);

        QColor jicolor = QColor("#A43B3B");
        jicolor.setAlphaF(0.2);
        m_jiLabel->setbackgroundColor(jicolor);
        m_jiLabel->setTextInfo(QColor("#C0C6D4"), hlabelF);

        m_topBorderColor = Qt::red;
        m_backgroundCircleColor = "#0059D2";
        m_weekendsTextColor = Qt::black;
        m_festivalTextColor = Qt::black;
    }
    update();
}

void CDayMonthView::setSearchFlag(bool flag)
{
    m_searchflag = flag;
    update();
}

/**
 * @brief CDayMonthView::setHuangliInfo     设置黄历信息
 * @param huangliInfo
 */
void CDayMonthView::setHuangliInfo(const CaHuangLiDayInfo &huangliInfo)
{
    m_huangliInfo = huangliInfo;
    updateDateLunarDay();
}

void CDayMonthView::setHasScheduleFlag(const QVector<bool> &hasScheduleFlag)
{
    m_dayMonthWidget->setHasScheduleFlag(hasScheduleFlag);
}

void CDayMonthView::initUI()
{
    m_today = new CTodayButton;
    m_today->setText(QCoreApplication::translate("today", "Today", "Today"));
    m_today->setFixedSize(100, DDEDayCalendar::D_MLableHeight);
    DPalette todaypa = m_today->palette();
    QColor todayColor = CScheduleDataManage::getScheduleDataManage()->getSystemActiveColor();
    todaypa.setColor(DPalette::ButtonText, todayColor);
    todaypa.setColor(DPalette::Dark, Qt::white);
    todaypa.setColor(DPalette::Light, Qt::white);
    QColor sbcolor("#002A57");
    sbcolor.setAlphaF(0.05);
    todaypa.setColor(DPalette::Shadow, sbcolor);
    QFont todayfont;
    todayfont.setPixelSize(DDECalendar::FontSizeFourteen);
    m_today->setFont(todayfont);
    m_today->setPalette(todaypa);
    m_prevButton = new DIconButton(DStyle::SP_ArrowLeft, this);
    m_prevButton->setFixedSize(36, 36);

    m_nextButton = new DIconButton(DStyle::SP_ArrowRight, this);
    m_nextButton->setFixedSize(36, 36);

    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->setMargin(0);
    titleLayout->setSpacing(0);
    titleLayout->setContentsMargins(0, 0, 0, 3);
    //add separator line
    m_currentMouth = new CustomFrame(this);
    m_currentMouth->setFixedSize(74, DDEDayCalendar::D_MLableHeight);
    QFont mlabelF;
    mlabelF.setWeight(QFont::Medium);
    mlabelF.setPixelSize(DDECalendar::FontSizeTwentyfour);
    m_currentMouth->setTextFont(mlabelF);
    m_currentMouth->setTextAlign(Qt::AlignCenter);
    titleLayout->addWidget(m_prevButton);
    titleLayout->addWidget(m_currentMouth);
    titleLayout->addWidget(m_nextButton);
    titleLayout->addStretch();
    titleLayout->addWidget(m_today, 0, Qt::AlignRight);

    //上半部分
    m_upLayout = new QVBoxLayout;
    m_upLayout->setMargin(0);
    m_upLayout->setSpacing(0);
    m_upLayout->setContentsMargins(22, 9, 0, 7);
    m_upLayout->addLayout(titleLayout);
    m_dayMonthWidget = new CDayMonthWidget();
    m_upLayout->addWidget(m_dayMonthWidget);

    //中间部分
    QVBoxLayout *midLayout = new QVBoxLayout;
    midLayout->setMargin(0);
    midLayout->setSpacing(0);
    midLayout->setContentsMargins(0, 0, 0, 20);
    m_currentDay = new CustomFrame(this);
    m_currentDay->setFixedHeight(DDEDayCalendar::DDLableHeight);
    m_currentDay->setMinimumWidth(width());
    m_currentDay->setTextAlign(Qt::AlignCenter);
    QFont daylabelF;
    daylabelF.setWeight(QFont::Medium);
    daylabelF.setPixelSize(DDECalendar::FontSizeOneHundred);
    m_currentDay->setTextFont(daylabelF);
    midLayout->addWidget(m_currentDay);

    m_currentWeek = new CustomFrame(this);
    m_currentWeek->setFixedHeight(DDEDayCalendar::DWLableHeight);
    m_currentWeek->setTextAlign(Qt::AlignCenter);
    QFont wlabelF;
    wlabelF.setPixelSize(DDECalendar::FontSizeSixteen);
    m_currentWeek->setTextFont(wlabelF);
    midLayout->addWidget(m_currentWeek);
    midLayout->addSpacing(2);

    m_currentYear = new CustomFrame(this);
    m_currentYear->setFixedHeight(DDEDayCalendar::DWLableHeight);
    m_currentYear->setTextAlign(Qt::AlignCenter);
    m_currentYear->setTextFont(wlabelF);
    midLayout->addWidget(m_currentYear);
    midLayout->addSpacing(2);

    m_currentLuna = new CustomFrame(this);
    m_currentLuna->setFixedHeight(DDEDayCalendar::DHualiInfoLableHeight);
    m_currentLuna->setTextAlign(Qt::AlignCenter);
    QFont hlabelF;
    hlabelF.setPixelSize(DDECalendar::FontSizeTwelve);
    m_currentLuna->setTextFont(hlabelF);
    midLayout->addWidget(m_currentLuna);

    m_yidownLayout = new QVBoxLayout;
    m_yidownLayout->setMargin(0);
    m_yidownLayout->setSpacing(0);
    m_yidownLayout->setContentsMargins(10, 5, 10, 0);
    hlabelF.setPixelSize(DDECalendar::FontSizeFourteen);
    m_yiLabel = new CDayHuangLiLabel(this);
    m_yiLabel->setbackgroundColor(QColor("#75C18E"));
    m_yiLabel->setTextInfo(QColor("#7B7B7B "), hlabelF);
    m_yiLabel->setFixedSize(DDEDayCalendar::DHualiDtailLableWidth, DDEDayCalendar::DHualiDtailLableHeight);
    m_yiLabel->setHuangLiText(QStringList());
    m_yidownLayout->addWidget(m_yiLabel);

    m_jidownLayout = new QVBoxLayout;
    m_jidownLayout->setMargin(0);
    m_jidownLayout->setSpacing(0);
    m_jidownLayout->setContentsMargins(10, 10, 10, 10);

    m_jiLabel = new CDayHuangLiLabel(this);
    m_jiLabel->setbackgroundColor(QColor("#C17575"));
    m_jiLabel->setTextInfo(QColor("#7B7B7B "), hlabelF);
    m_jiLabel->setFixedSize(DDEDayCalendar::DHualiDtailLableWidth, DDEDayCalendar::DHualiDtailLableHeight);
    m_jiLabel->setHuangLiText(QStringList(), 1);
    m_jidownLayout->addWidget(m_jiLabel);

    m_hhLayout = new QVBoxLayout;
    m_hhLayout->setMargin(0);
    m_hhLayout->setSpacing(0);
    m_hhLayout->addLayout(m_upLayout, 6);
    m_hhLayout->addLayout(midLayout);

    m_splitline = new DHorizontalLine;

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
    connect(m_today, &CTodayButton::clicked, this, &CDayMonthView::slottoday);
    connect(m_nextButton, &DIconButton::clicked, this, &CDayMonthView::slotnext);
    connect(m_dayMonthWidget, &CDayMonthWidget::signalChangeSelectDate, this, &CDayMonthView::signalChangeSelectDate);
}

/**
 * @brief CDayMonthView::updateDateShow     更新月/天界面显示
 */
void CDayMonthView::updateDateShow()
{
    QLocale locale;
    m_currentMouth->setTextStr(locale.monthName(m_selectDate.month(), QLocale::ShortFormat));
    m_currentDay->setTextStr(QString::number(m_selectDate.day()));

    if (m_selectDate.dayOfWeek() > 0)
        m_currentWeek->setTextStr(m_weeklist.at(m_selectDate.dayOfWeek() - 1));
    m_currentYear->setTextStr(m_selectDate.toString("yyyy/M"));

    if (m_selectDate == m_currentDate) {
        m_today->setText(QCoreApplication::translate("today", "Today", "Today"));
    } else {
        m_today->setText(QCoreApplication::translate("Return Today", "Today", "Return Today"));
    }
}

/**
 * @brief CDayMonthView::updateDateLunarDay     更新显示黄历信息
 */
void CDayMonthView::updateDateLunarDay()
{
    if (!m_huanglistate) return;
    m_currentLuna->setTextStr(m_huangliInfo.mGanZhiYear + "年 " + "【" + m_huangliInfo.mZodiac + "年】" + m_huangliInfo.mGanZhiMonth + "月 " + m_huangliInfo.mGanZhiDay + "日 ");
    QStringList yilist = m_huangliInfo.mSuit.split(".", QString::SkipEmptyParts);
    QStringList jilist = m_huangliInfo.mAvoid.split(".", QString::SkipEmptyParts);
    m_yiLabel->setHuangLiText(yilist);
    m_jiLabel->setHuangLiText(jilist, 1);
}

void CDayMonthView::changeSelectDate(const QDate &date)
{
    emit signalChangeSelectDate(date);
}

void CDayMonthView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    int leftmagin = qRound(width() * 0.0332 + 0.5);
    int rightmagin = leftmagin;
    int topmagin = qRound(height() * 0.0164 + 0.5);
    int buttonmagin = topmagin;
    m_upLayout->setContentsMargins(leftmagin, topmagin, rightmagin, buttonmagin);
    m_splitline->setFixedWidth(qRound(0.6925 * width() + 0.5));

    int hleftmagin = qRound(width() * 0.026 + 0.5);
    int hrightmagin = hleftmagin;
    int htopmagin = qRound(height() * 0.01773 + 0.5);
    int hbuttonmagin = htopmagin;
    int lw = width() - hleftmagin * 2;
    int lh = qRound(height() * 0.0992);
    m_yiLabel->setFixedSize(lw, lh);
    m_yidownLayout->setContentsMargins(hleftmagin, qRound(htopmagin * 0.5), hrightmagin, 0);
    m_jiLabel->setFixedSize(lw, lh);
    m_jidownLayout->setContentsMargins(hleftmagin, htopmagin, hrightmagin, hbuttonmagin);
}

void CDayMonthView::wheelEvent(QWheelEvent *event)
{
    //如果是拖拽则退出
    bool isDragging = false;
    emit signalIsDragging(isDragging);
    if (isDragging)
        return;
    if (event->delta() < 0) {
        //切换前一天
        changeSelectDate(m_selectDate.addDays(1));
    } else {
        //切换后一天
        changeSelectDate(m_selectDate.addDays(-1));
    }
}

void CDayMonthView::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    int labelwidth = width();
    int labelheight = height();
    DPalette anipa = this->palette();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 反锯齿;
    painter.save();
    painter.setBrush(anipa.background());
    painter.setPen(Qt::NoPen);
    QPainterPath painterPath;
    painterPath.moveTo(0, 0);
    painterPath.lineTo(0, labelheight);
    //  如果有搜索界面则为右边为直角否则为圆角
    if (!m_searchflag) {
        painterPath.lineTo(labelwidth - m_radius, labelheight);
        painterPath.arcTo(QRect(labelwidth - m_radius * 2, labelheight - m_radius * 2, m_radius * 2, m_radius * 2), 270, 90);
        painterPath.lineTo(labelwidth, m_radius);
        painterPath.arcTo(QRect(labelwidth - m_radius * 2, 0, m_radius * 2, m_radius * 2), 0, 90);
    } else {
        painterPath.lineTo(labelwidth, labelheight);
        painterPath.lineTo(labelwidth, 0);
    }
    painterPath.lineTo(0, 0);
    painterPath.closeSubpath();
    painter.drawPath(painterPath);
    painter.restore();
}

void CDayMonthView::slotprev()
{
    changeSelectDate(m_selectDate.addMonths(-1));
}

void CDayMonthView::slotnext()
{
    changeSelectDate(m_selectDate.addMonths(1));
}

void CDayMonthView::slottoday()
{
    changeSelectDate(m_currentDate);
}

CDayMonthWidget::CDayMonthWidget(QWidget *parent)
    : QWidget(parent)
    , m_isFocus(false)
{
    m_gridLayout = new QGridLayout;
    m_gridLayout->setMargin(0);
    m_gridLayout->setSpacing(0);
    m_dayNumFont.setPixelSize(DDECalendar::FontSizeTwelve);
    for (int r = 0; r != 6; ++r) {
        for (int c = 0; c != 7; ++c) {
            QWidget *cell = new QWidget;
            cell->installEventFilter(this);
            m_gridLayout->addWidget(cell, r, c, 1, 1);
            m_cellList.append(cell);
        }
    }
    this->setLayout(m_gridLayout);
    setFocusPolicy(Qt::StrongFocus);
    //设置最大高度
    setMaximumHeight(250);
}

CDayMonthWidget::~CDayMonthWidget()
{
}

void CDayMonthWidget::setTheMe(int type)
{
    m_currentDayTextColor = CScheduleDataManage::getScheduleDataManage()->getSystemActiveColor();
    if (type == 0 || type == 1) {
        m_defaultTextColor = Qt::black;
        m_selectedTextColor = Qt::white;
        m_notCurrentTextColor = "#b2b2b2";
        m_ceventColor = QColor(255, 93, 0);
    } else if (type == 2) {
        m_defaultTextColor = "#C0C6D4";
        m_selectedTextColor = "#B8D3FF";
        m_notCurrentTextColor = "#C0C6D4";
        m_notCurrentTextColor.setAlphaF(0.5);
        m_ceventColor = QColor(204, 77, 3);
    }
    update();
}

void CDayMonthWidget::setShowDate(const QVector<QDate> &showDate, const QDate &selectDate, const QDate &currentDate)
{
    m_showDays = showDate;
    m_selectDate = selectDate;
    m_currentDate = currentDate;
    //当前选择index
    m_selectedCell = m_showDays.indexOf(m_selectDate);
    update();
}

void CDayMonthWidget::setHasScheduleFlag(const QVector<bool> &hasScheduleFlag)
{
    m_vlineflag = hasScheduleFlag;
    update();
}

const QString CDayMonthWidget::getCellDayNum(int pos)
{
    return QString::number(m_showDays[pos].day());
}

const QDate CDayMonthWidget::getCellDate(int pos)
{
    return m_showDays[pos];
}

void CDayMonthWidget::paintCell(QWidget *cell)
{
    const QRect rect = cell->rect();

    const int pos = m_cellList.indexOf(cell);
    const bool isSelectedCell = pos == m_selectedCell;
    const bool isCurrentDay = getCellDate(pos) == m_currentDate;

    QPainter painter(cell);
    painter.setRenderHint(QPainter::Antialiasing);

    // draw selected cell background circle
    if (isSelectedCell) {
        const qreal r = rect.width() > rect.height() ? rect.height() * 0.9 : rect.width() * 0.9;
        const qreal x = rect.x() + (rect.width() - r) / 2;
        const qreal y = rect.y() + (rect.height() - r) / 2;
        QRectF fillRect = QRectF(x, y, r, r).marginsRemoved(QMarginsF(1.5, 2.5, 1.5, 1.5));
        painter.save();
        painter.setBrush(QBrush(CScheduleDataManage::getScheduleDataManage()->getSystemActiveColor()));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(fillRect);
        painter.restore();
        if (m_isFocus) {
            //绘制焦点获取效果
            QPen pen;
            pen.setWidth(2);
            pen.setColor(CScheduleDataManage::getScheduleDataManage()->getSystemActiveColor());
            painter.setPen(pen);
            //在原有的选中效果外面再绘制一圈
            QRectF foucsRect(fillRect.x() - 2, fillRect.y() - 2, fillRect.width() + 4, fillRect.height() + 4);
            painter.setBrush(Qt::NoBrush);
            painter.drawEllipse(foucsRect);
        }
    }

    painter.setPen(Qt::SolidLine);

    const QString dayNum = getCellDayNum(pos);

    // draw text of day
    if (isSelectedCell) {
        painter.setPen(m_selectedTextColor);
    } else if (isCurrentDay) {
        painter.setPen(m_currentDayTextColor);
    } else {
        if (m_selectDate.month() == getCellDate(pos).month())
            painter.setPen(m_defaultTextColor);
        else
            painter.setPen(m_notCurrentTextColor);
    }

    QRect test;
    painter.setFont(m_dayNumFont);

    painter.drawText(rect, Qt::AlignCenter, dayNum, &test);

    if (m_vlineflag.count() == DDEDayCalendar::PainterCellNum) {
        if (m_vlineflag[pos]) {
            painter.save();
            QPen pen;
            pen.setWidth(2);
            pen.setColor(m_ceventColor);
            painter.setPen(pen);
            painter.setBrush(QBrush(m_ceventColor));
            painter.setPen(Qt::NoPen);
            int r = cell->width() * (4 / 25);
            if (r < 4) {
                r = 4;
            } else if (r > 7) {
                r = 7;
            }
            painter.drawEllipse(cell->width() - r - 6, 4, r, r);
            painter.restore();
        }
    }
}

bool CDayMonthWidget::eventFilter(QObject *o, QEvent *e)
{
    if (m_showDays.size() != 42)
        return false;
    QWidget *cell = qobject_cast<QWidget *>(o);
    if (cell && m_cellList.contains(cell)) {
        const int pos = m_cellList.indexOf(cell);
        QDate date = m_showDays[pos];
        //如果需要显示时间小于1900年则退出
        if (date.year() < DDECalendar::QueryEarliestYear)
            return false;
        if (e->type() == QEvent::Paint) {
            paintCell(cell);
        } else if (e->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent *>(e);
            if (mouseEvent->button() == Qt::LeftButton) {
                cellClicked(cell);
            }
        }
    }
    return false;
}

void CDayMonthWidget::resizeEvent(QResizeEvent *event)
{
    //获取每个时间widget的高度和宽度
    qreal width = this->width() / 7;
    qreal height = this->height() / 6;
    const qreal r = width > height ? height * 0.9 : width * 0.9;
    //根据高度和宽度设置时间字体的大小
    m_dayNumFont.setPixelSize(qRound(12 + (r - 18) * 6 / 17.0));
    QWidget::resizeEvent(event);
}

void CDayMonthWidget::focusInEvent(QFocusEvent *event)
{
    switch (event->reason()) {
    case Qt::TabFocusReason:
    case Qt::BacktabFocusReason:
    case Qt::ActiveWindowFocusReason:
        m_isFocus = true;
        break;
    default:
        m_isFocus = false;
        break;
    };
    update();
}

void CDayMonthWidget::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event)
    m_isFocus = false;
    update();
}

void CDayMonthWidget::keyPressEvent(QKeyEvent *event)
{
    if (m_isFocus) {
        switch (event->key()) {
        case Qt::Key_Left:
            emit signalChangeSelectDate(m_selectDate.addDays(-1));
            break;
        case Qt::Key_Right:
            emit signalChangeSelectDate(m_selectDate.addDays(1));
            break;
        case Qt::Key_Up:
            emit signalChangeSelectDate(m_selectDate.addDays(-7));
            break;
        case Qt::Key_Down:
            emit signalChangeSelectDate(m_selectDate.addDays(7));
            break;
        default:

            break;
        }
    }
    QWidget::keyPressEvent(event);
}

void CDayMonthWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    m_isFocus = false;
}

void CDayMonthWidget::cellClicked(QWidget *cell)
{
    const int pos = m_cellList.indexOf(cell);
    if (pos == -1)
        return;
    setSelectedCell(pos);
}

void CDayMonthWidget::setSelectedCell(int index)
{
    if (m_selectedCell == index)
        return;
    emit signalChangeSelectDate(m_showDays.at(index));
}
