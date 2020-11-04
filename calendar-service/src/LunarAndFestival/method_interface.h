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
#ifndef METHOD_INTERFACE_H
#define METHOD_INTERFACE_H

#include <QDateTime>
#include <QVector>
#include <QMap>
#include "celestialbodies.h"
#include "lunarandfestival.h"

typedef struct _LunarDayInfo {
    QString GanZhiYear;      // 农历年的干支
    QString GanZhiMonth;     // 农历月的干支
    QString GanZhiDay;       // 农历日的干支
    QString LunarMonthName;  // 农历月名
    QString LunarDayName;    // 农历日名
    int32_t LunarLeapMonth;  // 未使用
    QString Zodiac;          // 农历年的生肖
    QString Term;            // 农历节气
    QString SolarFestival;   // 公历节日
    QString LunarFestival;   // 农历节日
    int32_t Worktime;        // 未使用
} LunarDayInfo;


class LunarAndFestivalAlgorithm
{
public:
    LunarAndFestivalAlgorithm();

    LunarDayInfo SolarToLunar(LunarDayInfo &lunardayinfo, int year, int month, int day);

    //获取当天的农历月名称
    QString GetLunarMonthName(lunarDayInfo &dayinfo);
    //获取当天的农历日名
    QString GetLunarDayName(lunarDayInfo &dayinfo);
    //获取当天的农历节日名,没有则返回空字符串
    QString GetDayFestival(lunarDayInfo &dayinfo);
    //获取二十四节气名,如果没有则返回空
    QString GetSolarTermsName(int order);
    //获取天干地支，将数字转换为天干地支
    QString GetTianGanDiZhi(int num);
    //获取当天的月干支
    QString GetGanZhiMonth(lunarDayInfo &dayinfo);
    //获取当天的年干支
    QString GetGanZhiYear(lunarDayInfo &dayinfo);
    //获取当天的日干支
    QString GetGanZhiDay(lunarDayInfo &dayinfo);
    //获取当天的生肖，即年份的生肖
    QString GetYearZodiac(lunarDayInfo &dayinfo);

    //弧度计算
    MoonEclipticParameter GetMoonEclipticParameter(MoonEclipticParameter &moonEclipticParameter, double T);
    //角度转换为弧度
    double ToRadians(double degrees);
    //把角度限制在[0, 2π]之间
    double Mod2Pi(double r);
    //计算月球地心黄经周期项的和
    double CalcMoonECLongitudePeriodic(MoonEclipticParameter &moonEclipticParameter);
    /*计算金星摄动,木星摄动以及地球扁率摄动对月球地心黄经的影响, T 是儒略世纪数，Lp和F单位是弧度*/
    // A1 = 119.75 + 131.849 * T                                             （4.13式）
    // A2 = 53.09 + 479264.290 * T                                           （4.14式）
    // A3 = 313.45 + 481266.484 * T                                          （4.15式）
    double CalcMoonLongitudePerturbation(double T, MoonEclipticParameter &moonEclipticParameter);
    //计算月球地心黄经
    double GetMoonEclipticLongitudeEC(double julianDay);
    //计算儒略世纪数
    double GetJulianCentury(double julianDay);
    //返回弧度
    EarthNutationParameter GetEarthNutationParameter(EarthNutationParameter &earthNutationParameter, double T);
    //计算某时刻的黄经章动干扰量
    double CalcEarthLongitudeNutation(double T);
    //把角秒换算成角度
    double SecondsToDegrees(double seconds);
    //把角秒换算成弧度
    double coefficient()
    {
        return SecondsToRadians(0.0001);
    }
    double SecondsToRadians(double seconds);
    /*计算某时刻的黄赤交角章动干扰量，dt是儒略千年数，返回值单位是度*/
    // 计算某时刻的黄赤交角章动干扰量
    // dt 是儒略世纪数
    // 返回弧度
    double CalcEarthObliquityNutation(double dt);
    // ToJulianDateHMS 计算Gregorian时间的儒略日数
    // 算法摘自 http://en.wikipedia.org/wiki/Julian_day
    double ToJulianDateHMS(int year, int month, int day, int hour, int minute, double second);
    // ToJulianDate 计算Gregorian日期的儒略日数，以TT当天中午12点为准(结果是整数)。
    // 算法摘自 http://en.wikipedia.org/wiki/Julian_day
    int ToJulianDate(int year, int month, int day);
    double GetEarthEclipticLongitudeForSun(double jd);
    /**
     * 按儒略日计算地球的日心黄经
     *
     * 参数： jd 儒略日
     * 返回 地球的日心黄经，单位是弧度(rad)
     */
    double GetSunEclipticLongitudeForEarth(double jd);
    //计算儒略千年数
    double GetJulianThousandYears(double jd);
    /**
     * 按儒略日计算地球的日心黄纬
     *
     * 参数 jd  儒略日
     * 返回 地球的日心黄纬，单位是弧度(rad)
     */
    double GetSunEclipticLatitudeForEarth(double jd);
    /**
     * 用于把vsop87理论算出来的经度转换成fk5目视系统的经度的修正值，参考 Jean Meeus 的 Astronomical
     * Algorithms 第二版(1998)第32章219页(32.3)式
     *
     * 参数 l
     *            vsop87经度(rad)
     * 参数 b
     *            vsop87纬度(rad)
     * 参数 jd
     *            儒略日
     * 返回 修正量(rad)
     */
    double Vsop2Fk5LongitudeCorrection(double l, double b, double jd);
    /**
     * 按照儒略日计算地球和太阳的距离
     *
     * 参数 jd  儒略日
     * 返回 地球和太阳的距离，单位是天文单位(au)
     */
    double GetSunRadiusForEarth(double jd);
    /**
     * 计算修正后的太阳的地心视黄经
     *
     * 参数 jd
     *            儒略日
     * 返回 修正后的地心黄经(rad)
     */
    // 常量
    double lightAberration()
    {
        return SecondsToRadians(20.4898);
    }
    // NewtonIteration 牛顿迭代法求解方程的根
    double NewtonIteration(double angle, double x0);
    // ModPi 把角度限制在[-π, π]之间
    double ModPi(double r);
};

#endif // LUNARANDFESTIVALALGORITHM_H
