/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
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
#include "schceduledlg.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include "calendartimeeidt.h"
#include "timeedit.h"
#include "customcalendarwidget.h"
CSchceduleDlg::CSchceduleDlg(int type, QWidget *parent): DDialog(parent)
{
    initUI();
    initConnection();
    m_type = type;
    if (type == 1) {
        setTitle(tr("Create Schcedule"));
        m_beginDateEdit->setDate(QDate::currentDate());
        m_beginTimeEdit->setTime(QTime::currentTime());
        m_endDateEdit->setDate(QDate::currentDate());
        m_endTimeEdit->setTime(QTime::currentTime().addSecs(3600));
        m_remindbtGroup->button(0)->setChecked(true);
        m_repeatbtGroup->button(0)->setChecked(true);
    } else {
        setTitle(tr("Edit Schcedule"));
    }
    setFocusPolicy(Qt::WheelFocus);
    setFixedSize(550, 460);
}

void CSchceduleDlg::setData(const ScheduleInfo &info)
{
    m_scheduleInfo = info;
    m_typeComBox->setCurrentIndex(info.infotype);
    m_textEdit->setText(info.titleName);
    m_beginDateEdit->setDate(info.beginDateTime.date());
    m_beginTimeEdit->setTime(info.beginDateTime.time());
    m_endDateEdit->setDate(info.endDateTime.date());
    m_endTimeEdit->setTime(info.endDateTime.time());
    m_remindbtGroup->button(info.remind)->setChecked(true);
    m_repeatbtGroup->button(info.repeat)->setChecked(true);
}

void CSchceduleDlg::setDate(const QDateTime &date)
{
    m_beginDateEdit->setDate(date.date());
    m_beginTimeEdit->setTime(date.time());
    m_endDateEdit->setDate(date.date());
    m_endTimeEdit->setTime(date.time().addSecs(3600));
    m_remindbtGroup->button(0)->setChecked(true);
    m_repeatbtGroup->button(0)->setChecked(true);
}

ScheduleInfo CSchceduleDlg::getData()
{
    return m_scheduleInfo;
}

void CSchceduleDlg::slotCancelBt()
{
    reject();
}

void CSchceduleDlg::slotOkBt()
{
    QDateTime beginDateTime, endDateTime;
    beginDateTime.setDate(m_beginDateEdit->date());
    beginDateTime.setTime(m_beginTimeEdit->getTime());
    endDateTime.setDate(m_endDateEdit->date());
    endDateTime.setTime(m_endTimeEdit->getTime());
    if (m_textEdit->toPlainText().isEmpty()) {
        QMessageBox::warning(this, tr("error"), tr("Schcedule is empty!"));
        return;
    }
    if (beginDateTime >= endDateTime) {
        QMessageBox::warning(this, tr("error"), tr("The end time less than begin time!"));
        return;
    }
    if (m_type == 0) m_scheduleInfo.id = -1;
    m_scheduleInfo.titleName = m_textEdit->toPlainText();
    m_scheduleInfo.infotype = m_typeComBox->currentIndex();
    m_scheduleInfo.remind = m_remindbtGroup->checkedId();
    m_scheduleInfo.repeat = m_repeatbtGroup->checkedId();
    m_scheduleInfo.beginDateTime = beginDateTime;
    m_scheduleInfo.endDateTime = endDateTime;
    qint64  alltime =  m_scheduleInfo.beginDateTime.secsTo(m_scheduleInfo.endDateTime);
    if (alltime > 86400) {
        m_scheduleInfo.allday = 1;
    } else {
        m_scheduleInfo.allday = 0;
    }
    accept();
}

void CSchceduleDlg::slotTextChange()
{
    QString textContent = m_textEdit->toPlainText();

    int length = textContent.count();

    int maxLength = 256; // 最大字符数

    if (length > maxLength) {
        QTextCursor cursor = m_textEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        if (cursor.hasSelection()) {
            cursor.clearSelection();
        }
        cursor.deletePreviousChar();
        //设置当前的光标为更改后的光标
        m_textEdit->setTextCursor(cursor);
        QMessageBox::information(this, tr("infomation"), tr("Max length is 256!"));
    }
}

