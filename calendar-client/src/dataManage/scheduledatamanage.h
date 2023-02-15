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
#ifndef SCHEDULEDATAMANAGE_H
#define SCHEDULEDATAMANAGE_H

#include <DGuiApplicationHelper>

#include <QThread>
#include <QDate>
#include <QMutex>

DGUI_USE_NAMESPACE
struct CSchedulesColor {
    int type;
    QColor gradientFromC;
    QColor gradientToC;
    QColor dotColor;
    QColor Purecolor;
    QColor shadowcolor;
    QColor textColor;
    QColor timeColor;
    QColor hightlightgradientFromC;
    QColor hightlightgradientToC;
    QColor hightlightPurecolor;
    QColor hovergradientFromC;
    QColor hovergradientToC;
    QColor hoverPurecolor;
    QColor splitColor;
    QColor pressgradientFromC;
    QColor pressgradientToC;
    QColor pressPurecolor;
};

class CScheduleDataManage
{
public:
    static CScheduleDataManage *getScheduleDataManage();
    CSchedulesColor getScheduleColorByType(int type);
    static QColor getSystemActiveColor();
    void setTheMe(int type = 0);
    int getTheme() const
    {
        return m_theme;
    }
private:
    CScheduleDataManage();
    ~CScheduleDataManage();

private:
    QVector<CSchedulesColor> m_vScheduleColor;
    int m_theme = 0;
    static CScheduleDataManage *m_vscheduleDataManage;
};
#endif // SCHEDULEVIEW_H
