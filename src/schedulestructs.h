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
#ifndef SCHEDULESTURCTS_H
#define SCHEDULESTURCTS_H

#include <QList>
#include <QDateTime>
#include <QColor>

typedef struct _tagScheduleInfo {
    int               id;
    QDateTime         beginDateTime;
    QDateTime         endDateTime;
    QString           titleName;
    QString           description;
    int               remind;  //0 当天 1 一天前 2 三天前 3 一周前
    int               repeat;  //0 一次 1每天    2 每周  3每月  4每年
    char              allday;  //1全天
    int               infotype;    //0工作 1 生活 2其他
} ScheduleInfo;
typedef struct _tagScheduleRemindInfo {
    int               id;
    QDateTime         remindDateTime;
    int               times;
    _tagScheduleRemindInfo()
    {
        times = 1;
    }
} ScheduleRemindInfo;

#endif
