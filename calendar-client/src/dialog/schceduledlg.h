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
#ifndef SCHCEDULEDLG_H
#define SCHCEDULEDLG_H

#include "schedulestructs.h"

#include <DArrowButton>
#include <DCheckBox>
#include <DDateEdit>
#include <DDialog>
#include <DFrame>
#include <DLineEdit>
#include <DPushButton>
#include <DSuggestButton>
#include <DTextEdit>
#include <DTimeEdit>
#include <DComboBox>

#include <QButtonGroup>
#include <QComboBox>
#include <QDateEdit>
#include <QLabel>
#include <QObject>
#include <QRadioButton>
#include <QTimeEdit>
#include <QWidget>

DWIDGET_USE_NAMESPACE
class CTimeEdit;
/**
 * @brief The CSchceduleDlg class
 */
class CSchceduleDlg : public DDialog
{
    Q_OBJECT
public:
    CSchceduleDlg(int type, QWidget *parent = nullptr, const bool isAllDay = true);
    ~CSchceduleDlg() override;
    void setData(const ScheduleDtailInfo &info);
    void setDate(const QDateTime &date);
    ScheduleDtailInfo getScheduleData();
    void setAllDay(bool flag);
signals:
    void signalViewtransparentFrame(int type);
    void signalScheduleUpdate(int id = 0);
public slots:
    void slotCancelBt(int buttonIndex, QString buttonName);
    void slotOkBt(int buttonIndex, QString buttonName);
    void slotTextChange();
    void slotendrepeatTextchange();
    void slotBDateEidtInfo(const QDate &date);
    void slotallDayStateChanged(int state);
    void slotbRpeatactivated(int index);
    void sloteRpeatactivated(int index);
protected:
    bool eventFilter(QObject *obj, QEvent *pEvent) override;
    void showEvent(QShowEvent *event) override;
    void changeEvent(QEvent *event) override;
private:
    void initUI();
    void initConnection();
    void initDateEdit();
    void initRmindRpeatUI();
    void setTheMe(const int type);
public:
    static void ChangeRecurInfo(QWidget *parent,const ScheduleDtailInfo &newinfo,
                                const ScheduleDtailInfo &oldinfo,int m_themetype);
    static void ChangeOnlyInfo(const ScheduleDtailInfo &newinfo,
                               const ScheduleDtailInfo &oldinfo);
private:
    //"类型“label
    QLabel *m_typeLabel = nullptr;
    /**
     * @brief m_typeComBox ”类型”combox
     *
     * 提供下拉框，选择不同的日程类型
     */
    DComboBox *m_typeComBox = nullptr;
    //“内容”label
    QLabel *m_contentLabel = nullptr;
    /**
     * @brief m_textEdit textedit
     *
     * 日程详情
     */
    DTextEdit *m_textEdit = nullptr;
    //“开始时间”label
    QLabel *m_beginTimeLabel = nullptr;
    //“开始时间”dateedit
    DDateEdit *m_beginDateEdit = nullptr;
    //“开始时间”timeedit
    CTimeEdit *m_beginTimeEdit = nullptr;
    //“结束时间”label
    QLabel *m_endTimeLabel = nullptr;
    //“结束时间”dateedit
    DDateEdit *m_endDateEdit = nullptr;
    //“结束时间”timeedit
    CTimeEdit *m_endTimeEdit = nullptr;
    //“全天”label
    QLabel *m_adllDayLabel = nullptr;
    /**
     * @brief m_allDayCheckbox “全天”checkbox
     *
     * 根据checkbox状态，判断是否为全天日程
     */
    DCheckBox *m_allDayCheckbox = nullptr;
    //“提醒”label
    QLabel *m_remindSetLabel = nullptr;
    /**
     * @brief m_rmindCombox “提醒”combox
     *
     * 提供下拉框，用来选择提醒的类型
     */
    DComboBox *m_rmindCombox = nullptr;
    //“重复”label
    QLabel *m_beginrepeatLabel = nullptr;
    /**
     * @brief m_beginrepeatCombox “重复”combox
     *
     * 提供下拉框，用来选择重复类型
     */
    DComboBox *m_beginrepeatCombox = nullptr;
    //“结束重复”label
    QLabel *m_endrepeatLabel = nullptr;
    /**
     * @brief m_endrepeatCombox “结束重复”combox
     *
     * 提供下拉框，用来选择结束类型
     */
    DComboBox *m_endrepeatCombox = nullptr;
    //结束重复于次数
    DLineEdit *m_endrepeattimes = nullptr;
    //结束重复于次数label
    QLabel *m_endrepeattimesLabel = nullptr;
    //结束重复于次数widget
    DWidget *m_endrepeattimesWidget;
    //结束重复于日期
    DDateEdit *m_endRepeatDate = nullptr;
    //结束重复于widget
    DWidget *m_endrepeatWidget = nullptr;
    //frame
    DFrame *m_gwi = nullptr;
    //title label
    QLabel *m_titleLabel = nullptr;
    //日程内容
    QString m_context;
    //是否为全天
    const bool m_createAllDay;
private:
    //日程
    ScheduleDtailInfo m_scheduleDtailInfo;
    // 1新建 0 编辑日程
    int m_type;
    //当前时间
    QDateTime m_currentDate;
    //结束时间
    QDateTime m_EndDate;
};

#endif  // SHCEDULEDLG_H
