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
#include "schcedulectrldlg.h"
#include "timeeditctrl.h"
DGUI_USE_NAMESPACE
CSchceduleDlg::CSchceduleDlg(int type, QWidget *parent): DDialog(parent)
{
    m_type = type;
    initUI();
    initConnection();
    if (type == 1) {
        m_titleLabel->setText(tr("Create Schcedule"));
        m_beginDateEdit->setDate(QDate::currentDate());
        m_beginTimeEdit->setTime(QTime::currentTime());
        m_endDateEdit->setDate(QDate::currentDate());
        m_endTimeEdit->setTime(QTime::currentTime().addSecs(3600));
    } else {
        m_titleLabel->setText(tr("Edit Schcedule"));
    }
    setFocusPolicy(Qt::WheelFocus);
    setFixedSize(438, 480);
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
    QDateTime datetime = date.addSecs(3600);
    m_endDateEdit->setDate(datetime.date());
    m_endTimeEdit->setTime(datetime.time());
}

ScheduleInfo CSchceduleDlg::getData()
{
    return m_scheduleInfo;
}

ScheduleDtailInfo CSchceduleDlg::getScheduleData()
{
    return m_scheduleDtailInfo;
}

void CSchceduleDlg::setAllDay(bool flag)
{
    m_allDayCheckbox->setChecked(flag);
}

void CSchceduleDlg::slotCancelBt()
{
    reject();
}

