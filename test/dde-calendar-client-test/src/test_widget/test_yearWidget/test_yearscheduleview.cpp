/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     hejinghai <hejinghai@uniontech.com>
*
* Maintainer: hejinghai <hejinghai@uniontech.com>
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
#include "test_yearscheduleview.h"
#include "constants.h"
#include <QDebug>

test_yearscheduleview::test_yearscheduleview()
{
    xYearScheduleItem = new CYearScheduleItem();
    cYearScheduleView = new CYearScheduleView();
    zYearScheduleOutView = new CYearScheduleOutView();
}

test_yearscheduleview::~test_yearscheduleview()
{
    delete xYearScheduleItem;
    xYearScheduleItem = nullptr;
    delete cYearScheduleView;
    cYearScheduleView = nullptr;
    delete zYearScheduleOutView;
    zYearScheduleOutView = nullptr;
}

//void CYearScheduleItem::setBackgroundColor(QColor color1)
TEST_F(test_yearscheduleview, setBackgroundColor)
{
    QColor color1(240, 100, 100);
    xYearScheduleItem->setBackgroundColor(color1);
}

//void CYearScheduleItem::setStateColor(QColor color1)
TEST_F(test_yearscheduleview, setStateColor)
{
    QColor color1(240, 100, 100);
    xYearScheduleItem->setStateColor(color1);
}

//void CYearScheduleItem::setText(QColor tcolor, QFont font)
TEST_F(test_yearscheduleview, setText)
{
    QColor tcolor(140, 140, 120);
    QFont font;
    font.setWeight(QFont::Medium);
    font.setPixelSize(DDECalendar::FontSizeTwelve);
    xYearScheduleItem->setText(tcolor, font);
}

//void CYearScheduleItem::setTimeC(QColor tcolor, QFont font)
TEST_F(test_yearscheduleview, setTimeC)
{
    QColor tcolor(140, 140, 120);
    QFont font;
    font.setWeight(QFont::Medium);
    font.setPixelSize(DDECalendar::FontSizeTwelve);
    xYearScheduleItem->setTimeC(tcolor, font);
}

//void CYearScheduleItem::setData(ScheduleDataInfo vScheduleInfo)
TEST_F(test_yearscheduleview, setData)
{

}
