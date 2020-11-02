#include <QCoreApplication>
#include <DLog>
#include <QDBusConnection>
#include <QDBusError>
#include "calendarservice.h"

void test(CalendarService &service)
{
    qDebug() << service.GetType(4);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setOrganizationName("deepin");
    a.setApplicationName("dde-calendar-service");

    Dtk::Core::DLogManager::registerConsoleAppender();
    Dtk::Core::DLogManager::registerFileAppender();

    qDebug() << "write log to" << Dtk::Core::DLogManager::getlogFilePath();
    //    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    //    if (!sessionBus.registerService(CalendarServiceName)) {
    //        qCritical() << "registerService failed:" << sessionBus.lastError();
    //        exit(0x0001);
    //    }
    CalendarService service;
    test(service);
    //    qDebug() << "sessionBus.registerService success" << Dtk::Core::DLogManager::getlogFilePath();
    //    if (!sessionBus.registerObject(CalendarPath,
    //                                   &service,
    //                                   QDBusConnection::ExportAllSlots |
    //                                   QDBusConnection::ExportAllSignals|
    //                                   QDBusConnection::ExportAllProperties)) {
    //        qCritical() << "registerObject failed:" << sessionBus.lastError();
    //        exit(0x0002);
    //    }
    return a.exec();
}
