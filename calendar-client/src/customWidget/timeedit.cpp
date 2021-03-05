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
#include "timeedit.h"

#include <QRegExpValidator>
#include <QEvent>
#include <QFocusEvent>

#include <QVBoxLayout>
#include "calendarmanage.h"

//视图容器最大高度
const int viewContainerMaxHeight = 305;

CTimeEdit::CTimeEdit(QWidget *parent)
    : DComboBox(parent)
    , m_timeFormat(CalendarManager::getInstance()->getCalendarDateDataManage()->getTimeFormat())
    , m_timeFormatValue(CalendarManager::getInstance()->getCalendarDateDataManage()->getTimeFormatValue())
    , m_timeEdit(new TimeEdit)
{
    initUI();
    initConnection();
}

CTimeEdit::~CTimeEdit()
{
}

void CTimeEdit::setTime(QTime time)
{
    m_time = time;
    setSelectItem();
    m_timeEdit->getLineEdit()->setText(time.toString(m_timeFormat));
}

QTime CTimeEdit::getTime()
{
    //获取显示的text
    QString timetext = m_timeEdit->getLineEdit()->displayText();
    //将text转换为时间
    m_time = QTime::fromString(timetext, m_timeFormat);
    return m_time;
}

/**
 * @brief CTimeEdit::setSelectItem
 * 根据当前编辑框设置下拉选中item
 */
void CTimeEdit::setSelectItem()
{
    int hour = m_time.hour();
    int minute = m_time.minute();
    //取30分钟倍数的时间
    minute = minute / 30 * 30;
    minute += m_time.minute() % 30 == 0 ? 0 : 30;
    //如果分钟为60则小时+1;
    if (minute == 60) {
        minute = 0;
        //如果为24时则设置为23:30
        if (++hour == 24) {
            hour = 23;
            minute = 30;
        }
    }
    //获取该时间对应的index
    int index = m_strList.indexOf(QTime(hour, minute).toString(m_timeFormat));
    //设置下拉列表当前index
    setCurrentIndex(index);
}

void CTimeEdit::setTimeFormat(int value)
{
    //清除列表
    clear();
    m_strList.clear();
    //根据value值,设置时间显示格式
    if (value) {
        m_timeFormat = "hh:mm";
        for (int i = 0; i < 24; i++) {
            m_strList << QString("%1:%2").arg(i, 2, 10, QLatin1Char('0')).arg(0, 2, 10, QLatin1Char('0'));
            m_strList << QString("%1:%2").arg(i, 2, 10, QLatin1Char('0')).arg(30);
        }
        addItems(m_strList);
        m_timeEdit->setDisplayFormat(m_timeFormat);
    } else {
        m_timeFormat = "h:mm";
        for (int i = 0; i < 24; i++) {
            m_strList << QString("%1:%2").arg(i, 1, 10, QLatin1Char('0')).arg(0, 2, 10, QLatin1Char('0'));
            m_strList << QString("%1:%2").arg(i, 1, 10, QLatin1Char('0')).arg(30);
        }
        addItems(m_strList);
        m_timeEdit->setDisplayFormat(m_timeFormat);
    }
}

void CTimeEdit::initUI()
{
    int timeFormatValue = 2;
    //设置下拉列表时间显示格式
    if (m_timeFormatValue) {
        timeFormatValue = 2;
    } else {
        timeFormatValue = 1;
    }
    //关闭自动补全
    this->setAutoCompletion(false);
    m_timeEdit->setTime(QTime::currentTime());
    m_timeEdit->setDisplayFormat(m_timeFormat);
    setLineEdit(m_timeEdit->getLineEdit());
    for (int i = 0; i < 24; i++) {
        m_strList << QString("%1:%2").arg(i, timeFormatValue, 10, QLatin1Char('0')).arg(0, 2, 10, QLatin1Char('0'));
        m_strList << QString("%1:%2").arg(i, timeFormatValue, 10, QLatin1Char('0')).arg(30);
    }
    addItems(m_strList);
    m_timeEdit->setParent(this);
}

void CTimeEdit::initConnection()
{
    connect(m_timeEdit, &TimeEdit::signalFocusOut, this, &CTimeEdit::signalFocusOut);
    connect(CalendarManager::getInstance(), &CalendarManager::signalTimeFormatChanged, this, &CTimeEdit::setTimeFormat);
}

void CTimeEdit::showPopup()
{
    QComboBox::showPopup();
    //获取下拉视图容器
    QFrame *viewContainer = this->findChild<QFrame *>();
    if (viewContainer) {
        //如果显示视图容器则设置最大高度
        viewContainer->setFixedHeight(viewContainerMaxHeight);
        //获取combobox底部坐标
        QPoint showPoint = mapToGlobal(this->rect().bottomLeft());
        //控制视图容器宽度
        viewContainer->setFixedWidth(this->width());
        //将视图容器移动到combobox的底部
        viewContainer->move(showPoint.x(), showPoint.y());
    }
    //因改变了容器的高度，所以需要重新定位当前位置
    this->view()->scrollTo(this->view()->currentIndex());
}

void CTimeEdit::focusInEvent(QFocusEvent *event)
{
//    如果为tab焦点进入则选中时间
    if (event->reason() == Qt::TabFocusReason) {
        m_timeEdit->setFocus(Qt::TabFocusReason);
    }
    QComboBox::focusInEvent(event);
}

TimeEdit::TimeEdit(QWidget *parent)
    : QTimeEdit(parent)
{
    setButtonSymbols(QTimeEdit::NoButtons);
}

/**
 * @brief TimeEdit::getLineEdit获取lineedit
 */
QLineEdit *TimeEdit::getLineEdit()
{
    return lineEdit();
}

/**
 * @brief TimeEdit::focusOutEvent focusout事件
 */
void TimeEdit::focusOutEvent(QFocusEvent *event)
{
    //发送focusout信号到dialog
    emit signalFocusOut();
    QTimeEdit::focusOutEvent(event);
}
