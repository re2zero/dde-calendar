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
#include "test_customframe.h"

test_customframe::test_customframe()
{
    mCustomFrame = new CustomFrame();
}

test_customframe::~test_customframe()
{
    delete mCustomFrame;
    mCustomFrame = nullptr;
}

//void CustomFrame::setBColor(QColor normalC)
TEST_F(test_customframe, setBcolor)
{
    mCustomFrame->setBColor(Qt::red);
}

//void CustomFrame::setRoundState(bool lstate, bool tstate, bool rstate, bool bstate)
TEST_F(test_customframe, setRoundState)
{
    mCustomFrame->setRoundState(true, true, true, true);
}

//void CustomFrame::setTextStr(QFont font, QColor tc, QString strc, int flag)
TEST_F(test_customframe, setTextStr)
{
    QFont font;
    mCustomFrame->setTextStr(font, Qt::red, "hello", 1);
}

//void CustomFrame::setTextStr(QString strc)
TEST_F(test_customframe, setTextstr)
{
    mCustomFrame->setTextStr("hello");
}

//void CustomFrame::setTextColor(QColor tc)
TEST_F(test_customframe, setTextColor)
{
    mCustomFrame->setTextColor(Qt::red);
}

//void CustomFrame::setTextFont(QFont font)
TEST_F(test_customframe, setTextFont)
{
    QFont font;
    mCustomFrame->setTextFont(font);
}

//void CustomFrame::setTextAlign(int flag)
TEST_F(test_customframe, setTextAlign)
{
    mCustomFrame->setTextAlign(2);
}

//void CustomFrame::setRadius(int radius)
TEST_F(test_customframe, setRadius)
{
    mCustomFrame->setRadius(4);
}

//void CustomFrame::setboreder(int framew)
TEST_F(test_customframe, setboreder)
{
    mCustomFrame->setboreder(4);
}

//void CustomFrame::setFixedSize(int w, int h)
TEST_F(test_customframe, setFixedSize)
{
    mCustomFrame->setFixedSize(40, 44);
}

