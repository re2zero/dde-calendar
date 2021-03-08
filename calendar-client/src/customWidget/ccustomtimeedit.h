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
#ifndef CCUSTOMTIMEEDIT_H
#define CCUSTOMTIMEEDIT_H

#include <QDateTimeEdit>

/**
 * @brief The CCustomTimeEdit class
 */
class CCustomTimeEdit : public QTimeEdit
{
    Q_OBJECT
public:
    explicit CCustomTimeEdit(QWidget *parent = nullptr);
    //获取编辑框
    QLineEdit *getLineEdit();
protected:
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
signals:
    void signalUpdateFocus(bool showFocus);
};

#endif // CCUSTOMTIMEEDIT_H
