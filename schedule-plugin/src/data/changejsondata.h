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
#ifndef CHANGEJSONDATA_H
#define CHANGEJSONDATA_H

#include "jsondata.h"

class changejsondata : public JsonData
{
public:
    changejsondata();
    /**
     * @brief fromDateTime  获取语义from时间数据
     * @return  时间数据
     */
    SemanticsDateTime fromDateTime() const;
    /**
     * @brief setFromDateTime       设置语义from时间数据
     * @param fromDateTime      时间数据
     */
    void setFromDateTime(const SemanticsDateTime &fromDateTime);
    /**
     * @brief toDateTime      获取语义toDateTime时间数据
     * @return   时间数据
     */
    SemanticsDateTime toDateTime() const;
    /**
     * @brief setToDateTime        设置语义toDateTime时间数据
     * @param toDateTime   时间数据
     */
    void setToDateTime(const SemanticsDateTime &toDateTime);

    QString toPlaceStr() const;
    void setToPlaceStr(const QString &toPlaceStr);
    /**
     * @brief isVaild   判断json是否为原始数据
     * @return  true为原始数据
     */
    bool isVaild() const override;

private:
    void jsonObjResolve(const QJsonObject &jsobj) override;
    void setDefaultValue() override;
    void fromDatetimeJsonResolve(const QJsonObject &jsobj);
    void toDatetimeJsonResolve(const QJsonObject &jsobj);
    void toPlaceJsonResolve(const QJsonObject &jsobj);

private:
    /**
     * @brief m_fromDateTime   需要修改的时间信息
     */
    SemanticsDateTime m_fromDateTime {};
    /**
     * @brief m_toDateTime   修改到的时间信息
     */
    SemanticsDateTime m_toDateTime {};
    /**
     * @brief m_toPlaceStr   修改的日常内容
     */
    QString m_toPlaceStr {};
};

#endif // CHANGEJSONDATA_H
