/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     chenhaifeng  <chenhaifeng@uniontech.com>
*
* Maintainer: chenhaifeng  <chenhaifeng@uniontech.com>
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
#include "scheduletask.h"

#include <QDebug>

CScheduleTask::CScheduleTask(QObject *parent)
    : QObject(parent)
{
    m_DBusManager = CScheduleDBus::getInstance();
    m_work = new DataGetWork(m_DBusManager);
    connect(&m_workerThread, &QThread::started, m_work, &DataGetWork::startQuery);
    connect(&m_workerThread, &QThread::finished, m_work, &QObject::deleteLater);
    connect(m_work, &DataGetWork::signalGetSchedule, this, &CScheduleTask::slotGetSchedule);
    connect(m_work, &DataGetWork::signalGetLunar, this, &CScheduleTask::slotGetLunar);
    connect(m_DBusManager, &CScheduleDBus::jobsUpdate, this, &CScheduleTask::jobsUpdate);
    m_work->moveToThread(&m_workerThread);
    m_workerThread.start();
}

CScheduleTask::~CScheduleTask()
{
    m_work->setStop(true);
    if (m_workerThread.isRunning()) {
        m_workerThread.quit();
        m_workerThread.wait();
    }
    CScheduleDBus::releaseInstance();
}

/**
 * @brief CScheduleTask::updateInfo                 更新缓存信息
 * @param startDate                                 开始时间
 * @param stopDate                                  结束时间
 */
void CScheduleTask::updateInfo(const QDate &startDate, const QDate &stopDate, const bool isGetLunar)
{
    m_queryScheduleInfo.clear();
    m_fullInfo.clear();
    if (isGetLunar) {
        m_huangliInfo.clear();
        m_festivalInfo.clear();
    }
    addQueryRange(startDate, stopDate, isGetLunar);
}

/**
 * @brief CScheduleTask::hasScheduleInfo        判断是否含有日程信息
 * @param startDate                             开始时间
 * @param stopDate                              结束时间
 * @return
 */
bool CScheduleTask::hasScheduleInfo(const QDate &startDate, const QDate &stopDate)
{
    return m_queryScheduleInfo.contains(startDate) && m_queryScheduleInfo.contains(stopDate);
}

/**
 * @brief CScheduleTask::getScheduleInfo        获取缓存中日程信息
 * @param startDate                             开始时间
 * @param stopDate                              结束时间
 * @return
 */
QMap<QDate, QVector<ScheduleDataInfo> > CScheduleTask::getScheduleInfo(const QDate &startDate, const QDate &stopDate)
{
    QMap<QDate, QVector<ScheduleDataInfo> > _resultInfo{};
    qint64 _offsetDay = startDate.daysTo(stopDate);
    QDate _infoDate = startDate;
    for (int i = 0; i <= _offsetDay; ++i) {
        _infoDate = startDate.addDays(i);
        if (m_queryScheduleInfo.contains(_infoDate)) {
            _resultInfo[_infoDate] = m_queryScheduleInfo[_infoDate];
        }
    }
    return _resultInfo;
}

/**
 * @brief CScheduleTask::getDateHasSchedule 获取日期是否含有日程标志
 * @return
 */
QMap<QDate, bool> CScheduleTask::getDateHasSchedule() const
{
    return m_fullInfo;
}

/**
 * @brief CScheduleTask::getHuangliInfo         获取缓存中农历信息
 * @param startDate
 * @param stopDate
 * @return
 */
QMap<QDate, CaHuangLiDayInfo> CScheduleTask::getHuangliInfo(const QDate &startDate, const QDate &stopDate)
{
    QMap<QDate, CaHuangLiDayInfo> _resultInfo{};
    qint64 _offsetDay = startDate.daysTo(stopDate);
    QDate _infoDate = startDate;
    for (int i = 0; i <= _offsetDay; ++i) {
        _infoDate = startDate.addDays(i);
        if (m_huangliInfo.contains(_infoDate)) {
            _resultInfo[_infoDate] = m_huangliInfo[_infoDate];
        }
    }
    return _resultInfo;
}

/**
 * @brief CScheduleTask::getFestivalInfo        获取缓存中班休信息
 * @param startDate
 * @param stopDate
 * @return
 */
