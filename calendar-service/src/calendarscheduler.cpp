/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     kongyunzhen <kongyunzhen@uniontech.com>
*
* Maintainer: kongyunzhen <kongyunzhen@uniontech.com>
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
#include "calendarscheduler.h"
#include "src/commondatastruct.h"
#include "src/utils.h"
#include "lunarmanager.h"
#include "pinyin/pinyinsearch.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMutexLocker>
#include <QDebug>

#define RECURENCELIMIT 3650 //递归次数限制

QMap<QString, quint32> CalendarScheduler::m_festivalIdMap;
quint32 CalendarScheduler::nextFestivalJobId = INT32_MAX;
QMutex CalendarScheduler::m_getJobIdMutex;
CalendarScheduler::CalendarScheduler(QObject *parent)
    : QObject(parent)
    , m_database(new SchedulerDatabase(this))
{
    IsFestivalJobEnabled();
}

QString CalendarScheduler::GetType(qint64 id)
{
    QString strres;
    foreach (auto var, globalPredefinedTypes) {
        if (var.ID == id) {
            //tojson
            QJsonObject obj;
            obj.insert("ID", static_cast<int>(var.ID));
            obj.insert("Name", var.Name);
            obj.insert("Color", var.Color);
            QJsonDocument doc;
            doc.setObject(obj);
            strres = QString::fromUtf8(doc.toJson());
            break;
        }
    }
    return strres;
}

QString CalendarScheduler::GetTypes()
{
    QString strres;
    QJsonArray jsonArray;
    QJsonDocument doc;
    foreach (auto var, globalPredefinedTypes) {
        QJsonObject obj;
        obj.insert("ID", static_cast<int>(var.ID));
        obj.insert("Name", var.Name);
        obj.insert("Color", var.Color);
        jsonArray.append(obj);
    }
    doc.setArray(jsonArray);
    strres = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
    return strres;
}

void CalendarScheduler::DeleteJob(qint64 id)
{
    m_database->DeleteJob(id);
}

void CalendarScheduler::DeleteType(qint64 id)
{
    m_database->DeleteType(id);
}


QString CalendarScheduler::GetJob(qint64 id)
{
    return m_database->GetJob(id);
}

// 给定日程Json信息，解析为job类型传入数据库
qint64 CalendarScheduler::CreateJob(const QString &jobInfo)
{
    // 现将给的Json信息转为Job类型
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jobInfo.toLocal8Bit(), &json_error));

    if (json_error.error != QJsonParseError::NoError) {
        return false;
    }

    QJsonObject rootObj = jsonDoc.object();
    Job job;

    if (rootObj.contains("ID")) {
        job.ID = rootObj.value("ID").toInt();
    }
    if (rootObj.contains("Type")) {
        job.Type = rootObj.value("Type").toInt();
    }
    if (rootObj.contains("Title")) {
        job.Title = rootObj.value("Title").toString();
    }
    if (rootObj.contains("Description")) {
        job.Description = rootObj.value("Description").toString();
    }
    if (rootObj.contains("AllDay")) {
        job.AllDay = rootObj.value("AllDay").toBool();
    }
    if (rootObj.contains("Start")) {
        // 此处时间转换为与client同样式
        job.Start = QDateTime::fromString(rootObj.value("Start").toString(), Qt::ISODate);
    }
    if (rootObj.contains("End")) {
        job.End = QDateTime::fromString(rootObj.value("End").toString(), Qt::ISODate);
    }
    if (rootObj.contains("RRule")) {
        job.RRule = rootObj.value("RRule").toString();
    }
    if (rootObj.contains("Remind")) {
        job.Remind = rootObj.value("Remind").toString();
    }
    if (rootObj.contains("Ignore")) {
        QJsonArray subArray = rootObj.value("Ignore").toArray();
        QJsonDocument doc;
        doc.setArray(subArray);
        job.Ignore = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
    }
    if (rootObj.contains("Title_pinyin")) {
        job.Title_pinyin = pinyinsearch::getPinPinSearch()->CreatePinyin(rootObj.value("Title").toString());
    }

    return m_database->CreateJob(job);
}

