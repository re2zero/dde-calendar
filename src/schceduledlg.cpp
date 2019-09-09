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
#include <DHiDPIHelper>
#include <QIcon>
#include <QIntValidator>
#include "scheduledatamanage.h"
#include <DMessageBox>
#include <DPalette>
DGUI_USE_NAMESPACE
CSchceduleDlg::CSchceduleDlg(int type, QWidget *parent): DDialog(parent)
{
    m_type = type;
    initUI();
    initConnection();
    if (type == 1) {
        setTitle(tr("Create Schcedule"));
        m_beginDateEdit->setDate(QDate::currentDate());
        m_beginTimeEdit->setTime(QTime::currentTime());
        m_endDateEdit->setDate(QDate::currentDate());
        m_endTimeEdit->setTime(QTime::currentTime().addSecs(3600));
    } else {
        setTitle(tr("Edit Schcedule"));
    }
    setFocusPolicy(Qt::WheelFocus);
    setFixedSize(438, 526);
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
}

void CSchceduleDlg::setData(const ScheduleDtailInfo &info)
{
    m_scheduleDtailInfo = info;
    m_typeComBox->setCurrentIndex(info.type.ID - 1);
    m_textEdit->setText(info.titleName);
    m_beginDateEdit->setDate(info.beginDateTime.date());
    m_beginTimeEdit->setTime(info.beginDateTime.time());
    m_endDateEdit->setDate(info.endDateTime.date());
    m_endTimeEdit->setTime(info.endDateTime.time());
    m_allDayCheckbox->setChecked(info.allday);
    slotallDayStateChanged(info.allday);
    initRmindRpeatUI();
}

void CSchceduleDlg::setDate(const QDateTime &date)
{
    m_currentDate = date;
    m_beginDateEdit->setDate(date.date());
    m_beginTimeEdit->setTime(date.time());
    m_endDateEdit->setDate(date.date());
    m_endTimeEdit->setTime(date.time().addSecs(3600));
}

ScheduleInfo CSchceduleDlg::getData()
{
    return m_scheduleInfo;
}

ScheduleDtailInfo CSchceduleDlg::getScheduleData()
{
    return m_scheduleDtailInfo;
}

void CSchceduleDlg::slotCancelBt()
{
    reject();
}

