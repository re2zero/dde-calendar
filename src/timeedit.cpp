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
#include "timeverticalscroll.h"
#include <QRegExpValidator>
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
    m_timeEdit->setText(m_time.toString("hh:mm"));
}

QTime CTimeEdit::getTime()
{
    m_time = QTime::fromString(m_timeEdit->text(), "hh:mm");
    return m_time;
}

void CTimeEdit::initUI()
{
    m_pListWidget = new DListWidget(this);

    m_timeEdit = new DLineEdit(this);
    m_timeEdit->setInputMask("00:00;#");
    m_timeEdit->setClearButtonEnabled(false);
    QRegExpValidator *validator = nullptr;
    QRegExp rx("0[0-9]:[0-5][0-9]|1[0-9]:[0-5][0-9]|2[0-3]:[0-5][0-9]");
    validator = new QRegExpValidator(rx, this);
    m_timeEdit->setValidator(validator);
    setModel(m_pListWidget->model());
    setView(m_pListWidget);
    setLineEdit(m_timeEdit);
    //disconnect(m_timeEdit, &QLineEdit::editingFinished, m_pListWidget, &QListWidget::update);
    //[2] 年月日选择控件
    QListWidgetItem *pitem = new QListWidgetItem(m_pListWidget);
    pitem->setSizeHint(QSize(m_pListWidget->width(), 300));

    m_pListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //pitem->setSizeHint(QSize(m_gradientItemList->width() - 5, 36)); //每次改变Item的高度
    //listItem->setBackgroundColor(Qt::white);
    pitem->setFlags(Qt::ItemIsTristate );
    m_verticalScroll = new CTimeVerticalScroll(m_pListWidget);
    m_verticalScroll->setMinimumWidth(m_pListWidget->width());
    m_pListWidget->setItemWidget(pitem, m_verticalScroll);
    m_verticalScroll->setRange(0, 23);
    m_timeEdit->setCursorPosition(1);
    m_pListWidget->addItem(pitem);
    m_pListWidget->setFixedWidth(width());
    m_verticalScroll->setFixedWidth(width());

}

void CTimeEdit::initConnection()
{
    connect(m_verticalScroll, &CTimeVerticalScroll::currentValueChanged, this, &CTimeEdit::slotcurrentValueChanged);
    connect(m_verticalScroll, &CTimeVerticalScroll::currentValueChangedClose, this, &CTimeEdit::slotcurrentValueChangedClose);
    //connect(m_timeEdit, &DLineEdit::editingFinished, this, &CTimeEdit::slotEidtChange);
    //connect(m_timeEdit, &DLineEdit::returnPressed, this, &CTimeEdit::slotEidtChange);
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
    // QComboBox::showPopup();
    m_pListWidget->setFixedWidth(width());
    m_pos = m_timeEdit->cursorPosition();
    m_verticalScroll->setFixedWidth(width());
    QString timetext = m_timeEdit->text();
    /*
        if (timetext.count() == 1)
        {
            timetext = "00:00";
        }
        if (timetext.count() == 3)
        {
            if (timetext.right(1) == ":")
            {
                timetext = timetext + "00";
            }
            else
            {
                timetext = "00"+ timetext;
            }
        }*/
    m_time = QTime::fromString(timetext, "hh:mm");
    if (m_pos < 3) {
        m_type = 0;
        m_verticalScroll->setRange(0, 23);
        m_verticalScroll->setCurrValue(m_time.hour());
    } else {
        m_type = 1;
        m_verticalScroll->setRange(0, 59);
        m_verticalScroll->setCurrValue(m_time.minute());
    }
    m_pListWidget->show();
    QComboBox::showPopup();
}
void CTimeEdit::hidePopup()
{
    QString timetext = m_time.toString("hh:mm");
    m_timeEdit->setText(timetext);
    QComboBox::hidePopup();
    return;
    // 移除旧item
    QListWidgetItem *item = NULL;
    int iCount = m_pListWidget->count();
    for (int i = iCount - 1; i >= 1; i--) {
        item = m_pListWidget->takeItem(i);
        delete item;
    }
}

void CTimeEdit::slotcurrentValueChanged(int value)
{
    if (m_type == 0) {

        QTime time = QTime(value, m_time.minute());
        if (!time.isValid()) return;
        m_time = time;
        m_timeEdit->setText(m_time.toString("hh:mm"));
    } else {
        QTime time = QTime(m_time.hour(), value);
        if (!time.isValid()) return;
        m_time = time;
        m_timeEdit->setText(m_time.toString("hh:mm"));
    }
    m_timeEdit->setCursorPosition(m_pos);
}

void CTimeEdit::slotcurrentValueChangedClose(int value)
{
    slotcurrentValueChanged(value);
    hidePopup();
}

void CTimeEdit::slotEidtChange()
{
    //QTime time = QTime(m_time.hour(), value);
    QTime time = QTime::fromString(m_timeEdit->text(), "hh:mm");

    QListWidgetItem *item = NULL;
    int iCount = m_pListWidget->count();
    for (int i = iCount - 1; i >= 1; i--) {
        item = m_pListWidget->takeItem(i);
        delete item;
    }
    if (!time.isValid()) return;
    m_time = time;
    m_timeEdit->setText(m_time.toString("hh:mm"));
}

