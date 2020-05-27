#ifndef DBUSDATAGETTHREAD_H
#define DBUSDATAGETTHREAD_H

#include <QObject>
#include <QThread>

#include "calendardbus.h"
#include "schedulestructs.h"

class CSchedulesDBus;
class DataGetWork;
class CaLunarMonthInfo;

struct YearScheduleInfo {
    int                             m_year;
    QMap<int,QDate>                 m_firstDay;
    QMap<int,QVector<ScheduleDateRangeInfo> >     m_monthInfo;
};
Q_DECLARE_METATYPE(YearScheduleInfo);

struct YearLunar {
    int                 m_year;
    QMap<int,CaLunarMonthInfo *>          m_monthLunar;
};

struct YearFestival {
    int                 m_year;
    QMap<QDate,int>     m_dayFestival;      //1:xiu 2:ban
};


class DbusDataGetThread : public QObject
{
    Q_OBJECT
public:
    explicit DbusDataGetThread(CSchedulesDBus *_DataManage,QObject *parent = nullptr);
    ~DbusDataGetThread();
signals:
    //获取日程信息
    void getScheduleInfo(YearScheduleInfo *ScheduleInfo);
    //获取阴历信息
    void getLunarInfo(YearLunar *lunarInfo);
    //获取节假日信息
    void getFestivalInfo(YearFestival *festivalInfo);

    void signalsgetInfo(const QString &str);

signals:
    void signalGetScheduleSuccess();

public slots:
private:
    QThread                 workerThread;
    DataGetWork             *m_work;
};


class DataGetWork :public QObject
{
    Q_OBJECT
public:
    explicit DataGetWork(CSchedulesDBus *_DataManage);
    ~DataGetWork();
public:
    //获取日程信息
    void getScheduleInfo(YearScheduleInfo *ScheduleInfo);
    //获取阴历信息
    void getLunarInfo(YearLunar *lunarInfo);
    //获取节假日信息
    void getFestivalInfo(YearFestival *festivalInfo);

signals:
    void signalGetScheduleSuccess();

private:
    CSchedulesDBus       *m_DataManage;

};

#endif // DBUSDATAGETTHREAD_H
