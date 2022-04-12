#include "jobtypelistview.h"
#include "cscheduleoperation.h"
#include "scheduletypeeditdlg.h"

#include <DHiDPIHelper>
#include <DStyle>
#include <DIconButton>

#include <QPainter>

Q_DECLARE_METATYPE(JobTypeInfo)
//Qt::UserRole + 1,会影响item的高度
static const int RoleJobTypeInfo = Qt::UserRole + 2;

JobTypeListView::JobTypeListView(QWidget *parent) : DListView(parent)
{
    initUI();
}

JobTypeListView::~JobTypeListView()
{
}

void JobTypeListView::initUI()
{
    setIconSize(QSize(16, 16));
    setContentsMargins(0, 0, 0, 0);
    setSpacing(0);
    setItemSpacing(10);

    m_modelJobType = new QStandardItemModel();
    setModel(m_modelJobType);
    setEditTriggers(QListView::NoEditTriggers);
    setSelectionMode(QListView::NoSelection);

    //
    updateJobType();
}

bool JobTypeListView::viewportEvent(QEvent *event)
{
    DListView::viewportEvent(event);

    int indexCurrentHover;

    if (QEvent::HoverLeave == event->type()) {
        QStandardItemModel *itemModel = qobject_cast<QStandardItemModel *>(model());
        if (nullptr == itemModel) {
            return true;
        }
        DStandardItem *itemJobType = dynamic_cast<DStandardItem *>(itemModel->item(m_iIndexCurrentHover));

        if (nullptr == itemJobType) {
            return true;
        }
        //typedef QList<DViewItemAction *> DViewItemActionList;
        for (DViewItemAction *a : itemJobType->actionList(Qt::Edge::RightEdge)) {
            a->setVisible(false);
        }
        m_iIndexCurrentHover = -1;
    } else if (QEvent::HoverMove == event->type()) {
        indexCurrentHover = indexAt(static_cast<QHoverEvent *>(event)->pos()).row();
        if (indexCurrentHover != m_iIndexCurrentHover) {
            DStandardItem *itemJobType;

            QStandardItemModel *itemModel = qobject_cast<QStandardItemModel *>(model());
            if (nullptr == itemModel) {
                return true;
            }
            //隐藏此前鼠标悬浮行的图标
            if (m_iIndexCurrentHover >= 0) {
                itemJobType = dynamic_cast<DStandardItem *>(itemModel->item(m_iIndexCurrentHover));
                if (nullptr == itemJobType) {
                    return true;
                }
                //typedef QList<DViewItemAction *> DViewItemActionList;
                for (DViewItemAction *a : itemJobType->actionList(Qt::Edge::RightEdge)) {
                    a->setVisible(false);
                }
            }

            if (indexCurrentHover < 0) {
                return true;
            }
            //展示此前鼠标悬浮行的图标
            m_iIndexCurrentHover = indexCurrentHover;
            //qInfo() << "HoverMove" << indexAt(static_cast<QHoverEvent*>(event)->pos());
            itemJobType = static_cast<DStandardItem *>(itemModel->item(m_iIndexCurrentHover));
            if (nullptr == itemJobType) {
                return true;
            }
            if (itemJobType->actionList(Qt::Edge::RightEdge).size() > 0) {
                for (DViewItemAction *a : itemJobType->actionList(Qt::Edge::RightEdge)) {
                    a->setVisible(true);
                }
            } else {
                // 设置其他style时，转换指针为空
                if (DStyle *ds = qobject_cast<DStyle *>(style())) {
                    auto actionEdit = new DViewItemAction(Qt::AlignVCenter, QSize(), QSize(), true);
                    actionEdit->setIcon(ds->standardIcon(DStyle::SP_AddButton));
                    actionEdit->setIcon(DHiDPIHelper::loadNxPixmap(":/resources/icon/edit.svg"));
                    actionEdit->setParent(this);
                    connect(actionEdit, &QAction::triggered, this, &JobTypeListView::slotUpdateJobType);

                    auto actionDelete = new DViewItemAction(Qt::AlignVCenter, QSize(), QSize(), true);
                    actionDelete->setIcon(ds->standardIcon(DStyle::SP_AddButton));
                    actionDelete->setIcon(DHiDPIHelper::loadNxPixmap(":/resources/icon/delete.svg"));
                    actionDelete->setParent(this);
                    connect(actionDelete, &QAction::triggered, this, &JobTypeListView::slotDeleteJobType);

                    itemJobType->setActionList(Qt::Edge::RightEdge, {actionEdit, actionDelete});
                }
            }
        }
    }
    return true;
}
bool JobTypeListView::updateJobType()
{
    m_modelJobType->clear();//先清理
    QString strColorHex;
    QString strJobType;
    QList<JobTypeInfo> lstJobType = JobTypeInfoManager::instance()->getJobTypeList();
    for (int i = 0;i < lstJobType.size();i++) {
        strColorHex = lstJobType[i].getColorHex();
        strJobType = lstJobType[i].getJobTypeName();
        if(strColorHex.isEmpty() || strJobType.isEmpty()){
            continue;
        }
        addJobTypeItem(lstJobType[i]);
    }

    setFixedHeight(m_modelJobType->rowCount() * (36 + 10 + 3));//默认是高度36，space：10

    emit signalAddStatusChanged(canAdd());
    return true;
}

bool JobTypeListView::canAdd()
{
    //最多20个类型
    return m_modelJobType->rowCount() < 20;

}

void JobTypeListView::addJobTypeItem(const JobTypeInfo &info)
{
    QSize size(24, 24);
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing );
    painter.setBrush(QColor(info.getColorHex()));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(0, 0, 24, 24, 12, 12);//圆

    DStandardItem *item = new DStandardItem(QIcon(pixmap), info.getJobTypeName());
    item->setData(QVariant::fromValue(info), RoleJobTypeInfo);

    m_modelJobType->appendRow(item);
}

void JobTypeListView::slotUpdateJobType()
{
    int index =  indexAt(mapFromGlobal(QCursor::pos())).row();
    if(index < 0 || index >= m_modelJobType->rowCount())
        return;
    QStandardItem *item = m_modelJobType->item(index);
    if(!item)
        return;

    JobTypeInfo info = item->data(RoleJobTypeInfo).value<JobTypeInfo>();
    ScheduleTypeEditDlg a(info);
    a.exec();
    updateJobType();//更新item
    return;
}

void JobTypeListView::slotDeleteJobType()
{
    int index =  indexAt(mapFromGlobal(QCursor::pos())).row();
    if(index < 0 || index >= m_modelJobType->rowCount())
        return;
    QStandardItem *item = m_modelJobType->item(index);
    if(!item)
        return;

    JobTypeInfo info = item->data(RoleJobTypeInfo).value<JobTypeInfo>();
    int colorTypeNo = info.getColorTypeNo();
    int typeNo = info.getJobTypeNo();

    CScheduleOperation so;
    so.deleteJobType(typeNo);
    if(!JobTypeInfoManager::instance()->isSysJobTypeColor(colorTypeNo)){
        //不是默认颜色类型，需要删除颜色类型
        so.deleteColorType(colorTypeNo);
    }
    updateJobType();//更新item
}