QMap<QDate, int> CScheduleTask::getFestivalInfo(const QDate &startDate, const QDate &stopDate)
{
    QMap<QDate, int> _resultInfo{};
    qint64 _offsetDay = startDate.daysTo(stopDate);
    QDate _infoDate = startDate;
    for (int i = 0; i <= _offsetDay; ++i) {
        _infoDate = startDate.addDays(i);
        if (m_festivalInfo.contains(_infoDate)) {
            _resultInfo[_infoDate] = m_festivalInfo[_infoDate];
        }
    }
    return _resultInfo;
}

/**
 * @brief CScheduleTask::getSearchScheduleInfo          搜索日程
 * @param key                                           关键字
 * @param startTime                                     开始时间
 * @param endTime                                       结束时间
 * @return
 */
QMap<QDate, QVector<ScheduleDataInfo> > CScheduleTask::getSearchScheduleInfo(const QString &key, const QDateTime &startTime, const QDateTime &endTime)
{
    m_searchScheduleInfo.clear();
    m_searchScheduleInfoVector.clear();
    m_DBusManager->QueryJobs(key, startTime, endTime, m_searchScheduleInfo);
    //获取搜索到的日程信息
    QMap<QDate, QVector<ScheduleDataInfo> >::const_iterator _iterator = nullptr;
    for (_iterator = m_searchScheduleInfo.constBegin(); _iterator != m_searchScheduleInfo.constEnd(); ++_iterator) {
        for (int i = 0; i < _iterator->size(); ++i) {
            if (!m_searchScheduleInfoVector.contains(_iterator.value().at(i))) {
                m_searchScheduleInfoVector.append(_iterator.value().at(i));
            }
        }
    }
    //更新界面显示
    emit signalUpdateSearchSchedule();
    return m_searchScheduleInfo;
}

/**
 * @brief CScheduleTask::getSearchScheduleInfo      获取缓存中搜索结果
 * @return
 */
QMap<QDate, QVector<ScheduleDataInfo> > CScheduleTask::getSearchScheduleInfo() const
{
    return m_searchScheduleInfo;
}

/**
 * @brief CScheduleTask::getSearchScheduleInfoVector    获取缓存搜到的日程
 * @return
 */
QVector<ScheduleDataInfo> CScheduleTask::getSearchScheduleInfoVector() const
{
    return m_searchScheduleInfoVector;
}

/**
 * @brief CScheduleTask::clearSearchScheduleInfo        清空搜索日程信息
 */
void CScheduleTask::clearSearchScheduleInfo()
{
    m_searchScheduleInfo.clear();
    m_searchScheduleInfoVector.clear();
    //更新界面显示
    emit signalUpdateSearchSchedule();
}

/**
 * @brief CScheduleTask::addQueryRange      添加需要查询日程的时间范围
 * @param startDate                         开始时间
 * @param stopDate                          结束时间
 */
void CScheduleTask::addQueryRange(const QDate &startDate, const QDate &stopDate, const bool isGetLunar)
{
    m_work->addQueryRange(startDate, stopDate, isGetLunar);
}

/**
 * @brief CScheduleTask::slotGetSchedule        接收查询的日程信息
 * @param scheduleInfo
 * @param hasSchedule
 */
void CScheduleTask::slotGetSchedule(const QMap<QDate, QVector<ScheduleDataInfo> > &scheduleInfo, const QMap<QDate, bool> &hasSchedule)
{
    m_queryScheduleInfo = scheduleInfo;
    m_fullInfo = hasSchedule;
    emit signalUpdateScheduleShow();
}

void CScheduleTask::slotGetLunar(const QMap<QDate, CaHuangLiDayInfo> &lunarInfo, const QMap<QDate, int> &festivalInfo)
{
    m_huangliInfo = lunarInfo;
    m_festivalInfo = festivalInfo;
    emit signalLunarGetSuccess();
}

DataGetWork::DataGetWork(CScheduleDBus *_DataManage)
    : m_DataManage(_DataManage)
{

}

DataGetWork::~DataGetWork()
{

}

/**
 * @brief DataGetWork::setStop      设置日程停止
 * @param isStop
 */
void DataGetWork::setStop(bool isStop)
{
    m_stop = isStop;
    m_waitCondition.wakeAll();
}

/**
 * @brief DataGetWork::addQueryRange        添加查询范围
 * @param startDate                         开始时间
 * @param stopDate                          结束时间
 */
void DataGetWork::addQueryRange(const QDate &startDate, const QDate &stopDate, const bool isGetLunar)
{
    m_mutex.lock();
    QueryRange _queryRange{startDate, stopDate};
    m_isGetLunar = isGetLunar;
    //添加查询
    m_queryScheduleRange.append(_queryRange);
    m_mutex.unlock();
    //唤起线程
    m_waitCondition.wakeAll();
}

