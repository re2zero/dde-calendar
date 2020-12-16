/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     chenhaifeng  <chenhaifeng@uniontech.com>
*
* Maintainer: chenhaifeng  <chenhaifeng@uniontech.com>
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
#include "cschedulebasewidget.h"
CalendarManager *CScheduleBaseWidget::m_calendarManager = nullptr;
CScheduleBaseWidget::CScheduleBaseWidget(QWidget *parent)
    : QWidget(parent)
{
    if (m_calendarManager == nullptr) {
        m_calendarManager = CalendarManager::getInstance();
        ShowDateRange _showDateRange = m_calendarManager->getCalendarDateDataManage()->getShowDateRange();
        //获取一年的日程信息
        updateDBusData(_showDateRange.startDate, _showDateRange.stopDate, getShowLunar());
    }
    m_calendarManager->addShowWidget(this);
}

CScheduleBaseWidget::~CScheduleBaseWidget()
{
    m_calendarManager->removeShowWidget(this);
    if (m_calendarManager->getShowWidgetSize() == 0)
        CalendarManager::releaseInstance();
}

/**
 * @brief CScheduleBaseWidget::setSelectDate    设置选择时间
 * @param selectDate
 */
bool CScheduleBaseWidget::setSelectDate(const QDate &selectDate, const bool isSwitchYear)
{
    bool _result = false;    //选择时间必须大于等于1900年
    if (selectDate.year() >= 1900) {
        m_calendarManager->getCalendarDateDataManage()->setSelectDate(selectDate, isSwitchYear);
        _result = true;
        //更新其它视图界面显示
        for (int i = 0; i < m_calendarManager->getShowWidgetSize(); ++i) {
            //如果为当前视图则不更新
            if (m_calendarManager->getShowWidget(i) == this || m_calendarManager->getShowWidget(i) == nullptr)
                continue;
            m_calendarManager->getShowWidget(i)->setYearData();
            m_calendarManager->getShowWidget(i)->updateShowDate();
        }
        ShowDateRange _showDateRange = m_calendarManager->getCalendarDateDataManage()->getShowDateRange();
        //如果缓存中不包含开始或结束时间则更新dbus数据
        if (!m_calendarManager->getScheduleTask()->hasScheduleInfo(_showDateRange.startDate, _showDateRange.stopDate)) {
            updateDBusData(_showDateRange.startDate, _showDateRange.stopDate, getShowLunar());
        }
    }
    return _result;
}

/**
 * @brief CScheduleBaseWidget::getSelectDate    获取选择时间
 * @return
 */
QDate CScheduleBaseWidget::getSelectDate() const
{
    return m_calendarManager->getCalendarDateDataManage()->getSelectDate();
}

/**
 * @brief CScheduleBaseWidget::setCurrendDateTime   设置当前时间
 * @param currentDate
 */
void CScheduleBaseWidget::setCurrendDateTime(const QDateTime &currentDate)
{
    m_calendarManager->getCalendarDateDataManage()->setCurrentDateTime(currentDate);
}

/**
 * @brief CScheduleBaseWidget::getCurrendDateTime   获取当前时间
 * @return
 */
QDateTime CScheduleBaseWidget::getCurrendDateTime() const
{
    return m_calendarManager->getCalendarDateDataManage()->getCurrentDate();
}

/**
 * @brief CScheduleBaseWidget::getShowLunar     获取是否显示农历信息
 * @return
 */
bool CScheduleBaseWidget::getShowLunar()
{
    return m_calendarManager->getShowLunar();
}

/**
 * @brief CScheduleBaseWidget::updateData       更新数据显示
 */
void CScheduleBaseWidget::updateData()
{
    updateShowDate();
    updateShowSchedule();
    updateShowLunar();
    updateSearchScheduleInfo();
}

/**
 * @brief CScheduleBaseWidget::updateDBusData           更新dbus数据
 */
void CScheduleBaseWidget::updateDBusData(const QDate &startDate, const QDate &stopDate, const bool isGetLunar)
{
    //获取日程任务
    CScheduleTask *_task = m_calendarManager->getScheduleTask();
    if (startDate.isValid() && stopDate.isValid()) {
        //更新日程信息
        _task->updateInfo(startDate, stopDate, isGetLunar);
    } else {
        qWarning() << "startDate or stopDate Err!";
    }
}

/**
 * @brief CScheduleBaseWidget::updateSearchScheduleInfo     更新搜索日程
 *  由子类完成
 */
void CScheduleBaseWidget::updateSearchScheduleInfo()
{

}

/**
 * @brief CScheduleBaseWidget::getLunarInfo 获取选择时间的农历信息
 * @return
 */
CaHuangLiDayInfo CScheduleBaseWidget::getLunarInfo()
{
    QMap<QDate, CaHuangLiDayInfo> _huangliInfo = m_calendarManager->getScheduleTask()->getHuangliInfo(getSelectDate(), getSelectDate());
    QMap<QDate, CaHuangLiDayInfo>::const_iterator interator = _huangliInfo.constBegin();
    if (interator == _huangliInfo.constEnd())
        return CaHuangLiDayInfo();
    m_lunarYear = QString("-%0%1年-").arg(interator.value().mGanZhiYear).arg(interator.value().mZodiac);
    m_lunarDay = QString("-农历%0%1-").arg(interator.value().mLunarMonthName).arg(interator.value().mLunarDayName);
    return interator.value();
}

/**
 * @brief CScheduleBaseWidget::slotScheduleShowUpdate       更新界面日程数据显示
 */
void CScheduleBaseWidget::slotScheduleShowUpdate()
{
    updateShowSchedule();
}
