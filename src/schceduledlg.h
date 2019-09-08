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

#include <QObject>
#include <QWidget>
#include <DDialog>
#include <DLabel>
#include <QComboBox>
#include <DTextEdit>
#include <QDateEdit>
#include <QTimeEdit>
#include <QRadioButton>
#include <DPushButton>
#include <QButtonGroup>
#include <DArrowButton>
#include <DDateEdit>
#include <DTimeEdit>
#include <DLineEdit>
#include <DCheckBox>

//#include <DDateEdit>
#include "schedulestructs.h"
DWIDGET_USE_NAMESPACE
class CCalendarTimeEidt;
class CTimeEdit;
class CCustomCalendarWidget;
class CSchceduleDlg : public DDialog
{
    Q_OBJECT
public:
    CSchceduleDlg(int type, QWidget *parent = 0);
    void setData(const ScheduleInfo &info);
    void setData(const ScheduleDtailInfo &info);
    void setDate(const QDateTime &date);
    ScheduleInfo getData();
    ScheduleDtailInfo getScheduleData();
public slots:
    void slotCancelBt();
    void slotOkBt();
    void slotTextChange();
    void slotBDateEidtInfo(const QDate &date);
    void slotEDateEidtInfo(const QDate &date);
    void slotallDayStateChanged(int state);
    void slotbRpeatactivated(int index);
    void sloteRpeatactivated(int index);
protected:
    // void focusInEvent(QFocusEvent *event);
private:
    void initUI();
    void initConnection();
    void initDateEdit();
    void initRmindRpeatUI();
private:
    DLabel                           *m_typeLabel = nullptr;
    DComboBox                        *m_typeComBox = nullptr;
    DLabel                           *m_contentLabel = nullptr;
    DTextEdit                        *m_textEdit = nullptr;
    DLabel                           *m_beginTimeLabel = nullptr;
    DDateEdit                        *m_beginDateEdit = nullptr;
    CTimeEdit                        *m_beginTimeEdit = nullptr;
    DLabel                           *m_endTimeLabel = nullptr;
    DDateEdit                        *m_endDateEdit = nullptr;
    CTimeEdit                        *m_endTimeEdit = nullptr;

    DLabel                           *m_adllDayLabel = nullptr;
    DCheckBox                        *m_allDayCheckbox = nullptr;
    DLabel                           *m_remindSetLabel = nullptr;
    DComboBox                        *m_rmindCombox = nullptr;
    DLabel                           *m_beginrepeatLabel = nullptr;
    DComboBox                        *m_beginrepeatCombox = nullptr;
    DLabel                           *m_endrepeatLabel = nullptr;
    DComboBox                        *m_endrepeatCombox = nullptr;
    DLineEdit                        *m_endrepeattimes;
    DLabel                           *m_endrepeattimesLabel = nullptr;
    DWidget                          *m_endrepeattimesWidget;
    DDateEdit                        *m_endRepeatDate = nullptr;
    DWidget                          *m_endrepeatWidget;
    DPushButton                      *m_cancelBt = nullptr;
    DPushButton                      *m_OkBt = nullptr;
    //CCalendarTimeEidt                *m_bCalendarTimeEidt;
    //CCalendarTimeEidt                *m_eCalendarTimeEidt;
    CCustomCalendarWidget            *m_bCustomDateW;
    CCustomCalendarWidget            *m_eCustomDateW;
private:
    ScheduleInfo                     m_scheduleInfo;//日程
    ScheduleDtailInfo                m_scheduleDtailInfo;
    int                              m_type; //0新建 1 编辑日程
    QDateTime                        m_currentDate;
};

#endif // SHCEDULEDLG_H
