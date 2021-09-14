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
#ifndef REMINDDATA_H
#define REMINDDATA_H

#include <QTime>
/**
 * @brief The RemindData class
 * 提醒规则
 */
class RemindData
{
public:
    explicit RemindData();
    RemindData(const int remindNum, const QTime &remindTime);
    //设置提前多久提醒
    void setRemindNum(const int num);
    //获取提前多久提醒
    int getRemindNum() const;
    //设置提醒时间
    void setRemindTime(const QTime &remindTime);
    //获取提醒时间
    QTime getRemindTime() const;
    //将sting转换为提醒数据
    static RemindData RemindStringToRemindData(const QString &remindStr);
    //将提醒数据转换为string
    static QString RemindDataToRemindString(const RemindData &remindData, const bool isAllDay);
private:
    /**
     * @brief m_RemindNum   提前多久提醒
     * -1 永不提醒
     * 若日程为全天 0～N 表示提前多少天提醒
     * 若日程为非全天 0～N 表示提前多少分钟提醒
     */
    int     m_RemindNum;
    /**
     * @brief m_RemindTime  提醒时间
     * 只有日程为全天 才有效
     */
    QTime   m_RemindTime;
};

#endif // REMINDDATA_H
