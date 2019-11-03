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
#include "myschceduleview.h"
#include <QVBoxLayout>
#include "schceduledlg.h"
#include "dbmanager.h"
#include "scheduledatamanage.h"
#include <DMessageBox>
#include <DPushButton>
#include <DHiDPIHelper>
#include <DPalette>
#include "schcedulectrldlg.h"
DGUI_USE_NAMESPACE
CMySchceduleView::CMySchceduleView(QWidget *parent) : DDialog(parent)
{
    setContentsMargins(0, 0, 0, 0);
    initUI();
    initConnection();
    //setTitle(tr("My Schcedule"));
    setFixedSize(403, 160);

    //setIconPixmap(DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-logo.svg").scaled(QSize(34, 34) * devicePixelRatioF()));
}
void CMySchceduleView::AutoFeed(QString text)
{
    QString strText = text;
    QFont labelF;
    labelF.setFamily("SourceHanSansSC-Medium");
    labelF.setPixelSize(14);
    QFontMetrics fm(labelF);
    int row = 0;

    QString str;
    if (!strText.isEmpty()) {
        for (int i = 0; i < strText.count(); i++) {
            str.append(strText.at(i));
            int widthT = fm.width(str) + 5;
            if (widthT >= 340) {
                strText.insert(i - 1, "\n");
                i--;
                str.clear();
                row++;
            }
        }
    }
    m_schceduleLabel->setFixedHeight((row + 1) * 24);
    setFixedHeight(row * 24 + 160);
    m_schceduleLabel->setText(strText);
    m_schceduleLabel->adjustSize();
}
void CMySchceduleView::setSchedules(ScheduleDtailInfo info)
{
    m_scheduleInfo = info;
    AutoFeed(info.titleName);
    //m_schceduleLabel->setText(info.titleName);
    m_timeLabel->setText(info.beginDateTime.toString("yyyy-MM-dd hh:mm") + " ~ " + info.endDateTime.toString("yyyy-MM-dd hh:mm"));
}

void CMySchceduleView::slotEditBt()
{
    CSchceduleDlg dlg(0, this);
    dlg.setData(m_scheduleInfo);
    if (dlg.exec() == DDialog::Accepted) {
        accept();
        emit signalsEditorDelete(1);
    }
}

void CMySchceduleView::slotDeleteBt()
{
    int themetype = CScheduleDataManage::getScheduleDataManage()->getTheme();

    if (m_scheduleInfo.rpeat == 0) {
        CSchceduleCtrlDlg msgBox;
        //msgBox.setWindowFlags(Qt::FramelessWindowHint);
        // msgBox.setIconPixmap(DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-logo.svg").scaled(QSize(34, 34) * devicePixelRatioF()));
        msgBox.setText(tr("You are deleted schedule"));
        msgBox.setInformativeText(tr("Are you sure you want to delete this schedule?"));
        DPushButton *noButton = msgBox.addPushButton(tr("Cancel"));
        DPushButton *yesButton = msgBox.addPushButton(tr("Delete Schedule"));
        DPalette pa = yesButton->palette();

        if (themetype == 0 || themetype == 1) {
            pa.setColor(DPalette::ButtonText, Qt::red);

        } else {
            pa.setColor(DPalette::ButtonText, "#FF5736");

        }
        yesButton->setPalette(pa);
        msgBox.updatesize();
        msgBox.exec();

        if (msgBox.clickButton() == noButton) {
            return;
        } else if (msgBox.clickButton() == yesButton) {
            CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->deleteScheduleInfoById(m_scheduleInfo.id);
        }
    } else {
        if (m_scheduleInfo.RecurID == 0) {
            CSchceduleCtrlDlg msgBox;
            //msgBox.setWindowFlags(Qt::FramelessWindowHint);
            //msgBox.setIconPixmap(DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-logo.svg").scaled(QSize(34, 34) * devicePixelRatioF()));

            msgBox.setText(tr("You are deleted schedule"));
            msgBox.setInformativeText(tr("You want to delete all repeat of the schedule, or just delete the selected repeat?"));
            DPushButton *noButton = msgBox.addPushButton(tr("Cancel"));
            DPushButton *yesallbutton = msgBox.addPushButton(tr("All Deleted"));
            DPushButton *yesButton = msgBox.addPushButton(tr("Just Delete Schedule"));
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
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->deleteScheduleInfoById(m_scheduleInfo.id);
            } else if (msgBox.clickButton() == yesButton) {

                ScheduleDtailInfo newschedule;
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(m_scheduleInfo.id, newschedule);
                newschedule.ignore.append(m_scheduleInfo.beginDateTime);
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);
            }
        } else {
            CSchceduleCtrlDlg msgBox;
            //msgBox.setWindowFlags(Qt::FramelessWindowHint);
            //msgBox.setIconPixmap(DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-logo.svg").scaled(QSize(34, 34) * devicePixelRatioF()));
            msgBox.setText(tr("You are deleted schedule"));
            msgBox.setInformativeText(tr("You want to delete the schedule of this repetition and all repeat in the future, or just delete all repeat?"));
            DPushButton *noButton = msgBox.addPushButton(tr("Cancel"));
            DPushButton *yesallbutton = msgBox.addPushButton(tr("Delete all schedule in the future"));
            DPushButton *yesButton = msgBox.addPushButton(tr("Just Delete Schedule"));
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
                ScheduleDtailInfo newschedule;
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(m_scheduleInfo.id, newschedule);

                newschedule.enddata.type = 2;
                newschedule.enddata.date = m_scheduleInfo.beginDateTime;
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);

            } else if (msgBox.clickButton() == yesButton) {

                ScheduleDtailInfo newschedule;
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(m_scheduleInfo.id, newschedule);

                newschedule.ignore.append(m_scheduleInfo.beginDateTime);
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);
            }
        }
    }
    accept();
    emit signalsEditorDelete(1);
}

