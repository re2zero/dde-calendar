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
#include "timeeditctrl.h"
#include <QRegExpValidator>
CTimeEditCtrl::CTimeEditCtrl(QWidget *parent)
    : DComboBox(parent)
{
    initUI();
    initConnection();
    setFocus(Qt::MouseFocusReason);
}

CTimeEditCtrl::~CTimeEditCtrl()
{
}

void CTimeEditCtrl::setTime(QTime time)
{
    m_time = time;
    m_timeEdit->setText(m_time.toString("hh:mm"));
}

QTime CTimeEditCtrl::getTime()
{
    m_time = QTime::fromString(m_timeEdit->text(), "hh:mm");
    return m_time;
}

void CTimeEditCtrl::initUI()
{
    m_pListWidget = new DListWidget(this);
    m_pListWidget->setContentsMargins(0, 8, 0, 8);
    m_pListWidget->setSpacing(0);
    m_timeEdit = new DLineEdit(this);
    m_timeEdit->lineEdit()->setInputMask("00:00;#");
    m_timeEdit->setClearButtonEnabled(false);
    QRegExpValidator *validator = nullptr;
    QRegExp rx("0[0-9]:[0-5][0-9]|1[0-9]:[0-5][0-9]|2[0-3]:[0-5][0-9]");
    validator = new QRegExpValidator(rx, this);
    m_timeEdit->lineEdit()->setValidator(validator);
    setModel(m_pListWidget->model());
    setView(m_pListWidget);
    setLineEdit(m_timeEdit->lineEdit());
    //disconnect(m_timeEdit, &QLineEdit::editingFinished, m_pListWidget, &QListWidget::update);
    m_pListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_timeEdit->lineEdit()->setCursorPosition(1);
    m_pListWidget->setFixedWidth(width());
    //m_pListWidget->setFixedHeight(311);
    for (int i = 0; i < 10; i++) {
        m_hourslist.append("0" + QString::number(i));
        m_minlist.append("0" + QString::number(i));
    }
    for (int i = 10; i < 24; i++) {
        m_hourslist.append(QString::number(i));
    }
    for (int i = 10; i < 60; i++) {
        m_minlist.append(QString::number(i));
    }
    //setIconSize(QSize(width(), 311));
    //setEditable(true);
    //setMaxVisibleItems(8);
}

void CTimeEditCtrl::initConnection()
{
    m_timeEdit->disconnect(SIGNAL(returnPressed()));
    m_timeEdit->disconnect(SIGNAL(editingFinished()));
    m_timeEdit->disconnect(SIGNAL(selectionChanged()));
    m_timeEdit->disconnect(SIGNAL(textChanged(const QString &)));
    m_timeEdit->disconnect(SIGNAL(textEdited(const QString &)));
    m_timeEdit->disconnect(SIGNAL(cursorPositionChanged(int, int)));

    connect(this, SIGNAL(activated(int)), this, SLOT(slotcurrentValueChanged(int)));

    //disconnect(SIGNAL(activated(int)));
    //disconnect(SIGNAL(activated(const QString &)));
    disconnect(SIGNAL(currentIndexChanged(int)));
    disconnect(SIGNAL(currentIndexChanged(const QString &)));
    disconnect(SIGNAL(currentTextChanged(const QString &)));
    disconnect(SIGNAL(editTextChanged(const QString &)));
    disconnect(SIGNAL(highlighted(int)));
    disconnect(SIGNAL(highlighted(const QString &)));
}

void CTimeEditCtrl::showPopup()
{
    // QComboBox::showPopup();
    m_pListWidget->setFixedWidth(width());
    m_pos = m_timeEdit->lineEdit()->cursorPosition();
    QString timetext = m_timeEdit->text();
    m_pListWidget->clear();
    m_time = QTime::fromString(timetext, "hh:mm");
    if (m_pos < 3) {
        m_type = 0;
        addlistvalue(m_hourslist, m_pListWidget, m_time.hour());
    } else {
        m_type = 1;
        addlistvalue(m_minlist, m_pListWidget, m_time.minute());
        //m_pListWidget->addItems(m_minlist);
    }
    m_pListWidget->show();
    QComboBox::showPopup();
}
void CTimeEditCtrl::hidePopup()
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

void CTimeEditCtrl::addlistvalue(QStringList vListData, DListWidget *listw, int currentvalue)
{
    listw->clear();
    for (int i = 0; i < vListData.count(); i++) {
        QListWidgetItem *titem = new QListWidgetItem(vListData.at(i), listw);
        titem->setSizeHint(QSize(listw->width(), 36));
        titem->setTextAlignment(Qt::AlignCenter);
        listw->addItem(titem);
    }
    listw->setCurrentRow(currentvalue, QItemSelectionModel::SelectCurrent);
}

void CTimeEditCtrl::slotcurrentValueChanged(int value)
{
    if (m_type == 0) {

        QTime time = QTime(m_hourslist.at(value).toInt(), m_time.minute());
        if (!time.isValid()) return;
        m_time = time;
        m_timeEdit->setText(m_time.toString("hh:mm"));
    } else {
        QTime time = QTime(m_time.hour(), m_minlist.at(value).toInt());
        if (!time.isValid()) return;
        m_time = time;
        m_timeEdit->setText(m_time.toString("hh:mm"));
    }
    m_timeEdit->lineEdit()->setCursorPosition(m_pos);
}

void CTimeEditCtrl::slotcurrentValueChanged(const QString &text)
{

}

void CTimeEditCtrl::slotcurrentValueChangedClose(int value)
{
    slotcurrentValueChanged(value);
    hidePopup();
}

void CTimeEditCtrl::slotEidtChange()
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

