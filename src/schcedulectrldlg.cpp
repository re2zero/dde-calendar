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
#include "schcedulectrldlg.h"
#include <QVBoxLayout>
#include "schceduledlg.h"
#include "dbmanager.h"
#include "scheduledatamanage.h"
#include <DMessageBox>
#include <DPushButton>
#include <DHiDPIHelper>
#include <DPalette>
DGUI_USE_NAMESPACE
CSchceduleCtrlDlg::CSchceduleCtrlDlg(QWidget *parent) : DDialog(parent)
{
    initUI();
    initConnection();
    //setTitle(tr("My Schcedule"));
    setFixedSize(380, 160);

}

void CSchceduleCtrlDlg::initUI()
{
    m_icon = new DLabel(this);
    m_icon->setFixedSize(32, 32);
    m_icon->setPixmap(DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-logo.svg")
                      .scaled(m_icon->size() * devicePixelRatioF()));
    m_icon->move(10, 9);
    m_Title = new DLabel(this);
    m_Title->setFixedHeight(48);
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


    m_mainBoxLayout = new QVBoxLayout(this);
    m_mainBoxLayout->setMargin(0);
    m_mainBoxLayout->setSpacing(0);
    m_mainBoxLayout->setContentsMargins(10, 10, 10, 10);

    m_firstLabel = new DLabel(this);
    m_firstLabel->setAlignment(Qt::AlignCenter);
    QFont labelF;
    labelF.setFamily("SourceHanSansSC-Medium");
    labelF.setPixelSize(14);
    DPalette wpa = m_firstLabel->palette();
    if (themetype == 0 || themetype == 1) {
        wpa.setColor(DPalette::WindowText, QColor("#2C4767"));

    } else {
        wpa.setColor(DPalette::WindowText, QColor("#A8B7D1"));
    }
    m_firstLabel->setPalette(wpa);
    m_firstLabel->setFont(labelF);
    m_mainBoxLayout->addWidget(m_firstLabel);


    m_seconLabel = new DLabel(this);
    //m_seconLabel->setFixedHeight(26);
    m_seconLabel->setAlignment(Qt::AlignCenter);
    QFont labelT;
    labelT.setFamily("SourceHanSansSC-Bold");
    labelT.setPixelSize(14);
    DPalette tpa = m_seconLabel->palette();


    if (themetype == 0 || themetype == 1) {
        tpa.setColor(DPalette::WindowText, QColor("#6A829F"));
    } else {
        tpa.setColor(DPalette::WindowText, QColor("#6A829F"));
    }



    m_seconLabel->setPalette(tpa);
    m_seconLabel->setFont(labelT);
    m_mainBoxLayout->addWidget(m_seconLabel);


    m_btBoxLayout = new QHBoxLayout;
    m_btBoxLayout->setMargin(0);
    m_btBoxLayout->setSpacing(0);
    m_btBoxLayout->setContentsMargins(0, 0, 0, 0);

    m_mainBoxLayout->addLayout(m_btBoxLayout);
    DFrame *gwi = new DFrame(this);
    gwi->setLayout(m_mainBoxLayout);
    gwi->setGeometry(0, 51, 380, 110);
}

void CSchceduleCtrlDlg::initConnection()
{

}

DPushButton *CSchceduleCtrlDlg::addPushButton(QString btName)
{
    return NULL;
}

void CSchceduleCtrlDlg::setText(QString str)
{
    m_firstLabel->setText(str);
}

void CSchceduleCtrlDlg::setInformativeText(QString str)
{
    m_seconLabel->setText(str);
}
