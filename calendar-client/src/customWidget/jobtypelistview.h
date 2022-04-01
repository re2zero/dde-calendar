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
    bool addJobType();

    bool viewportEvent(QEvent *event) override;
private:
    void initUI();
    void addJobTypeItem(int idx, QString strColorHex, QString strJobType);

signals:

public slots:

private:
    QList<JobTypeInfo> m_lstJobType {};
    QStandardItemModel *m_modelJobType {nullptr};
    int m_iIndexCurrentHover = -1;
};

#endif // JOBTYPELISTVIEW_H
