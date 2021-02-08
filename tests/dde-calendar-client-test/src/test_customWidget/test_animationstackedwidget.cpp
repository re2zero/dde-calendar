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
#include "test_animationstackedwidget.h"

test_animationstackedwidget::test_animationstackedwidget()
{
    mAnimationStackedWidget = new AnimationStackedWidget();
}

test_animationstackedwidget::~test_animationstackedwidget()
{
    delete mAnimationStackedWidget;
    mAnimationStackedWidget = nullptr;
}

//void AnimationStackedWidget::setDuration(int duration)
TEST_F(test_animationstackedwidget, setDuration)
{
    mAnimationStackedWidget->setDuration(400);
}

//void AnimationStackedWidget::animationFinished()
TEST_F(test_animationstackedwidget, animationfinished)
{
    mAnimationStackedWidget->animationFinished();
}

//void AnimationStackedWidget::setCurrent(int index)
TEST_F(test_animationstackedwidget, setCurrent)
{
    mAnimationStackedWidget->setCurrent(4);
}

//void AnimationStackedWidget::setPre()
TEST_F(test_animationstackedwidget, setPre)
{
    mAnimationStackedWidget->setPre();
}

//void AnimationStackedWidget::setNext()
TEST_F(test_animationstackedwidget, setNext)
{
    mAnimationStackedWidget->setNext();
}

//void AnimationStackedWidget::setCurrentWidget(int &index, int beginWidth)
TEST_F(test_animationstackedwidget, setCurrentWidget)
{
    int index = 2;
    mAnimationStackedWidget->setCurrentWidget(index, 8);
}

//int AnimationStackedWidget::getBeginValue()
TEST_F(test_animationstackedwidget, getBeginValue)
{
    mAnimationStackedWidget->getBeginValue();
}

//AnimationStackedWidget::MoveOrientation AnimationStackedWidget::getMoveOrientation(const int currIndex, const int nextIndex)
TEST_F(test_animationstackedwidget, getMoveOrientation)
{
    mAnimationStackedWidget->getMoveOrientation(2, 4);
}

//double AnimationStackedWidget::offset() const
TEST_F(test_animationstackedwidget, offset)
{
    mAnimationStackedWidget->offset();
}

//void AnimationStackedWidget::setOffset(double offset)
TEST_F(test_animationstackedwidget, setOffset)
{
    mAnimationStackedWidget->setOffset(2.0);
}

