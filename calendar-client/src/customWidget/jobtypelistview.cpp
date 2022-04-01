#include "jobtypelistview.h"

#include <DHiDPIHelper>
#include <DStyle>

#include <QPainter>

JobTypeListView::JobTypeListView(QWidget *parent) : DListView(parent)
{
    initUI();
}
void JobTypeListView::initUI()
{
    setIconSize(QSize(16, 16));
    setContentsMargins(0, 0, 0, 0);
    setSpacing(0);
    setItemSpacing(10);
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_modelJobType = new QStandardItemModel();
    setModel(m_modelJobType);
    addJobType();
}

bool JobTypeListView::viewportEvent(QEvent *event)
{
    DListView::viewportEvent(event);

    int indexCurrentHover;

    if(QEvent::HoverLeave == event->type()){
        QStandardItemModel *itemModel = qobject_cast<QStandardItemModel*>(model());
        if(nullptr == itemModel){
            return true;
        }
        DStandardItem *itemJobType = dynamic_cast<DStandardItem *>(itemModel->item(m_iIndexCurrentHover));

        if(nullptr == itemJobType){
            return true;
        }
        //typedef QList<DViewItemAction *> DViewItemActionList;
        for(DViewItemAction* a : itemJobType->actionList(Qt::Edge::RightEdge)){
            a->setVisible(false);
        }
        m_iIndexCurrentHover = -1;
    }
    else if(QEvent::HoverMove == event->type()){
        indexCurrentHover = indexAt(static_cast<QHoverEvent*>(event)->pos()).row();
        if(indexCurrentHover != m_iIndexCurrentHover){
            DStandardItem *itemJobType;

            QStandardItemModel *itemModel = qobject_cast<QStandardItemModel*>(model());
            if(nullptr == itemModel){
                return true;
            }
            //隐藏此前鼠标悬浮行的图标
            if(m_iIndexCurrentHover >= 0){
                itemJobType = dynamic_cast<DStandardItem *>(itemModel->item(m_iIndexCurrentHover));
                if(nullptr == itemJobType){
                    return true;
                }
                //typedef QList<DViewItemAction *> DViewItemActionList;
                for(DViewItemAction* a : itemJobType->actionList(Qt::Edge::RightEdge)){
                    a->setVisible(false);
                }
            }

            if(indexCurrentHover < 0){
                return true;
            }
            //展示此前鼠标悬浮行的图标
            m_iIndexCurrentHover = indexCurrentHover;
            //qInfo() << "HoverMove" << indexAt(static_cast<QHoverEvent*>(event)->pos());
            itemJobType = static_cast<DStandardItem *>(itemModel->item(m_iIndexCurrentHover));
            if(nullptr == itemJobType){
                return true;
            }
            if(itemJobType->actionList(Qt::Edge::RightEdge).size() > 0){
                for(DViewItemAction* a : itemJobType->actionList(Qt::Edge::RightEdge)){
                    a->setVisible(true);
                }
            }
            else {
                // 设置其他style时，转换指针为空
                if (DStyle *ds = qobject_cast<DStyle *>(style())) {
                    auto actionEdit = new DViewItemAction(Qt::AlignVCenter, QSize(), QSize(), true);
                    actionEdit->setIcon(ds->standardIcon(DStyle::SP_AddButton));
                    actionEdit->setIcon(DHiDPIHelper::loadNxPixmap(":/resources/icon/edit.svg"));
                    actionEdit->setParent(this);
                    auto actionDelete = new DViewItemAction(Qt::AlignVCenter, QSize(), QSize(), true);
                    actionDelete->setIcon(ds->standardIcon(DStyle::SP_AddButton));
                    actionDelete->setIcon(DHiDPIHelper::loadNxPixmap(":/resources/icon/delete.svg"));
                    actionDelete->setParent(this);
                    itemJobType->setActionList(Qt::Edge::RightEdge, {actionEdit, actionDelete});
                }
            }
        }
    }
    return true;
}
bool JobTypeListView::addJobType()
{
    QString strColorHex;
    QString strJobType;

    JobTypeInfoManager::instance()->updateInfo();//1.新建日程时更新 2.管理界面更新  =====>>  TODO：2种情况更新即可：1.程序初始化时，更新 2.日程类型增删改时
    m_lstJobType = JobTypeInfoManager::instance()->getJobTypeList();
    for (int i = 0;i < m_lstJobType.size();i++) {
        strColorHex = m_lstJobType[i].getColorHex();
        strJobType = m_lstJobType[i].getJobTypeName();
        if(strColorHex.isEmpty() || strJobType.isEmpty()){
            return false;
        }
        addJobTypeItem(i, m_lstJobType[i].getColorHex(),m_lstJobType[i].getJobTypeName());
    }

    setFixedHeight(m_lstJobType.size() * (36 + 10 + 3));//默认是高度36，space：10
    qInfo() << this->height();
    qInfo() << this->itemMargins();
    return true;
}

void JobTypeListView::addJobTypeItem(int idx, QString strColorHex, QString strJobType)
{
    QSize size(24, 24);
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing );
    painter.setBrush(QColor(strColorHex));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(0, 0, 24, 24, 12, 12);//圆

    DStandardItem *itemJobType = new DStandardItem(QIcon(pixmap), strJobType);

    m_modelJobType->insertRow(idx,itemJobType);
    //connect(this, &QAbstractItemView::mouseMoveEvent, this, [=]{})
}