void CMySchceduleView::initUI()
{
    m_icon = new DLabel(this);
    m_icon->setFixedSize(30, 30);
    m_icon->setPixmap(DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-logo.svg")
                      .scaled(m_icon->size() * devicePixelRatioF()));
    m_icon->move(11, 10);
    m_Title = new DLabel(this);
    m_Title->setFixedSize(108, 51);
    m_Title->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

    QFont labelTitle;
    labelTitle.setFamily("SourceHanSansSC-Bold");
    labelTitle.setPixelSize(17);
    int themetype = CScheduleDataManage::getScheduleDataManage()->getTheme();

    DPalette titlepa = m_Title->palette();
    if (themetype == 0 || themetype == 1) {
        titlepa.setColor(DPalette::WindowText, QColor("#001A2E"));

    } else {
        titlepa.setColor(DPalette::WindowText, QColor("#C0C6D4"));
    }

    m_Title->setPalette(titlepa);
    m_Title->setFont(labelTitle);
    m_Title->setText(tr("My Schcedule"));
    m_Title->move(136, 0);



    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    m_schceduleLabel = new DLabel(this);
    m_schceduleLabel->setFixedHeight(26);
    m_schceduleLabel->setAlignment(Qt::AlignCenter);
    QFont labelF;
    labelF.setFamily("SourceHanSansSC-Medium");
    labelF.setPixelSize(14);
    DPalette wpa = m_schceduleLabel->palette();
    if (themetype == 0 || themetype == 1) {
        wpa.setColor(DPalette::WindowText, QColor("#2C4767"));

    } else {
        wpa.setColor(DPalette::WindowText, QColor("#A8B7D1"));
    }
    m_schceduleLabel->setPalette(wpa);
    m_schceduleLabel->setFont(labelF);
    mainLayout->addWidget(m_schceduleLabel);


    m_timeLabel = new DLabel(this);
    m_timeLabel->setFixedHeight(26);
    m_timeLabel->setAlignment(Qt::AlignCenter);
    QFont labelT;
    labelT.setFamily("SourceHanSansSC-Bold");
    labelT.setPixelSize(14);
    DPalette tpa = m_timeLabel->palette();


    if (themetype == 0 || themetype == 1) {
        tpa.setColor(DPalette::WindowText, QColor("#6A829F"));
    } else {
        tpa.setColor(DPalette::WindowText, QColor("#6A829F"));
    }



    m_timeLabel->setPalette(tpa);
    m_timeLabel->setFont(labelT);
    mainLayout->addWidget(m_timeLabel);

    DVerticalLine *btframe = new DVerticalLine(this);
    btframe->setFixedSize(3, 28);
    QHBoxLayout *hBtLayout = new QHBoxLayout;
    hBtLayout->setMargin(0);
    hBtLayout->setSpacing(0);
    hBtLayout->setContentsMargins(0, 0, 0, 0);
    // hBtLayout->addStretch();
    m_editBt = new DPushButton(tr("Edit"));
    DPalette pa = m_editBt->palette();
    if (themetype == 0 || themetype == 1) {
        pa.setColor(DPalette::ButtonText, Qt::white);
        pa.setColor(DPalette::Dark, QColor("#25B7FF"));
        pa.setColor(DPalette::Light, QColor("#0098FF"));
    } else {
        pa.setColor(DPalette::ButtonText, "#B8D3FF");
        pa.setColor(DPalette::Dark, QColor("#0056C1"));
        pa.setColor(DPalette::Light, QColor("#004C9C"));
    }
    m_editBt->setPalette(pa);
    m_editBt->setFixedSize(170, 36);
    m_deleteBt = new DPushButton(tr("Delete"));
    m_deleteBt->setFixedSize(170, 36);
    hBtLayout->addWidget(m_deleteBt);
    hBtLayout->addWidget(btframe);
    hBtLayout->addWidget(m_editBt);
    mainLayout->addLayout(hBtLayout);
    DFrame *gwi = new DFrame(this);
    gwi->setContentsMargins(0, 0, 0, 0);
    gwi->setLayout(mainLayout);
    DPalette anipa = gwi->palette();
    QColor color = "#F8F8F8";
    color.setAlphaF(0.0);
    anipa.setColor(DPalette::Background, color);
    gwi->setAutoFillBackground(true);
    gwi->setPalette(anipa);
    gwi->setBackgroundRole(DPalette::Background);
    // gwi->setGeometry(0, 51, 380, 110);
    addContent(gwi, Qt::AlignCenter);
}

void CMySchceduleView::initConnection()
{
    connect(m_editBt, &DPushButton::clicked, this, &CMySchceduleView::slotEditBt);
    connect(m_deleteBt, &DPushButton::clicked, this, &CMySchceduleView::slotDeleteBt);
}