// 可将要改动的日程信息直接传入数据库操作层中
void CalendarScheduler::UpdateJob(const QString &jobInfo)
{
    m_database->UpdateJob(jobInfo);
}

// 可将要改动的日程类型信息直接传入数据库操作层中
void CalendarScheduler::UpdateType(const QString &typeInfo)
{
    m_database->UpdateType(typeInfo);
}

/**
 * @brief  GetJobs 获取指定范围内的日程
 * @param start 起始时间
 * @param end 结束时间
 * @return 返回指定范围内的日程JSON格式
 */
QString CalendarScheduler::GetJobs(const QDateTime &start, const QDateTime &end)
{
    QString strJson;
    QList<Job> joblist = m_database->GetAllOriginJobs();
    QList<stJobArr> jobArrList = GetJobsBetween(start, end, joblist); //获取时间范围内所有的Job
    strJson = JobArrListToJsonStr(jobArrList);
    return strJson;
}

/**
 * @brief  QueryJobs 查询指定时间范围日程
 * @param params 查询条件json字符串
 * @return 返回指定范围内的日程JSON格式
 */
QString CalendarScheduler::QueryJobs(const QString &params)
{
    QJsonDocument doc;
    QJsonParseError err;
    doc = QJsonDocument::fromJson(params.toUtf8(), &err);
    QJsonObject obj = doc.object();
    QString strKey = obj.value("Key").toString();
    QDateTime starttime = Utils::fromconvertData(obj.value("Start").toString());
    QDateTime endtime = Utils::fromconvertData(obj.value("End").toString());
    QList<Job> joblist = m_database->GetAllOriginJobs(strKey);
    QList<stJobArr> jobArrList = GetJobsBetween(starttime, endtime, joblist, strKey);
    QString strJson = JobArrListToJsonStr(jobArrList);
    qDebug() << strJson;
    return strJson;
}

//检测当前环境是否为中文环境来决定是否开启获取节日日程开关
void CalendarScheduler::IsFestivalJobEnabled()
{
    QString str_lcall(qgetenv("LC_ALL").data());
    QString str_language = qgetenv("LANGUAGE");
    QString str_lang = qgetenv("LANG");
    qDebug() << str_lcall << str_language << str_lang;
    if (("C" != str_lcall) && (str_language.startsWith("zh") || str_lang.startsWith("zh"))) {
        m_festivalJobEnabled = true;
    }
}

QList<stJobArr> CalendarScheduler::GetJobsBetween(const QDateTime &start, const QDateTime &end, const QList<Job> &joblist, const QString &querykey, bool bextend)
{
    QList<stJobArr> jobArrList;
    int days = start.daysTo(end);
    for (int i = 0; i <= days; ++i) {
        stJobArr arr;
        QDateTime tem = start.addDays(i);
        qDebug() << tem.date();
        arr.date = tem.date();
        jobArrList.append(arr);
    }
    //判断是否需要获取中国节日日程
    if (m_festivalJobEnabled) {
        FillFestivalJobs(start, end, querykey, jobArrList);
    }

    foreach (Job jb, joblist) {
        qint64 interval = jb.Start.secsTo(jb.End); //当前job开始结束时间差，单位秒
        QList<stJobTime> jobtimelist = GetJobTimesBetween(start, end, jb);
        if (jobtimelist.isEmpty()) {
            continue;
        }
        foreach (stJobTime jobtime, jobtimelist) {
            Job jobtem = jb;
            if (0 != jobtime.recurID) {
                jobtem.Start = jobtime.start;
                jobtem.End = jobtime.start.addSecs(interval);
                jobtem.RecurID = jobtime.recurID;
            }
            int idx = start.daysTo(jobtime.start);
            if (idx >= 0 && idx <= days) {
                jobArrList[idx].jobs.append(jobtem);
            }

            //  extend 指把跨越多天的 job 给扩展开，比如开始日期为 2019-09-01 结束日期为 2019-09-02 的
            // job, 将会扩展出一个job 放入 2019-09-02 那天的 extendJobs 字段中。
            if (!bextend) {
                continue;
            }

            int extenddays = jobtime.start.daysTo(jobtime.end);
            for (int i = 0; i < extenddays; ++i) {
                int tIdx = idx + i + 1;
                if (tIdx == jobArrList.size()) {
                    break;
                }
                if (tIdx < 0) {
                    continue;
                }
                jobArrList[tIdx].extends.append(jobtem);
            }
        }
    }
    return jobArrList;
}

