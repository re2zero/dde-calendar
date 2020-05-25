#include "draginfoitem.h"

#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QDebug>

bool DragInfoItem::m_press = false;
ScheduleDtailInfo DragInfoItem::m_HoverInfo;
DragInfoItem::DragInfoItem(QRect rect, QGraphicsItem *parent)
    :QGraphicsRectItem(parent),
     m_rect(rect)
{
    setRect(m_rect);
    setAcceptHoverEvents(true);

    const int duration = 200;
    m_properAnimationFirst = new QPropertyAnimation( this, "offset", this);
    m_properAnimationFirst->setObjectName("First");
    m_properAnimationSecond = new QPropertyAnimation( this, "offset", this);
    m_properAnimationSecond->setObjectName("Second");
    m_properAnimationFirst->setDuration(duration);
    m_properAnimationSecond->setDuration(duration);
    m_properAnimationFirst->setEasingCurve(QEasingCurve::InOutQuad);
    m_properAnimationSecond->setEasingCurve(QEasingCurve::InOutQuad);
    m_Group = new QSequentialAnimationGroup(this);
    m_Group->addAnimation(m_properAnimationFirst);
    m_Group->addAnimation(m_properAnimationSecond);
    connect(m_Group
            , &QPropertyAnimation::finished
            , this
            , &DragInfoItem::animationFinished);


}

DragInfoItem::~DragInfoItem()
{

}

void DragInfoItem::setData(const ScheduleDtailInfo &vScheduleInfo)
{
    m_vScheduleInfo = vScheduleInfo;
}

ScheduleDtailInfo DragInfoItem::getData() const
{
    return  m_vScheduleInfo;
}

void DragInfoItem::setPressFlag(const bool flag)
{
    m_press = flag;
}

void DragInfoItem::setFont(DFontSizeManager::SizeType type)
{
    m_sizeType = type;
}

void DragInfoItem::setOffset(const int &offset)
{
    setRect(QRect(m_rect.x() - offset,
                  m_rect.y() - offset / 2,
                  m_rect.width() + offset * 2,
                  m_rect.height() + offset));
    setZValue(offset);
}


void DragInfoItem::setStartValue(const int value)
{
    m_properAnimationFirst->setStartValue(value);
    m_properAnimationSecond->setEndValue(value);
}

void DragInfoItem::setEndValue(const int value)
{
    m_properAnimationFirst->setEndValue(value);
    m_properAnimationSecond->setStartValue(value);

}

void DragInfoItem::startAnimation()
{
    if (m_Group->state() != QAnimationGroup::Running) {
        m_Group->start();
    }
}

void DragInfoItem::animationFinished()
{
    m_isAnimation = false;
}

void DragInfoItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    m_HoverInfo = m_vScheduleInfo;
    update();
}

void DragInfoItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    m_HoverInfo = ScheduleDtailInfo();
    update();
}


void DragInfoItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    m_vHoverflag = m_HoverInfo == m_vScheduleInfo;
    paintBackground(painter,this->rect());
}