/**
 * @brief DataGetWork::startQuery   开始查询
 */
void DataGetWork::startQuery()
{
    while (!m_stop) {
        //如果需要查询日期的缓存不为空则开始查询
        if (m_queryScheduleRange.size() > 0) {
            QueryRange _queryRange;
            m_mutex.lock();
            //获取最后一个
            _queryRange = m_queryScheduleRange.last();
            //清空
            m_queryScheduleRange.clear();
            m_mutex.unlock();
            //是否获取农历信息
            if (m_isGetLunar) {
                queryLunarInfo(_queryRange);
            }
            queryScheduleInfo(_queryRange);
        } else {
            //否则线程挂起
            m_mutex.lock();
            m_waitCondition.wait(&m_mutex);
            m_mutex.unlock();
        }
    }
}

/**
 * @brief DataGetWork::queryScheduleInfo        查询日程信息
 */
void DataGetWork::queryScheduleInfo(const QueryRange &queryRange)
{
    QMap<QDate, QVector<ScheduleDataInfo> > _queryScheduleInfo{};
    QMap<QDate, bool> _dateHasSchedule{};
    //查询日程数据
    m_DataManage->GetJobs(queryRange._startDate, queryRange._stopDate, _queryScheduleInfo);
    QMap<QDate, QVector<ScheduleDataInfo> >::const_iterator _infoIter = _queryScheduleInfo.constBegin();
    //遍历确认这些日期是否包含日程
    for (; _infoIter != _queryScheduleInfo.constEnd(); ++_infoIter) {
        _queryScheduleInfo[_infoIter.key()] = _infoIter.value();
        _dateHasSchedule[_infoIter.key()] = _infoIter.value().size() > 0 ? true : false;
    }
    //如果缓存为空则更新
    if (m_queryScheduleRange.size() == 0)
        emit signalGetSchedule(_queryScheduleInfo, _dateHasSchedule);
}

void DataGetWork::queryLunarInfo(const QueryRange &queryRange)
{
    bool _fill = false;
    CaHuangLiMonthInfo _monthInfo;
    QMap<QDate, CaHuangLiDayInfo> _huangliYear{};
    QMap<QDate, int> _festivalYear{};
    quint32 _year ;
    quint32 _month ;
    QDate _beginDate;
    QVector<FestivalInfo> _festivallist{};
    //获取开始时间和结束时间相差多少个月
    const int _offsetMonth = (queryRange._stopDate.year() - queryRange._startDate.year()) * 12 + queryRange._stopDate.month() - queryRange._startDate.month();
    //获取开始时间至结束时间所在月的农历和节假日信息
    for (int i = 0; i <= _offsetMonth; ++i) {
        _monthInfo.clear();
        _beginDate = queryRange._startDate.addMonths(i);
        _year = static_cast<quint32>(_beginDate.year());
        _month = static_cast<quint32>(_beginDate.month());
        m_DataManage->GetFestivalMonth(_year, _month, _festivallist);
        m_DataManage->GetHuangLiMonth(_year, _month, _fill, _monthInfo);
        QDate _getDate(_beginDate.year(), _beginDate.month(), 1);
        Q_ASSERT(_monthInfo.mDays == _monthInfo.mCaLunarDayInfo.size());
        for (int j = 0; j < _monthInfo.mDays; ++j) {
            _huangliYear[_getDate.addDays(j)] = _monthInfo.mCaLunarDayInfo.at(j);
        }
    }
    QDate _queryDate;
    //获取全年的班休信息
    for (int i = 0; i <= queryRange._startDate.daysTo(queryRange._stopDate); ++i) {
        _queryDate  = queryRange._startDate.addDays(i);
        _festivalYear[_queryDate] = getFestivalInfoByDate(_queryDate, _festivallist);
    }
    emit signalGetLunar(_huangliYear, _festivalYear);
}

char DataGetWork::getFestivalInfoByDate(const QDate &date, const QVector<FestivalInfo> &festivalInfo)
{
    for (int i = 0; i < festivalInfo.count(); i++) {
        for (int j = 0; j < festivalInfo[i].listHoliday.count(); j++) {
            if (festivalInfo[i].listHoliday[j].date == date) {
                return festivalInfo[i].listHoliday[j].status;
            }
        }
    }
    return 0;
}
