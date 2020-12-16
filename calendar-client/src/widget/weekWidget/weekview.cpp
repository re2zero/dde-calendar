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

#include "weekview.h"
#include "scheduledatamanage.h"
#include "constants.h"

#include <DWidget>
#include <DHiDPIHelper>

#include <QHBoxLayout>
#include <QPainter>
#include <QBrush>
#include <QEvent>
#include <QMessageBox>
#include <QWheelEvent>
#include <QtGlobal>
#include <QMouseEvent>

DWIDGET_USE_NAMESPACE

CWeekView::CWeekView(const GetWeekNumOfYear &getWeekNumOfYear, QWidget *parent)
    : QWidget(parent)
    , m_getWeekNumOfYear(getWeekNumOfYear)
    , m_touchGesture(this)
{
    m_dayNumFont.setPixelSize(DDECalendar::FontSizeSixteen);
    m_dayNumFont.setWeight(QFont::Light);
    // cells grid
    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->setMargin(0);
    hboxLayout->setSpacing(0);

    for (int c = 0; c != DDEWeekCalendar::NumWeeksDisplayed; ++c) {
        QWidget *cell = new QWidget;
        cell->setFixedSize(DDEWeekCalendar::WWeekCellWidth, DDEWeekCalendar::WWeekCellHeight);
    }
    //上一周按钮
    m_prevButton = new DIconButton(DStyle::SP_ArrowLeft, this);
    m_prevButton->setFixedSize(36, 36);
    connect(m_prevButton, &DIconButton::clicked, this, &CWeekView::signalBtnPrev);
    //下一周按钮
    m_nextButton = new DIconButton(DStyle::SP_ArrowRight, this);
    m_nextButton->setFixedSize(36, 36);
    connect(m_nextButton, &DIconButton::clicked, this, &CWeekView::signalBtnNext);

    hboxLayout->addWidget(m_prevButton);
    //显示周数的widget
    for (int c = 0; c != DDEWeekCalendar::NumWeeksDisplayed; ++c) {
        QWidget *cell = new QWidget;
        //设置事件过滤器
        cell->installEventFilter(this);
        cell->setFocusPolicy(Qt::ClickFocus);
        hboxLayout->addWidget(cell);
        m_cellList.append(cell);
    }
    hboxLayout->addWidget(m_nextButton);
    //设置布局
    setLayout(hboxLayout);
    setMinimumWidth(150);
}

CWeekView::~CWeekView()
{
    for (int i = 0; i < DDEWeekCalendar::NumWeeksDisplayed; i++) {
        m_cellList.at(i)->removeEventFilter(this);
        delete m_cellList.at(i);
    }
    m_cellList.clear();
}

/**
 * @brief setCurrentDate        设置选择时间，并更新
 * @param date                  时间
 */
void CWeekView::setSelectDate(const QDate date)
{
    m_selectDate = date;
    updateDate();
}

/**
 * @brief CWeekView::setCurrent     设置当前时间
 * @param date
 */
void CWeekView::setCurrent(const QDateTime &dateTime)
{
    m_currentDate = dateTime;
}

/**
 * @brief setTheMe  根据系统主题类型设置颜色
 * @param type      系统主题类型
 */
void CWeekView::setTheMe(int type)
{
    m_themetype = type;

    if (type == 0 || type == 1) {
        m_defaultTextColor = Qt::black;
        m_backgrounddefaultColor = Qt::white;
        m_currentDayTextColor = Qt::white;
        m_backgroundcurrentDayColor = CScheduleDataManage::getScheduleDataManage()->getSystemActiveColor();
        m_fillColor = "#FFFFFF";
    } else if (type == 2) {
        m_defaultTextColor = "#C0C6D4";
        m_backgrounddefaultColor = "#FFFFFF";
        m_backgrounddefaultColor.setAlphaF(0.05);
        m_currentDayTextColor = "#B8D3FF";
        m_backgroundcurrentDayColor = CScheduleDataManage::getScheduleDataManage()->getSystemActiveColor();
        m_fillColor = "#000000";
        m_fillColor.setAlphaF(0.05);
    }
}

void CWeekView::switchDate(const QDate &date)
{
    if (date.year() >= DDECalendar::QueryEarliestYear) {
        setSelectDate(date);
    }
}

void CWeekView::slotprev()
{
    switchDate(m_selectDate.addDays(-DDEWeekCalendar::AFewDaysofWeek));
}

