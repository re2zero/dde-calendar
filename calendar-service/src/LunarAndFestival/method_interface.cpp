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
#include "method_interface.h"
#include "QtCore/qmath.h"

LunarAndFestivalAlgorithm::LunarAndFestivalAlgorithm()
{
}
/**
 * @brief LunarAndFestivalAlgorithm::GetMonthName 获取当天的农历月名称
 * @param dayinfo 农历日信息
 * @return 阴历月份
 */
QString LunarAndFestivalAlgorithm::GetLunarMonthName(lunarDayInfo &dayinfo)
{
    QString monthName = lunarMonthNames[dayinfo.LunarMonth.Name-1];
    if (dayinfo.LunarMonth.IsLeap) {
        return "闰" + monthName + "月";
    }
    return monthName + "月";
}
/**
 * @brief LunarAndFestivalAlgorithm::GetLunarDayName 获取当天的农历日名
 * @param dayinfo 农历日信息
 * @return 阴历日
 */
QString LunarAndFestivalAlgorithm::GetLunarDayName(lunarDayInfo &dayinfo)
{
    return lunarDayNames[dayinfo.LunarDay - 1];
}
/**
 * @brief LunarAndFestivalAlgorithm::GetDayFestival 获取当天的农历节日名,没有则返回空字符串
 * @param dayinfo 农历日信息
 * @return 　农历节日名称
 */
QString LunarAndFestivalAlgorithm::GetDayFestival(lunarDayInfo &dayinfo)
{
    int key = dayinfo.LunarMonth.Name * 100 + dayinfo.LunarDay;
    if (lunarFestival.contains(key)) {
        return lunarFestival[key];
    }

    // 农历腊月（十二月）的最后个一天
    if (dayinfo.LunarMonth.Name == 12 && dayinfo.LunarDay == dayinfo.LunarMonth.Days) {
        return "除夕";
    }
    if (dayinfo.SolarTerm == QingMing) {
        return "清明节";
    }
    return "";
}
/**
 * @brief LunarAndFestivalAlgorithm::GetSolarTermsName 获取二十四节气名,如果没有则返回空
 * @param order　节气枚举值
 * @return 节气名称
 */
QString LunarAndFestivalAlgorithm::GetSolarTermsName(int order)
{
    if (0 <= order && order <= 23) {
        return SolarTermNames[order];
    }
    return "";
}
/**
 * @brief LunarAndFestivalAlgorithm::GetTianGanDiZhi 获取天干地支，将数字转换为天干地支
 * @param num　需要转换的数字
 * @return 天干地支
 */
QString LunarAndFestivalAlgorithm::GetTianGanDiZhi(int num)
{
    return TianGan[num % 10] + DiZhi[num % 12];
}
/**
 * @brief LunarAndFestivalAlgorithm::GetGanZhiMonth 获取当天的月干支
 * @param dayinfo　农历日信息
 * @return 天干地支
 */
QString LunarAndFestivalAlgorithm::GetGanZhiMonth(lunarDayInfo &dayinfo)
{
    return GetTianGanDiZhi((dayinfo.Year - 1900) * 12 + dayinfo.MonthZhi + 12);
}
/**
 * @brief LunarAndFestivalAlgorithm::GetGanZhiYear 获取当天的年干支
 * @param dayinfo 农历日信息
 * @return 天干地支
 */
QString LunarAndFestivalAlgorithm::GetGanZhiYear(lunarDayInfo &dayinfo)
{
    return GetTianGanDiZhi(dayinfo.LunarYear - 1864);
}
/**
 * @brief LunarAndFestivalAlgorithm::GetGanZhiDay 获取当天的日干支
 * @param dayinfo 农历日信息
 * @return 天干地支
 */
QString LunarAndFestivalAlgorithm::GetGanZhiDay(lunarDayInfo &dayinfo)
{
    QDateTime unixDateTime = QDateTime(QDate(dayinfo.Year, dayinfo.Month, dayinfo.Day)
                                       ,QTime(0, 0, 0, 0), Qt::TimeSpec::UTC);
    qint64 unixTime = unixDateTime.currentSecsSinceEpoch();
    int dayCyclical = int(unixTime / 86400) + 29219 + 18;
    return GetTianGanDiZhi(dayCyclical);
}
/**
 * @brief LunarAndFestivalAlgorithm::GetYearZodiac 获取当天的生肖，即年份的生肖
 * @param dayinfo 农历日信息
 * @return 年份生肖
 */
