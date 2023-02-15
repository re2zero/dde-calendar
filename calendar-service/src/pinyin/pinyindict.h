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
#ifndef PINYINDICT_H
#define PINYINDICT_H

#include <QMap>
#include <QVector>

//带有音调的拼音字典
extern QMap<int, QString> PinyinDict_1;
extern QMap<int, QString> PinyinDict_2;
extern QMap<int, QString> PinyinDict_3;
extern QMap<int, QString> PinyinDict_4;
extern QMap<int, QString> PinyinDict_5;
extern QMap<int, QString> PinyinDict_6;
extern QMap<int, QString> PinyinDict_7;
extern QMap<int, QString> PinyinDict_8;
extern QMap<int, QString> PinyinDict_9;
extern QMap<int, QString> PinyinDict_10;
extern QMap<int, QString> PinyinDict_11;
extern QMap<int, QString> PinyinDict_12;
extern QMap<int, QString> PinyinDict_13;
extern QMap<int, QString> PinyinDict_14;
extern QMap<int, QString> PinyinDict_15;
extern QMap<int, QString> PinyinDict_16;
extern QMap<int, QString> PinyinDict_17;
extern QMap<int, QString> PinyinDict_18;
extern QMap<int, QString> PinyinDict_19;
extern QMap<int, QString> PinyinDict_20;
extern QMap<int, QString> PinyinDict_21;
//获取拼音字典
const QVector<QMap<int,QString> > getPinYinDictVector();
// 带音标字符。
extern QMap<QString, QString> phoneticSymbol;
/* 合法拼音列表 */
extern QVector<QString> validPinyinList;

#endif // PINYINDICT_H
