/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     kirigaya <kirigaya@mkacg.com>
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
#ifndef SCHEDULEDLG_H
#define SCHEDULEDLG_H

#include "src/scheduledatainfo.h"

#include <DCheckBox>
#include <DDateEdit>
#include <DDialog>
#include <DFrame>
#include <DLineEdit>
#include <DTextEdit>
#include <DComboBox>

#include <QLabel>
#include <QTimeEdit>

DWIDGET_USE_NAMESPACE
class CTimeEdit;
class CScheduleDlg : public DDialog
{
    Q_OBJECT
public:
    CScheduleDlg(int type, QWidget *parent = nullptr, const bool isAllDay = true);
    ~CScheduleDlg() override;
    void setData(const ScheduleDataInfo &info);
    void setDate(const QDateTime &date);
    ScheduleDataInfo getScheduleData();
    void setAllDay(bool flag);
private:
    //确定按钮处理
    bool clickOkBtn();
signals:
    void signalViewtransparentFrame(int type);
    void signalScheduleUpdate(int id = 0);
public slots:
    //对话框按钮点击处理
    void slotBtClick(int buttonIndex, QString buttonName);
    void slotTextChange();
    void slotendrepeatTextchange();
    void slotBDateEidtInfo(const QDate &date);
    void slotallDayStateChanged(int state);
    void slotbRpeatactivated(int index);
    void sloteRpeatactivated(int index);
protected:
    bool eventFilter(QObject *obj, QEvent *pEvent) override;
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void changeEvent(QEvent *event) override;
private:
    void initUI();
    void initConnection();
    void initDateEdit();
    void initRmindRpeatUI();
    /**
     * @brief setTheMe  根据主题type设置颜色
     * @param type  主题type
     */
    void setTheMe(const int type);
private:
    QLabel *m_typeLabel = nullptr;
    DComboBox *m_typeComBox = nullptr;
    QLabel *m_contentLabel = nullptr;
    DTextEdit *m_textEdit = nullptr;
    QLabel *m_beginTimeLabel = nullptr;
    DDateEdit *m_beginDateEdit = nullptr;
    CTimeEdit *m_beginTimeEdit = nullptr;
    QLabel *m_endTimeLabel = nullptr;
    DDateEdit *m_endDateEdit = nullptr;
    CTimeEdit *m_endTimeEdit = nullptr;

    QLabel *m_adllDayLabel = nullptr;
    DCheckBox *m_allDayCheckbox = nullptr;
    QLabel *m_remindSetLabel = nullptr;
    DComboBox *m_rmindCombox = nullptr;
    QLabel *m_beginrepeatLabel = nullptr;
    DComboBox *m_beginrepeatCombox = nullptr;
    QLabel *m_endrepeatLabel = nullptr;
    DComboBox *m_endrepeatCombox = nullptr;
    DLineEdit *m_endrepeattimes = nullptr;
    QLabel *m_endrepeattimesLabel = nullptr;
    DWidget *m_endrepeattimesWidget;
    DDateEdit *m_endRepeatDate = nullptr;
    DWidget *m_endrepeatWidget = nullptr;
    DFrame *m_gwi = nullptr;
    QLabel *m_titleLabel = nullptr;

    QString m_context;
    const bool m_createAllDay;
    bool m_setAccept {false}; //是否设置返回代码为Rejected
private:
    //日程
    ScheduleDataInfo m_ScheduleDataInfo;
    int m_type; // 1新建 0 编辑日程
    QDateTime m_currentDate;
    QDateTime m_EndDate;
};

#endif // SCHEDULEDLG_H
