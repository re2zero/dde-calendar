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

#include "environments.h"
#include "calendarmainwindow.h"
#include "exportedinterface.h"
#include "configsettings.h"
#include "accessible/accessible.h"
#include "src/DebugTimeManager.h"
#include "tabletconfig.h"

#include <DApplication>
#include <DLog>
#include <DApplicationHelper>
#include <DApplicationSettings>

#include <QDBusConnection>

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    PERF_PRINT_BEGIN("POINT-01", "");
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    //适配deepin-turbo启动加速
    DApplication *app = nullptr;
#if(DTK_VERSION < DTK_VERSION_CHECK(5,4,0,0))
    app = new DApplication(argc, argv);
#else
    app = DApplication::globalApplication(argc, argv);
#endif
//获取设置是否为平板模式
#if (DTK_VERSION >= DTK_VERSION_CHECK(5, 4, 7, 0))
    TabletConfig::setIsTablet(DGuiApplicationHelper::isTabletEnvironment());
#endif

    QAccessible::installFactory(accessibleFactory);
    app->setOrganizationName("deepin");
    app->setApplicationName("dde-calendar");
    app->loadTranslator();
    app->setApplicationVersion(VERSION);
    // meta information that necessary to create the about dialog.
    app->setProductName(QApplication::translate("CalendarWindow", "Calendar"));
    QIcon t_icon = QIcon::fromTheme("dde-calendar");
    app->setProductIcon(t_icon);
    app->setApplicationDescription(QApplication::translate("CalendarWindow", "Calendar is a tool to view dates, and also a smart daily planner to schedule all things in life. "));
    app->setApplicationAcknowledgementPage("https://www.deepin.org/acknowledgments/dde-calendar");
    //命令行参数
    QCommandLineParser _commandLine;       //建立命令行解析
    _commandLine.addHelpOption();          //增加-h/-help解析命令
    _commandLine.addVersionOption();       //增加-v 解析命令
    _commandLine.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    _commandLine.process(*app);
//如果dtk版本为5.2.0.1以上则使用新的dtk接口
#if (DTK_VERSION > DTK_VERSION_CHECK(5, 2, 0, 1))
    //设置为-1将永久等待
    DGuiApplicationHelper::setSingleInstanceInterval(-1);
#endif
    if (!DGuiApplicationHelper::instance()->setSingleInstance(
                app->applicationName(),
                DGuiApplicationHelper::UserScope)) {
        qDebug() << "there's an dde-calendar instance running.";
        QProcess::execute("dbus-send --print-reply --dest=com.deepin.Calendar "
                          "/com/deepin/Calendar com.deepin.Calendar.RaiseWindow");
        return 0;
    }

    app->setAutoActivateWindows(true);
    CConfigSettings::init();
    bool isOk = false;
    int viewtype = CConfigSettings::value("base.view").toInt(&isOk);
    if (!isOk)
        viewtype = 2;
    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();
    //获取应用配置
    DApplicationSettings applicationset(app);
    //为了与老版本配置兼容
    Calendarmainwindow ww(viewtype - 1);
    ExportedInterface einterface(&ww);
    einterface.registerAction("CREATE", "create a new schedule");
    einterface.registerAction("VIEW", "check a date on calendar");
    einterface.registerAction("QUERY", "find a schedule information");
    einterface.registerAction("CANCEL", "cancel a schedule");
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerService("com.deepin.Calendar");
    dbus.registerObject("/com/deepin/Calendar", &ww);
    ww.slotTheme(DApplicationHelper::instance()->themeType());
    ww.show();

    PERF_PRINT_END("POINT-01");
    return app->exec();
}
