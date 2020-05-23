#ifndef DRAGINFOITEM_H
#define DRAGINFOITEM_H
#include <QGraphicsRectItem>
#include <DFontSizeManager>

#include "scheduledatamanage.h"

DWIDGET_USE_NAMESPACE

class QPropertyAnimation;
class QSequentialAnimationGroup;


class DragInfoItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
    Q_PROPERTY(int offset WRITE setOffset)
public:
    explicit DragInfoItem(QRect rect, QGraphicsItem *parent = nullptr);
    ~DragInfoItem() override;
    void setData(const ScheduleDtailInfo  &vScheduleInfo);
    ScheduleDtailInfo getData() const;

    void setFont(DFontSizeManager::SizeType type);
    void setOffset(const int &offset);
    void setStartValue(const int value);
    void setEndValue(const int value);
    void startAnimation();
    bool isRunning()const
    {
        return m_isAnimation;
    }
public:
    static void setPressFlag(const bool flag);
public slots:
    void animationFinished();
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
protected:
    virtual void paintBackground(QPainter *painter,const QRectF &rect,const int isPixMap = false) =0;
protected:
    ScheduleDtailInfo                   m_vScheduleInfo;
    QFont                               m_font;
    bool                                m_vSelectflag =false;
    bool                                m_vHoverflag = false;
    bool                                m_vHighflag = false;
    QRect                               m_rect;
    bool                                m_isAnimation = false;
    DFontSizeManager::SizeType          m_sizeType  = DFontSizeManager::T8;
    QPropertyAnimation                  *m_properAnimationFirst;
    QPropertyAnimation                  *m_properAnimationSecond;
    QSequentialAnimationGroup           *m_Group;
    static bool                          m_press;
};

#endif // DRAGINFOITEM_H
