#include "createschedulewidget.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QProcess>
#include <QPushButton>

#include "buttonwidget.h"
#include "../globaldef.h"

createSchedulewidget::createSchedulewidget(QWidget *parent)
    : IconDFrame(parent)
    , m_scheduleDtailInfo(ScheduleDtailInfo())
    , m_scheduleitemwidget(new scheduleitemwidget(this))
{
    connect(m_scheduleitemwidget, &scheduleitemwidget::signalItemPress, this, &createSchedulewidget::slotItemPress);
    //    updateUI();
}

ScheduleDtailInfo &createSchedulewidget::getScheduleDtailInfo()
{
    return m_scheduleDtailInfo;
}

void createSchedulewidget::setTitleName(QString titleName)
{
    m_titleName = titleName;
}

void createSchedulewidget::setDateTime(QDateTime begintime, QDateTime endtime)
{
    m_BeginDateTime = begintime;
    m_EndDateTime = endtime;
}

void createSchedulewidget::setRpeat(int rpeat)
{
    m_rpeat = rpeat;
}

void createSchedulewidget::setschedule()
{
    QDateTime m_beginTime;
    QDateTime m_endTime;
    m_beginTime = m_BeginDateTime;
    m_endTime = m_EndDateTime;

    m_scheduleDtailInfo.beginDateTime = m_beginTime;
    m_scheduleDtailInfo.endDateTime = m_endTime;
    m_scheduleDtailInfo.titleName = m_titleName;
    m_scheduleDtailInfo.type.ID = 3;
    m_scheduleDtailInfo.id = 0;
    //    m_scheduleDtailInfo.RecurID = 0;
    m_scheduleDtailInfo.allday = false;
    m_scheduleDtailInfo.remind = true;
    m_scheduleDtailInfo.remindData.n = 0;
    m_scheduleDtailInfo.rpeat = m_rpeat;
    //    if (m_rpeat != 0) {
    //    m_scheduleDtailInfo.enddata.type = 0;
    //    }
}

void createSchedulewidget::scheduleEmpty(bool isEmpty)
{
    m_scheduleEmpty = isEmpty;
}

void createSchedulewidget::updateUI()
{
    if (m_scheduleEmpty) {
        //创建日程
        QVector<ScheduleDtailInfo> scheduleInfo = getCreatScheduleFromDbus();
        if (!scheduleInfo.isEmpty()) {
            QVBoxLayout *mainlayout = new QVBoxLayout();
            m_scheduleitemwidget->setScheduleDtailInfo(scheduleInfo);
            m_scheduleitemwidget->addscheduleitem();
            mainlayout->addWidget(m_scheduleitemwidget);
            setCenterLayout(mainlayout);
        }
    } else {
        //是否创建日程
        QVBoxLayout *mainlayout = new QVBoxLayout();
        buttonwidget *button = new buttonwidget(this);
        button->addbutton(CANCEL_BUTTON_STRING, false, buttonwidget::ButtonRecommend);
        button->addbutton(CONFIRM_BUTTON_STRING);
        connect(button, &buttonwidget::buttonClicked, this, &createSchedulewidget::slotsbuttonchance);
        m_scheduleitemwidget->setScheduleDtailInfo(m_scheduleInfo);
        m_scheduleitemwidget->addscheduleitem();
        mainlayout->addWidget(m_scheduleitemwidget);
        mainlayout->addSpacing(4);
        mainlayout->addWidget(button);
        setCenterLayout(mainlayout);
    }
}

void createSchedulewidget::setScheduleDbus(CSchedulesDBus *dbus)
{
    m_dbus = dbus;
}

bool createSchedulewidget::buttonclicked()
{
    return m_buttonclicked;
}

void createSchedulewidget::slotsbuttonchance(int index, const QString &text)
{
    Q_UNUSED(text)
    if (index == 0) {
        m_buttonclicked = false;
    }
    if (index == 1) {
        m_buttonclicked = true;
    }
}

void createSchedulewidget::slotItemPress(const ScheduleDtailInfo &info)
{
    QProcess proc;
    proc.startDetached(PROCESS_OPEN_CALENDAR);
    QThread::msleep(750);
    QString schedulestr = CSchedulesDBus::createScheduleDtailInfojson(info);
    QDBusMessage message = QDBusMessage::createMethodCall(DBUS_CALENDAR_SERVICE,
                                                          DBUS_CALENDAR_PATCH,
                                                          DBUS_CALENDAR_INTFACE,
                                                          "OpenSchedule");
    message << schedulestr;
    //发送消息
    QDBusMessage response = QDBusConnection::sessionBus().call(message);
}

QVector<ScheduleDtailInfo> createSchedulewidget::getCreatScheduleFromDbus()
{
    QVector<ScheduleDtailInfo> scheduleInfo;
    QVector<ScheduleDateRangeInfo> CScheduleInfo;
    scheduleInfo.clear();
    CScheduleInfo.clear();

    m_dbus->QueryJobs(m_scheduleDtailInfo.titleName, m_scheduleDtailInfo.beginDateTime, m_scheduleDtailInfo.endDateTime, CScheduleInfo);

    for (int i = 0; i < CScheduleInfo.count(); i++) {
        for (int j = 0; j < CScheduleInfo.at(i).vData.count(); i++) {
            if (CScheduleInfo.at(i).vData.at(j).titleName == m_scheduleDtailInfo.titleName
                && CScheduleInfo.at(i).vData.at(j).beginDateTime == m_scheduleDtailInfo.beginDateTime
                && CScheduleInfo.at(i).vData.at(j).endDateTime == m_scheduleDtailInfo.endDateTime) {
                scheduleInfo.append(CScheduleInfo.at(i).vData.at(j));
                return scheduleInfo;
            }
        }
    }
    return scheduleInfo;
}
