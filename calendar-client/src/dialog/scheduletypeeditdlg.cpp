/*
* Copyright (C) 2019 ~ 2019 UnionTech Software Technology Co.,Ltd.
*
* Author:     leilong <leilong@uniontech.com>
*
* Maintainer: leilong <leilong@uniontech.com>
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
#include "scheduletypeeditdlg.h"
#include <QLabel>
#include <QRadioButton>
#include <DFrame>

ScheduleTypeEditDlg::ScheduleTypeEditDlg(QWidget *parent)
    : DDialog(parent)
    , m_title(tr("New event type"))
{
    init();
}

ScheduleTypeEditDlg::ScheduleTypeEditDlg(QString scheduleName, QWidget *parent)
    : DDialog(parent)
    , m_scheduleName(scheduleName)
    , m_title(tr("Edit event type"))
{
    init();
}

ScheduleTypeEditDlg::ScheduleTypeEditDlg(QString title, QString scheduleName, QWidget *parent)
    : DDialog(parent)
    , m_scheduleName(scheduleName)
    , m_title(title)
{
    init();
}

void ScheduleTypeEditDlg::init()
{
    initView();
    connect(m_colorSeletor, &ColorSeletorWidget::signalColorChange, this, &ScheduleTypeEditDlg::slotColorChange);
    connect(m_lineEdit, &DLineEdit::textChanged, this, &ScheduleTypeEditDlg::slotEditTextChanged);
    initData();
}

void ScheduleTypeEditDlg::initView(){
    setFixedSize(QSize(400, 220));

    m_titleLabel = new QLabel(this);
    QFont titlelabelF;
    titlelabelF.setWeight(QFont::DemiBold);
    titlelabelF.setPixelSize(17);
    m_titleLabel->setFixedSize(400, 25);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->move(0, 19);
    m_titleLabel->setFont(titlelabelF);

    QVBoxLayout *maintlayout = new QVBoxLayout;
    maintlayout->setMargin(0);
    maintlayout->setSpacing(10);

    QHBoxLayout *eLayout = new QHBoxLayout;
    QLabel *eName = new QLabel(tr("Name:"));
    eName->setFixedWidth(42);

    m_lineEdit = new DLineEdit();
    m_lineEdit->setClearButtonEnabled(false); //不显示按钮
    eLayout->addWidget(eName);
    eLayout->addWidget(m_lineEdit, 1);

    QHBoxLayout *cLayout = new QHBoxLayout;
    QLabel *cName = new QLabel(tr("Color:"));
    cName->setFixedWidth(42);
    m_colorSeletor = new ColorSeletorWidget();

    cLayout->addWidget(cName);
    cLayout->addWidget(m_colorSeletor);
    cLayout->addStretch(1);

    maintlayout->addLayout(eLayout);
    maintlayout->addLayout(cLayout);

    eLayout->setSpacing(10);
    eLayout->setMargin(0);
    cLayout->setSpacing(10);
    cLayout->setMargin(0);
    maintlayout->setSpacing(20);
    maintlayout->setContentsMargins(0, 10, 0, 0);

    DFrame *gwi = new DFrame(this);
    gwi->setFrameShape(QFrame::NoFrame);
    gwi->setLayout(maintlayout);
    gwi->setMinimumWidth(360);
    addContent(gwi, Qt::AlignCenter);

    //添加按钮
    addButton(tr("Cancel", "button"));
    addButton(tr("Save", "button"), false, DDialog::ButtonRecommend);
    for (int i = 0; i < buttonCount(); i++) {
        QAbstractButton *button = getButton(i);
        button->setFixedSize(170, 36);
    }
}

void ScheduleTypeEditDlg::initData()
{
    m_titleLabel->setText(m_title);
    m_lineEdit->setText(m_scheduleName);
    m_colorSeletor->initColorButton(0);
}

void ScheduleTypeEditDlg::slotColorChange(QColor color)
{
    QPalette palette = m_lineEdit->palette();
    palette.setColor(QPalette::Button, color);
    m_lineEdit->setPalette(palette);    //更改文本编辑框颜色
}

void ScheduleTypeEditDlg::slotEditTextChanged(const QString &text)
{
    Q_UNUSED(text)
    //文本编辑框中文本改变事件，待类型名合法性判断接口完成后完善
}
