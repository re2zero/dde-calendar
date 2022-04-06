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
#ifndef COLORPICKERWIDGET_H
#define COLORPICKERWIDGET_H

#include "colorlabel.h"
#include "colorslider.h"

#include <DAbstractDialog>
#include <DLineEdit>
#include <DPushButton>
#include <QLabel>

DWIDGET_USE_NAMESPACE
class CColorPickerWidget : public DAbstractDialog
{
    Q_OBJECT
public:
    explicit CColorPickerWidget(QWidget *parent = nullptr);
    ~CColorPickerWidget();

    QColor getSelectedColor();

private:
    void initUI();
    void setColorHexLineEdit();

public slots:
    /**
     * @brief slotSetColor 设置颜色
     * @param c　颜色
     * @param internalChanged　由内变化
     */
    void slotSetColor(const QColor &c);

    /**
     * @brief slotUpdateColor　更新颜色
     */
    void slotUpdateColor(const QColor &color = QColor());

    void slotCancelBtnClicked();
    void slotEnterBtnClicked();

private:
    ColorLabel *m_colorLabel;
    ColorSlider *m_colorSlider;

    DLineEdit *m_colHexLineEdit;
    DLabel *m_wordLabel;
    DPushButton *m_cancelBtn;
    DPushButton *m_enterBtn;

    QColor curColor;
};

#endif // COLORPICKERWIDGET_H
