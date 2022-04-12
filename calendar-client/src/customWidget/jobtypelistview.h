#ifndef JOBTYPELISTVIEW_H
#define JOBTYPELISTVIEW_H

#include "scheduledatamanage.h"

#include <DListView>
#include <DIconButton>

#include <QWidget>
#include <QList>

DWIDGET_USE_NAMESPACE

class JobTypeListView : public DListView
{
    Q_OBJECT
public:
    explicit JobTypeListView(QWidget *parent = nullptr);
    ~JobTypeListView();
    bool updateJobType();

    /**
     * @brief canAdd 是否可以继续新增类型
     */
    bool canAdd();
protected:
    bool viewportEvent(QEvent *event) override;
private:
    void initUI();//初始化
    /**
     * @brief addJobTypeItem 添加item
     */
    void addJobTypeItem(const JobTypeInfo &info);//新增一行【日程类型】数据

signals:
    /**
     * @brief signalAddStatusChanged 信号：是否可以继续新增类型
     */
    void signalAddStatusChanged(bool);
public slots:
    void slotUpdateJobType();
    void slotDeleteJobType();

private:
    QStandardItemModel *m_modelJobType {nullptr};
    int m_iIndexCurrentHover = -1;
};

#endif // JOBTYPELISTVIEW_H
