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
    void setDate(const QDateTime &date);
    ScheduleInfo getData();
public slots:
    void slotCancelBt();
    void slotOkBt();
    void slotTextChange();
    void slotBDateEidtInfo(const QDate &date);
    void slotEDateEidtInfo(const QDate &date);

protected:
    // void focusInEvent(QFocusEvent *event);
private:
    void initUI();
    void initConnection();
    void initDateEdit();
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
    DLabel                           *m_remindSetLabel = nullptr;
    QButtonGroup                     *m_remindbtGroup = nullptr;
    QRadioButton                     *m_currentDayBt = nullptr;
    QRadioButton                     *m_earlierDayBt = nullptr;
    QRadioButton                     *m_threedaysagoBt = nullptr;
    QRadioButton                     *m_weekDayagoBt = nullptr;
    QButtonGroup                     *m_repeatbtGroup = nullptr;
    DLabel                           *m_repeatLabel = nullptr;
    QRadioButton                     *m_onceTimeBt = nullptr;
    QRadioButton                     *m_everyDayBt = nullptr;
    QRadioButton                     *m_everyweekBt = nullptr;
    QRadioButton                     *m_everymonthBt = nullptr;
    QRadioButton                     *m_everyyearBt = nullptr;
    DPushButton                      *m_cancelBt = nullptr;
    DPushButton                      *m_OkBt = nullptr;
    //CCalendarTimeEidt                *m_bCalendarTimeEidt;
    //CCalendarTimeEidt                *m_eCalendarTimeEidt;
    CCustomCalendarWidget            *m_bCustomDateW;
    CCustomCalendarWidget            *m_eCustomDateW;
private:
    ScheduleInfo                     m_scheduleInfo;//日程
    int                              m_type; //0新建 1 编辑日程
};

#endif // SHCEDULEDLG_H