QString LunarAndFestivalAlgorithm::GetYearZodiac(lunarDayInfo &dayinfo)
{
    return Animals[(dayinfo.LunarYear - 4) % 12];
}

double LunarAndFestivalAlgorithm::GetEarthEclipticLongitudeForSun(double jd)
{
    // 计算地球的日心黄经
    double l = GetSunEclipticLongitudeForEarth(jd);
    // 计算地球的日心黄纬
    double b = GetSunEclipticLatitudeForEarth(jd);
    // 修正章动
    l += CalcEarthLongitudeNutation(GetJulianCentury(jd));
    // 转换到fk5
    l += Vsop2Fk5LongitudeCorrection(l, b, jd);
    // 转换成太阳的地心黄经
    l = Mod2Pi(l + M_PI);
    // 计算光行差
    // 计算日地距离
    double r = GetSunRadiusForEarth(jd);
    // 太阳到地球的光行差参数
    l -= lightAberration() / r;
    return l;
}

double LunarAndFestivalAlgorithm::GetSunEclipticLongitudeForEarth(double jd)
{
    double t = GetJulianThousandYears(jd);
    double L0 = GetEarthL0(t);
    double L1 = GetEarthL1(t);
    double L2 = GetEarthL2(t);
    double L3 = GetEarthL3(t);
    double L4 = GetEarthL4(t);
    double L5 = GetEarthL5(t);
    double L = ((((L5*t+L4)*t+L3)*t+L2)*t+L1)*t + L0;
    return Mod2Pi(L);
}

double LunarAndFestivalAlgorithm::GetJulianThousandYears(double jd)
{
    //1000年的日数
    const double  DaysOf1000Years = 365250.0;
    return (jd - J2000) / DaysOf1000Years;
}

double LunarAndFestivalAlgorithm::GetSunEclipticLatitudeForEarth(double jd)
{
    double t = GetJulianThousandYears(jd);
    double B0 = GetEarthB0(t);
    double B1 = GetEarthB1(t);
    double B2 = GetEarthB2(t);
    double B3 = GetEarthB3(t);
    double B4 = GetEarthB4(t);
    double B = ((((B4*t)+B3)*t+B2)*t+B1)*t + B0;
    return B;
}

double LunarAndFestivalAlgorithm::Vsop2Fk5LongitudeCorrection(double l, double b, double jd)
{
    double t = GetJulianCentury(jd);
    double lp = l - ToRadians(1.397) * t - ToRadians(0.00031) * t * t;
    return SecondsToRadians(-0.09033 + 0.03916 * (qCos(lp) + qSin(lp)) * qTan(b));
}

double LunarAndFestivalAlgorithm::GetSunRadiusForEarth(double jd)
{
    double t = GetJulianThousandYears(jd);
    double R0 = GetEarthR0(t);
    double R1 = GetEarthR1(t);
    double R2 = GetEarthR2(t);
    double R3 = GetEarthR3(t);
    double R4 = GetEarthR4(t);
    double R5 = GetEarthR5(t);
    double R = ((((R5 * t + R4) * t + R3) * t+ R2) * t + R1) * t + R0;
    return R;
}

double LunarAndFestivalAlgorithm::NewtonIteration(double angle, double x0)
{
    const double Epsilon = 1e-7;
    const double Delta = 5e-6;
    double x;

    while (1) {
        x = x0;
        double fx = ModPi(GetEarthEclipticLongitudeForSun(x) - angle);
        // 导数
        double fpx = (ModPi(GetEarthEclipticLongitudeForSun(x + Delta) - angle)
                      - GetEarthEclipticLongitudeForSun(x - Delta) - angle) / Delta / 2;
        x0 = x - fx/fpx;
        if (qAbs(x0 - x) <= Epsilon) {
            break;
        }
    }
    return x;
}

double LunarAndFestivalAlgorithm::ModPi(double r)
{
    while (r < -M_PI) {
        r += M_PI * 2;
    }
    while (r > M_PI) {
        r -= M_PI * 2;
    }
    return r;
}
/**
 * @brief LunarAndFestivalAlgorithm::GetMoonEclipticParameter 弧度计算
 * @param T 儒略世纪数
 * @return 弧度
 */
