// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef CSCHEDULEDBUS_H
#define CSCHEDULEDBUS_H

#include "dschedule.h"
#include "huangliData/lunardatastruct.h"
#include "huangliData/dbusdatastruct.h"

#include <QDBusAbstractInterface>
#include <QDBusPendingReply>
#include <QDateTime>


#define DBUS_INTEERFACENAME "com.deepin.dataserver.Calendar"
#define DBUS_NAME "com.deepin.dataserver.Calendar"
#define DBUS_PATH "/com/deepin/dataserver/Calendar"

//TODO:使用新类获取农历信息和日程信息
class CScheduleDBus : public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static CScheduleDBus *getInstance();
    static void releaseInstance();
    ~CScheduleDBus();
public:
    static inline const char *staticInterfaceName()
    {
        return DBUS_INTEERFACENAME;
    }
public:
    //获取节假日班休信息
    bool GetFestivalMonth(quint32 year, quint32 month, QVector<FestivalInfo> &out);
    //按天获取农历信息
    bool GetHuangLiDay(const QDate &getDay, CaHuangLiDayInfo &out);
    //按月获取农历信息
    bool GetHuangLiMonth(const quint32 year, const quint32 month, bool &fill, CaHuangLiMonthInfo &out);
    //获取当天的农历月日期和日日期名
    QString getHuangLiShortName(const QDate &date);

private:
    explicit CScheduleDBus(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = nullptr);

    static CScheduleDBus *m_scheduleDBus;

};
#endif // CSCHEDULEDBUS_H
