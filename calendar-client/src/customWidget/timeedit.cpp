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

//视图容器最大高度
const int viewContainerMaxHeight = 305;

CTimeEdit::CTimeEdit(QWidget *parent)
    : DComboBox(parent)
{
    initUI();
    initConnection();
    setFocus(Qt::MouseFocusReason);
}

CTimeEdit::~CTimeEdit()
{
}

void CTimeEdit::setTime(QTime time)
{
    m_time = time;
    setSelectItem();
    m_timeEdit->setText(time.toString("hh:mm"));
}

QTime CTimeEdit::getTime()
{
    //获取显示的text
    QString timetext = m_timeEdit->lineEdit()->displayText();
    //将text转换为时间
    m_time = QTime::fromString(timetext, "hh:mm");
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
    int index = m_strList.indexOf(QTime(hour, minute).toString("hh:mm"));
    //设置下拉列表当前index
    setCurrentIndex(index);
}

void CTimeEdit::initUI()
{
    //关闭自动补全
    this->setAutoCompletion(false);
    m_timeEdit = new DLineEdit(this);
    m_timeEdit->lineEdit()->setInputMask("00:00;0");
    m_timeEdit->setClearButtonEnabled(false);
    QRegExpValidator *validator = nullptr;
    QRegExp rx("0[0-9]:[0-5][0-9]|1[0-9]:[0-5][0-9]|2[0-3]:[0-5][0-9]");
    validator = new QRegExpValidator(rx, this);
    m_timeEdit->lineEdit()->setValidator(validator);
    setLineEdit(m_timeEdit->lineEdit());
    for (int i = 0; i < 24; i++) {
        m_strList << QString("%1:%2").arg(i, 2, 10, QLatin1Char('0')).arg(0, 2, 10, QLatin1Char('0'));
        m_strList << QString("%1:%2").arg(i, 2, 10, QLatin1Char('0')).arg(30);
    }
    this->addItems(m_strList);
}

void CTimeEdit::initConnection()
{
    m_timeEdit->disconnect(SIGNAL(returnPressed()));
    m_timeEdit->disconnect(SIGNAL(editingFinished()));
    m_timeEdit->disconnect(SIGNAL(selectionChanged()));
    m_timeEdit->disconnect(SIGNAL(textChanged(const QString &)));
    m_timeEdit->disconnect(SIGNAL(textEdited(const QString &)));
    m_timeEdit->disconnect(SIGNAL(cursorPositionChanged(int, int)));

    disconnect(SIGNAL(activated(int)));
    disconnect(SIGNAL(activated(const QString &)));
    disconnect(SIGNAL(currentIndexChanged(int)));
    disconnect(SIGNAL(currentIndexChanged(const QString &)));
    disconnect(SIGNAL(currentTextChanged(const QString &)));
    disconnect(SIGNAL(editTextChanged(const QString &)));
    disconnect(SIGNAL(highlighted(int)));
    disconnect(SIGNAL(highlighted(const QString &)));
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
    Q_UNUSED(event);
    //如果tab切换到则全部选中
    this->lineEdit()->selectAll();
    //更新显示
    this->update();
}
