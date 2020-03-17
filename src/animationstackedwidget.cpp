#include "animationstackedwidget.h"

#include <QPainter>

AnimationStackedWidget::AnimationStackedWidget(QWidget *parent)
    : QStackedWidget(parent)
    , animation(new QPropertyAnimation(this, QByteArray()))
{
    setDuration(1000);
    connect(animation
            , &QPropertyAnimation::valueChanged
            , this
            , &AnimationStackedWidget::valueChanged_slot);
    connect(animation
            , &QPropertyAnimation::finished
            , this
            , &AnimationStackedWidget::animationFinished);
}

AnimationStackedWidget::~AnimationStackedWidget()
{

}

void AnimationStackedWidget::setDuration(int duration)
{
    this->duration = duration;
}


void AnimationStackedWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    if ( isAnimation ) {
        QPainter paint(this);
        paintPrevious(paint, currentIndex());
        paintNext(paint, nextIndex);
    }
}

void AnimationStackedWidget::valueChanged_slot(QVariant value)
{
    currentValue = value;
    update();
}

void AnimationStackedWidget::animationFinished()
{
    isAnimation = false;
    widget(currentIndex())->show();
    setCurrentIndex(nextIndex);
    emit signalIsFinished();
}


void AnimationStackedWidget::setCurrent(int index)
{
    //如果正在动画，那么return
    if ( isAnimation ) {
        return;
    }
    if (index < currentIndex()) {
        m_moveOri = LeftToRight;
    } else if (index > currentIndex()) {
        m_moveOri = RightToLeft;
    } else {
        emit signalIsFinished();
        return;
    }
    isAnimation = true;
    widgetCount = count();
    int c = currentIndex();
    nextIndex = index;

    //隐藏当前的widget
    widget(c)->hide();
    //开始动画并设置间隔和开始、结束值
    QRect g = geometry();
    int width = g.width();
    animation->setStartValue(width);
    animation->setEndValue(0);
    animation->setDuration(duration);
    animation->start();
}

void AnimationStackedWidget::paintPrevious(QPainter &paint, int currentIndex)
{
    //获得当前页面的Widget
    QWidget *w = widget(currentIndex);
    QPixmap pixmap(w->size());
    //将Widget的内容渲染到QPixmap对象中，即将Widget变成一张图片
    w->render(&pixmap);
    QRect r = w->geometry();
    //绘制当前的Widget
    double value = currentValue.toDouble();
    QRectF r1(0.0, 0.0, value, r.height());
    QRectF r2(r.width() - value, 0, value, r.height());
    switch (m_moveOri) {
    case LeftToRight:
        paint.drawPixmap(r2, pixmap, r1);
        break;
    case RightToLeft:
        paint.drawPixmap(r1, pixmap, r2);
        break;

    }
//    paint.drawPixmap(r2, pixmap, r1);

}

void AnimationStackedWidget::paintNext(QPainter &paint, int nextIndex)
{
    QWidget *nextWidget = widget(nextIndex);
    QRect r = geometry();
    //这行代码不加会有bug，第一次切换的时候，QStackedWidget并没有为child分配大小
    nextWidget->resize(r.width(), r.height());
    QPixmap nextPixmap(nextWidget->size());
    nextWidget->render(&nextPixmap);
    double value = currentValue.toDouble();
    QRectF r1(value, 0.0, r.width() - value, r.height());
    QRectF r2(0.0, 0.0, r.width() - value, r.height());
    switch (m_moveOri) {
    case LeftToRight:
        paint.drawPixmap(r2, nextPixmap, r1);
        break;
    case RightToLeft:
        paint.drawPixmap(r1, nextPixmap, r2);
        break;

    }
//    paint.drawPixmap(r2, nextPixmap, r1);
}
