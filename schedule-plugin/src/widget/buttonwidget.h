/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     chenhaifeng  <chenhaifeng@uniontech.com>
*
* Maintainer: chenhaifeng  <chenhaifeng@uniontech.com>
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
#ifndef BUTTONWIDGET_H
#define BUTTONWIDGET_H
#include <DWidget>
#include <QAbstractButton>
#include <QHBoxLayout>
#include <QObject>

DWIDGET_USE_NAMESPACE

namespace BTWidget {
const int BUTTON_LAYOUT_TOP_MARGIN = 0;
const int BUTTON_LAYOUT_BOTTOM_MARGIN = 0;
const int BUTTON_LAYOUT_LEFT_MARGIN = 0;
const int BUTTON_LAYOUT_RIGHT_MARGIN = 0;
} // namespace BTWidget

class buttonwidget : public DWidget
{
    Q_OBJECT
public:
    explicit buttonwidget(QWidget *parent = nullptr);

    enum ButtonType {
        ButtonNormal,
        ButtonWarning,
        ButtonRecommend
    };
    //将isDefault设置为true，保证有焦点时，点击enter可以选中
    int addbutton(const QString &text, bool isDefault = true, ButtonType type = ButtonNormal);
    void insertButton(int index, const QString &text, bool isDefault = true, ButtonType type = ButtonNormal);
    void insertButton(int index, QAbstractButton *button, bool isDefault = true);

    int buttonCount() const;

signals:
    void buttonClicked(int index, const QString &text);
public slots:
    void onButtonClicked(bool clicked);

private:
    QList<QAbstractButton *> buttonList;
    QSpacerItem *spacer = nullptr;

    QHBoxLayout *buttonLayout;
    int clickedButtonIndex;
};

#endif // BUTTONWIDGET_H
