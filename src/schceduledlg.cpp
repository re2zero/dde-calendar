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
    beginDateTime.setTime(m_beginTimeEdit->time());
    endDateTime.setDate(m_endDateEdit->date());
    endDateTime.setTime(m_endTimeEdit->time());
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

void CSchceduleDlg::slotBeginTimeBt()
{
    m_btimeeditflag = !m_btimeeditflag;

    if (m_btimeeditflag) {
        int px = m_beginTimeEdit->x();
        int py = m_beginTimeEdit->y();
        QPoint pos = mapToGlobal(QPoint(px, py));
        m_bCalendarTimeEidt->setTime(m_beginTimeEdit->time());
        m_bCalendarTimeEidt->move(pos.x() + 20, pos.y() + 2 * m_beginTimeEdit->height() + 15);
        m_bCalendarTimeEidt->show();
    } else {
        m_bCalendarTimeEidt->hide();
    }
}

void CSchceduleDlg::slotEndTimeBt()
{
    m_etimeeditflag = !m_etimeeditflag;

    if (m_etimeeditflag) {
        int px = m_endTimeEdit->x();
        int py = m_endTimeEdit->y();
        QPoint pos = mapToGlobal(QPoint(px, py));
        m_eCalendarTimeEidt->setTime(m_endTimeEdit->time());
        m_eCalendarTimeEidt->move(pos.x() + 20, pos.y() + 2 * m_endTimeEdit->height() + 15);
        m_eCalendarTimeEidt->show();
    } else {
        m_eCalendarTimeEidt->hide();
    }
}

void CSchceduleDlg::slotBCalendarTimeEidtInfo()
{
    if (m_bCalendarTimeEidt->getCrrentAMorPm() == tr("AM")) {
        QTime time(m_bCalendarTimeEidt->getHour(), m_bCalendarTimeEidt->getMin());
        m_beginTimeEdit->setTime(time);
    } else {
        QTime time(m_bCalendarTimeEidt->getHour() + 12, m_bCalendarTimeEidt->getMin());

        m_beginTimeEdit->setTime(time);
    }
}

void CSchceduleDlg::slotECalendarTimeEidtInfo()
{
    if (m_eCalendarTimeEidt->getCrrentAMorPm() == tr("AM")) {
        m_endTimeEdit->setTime(QTime(m_eCalendarTimeEidt->getHour(), m_eCalendarTimeEidt->getMin()));
    } else {
        m_endTimeEdit->setTime(QTime(m_eCalendarTimeEidt->getHour() + 12, m_eCalendarTimeEidt->getMin()));
    }
}

void CSchceduleDlg::slotBTimeEidtInfo(const QTime &time)
{
    m_bCalendarTimeEidt->setTime(time);
}

