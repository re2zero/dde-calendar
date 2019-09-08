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
#include "scheduledatamanage.h"

#include "schedulesdbus.h"

CScheduleDataManage *CScheduleDataManage::m_vscheduleDataManage = new CScheduleDataManage;
CScheduleDataCtrl *CScheduleDataManage::getscheduleDataCtrl()
{
    return m_scheduleDataCtrl;
}

CScheduleDataManage *CScheduleDataManage::getScheduleDataManage()
{
    return m_vscheduleDataManage;
}

void CScheduleDataManage::clear()
{
    delete m_scheduleDataCtrl;
}

CScheduleDataManage::CScheduleDataManage ()
{
    m_scheduleDataCtrl = new CScheduleDataCtrl;
}

CScheduleDataManage::~CScheduleDataManage()
{

}

CScheduleDataCtrl::CScheduleDataCtrl()
{
    qRegisterMetaType<QVector<ScheduleDateRangeInfo>>("QVector<ScheduleDateRangeInfo>");
    m_dbus = new CSchedulesDBus("com.deepin.daemon.Calendar",
                                "/com/deepin/daemon/Calendar/Scheduler",
                                QDBusConnection::sessionBus(), this);
    m_scheduleDateCache = nullptr;

    if (!m_scheduleDateCache)
        m_scheduleDateCache = new QMap<QDate, ScheduleDateRangeInfo>;
    m_thread = new CDataProcessThread(m_dbus);
    connect(m_thread, &CDataProcessThread::signalsDataProcess, this, &CScheduleDataCtrl::slotDataProcess);
}

CScheduleDataCtrl::~CScheduleDataCtrl()
{
    delete  m_scheduleDateCache;
    delete  m_dbus;
    delete m_thread;
}



qint64 CScheduleDataCtrl::addSchedule(const ScheduleDtailInfo &scheduledate)
{
    clearData();
    return m_dbus->CreateJob(scheduledate);
}

bool CScheduleDataCtrl::getScheduleInfoById(int id, ScheduleDtailInfo &out)
{
    return m_dbus->GetJob(id, out);
}

bool CScheduleDataCtrl::getScheduleInfo(const QDate bdate, const QDate edate, QVector<ScheduleDateRangeInfo> &out)
{
    return m_dbus->GetJobs(bdate.year(), bdate.month(), bdate.day(), edate.year(), edate.month(), edate.day(), out);
}

bool CScheduleDataCtrl::updateScheduleInfo(const ScheduleDtailInfo &scheduledate)
{
    clearData();
    return m_dbus->UpdateJob(scheduledate);
}

bool CScheduleDataCtrl::deleteScheduleInfoById(int id)
{
    clearData();
    return m_dbus->DeleteJob(id);
}

bool CScheduleDataCtrl::GetType(qint64 jobId, ScheduleType &out)
{
    while (m_thread->isRunning());
    return m_dbus->GetType(jobId, out);
}

void CScheduleDataCtrl::slotupdatescheduleD(QWidget *w, QDate begin, QDate end)
{
    m_currentWidget = w;
    m_begindate = begin;
    m_enddate = end;
    m_thread->DataProcessStart(begin, end);
}

void CScheduleDataCtrl::slotDataProcess(QVector<ScheduleDateRangeInfo> out)
{
    emit signalsupdatescheduleD(m_currentWidget, out);
}

void CScheduleDataCtrl::clearData()
{
    while (m_thread->isRunning());
    m_scheduleDateCache->clear();
}

CDataProcessThread::CDataProcessThread(CSchedulesDBus *_DataManage)
{
    m_DataManage = _DataManage;
    m_stopFlag = false;
}

CDataProcessThread::~CDataProcessThread()
{
    //防止未运行完
    m_mutex.lock();
    m_stopFlag = true;
    m_mutex.unlock();
    //是否允许
    while (isRunning()) {}
}

void CDataProcessThread::DataProcessStart(QDate begin, QDate end)
{
    m_mutex.lock();
    this->m_stopFlag = true;
    this->m_begindate = begin;
    this->m_enddate = end;
    m_mutex.unlock();

    while (isRunning()) {}
    m_mutex.lock();
    m_stopFlag = false;
    m_mutex.unlock();

    start(QThread::NormalPriority);
}

void CDataProcessThread::run()
{
    forever {
        //运行
        m_mutex.lock();
        QDate bdate =  m_begindate;
        QDate edate = m_enddate;
        bool t_stopFlag = m_stopFlag;
        m_mutex.unlock();
        if (t_stopFlag)
            return;
        QVector<ScheduleDateRangeInfo> out;
        bool flag = m_DataManage->GetJobs(bdate.year(), bdate.month(), bdate.day(), edate.year(), edate.month(), edate.day(), out);
        if (!flag) return;
        emit signalsDataProcess(out);
        break;
    }
}
