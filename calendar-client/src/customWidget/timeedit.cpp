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
#include <QPainter>

#include <QVBoxLayout>
#include "calendarmanage.h"

//视图容器最大高度
const int viewContainerMaxHeight = 305;

CTimeEdit::CTimeEdit(QWidget *parent)
    : DComboBox(parent)
    , m_timeFormat(CalendarManager::getInstance()->getCalendarDateDataManage()->getTimeFormat())
    , m_timeFormatValue(
          CalendarManager::getInstance()->getCalendarDateDataManage()->getTimeFormatValue())
    , m_timeEdit(new CCustomTimeEdit())
    , m_hasFocus(false)
    , m_miniTime(QTime(0, 0, 0))
{
    initUI();
    initConnection();
}

CTimeEdit::~CTimeEdit()
{
    delete m_timeEdit;
}

void CTimeEdit::setMineTime(const QTime &mineTime)
{
    m_miniTime = mineTime;
}

void CTimeEdit::setTime(const QTime &time)
{
    m_time = time;
    setSelectItem(m_time);
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

void CTimeEdit::updateListItem(bool isShowTimeInterval)
{
    m_isShowTimeInterval = isShowTimeInterval;
    //清除列表
    clear();
    for (int i = 0; i < 48; ++i) {
        qreal timeInterval = i * 0.5;
        QString timeIntervalStr;
        if ( m_isShowTimeInterval ) {
            if ( timeInterval < 1 ) {
                timeIntervalStr = tr("(%1mins)").arg(i * 30);
            } else if (timeInterval == 1) {
                timeIntervalStr = tr("(%1 hour)").arg(timeInterval);
            } else {
                timeIntervalStr = tr("(%1 hours)").arg(timeInterval);
            }
        } else {
            timeIntervalStr = "";
        }
        QString userData = m_miniTime.addSecs(i * 30 * 60).toString(m_timeFormat);
        QString showStr = userData + timeIntervalStr;
        addItem(showStr, userData);
    }
}

void CTimeEdit::setTimeFormat(int value)
{
    //获取edit的当前时间
    QTime editCurrentTime = getTime();

    //根据value值,设置时间显示格式
    if (value) {
        m_timeFormat = "hh:mm";
    } else {
        m_timeFormat = "h:mm";
    }

    m_timeEdit->setDisplayFormat(m_timeFormat);

    //设置改变格式后的时间
    setTime(editCurrentTime);
    updateListItem(m_isShowTimeInterval);
}

void CTimeEdit::slotFocusDraw(bool showFocus)
{
    m_hasFocus = showFocus;
    update();
}

void CTimeEdit::setSelectItem(const QTime &time)
{
    //若有则设置选中，若没有则取消选中设置定位到相近值附近
    int similarNumber = -1;
    int diff = 24 * 60 * 60 * 1000;
    for (int i = 0 ; i < count(); ++i) {
        QVariant &&userData = itemData(i);
        QTime &&listTime = QTime::fromString(userData.toString(), m_timeFormat);
        int &&timeDiff = qAbs(listTime.msecsTo(time));
        if ( timeDiff < diff) {
            diff = timeDiff;
            similarNumber = i;
        }
    }
    if (diff == 0) {
        setCurrentIndex(similarNumber);
    } else {
        setCurrentIndex(-1);
    }
    scrollPosition = this->model()->index(similarNumber, 0);
}

void CTimeEdit::slotSetPlainText(const QString &arg)
{
    QString userData = currentData().toString();
    if ( userData.isEmpty()) {
        this->lineEdit()->setText(arg);
    } else {
        this->lineEdit()->setText(userData);
    }
}

void CTimeEdit::slotActivated(const QString &arg)
{
    slotSetPlainText(arg);
    emit signaleditingFinished();
}

void CTimeEdit::slotEditingFinished()
{
    setTime(m_timeEdit->time());
    emit signaleditingFinished();
}

void CTimeEdit::initUI()
{
    //关闭自动补全
    this->setAutoCompletion(false);
    //设置edit的宽度
    m_timeEdit->getLineEdit()->setFixedWidth(100);
    m_timeEdit->setDisplayFormat(m_timeFormat);
    updateListItem(m_isShowTimeInterval);
    setLineEdit(m_timeEdit->getLineEdit());
    m_timeEdit->setParent(this);
    setInsertPolicy(QComboBox::NoInsert);
}

void CTimeEdit::initConnection()
{
    connect(CalendarManager::getInstance(), &CalendarManager::signalTimeFormatChanged, this,
            &CTimeEdit::setTimeFormat);
    connect(m_timeEdit, &CCustomTimeEdit::signalUpdateFocus, this, &CTimeEdit::slotFocusDraw);
    connect(m_timeEdit->getLineEdit(), &QLineEdit::editingFinished, this,
            &CTimeEdit::slotEditingFinished);
    connect(this, static_cast<void (CTimeEdit::*)(const QString &)>(&CTimeEdit::activated), this,
            &CTimeEdit::slotActivated);
}

void CTimeEdit::showPopup()
{
    DComboBox::showPopup();
    //获取下拉视图容器
    QFrame *viewContainer = this->findChild<QFrame *>();
    if (viewContainer) {
        //移动前先隐藏
        viewContainer->hide();
        //如果显示视图容器则设置高度
        viewContainer->setFixedHeight(viewContainerMaxHeight);
        //设置最大高度
        viewContainer->setMaximumHeight(viewContainerMaxHeight + 1);
        //获取combobox底部坐标
        QPoint showPoint = mapToGlobal(this->rect().bottomLeft());
        //控制视图容器宽度
        viewContainer->setFixedWidth(this->width() + 10);
        //将视图容器移动到combobox的底部
        viewContainer->move(showPoint.x(), showPoint.y());
        //显示
        viewContainer->show();
    }
//    //因改变了容器的高度，所以需要重新定位当前位置
    if (this->view()->currentIndex() == scrollPosition) {
        this->view()->scrollTo(scrollPosition, QAbstractItemView::PositionAtCenter);
    } else {
        this->view()->scrollTo(scrollPosition, QAbstractItemView::PositionAtTop);
    }

}

void CTimeEdit::focusInEvent(QFocusEvent *event)
{
    DComboBox::focusInEvent(event);
    //    如果为tab焦点进入则选中时间
    if (event->reason() == Qt::TabFocusReason) {
        lineEdit()->setFocus(Qt::TabFocusReason);
    }
}

void CTimeEdit::paintEvent(QPaintEvent *e)
{
    DComboBox::paintEvent(e);
    //如果有焦点则设置焦点显示效果
    if (m_hasFocus) {
        QPainter painter(this);
        QStyleOptionFocusRect option;
        option.initFrom(this);
        option.backgroundColor = palette().color(QPalette::Background);
        style()->drawPrimitive(QStyle::PE_FrameFocusRect, &option, &painter, this);
    }
}
