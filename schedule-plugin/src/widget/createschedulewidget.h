/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     chenhaifeng  <chenhaifeng@uniontech.com>
*
* Maintainer: chenhaifeng  <chenhaifeng@uniontech.com>
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
#ifndef CREATESCHEDULE_H
#define CREATESCHEDULE_H

#include <QObject>
#include <DFrame>
#include <QPaintEvent>
#include "../data/schedulestructs.h"
#include "icondframe.h"
#include "scheduleitemwidget.h"
#include "../dbus/schedulesdbus.h"

DWIDGET_USE_NAMESPACE

class createSchedulewidget : public IconDFrame
{
    Q_OBJECT
public:
    explicit createSchedulewidget(QWidget *parent = nullptr);

public:
    ScheduleDtailInfo &getScheduleDtailInfo();
    void setTitleName(const QString &titleName);
    void setDateTime(QDateTime begintime, QDateTime endtime);
    void setRpeat(int rpeat);
    void setschedule();
    void scheduleEmpty(bool isEmpty);
    void updateUI();
    void setScheduleDbus(CSchedulesDBus *dbus);
    bool buttonclicked();
public slots:
    void slotsbuttonchance(int index, const QString &text);
    void slotItemPress(const ScheduleDtailInfo &info);
    /**
     * @brief getCreatScheduleFromDbus 通过dbus获取新建的日程信息
     */
    void getCreatScheduleFromDbus();

private:
    ScheduleDtailInfo m_scheduleDtailInfo;
    QDateTime m_BeginDateTime;
    QDateTime m_EndDateTime;
    QString m_titleName;
    int m_rpeat;
    bool m_scheduleEmpty = false;
    scheduleitemwidget *m_scheduleitemwidget;
    QVector<ScheduleDtailInfo> m_scheduleInfo;
    CSchedulesDBus *m_dbus = nullptr;
    bool m_buttonclicked = false;

signals:

public slots:
};

#endif // CREATESCHEDULE_H