QList<stJobTime> CalendarScheduler::GetJobTimesBetween(const QDateTime &start, const QDateTime &end, const Job &job)
{
    QList<stJobTime> jobtimelist;
    if (start <= end) {
        QList<QDateTime> igonrelist = GetIgnoreList(job);
        //如果没有规则，则直接判断其开始结束时间就可以了
        if (job.RRule.isEmpty()) {
            //开始时间小于job的结束时间并且结束时间要大于job的开始时间，保证有交集,保证
            if (start <= job.End && end >= job.Start) {
                stJobTime jobtime;
                jobtime.start = job.Start;
                jobtimelist.append(jobtime);
            }
            return jobtimelist;
        }

        int count = 0; //当前为日程的第几次重复
        stRRuleOptions options = ParseRRule(job.RRule);
        QDateTime jobstart = job.Start; //当前原始job的起始时间
        QDateTime jobend = job.End; //当前原始job的结束时间
        int dateinterval = jobstart.daysTo(jobend); //job的开始结束间隔日期
        QDateTime next = jobstart; //next为下一新建日程起始日期
        //只有当下一个新建日程的起始日期小于查询日期的结束日期才会有交集，否则没有意义
        //注意一定要判断是否有交集，因为Job的创建日期可能远远早于查询日期，查询到的是多次重复后与查询时间有交集的
        while (true) {
            QDateTime copystart = next;
            if (copystart > end) {
                //起始日期超出查询结束日期直接退出
                break;
            }

            QDateTime copyend = next.addDays(dateinterval);
            //如果查询时间范围和Job时间范围有交集则保存
            //另外需要保证该新建任务没有被删除即未被ignore，新建任务重复的规则删除是删除该次重复包含的的所有天，
            if (OverLap(start, end, copystart, copyend) && !ContainsInIgnoreList(igonrelist, copystart)) {
                stJobTime jt;
                jt.start = copystart;
                jt.end = copyend;
                jt.recurID = count;
                jobtimelist.append(jt);
            }
            count++;
            //当结束重复为按多少次结束判断时，检查重复次数是否达到，达到则退出
            //当重复次数达到最大限制直接返回
            //options.tcount表示重复的次数，而count表示总次数，所以这里不能有“=”
            if ((options.type == RepeatOverCount && options.tcount < count)
                    || count > RECURENCELIMIT) {
                break;
            }
            //根据rule获取下一个Job的起始日期
            next = GetNextJobStartTimeByRule(options, copystart);
            //判断next是否有效,时间大于RRule的until
            //判断next是否大于查询的截止时间
            if ((options.type == RepeatOverUntil && next >= options.overdate)
                    || next > end) {
                break;
            }
            copystart = next;
        }
    } else {
        qDebug() << __FUNCTION__ << "start time later than end time param error! do nothoing";
    }

    return jobtimelist;
}

/**
 * @brief  FillFestivalJobs 获取指定范围内的节日日程信息
 * @param start 起始时间
 * @param end 结束时间
 */
