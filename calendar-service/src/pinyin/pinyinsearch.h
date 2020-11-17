/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     zhengxiaokang <zhengxiaokang@uniontech.com>
*
* Maintainer: zhengxiaokang <zhengxiaokang@uniontech.com>
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
#ifndef PINYINSEARCH_H
#define PINYINSEARCH_H

#include <QVector>
#include <QMap>

class pinyinsearch
{
public:
    /* 初始化合法拼音表 */
    pinyinsearch();
    /* 判断字符串是否只可以进行拼音查询 */
    bool canQueryByPinyin(QString str);
    /* 创建拼音字符串 */
    QString createPinyin(QString zh);
    /* 构造拼音查询表达式 */
    QString createPinyinQuery(QString pinyin);
    /* 构造拼音查询正则表达式 */
    QString createPinyinRegexp(QString pinyin);
    /* 判断汉字和拼音是否匹配 */
    bool pinyinMatch(QString zh, QString py);

private:
    //获取汉字对应的拼音，不带音调
    QList<QStringList> Pinyin(QString str);
    //找到汉字对应的拼音
    QStringList SinglePinyin(QString index);
    //去掉拼音中的音调
    QStringList removeYin(QStringList pinyin);
    /* 单个合法拼音的最大长度 */
    int singlePinyinMaxLength = 0;
    /* 合法单拼音表 */
    QMap<QString, bool> validPinyinMap {};
};

#endif
