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
#ifndef JSONDATA_H
#define JSONDATA_H

#include <QObject>
#include <QVector>
#include "timedatastruct.h"
class QJsonObject;
class JsonData
{
public:
    enum RepeatStatus { NONE,
                        EVED,
                        EVEW,
                        EVEM,
                        EVEY,
                        WORKD,
                        RESTD
    };
    enum DateTimeStatus { NODATE,
                          NOTIME,
                          ALLHAS
    };
    enum PropertyStatus { PRO_NONE,
                          NEXT,
                          LAST,
                          ALL,
                          PRO_THIS
    };

public:
    JsonData();
    virtual ~JsonData();
    void JosnResolve(const QJsonObject &jsObj);
    QString SuggestMsg() const;
    void setSuggestMsg(const QString &SuggestMsg);

    bool ShouldEndSession() const;
    void setShouldEndSession(bool ShouldEndSession);

    void setRepeatStatus(const RepeatStatus &RepeatStatus);

    QString TitleName() const;
    void setTitleName(const QString &TitleName);

    void setDateTimeStatus(const DateTimeStatus &DateTimeStatus);

    RepeatStatus getRepeatStatus() const;

    QVector<int> getRepeatNum() const;
    void setRepeatNum(const QVector<int> &RepeatNum);

    SemanticsDateTime getDateTime() const;
    void setDateTime(const SemanticsDateTime &DateTime);

    PropertyStatus getPropertyStatus() const;
    void setPropertyStatus(const PropertyStatus &propertyStatus);

    int offset() const;
    void setOffset(int offset);

    /**
     * @brief isVaild   判断json是否为原始数据
     * @return  true为原始数据
     */
    virtual bool isVaild() const;

protected:
    void contentJsonResolve(const QJsonObject &jsobj);
    void repeatJsonResolve(const QJsonObject &jsobj);
    /**
     * @brief datetimeJsonResolve       时间关键字解析
     * @param jsobj     json对象
     */
    void datetimeJsonResolve(const QJsonObject &jsobj);
    void propertyJsonResolve(const QJsonObject &jsobj);
    /**
     * @brief posRankOffsetResolve      偏移关键字解析
     * @param jsobj     json对象
     */
    void posRankOffsetResolve(const QJsonObject &jsobj);
    SemanticsDateTime suggestDatetimeResolve(const QJsonObject &jsobj);
    /**
     * @brief resolveNormValue      解析建议时间数据
     * @param str       string格式的建议时间
     * @return      建议时间
     */
    SuggestDatetimeInfo resolveNormValue(const QString &str);
    /**
     * @brief resolveDateTimeValeu      解析模糊时间数据
     * @param dateTimeStr      string格式的模糊时间
     * @return          模糊时间
     */
    DateTimeInfo resolveDateTimeValeu(const QString &dateTimeStr);
    //修改助手返回日期和时间字符格式
    QString strTransform(QString oldStr);
    //修改助手返回日期的字符格式
    QString strDateTransform(QString oldStrDate);
    virtual void setDefaultValue();
    virtual void jsonObjResolve(const QJsonObject &jsobj);

private:
    QString m_SuggestMsg {""};
    bool m_ShouldEndSession {false};
    RepeatStatus m_RepeatStatus {NONE};
    QVector<int> m_RepeatNum {};
    DateTimeStatus m_DateTimeStatus {NODATE};
    PropertyStatus m_propertyStatus {PRO_NONE};
    QString m_TitleName {""};
    SemanticsDateTime m_DateTime {};
    int m_offset {-1};
};

#endif // JSONDATA_H
