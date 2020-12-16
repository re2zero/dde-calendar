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
#include "reminddata.h"

RemindData::RemindData()
    : m_RemindNum(-1)
    , m_RemindTime(9, 0, 0)
{

}

RemindData::RemindData(const int remindNum, const QTime &remindTime)
    : m_RemindNum(remindNum)
    , m_RemindTime(remindTime)
{

}

/**
 * @brief setRemindNum  设置提前多久提醒
 * @param num
 */
void RemindData::setRemindNum(const int num)
{
    m_RemindNum = num;
}

/**
 * @brief getRemindNum  获取提前多久提醒
 * @return
 */
int RemindData::getRemindNum() const
{
    return m_RemindNum;
}

/**
 * @brief setRemindTime 设置提醒时间
 * @param remindTime
 */
void RemindData::setRemindTime(const QTime &remindTime)
{
    m_RemindTime = remindTime;
}

/**
 * @brief getRemindTime 获取提醒时间
 * @return
 */
QTime RemindData::getRemindTime() const
{
    return  m_RemindTime;
}

/**
 * @brief RemindStringToRemindData      将sting转换为提醒数据
 * @param remindStr                     提醒的string数据
 * @return
 */
RemindData RemindData::RemindStringToRemindData(const QString &remindStr)
{
    RemindData _restuleRemindData;
    if (!remindStr.isEmpty()) {
        QStringList liststr = remindStr.split(";", QString::SkipEmptyParts);
        _restuleRemindData.setRemindNum(liststr.at(0).toInt());
        if (liststr.size() > 1) {
            _restuleRemindData.setRemindTime(QTime::fromString(liststr.at(1), "hh:mm"));
        }
    }
    return _restuleRemindData;
}

/**
 * @brief RemindDataToRemindString      将提醒数据转换为string
 * @param remindData                    提醒数据
 * @param isAllDay                      是否为全天
 * @return
 */
QString RemindData::RemindDataToRemindString(const RemindData &remindData, const bool isAllDay)
{
    QString _restuleStr{""};
    if (remindData.getRemindNum() != -1) {
        _restuleStr += QString::number(remindData.getRemindNum());
        if (isAllDay) {
            _restuleStr += QString(";%1").arg(remindData.getRemindTime().toString("hh:mm"));
        }
    }
    return _restuleStr;
}
