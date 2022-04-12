#ifndef JOBTYPECOMBOBOX_H
#define JOBTYPECOMBOBOX_H

#include "src/scheduledatainfo.h"
#include "scheduledatamanage.h"
#include "cpushbutton.h"

#include <DComboBox>
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
    void signalAddTypeBtnClicked();

public slots:

protected slots:
    void slotBtnAddItemClicked();
    void showPopup() override;

private:
    void initUI();
    void addJobTypeItem(int idx, QString strColorHex, QString strJobType);

private:
    CPushButton *m_btnAdd {nullptr};
    QList<JobTypeInfo> m_lstJobType;
};

#endif // JOBTYPECOMBOBOX_H
