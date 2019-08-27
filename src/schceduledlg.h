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
#include <ddialog.h>
#include <dlabel.h>
#include <QComboBox>
#include <dtextedit.h>
#include <QDateEdit>
#include <QTimeEdit>
#include <QRadioButton>
#include <dpushbutton.h>
#include <dtextbutton.h>
#include <QButtonGroup>
#include <DArrowButton>
//#include <DDateEdit>
#include "schedulestructs.h"
DWIDGET_USE_NAMESPACE
class CCalendarTimeEidt;
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
    void slotBeginTimeBt();
    void slotEndTimeBt();
    void slotBCalendarTimeEidtInfo();
    void slotECalendarTimeEidtInfo();
    void slotBTimeEidtInfo(const QTime &time);
    void slotETimeEidtInfo(const QTime &time);
protected:
    void focusInEvent(QFocusEvent *event);
private:
    void initUI();
    void initConnection();
private:
    DLabel                           *m_typeLabel = nullptr;
    QComboBox                        *m_typeComBox = nullptr;
    DLabel                           *m_contentLabel = nullptr;
    DTextEdit                        *m_textEdit = nullptr;
    DLabel                           *m_beginTimeLabel = nullptr;
    QDateEdit                        *m_beginDateEdit = nullptr;
    QTimeEdit                        *m_beginTimeEdit = nullptr;
    DLabel                           *m_endTimeLabel = nullptr;
    QDateEdit                        *m_endDateEdit = nullptr;
    QTimeEdit                        *m_endTimeEdit = nullptr;
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
    DTextButton                      *m_cancelBt = nullptr;
    DTextButton                      *m_OkBt = nullptr;
    DArrowButton                     *m_beginTimeEidtBt;
    DArrowButton                     *m_endTimeEidtBt;

    bool                             m_btimeeditflag = false;
    bool                             m_etimeeditflag = false;
    CCalendarTimeEidt                *m_bCalendarTimeEidt;
    CCalendarTimeEidt                *m_eCalendarTimeEidt;
private:
    ScheduleInfo                     m_scheduleInfo;//日程
    int                              m_type; //0新建 1 编辑日程
};

#endif // SHCEDULEDLG_H
