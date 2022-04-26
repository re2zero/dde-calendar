/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     leilong  <leilong@uniontech.com>
*
* Maintainer: leilong  <leilong@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef JOBTYPECOMBOBOX_H
#define JOBTYPECOMBOBOX_H

#include "src/scheduledatainfo.h"
#include "scheduledatamanage.h"
#include "cpushbutton.h"

#include <DComboBox>
#include <DAlertControl>

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

    void setAlert(bool isAlert);
    bool isAlert() const;
    void showAlertMessage(const QString &text, int duration = 3000);
    void showAlertMessage(const QString &text, QWidget *follower, int duration = 3000);
    void setAlertMessageAlignment(Qt::Alignment alignment);
    void hideAlertMessage();

signals:
    void signalAddTypeBtnClicked();
    void alertChanged(bool alert) const;
public slots:

protected slots:
    void slotBtnAddItemClicked();

protected:
    void showPopup() override;
    bool eventFilter(QObject *, QEvent *) override;

private:
    void initUI();
    void addJobTypeItem(int idx, QString strColorHex, QString strJobType);
    void addCustomWidget(QFrame *);
    void setItemSelectable(bool status);

private:
    QWidget *m_customWidget {nullptr};
    CPushButton *m_addBtn {nullptr};
    QList<JobTypeInfo> m_lstJobType;
    int m_hoverSelectedIndex = -1; //鼠标悬停的选项下标
    int m_itemNumIndex = 0; //item数量
    DAlertControl *m_control {nullptr};
    QLineEdit *m_lineEdit {nullptr};
};

#endif // JOBTYPECOMBOBOX_H