void CSchceduleDlg::slotOkBt()
{
    int themetype = CScheduleDataManage::getScheduleDataManage()->getTheme();

    ScheduleDtailInfo scheduleDtailInfo = m_scheduleDtailInfo;
    QDateTime beginDateTime, endDateTime;
    beginDateTime.setDate(m_beginDateEdit->date());
    beginDateTime.setTime(m_beginTimeEdit->getTime());
    endDateTime.setDate(m_endDateEdit->date());
    endDateTime.setTime(m_endTimeEdit->getTime());
    if (m_type == 1) {
        if (m_textEdit->toPlainText().isEmpty()) {
            scheduleDtailInfo.titleName = m_textEdit->placeholderText();
        } else {
            scheduleDtailInfo.titleName = m_textEdit->toPlainText();
        }
    } else {
        scheduleDtailInfo.titleName = m_textEdit->toPlainText();
    }
    if (scheduleDtailInfo.titleName.isEmpty()) {
        QMessageBox::warning(this, tr("error"), tr("Schcedule is empty!"));
        return;
    }
    if (beginDateTime >= endDateTime) {
        QMessageBox::warning(this, tr("error"), tr("The end time less than begin time!"));
        return;
    }
    if (m_type == 1) scheduleDtailInfo.id = 0;
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
            } else if (m_rmindCombox->currentIndex() == 7) {
                scheduleDtailInfo.remindData.n = 10080;
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

                CSchceduleCtrlDlg msgBox(this);
                //msgBox.setWindowFlags(Qt::FramelessWindowHint);
                //msgBox.setIconPixmap(DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-logo.svg").scaled(QSize(34, 34) * devicePixelRatioF()));
                if (scheduleDtailInfo.allday) {
                    msgBox.setText(tr("Repetitive schedule of all must have the same throughout the state."));

                } else {
                    msgBox.setText(tr("Repetitive schedule of all must have the same not throughout the state."));
                }
                msgBox.setInformativeText(tr("Would you like to change all repeat?"));
                DPushButton *noButton = msgBox.addPushButton(tr("Cancel"));
                DPushButton *yesButton = msgBox.addPushButton(tr("All Changes"));
                msgBox.updatesize();
                DPalette pa = yesButton->palette();
                if (themetype == 0 || themetype == 1) {
                    pa.setColor(DPalette::ButtonText, Qt::red);

                } else {
                    pa.setColor(DPalette::ButtonText, "#FF5736");

                }
                yesButton->setPalette(pa);
                msgBox.exec();

                if (msgBox.clickButton() == noButton) {
                    return;
                } else if (msgBox.clickButton() == yesButton) {
                    ScheduleDtailInfo updatescheduleData;
                    CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(scheduleDtailInfo.id, updatescheduleData);
                    scheduleDtailInfo.beginDateTime = updatescheduleData.beginDateTime;
                    scheduleDtailInfo.endDateTime = updatescheduleData.endDateTime;
                    scheduleDtailInfo.RecurID = updatescheduleData.RecurID;
                    CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(scheduleDtailInfo);
                }

            } else if (m_scheduleDtailInfo.rpeat != scheduleDtailInfo.rpeat) {
                CSchceduleCtrlDlg msgBox(this);
                //msgBox.setWindowFlags(Qt::FramelessWindowHint);
                //msgBox.setIconPixmap(DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-logo.svg").scaled(QSize(34, 34) * devicePixelRatioF()));
                msgBox.setText(tr("Are you sure you want to change the schedule of repeat rules?"));
                msgBox.setInformativeText(tr("Would you like to change all repeat?"));
                DPushButton *noButton = msgBox.addPushButton(tr("Cancel"));
                DPushButton *yesButton = msgBox.addPushButton(tr("All Changes"));
                msgBox.updatesize();
                DPalette pa = yesButton->palette();
                if (themetype == 0 || themetype == 1) {
                    pa.setColor(DPalette::ButtonText, Qt::red);

                } else {
                    pa.setColor(DPalette::ButtonText, "#FF5736");

                }
                yesButton->setPalette(pa);
                msgBox.exec();

                if (msgBox.clickButton() == noButton) {
                    return;
                } else if (msgBox.clickButton() == yesButton) {
                    CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(scheduleDtailInfo);
                }
            } else {
                if (m_scheduleDtailInfo.RecurID == 0) {
                    CSchceduleCtrlDlg msgBox(this);
                    //msgBox.setWindowFlags(Qt::FramelessWindowHint);
                    //msgBox.setIconPixmap(DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-logo.svg").scaled(QSize(34, 34) * devicePixelRatioF()));
                    msgBox.setText(tr("You are changing the schedule again"));
                    msgBox.setInformativeText(tr("You are to change the schedule only the repeat, to repeat or change it all?"));
                    DPushButton *noButton = msgBox.addPushButton(tr("Cancel"));
                    DPushButton *yesallbutton = msgBox.addPushButton(tr("ALL"));
                    DPushButton *yesButton = msgBox.addPushButton(tr("Only Schedule"));
                    msgBox.updatesize();
                    DPalette pa = yesButton->palette();
                    if (themetype == 0 || themetype == 1) {
                        pa.setColor(DPalette::ButtonText, Qt::white);
                        pa.setColor(DPalette::Dark, QColor("#25B7FF"));
                        pa.setColor(DPalette::Light, QColor("#0098FF"));
                    } else {
                        pa.setColor(DPalette::ButtonText, "#B8D3FF");
                        pa.setColor(DPalette::Dark, QColor("#0056C1"));
                        pa.setColor(DPalette::Light, QColor("#004C9C"));
                    }
                    yesButton->setPalette(pa);
                    msgBox.exec();

                    if (msgBox.clickButton() == noButton) {
                        return;
                    } else if (msgBox.clickButton() == yesallbutton) {
                        CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(scheduleDtailInfo);
                    } else if (msgBox.clickButton() == yesButton) {
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
                    CSchceduleCtrlDlg msgBox(this);
                    //msgBox.setWindowFlags(Qt::FramelessWindowHint);
                    // msgBox.setIconPixmap(DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-logo.svg").scaled(QSize(34, 34) * devicePixelRatioF()));

                    msgBox.setText(tr("You are changing the schedule again"));
                    msgBox.setInformativeText(tr("Do you want to change the schedule only is this a repeat, besides it also changes the schedule all repeat in the future?"));
                    DPushButton *noButton = msgBox.addPushButton(tr("Cancel"));
                    DPushButton *yesallbutton = msgBox.addPushButton(tr("All future Schedule"));
                    DPushButton *yesButton = msgBox.addPushButton(tr("Only Schedule"));
                    msgBox.updatesize();
                    DPalette pa = yesButton->palette();
                    if (themetype == 0 || themetype == 1) {
                        pa.setColor(DPalette::ButtonText, Qt::white);
                        pa.setColor(DPalette::Dark, QColor("#25B7FF"));
                        pa.setColor(DPalette::Light, QColor("#0098FF"));
                    } else {
                        pa.setColor(DPalette::ButtonText, "#B8D3FF");
                        pa.setColor(DPalette::Dark, QColor("#0056C1"));
                        pa.setColor(DPalette::Light, QColor("#004C9C"));
                    }
                    yesButton->setPalette(pa);
                    msgBox.exec();

                    if (msgBox.clickButton() == noButton) {
                        return;
                    } else if (msgBox.clickButton() == yesallbutton) {
                        ScheduleDtailInfo newschedule = scheduleDtailInfo;
                        newschedule.RecurID = 0;
                        newschedule.id = 0;
                        CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->addSchedule(newschedule);
                        ScheduleDtailInfo updatescheduleData;
                        CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(m_scheduleDtailInfo.id, updatescheduleData);
                        updatescheduleData.enddata.type = 2;
                        updatescheduleData.enddata.date = m_scheduleDtailInfo.beginDateTime;
                        CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(updatescheduleData);
                    } else if (msgBox.clickButton() == yesButton) {
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
        m_rmindCombox->addItem(tr("A week ago"));
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
        m_rmindCombox->addItem(tr("schedule day(At 9 am)"));
        m_rmindCombox->addItem(tr("1 day ago(At 9 am)"));
        m_rmindCombox->addItem(tr("2 day ago(At 9 am)"));
        m_rmindCombox->addItem(tr("A week ago(At 9 am)"));
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
            m_rmindCombox->setCurrentIndex(2);
        }
    }

    //m_rmindCombox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

void CSchceduleDlg::slotbRpeatactivated(int index)
{
    if (index > 0) {
        m_endrepeatWidget->setVisible(true);
        setFixedSize(438, 526);
        m_gwi->setGeometry(0, 68, 438, 458);
    } else {
        m_endrepeatWidget->setVisible(false);
        setFixedSize(438, 480);
        m_gwi->setGeometry(0, 68, 438, 412);
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

bool CSchceduleDlg::eventFilter(QObject *obj, QEvent *pEvent)
{
    if (obj == m_textEdit) {
        if (pEvent->type() == QEvent::FocusIn) {
            //清空编辑框默认占位符
            m_textEdit->setPlaceholderText("");
        } else if (pEvent->type() == QEvent::FocusOut) {
            //设置编辑框默认占位符
            m_textEdit->setPlaceholderText(tr("New Schedule"));
        }
    }
    return QDialog::eventFilter(obj, pEvent);
}

void CSchceduleDlg::initUI()
{
    m_titleLabel = new DLabel(this);
    QFont titlelabelF;
    titlelabelF.setFamily("SourceHanSansSC-Medium");
    titlelabelF.setPixelSize(14);
    QColor btitleColor = "#000000";
    btitleColor.setAlphaF(0.01);
    DPalette titlepa = m_titleLabel->palette();
    int themetype = CScheduleDataManage::getScheduleDataManage()->getTheme();
    if (themetype == 0 || themetype == 1) {
        titlepa.setColor(DPalette::WindowText, QColor("#001A2E"));
        titlepa.setColor(DPalette::Window, btitleColor);
    } else {
        titlepa.setColor(DPalette::WindowText, QColor("#C0C6D4"));
        titlepa.setColor(DPalette::Window, btitleColor);
    }
    m_titleLabel->setPalette(titlepa);
    m_titleLabel->setFixedSize(148, 51);
    m_titleLabel->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    //m_titleLabel->setGeometry(165, 0, 108, 51);
    m_titleLabel->move(145, 0);


    setSpacing(0);
    QFont mlabelF;
    mlabelF.setFamily("SourceHanSansSC-Medium");
    mlabelF.setPixelSize(14);
    DPalette pa =  m_titleLabel->palette();
    if (themetype == 0 || themetype == 1) {
        pa.setColor(DPalette::WindowText, QColor("#414D68"));
    } else {
        pa.setColor(DPalette::WindowText, QColor("#C0C6D4"));
    }
    // pa.setColor(DPalette::WindowText, QColor("#414D68"));

    QVBoxLayout *maintlayout = new QVBoxLayout;
    QHBoxLayout *typelayout  = new QHBoxLayout;
    typelayout->setSpacing(0);
    typelayout->setMargin(0);
    maintlayout->setContentsMargins(20, 0, 20, 10);
    m_typeLabel = new DLabel(tr("Type:"));
    m_typeLabel->setFont(mlabelF);
    m_typeLabel->setPalette(pa);
    m_typeLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_typeLabel->setFixedSize(78, 36);
    m_typeComBox = new DComboBox ();
    m_typeComBox->setFixedSize(319, 36);
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
    m_contentLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_contentLabel->setFixedWidth(78);
    conttelabellayout->addWidget(m_contentLabel);
    conttelabellayout->addStretch();
    m_textEdit = new DTextEdit(this);
    m_textEdit->setFixedSize(319, 86);
    m_textEdit->setTextBackgroundColor(Qt::white);
    DPalette tpa = m_textEdit->palette();
    if (themetype == 0 || themetype == 1) {
        QColor conttelc("#000000");
        conttelc.setAlphaF(0.08);
        tpa.setColor(DPalette::Background, conttelc);
    } else {
        QColor conttelc("#FFFFFF");
        conttelc.setAlphaF(0.15);
        tpa.setColor(DPalette::Background, conttelc);
    }

    m_textEdit->setPalette(tpa);
    if (m_type == 1) {
        m_textEdit->setPlaceholderText(tr("New Schedule"));
        //设置关联控件，用于QTextEdit控件捕获MouseButtonPress等事件
        QWidget *mpContentWidget = m_textEdit->viewport();
        //设置事件过滤器
        m_textEdit->installEventFilter(this);
        mpContentWidget->installEventFilter(this);
    }

    contentLabellayout->addLayout(conttelabellayout);
    contentLabellayout->addWidget(m_textEdit);
    maintlayout->addLayout(contentLabellayout);

    QHBoxLayout *alldayLabellayout  = new QHBoxLayout;
    alldayLabellayout->setSpacing(0);
    alldayLabellayout->setMargin(0);
    m_adllDayLabel = new DLabel(tr("All Day:"));
    m_adllDayLabel->setFont(mlabelF);
    m_adllDayLabel->setPalette(pa);
    m_adllDayLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
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
    m_beginTimeLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_beginTimeLabel->setFixedSize(78, 36);
    m_beginDateEdit = new QDateEdit(this);
    m_beginDateEdit->setFixedSize(200, 36);

    m_beginTimeEdit = new CTimeEdit(this);
    m_beginTimeEdit->setFixedSize(109, 36);
    m_beginDateEdit->setCalendarPopup(true);
    m_beginDateEdit->setDisplayFormat("yyyy-MM-dd");
    //QHBoxLayout *begintimeelayout  = new QHBoxLayout;
    //begintimeelayout->addWidget(m_beginDateEdit, 1);
    //begintimeelayout->addWidget(m_beginTimeEdit, 1);
    //begintimeelayout->setMargin(0);
    //begintimeelayout->setSpacing(0);
    //begintimeelayout->addStretch();
    beginLabellayout->addWidget(m_beginTimeLabel);
    beginLabellayout->addWidget(m_beginDateEdit);
    beginLabellayout->addSpacing(10);
    beginLabellayout->addWidget(m_beginTimeEdit);
    beginLabellayout->addStretch();
    // beginLabellayout->addLayout(begintimeelayout);
    maintlayout->addLayout(beginLabellayout);


    QHBoxLayout *endLabellayout  = new QHBoxLayout;
    endLabellayout->setSpacing(0);
    endLabellayout->setMargin(0);
    m_endTimeLabel = new DLabel (tr("End Time:"));
    m_endTimeLabel->setFont(mlabelF);
    m_endTimeLabel->setPalette(pa);
    m_endTimeLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_endTimeLabel->setFixedSize(78, 36);
    m_endDateEdit = new QDateEdit(this);
    m_endDateEdit->setFixedSize(200, 36);
    m_endTimeEdit = new CTimeEdit(this);
    m_endTimeEdit->setFixedSize(109, 36);
    m_endDateEdit->setCalendarPopup(true);
    m_endDateEdit->setDisplayFormat("yyyy-MM-dd");

    endLabellayout->addWidget(m_endTimeLabel);
    endLabellayout->addWidget(m_endDateEdit);
    endLabellayout->addSpacing(10);

    endLabellayout->addWidget(m_endTimeEdit);
    endLabellayout->addStretch();
    //endLabellayout->addLayout(endtimeelayout);
    maintlayout->addLayout(endLabellayout);

    QHBoxLayout *rmindLabellayout  = new QHBoxLayout;
    rmindLabellayout->setSpacing(0);
    rmindLabellayout->setMargin(0);
    m_remindSetLabel = new DLabel (tr("Remind Set:"));
    m_remindSetLabel->setFont(mlabelF);
    m_remindSetLabel->setPalette(pa);
    m_remindSetLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_remindSetLabel->setFixedWidth(78);

    m_rmindCombox = new DComboBox();
    m_rmindCombox->setFixedSize(200, 36);
    m_rmindCombox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
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
    m_beginrepeatLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_beginrepeatLabel->setFixedWidth(78);
    m_beginrepeatCombox = new DComboBox();
    m_beginrepeatCombox->setFixedSize(200, 36);
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
    m_endrepeatLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_endrepeatLabel->setFixedWidth(78);
    m_endrepeatCombox = new DComboBox();
    m_endrepeatCombox->setFixedSize(200, 36);
    m_endrepeatCombox->addItem(tr("Never"));
    m_endrepeatCombox->addItem(tr("In"));
    m_endrepeatCombox->addItem(tr("On the date"));
    endrepeatLabellayout->addWidget(m_endrepeatLabel);
    endrepeatLabellayout->addWidget(m_endrepeatCombox);

    QHBoxLayout *endrepeattimeslayout  = new QHBoxLayout;
    m_endrepeattimes = new DLineEdit(this);
    m_endrepeattimes->setFixedSize(71, 36);
    m_endrepeattimes->setText(QString::number(10));
    m_endrepeattimes->setClearButtonEnabled(false);
    QRegExp rx("^[1-9]\\d{0,2}$");
    QValidator *validator = new QRegExpValidator(rx, this);
    m_endrepeattimes->lineEdit()->setValidator(validator);
    m_endrepeattimesLabel = new DLabel(tr("s"));
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
    m_endRepeatDate->setFixedSize(120, 36);
    m_endRepeatDate->setDate(QDate::currentDate());
    m_endRepeatDate->setDisplayFormat("yyyy-MM-dd");
    QFont enddatefont;
    enddatefont.setFamily("SourceHanSansSC-Medium");
    enddatefont.setPixelSize(11);
    m_endRepeatDate->setFont(enddatefont);
    //m_endRepeatDate->setMinimumWidth(150);
    //m_endRepeatDate->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    endrepeatLabellayout->addWidget(m_endRepeatDate);
    endrepeatLabellayout->addStretch();
    m_endRepeatDate->setVisible(false);
    m_endrepeatWidget = new DWidget;
    m_endrepeatWidget->setLayout(endrepeatLabellayout);
    maintlayout->addWidget(m_endrepeatWidget);
    m_endrepeatWidget->setVisible(false);
    QHBoxLayout *downlayout  = new QHBoxLayout;
    downlayout->setContentsMargins(0, 20, 0, 0);
    m_cancelBt = new DPushButton(tr("Cancel"));
    m_cancelBt->setFixedSize(189, 36);
    m_OkBt = new DPushButton(tr("OK"));
    m_OkBt->setFixedSize(189, 36);
    DPalette okpa = m_OkBt->palette();
    if (themetype == 0 || themetype == 1) {
        okpa.setColor(DPalette::ButtonText, Qt::white);
        okpa.setColor(DPalette::Dark, QColor("#25B7FF"));
        okpa.setColor(DPalette::Light, QColor("#0098FF"));
    } else {
        okpa.setColor(DPalette::ButtonText, "#B8D3FF");
        okpa.setColor(DPalette::Dark, QColor("#0056C1"));
        okpa.setColor(DPalette::Light, QColor("#004C9C"));
    }

    m_OkBt->setPalette(okpa);
    downlayout->addWidget(m_cancelBt);
    DVerticalLine *verline = new DVerticalLine(this);
    verline->setFixedSize(3, 28);
    downlayout->addWidget(verline);
    downlayout->addWidget(m_OkBt);
    m_gwi = new DFrame(this);
    maintlayout->addLayout(downlayout);
    maintlayout->addStretch();
    m_gwi->setLayout(maintlayout);
    m_gwi->setGeometry(0, 68, 438, 412);
    //addContent(gwi);
    //setLayout(maintlayout);
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
    m_beginDateEdit->setMaximumDate(QDate(9999, 12, 31)); //
    //m_beginDateEdit->setContextMenuPolicy(Qt::NoContextMenu);
    // m_bCustomDateW->disconnect(SIGNAL(selectionChanged()));
    //m_bCustomDateW->disconnect(SIGNAL(clicked(QDate)));

    m_eCustomDateW = new CCustomCalendarWidget(0);
    m_endDateEdit->setCalendarWidget(m_eCustomDateW);
    m_endDateEdit->setMinimumDate(QDate(1900, 1, 1)); // 0天
    m_endDateEdit->setMaximumDate(QDate(9999, 12, 31)); //
    m_endDateEdit->setContextMenuPolicy(Qt::NoContextMenu);
    //m_eCustomDateW->disconnect(SIGNAL(selectionChanged()));
    //m_eCustomDateW->disconnect(SIGNAL(clicked(QDate)));

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
            } else if (m_scheduleDtailInfo.remindData.n == 10080) {
                m_rmindCombox->setCurrentIndex(6);
            }
        } else {
            m_rmindCombox->setCurrentIndex(0);
        }
    }
    slotbRpeatactivated(m_scheduleDtailInfo.rpeat);
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

