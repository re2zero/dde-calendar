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
#ifndef CMONTHDAYITEM_H
#define CMONTHDAYITEM_H

#include "cscenebackgrounditem.h"

#include <QDate>
#include <QFont>

/**
 * @brief The CMonthDayItem class
 *  月视图绘制天item
 */
class CMonthDayItem : public CSceneBackgroundItem
{
    Q_OBJECT
public:
    enum HolidayStatus { H_NONE = 0x00,
                         H_REST = 0x01,
                         H_WORK = 0x02 };
    explicit CMonthDayItem(QGraphicsItem *parent = nullptr);
    ~CMonthDayItem() override;
    //设置农历信息
    void setLunar(const QString &lunar);
    //设置班休状态
    void setStatus(const HolidayStatus &status);
    //设置是否为当前选择月份
    void setCurrentMonth(bool isCurrent)
    {
        m_IsCurrentMonth = isCurrent;
    }
    //设置主题
    void setTheMe(int type = 0);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    QString m_DayLunar;
    HolidayStatus m_DayStatus;
    QFont m_dayNumFont;
    QColor m_dayNumColor;
    QColor m_dayNumCurrentColor;
    QFont m_LunerFont;
    QColor m_LunerColor;
    QColor m_BorderColor;
    bool m_IsCurrentMonth = false;
    QColor m_fillColor = Qt::white;
    QColor m_banColor = "#FBE9B7";
    QColor m_xiuColor = "#D4FFB3";
    int m_themetype = 0;
    QColor m_currentColor;
    const int m_radius = 18;

public:
    static bool m_LunarVisible;
};

#endif // CMONTHDAYITEM_H
