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
#include "test_todaybutton.h"

test_todaybutton::test_todaybutton()
{
    mTodayButton = new CTodayButton();
}

test_todaybutton::~test_todaybutton()
{
    delete mTodayButton;
    mTodayButton = nullptr;
}

//void CTodayButton::setBColor(QColor normalC, QColor hoverC, QColor pressc, QColor normalC1, QColor hoverC1, QColor pressc1)
TEST_F(test_todaybutton, setBColor)
{
    mTodayButton->setBColor("#FFFFFF", "#000000", "#000000", "#FFFFFF", "#000000", "#000000");
}

//void CTodayButton::setTColor(QColor normalC, QColor hoverC, QColor pressc)
TEST_F(test_todaybutton, setTColor)
{
    mTodayButton->setTColor(Qt::red, "#001A2E", "#0081FF");
}

//void CTodayButton::setshadowColor(QColor sc)
TEST_F(test_todaybutton, setshadowColor)
{
    mTodayButton->setshadowColor("#FFFFFF");
}

//
