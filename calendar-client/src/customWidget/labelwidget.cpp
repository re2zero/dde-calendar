/*
   * Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
   *
   * Author:     chenhaifeng <chenhaifeng@uniontech.com>
   *
   * Maintainer: chenhaifeng <chenhaifeng@uniontech.com>
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
#include "labelwidget.h"
#include <QPainter>
#include <QStyleOptionFocusRect>

LabelWidget::LabelWidget(QWidget *parent)
    : QLabel(parent)
{
    //设置焦点选中类型
    setFocusPolicy(Qt::FocusPolicy::TabFocus);
}

LabelWidget::~LabelWidget()
{

}

void LabelWidget::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    if (hasFocus()) {
        //有焦点，绘制焦点
        QStyleOptionFocusRect option;
        option.initFrom(this);
        option.backgroundColor = palette().color(QPalette::Background);
        style()->drawPrimitive(QStyle::PE_FrameFocusRect, &option, &painter, this);
    }
    QLabel::paintEvent(ev);
}
