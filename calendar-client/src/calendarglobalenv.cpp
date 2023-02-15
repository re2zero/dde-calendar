/*
   * Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
   *
   * Author:     chenhaifeng <chenhaifeng@uniontech.com>
   *
   * Maintainer: chenhaifeng <chenhaifeng@uniontech.com>
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
#include "calendarglobalenv.h"

CalendarGlobalEnv *CalendarGlobalEnv::getGlobalEnv()
{
    static CalendarGlobalEnv globalEnv;
    return &globalEnv;
}

CalendarGlobalEnv *CalendarGlobalEnv::operator->() const
{
    return getGlobalEnv();
}

//注册关键字
bool CalendarGlobalEnv::registerKey(const QString &key, const QVariant &variant)
{
    //如果不包含则添加
    if (!m_GlobalEnv.contains(key)) {
        m_GlobalEnv[key] = variant;
        return true;
    }
    return false;
}

//修改值
bool CalendarGlobalEnv::reviseValue(const QString &key, const QVariant &variant)
{
    //如果包含，则修改
    if (m_GlobalEnv.contains(key)) {
        m_GlobalEnv[key] = variant;
        return true;
    }
    return false;
}

//移除注册的关键字
bool CalendarGlobalEnv::removeKey(const QString &key)
{
    //如果包含则移除
    if (m_GlobalEnv.contains(key)) {
        m_GlobalEnv.remove(key);
        return true;
    }
    return false;
}

//根据关键字获取对应的值
bool CalendarGlobalEnv::getValueByKey(const QString &key, QVariant &variant)
{
    if (m_GlobalEnv.contains(key)) {
        variant = m_GlobalEnv.value(key);
        return true;
    }
    return false;
}

CalendarGlobalEnv::CalendarGlobalEnv()
    : m_GlobalEnv{}
{

}
