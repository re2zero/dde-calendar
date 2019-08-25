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
#include "dbmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QMutexLocker>
#include <QVariant>
QString ScheduleDbManager::m_dbName = "defaultname";
//QString ScheduleDbManager::m_dbPath = "/usr/share/dde-calendar/calendarSchedule.db";
QString ScheduleDbManager::m_dbPath = "calendarSchedule.db";
bool ScheduleDbManager::m_initFlag = false;
QSqlDatabase     ScheduleDbManager::m_dbSqlDatabase = QSqlDatabase();
QMutex    gmutex;
ScheduleDbManager::ScheduleDbManager()
{

}

int ScheduleDbManager::addSchedule( const ScheduleInfo &scheduledate )
{
    QMutexLocker mutexlocker(&gmutex);
    QSqlQuery query(m_dbSqlDatabase);
    int id = -1;
    query.prepare("INSERT INTO Events (title , description , dstart , dend, eventColorid,allDay,eventrepeatid,reminderid) VALUES (:title , :description , :dstart , :dend, :eventColorid,:allDay,:eventrepeatid,:reminderid)");
    query.bindValue(":title", scheduledate.titleName.toUtf8());
    query.bindValue(":description", scheduledate.description.toUtf8());
    query.bindValue(":dstart", scheduledate.beginDateTime.toString("yyyyMMddhhmmsszzz"));
    query.bindValue(":dend", scheduledate.endDateTime.toString("yyyyMMddhhmmsszzz"));
    query.bindValue(":eventColorid", scheduledate.infotype);
    query.bindValue(":allDay", scheduledate.allday);
    query.bindValue(":eventrepeatid", scheduledate.repeat);
    query.bindValue(":reminderid", scheduledate.remind);
    if (query.exec()) {
        query.prepare("select last_insert_rowid() from Events");
        query.exec();
        query.next();
        id = query.value(0).toInt();
    }
    return id;
}


QVector<ScheduleInfo> ScheduleDbManager::getAllScheduleInfo()
{
    QMutexLocker mutexlocker(&gmutex);
    QVector<ScheduleInfo> scheduleInfoList;
    QSqlQuery query(m_dbSqlDatabase);
    query.prepare( "SELECT * FROM Events" );
    if ( !query.exec() ) return scheduleInfoList;
    QSqlRecord rec = query.record();
    int cols = rec.count();
    for ( int r = 0; query.next(); r++ ) {
        ScheduleInfo  onescheduleInfo;
        for ( int c = 0; c < cols; c++ ) {
            if (rec.fieldName(c) == "_id") {
                onescheduleInfo.id = query.value(c).toInt();
            } else if (rec.fieldName(c) == "title") {
                onescheduleInfo.titleName = query.value(c).toString();
            } else if (rec.fieldName(c) == "eventColorid") {
                onescheduleInfo.infotype = query.value(c).toInt();
            } else if (rec.fieldName(c) == "description") {
                onescheduleInfo.description = query.value(c).toString();
            } else if (rec.fieldName(c) == "dstart") {
                onescheduleInfo.beginDateTime = QDateTime::fromString(query.value(c).toString(), "yyyyMMddhhmmsszzz");
            } else if (rec.fieldName(c) == "dend") {
                onescheduleInfo.endDateTime = QDateTime::fromString(query.value(c).toString(), "yyyyMMddhhmmsszzz");
            } else if (rec.fieldName(c) == "allDay") {
                onescheduleInfo.allday = query.value(c).toInt();
            } else if (rec.fieldName(c) == "eventrepeatid") {
                onescheduleInfo.repeat = query.value(c).toInt();
            } else if (rec.fieldName(c) == "reminderid") {
                onescheduleInfo.remind = query.value(c).toInt();
            }
        }
        scheduleInfoList.append(onescheduleInfo);
    }
    return scheduleInfoList;
}


