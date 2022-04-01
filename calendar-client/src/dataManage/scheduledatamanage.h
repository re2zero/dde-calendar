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
#ifndef SCHEDULEDATAMANAGE_H
#define SCHEDULEDATAMANAGE_H
#include "src/scheduledatainfo.h"

#include <DGuiApplicationHelper>

#include <QThread>
#include <QDate>
#include <QMutex>

DGUI_USE_NAMESPACE
struct CSchedulesColor {
    int type;
    QColor gradientFromC;
    QColor gradientToC;
    QColor dotColor;
    QColor Purecolor;
    QColor shadowcolor;
    QColor textColor;
    QColor timeColor;
    QColor hightlightgradientFromC;
    QColor hightlightgradientToC;
    QColor hightlightPurecolor;
    QColor hovergradientFromC;
    QColor hovergradientToC;
    QColor hoverPurecolor;
    QColor splitColor;
    QColor pressgradientFromC;
    QColor pressgradientToC;
    QColor pressPurecolor;
};

class CScheduleDataManage
{
public:
    static CScheduleDataManage *getScheduleDataManage();
    CSchedulesColor getScheduleColorByType(int type);
    static QColor getSystemActiveColor();
    void setTheMe(int type = 0);
    int getTheme() const
    {
        return m_theme;
    }
private:
    CScheduleDataManage();
    ~CScheduleDataManage();

private:
    QVector<CSchedulesColor> m_vScheduleColor;
    int m_theme = 0;
    static CScheduleDataManage *m_vscheduleDataManage;
};

class JobTypeInfoManager{
private:
    JobTypeInfoManager();
public:
    static JobTypeInfoManager *instance();
    /**
     * @brief updateInfo  更新信息
     */
    void updateInfo();
    //查询列表、返回列表
    QList<JobTypeInfo> getJobTypeList()
    {
        return m_lstJobType;
    }

    QList<JobTypeColorInfo> getJobTypeColorList()
    {
        return  m_lstJobTypeColor;
    }

    /**
     * @brief isSysJobTypeColor   是否是默认颜色
     * @param colorTypeNo         颜色类型编号
     * @return                    是否是默认颜色
     */
    bool isSysJobTypeColor(int colorTypeNo);
    /**
     * @brief getSysJobTypeColor  获取指定编号的默认颜色
     * @param colorTypeNo         颜色类型编号
     * @param jobTypeColorInfo    颜色信息
     * @return                    操作结果
     */
    bool getSysJobTypeColor(int colorTypeNo, JobTypeColorInfo& jobTypeColorInfo);
    //查询日程类型
    bool getJobTypeByNo(int iNo, JobTypeInfo& jobType);

    //查询日程类型名称是否重复
    bool isJobTypeNameUsed(QString strName);

private:
    QList<JobTypeInfo> m_lstJobType;
    QList<JobTypeColorInfo> m_lstJobTypeColor;
};
#endif // SCHEDULEVIEW_H