void CalendarScheduler::FillFestivalJobs(const QDateTime &start, const QDateTime &end, const QString &querykey, QList<stJobArr> &listjob)
{
    QList<stDayFestival> festivaldays = GetFestivalsInRange(start, end);

    if (!querykey.isEmpty()) {
        festivaldays = FilterDayFestival(festivaldays, querykey);
    }

    foreach (stDayFestival day, festivaldays) {
        int index = start.daysTo(day.date);
        if (index >= 0 && index < listjob.size()) {
            foreach (QString festival, day.Festivals) {
                if (!festival.isEmpty()) {
                    Job jb;
                    jb.Title = festival;
                    jb.Type = JobTypeFestival;
                    jb.AllDay = true;
                    jb.Start = day.date;
                    jb.End = day.date;
                    jb.RRule = "FREQ=YEARLY";
                    jb.ID = GetFestivalId(festival);
                    listjob[index].jobs.append(jb);
                }
            }
        }
    }
}

/**
 * @brief  GetIgnoreList 获取指定Job的忽略列表
 * @param job 查询的Job
 * @return 当前Job的忽略列表
 */
QList<QDateTime> CalendarScheduler::GetIgnoreList(const Job &job)
{
    QList<QDateTime> list;
    if (!job.Ignore.isEmpty()) {
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(job.Ignore.toLocal8Bit(), &err);
        if (QJsonParseError::NoError == err.error) {
            QJsonArray arr = doc.array();
            for (int i = 0; i < arr.count(); ++i) {
                QString str = arr.at(i).toString();
                QDateTime datetime = QDateTime::fromString(str, Qt::ISODate);
                list.append(datetime);
            }
        }
    }
    return list;
}

/**
 * @brief  ContainsInIgnoreList 检查日期是否在忽略列表里
 * @param ignorelist 日期忽略列表
 * @param start 需要检测时间
 * @return bool 如果在忽略列表中则返回true
 */
bool CalendarScheduler::ContainsInIgnoreList(const QList<QDateTime> ignorelist, const QDateTime &time)
{
    return ignorelist.contains(time);
}

/**
 * @brief  OverLap 判断两个时间范围是否有交集
 * @param start 第一个时间范围的开始时间
 * @param end 第一个时间范围的结束时间
 * @param jobstart 第二个时间范围的开始时间
 * @param jobend 第二个时间范围的结束时间
 * @return bool 有交集返回true否则返回false
 */
bool CalendarScheduler::OverLap(const QDateTime &start, const QDateTime &end, const QDateTime &jobstart, const QDateTime &jobend)
{
    bool boverlap = false;
    //只需要判断date即可，不需要比较time
    if ((start.date() <= jobstart.date() && end.date() >= jobstart.date())
            || (start.date() >= jobstart.date() && start.date() <= jobend.date())) {
        boverlap = true;
    }
    return boverlap;
}

/**
 * @brief  GetNextJobStartTimeByRule 根据规则获取下一个job开始时间
 * @param options 包含重复规则相关字段结构体
 * @param datetime 上一个Job的时间
 * @return QDateTime 下一个重复Job的起始时间
 */
QDateTime CalendarScheduler::GetNextJobStartTimeByRule(const stRRuleOptions &options, const QDateTime &datetime)
{
    QDateTime next;
    quint8 dayofweek;
    switch (options.rpeat) {
    case RepeatDaily:
        next = datetime.addDays(1);
        break;
    case RepeatWorkDay:
        dayofweek = datetime.date().dayOfWeek();
        //计算当前为周几如果是周五需要跳过周六周日，否则直接下一天
        if (dayofweek == Qt::Friday) {
            next = datetime.addDays(3);
        } else {
            next = datetime.addDays(1);
        }
        break;
    case RepeatWeekly:
        next = datetime.addDays(7);
        break;
    case RepeatMonthly:
        next = datetime.addMonths(1);
        break;
    case RepeatYearly:
        next = datetime.addYears(1);
        break;
    default:
        break;
    }

    return next;
}

/**
 * @brief  ParseRRule 解析重复规则
 * @param rule 规则字符串
 * @return stRRuleOptions 包含重复规则相关字段的结构体
 */