ScheduleInfo ScheduleDbManager::getScheduleInfoById( int id )
{
    QMutexLocker mutexlocker(&gmutex);
    QSqlQuery query(m_dbSqlDatabase);
    ScheduleInfo  onescheduleInfo;
    query.prepare( "SELECT * FROM Events WHERE _id = ?" );
    query.addBindValue(id);
    if ( !query.exec() ) return onescheduleInfo;
    onescheduleInfo.id = id;
    QSqlRecord rec = query.record();
    int cols = rec.count();
    query.next();
    for ( int c = 0; c < cols; c++ ) {
        if (rec.fieldName(c) == "title") {
            onescheduleInfo.titleName = query.value(c).toString();
        } else if (rec.fieldName(c) == "eventColorid") {
            onescheduleInfo.infotype = query.value(c).toInt();
        } else if (rec.fieldName(c) == "description") {
            onescheduleInfo.description = query.value(c).toString();
        } else if (rec.fieldName(c) == "dstart") {
            onescheduleInfo.beginDateTime = QDateTime::fromString(query.value(c).toString(), "yyyyMMddhhmmsszzz");
        } else if (rec.fieldName(c) == "dend") {
            onescheduleInfo.endDateTime = QDateTime::fromString(query.value(c).toString(), "yyyyMMddhhmmsszzz");
        } else if (rec.fieldName(c) == "allDay") {
            onescheduleInfo.allday = query.value(c).toInt();
        } else if (rec.fieldName(c) == "eventrepeatid") {
            onescheduleInfo.repeat = query.value(c).toInt();
        } else if (rec.fieldName(c) == "reminderid") {
            onescheduleInfo.remind = query.value(c).toInt();
        }
    }
    return onescheduleInfo;
}

QVector<ScheduleInfo> ScheduleDbManager::getScheduleInfo( const QDate date, int type )
{
    QMutexLocker mutexlocker(&gmutex);
    QVector<ScheduleInfo> scheduleInfoList;
    QSqlQuery query(m_dbSqlDatabase);
    query.prepare( "SELECT * FROM Events" );
    if ( !query.exec() ) return scheduleInfoList;
    QSqlRecord rec = query.record();
    int cols = rec.count();
    for ( int r = 0; query.next(); r++ ) {
        ScheduleInfo  onescheduleInfo;
        bool flag = false;
        for ( int c = 0; c < cols; c++ ) {
            if (rec.fieldName(c) == "_id") {
                onescheduleInfo.id = query.value(c).toInt();
            } else if (rec.fieldName(c) == "title") {
                onescheduleInfo.titleName = query.value(c).toString();
            } else if (rec.fieldName(c) == "eventColorid") {
                onescheduleInfo.infotype = query.value(c).toInt();
            } else if (rec.fieldName(c) == "description") {
                onescheduleInfo.description = query.value(c).toString();
            } else if (rec.fieldName(c) == "dstart") {
                onescheduleInfo.beginDateTime = QDateTime::fromString(query.value(c).toString(), "yyyyMMddhhmmsszzz");
                QDate tempdate = onescheduleInfo.beginDateTime.date();
                switch (type) {
                case 0:
                    if (tempdate == date) {
                        flag = true;
                    }
                    break;
                case 1: {
                    if (tempdate.year() == date.year() &&
                            tempdate.month() == date.month() &&
                            tempdate.weekNumber() == date.weekNumber()) {
                        flag = true;
                    }
                }

                break;
                case 2:
                    if (tempdate.year() == date.year() &&
                            tempdate.month() == date.month() ) {
                        flag = true;
                    }
                    break;
                case 3:
                    if (tempdate.year() == date.year() ) {
                        flag = true;
                    }
                    break;
                }
            } else if (rec.fieldName(c) == "dend") {
                onescheduleInfo.endDateTime = QDateTime::fromString(query.value(c).toString(), "yyyyMMddhhmmsszzz");
            } else if (rec.fieldName(c) == "allDay") {
                onescheduleInfo.allday = query.value(c).toInt();
            } else if (rec.fieldName(c) == "eventrepeatid") {
                onescheduleInfo.repeat = query.value(c).toInt();
            } else if (rec.fieldName(c) == "reminderid") {
                onescheduleInfo.remind = query.value(c).toInt();
            }
        }
        if (flag) scheduleInfoList.append(onescheduleInfo);
    }
    return scheduleInfoList;
}


QVector<ScheduleInfo> ScheduleDbManager::getALLDayScheduleInfo( const QDate date )
{
    QMutexLocker mutexlocker(&gmutex);
    QVector<ScheduleInfo> scheduleInfoList;
    QSqlQuery query(m_dbSqlDatabase);
    query.prepare( "SELECT * FROM Events" );
    if ( !query.exec() ) return scheduleInfoList;
    QSqlRecord rec = query.record();
    int cols = rec.count();
    for ( int r = 0; query.next(); r++ ) {
        ScheduleInfo  onescheduleInfo;
        for ( int c = 0; c < cols; c++ ) {
            if (rec.fieldName(c) == "_id") {
                onescheduleInfo.id = query.value(c).toInt();
            } else if (rec.fieldName(c) == "title") {
                onescheduleInfo.titleName = query.value(c).toString();
            } else if (rec.fieldName(c) == "eventColorid") {
                onescheduleInfo.infotype = query.value(c).toInt();
            } else if (rec.fieldName(c) == "description") {
                onescheduleInfo.description = query.value(c).toString();
            } else if (rec.fieldName(c) == "dstart") {
                onescheduleInfo.beginDateTime = QDateTime::fromString(query.value(c).toString(), "yyyyMMddhhmmsszzz");
            } else if (rec.fieldName(c) == "dend") {
                onescheduleInfo.endDateTime = QDateTime::fromString(query.value(c).toString(), "yyyyMMddhhmmsszzz");
            } else if (rec.fieldName(c) == "allDay") {
                onescheduleInfo.allday = query.value(c).toInt();
            } else if (rec.fieldName(c) == "eventrepeatid") {
                onescheduleInfo.repeat = query.value(c).toInt();
            } else if (rec.fieldName(c) == "reminderid") {
                onescheduleInfo.remind = query.value(c).toInt();
            }
        }

        if (onescheduleInfo.allday) {
            QDate btempdate = onescheduleInfo.beginDateTime.date();
            QDate endtempdate = onescheduleInfo.endDateTime.date();
            if (btempdate.daysTo(date) >= 0 && date.daysTo(endtempdate) >= 0) {
                scheduleInfoList.append(onescheduleInfo);
            }
        }
    }
    return scheduleInfoList;
}

