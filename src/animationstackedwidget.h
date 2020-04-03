#ifndef AnimationStackedWidget_H
#define AnimationStackedWidget_H

#include <QStackedWidget>
#include <QStackedLayout>
#include <QPropertyAnimation>

class AnimationStackedWidget : public QStackedWidget
{
    Q_OBJECT
    enum MoveOrientation {LeftToRight, RightToLeft, TopToBottom, BottomToTop};
public:
    enum AnimationOri {LR, TB}; //LR:Left Right  ,TB:Top  Bottom
public:
    explicit AnimationStackedWidget(const AnimationOri ori = LR, QWidget *parent = nullptr);
    ~AnimationStackedWidget() Q_DECL_OVERRIDE;
    //设置动画持续的间隔
    void setDuration(int duration = 500);
    bool IsRunning()const
    {
        return isAnimation;
    }
protected:
    void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;
signals:
    void signalIsFinished();
public slots:
    //属性动画值改变的槽
    void valueChanged_slot(QVariant );
    //动画切换完成
    void animationFinished();
    //设置当前页
    void setCurrent(int index);
    void setPre();
    void setNext();
private:
    void paintPrevious(QPainter &, int);
    void paintNext(QPainter &, int);
private:
    AnimationOri     m_animationOri;
    QPropertyAnimation *animation;
    int duration;
    bool isAnimation = false;
    QVariant       currentValue;
    int         widgetCount;
    int         nextIndex;
    MoveOrientation  m_moveOri = MoveOrientation::LeftToRight;

};

#endif // AnimationStackedWidget_H