stRRuleOptions CalendarScheduler::ParseRRule(const QString &rule)
{
    //无规则的不走这里判断所以此处默认rule不为空
    stRRuleOptions options;
    QStringList rruleslist = rule.split(";", QString::SkipEmptyParts);
    //rpeat重复规则 0 无  1 每天 2 每个工作日 3 每周 4每月 5每年
    //type结束重复类型 0 永不 1  多少次结束  2 结束日期
    if (rruleslist.contains("FREQ=DAILY") && rruleslist.contains("BYDAY=MO,TU,WE,TH,FR")) {
        options.rpeat = RepeatWorkDay;
    } else if (rruleslist.contains("FREQ=DAILY")) {
        options.rpeat = RepeatDaily;
    } else if (rruleslist.contains("FREQ=WEEKLY")) {
        options.rpeat = RepeatWeekly;
    } else if (rruleslist.contains("FREQ=MONTHLY")) {
        options.rpeat = RepeatMonthly;
    } else if (rruleslist.contains("FREQ=YEARLY")) {
        options.rpeat = RepeatYearly;
    }

    for (int i = 0; i < rruleslist.count(); i++) {
        if (rruleslist.at(i).contains("COUNT=")) {
            QStringList liststr = rruleslist.at(i).split("=", QString::SkipEmptyParts);
            options.type = RepeatOverCount;
            options.tcount = liststr.at(1).toInt() - 1;
        }

        if (rruleslist.at(i).contains("UNTIL=")) {
            QStringList liststr = rruleslist.at(i).split("=", QString::SkipEmptyParts);
            options.type = RepeatOverUntil;
            options.overdate = QDateTime::fromString(liststr.at(1).left(liststr.at(1).count() - 1), "yyyyMMddThhmmss");
            options.overdate = options.overdate.addDays(1);
        }
    }
    return options;
}

/**
 * @brief  JobToObject 将Job转换成QJsonObject
 * @param job Job结构体
 * @return QJsonObject
 */
QJsonObject CalendarScheduler::JobToObject(const Job &job)
{
    QJsonObject obj;
    obj.insert("ID", job.ID);
    obj.insert("Type", job.Type);
    obj.insert("Title", job.Title);
    obj.insert("Description", job.Description);
    obj.insert("AllDay", job.AllDay);
    obj.insert("Start", Utils::toconvertData(job.Start));
    obj.insert("End", Utils::toconvertData(job.End));
    obj.insert("RRule", job.RRule);
    obj.insert("Remind", job.Remind);
    obj.insert("Ignore", job.Ignore);
    obj.insert("RecurID", job.RecurID);

    return obj;
}

/**
 * @brief  JobArrListToJsonStr 将jobArrList转化为json字符串
 * @param jobArrList jobArrList
 * @return json字符串
 */
QString CalendarScheduler::JobArrListToJsonStr(const QList<stJobArr> &jobArrList)
{
    QString strJson;
    QJsonDocument doc;
    QJsonArray jsonarr;
    foreach (stJobArr jobarr, jobArrList) {
        QJsonObject obj;
        QJsonArray jobsJsonArr;
        QJsonObject objjob;
        obj.insert("Date", jobarr.date.toString("yyyy-MM-dd"));
        foreach (Job job, jobarr.jobs) {
            objjob = JobToObject(job);
            jobsJsonArr.append(objjob);
        }
        foreach (Job job, jobarr.extends) {
            objjob = JobToObject(job);
            if (!jobarr.jobs.isEmpty()) {
                jobsJsonArr.append(objjob);
            }
        }
        obj.insert("Jobs", jobsJsonArr);
        jsonarr.append(obj);
    }
    doc.setArray(jsonarr);
    strJson = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
    return strJson;
}

/**
 * @brief  GetFestivalId 根据节日名称获取节日id
 * @param name 节日名称
 * @return 节日id
 */
quint32 CalendarScheduler::GetFestivalId(const QString &name)
{
    //自动加锁，函数结束销毁时自动解锁
    QMutexLocker lock(&m_getJobIdMutex);
    quint32 id = 0;
    auto it = m_festivalIdMap.find(name);
    if (it != m_festivalIdMap.end()) {
        id = it.value();
    } else {
        id = nextFestivalJobId;
        m_festivalIdMap.insert(name, id);
        nextFestivalJobId--;
    }
    return id;
}