void CSchceduleDlg::slotBDateEidtInfo(const QDate &date)
{
    m_beginDateEdit->setDate(date);
}

void CSchceduleDlg::slotEDateEidtInfo(const QDate &date)
{
    m_endDateEdit->setDate(date);
}

void CSchceduleDlg::initUI()
{

    QVBoxLayout *rightlayout = new QVBoxLayout;
    QVBoxLayout *leftlayout = new QVBoxLayout;
    m_typeLabel = new DLabel(tr("Type:"));
    m_typeComBox = new QComboBox ();
    m_typeComBox->addItem(tr("Work"));
    m_typeComBox->addItem(tr("Life"));
    m_typeComBox->addItem(tr("Other"));
    leftlayout->addWidget(m_typeLabel, 1);
    rightlayout->addWidget(m_typeComBox);

    m_contentLabel = new DLabel (tr("Content:"));
    leftlayout->addWidget(m_contentLabel, 7);
    m_textEdit = new DTextEdit(this);
    m_textEdit->setTextBackgroundColor(Qt::white);
    m_textEdit->setStyleSheet("QTextEdit { background: white }");
    m_textEdit->setTextColor(Qt::black);
    rightlayout->addWidget(m_textEdit);

    m_beginTimeLabel = new DLabel (tr("Begin Time:"));
    leftlayout->addWidget(m_beginTimeLabel, 1);
    QHBoxLayout *begintimelayout  = new QHBoxLayout;
    m_beginDateEdit = new QDateEdit(this);
    m_beginTimeEdit = new CTimeEdit(this);
    m_beginDateEdit->setCalendarPopup(true);
    //m_beginTimeEdit->setCalendarPopup(true);
    m_beginDateEdit->setDisplayFormat("yyyy-MM-dd");
    //m_beginTimeEdit->setDisplayFormat("HH:mm");
    //m_beginTimeEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    begintimelayout->addWidget(m_beginDateEdit, 1);
    QHBoxLayout *begintimeelayout  = new QHBoxLayout;
    begintimeelayout->addWidget(m_beginTimeEdit);

    begintimelayout->addLayout(begintimeelayout, 1);

    rightlayout->addLayout(begintimelayout);

    m_endTimeLabel = new DLabel (tr("End Time:"));
    leftlayout->addWidget(m_endTimeLabel, 1);
    QHBoxLayout *endtimelayout  = new QHBoxLayout;
    m_endDateEdit = new QDateEdit(this);
    m_endTimeEdit = new CTimeEdit(this);
    m_endDateEdit->setCalendarPopup(true);
    // m_endTimeEdit->setCalendarPopup(true);
    m_endDateEdit->setDisplayFormat("yyyy-MM-dd");
    //m_endTimeEdit->setDisplayFormat("HH:mm");
    // m_endTimeEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);

    QHBoxLayout *endtimeelayout  = new QHBoxLayout;

    endtimelayout->addWidget(m_endDateEdit, 1);
    endtimeelayout->addWidget(m_endTimeEdit);
    endtimelayout->addLayout(endtimeelayout, 1);

    rightlayout->addLayout(endtimelayout);

    m_remindSetLabel = new DLabel (tr("Remind Set:"));
    leftlayout->addWidget(m_remindSetLabel, 1);
    m_remindbtGroup = new QButtonGroup(this);
    QHBoxLayout *remindlayout  = new QHBoxLayout;
    m_currentDayBt = new QRadioButton(tr("Current Day"));
    remindlayout->addWidget(m_currentDayBt, 1);
    m_remindbtGroup->addButton(m_currentDayBt, 0);
    m_earlierDayBt = new QRadioButton(tr("Earlier Day"));
    remindlayout->addWidget(m_earlierDayBt, 1);
    m_remindbtGroup->addButton(m_earlierDayBt, 1);
    m_threedaysagoBt = new QRadioButton(tr("Three Days Ago"));
    remindlayout->addWidget(m_threedaysagoBt, 1);
    m_remindbtGroup->addButton(m_threedaysagoBt, 2);
    m_weekDayagoBt = new QRadioButton(tr("Week Day ago"));
    remindlayout->addWidget(m_weekDayagoBt, 1);
    m_remindbtGroup->addButton(m_weekDayagoBt, 3);
    rightlayout->addLayout(remindlayout);

    m_repeatLabel = new DLabel (tr("Repeat Set:"));
    leftlayout->addWidget(m_repeatLabel, 1);
    m_repeatbtGroup = new QButtonGroup(this);
    QHBoxLayout *repeatlayout  = new QHBoxLayout;
    m_onceTimeBt = new QRadioButton(tr("Once Time"));
    repeatlayout->addWidget(m_onceTimeBt, 1);
    m_repeatbtGroup->addButton(m_onceTimeBt, 0);
    m_everyDayBt = new QRadioButton(tr("Every Day"));
    repeatlayout->addWidget(m_everyDayBt, 1);
    m_repeatbtGroup->addButton(m_everyDayBt, 1);
    m_everyweekBt = new QRadioButton(tr("Every Week"));
    repeatlayout->addWidget(m_everyweekBt, 1);
    m_repeatbtGroup->addButton(m_everyweekBt, 2);
    m_everymonthBt = new QRadioButton(tr("Every Month"));
    repeatlayout->addWidget(m_everymonthBt, 1);
    m_repeatbtGroup->addButton(m_everymonthBt, 3);
    m_everyyearBt = new QRadioButton(tr("Every Year"));
    repeatlayout->addWidget(m_everyyearBt, 1);
    m_repeatbtGroup->addButton(m_everyyearBt, 4);
    rightlayout->addLayout(repeatlayout);
    QHBoxLayout *uplayout  = new QHBoxLayout;
    uplayout->addLayout(leftlayout);
    uplayout->addLayout(rightlayout);
    QHBoxLayout *downlayout  = new QHBoxLayout;
    m_cancelBt = new DPushButton(tr("Cancel"));
    m_OkBt = new DPushButton(tr("OK"));
    downlayout->addWidget(m_cancelBt);
    downlayout->addWidget(m_OkBt);
    QWidget *gwi = new QWidget;
    QVBoxLayout *maintlayout = new QVBoxLayout;
    maintlayout->addLayout(uplayout);
    maintlayout->addLayout(downlayout);
    gwi->setLayout(maintlayout);
    addContent(gwi);
    initDateEdit();
}