QVector<ScheduleRemindInfo> ScheduleDbManager::getScheduleRemind( const QDate date )
{
    QMutexLocker mutexlocker(&gmutex);
    QVector<ScheduleRemindInfo> scheduleRemindList;
    QSqlQuery query(m_dbSqlDatabase);
    query.prepare( "SELECT * FROM Events" );
    if ( !query.exec() ) return scheduleRemindList;
    QSqlRecord rec = query.record();
    int cols = rec.count();
    for ( int r = 0; query.next(); r++ ) {
        ScheduleRemindInfo  oneRemindInfo;
        int remind = -1;
        for ( int c = 0; c < cols; c++ ) {
            if (rec.fieldName(c) == "_id") {
                oneRemindInfo.id = query.value(c).toInt();
            } else if (rec.fieldName(c) == "dstart") {
                oneRemindInfo.remindDateTime = QDateTime::fromString(query.value(c).toString(), "yyyyMMddhhmmsszzz");
            } else if (rec.fieldName(c) == "reminderid") {
                remind = query.value(c).toInt();
            }
        }
        QSqlQuery rmindquery(m_dbSqlDatabase);
        rmindquery.prepare( "SELECT minutes FROM Reminders Where reminderid = ?" );
        rmindquery.addBindValue(remind);
        if ( !rmindquery.exec() ) continue;
        if (rmindquery.next()) {
            int minnets = rmindquery.value(0).toInt();
            oneRemindInfo.remindDateTime = oneRemindInfo.remindDateTime.addSecs((qint64)minnets * (-60));
            if (oneRemindInfo.remindDateTime.date() == date) {
                scheduleRemindList.append(oneRemindInfo);
            }
        }
    }
    return scheduleRemindList;
}


QColor ScheduleDbManager::getTypeColor( int type )
{
    QMutexLocker mutexlocker(&gmutex);
    QSqlQuery query(m_dbSqlDatabase);
    query.prepare("SELECT eventColor FROM Colors Where eventColorid = ?" );
    query.addBindValue(type);
    if (!query.exec()) {
        return QColor();
    }
    if (query.next()) {
        QString colorname = query.value(0).toString();
        return QColor(colorname);
    }
    return QColor();
}

bool ScheduleDbManager::updateScheduleInfo( const ScheduleInfo &scheduledate )
{
    QMutexLocker mutexlocker(&gmutex);
    QSqlQuery query(m_dbSqlDatabase);

    query.prepare("UPDATE Events set title=? , description=? , dstart=? , dend=?, eventColorid=?,allDay=?,eventrepeatid=?,reminderid =? WHERE _id=?");
    query.addBindValue(scheduledate.titleName);
    query.addBindValue(scheduledate.description);
    query.addBindValue(scheduledate.beginDateTime.toString("yyyyMMddhhmmsszzz"));
    query.addBindValue(scheduledate.endDateTime.toString("yyyyMMddhhmmsszzz"));
    query.addBindValue(scheduledate.infotype);
    query.addBindValue(scheduledate.allday);
    query.addBindValue(scheduledate.repeat);
    query.addBindValue(scheduledate.remind);
    query.addBindValue(scheduledate.id);
    if (query.exec()) {
        return true;
    }
    return false;
}


bool ScheduleDbManager::deleteScheduleInfoById( int id )
{
    QMutexLocker mutexlocker(&gmutex);
    QSqlQuery query(m_dbSqlDatabase);

    query.prepare("DELETE from Events WHERE _id=?");

    query.addBindValue(id);

    if (query.exec()) {
        return true;

    }
    return false;
}

