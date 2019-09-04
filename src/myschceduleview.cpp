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
CMySchceduleView::CMySchceduleView(QWidget *parent) : DDialog(parent)
{
    initUI();
    initConnection();
    setTitle(tr("My Schcedule"));
    setFixedSize(400, 180);
}

void CMySchceduleView::setSchedules(ScheduleInfo info)
{
    m_scheduleInfo = info;
    m_schceduleLabel->setText(info.titleName);
    m_timeLabel->setText(info.beginDateTime.toString("yyyy-MM-dd hh:mm") + " ~ " + info.endDateTime.toString("yyyy-MM-dd hh:mm"));
}

void CMySchceduleView::slotEditBt()
{
    CSchceduleDlg dlg(0, this);
    dlg.setData(m_scheduleInfo);
    if (dlg.exec() == DDialog::Accepted) {
        ScheduleInfo info = dlg.getData();
        info.id = m_scheduleInfo.id;
        m_scheduleInfo = info;
        ScheduleDbManager::updateScheduleInfo(info);
        if (m_scheduleInfo.beginDateTime.date() == info.beginDateTime.date()) {
            setSchedules(m_scheduleInfo);
            emit signalsEditorDelete(0);
        } else {
            accept();
            emit signalsEditorDelete(1);
        }
    }
}

void CMySchceduleView::slotDeleteBt()
{
    accept();
    ScheduleDbManager::deleteScheduleInfoById(m_scheduleInfo.id);
    emit signalsEditorDelete(1);
}

void CMySchceduleView::initUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(20, 10, 10, 0);

    m_schceduleLabel = new DLabel(this);
    m_schceduleLabel->setFixedHeight(22);
    m_schceduleLabel->setAlignment(Qt::AlignLeft);
    QFont labelF;
    labelF.setFamily("PingFangSC-Semibold");
    labelF.setPixelSize(16);
    QPalette wpa;
    wpa.setColor(QPalette::WindowText, QColor("#414D68"));
    m_schceduleLabel->setPalette(wpa);
    m_schceduleLabel->setFont(labelF);
    mainLayout->addWidget(m_schceduleLabel, 2);

    QFrame *lineframe = new QFrame(this);
    lineframe->setFrameShape(QFrame::HLine);
    lineframe->setFrameShadow(QFrame::Plain);
    lineframe->setLineWidth(2);
    lineframe->setFixedSize(360, 2);
    QHBoxLayout *hlineLayout = new QHBoxLayout;
    hlineLayout->setMargin(0);
    hlineLayout->setSpacing(0);
    hlineLayout->setContentsMargins(0, 0, 0, 3);
    hlineLayout->addStretch(1);
    hlineLayout->addWidget(lineframe);
    hlineLayout->addStretch(1);
    mainLayout->addLayout(hlineLayout);

    m_timeLabel = new DLabel(this);
    m_timeLabel->setFixedHeight(22);
    m_timeLabel->setAlignment(Qt::AlignLeft);
    m_timeLabel->setPalette(wpa);
    m_timeLabel->setFont(labelF);
    mainLayout->addWidget(m_timeLabel, 2);

    QFrame *btframe = new QFrame(this);
    QHBoxLayout *hBtLayout = new QHBoxLayout;
    hBtLayout->setMargin(0);
    hBtLayout->setSpacing(0);
    hBtLayout->setContentsMargins(0, 0, 0, 3);
    hBtLayout->addStretch();
    m_editBt = new DPushButton(tr("Edit"));
    m_deleteBt = new DPushButton(tr("Delete"));
    hBtLayout->addWidget(m_deleteBt);

    QFrame *vlineframe = new QFrame(this);
    vlineframe->setFrameShape(QFrame::VLine);
    vlineframe->setFrameShadow(QFrame::Plain);
    vlineframe->setLineWidth(2);
    //vlineframe->setFixedSize(300, 2);
    hBtLayout->addWidget(vlineframe);
    hBtLayout->addWidget(m_editBt);
    btframe->setLayout(hBtLayout);
    mainLayout->addWidget(btframe, 1);
    QWidget *gwi = new QWidget;
    QVBoxLayout *tlayout = new QVBoxLayout;
    tlayout->setMargin(0);
    tlayout->setSpacing(0);
    tlayout->addLayout(mainLayout);
    gwi->setLayout(tlayout);
    addContent(gwi);
}

void CMySchceduleView::initConnection()
{
    connect(m_editBt, &DPushButton::clicked, this, &CMySchceduleView::slotEditBt);
    connect(m_deleteBt, &DPushButton::clicked, this, &CMySchceduleView::slotDeleteBt);
}