void CSchceduleDlg::slotETimeEidtInfo(const QTime &time)
{
    m_eCalendarTimeEidt->setTime(time);
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
    m_beginTimeEdit = new QTimeEdit(this);
    m_beginDateEdit->setCalendarPopup(true);
    //m_beginTimeEdit->setCalendarPopup(true);
    m_beginDateEdit->setDisplayFormat("yyyy-MM-dd");
    m_beginTimeEdit->setDisplayFormat("HH:mm");
    m_beginTimeEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    begintimelayout->addWidget(m_beginDateEdit, 1);
    QHBoxLayout *begintimeelayout  = new QHBoxLayout;
    begintimeelayout->addWidget(m_beginTimeEdit);
    m_beginTimeEidtBt = new DArrowButton;
    m_beginTimeEidtBt->setFixedWidth(20);
    m_beginTimeEidtBt->setArrowDirection(DArrowButton::ArrowDown);
    begintimeelayout->addWidget(m_beginTimeEidtBt);
    begintimelayout->addLayout(begintimeelayout, 1);

    rightlayout->addLayout(begintimelayout);

    m_endTimeLabel = new DLabel (tr("End Time:"));
    leftlayout->addWidget(m_endTimeLabel, 1);
    QHBoxLayout *endtimelayout  = new QHBoxLayout;
    m_endDateEdit = new QDateEdit(this);
    m_endTimeEdit = new QTimeEdit(this);
    m_endDateEdit->setCalendarPopup(true);
    // m_endTimeEdit->setCalendarPopup(true);
    m_endDateEdit->setDisplayFormat("yyyy-MM-dd");
    m_endTimeEdit->setDisplayFormat("HH:mm");
    m_endTimeEdit->setButtonSymbols(QAbstractSpinBox::NoButtons);
    m_endTimeEidtBt = new DArrowButton;
    m_endTimeEidtBt->setFixedWidth(20);
    m_endTimeEidtBt->setArrowDirection(DArrowButton::ArrowDown);
    QHBoxLayout *endtimeelayout  = new QHBoxLayout;

    endtimelayout->addWidget(m_endDateEdit, 1);
    endtimeelayout->addWidget(m_endTimeEdit);
    endtimeelayout->addWidget(m_endTimeEidtBt);
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
    m_cancelBt = new DTextButton(tr("Cancel"));
    m_OkBt = new DTextButton(tr("OK"));
    downlayout->addWidget(m_cancelBt);
    downlayout->addWidget(m_OkBt);
    QWidget *gwi = new QWidget;
    QVBoxLayout *maintlayout = new QVBoxLayout;
    maintlayout->addLayout(uplayout);
    maintlayout->addLayout(downlayout);
    gwi->setLayout(maintlayout);
    addContent(gwi);
    m_bCalendarTimeEidt = new CCalendarTimeEidt(m_beginTimeEdit);
    m_eCalendarTimeEidt = new CCalendarTimeEidt(m_endTimeEdit);
    m_bCalendarTimeEidt->setFixedSize(200, 300);
    m_eCalendarTimeEidt->setFixedSize(200, 300);
    m_bCalendarTimeEidt->hide();
    m_eCalendarTimeEidt->hide();
    m_bCalendarTimeEidt->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    //m_bCalendarTimeEidt->setAttribute(Qt::WA_TranslucentBackground);
    m_eCalendarTimeEidt->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    //m_bCalendarTimeEidt->setAttribute(Qt::WA_TranslucentBackground);
}

void CSchceduleDlg::initConnection()
{
    connect(m_cancelBt, &DTextButton::clicked, this, &CSchceduleDlg::slotCancelBt);
    connect(m_OkBt, &DTextButton::clicked, this, &CSchceduleDlg::slotOkBt);
    connect(m_textEdit, &DTextEdit::textChanged, this, &CSchceduleDlg::slotTextChange);
    connect(m_beginTimeEidtBt, &DArrowButton::mousePress, this, &CSchceduleDlg::slotBeginTimeBt);
    connect(m_endTimeEidtBt, &DArrowButton::mousePress, this, &CSchceduleDlg::slotEndTimeBt);
    connect(m_bCalendarTimeEidt, &CCalendarTimeEidt::currentTValueChanged, this, &CSchceduleDlg::slotBCalendarTimeEidtInfo);
    connect(m_bCalendarTimeEidt, &CCalendarTimeEidt::currentHValueChanged, this, &CSchceduleDlg::slotBCalendarTimeEidtInfo);
    connect(m_bCalendarTimeEidt, &CCalendarTimeEidt::currentMValueChanged, this, &CSchceduleDlg::slotBCalendarTimeEidtInfo);

    connect(m_eCalendarTimeEidt, &CCalendarTimeEidt::currentTValueChanged, this, &CSchceduleDlg::slotECalendarTimeEidtInfo);
    connect(m_eCalendarTimeEidt, &CCalendarTimeEidt::currentHValueChanged, this, &CSchceduleDlg::slotECalendarTimeEidtInfo);
    connect(m_eCalendarTimeEidt, &CCalendarTimeEidt::currentMValueChanged, this, &CSchceduleDlg::slotECalendarTimeEidtInfo);

    //connect(m_beginTimeEdit, &QTimeEdit::userTimeChanged, this, &CSchceduleDlg::slotBTimeEidtInfo);
    //connect(m_endTimeEdit, &QTimeEdit::userTimeChanged, this, &CSchceduleDlg::slotETimeEidtInfo);

}
void CSchceduleDlg::focusInEvent(QFocusEvent *event)
{
    m_eCalendarTimeEidt->hide();
    m_bCalendarTimeEidt->hide();
    m_btimeeditflag = false;
    m_etimeeditflag = false;
}
