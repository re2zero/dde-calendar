/*
* Copyright (C) 2019 ~ 2019 UnionTech Software Technology Co.,Ltd.
*
* Author:     changze <changze@uniontech.com>
*
* Maintainer: changze <changze@uniontech.com>
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
#ifndef LUNARCALENDARWIDGET_H
#define LUNARCALENDARWIDGET_H

#include <QCalendarWidget>
class QLabel;
class CalenderStyle;

class LunarCalendarWidget : public QCalendarWidget
{
    Q_OBJECT
public:
    explicit LunarCalendarWidget(QWidget *parent = nullptr);
    virtual ~LunarCalendarWidget();

    /**
     * @brief setLunarYearText 设置阴历年描述语
     * @param text
     */
    void setLunarYearText(const QString &text);
    /**
     * @brief lunarYearText 返回阴历年描述语
     * @return
     */
    QString lunarYearText();

protected:
    virtual QSize minimumSizeHint() const override;

private:
    QLabel *m_lunarLabel   = nullptr;//阴历年的label
    CalenderStyle *m_style = nullptr;//阴历相关样式
};

#endif // LUNARCALENDARWIDGET_H
