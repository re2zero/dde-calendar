/*
* Copyright (C) 2019 ~ 2021 Uniontech Software Technology Co.,Ltd.
*
* Author:     zhengxiaokang <zhengxiaokang@uniontech.com>
*
* Maintainer: zhengxiaokang <zhengxiaokang@uniontech.com>
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
#include "test_touchgestureoperation.h"

test_touchgestureoperation::test_touchgestureoperation()
{
    mTouchGestureOperation = new touchGestureOperation();
}

test_touchgestureoperation::~test_touchgestureoperation()
{
    delete mTouchGestureOperation;
    mTouchGestureOperation = nullptr;
}

//void touchGestureOperation::setUpdate(bool b)
TEST_F(test_touchgestureoperation, setUpdate)
{
    mTouchGestureOperation->setUpdate(true);
}

//bool touchGestureOperation::isUpdate() const
TEST_F(test_touchgestureoperation, isUpdate)
{
    mTouchGestureOperation->setUpdate(false);
    bool res = mTouchGestureOperation->isUpdate();
    EXPECT_FALSE(res);
    mTouchGestureOperation->setUpdate(true);
    res = mTouchGestureOperation->isUpdate();
    EXPECT_TRUE(res);
}

//touchGestureOperation::TouchState touchGestureOperation::getTouchState() const
TEST_F(test_touchgestureoperation, getTouchState)
{
    mTouchGestureOperation->getTouchState();
}

//touchGestureOperation::TouchMovingDirection touchGestureOperation::getMovingDir() const
TEST_F(test_touchgestureoperation, getMovingDir)
{
    mTouchGestureOperation->getMovingDir();
}

//touchGestureOperation::TouchMovingDirection touchGestureOperation::getTouchMovingDir(QPointF &startPoint, QPointF &stopPoint, qreal &movingLine)
TEST_F(test_touchgestureoperation, getTouchMovingDir)
{
    QPointF point1;
    QPointF point2;
    qreal num = 2;
    mTouchGestureOperation->getTouchMovingDir(point1, point2, num);
}

//void touchGestureOperation::calculateAzimuthAngle(QPointF &startPoint, QPointF &stopPoint)
TEST_F(test_touchgestureoperation, calculateAzimuthAngle)
{
    QPointF point1;
    QPointF point2;
    mTouchGestureOperation->calculateAzimuthAngle(point1, point2);
}
