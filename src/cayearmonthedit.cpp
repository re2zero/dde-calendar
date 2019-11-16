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
#include "cayearmonthedit.h"
#include "timeverticalscroll.h"
#include <QRegExpValidator>
CCaYearMonthEdit::CCaYearMonthEdit(QWidget *parent)
    : DComboBox(parent)
{
    initUI();
    initConnection();
}

CCaYearMonthEdit::~CCaYearMonthEdit()
{
}

void CCaYearMonthEdit::setRange(int minv, int maxv)
{
    m_min = minv;
    m_max = maxv;
}

void CCaYearMonthEdit::setPadStr(QString vstr)
{
    m_paddingStr = vstr;
    m_verticalScroll->setPadingStr(vstr);
}

int CCaYearMonthEdit::getNum()
{
    return m_num;
}

void CCaYearMonthEdit::initUI()
{
    m_pListWidget = new DListWidget(this);
    m_pListWidget->setLineWidth(0);
    m_timeEdit = new QLineEdit(m_pListWidget);
    //m_timeEdit->setInputMask("00:00;#");
    setModel(m_pListWidget->model());
    setView(m_pListWidget);
    setLineEdit(m_timeEdit);
    m_timeEdit->setAlignment(Qt::AlignRight);//右对齐
    m_timeEdit->setReadOnly(true);
    //disconnect(m_timeEdit, &QLineEdit::editingFinished, m_pListWidget, &QListWidget::update);
    //[2] 年月日选择控件
    QListWidgetItem *pitem = new QListWidgetItem(m_pListWidget);
    pitem->setSizeHint(QSize(m_pListWidget->width(), 300));
    pitem->setFlags(Qt::ItemIsTristate );
    m_pListWidget->addItem(pitem);


    m_verticalScroll = new CTimeVerticalScroll(m_pListWidget);
    m_pListWidget->setItemWidget(pitem, m_verticalScroll);
    m_verticalScroll->setRange(0, 23);
    m_verticalScroll->setmZeropadd(false);
}

void CCaYearMonthEdit::initConnection()
{
    connect(m_verticalScroll, &CTimeVerticalScroll::currentValueChanged, this, &CCaYearMonthEdit::slotcurrentValueChanged);
    connect(m_verticalScroll, &CTimeVerticalScroll::currentValueChangedClose, this, &CCaYearMonthEdit::slotcurrentValueChangedClose);

}

void CCaYearMonthEdit::showPopup()
{
    m_pListWidget->setFixedWidth(width());
    m_verticalScroll->setFixedWidth(width());

    QString timetext = m_timeEdit->text();
    timetext.remove(m_paddingStr);
    m_num = timetext.toInt();

    m_verticalScroll->setRange(m_min, m_max);
    m_verticalScroll->setCurrValue(m_num);

    m_pListWidget->show();
    QComboBox::showPopup();
}
void CCaYearMonthEdit::hidePopup()
{
    QComboBox::hidePopup();
    // 移除旧item
    QListWidgetItem *item = NULL;
    int iCount = m_pListWidget->count();
    for (int i = iCount - 1; i >= 1; i--) {
        item = m_pListWidget->takeItem(i);
        delete item;
    }
}

void CCaYearMonthEdit::setCurrentNum(int num)
{
    m_num = num;
    m_timeEdit->setText(QString::number(m_num) + m_paddingStr);
}

void CCaYearMonthEdit::slotcurrentValueChanged(int value)
{
    m_num = value;
    m_timeEdit->setText(QString::number(m_num) + m_paddingStr);
    emit signalsCurrentValueChange(m_num);
}

void CCaYearMonthEdit::slotcurrentValueChangedClose(int value)
{
    slotcurrentValueChanged(value);
    hidePopup();
}