MoonEclipticParameter LunarAndFestivalAlgorithm::GetMoonEclipticParameter(MoonEclipticParameter &moonEclipticParameter, double T)
{
    MoonEclipticParameter m_radian = moonEclipticParameter;
    double T2 = T * T;
    double T3 = T2 * T;
    double T4 = T3 * T;

    /*月球平黄经*/
    m_radian.Lp = Mod2Pi(ToRadians(218.3164591 + 481267.88134236*T - 0.0013268*T2 + T3/538841.0 - T4/65194000.0));
    /*月日距角*/
    m_radian.D = Mod2Pi(ToRadians(297.8502042 + 445267.1115168*T - 0.0016300*T2 + T3/545868.0 - T4/113065000.0));
    /*太阳平近点角*/
    m_radian.M = Mod2Pi(ToRadians(357.5291092 + 35999.0502909*T - 0.0001536*T2 + T3/24490000.0));
    /*月亮平近点角*/
    m_radian.Mp = Mod2Pi(ToRadians(134.9634114 + 477198.8676313*T + 0.0089970*T2 + T3/69699.0 - T4/14712000.0));
    /*月球经度参数(到升交点的平角距离)*/
    m_radian.F = Mod2Pi(ToRadians(93.2720993 + 483202.0175273*T - 0.0034029*T2 - T3/3526000.0 + T4/863310000.0));
    /* 反映地球轨道偏心率变化的辅助参量 */
    m_radian.E = 1 - 0.002516*T - 0.0000074*T2;

    return m_radian;
}
/**
 * @brief LunarAndFestivalAlgorithm::ToRadians 角度转换为弧度
 * @param degrees 角度
 * @return 弧度
 */
double LunarAndFestivalAlgorithm::ToRadians(double degrees)
{
    return degrees * M_PI / 180;
}
/**
 * @brief LunarAndFestivalAlgorithm::Mod2Pi 把角度限制在[0, 2π]之间
 * @param r 角度
 * @return 角度
 */
double LunarAndFestivalAlgorithm::Mod2Pi(double r)
{
    while (r < 0) {
        r += M_PI * 2;
    }
    while (r > 2 * M_PI) {
        r -= M_PI * 2;
    }
    return r;
}
/**
 * @brief LunarAndFestivalAlgorithm::CalcMoonECLongitudePeriodic 计算月球地心黄经周期项的和
 * @param raDian 弧度
 * @return 月球地心黄经周期项的和
 */
double LunarAndFestivalAlgorithm::CalcMoonECLongitudePeriodic(MoonEclipticParameter &moonEclipticParameter)
{
    double EI = 0.0;
    for (int i = 0; i < MoonLongitude.count(); i++) {
        double theta = MoonLongitude[i].D * moonEclipticParameter.D + MoonLongitude[i].M * moonEclipticParameter.M
                       +MoonLongitude[i].Mp * moonEclipticParameter.Mp + MoonLongitude[i].F * moonEclipticParameter.F;
        EI += MoonLongitude[i].EiA * qSin(theta) * qPow(moonEclipticParameter.E, qAbs(MoonLongitude[i].M));
    }
    return EI;
}
/**
 * @brief LunarAndFestivalAlgorithm::CalcMoonLongitudePerturbation 计算金星摄动,木星摄动以及地球扁率摄动对月球地心黄经的影响
 * @param T 儒略世纪数
 * @param raDian 弧度
 * @return
 */
double LunarAndFestivalAlgorithm::CalcMoonLongitudePerturbation(double T, MoonEclipticParameter &moonEclipticParameter)
{
    double A1 = Mod2Pi(ToRadians(119.75 + 131.849*T));
    double A2 = Mod2Pi(ToRadians(53.09 + 479264.290*T));

    return 3958.0 * qSin(A1) + 1962.0 * qSin(moonEclipticParameter.Lp - moonEclipticParameter.F) + 318.0 * qSin(A2);
}
/**
 * @brief LunarAndFestivalAlgorithm::GetMoonEclipticLongitudeEC 计算月球地心黄经
 * @param julianDay 儒略日
 * @return 弧度
 */
double LunarAndFestivalAlgorithm::GetMoonEclipticLongitudeEC(double julianDay)
{
    MoonEclipticParameter m_radian;
    double T = GetJulianCentury(julianDay);
    m_radian = GetMoonEclipticParameter(m_radian, T);
    // Lp 计算是正确的
    /*计算月球地心黄经周期项*/
    double EI = CalcMoonECLongitudePeriodic(m_radian);
    /*修正金星,木星以及地球扁率摄动*/
    EI += CalcMoonLongitudePerturbation(T, m_radian);
    double longitude = m_radian.Lp + ToRadians(EI / 1000000.0);
    /*计算天体章动干扰*/
    longitude = CalcEarthLongitudeNutation(T);
    return longitude;
}
/**
 * @brief LunarAndFestivalAlgorithm::GetJulianCentury 计算儒略世纪数
 * @param julianDay 儒略日
 * @return 儒略世纪数
 */
