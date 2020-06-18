#ifndef DRAGINFOGRAPHICSVIEW_H
#define DRAGINFOGRAPHICSVIEW_H
#include <DGraphicsView>
#include <QDate>
#include <QDrag>


#include "scheduledatamanage.h"
#include "draginfoitem.h"

DWIDGET_USE_NAMESPACE

class DragInfoGraphicsView : public DGraphicsView
{
    Q_OBJECT
public:
    explicit DragInfoGraphicsView(DWidget *parent = nullptr);
    ~DragInfoGraphicsView() override;
public:
//鼠标位置
    enum PosInItem {LEFT,MIDDLE,RIGHT,TOP,BOTTOM};
    //鼠标移动状态
    enum DragStatus {IsCreate,ChangeBegin,ChangeEnd,ChangeWhole,NONE};

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    bool event(QEvent *e) override;
private:
    void slotCreate();
    void setPressSelectInfo(const ScheduleDtailInfo &info);
    void updateScheduleInfo(const ScheduleDtailInfo &info);
    void DragPressEvent(const QPoint &pos,DragInfoItem *item);
    void mouseReleaseScheduleUpdate();
protected:
    void DeleteItem(const ScheduleDtailInfo &info);
protected:
    virtual void setDragPixmap(QDrag *drag,DragInfoItem *item);
    virtual void slotCreate(const QDateTime &date);
    //符合创建条件
    virtual bool MeetCreationConditions(const QDateTime &date) =0;
    virtual void upDateInfoShow(const DragStatus &status = NONE,const ScheduleDtailInfo &info =ScheduleDtailInfo()) =0;
    virtual QDateTime getPosDate(const QPoint &p) =0;
    virtual void MoveInfoProcess(ScheduleDtailInfo &info,const QPointF &pos) = 0;
    virtual PosInItem getPosInItem(const QPoint &p,const QRectF &itemRect) =0;
    virtual ScheduleDtailInfo getScheduleInfo(const QDateTime &beginDate,const QDateTime &endDate);
    virtual void ShowSchedule(DragInfoItem *infoitem);
    //设置主题
    virtual void setTheMe(int type = 0) =0;
    //判断时间是否相等
    virtual bool IsEqualtime(const QDateTime &timeFirst,const QDateTime &timeSecond) =0;
    //根据鼠标移动的距离判断是否创建日程
    virtual bool JudgeIsCreate(const QPointF &pos) =0;
    virtual void RightClickToCreate(QGraphicsItem *listItem,const QPoint &pos) =0;
    //
    virtual QDateTime getDragScheduleInfoBeginTime(const QDateTime &moveDateTime) =0;
    //
    virtual QDateTime getDragScheduleInfoEndTime(const QDateTime &moveDateTime) =0;
signals:
    //更新获取日程信息
    void signalsUpdateShcedule();
    void signalViewtransparentFrame(const int id = 0);
    void signalScheduleShow(const bool isShow, const ScheduleDtailInfo &out = ScheduleDtailInfo());
protected:
    int                                 m_themetype = 0;
    QGraphicsScene                      *m_Scene;
    bool                                m_press = false;

    QAction                             *m_createAction;     // 创建日程
    QAction                             *m_editAction;
    QAction                             *m_deleteAction;
    DMenu                               *m_rightMenu;

    QDateTime                           m_createDate;
    DragStatus                          m_DragStatus =NONE;
    bool                                m_isCreate;
    QDateTime                           m_PressDate;
    QDateTime                           m_MoveDate;
    QPoint                              m_PressPos;
    ScheduleDtailInfo                   m_DragScheduleInfo;
    QDateTime                           m_InfoBeginTime;
    QDateTime                           m_InfoEndTime;
    QDrag                               *m_Drag;
    //点击的原始info
    ScheduleDtailInfo                   m_PressScheduleInfo;
    QRectF                              m_PressRect;
};

#endif // DRAGINFOGRAPHICSVIEW_H
