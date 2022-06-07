// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "cscheduledbus.h"

#include <QDebug>
#include <QDBusReply>
#include <QJsonDocument>


CScheduleDBus *CScheduleDBus::m_scheduleDBus = nullptr;
CScheduleDBus::CScheduleDBus(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
}

CScheduleDBus *CScheduleDBus::getInstance()
{
    if (m_scheduleDBus == nullptr) {
        m_scheduleDBus = new CScheduleDBus(DBUS_NAME, DBUS_PATH, QDBusConnection::sessionBus());
    }
    return m_scheduleDBus;
}

void CScheduleDBus::releaseInstance()
{
    delete m_scheduleDBus;
    m_scheduleDBus = nullptr;
}

CScheduleDBus::~CScheduleDBus()
{

}

/**
 * @brief CScheduleDBus::GetFestivalMonth       获取班休信息
 * @param year                                  年
 * @param month                                 月
 * @return
 */
bool CScheduleDBus::GetFestivalMonth(quint32 year, quint32 month, QVector<FestivalInfo> &out)
{
//    QList<QVariant> argumentList;
//    argumentList << QVariant::fromValue(year) << QVariant::fromValue(month) ;
//    QDBusPendingCall pCall = asyncCallWithArgumentList(QStringLiteral("GetFestivalMonth"), argumentList);
//    pCall.waitForFinished();
//    QDBusMessage reply = pCall.reply();
//    if (reply.type() != QDBusMessage::ReplyMessage) {
//        qWarning() << "GetFestivalMonth err ," << reply;
//        return false;
//    }
//    QDBusReply<QString> jobs =  reply;
//    if (!jobs.isValid())
//        return false;

//    QJsonParseError json_error;
//    QJsonDocument jsonDoc(QJsonDocument::fromJson(jobs.value().toLocal8Bit(), &json_error));

//    if (json_error.error != QJsonParseError::NoError) {
//        return false;
//    }

//    QJsonArray rootarry = jsonDoc.array();

//    for (int i = 0; i < rootarry.size(); i++) {

//        QJsonObject subObj = rootarry.at(i).toObject();
//        FestivalInfo festivalInfoday;

//        //因为是预先定义好的JSON数据格式，所以这里可以这样读取
//        if (subObj.contains("id")) {
//            festivalInfoday.ID = subObj.value("id").toString();
//        }
//        if (subObj.contains("name")) {
//            festivalInfoday.FestivalName = subObj.value("name").toString();
//        }
//        if (subObj.contains("description")) {
//            festivalInfoday.description = subObj.value("description").toString();
//        }
//        if (subObj.contains("rest")) {
//            festivalInfoday.Rest = subObj.value("rest").toString();
//        }
//        if (subObj.contains("month")) {
//            festivalInfoday.month = subObj.value("month").toInt();
//        }
//        if (subObj.contains("list")) {
//            QJsonArray sublistArray = subObj.value("list").toArray();
//            for (int j = 0; j < sublistArray.size(); j++) {
//                QJsonObject hsubObj = sublistArray.at(j).toObject();
//                HolidayInfo dayinfo;
//                if (hsubObj.contains("status")) {
//                    dayinfo.status = static_cast<char>(hsubObj.value("status").toInt());
//                }
//                if (hsubObj.contains("date")) {
//                    dayinfo.date = QDate::fromString(hsubObj.value("date").toString(), "yyyy-M-d");
//                }
//                festivalInfoday.listHoliday.append(dayinfo);
//            }
//        }
//        festivalInfoday.year = static_cast<int>(year);
//        out.append(festivalInfoday);
//    }
    return true;
}

/**
 * @brief CScheduleDBus::GetHuangLiDay          获取某天的农历信息
 * @param getDay
 * @param out
 * @return
 */
bool CScheduleDBus::GetHuangLiDay(const QDate &getDay, CaHuangLiDayInfo &out)
{
//    QList<QVariant> argumentList;
//    argumentList << QVariant::fromValue(static_cast<quint32>(getDay.year()))
//                 << QVariant::fromValue(static_cast<quint32>(getDay.month()))
//                 << QVariant::fromValue(static_cast<quint32>(getDay.day()));
//    QDBusPendingCall pCall = asyncCallWithArgumentList(QStringLiteral("GetHuangLiDay"), argumentList);
//    pCall.waitForFinished();
//    QDBusMessage reply = pCall.reply();
//    if (reply.type() != QDBusMessage::ReplyMessage) {
//        qWarning() << "GetHuangLiDay err ," << reply;
//        return false;
//    }
//    QDBusReply<QString> jobs =  reply;
//    if (!jobs.isValid())
//        return false;
//    bool _isVoild;
//    out.strJsonToInfo(jobs.value(), _isVoild);
//    return _isVoild;
}

/**
 * @brief CScheduleDBus::GetHuangLiMonth        获取某月的农历信息
 * @param year
 * @param month
 * @param fill
 * @return
 */
bool CScheduleDBus::GetHuangLiMonth(const quint32 year, const quint32 month, bool &fill, CaHuangLiMonthInfo &out)
{
//    QList<QVariant> argumentList;
//    argumentList << QVariant::fromValue(year) << QVariant::fromValue(month) << QVariant::fromValue(fill);
//    QDBusPendingCall pCall = asyncCallWithArgumentList(QStringLiteral("GetHuangLiMonth"), argumentList);
//    pCall.waitForFinished();
//    QDBusMessage reply = pCall.reply();
//    if (reply.type() != QDBusMessage::ReplyMessage) {
//        qWarning() << "GetHuangLiMonth err ," << reply;
//        return false;
//    }
//    QDBusReply<QString> jobs =  reply;
//    if (!jobs.isValid())
//        return false;
//    bool _infoIsVaild;
//    out.strJsonToInfo(jobs.value(), _infoIsVaild);
//    return _infoIsVaild;
}

QString CScheduleDBus::getHuangLiShortName(const QDate &date)
{
//    CaHuangLiDayInfo info;
//    if (GetHuangLiDay(date, info)) {
//        return info.mLunarMonthName + info.mLunarDayName;
//    }
//    return "";
}
