#ifndef JOBTYPECOMBOBOX_H
#define JOBTYPECOMBOBOX_H

#include "src/scheduledatainfo.h"
#include "scheduledatamanage.h"

#include <DComboBox>
#include <DIconButton>
#include <DPushButton>

#include <QLabel>
#include <QHBoxLayout>
DWIDGET_USE_NAMESPACE

class JobTypeComboBox : public DComboBox
{
    Q_OBJECT
public:
    explicit JobTypeComboBox(QWidget *parent = nullptr);
    ~JobTypeComboBox() override;
    bool updateJobType();
    int getCurrentJobTypeNo();
    void setCurrentJobTypeNo(int strJobTypeNo);

signals:

public slots:

protected slots:
    void slotBtnAddItemClicked();
    void showPopup() override;

private:
    void initUI();
    void addJobTypeItem(int idx, QString strColorHex, QString strJobType);

private:
    DPushButton *m_btnAdd {nullptr};
    QList<JobTypeInfo> m_lstJobType;
};

#endif // JOBTYPECOMBOBOX_H
