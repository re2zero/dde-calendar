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
//#include "cscheduledbusstub.h"

//#include "cscheduledbus.h"

//#include <QObject>

//qint64 CreateJob_stub(void *obj, const ScheduleDataInfo &info)
//{
//    Q_UNUSED(obj)
//    Q_UNUSED(info)
//    return 1;
//}

//bool UpdateJob_stub(void *obj, const ScheduleDataInfo &info)
//{
//    Q_UNUSED(obj)
//    Q_UNUSED(info)
//    return true;
//}

//bool DeleteJob_stub(void *obj, qint64 jobId)
//{
//    Q_UNUSED(obj)
//    Q_UNUSED(jobId)
//    return true;
//}

//bool GetJob_stub(void *obj, qint64 jobId, ScheduleDataInfo &out)
//{
//    Q_UNUSED(obj)
//    Q_UNUSED(jobId)
//    Q_UNUSED(out)
//    return true;
//}

//bool QueryJobs_stub(void *obj, QString key, QDateTime starttime, QDateTime endtime, QMap<QDate, QVector<ScheduleDataInfo>> &out)
//{
//    Q_UNUSED(obj)
//    Q_UNUSED(key)
//    Q_UNUSED(starttime)
//    Q_UNUSED(endtime)
//    Q_UNUSED(out)
//    return true;
//}

//void cscheduleDbusStub(Stub &stub)
//{
//    stub.set(ADDR(CScheduleDBus, CreateJob), CreateJob_stub);
//    stub.set(ADDR(CScheduleDBus, UpdateJob), UpdateJob_stub);
//    stub.set(ADDR(CScheduleDBus, DeleteJob), DeleteJob_stub);
//    stub.set(ADDR(CScheduleDBus, GetJob), GetJob_stub);
//    stub.set((bool (CScheduleDBus::*)(QString, QDateTime, QDateTime, QMap<QDate, QVector<ScheduleDataInfo>> &))ADDR(CScheduleDBus, QueryJobs), QueryJobs_stub);
//}
