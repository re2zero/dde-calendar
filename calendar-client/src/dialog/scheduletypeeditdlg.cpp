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
#include "scheduledatamanage.h"
#include "cscheduleoperation.h"
#include "configsettings.h"

#include <DFrame>
#include <DTitlebar>
#include <DIconButton>

#include <QLabel>
#include <QRadioButton>

ScheduleTypeEditDlg::ScheduleTypeEditDlg(QWidget *parent)
    : DDialog(parent)
    , m_title(tr("New event type"))
{
    init();
}

ScheduleTypeEditDlg::ScheduleTypeEditDlg(const JobTypeInfo &jobTypeOld, QWidget *parent)
    : DDialog(parent)
    , m_jobTypeOld(jobTypeOld)
    , m_title(tr("Edit event type"))
{
    init();
}

void ScheduleTypeEditDlg::init()
{
    initView();
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
    connect(this->getButton(0), &QPushButton::clicked, this, &ScheduleTypeEditDlg::slotBtnCancel);
    connect(this->getButton(1), &QPushButton::clicked, this, &ScheduleTypeEditDlg::slotBtnNext);
}

void ScheduleTypeEditDlg::initData()
{
    m_titleLabel->setText(m_title);
    m_lineEdit->setText(m_jobTypeOld.getJobTypeName());
    this->getButton(1)->setEnabled(!m_jobTypeOld.getJobTypeName().isEmpty());//如果是新增，则保存按钮默认不可用
    if(nullptr != m_colorSeletor){
        JobTypeInfoManager::instance()->getJobTypeColorByNo(m_jobTypeOld.getColorTypeNo(), m_jobTypeColorOld);

        if (CConfigSettings::getInstance()->contains("LastUserColor")){
            QString colorName = CConfigSettings::getInstance()->value("LastUserColor").toString();
            if (!colorName.isEmpty()) {
                m_colorSeletor->setUserColor(JobTypeColorInfo(0, colorName, 7));
            }
        }

        if (CConfigSettings::getInstance()->contains("LastSysColorTypeNo")){
            int colorId = CConfigSettings::getInstance()->value("LastSysColorTypeNo").toInt();
            if (colorId > 0) {
                m_colorSeletor->setSelectedColorById(colorId);
            }
        }
        if( 0 < m_jobTypeColorOld.getTypeNo()){
            //todo:
            //1.新建时，默认选中上次选中的默认颜色后的一个
            //2.自定义颜色，如果m_jobTypeColorOld不是自定义颜色，则把用户上一次保存的自定义颜色加到选择器的自定义颜色
            m_colorSeletor->setSelectedColorByIndex(m_jobTypeColorOld);//设置选中颜色setSelectedColor
            //setUserColor用来添加自定义颜色
        }
    }
}

void ScheduleTypeEditDlg::slotEditTextChanged(const QString &strName)
{
    //文本编辑框中文本改变事件
    //1不能为空，2不能全空格，3不能重名
    if(strName.isEmpty()){
        //名称为空，返回
        m_lineEdit->showAlertMessage(tr("Enter a name please"));
        this->getButton(1)->setEnabled(false);
        return;
    }
    if(strName.trimmed().isEmpty()){
        //名称为全空格，返回
        m_lineEdit->showAlertMessage(tr("The name can not only contain whitespaces"));
        this->getButton(1)->setEnabled(false);
        return;
    }

    if(JobTypeInfoManager::instance()->isJobTypeNameUsed(strName)){
        //重名，返回
        m_lineEdit->showAlertMessage(tr("The name already exists"));
        this->getButton(1)->setEnabled(false);
        return;
    }
    this->getButton(1)->setEnabled(true);
    m_jobTypeNew.setJobTypeName(strName);
    return;
}

void ScheduleTypeEditDlg::slotBtnCancel()
{
    this->close();
}

void ScheduleTypeEditDlg::slotBtnNext()
{
    QString strName = m_lineEdit->text();
    if(strName != m_jobTypeOld.getJobTypeName()){
        CScheduleOperation so;
        //先修改颜色
        JobTypeColorInfo colorInfoNew = m_colorSeletor->getSelectedColorInfo();
        m_jobTypeNew.setColorTypeNo(colorInfoNew.getTypeNo());
        m_jobTypeNew.setColorHex(colorInfoNew.getColorHex());

        so.updateJobType(m_jobTypeOld, m_jobTypeNew);//更新日程类型
    }
}

