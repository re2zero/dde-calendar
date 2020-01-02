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
#include <QLabel>
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
#include <DFrame>
//#include <DDateEdit>
#include "schedulestructs.h"
DWIDGET_USE_NAMESPACE
class CTimeEdit;
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
    void setAllDay(bool flag);
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
    bool eventFilter(QObject *obj, QEvent *pEvent);
private:
    void initUI();
    void initConnection();
    void initDateEdit();
    void initRmindRpeatUI();
private:
    QLabel                           *m_typeLabel = nullptr;
    DComboBox                        *m_typeComBox = nullptr;
    QLabel                           *m_contentLabel = nullptr;
    DTextEdit                        *m_textEdit = nullptr;
    QLabel                           *m_beginTimeLabel = nullptr;
    DDateEdit                        *m_beginDateEdit = nullptr;
    CTimeEdit                        *m_beginTimeEdit = nullptr;
    QLabel                           *m_endTimeLabel = nullptr;
    DDateEdit                        *m_endDateEdit = nullptr;
    CTimeEdit                        *m_endTimeEdit = nullptr;

    QLabel                           *m_adllDayLabel = nullptr;
    DCheckBox                        *m_allDayCheckbox = nullptr;
    QLabel                           *m_remindSetLabel = nullptr;
    DComboBox                        *m_rmindCombox = nullptr;
    QLabel                           *m_beginrepeatLabel = nullptr;
    DComboBox                        *m_beginrepeatCombox = nullptr;
    QLabel                           *m_endrepeatLabel = nullptr;
    DComboBox                        *m_endrepeatCombox = nullptr;
    DLineEdit                        *m_endrepeattimes;
    QLabel                           *m_endrepeattimesLabel = nullptr;
    DWidget                          *m_endrepeattimesWidget;
    DDateEdit                        *m_endRepeatDate = nullptr;
    DWidget                          *m_endrepeatWidget;
    DPushButton                      *m_cancelBt = nullptr;
    DPushButton                      *m_OkBt = nullptr;
    //CCalendarTimeEidt                *m_bCalendarTimeEidt;
    //CCalendarTimeEidt                *m_eCalendarTimeEidt;
    //CCustomCalendarWidget            *m_bCustomDateW;
    //CCustomCalendarWidget            *m_eCustomDateW;
    DFrame                           *m_gwi;
    QLabel                           *m_titleLabel;

    QString                           m_context;
private:
    ScheduleInfo                     m_scheduleInfo;//日程
    ScheduleDtailInfo                m_scheduleDtailInfo;
    int                              m_type; //1新建 0 编辑日程
    QDateTime                        m_currentDate;
};

#endif // SHCEDULEDLG_H
