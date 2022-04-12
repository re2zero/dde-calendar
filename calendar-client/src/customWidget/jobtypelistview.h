#ifndef JOBTYPELISTVIEW_H
#define JOBTYPELISTVIEW_H

#include "scheduledatamanage.h"

#include <DListView>

#include <QWidget>
#include <QList>

DWIDGET_USE_NAMESPACE

class JobTypeListView : public DListView
{
    Q_OBJECT
public:
    explicit JobTypeListView(QWidget *parent = nullptr);
    bool updateJobType();

    bool viewportEvent(QEvent *event) override;
private:
    void initUI();//初始化
    void addJobTypeItem(QString strColorHex, QString strJobType);//新增一行【日程类型】数据

signals:

public slots:
    void slotUpdateJobType();
    void slotDeleteJobType();

private:
    QList<JobTypeInfo> m_lstJobType {};
    QStandardItemModel *m_modelJobType {nullptr};
    int m_iIndexCurrentHover = -1;
};

#endif // JOBTYPELISTVIEW_H
