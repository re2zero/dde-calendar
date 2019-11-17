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

#ifndef MONTHWEEKVIEW_H
#define MONTHWEEKVIEW_H

#include "constants.h"
#include <DWidget>
#include <QHBoxLayout>
#include <DLabel>
#include <DHorizontalLine>
#include "customframe.h"
DWIDGET_USE_NAMESPACE
class CMonthWeekView : public DWidget
{
    Q_OBJECT
public:
    explicit CMonthWeekView(QWidget *parent = 0);
    void setList(int weekday);
    void setTheMe(int type = 0);
    void updateWeek();
private:
    int checkDay(int weekday);
protected:
    void resizeEvent(QResizeEvent *event);
private:
    QHBoxLayout *m_mainLayout;
    QVector<QPair<CustomFrame *, int> >        m_weekData;
    QVector<DHorizontalLine *>             m_vline;
};

#endif // WEEKINDICATOR_H
