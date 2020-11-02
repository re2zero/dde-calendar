#include <QCoreApplication>
#include <DLog>
#include <QDBusConnection>
#include <QDBusError>
#include "calendarservice.h"

const QString CalendarServiceName = "com.deepin.calendar";
const QString CalendarPath = "/com/deepin/calendar";

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setOrganizationName("deepin");
    a.setApplicationName("dde-calendar-service");

    Dtk::Core::DLogManager::registerConsoleAppender();
    Dtk::Core::DLogManager::registerFileAppender();

    qDebug() << "write log to" << Dtk::Core::DLogManager::getlogFilePath();
    QDBusConnection systemBus = QDBusConnection::systemBus();
    if (!systemBus.registerService(CalendarServiceName)) {
        qCritical() << "registerService failed:" << systemBus.lastError();
        exit(0x0001);
    }
    CalendarService service;
    qDebug() << "systemBus.registerService success" << Dtk::Core::DLogManager::getlogFilePath();
    if (!systemBus.registerObject(CalendarPath,
                                  &service,
                                  QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals)) {
        qCritical() << "registerObject failed:" << systemBus.lastError();
        exit(0x0002);
    }
    return a.exec();
}
