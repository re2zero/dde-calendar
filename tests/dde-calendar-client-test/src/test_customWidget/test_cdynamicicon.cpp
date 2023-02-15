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
#include "test_cdynamicicon.h"

test_cdynamicicon::test_cdynamicicon()
{
    mDynamicicon = new CDynamicIcon(8, 8);
}

test_cdynamicicon::~test_cdynamicicon()
{
    delete mDynamicicon;
    mDynamicicon = nullptr;
}

//void CDynamicIcon::setDate(const QDate &date)
TEST_F(test_cdynamicicon, setDate)
{
    mDynamicicon->setDate(QDate::currentDate());
}

//void CDynamicIcon::setTitlebar(DTitlebar *titlebar)
TEST_F(test_cdynamicicon, setTitlebar)
{
    DTitlebar *titlebar = new DTitlebar();
    mDynamicicon->setTitlebar(titlebar);
    EXPECT_EQ(mDynamicicon->m_Titlebar, titlebar);
    delete titlebar;
}

