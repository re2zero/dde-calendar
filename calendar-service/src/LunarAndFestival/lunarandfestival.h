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
#ifndef LUNARANDFESTIVAL_H
#define LUNARANDFESTIVAL_H

#include <QVector>
#include <QMap>
#include <QDateTime>

// MonthInfo 保存农历月信息
typedef struct _lunarMonthInfo {
    int         LunarYear;        // 农历年
    int         Name ;            // 农历月名
    int         Days;             // 本月天数
    double      ShuoJD;           // 本月朔日时间 北京时间 儒略日
    QDateTime   ShuoTime;         // 本月朔日时间 北京时间
    bool        IsLeap;           // 是否为闰月
} lunarMonthInfo;

// DayInfo 保存农历日信息
typedef struct _lunarDayInfo {
    int         Year;              // 公历年
    int         Month;             // 公历月
    int         Day;               // 公历日
    int         LunarYear;         // 农历年
    int         LunarDay ;         // 农历日
    lunarMonthInfo   LunarMonth;        // 农历月
    int         MonthZhi;          // 农历日所在的月的地支
    int         SolarTerm ;        // 0~23 二十四节气 ，-1 非节气
} lunarDayInfo;

typedef struct _day {
    int Year;
    int Month;
    int Day;
} day;

// Calendar 保存公历年内计算农历所需的信息
typedef struct _Calendar {
    int Year;                                  // 公历年份
    QVector<double> SolarTermJDs;              // 相关的 25 节气 北京时间 儒略日
    QVector<QDateTime> SolarTermTimes;         // 对应 SolarTermJDs 转换为 time.Time 的时间
    QVector<double> NewMoonJDs;                // 相关的 15 个朔日 北京时间 儒略日
    QVector<lunarMonthInfo *> Months;          // 月
    QVector<int> solarTermYearDays;            // 十二节的 yearDay 列表
} Calendar;

static QMap<int, Calendar *> ccCache{};

// 十二月名
static QVector<QString> lunarMonthNames = {"正", "二", "三", "四", "五", "六", "七", "八", "九", "十", "冬", "腊"};

static QVector<int> monthDays = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// 农历日名
// 月份分为大月和小月，大月三十天，小月二十九天
static QVector<QString> lunarDayNames = {
    "初一", "初二", "初三", "初四", "初五", "初六", "初七", "初八", "初九", "初十",
    "十一", "十二", "十三", "十四", "十五", "十六", "十七", "十八", "十九", "二十",
    "廿一", "廿二", "廿三", "廿四", "廿五", "廿六", "廿七", "廿八", "廿九", "三十"
};

//农历节日
static QMap<int,QString> lunarFestival {
    {101,  "春节"}
    ,{115,  "元宵节"}
    ,{505,  "端午节"}
    ,{707,  "七夕节"}
    ,{815,  "中秋节"}
    ,{909,  "重阳节"}
    ,{1208, "腊八节"}
};

// 十二生肖
static QVector<QString> Animals = {"鼠", "牛", "虎", "兔", "龙", "蛇", "马", "羊", "猴", "鸡", "狗", "猪"};
// 天干
static QVector<QString> TianGan = {"甲", "乙", "丙", "丁", "戊", "己", "庚", "辛", "壬", "癸"};
// 地支
static QVector<QString> DiZhi = {"子", "丑", "寅", "卯", "辰", "巳", "午", "未", "申", "酉", "戌", "亥"};

//二十四节气，中文
static QVector<QString> SolarTermNames = {
    "春分",
    "清明",
    "谷雨",
    "立夏",
    "小满",
    "芒种",
    "夏至",
    "小暑",
    "大暑",
    "立秋",
    "处暑",
    "白露",
    "秋分",
    "寒露",
    "霜降",
    "立冬",
    "小雪",
    "大雪",
    "冬至",
    "小寒",
    "大寒",
    "立春",
    "雨水",
    "惊蛰",
};

//二十四节气
enum solarTerms {
    ChunFen = 0,
    QingMing,
    GuYu,
    LiXia,
    XiaoMan,
    MangZhong,
    XiaZhi,
    XiaoShu,
    DaShu,
    LiQiu,
    ChuShu,
    BaiLu,
    QiuFen,
    HanLu,
    ShuangJiang,
    LiDong,
    XiaoXue,
    DaXue,
    DongZhi,
    XiaoHan,
    DaHan,
    LiChun,
    YuShui,
    JingZhe,
};

static  QMap<int, QString> solarFestival = {
    {101,  "元旦"}
    ,{214,  "情人节"}
    ,{305,  "学雷锋纪念日"}
    ,{308,  "妇女节"}
    ,{312,  "植树节"}
    ,{401,  "愚人节"}
    ,{415,  "全民国家安全教育日"}
    ,{501,  "劳动节"}
    ,{504,  "青年节"}
    ,{601,  "儿童节"}
    ,{701,  "建党节,香港回归纪念日"}
    ,{801,  "建军节"}
    ,{903,  "抗日战争胜利纪念日"}
    ,{910,  "教师节"}
    ,{1001, "国庆节"}
    ,{1213, "南京大屠杀死难者国家公祭日"}
    ,{1220, "澳门回归纪念"}
    ,{1224, "平安夜"}
    ,{1225, "圣诞节"}
    ,{1226, "毛泽东诞辰纪念"}
};


#endif // ABOUTLUNARANDFESTIVAL_H