int ScheduleDbManager::initDataBase()
{
    if (m_initFlag) return -1;
    m_dbSqlDatabase = QSqlDatabase::addDatabase("QSQLITE");//创建数据库连接对象
    m_dbSqlDatabase.setDatabaseName(m_dbPath);
    if (!m_dbSqlDatabase.isDriverAvailable("QSQLITE")) {
        return -2;
    }
    if (!m_dbSqlDatabase.open()) {
        return -2;
    }
    QSqlQuery qry(m_dbSqlDatabase);
    QString t_Reminders = "SELECT COUNT(*) FROM sqlite_master where type ='table' and name ='Reminders'";
    if (!qry.exec(t_Reminders)) {
        return m_initFlag;
    }
    if (qry.next()) {
        if (qry.value(0).toInt() == 0) {
            qry.exec("CREATE TABLE Reminders (reminderid INTEGER PRIMARY KEY,eventtype CHAR,minutes INTEGER);");
            qry.exec("INSERT INTO Reminders (reminderid , eventtype , minutes) VALUES (0 , '5 min',5)");
            qry.exec("INSERT INTO Reminders (reminderid , eventtype , minutes) VALUES (1 , '10 min',10)");
            qry.exec("INSERT INTO Reminders (reminderid , eventtype , minutes) VALUES (2 , '60 min',60)");
            qry.exec("INSERT INTO Reminders (reminderid , eventtype , minutes) VALUES (3 , '120 min',120)");
            qry.exec("INSERT INTO Reminders (reminderid , eventtype , minutes) VALUES (4 , 'DAY',1440)");
            qry.exec("INSERT INTO Reminders (reminderid , eventtype , minutes) VALUES (5 , '3 DAY',4320)");
            qry.exec("INSERT INTO Reminders (reminderid , eventtype , minutes) VALUES (6 , '7 DAY',10080)");
        }
    }

    QString t_colortable = "SELECT COUNT(*) FROM sqlite_master where type ='table' and name ='Colors'";
    if (!qry.exec(t_colortable)) {
        return m_initFlag;
    }
    if (qry.next()) {
        if (qry.value(0).toInt() == 0) {
            qry.exec("CREATE TABLE Colors (eventColorid INTEGER PRIMARY KEY,eventtype CHAR NOT NULL,eventColor CHAR NOT NULL);");
            qry.exec("INSERT INTO Colors (eventColorid , eventtype , eventColor) VALUES (0 , 'Work' ,'#FF0033')");
            qry.exec("INSERT INTO Colors (eventColorid , eventtype , eventColor) VALUES (1 , 'life' ,'#33CC33')");
            qry.exec("INSERT INTO Colors (eventColorid , eventtype , eventColor) VALUES (2 , 'other','#9933CC')");
        }
    }

    QString t_EvnetRepeatable = "SELECT COUNT(*) FROM sqlite_master where type ='table' and name ='EvnetRepeat'";
    if (!qry.exec(t_EvnetRepeatable)) {
        return m_initFlag;
    }
    if (qry.next()) {
        if (qry.value(0).toInt() == 0) {
            qry.exec("CREATE TABLE EvnetRepeat (eventreapeatid INTEGER PRIMARY KEY,reapeattype CHAR NOT NULL);");
            qry.exec("INSERT INTO EvnetRepeat (eventreapeatid , reapeattype) VALUES (0 , 'Once Time')");
            qry.exec("INSERT INTO EvnetRepeat (eventreapeatid , reapeattype) VALUES (1 , 'Every Day')");
            qry.exec("INSERT INTO EvnetRepeat (eventreapeatid , reapeattype) VALUES (2 , 'Every Week')");
            qry.exec("INSERT INTO EvnetRepeat (eventreapeatid , reapeattype) VALUES (3 , 'Every Month')");
            qry.exec("INSERT INTO EvnetRepeat (eventreapeatid , reapeattype) VALUES (4 , 'Every Year')");
        }
    }
    QString t_eventtable = "SELECT COUNT(*) FROM sqlite_master where type ='table' and name ='Events'";
    if (!qry.exec(t_eventtable)) {
        return m_initFlag;
    }
    if (qry.next()) {
        if (qry.value(0).toInt() == 0) {
            qry.exec("CREATE TABLE Events (_id INTEGER PRIMARY KEY AUTOINCREMENT,title CHAR,description VARCHAR,dstart CHAR,dend CHAR,eventColorid INTEGER,allDay INTEGER NOT NULL DEFAULT 0,eventrepeatid INTEGER,reminderid INTEGER);");
        }
    }

    m_initFlag = true;
    return 0;
}

void ScheduleDbManager::close()
{
    m_dbSqlDatabase.close();
}




