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
#ifndef COMMONDATASTRUCT_H
#define COMMONDATASTRUCT_H
#include <QString>
#include <QVector>

enum jobtypes {
    jobTypeWork = 1,
    jobTypeLife,
    jobTypeOther,
    JobTypeFestival,
};

typedef struct JobTypeJSON {
    uint ID;
    QString Name;
    QString Color;
} stJobTypeJSON;

//关于名字的国际化暂时不做处理，如果后期确认需要国际化的很少则通过client做适配来处理
static QVector<stJobTypeJSON> globalPredefinedTypes {
    {
        jobTypeWork,
        "Work",
        "#FF0000" // red
    },
    {
        jobTypeLife,
        "Life",
        "#00FF00" // green
    },
    {
        jobTypeOther,
        "Other",
        "#800080" // purple
    },
    {
        JobTypeFestival,
        "Festival",
        "#FFFF00" // yellow
    }};
#endif // COMMONDATASTRUCT_H