void CSchceduleDlg::initConnection()
{
    connect(m_cancelBt, &DPushButton::clicked, this, &CSchceduleDlg::slotCancelBt);
    connect(m_OkBt, &DPushButton::clicked, this, &CSchceduleDlg::slotOkBt);
    connect(m_textEdit, &DTextEdit::textChanged, this, &CSchceduleDlg::slotTextChange);

    connect(m_bCustomDateW, &CCustomCalendarWidget::signalSetCalendarTime, this, &CSchceduleDlg::slotBDateEidtInfo);

    connect(m_eCustomDateW, &CCustomCalendarWidget::signalSetCalendarTime, this, &CSchceduleDlg::slotEDateEidtInfo);
}

void CSchceduleDlg::initDateEdit()
{
    m_bCustomDateW = new CCustomCalendarWidget(0);
    m_beginDateEdit->setCalendarWidget(m_bCustomDateW);
    m_beginDateEdit->setMinimumDate(QDate(1900, 1, 1)); // 0天
    m_beginDateEdit->setMaximumDate(QDate(3000, 1, 1)); //
    //m_beginDateEdit->setContextMenuPolicy(Qt::NoContextMenu);
    m_bCustomDateW->disconnect(SIGNAL(selectionChanged()));
    m_bCustomDateW->disconnect(SIGNAL(clicked(QDate)));

    m_eCustomDateW = new CCustomCalendarWidget(0);
    m_endDateEdit->setCalendarWidget(m_eCustomDateW);
    m_endDateEdit->setMinimumDate(QDate(1900, 1, 1)); // 0天
    m_endDateEdit->setMaximumDate(QDate(3000, 1, 1)); //
    m_endDateEdit->setContextMenuPolicy(Qt::NoContextMenu);
    m_eCustomDateW->disconnect(SIGNAL(selectionChanged()));
    m_eCustomDateW->disconnect(SIGNAL(clicked(QDate)));

}

