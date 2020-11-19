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
#include "calendardatedatamanage.h"

#include <QtMath>

/**
 * @brief CalendarDateDataManage    构造函数
 * @param parent
 */
CalendarDateDataManage::CalendarDateDataManage()
    : m_currentDateTime(QDateTime::currentDateTime())
    ,m_selectDate(m_currentDateTime.date())
    ,m_weekFirstDay(Qt::Sunday)
    ,m_weekDayFormat("ddd")
{

}
/**
 * @brief setSelectDate     设置选择时间
 * @param selectDate        选择时间
 */
void CalendarDateDataManage::setSelectDate(const QDate &selectDate)
{
    m_selectDate = selectDate;
}
/**
 * @brief getSelectDate     获取选择时间
 * @return                  返回选择时间
 */
QDate CalendarDateDataManage::getSelectDate() const
{
    return m_selectDate;
}
/**
 * @brief setCurrentDateTime        设置当前时间
 * @param currentDateTime           当前时间
 */
void CalendarDateDataManage::setCurrentDateTime(const QDateTime &currentDateTime)
{
    m_currentDateTime = currentDateTime;
}
/**
 * @brief getCurrentDate    获取当前时间
 * @return                  返回当前时间
 */
QDateTime CalendarDateDataManage::getCurrentDate() const
{
    return  m_currentDateTime;
}
/**
 * @brief getYearDate       获取全年的时间
 * @param year              设置的年份
 * @return                  返回全年的时间，按照月份分组
 */
QMap<int,QVector<QDate> > CalendarDateDataManage::getYearDate(const int &year)
{
    QMap<int ,QVector<QDate> > _resultMap;
    for (int i = 1; i < 13; ++i) {
        _resultMap[i] = getMonthDate(year,i);
    }
    return _resultMap;
}
/**
 * @brief getMonthDate      获取月份的所有时间
 * @param year              设置的年份
 * @param month             设置的月份
 * @return                  返回当月全部时间
 */
QVector<QDate> CalendarDateDataManage::getMonthDate(const int &year, const int &month)
{
    QVector<QDate> _resultDate;
    //自然月的第一天
    const QDate _monthFirstDay{year,month,1};
    //获取显示月的第一天
    const QDate _firstShowDayOfMonth = getFirstDayOfWeek(_monthFirstDay);
    //获取该月所有显示时间
    for (int i = 0; i < 42; ++i) {
        _resultDate.append(_firstShowDayOfMonth.addDays(i));
    }
    return _resultDate;
}
/**
 * @brief getWeekDate       获取一周的所有时间
 * @param date              设置的时间
 * @return                  返回这个周全部时间
 */
QVector<QDate> CalendarDateDataManage::getWeekDate(const QDate &date)
{
    QVector<QDate> _resultDate;
    //获取这个周的第一天日期
    const QDate _firstDayofWeek = getFirstDayOfWeek(date);
    //获取该周所有显示时间
    for (int i = 0; i<7; ++i) {
        _resultDate.append(_firstDayofWeek.addDays(i));
    }
    return _resultDate;
}
/**
 * @brief getDayDateBySelectDate    根据选择时间获取当天日期
 * @return                  返回选择时间
 */
QDate CalendarDateDataManage::getDayDateBySelectDate() const
{
    return m_selectDate;
}
/**
 * @brief setWeekFirstDay           设置每周以周几作为每周第一天
 * @param firstDay                  每周第一天
 */
void CalendarDateDataManage::setWeekFirstDay(const Qt::DayOfWeek &firstDay)
{
    m_weekFirstDay = firstDay;
}
/**
 * @brief getWeekFirstDay           获取每周以周几作为每周第一天
 * @return                          每周第一天
 */
Qt::DayOfWeek CalendarDateDataManage::getWeekFirstDay() const
{
    return  m_weekFirstDay;
}
/**
 * @brief setWeekDayFormatByID          设置周显示格式
 * @param weekDayFormatID
 *                      0 "dddd"
 *                      1 "ddd"
 */
void CalendarDateDataManage::setWeekDayFormatByID(const int &weekDayFormatID)
{
    switch (weekDayFormatID) {
    case 0:
        m_weekDayFormat = "dddd";
        break;
    default:
        m_weekDayFormat = "ddd";
        break;
    }
}
/**
 * @brief getWeekDayFormat          获取周显示格式
 * @return                          周显示格式
 * "ddd"    周一
 * "dddd"   星期一
 */
QString CalendarDateDataManage::getWeekDayFormat() const
{
    return m_weekDayFormat;
}
/**
 * @brief getFirstDayOfWeek         根据日期获取当前周第一天的日期
 * @param date                      选择的日期
 * @return                          当前周第一天的日期
 */
QDate CalendarDateDataManage::getFirstDayOfWeek(const QDate &date)
{
    //根据选择时间周工作日和每周第一天的周工作日得到偏移量
    int _offset = date.dayOfWeek() - m_weekFirstDay;
    //根据偏移量获取需要添加还有减去的偏移天数
    const int _offsetDay = _offset <0 ?_offset +7:_offset;
    //返回这周第一天的日期
    return date.addDays(0 - _offsetDay);
}
/**
 * @brief getWeekNumOfYear		根据日期获取该日期处于该年第多少周
 * @param date                  选择的日期
 * @return                      处于当年第多少周
 */
int CalendarDateDataManage::getWeekNumOfYear(const QDate &date)
{
    int _weekNum {0};
    //该年第一天
    const QDate _firstDayOfYear{date.year(),1,1};
    //该年显示的第一天日期
    const QDate _firstShowDayOfYear = getFirstDayOfWeek(_firstDayOfYear);
    //处于该年显示第多少天
    const qint64  _dayOfShowYear = _firstShowDayOfYear.daysTo(_firstDayOfYear) + date.dayOfYear();
    _weekNum = qFloor(_dayOfShowYear/7) + 1;
    return  _weekNum;
}