void CSchceduleDlg::slotOkBt()
{
    ScheduleDtailInfo scheduleDtailInfo = m_scheduleDtailInfo;
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
    scheduleDtailInfo.allday = m_allDayCheckbox->isChecked();
    if (m_rmindCombox->currentIndex() == 0) scheduleDtailInfo.remind = false;
    else {
        scheduleDtailInfo.remind = true;
    }
    if (scheduleDtailInfo.allday) {
        if (scheduleDtailInfo.remind) {
            scheduleDtailInfo.remindData.time = QTime(9, 0);
            if (m_rmindCombox->currentIndex() == 1) {
                scheduleDtailInfo.remindData.n = 0;
            } else if (m_rmindCombox->currentIndex() == 2) {
                scheduleDtailInfo.remindData.n = 1;
            } else if (m_rmindCombox->currentIndex() == 3) {
                scheduleDtailInfo.remindData.n = 2;
            } else if (m_rmindCombox->currentIndex() == 4) {
                scheduleDtailInfo.remindData.n = 7;
            }
        }
    } else {
        if (scheduleDtailInfo.remind) {
            if (m_rmindCombox->currentIndex() == 1) {
                scheduleDtailInfo.remindData.n = 0;
            } else if (m_rmindCombox->currentIndex() == 2) {
                scheduleDtailInfo.remindData.n = 15;
            } else if (m_rmindCombox->currentIndex() == 3) {
                scheduleDtailInfo.remindData.n = 30;
            } else if (m_rmindCombox->currentIndex() == 4) {
                scheduleDtailInfo.remindData.n = 60;
            } else if (m_rmindCombox->currentIndex() == 5) {
                scheduleDtailInfo.remindData.n = 1440;
            } else if (m_rmindCombox->currentIndex() == 6) {
                scheduleDtailInfo.remindData.n = 2880;
            }
        }
    }
    scheduleDtailInfo.rpeat = m_beginrepeatCombox->currentIndex();
    if (scheduleDtailInfo.rpeat != 0) {
        scheduleDtailInfo.enddata.type = m_endrepeatCombox->currentIndex();
        if (m_endrepeatCombox->currentIndex() == 1) {
            if (m_endrepeattimes->text().isEmpty()) {
                QMessageBox::warning(this, tr("error"), tr("The end repeat times is null!"));
                return;
            }
            scheduleDtailInfo.enddata.tcount = m_endrepeattimes->text().toInt();
        } else if (m_endrepeatCombox->currentIndex() == 2) {
            QDateTime endrpeattime = beginDateTime;
            endrpeattime.setDate(m_endRepeatDate->date());
            if (beginDateTime >= endrpeattime) {
                QMessageBox::warning(this, tr("error"), tr("The end repeat time less than begin time!"));
                return;
            }
            scheduleDtailInfo.enddata.date = endrpeattime;
        }
    }
    scheduleDtailInfo.titleName = m_textEdit->toPlainText();
    CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->GetType(m_typeComBox->currentIndex() + 1, scheduleDtailInfo.type);
    scheduleDtailInfo.beginDateTime = beginDateTime;
    scheduleDtailInfo.endDateTime = endDateTime;

    if (m_type == 1) {
        CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->addSchedule(scheduleDtailInfo);
    } else if (m_type == 0) {

        if (m_scheduleDtailInfo.rpeat == 0 && m_scheduleDtailInfo.rpeat == scheduleDtailInfo.rpeat) {
            CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(scheduleDtailInfo);

        } else {
            if (m_scheduleDtailInfo.allday != scheduleDtailInfo.allday) {

                DMessageBox msgBox;
                msgBox.setWindowFlags(Qt::FramelessWindowHint);
                msgBox.setIconPixmap(DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-logo.svg").scaled(QSize(34, 34) * devicePixelRatioF()));
                if (scheduleDtailInfo.allday) {
                    msgBox.setText(tr("Repetitive schedule of all must have the same throughout the state."));

                } else {
                    msgBox.setText(tr("Repetitive schedule of all must have the same not throughout the state."));
                }
                msgBox.setInformativeText(tr("Would you like to change all repeat?"));
                DPushButton *noButton = msgBox.addButton(tr("Cancel"), DMessageBox::NoRole);
                DPushButton *yesButton = msgBox.addButton(tr("All Changes"), DMessageBox::YesRole);
                DPalette pa = yesButton->palette();
                pa.setColor(DPalette::ButtonText, Qt::red);
                yesButton->setPalette(pa);
                msgBox.exec();

                if (msgBox.clickedButton() == noButton) {
                    return;
                } else if (msgBox.clickedButton() == yesButton) {
                    CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(scheduleDtailInfo);
                }

            } else if (m_scheduleDtailInfo.rpeat != scheduleDtailInfo.rpeat) {
                DMessageBox msgBox;
                msgBox.setWindowFlags(Qt::FramelessWindowHint);
                msgBox.setIconPixmap(DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-logo.svg").scaled(QSize(34, 34) * devicePixelRatioF()));
                msgBox.setText(tr("Repetitive schedule of all must have the same throughout the state."));
                msgBox.setInformativeText(tr("Would you like to change all repeat?"));
                DPushButton *noButton = msgBox.addButton(tr("Cancel"), DMessageBox::NoRole);
                DPushButton *yesButton = msgBox.addButton(tr("All Changes"), DMessageBox::YesRole);
                DPalette pa = yesButton->palette();
                pa.setColor(DPalette::ButtonText, Qt::red);
                yesButton->setPalette(pa);
                msgBox.exec();

                if (msgBox.clickedButton() == noButton) {
                    return;
                } else if (msgBox.clickedButton() == yesButton) {
                    CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(scheduleDtailInfo);
                }
            } else {
                if (m_scheduleDtailInfo.RecurID == 0) {
                    DMessageBox msgBox;
                    msgBox.setWindowFlags(Qt::FramelessWindowHint);
                    msgBox.setIconPixmap(DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-logo.svg").scaled(QSize(34, 34) * devicePixelRatioF()));
                    msgBox.setText(tr("You are changing the schedule again."));
                    msgBox.setInformativeText(tr("You are to change the schedule only the repeat, to repeat or change it all?"));
                    DPushButton *noButton = msgBox.addButton(tr("Cancel"), DMessageBox::NoRole);
                    DPushButton *yesallbutton = msgBox.addButton(tr("ALL"), DMessageBox::YesRole);
                    DPushButton *yesButton = msgBox.addButton(tr("Only Schedule"), DMessageBox::YesRole);
                    DPalette pa = yesButton->palette();
                    pa.setColor(DPalette::ButtonText, Qt::white);
                    pa.setColor(DPalette::Dark, QColor("#0098FF"));
                    pa.setColor(DPalette::Light, QColor("#0098FF"));
                    yesButton->setPalette(pa);
                    msgBox.exec();

                    if (msgBox.clickedButton() == noButton) {
                        return;
                    } else if (msgBox.clickedButton() == yesallbutton) {
                        CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(scheduleDtailInfo);
                    } else if (msgBox.clickedButton() == yesButton) {
                        ScheduleDtailInfo newschedule = scheduleDtailInfo;
                        newschedule.rpeat = 0;
                        newschedule.RecurID = 0;
                        newschedule.id = 0;
                        CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->addSchedule(newschedule);
                        ScheduleDtailInfo updatescheduleData;
                        CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(m_scheduleDtailInfo.id, updatescheduleData);
                        updatescheduleData.ignore.append(m_scheduleDtailInfo.beginDateTime);

                        //m_scheduleDtailInfo.ignore.append(m_scheduleDtailInfo.beginDateTime);
                        CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(updatescheduleData);
                    }
                } else {
                    DMessageBox msgBox;
                    msgBox.setWindowFlags(Qt::FramelessWindowHint);
                    msgBox.setIconPixmap(DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-logo.svg").scaled(QSize(34, 34) * devicePixelRatioF()));

                    msgBox.setText(tr("You are changing the schedule again."));
                    msgBox.setInformativeText(tr("Do you want to change the schedule only is this a repeat, besides it also changes the schedule all repeat in the future?"));
                    DPushButton *noButton = msgBox.addButton(tr("Cancel"), DMessageBox::NoRole);
                    DPushButton *yesallbutton = msgBox.addButton(tr("All future Schedule"), DMessageBox::YesRole);
                    DPushButton *yesButton = msgBox.addButton(tr("Only Schedule"), DMessageBox::YesRole);
                    DPalette pa = yesButton->palette();
                    pa.setColor(DPalette::ButtonText, Qt::white);
                    pa.setColor(DPalette::Dark, QColor("#0098FF"));
                    pa.setColor(DPalette::Light, QColor("#0098FF"));
                    yesButton->setPalette(pa);
                    msgBox.exec();

                    if (msgBox.clickedButton() == noButton) {
                        return;
                    } else if (msgBox.clickedButton() == yesallbutton) {
                        ScheduleDtailInfo newschedule = scheduleDtailInfo;
                        newschedule.RecurID = 0;
                        newschedule.id = 0;
                        CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->addSchedule(newschedule);
                        ScheduleDtailInfo updatescheduleData;
                        CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(m_scheduleDtailInfo.id, updatescheduleData);
                        updatescheduleData.enddata.type = 2;
                        updatescheduleData.enddata.date = m_scheduleDtailInfo.beginDateTime;
                        CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(updatescheduleData);
                    } else if (msgBox.clickedButton() == yesButton) {
                        ScheduleDtailInfo newschedule = scheduleDtailInfo;
                        newschedule.rpeat = 0;
                        newschedule.RecurID = 0;
                        newschedule.id = 0;
                        CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->addSchedule(newschedule);
                        ScheduleDtailInfo updatescheduleData;
                        CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(m_scheduleDtailInfo.id, updatescheduleData);
                        updatescheduleData.ignore.append(m_scheduleDtailInfo.beginDateTime);
                        CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(updatescheduleData);
                    }
                }

            }
        }
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

void CSchceduleDlg::slotallDayStateChanged(int state)
{
    m_rmindCombox->clear();
    if (!state) {
        m_rmindCombox->addItem(tr("None"));
        m_rmindCombox->addItem(tr("Schedule occurs"));
        m_rmindCombox->addItem(tr("15 minutes ago"));
        m_rmindCombox->addItem(tr("30 minutes ago"));
        m_rmindCombox->addItem(tr("1 hour ago"));
        m_rmindCombox->addItem(tr("1 day ago"));
        m_rmindCombox->addItem(tr("2 day ago"));
        m_beginTimeEdit->setVisible(true);
        m_endTimeEdit->setVisible(true);
        if (m_type == 0) {
            m_beginDateEdit->setDate(m_scheduleDtailInfo.beginDateTime.date());
            m_beginTimeEdit->setTime(m_scheduleDtailInfo.beginDateTime.time());
            m_endDateEdit->setDate(m_scheduleDtailInfo.endDateTime.date());
            m_endTimeEdit->setTime(m_scheduleDtailInfo.endDateTime.time());
        } else {

            m_beginDateEdit->setDate(m_currentDate.date());
            m_beginTimeEdit->setTime(m_currentDate.time());
            m_endDateEdit->setDate(m_currentDate.date());
            m_endTimeEdit->setTime(m_currentDate.time().addSecs(3600));
        }
    } else {
        m_rmindCombox->addItem(tr("None"));
        m_rmindCombox->addItem(tr(" schedule of the day(At 9 am)"));
        m_rmindCombox->addItem(tr("1 day ago(At 9 am)"));
        m_rmindCombox->addItem(tr("2 day ago(At 9 am)"));
        m_rmindCombox->addItem(tr("A week ago"));
        m_beginTimeEdit->setVisible(false);
        m_endTimeEdit->setVisible(false);
        if (m_type == 0) {
            m_beginDateEdit->setDate(m_scheduleDtailInfo.beginDateTime.date());
            m_beginTimeEdit->setTime(QTime(0, 0));
            m_endDateEdit->setDate(m_scheduleDtailInfo.endDateTime.date());
            m_endTimeEdit->setTime(QTime(23, 59));
        } else {
            m_beginDateEdit->setDate(m_currentDate.date());
            m_endDateEdit->setDate(m_currentDate.date());
            m_beginTimeEdit->setTime(QTime(0, 0));
            m_endTimeEdit->setTime(QTime(23, 59));
        }
    }
}

void CSchceduleDlg::slotbRpeatactivated(int index)
{
    if (index > 0)
        m_endrepeatWidget->setVisible(true);
    else {
        m_endrepeatWidget->setVisible(false);
    }
}

void CSchceduleDlg::sloteRpeatactivated(int index)
{
    if (index == 0) {
        m_endrepeattimesWidget->setVisible(false);
        m_endRepeatDate->setVisible(false);
    } else if (index == 1) {
        m_endrepeattimesWidget->setVisible(true);
        m_endRepeatDate->setVisible(false);
    } else {
        m_endrepeattimesWidget->setVisible(false);
        m_endRepeatDate->setVisible(true);
    }
}

void CSchceduleDlg::initUI()
{
    QFont mlabelF;
    mlabelF.setFamily("SourceHanSansSC-Medium");
    mlabelF.setPixelSize(14);
    DPalette pa;
    pa.setColor(DPalette::WindowText, QColor("#414D68"));

    QVBoxLayout *maintlayout = new QVBoxLayout;
    QHBoxLayout *typelayout  = new QHBoxLayout;
    typelayout->setSpacing(0);
    typelayout->setMargin(0);
    maintlayout->setContentsMargins(20, 10, 20, 0);
    m_typeLabel = new DLabel(tr("Type:"));
    m_typeLabel->setFont(mlabelF);
    m_typeLabel->setPalette(pa);
    m_typeLabel->setAlignment(Qt::AlignLeft);
    m_typeLabel->setFixedWidth(78);
    m_typeComBox = new DComboBox ();
    m_typeComBox->insertItem(0, QIcon(DHiDPIHelper::loadNxPixmap(":/resources/icon/icon_type_work.svg").scaled(QSize(20, 20) * devicePixelRatioF())), tr("Work"));
    m_typeComBox->insertItem(1, QIcon(DHiDPIHelper::loadNxPixmap(":/resources/icon/icon_type_life.svg").scaled(QSize(20, 20) * devicePixelRatioF())), tr("Life"));
    m_typeComBox->insertItem(2, QIcon(DHiDPIHelper::loadNxPixmap(":/resources/icon/icon_type_other.svg").scaled(QSize(20, 20) * devicePixelRatioF())), tr("Other"));
    typelayout->addWidget(m_typeLabel);
    typelayout->addWidget(m_typeComBox);
    maintlayout->addLayout(typelayout);

    QHBoxLayout *contentLabellayout  = new QHBoxLayout;
    contentLabellayout->setSpacing(0);
    contentLabellayout->setMargin(0);
    QVBoxLayout *conttelabellayout = new QVBoxLayout;
    conttelabellayout->setSpacing(0);
    conttelabellayout->setMargin(0);
    m_contentLabel = new DLabel(tr("Content:"));
    m_contentLabel->setFont(mlabelF);
    m_contentLabel->setPalette(pa);
    m_contentLabel->setAlignment(Qt::AlignLeft);
    m_contentLabel->setFixedWidth(78);
    conttelabellayout->addWidget(m_contentLabel);
    conttelabellayout->addStretch();
    m_textEdit = new DTextEdit(this);
    m_textEdit->setTextBackgroundColor(Qt::white);
    DPalette tpa;
    tpa.setColor(DPalette::Background, Qt::white);
    m_textEdit->setPalette(tpa);
    contentLabellayout->addLayout(conttelabellayout);
    contentLabellayout->addWidget(m_textEdit);
    maintlayout->addLayout(contentLabellayout);

    QHBoxLayout *alldayLabellayout  = new QHBoxLayout;
    alldayLabellayout->setSpacing(0);
    alldayLabellayout->setMargin(0);
    m_adllDayLabel = new DLabel(tr("All Day:"));
    m_adllDayLabel->setFont(mlabelF);
    m_adllDayLabel->setPalette(pa);
    m_adllDayLabel->setAlignment(Qt::AlignLeft);
    m_adllDayLabel->setFixedWidth(78);
    m_allDayCheckbox = new DCheckBox(this);
    alldayLabellayout->addWidget(m_adllDayLabel);
    alldayLabellayout->addWidget(m_allDayCheckbox);
    maintlayout->addLayout(alldayLabellayout);

    QHBoxLayout *beginLabellayout  = new QHBoxLayout;
    beginLabellayout->setSpacing(0);
    beginLabellayout->setMargin(0);
    m_beginTimeLabel = new DLabel (tr("Begin Time:"));
    m_beginTimeLabel->setFont(mlabelF);
    m_beginTimeLabel->setPalette(pa);
    m_beginTimeLabel->setAlignment(Qt::AlignLeft);
    m_beginTimeLabel->setFixedWidth(78);
    m_beginDateEdit = new QDateEdit(this);
    m_beginDateEdit->setFixedHeight(36);
    m_beginTimeEdit = new CTimeEdit(this);
    m_beginTimeEdit->setFixedHeight(36);
    m_beginDateEdit->setCalendarPopup(true);
    m_beginDateEdit->setDisplayFormat("yyyy-MM-dd");
    //QHBoxLayout *begintimeelayout  = new QHBoxLayout;
    //begintimeelayout->addWidget(m_beginDateEdit, 1);
    //begintimeelayout->addWidget(m_beginTimeEdit, 1);
    //begintimeelayout->setMargin(0);
    //begintimeelayout->setSpacing(0);
    //begintimeelayout->addStretch();
    beginLabellayout->addWidget(m_beginTimeLabel, 1);
    beginLabellayout->addWidget(m_beginDateEdit, 2);
    beginLabellayout->addWidget(m_beginTimeEdit, 2);
    // beginLabellayout->addLayout(begintimeelayout);
    maintlayout->addLayout(beginLabellayout);


    QHBoxLayout *endLabellayout  = new QHBoxLayout;
    endLabellayout->setSpacing(0);
    endLabellayout->setMargin(0);
    m_endTimeLabel = new DLabel (tr("End Time:"));
    m_endTimeLabel->setFont(mlabelF);
    m_endTimeLabel->setPalette(pa);
    m_endTimeLabel->setAlignment(Qt::AlignLeft);
    m_endTimeLabel->setFixedWidth(78);
    m_endDateEdit = new QDateEdit(this);
    m_endDateEdit->setFixedHeight(36);
    m_endTimeEdit = new CTimeEdit(this);
    m_endTimeEdit->setFixedHeight(36);
    m_endDateEdit->setCalendarPopup(true);
    m_endDateEdit->setDisplayFormat("yyyy-MM-dd");
    QHBoxLayout *endtimeelayout  = new QHBoxLayout;
    endtimeelayout->setMargin(0);
    endtimeelayout->setSpacing(0);
    endtimeelayout->addWidget(m_endDateEdit, 1);
    endtimeelayout->addWidget(m_endTimeEdit, 1);
    endtimeelayout->addStretch();
    endLabellayout->addWidget(m_endTimeLabel);
    endLabellayout->addLayout(endtimeelayout);
    maintlayout->addLayout(endLabellayout);

    QHBoxLayout *rmindLabellayout  = new QHBoxLayout;
    rmindLabellayout->setSpacing(0);
    rmindLabellayout->setMargin(0);
    m_remindSetLabel = new DLabel (tr("Remind Set:"));
    m_remindSetLabel->setFont(mlabelF);
    m_remindSetLabel->setPalette(pa);
    m_remindSetLabel->setAlignment(Qt::AlignLeft);
    m_remindSetLabel->setFixedWidth(78);
    m_rmindCombox = new DComboBox();
    rmindLabellayout->addWidget(m_remindSetLabel);
    rmindLabellayout->addWidget(m_rmindCombox);
    rmindLabellayout->addStretch();
    maintlayout->addLayout(rmindLabellayout);

    QHBoxLayout *repeatLabellayout  = new QHBoxLayout;
    repeatLabellayout->setSpacing(0);
    repeatLabellayout->setMargin(0);
    m_beginrepeatLabel = new DLabel (tr("Repeat:"));
    m_beginrepeatLabel->setFont(mlabelF);
    m_beginrepeatLabel->setPalette(pa);
    m_beginrepeatLabel->setAlignment(Qt::AlignLeft);
    m_beginrepeatLabel->setFixedWidth(78);
    m_beginrepeatCombox = new DComboBox();
    m_beginrepeatCombox->addItem(tr("None"));
    m_beginrepeatCombox->addItem(tr("Every day"));
    m_beginrepeatCombox->addItem(tr("Every working day"));
    m_beginrepeatCombox->addItem(tr("Once a week"));
    m_beginrepeatCombox->addItem(tr("A month"));
    m_beginrepeatCombox->addItem(tr("Every year"));
    repeatLabellayout->addWidget(m_beginrepeatLabel);
    repeatLabellayout->addWidget(m_beginrepeatCombox);
    repeatLabellayout->addStretch();
    maintlayout->addLayout(repeatLabellayout);

    QHBoxLayout *endrepeatLabellayout  = new QHBoxLayout;
    endrepeatLabellayout->setSpacing(0);
    endrepeatLabellayout->setMargin(0);
    m_endrepeatLabel = new DLabel (tr("End Repeat:"));
    m_endrepeatLabel->setFont(mlabelF);
    m_endrepeatLabel->setPalette(pa);
    m_endrepeatLabel->setAlignment(Qt::AlignLeft);
    m_endrepeatLabel->setFixedWidth(78);
    m_endrepeatCombox = new DComboBox();
    m_endrepeatCombox->addItem(tr("never"));
    m_endrepeatCombox->addItem(tr("in"));
    m_endrepeatCombox->addItem(tr("On the date"));
    endrepeatLabellayout->addWidget(m_endrepeatLabel);
    endrepeatLabellayout->addWidget(m_endrepeatCombox);

    QHBoxLayout *endrepeattimeslayout  = new QHBoxLayout;
    m_endrepeattimes = new DLineEdit;
    m_endrepeattimes->setFixedHeight(36);
    m_endrepeattimes->setText(QString::number(10));
    m_endrepeattimes->setClearButtonEnabled(false);
    QValidator *validator = new QIntValidator(1, 9999, this);
    m_endrepeattimes->setValidator(validator);
    m_endrepeattimesLabel = new DLabel(tr("After time"));
    m_endrepeattimesLabel->setFont(mlabelF);
    m_endrepeattimesLabel->setPalette(pa);
    endrepeattimeslayout->addWidget(m_endrepeattimes);
    endrepeattimeslayout->addWidget(m_endrepeattimesLabel);
    m_endrepeattimesWidget = new DWidget;
    m_endrepeattimesWidget->setLayout(endrepeattimeslayout);
    m_endrepeattimesWidget->setVisible(false);
    endrepeatLabellayout->addWidget(m_endrepeattimesWidget);

    m_endRepeatDate = new DDateEdit;
    m_endRepeatDate->setCalendarPopup(true);
    m_endRepeatDate->setFixedHeight(36);
    m_endRepeatDate->setDate(QDate::currentDate());
    endrepeatLabellayout->addWidget(m_endRepeatDate);
    endrepeatLabellayout->addStretch();
    m_endRepeatDate->setVisible(false);
    m_endrepeatWidget = new DWidget;
    m_endrepeatWidget->setLayout(endrepeatLabellayout);
    maintlayout->addWidget(m_endrepeatWidget);
    m_endrepeatWidget->setVisible(false);
    QHBoxLayout *downlayout  = new QHBoxLayout;
    m_cancelBt = new DPushButton(tr("Cancel"));
    m_OkBt = new DPushButton(tr("OK"));
    DPalette okpa = m_OkBt->palette();
    okpa.setColor(DPalette::ButtonText, Qt::white);
    okpa.setColor(DPalette::Dark, QColor("#0098FF"));
    okpa.setColor(DPalette::Light, QColor("#0098FF"));
    m_OkBt->setPalette(okpa);
    downlayout->addWidget(m_cancelBt);
    downlayout->addWidget(m_OkBt);
    QWidget *gwi = new QWidget;
    maintlayout->addStretch();
    maintlayout->addLayout(downlayout);
    gwi->setLayout(maintlayout);
    addContent(gwi);
    initDateEdit();
    if (m_type == 1)
        slotallDayStateChanged(0);
}

void CSchceduleDlg::initConnection()
{
    connect(m_cancelBt, &DPushButton::clicked, this, &CSchceduleDlg::slotCancelBt);
    connect(m_OkBt, &DPushButton::clicked, this, &CSchceduleDlg::slotOkBt);
    connect(m_textEdit, &DTextEdit::textChanged, this, &CSchceduleDlg::slotTextChange);

    connect(m_allDayCheckbox, &DCheckBox::stateChanged, this, &CSchceduleDlg::slotallDayStateChanged);
    connect(m_beginrepeatCombox, QOverload<int>::of(&QComboBox::activated), this, &CSchceduleDlg::slotbRpeatactivated);
    connect(m_endrepeatCombox, QOverload<int>::of(&QComboBox::activated), this, &CSchceduleDlg::sloteRpeatactivated);
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

void CSchceduleDlg::initRmindRpeatUI()
{
    if (m_scheduleDtailInfo.allday) {
        if (m_scheduleDtailInfo.remind) {
            if (m_scheduleDtailInfo.remindData.n == 0) {
                m_rmindCombox->setCurrentIndex(1);
            } else if (m_scheduleDtailInfo.remindData.n == 1) {
                m_rmindCombox->setCurrentIndex(2);
            } else if (m_scheduleDtailInfo.remindData.n == 2) {
                m_rmindCombox->setCurrentIndex(3);
            } else if (m_scheduleDtailInfo.remindData.n == 7) {
                m_rmindCombox->setCurrentIndex(4);
            }
        } else {
            m_rmindCombox->setCurrentIndex(0);
        }

    } else {
        if (m_scheduleDtailInfo.remind) {
            if (m_scheduleDtailInfo.remindData.n == 0) {
                m_rmindCombox->setCurrentIndex(1);
            } else if (m_scheduleDtailInfo.remindData.n == 15) {
                m_rmindCombox->setCurrentIndex(2);
            } else if (m_scheduleDtailInfo.remindData.n == 30) {
                m_rmindCombox->setCurrentIndex(3);
            } else if (m_scheduleDtailInfo.remindData.n == 60) {
                m_rmindCombox->setCurrentIndex(4);
            } else if (m_scheduleDtailInfo.remindData.n == 1440) {
                m_rmindCombox->setCurrentIndex(5);
            } else if (m_scheduleDtailInfo.remindData.n == 2880) {
                m_rmindCombox->setCurrentIndex(6);
            }
        } else {
            m_rmindCombox->setCurrentIndex(0);
        }
    }
    m_beginrepeatCombox->setCurrentIndex(m_scheduleDtailInfo.rpeat);
    if (m_scheduleDtailInfo.rpeat != 0) {
        if (m_scheduleDtailInfo.enddata.type == 0) {
            m_endrepeatCombox->setCurrentIndex(0);
        } else if (m_scheduleDtailInfo.enddata.type == 1) {
            m_endrepeatCombox->setCurrentIndex(1);
            m_endrepeattimes->setText(QString::number(m_scheduleDtailInfo.enddata.tcount));
        } else if (m_scheduleDtailInfo.enddata.type == 2) {
            m_endrepeatCombox->setCurrentIndex(2);
            m_endRepeatDate->setDate(m_scheduleDtailInfo.enddata.date.date());
        }
        m_endrepeatWidget->show();
        sloteRpeatactivated(m_scheduleDtailInfo.enddata.type);
    } else {
        m_endrepeatWidget->hide();
    }
}

