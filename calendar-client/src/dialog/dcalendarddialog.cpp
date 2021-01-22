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
#include "dcalendarddialog.h"

#include "constants.h"

DCalendarDDialog::DCalendarDDialog(QWidget *parent)
    : DDialog(parent)
{
}

int DCalendarDDialog::exec()
{
    //移动窗口至顶层窗口中心位置
    moveCentorShow();
    return DDialog::exec();
}

void DCalendarDDialog::mouseMoveEvent(QMouseEvent *event)
{
    //如果为平板模式使其不可移动
    if (DDECalendar::isTable) {
        Q_UNUSED(event);
    } else {
        DDialog::mouseMoveEvent(event);
    }
}

void DCalendarDDialog::keyPressEvent(QKeyEvent *event)
{
    //如果dtk版本在5.3.0以下调用QDialog 以上调用DDialog
#if (DTK_VERSION < DTK_VERSION_CHECK(5, 3, 0, 0))
    return QDialog::keyPressEvent(event);
#else
    return DDialog::keyPressEvent(event);
#endif
}

bool DCalendarDDialog::eventFilter(QObject *o, QEvent *e)
{
    //如果dtk版本在5.3.0以下调用QDialog 以上调用DDialog
#if (DTK_VERSION < DTK_VERSION_CHECK(5, 3, 0, 0))
    return QDialog::eventFilter(o, e);
#else
    return DDialog::eventFilter(o, e);
#endif
}

/**
 * @brief CMySchceduleView::moveCentorShow      在顶层窗口居中显示
 */
void DCalendarDDialog::moveCentorShow()
{
    //需要获取的顶层窗口
    QWidget *_parentWidget = this;
    QWidget *tmpWidget = nullptr;
    do {
        //获取父类对象，如果为qwiget则赋值否则退出
        tmpWidget = qobject_cast<QWidget *>(_parentWidget->parent());
        if (tmpWidget == nullptr) {
            break;
        } else {
            _parentWidget = tmpWidget;
        }
    } while (_parentWidget != nullptr);
    //获取最顶层窗口的中心坐标
    const QPoint global = _parentWidget->mapToGlobal(_parentWidget->rect().center());
    //居中显示
    move(global.x() - width() / 2, global.y() - height() / 2);
}
