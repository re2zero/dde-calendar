/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     uniontech  <uniontech@uniontech.com>
*
* Maintainer: uniontech  <chenhaifeng@uniontech.com>
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
#ifndef TIMEDATASTRUCT_H
#define TIMEDATASTRUCT_H

#include <QDateTime>
#include <QVector>
/**
 * @brief The ToDateTimeInfo struct 修改时间信息
 * m_Date   日期信息
 * m_Time   时间信息
 * hasDate  是否存在日期信息
 * hasTime  是否存在时间信息
 */
struct DateTimeInfo {
    QDate m_Date{};
    QTime m_Time{};
    bool  hasDate{false};
    bool  hasTime{false};
};
/**
 * @brief The SuggestDatetimeInfo struct  建议时间信息
 * datetime         时间信息
 * hasTime          是否存在具体时间点
 */
struct SuggestDatetimeInfo{
    QDateTime datetime {};
    bool hasTime {false};
};

/**
 * @brief The ScheduleDateTime struct   语言时间信息
 * m_DateTime       时间信息，获取时间点的时候需要用到
 * m_sugTime        建议使用的时间信息
 */
struct SemanticsDateTime {
    QVector<DateTimeInfo>  dateTime{};
    QVector<SuggestDatetimeInfo>  suggestDatetime{};
    /**
     * @brief clear 清空数据
     */
    void clear(){
        this->dateTime.clear();
        this->suggestDatetime.clear();
    }
};



#endif // TIMEDATASTRUCT_H