void CWeekView::slotnext()
{
    switchDate(m_selectDate.addDays(DDEWeekCalendar::AFewDaysofWeek));
}
void CWeekView::paintCell(QWidget *cell)
{
    const QRect rect(0, 0, cell->width(), cell->height());
    const int pos = m_cellList.indexOf(cell);
    //计算当前日期周数
    const int _showWeekNum = m_getWeekNumOfYear(m_days[pos]);
    const int _currentWeekNum = m_getWeekNumOfYear(m_currentDate.date());

    const bool isCurrentDay = _showWeekNum == _currentWeekNum && m_days[pos].year() == m_currentDate.date().year();
    const bool isSelectDay = m_days[pos].weekNumber() == m_selectDate.weekNumber();

    QPainter painter(cell);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.save();
    painter.setBrush(QBrush(m_fillColor));
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect); //画矩形
    painter.restore();
    painter.setPen(Qt::SolidLine);

    const QString dayNum = QString::number(_showWeekNum);

    if (isSelectDay) {
        QRect fillRect((cell->width() - 24) / 2, (cell->height() - 32) / 2 + 4, 24, 24);
        painter.save();
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(QBrush(CScheduleDataManage::getScheduleDataManage()->getSystemActiveColor()));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(fillRect);
        painter.restore();
        painter.setPen(m_currentDayTextColor);
        painter.setFont(m_dayNumFont);
        painter.drawText(QRect(0, 0, cell->width(), cell->height()), Qt::AlignCenter, dayNum);
    } else {
        if (isCurrentDay) {
            painter.setPen(m_backgroundcurrentDayColor);
        } else {
            painter.setPen(m_defaultTextColor);
        }
        painter.setFont(m_dayNumFont);
        painter.drawText(QRect(0, 0, cell->width(), cell->height()), Qt::AlignCenter, dayNum);
    }
    painter.end();
}

/**
 * @brief eventFilter 过滤器
 * @param o 事件对象
 * @param e 事件类型
 * @return  false
 */
bool CWeekView::eventFilter(QObject *o, QEvent *e)
{
    QWidget *cell = qobject_cast<QWidget *>(o);
    if (cell && m_cellList.contains(cell)) {
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

void CWeekView::cellClicked(QWidget *cell)
{
    const int pos = m_cellList.indexOf(cell);

    if (pos == -1)
        return;

    setSelectedCell(pos);
    update();
}

/**
 * @brief setSelectedCell   设置被选择的周数
 * @param index             周数所在的索引
 */
void CWeekView::setSelectedCell(int index)
{
    if (m_selectedCell == index)
        return;

    const int prevPos = m_selectedCell;
    m_selectedCell = index;

    m_cellList.at(prevPos)->update();
    m_cellList.at(index)->update();
    m_selectDate = m_days[index];
    const QString dayNum = QString::number(m_getWeekNumOfYear(m_selectDate));

    if (m_days[index].year() < DDECalendar::QueryEarliestYear && dayNum != "1")
        return;
    emit signalsSelectDate(m_days[index]);
}

void CWeekView::updateDate()
{
    for (int i = 0 ; i < DDEWeekCalendar::NumWeeksDisplayed; ++i) {
        m_days[i]  = m_selectDate.addDays((i - 4) * DDEWeekCalendar::AFewDaysofWeek);
        if (m_days[i] == m_selectDate)
            m_selectedCell = i;
    }
    update();
}
/**
 * @brief wheelEvent 鼠标滚轮切换上一周下一周
 * @param event 鼠标滚轮事件
 */
void CWeekView::wheelEvent(QWheelEvent *event)
{
    bool isDragging = false;
    emit signalIsDragging(isDragging);
    //判断是否是拖拽状态
    if (!isDragging) {
        if (event->angleDelta().x() > 100 || event->angleDelta().y() > 100) {
            //上一周
            signalBtnPrev();
        }
        if (event->angleDelta().x() < -100 || event->angleDelta().y() < -100) {
            //下一周
            signalBtnNext();
        }
    }
}

bool CWeekView::event(QEvent *e)
{
    if (m_touchGesture.event(e)) {
        //获取触摸状态
        switch (m_touchGesture.getTouchState()) {
        case touchGestureOperation::T_SLIDE: {
            //在滑动状态如果可以更新数据则切换月份
            if (m_touchGesture.isUpdate()) {
                m_touchGesture.setUpdate(false);
                switch (m_touchGesture.getMovingDir()) {
                case touchGestureOperation::T_LEFT:
                    slotnext();
                    break;
                case touchGestureOperation::T_RIGHT:
                    slotprev();
                    break;
                default:
                    break;
                }
            }
            break;
        }
        default:
            break;
        }
        return true;
    } else {
        return DWidget::event(e);
    }
}

void CWeekView::resizeEvent(QResizeEvent *event)
{
    //获取当前所有cell的宽度
    const int _allCellWidth = width() - 36 * 2;
    //获取当前cell的宽度
    int w = _allCellWidth / DDEWeekCalendar::NumWeeksDisplayed;
    //最小显示的宽度
    const int _minWidget = 36;
    //默认都显示
    QVector<bool> vindex(10, true);
    //cell的宽度小于最小宽度则隐藏部分显示
    if (w < _minWidget) {
        //计算前后需要隐藏的个数
        int t_num = qRound((_minWidget * DDEWeekCalendar::NumWeeksDisplayed - _allCellWidth) / _minWidget / 2.0);
        for (int i = 0; i < t_num; i++) {
            vindex[i] = false;
            vindex[9 - i] = false;
        }
    }
    //设置是否显示
    for (int i = 0; i < DDEWeekCalendar::NumWeeksDisplayed; i++) {
        m_cellList[i]->setVisible(vindex[i]);
    }
    QWidget::resizeEvent(event);
    //更新显示
    update();
}