double LunarAndFestivalAlgorithm::GetJulianCentury(double julianDay)
{
    // 100年的日数
    const double DaysOfCentury = 36525.0;
    return (julianDay - J2000) / DaysOfCentury;
}
/**
 * @brief LunarAndFestivalAlgorithm::GetEarthNutationParameter 返回弧度
 * @param earthNutationParameter 弧度
 * @param T 儒略世纪数
 * @return 弧度
 */
EarthNutationParameter LunarAndFestivalAlgorithm::GetEarthNutationParameter(EarthNutationParameter &earthNutationParameter, double T)
{
    EarthNutationParameter m_radian = earthNutationParameter;
    double T2 = T * T;
    double T3 = T2 * T;

    /*平距角（如月对地心的角距离）*/
    m_radian.D = ToRadians(297.85036 + 445267.111480*T - 0.0019142*T2 + T3/189474.0);
    /*太阳（地球）平近点角*/
    m_radian.M = ToRadians(357.52772 + 35999.050340*T - 0.0001603*T2 - T3/300000.0);
    /*月亮平近点角*/
    m_radian.Mp = ToRadians(134.96298 + 477198.867398*T + 0.0086972*T2 + T3/56250.0);
    /*月亮纬度参数*/
    m_radian.F = ToRadians(93.27191 + 483202.017538*T - 0.0036825*T2 + T3/327270.0);
    /*黄道与月亮平轨道升交点黄经*/
    m_radian.Omega = ToRadians(125.04452 - 1934.136261*T + 0.0020708*T2 + T3/450000.0);
    return m_radian;
}

double LunarAndFestivalAlgorithm::CalcEarthLongitudeNutation(double T)
{
    EarthNutationParameter m_radian;
    m_radian = GetEarthNutationParameter(m_radian,T);
    double result = 0.0;
    for (int i = 0; i < nuation.count(); i++) {
        double theta = nuation[i].D * m_radian.D + nuation[i].M * m_radian.M + nuation[i].Mp * m_radian.Mp
                       + nuation[i].F * m_radian.F + nuation[i].Omega * m_radian.Omega;
        result += (nuation[i].Sine1 + nuation[i].Sine2 * T) * qSin(theta);
    }
    //乘以章动表的系数 0.0001 角秒
    return result * coefficient();
}
/**
 * @brief LunarAndFestivalAlgorithm::SecondsToDegrees 把角秒换算成角度
 * @param seconds 角秒
 * @return 角度
 */
double LunarAndFestivalAlgorithm::SecondsToDegrees(double seconds)
{
    return seconds / 3600;
}

double LunarAndFestivalAlgorithm::SecondsToRadians(double seconds)
{
    return ToRadians(SecondsToDegrees(seconds));
}

double LunarAndFestivalAlgorithm::CalcEarthObliquityNutation(double dt)
{
    EarthNutationParameter m_radian;
    m_radian = GetEarthNutationParameter(m_radian, dt);
    double result = 0.0;
    for (int i = 0; i < nuation.count(); i++) {
        double theta = nuation[i].D * m_radian.D + nuation[i].M * m_radian.M + nuation[i].Mp * m_radian.Mp
                       + nuation[i].F * m_radian.F + nuation[i].Omega * m_radian.Omega;
        result += (nuation[i].Cosine1 + nuation[i].Cosine2*dt) * qCos(theta);
    }
    //乘以章动表的系数 0.0001 角秒
    return result * coefficient();
}

double LunarAndFestivalAlgorithm::ToJulianDateHMS(int year, int month, int day, int hour, int minute, double second)
{
    int jdn = ToJulianDate(year, month, day);
    return double(jdn) + (double(hour) - 12) / 24.0 + double(minute) / 1440.0 + second / 86400.0;
}

int LunarAndFestivalAlgorithm::ToJulianDate(int year, int month, int day)
{
    int a = (14 - month) / 12;
    int y = year + 4800 - a;
    int m = month + 12*a - 3;
    return day + (153 * m + 2) / 5 + 365 * y + y /4  - y / 100 + y / 400 - 32045;
}


