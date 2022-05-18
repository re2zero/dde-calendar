// SPDX-FileCopyrightText: 2019 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DUNIONIDDAV_H
#define DUNIONIDDAV_H

/**
 * @brief The DUnionIDDav class
 * Union ID 云同步
 */
class DUnionIDDav
{
public:
    DUnionIDDav();
    //创建帐户信息文件
    void createAccountFile();
    //创建类型文件
    void createTypeFile();
    //创建日程文件
    void createCalendarFile();
    //删除文件夹
    void deleteFolder();
    //创建文件夹
    void createFolder();
    //判断文件夹是否存在
    bool exists();
};

#endif // DUNIONIDDAV_H
