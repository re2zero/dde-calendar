/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     leilong  <leilong@uniontech.com>
*
* Maintainer: leilong  <leilong@uniontech.com>
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
#ifndef CPUSHBUTTON_H
#define CPUSHBUTTON_H

#include <DIconButton>

#include <QLabel>

DWIDGET_USE_NAMESPACE

class CPushButton : public QWidget
{
    Q_OBJECT
public:
    explicit CPushButton(QWidget *parent = nullptr);

    //设置高亮状态
    void setHighlight(bool status);
    //返回高亮状态
    bool isHighlight();

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    bool m_Highlighted = false; //记录当前是否处于高亮状态
    bool m_pressed = false; //记录鼠标是否按下
    QLabel *m_textLabel = nullptr;  //文字控件
    DIconButton *m_iconButton = nullptr; //icon控件
};

#endif // CPUSHBUTTON_H
