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
#include "colorseletorwidget.h"
#include <QPushButton>

ColorSeletorWidget::ColorSeletorWidget(QWidget *parent) : QWidget(parent)
{
    init();
}

void ColorSeletorWidget::init()
{
    initView();
    m_colorGroup = new QButtonGroup(this);
    m_colorGroup->setExclusive(true);
    connect(m_colorGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), this, &ColorSeletorWidget::slotButtonClicked);
}

void ColorSeletorWidget::initColorButton(int index)
{
    //留着测试使用，待与后台数据联调时再处理
    reset();
    addColor(QColor("#FF5E97"));
    addColor(QColor("#FF9436"));
    addColor(QColor("#FFDC00"));
    addColor(QColor("#5BDD80"));
    addColor(QColor("#00B99B"));
    addColor(QColor("#4293FF"));
    addColor(QColor("#5D51FF"));
    addColor(QColor("#A950FF"));
    addColor(QColor("#717171"));

    if (index >= 0 && index < m_colorGroup->buttons().size()) {
        m_colorGroup->buttons().at(index)->click();
    }
}

void ColorSeletorWidget::reset()
{
    //清空所有的色彩实体和控件
    m_colorEntityMap.clear();
    QList<QAbstractButton*> buttons = m_colorGroup->buttons();
    for (QAbstractButton* but : buttons) {
        m_colorGroup->removeButton(but);
        m_colorLayout->removeWidget(but);
        delete but;
    }
}

void ColorSeletorWidget::addColor(const QColor& color, const QString& info)
{
    ColorEntity cEntity = {color, info, TypeSystem};
    addColor(cEntity);
}

void ColorSeletorWidget::addColor(const ColorEntity& cEntity)
{
    static int count = 0;   //静态变量，充当色彩控件id
    count++;
    m_colorEntityMap[count] = cEntity;      //映射id与控件
    CRadioButton *radio = new CRadioButton();
    radio->setColor(cEntity.color);         //设置控件颜色
    radio->setFixedSize(18, 18);
    m_colorGroup->addButton(radio, count);
    m_colorLayout->addWidget(radio);
    if (cEntity.type == TypeUser) {
        m_userColorBut = radio;             //记录用户色彩指针
        m_userColorButId = count;
    }
}

QColor ColorSeletorWidget::getSelectedColor()
{
    return m_colorEntity.color;
}

void ColorSeletorWidget::setSelectedColor(int index)
{
    if (index >= 0 && index < m_colorGroup->buttons().size()) {
        QAbstractButton* but = m_colorGroup->buttons().at(index);
        if (nullptr != but) {
            but->click();
        }
    }
}

void ColorSeletorWidget::setSelectedColor(const QColor& color)
{
    bool isFind = false;
    //遍历所有控件
    for (QAbstractButton* but: m_colorGroup->buttons()) {
        if (nullptr != but && qobject_cast<CRadioButton*>(but)->getColor() == color) {
            but->click();
            isFind = true;
        }
    }
    if (!isFind){
        setUserColor(color);
    }
}

void ColorSeletorWidget::initView()
{
    m_colorLayout = new QHBoxLayout();

    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->addLayout(m_colorLayout);
    QPushButton *addColorBut = new QPushButton();
    addColorBut->setIcon(DStyle().standardIcon(DStyle::SP_IncreaseElement));
    addColorBut->setFixedSize(18, 18);
    addColorBut->setIconSize(QSize(10, 10));
    hLayout->addWidget(addColorBut);
    hLayout->addStretch(1);

    m_colorLayout->setMargin(0);
    m_colorLayout->setSpacing(3);
    hLayout->setMargin(0);
    hLayout->setSpacing(3);

    this->setLayout(hLayout);

    connect(addColorBut, &QPushButton::clicked, this, &ColorSeletorWidget::slotAddColorButClicked);
}

void ColorSeletorWidget::slotButtonClicked(int butId)
{
    ColorEntity cEn = m_colorEntityMap.find(butId).value();
    if (cEn.color != m_colorEntity.color) {
        m_colorEntity = cEn;
        emit signalColorChange(m_colorEntity.color);
    }
}

void ColorSeletorWidget::slotAddColorButClicked()
{
    CColorPickerWidget *colorPicker = new CColorPickerWidget(this);

    if (colorPicker->exec()) {
        //设置用户自定义控件颜色
        setUserColor(colorPicker->getSelectedColor());
        m_userColorBut->click();
    }
    delete colorPicker;
}

void ColorSeletorWidget::setUserColor(const QColor& color)
{
    if (nullptr == m_userColorBut) {
        addColor({color, "", TypeUser});
    }
    m_userColorBut->setColor(color);
    ColorEntity entity = m_colorEntityMap[m_userColorButId];
    entity.color = color;
    m_colorEntityMap[m_userColorButId] = entity;
}
