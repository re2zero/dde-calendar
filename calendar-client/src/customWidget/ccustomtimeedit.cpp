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
#include "ccustomtimeedit.h"

#include <QMouseEvent>
#include <QLineEdit>
#include <QKeyEvent>

CCustomTimeEdit::CCustomTimeEdit(QWidget *parent)
    : QTimeEdit(parent)
{
    //设置edit最大宽度，不影响其他控件使用
//    setMaximumWidth(100);
    setButtonSymbols(QTimeEdit::NoButtons);
}

/**
 * @brief CCustomTimeEdit::getLineEdit      获取编辑框
 * @return
 */
QLineEdit *CCustomTimeEdit::getLineEdit()
{
    return lineEdit();
}

void CCustomTimeEdit::focusInEvent(QFocusEvent *event)
{
    QTimeEdit::focusInEvent(event);
    emit signalUpdateFocus(true);
}

void CCustomTimeEdit::focusOutEvent(QFocusEvent *event)
{
    QTimeEdit::focusOutEvent(event);
    emit signalUpdateFocus(false);
}

void CCustomTimeEdit::mousePressEvent(QMouseEvent *event)
{
    //设置父类widget焦点
    parentWidget()->setFocus(Qt::TabFocusReason);
    //设置点击位置的光标
    lineEdit()->setCursorPosition(lineEdit()->cursorPositionAt(event->pos()));
    QAbstractSpinBox::mousePressEvent(event);
}

void CCustomTimeEdit::keyPressEvent(QKeyEvent *event)
{
    QTimeEdit::keyPressEvent(event);
    //鼠标左右键,切换光标位置
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Right) {
        parentWidget()->setFocus(Qt::TabFocusReason);
        lineEdit()->setCursorPosition(currentSectionIndex());
    }
}

void CCustomTimeEdit::mouseDoubleClickEvent(QMouseEvent *event)
{
    QTimeEdit::mouseDoubleClickEvent(event);
    //鼠标双击,选中section
    setSelectedSection(currentSection());
}
