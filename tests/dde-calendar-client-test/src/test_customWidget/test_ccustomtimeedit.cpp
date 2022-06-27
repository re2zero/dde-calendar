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
#include "test_ccustomtimeedit.h"

#include <QTest>
#include <QFocusEvent>

void test_CCustomTimeEdit::SetUp()
{
    m_timeEdit = new CCustomTimeEdit();
}

void test_CCustomTimeEdit::TearDown()
{
    delete m_timeEdit;
    m_timeEdit = nullptr;
}

//mouseEventTest
TEST_F(test_CCustomTimeEdit, mouseEventTest)
{
    QWidget *widget = new QWidget();
    m_timeEdit->setParent(widget);
    QTest::mousePress(m_timeEdit, Qt::LeftButton);
    QTest::mouseRelease(m_timeEdit, Qt::LeftButton);
    QTest::mouseDClick(m_timeEdit, Qt::LeftButton);
    QTest::mouseRelease(m_timeEdit, Qt::LeftButton);

    m_timeEdit->setParent(nullptr);
    delete  widget;
}

TEST_F(test_CCustomTimeEdit,focusInEvent)
{
    QFocusEvent focusEvent_in(QEvent::FocusIn,Qt::FocusReason::ActiveWindowFocusReason);
    QApplication::sendEvent(m_timeEdit,&focusEvent_in);
}

TEST_F(test_CCustomTimeEdit,focusOutEvent)
{
    QFocusEvent focusEvent_out( QEvent::FocusOut,Qt::FocusReason::TabFocusReason);
    QApplication::sendEvent(m_timeEdit,&focusEvent_out);
}

TEST_F(test_CCustomTimeEdit,keyPressEvent)
{
    QWidget *widget = new QWidget();
    m_timeEdit->setParent(widget);
    QKeyEvent keyevent(QEvent::KeyPress,Qt::Key_Left,Qt::NoModifier);
    QApplication::sendEvent(m_timeEdit,&keyevent);
    m_timeEdit->setParent(nullptr);
    delete  widget;
}
