/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#include "calendarwindow.h"

#include <QFile>
#include <QDebug>
#include <QDesktopWidget>
#include <QDBusConnection>

#include <DApplication>
#include <DLog>

DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE

QString GetStyleSheetContent() {
    QFile file(":/resources/dde-calendar.qss");
    bool result = file.open(QFile::ReadOnly);
    if (result) {
        QString content(file.readAll());
        file.close();
        return content;
    } else {
        return "";
    }
}

QRect PrimaryRect() {
    QDesktopWidget *w = QApplication::desktop();
    return w->screenGeometry(w->primaryScreen());
}

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication a(argc, argv);
    a.setOrganizationName("deepin");
    a.setApplicationName("dde-calendar");
    a.setApplicationVersion("1.0");

    QTranslator translator;
    translator.load("/usr/share/dde-calendar/translations/dde-calendar_" + QLocale::system().name());
    a.installTranslator(&translator);

    // meta information that necessary to create the about dialog.
    a.setProductName(QApplication::translate("CalendarWindow", "Deepin Calendar"));
    a.setProductIcon(QPixmap(":/resources/icon/dde-calendar_96.png"));
    a.setApplicationDescription(QApplication::translate("CalendarWindow", "Calendar is a date tool."));

    if (!a.setSingleInstance("dde-calendar")) {
        qDebug() << "there's an dde-calendar instance running.";
        QProcess::execute("dbus-send --print-reply --dest=com.deepin.dde.Calendar "
                          "/com/deepin/dde/Calendar com.deepin.dde.Calendar.RaiseWindow");

        return 0;
    }

    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();

    a.setTheme("light");
    a.setStyleSheet(GetStyleSheetContent());
    a.loadTranslator();

    CalendarWindow cw;
    cw.move(PrimaryRect().center() - cw.geometry().center());
    cw.show();

    QDBusConnection dbus = QDBusConnection::sessionBus();
    if (dbus.registerService("com.deepin.dde.Calendar")) {
        dbus.registerObject("/com/deepin/dde/Calendar", &cw);
    }

    return a.exec();
}
