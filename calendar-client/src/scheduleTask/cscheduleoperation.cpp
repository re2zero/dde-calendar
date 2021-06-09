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
#include "cscheduleoperation.h"

#include "cscheduledbus.h"
#include "schedulectrldlg.h"

CScheduleOperation::CScheduleOperation(QWidget *parent)
    : QObject(parent)
    , m_DBusManager(CScheduleDBus::getInstance())
    , m_widget(parent)
{

}

/**
 * @brief CScheduleOperation::createSchedule        创建日程
 * @param scheduleInfo
 */
bool CScheduleOperation::createSchedule(const ScheduleDataInfo &scheduleInfo)
{
    return m_DBusManager->CreateJob(scheduleInfo);
}

/**
 * @brief CScheduleOperation::changeSchedule        修改日程
 * @param newInfo
 * @param oldInfo
 */
bool CScheduleOperation::changeSchedule(const ScheduleDataInfo &newInfo, const ScheduleDataInfo &oldInfo)
{
    bool _result {false};
    if (newInfo.getRepetitionRule().getRuleId() == 0 && newInfo.getRepetitionRule().getRuleId() == oldInfo.getRepetitionRule().getRuleId()) {
        //如果为普通日程且没有修改重复类型则更新日程
        _result = m_DBusManager->UpdateJob(newInfo);
    } else {
        //如果切换了全天状态则提醒是否修改全部
        if (newInfo.getAllDay() != oldInfo.getAllDay()) {
            emit signalViewtransparentFrame(1);
            CScheduleCtrlDlg msgBox(m_widget);
            msgBox.setText(
                tr("All occurrences of a repeating event must have the same all-day status."));
            msgBox.setInformativeText(tr("Do you want to change all occurrences?"));
            msgBox.addPushButton(tr("Cancel", "button"), true);
            msgBox.addWaringButton(tr("Change All"), true);
            msgBox.exec();
            emit signalViewtransparentFrame(0);

            if (msgBox.clickButton() == 0) {
                _result = false;
            } else if (msgBox.clickButton() == 1) {
                //更新日程
                _result =  m_DBusManager->UpdateJob(newInfo);
            }
        } else if (oldInfo.getRepetitionRule().getRuleId() != newInfo.getRepetitionRule().getRuleId()) {
            //修改重复规则
            emit signalViewtransparentFrame(1);
            CScheduleCtrlDlg msgBox(m_widget);
            msgBox.setText(tr("You are changing the repeating rule of this event."));
            msgBox.setInformativeText(tr("Do you want to change all occurrences?"));
            msgBox.addPushButton(tr("Cancel", "button"), true);
            msgBox.addWaringButton(tr("Change All"), true);
            msgBox.exec();
            emit signalViewtransparentFrame(0);
            if (msgBox.clickButton() == 0) {
                _result = false;
            } else if (msgBox.clickButton() == 1) {
                //更新日程
                _result = m_DBusManager->UpdateJob(newInfo);
            }
        } else {
            _result = changeRecurInfo(newInfo, oldInfo);
        }
    }
    return  _result;
}

/**
 * @brief CScheduleOperation::deleteSchedule        删除日程
 * @param scheduleInfo
 */
bool CScheduleOperation::deleteSchedule(const ScheduleDataInfo &scheduleInfo)
{
    bool _restuleBool {false};
    //如果为普通日程
    if (scheduleInfo.getRepetitionRule().getRuleId() == 0) {
        emit signalViewtransparentFrame(1);
        CScheduleCtrlDlg msgBox(m_widget);
        msgBox.setText(tr("You are deleting an event."));
        msgBox.setInformativeText(tr("Are you sure you want to delete this event?"));
        msgBox.addPushButton(tr("Cancel", "button"), true);
        msgBox.addWaringButton(tr("Delete", "button"), true);
        msgBox.exec();
        emit signalViewtransparentFrame(0);
        if (msgBox.clickButton() == 0) {
            return false;
        } else if (msgBox.clickButton() == 1) {
            m_DBusManager->DeleteJob(scheduleInfo.getID());
            _restuleBool = true;
        }
    } else {
        //如果为重复日程的第一个
        if (scheduleInfo.getRecurID() == 0) {
            emit signalViewtransparentFrame(1);
            CScheduleCtrlDlg msgBox(m_widget);
            msgBox.setText(tr("You are deleting an event."));
            msgBox.setInformativeText(tr("Do you want to delete all occurrences of this event, or only the selected occurrence?"));
            msgBox.addPushButton(tr("Cancel", "button"));
            msgBox.addPushButton(tr("Delete All"));
            msgBox.addWaringButton(tr("Delete Only This Event"));
            msgBox.exec();
            emit signalViewtransparentFrame(0);
            if (msgBox.clickButton() == 0) {
                return false;
            } else if (msgBox.clickButton() == 1) {
                //删除所有日程
                m_DBusManager->DeleteJob(scheduleInfo.getID());
                _restuleBool = true;
            } else if (msgBox.clickButton() == 2) {
                //仅删除此日程
                ScheduleDataInfo newschedule;
                m_DBusManager->GetJob(scheduleInfo.getID(), newschedule);
                newschedule.getIgnoreTime().append(scheduleInfo.getBeginDateTime());
                m_DBusManager->UpdateJob(newschedule);
                _restuleBool = true;
            }
        } else {
            emit signalViewtransparentFrame(1);
            CScheduleCtrlDlg msgBox(m_widget);
            msgBox.setText(tr("You are deleting an event."));
            msgBox.setInformativeText(tr("Do you want to delete this and all future occurrences of this event, or only the selected occurrence?"));
            msgBox.addPushButton(tr("Cancel", "button"));
            msgBox.addPushButton(tr("Delete All Future Events"));
            msgBox.addWaringButton(tr("Delete Only This Event"));
            msgBox.exec();
            emit signalViewtransparentFrame(0);

            if (msgBox.clickButton() == 0) {
                return false;
            } else if (msgBox.clickButton() == 1) {
                //删除选中日程及之后的日程
                ScheduleDataInfo newschedule;
                //获取原始日程信息
                m_DBusManager->GetJob(scheduleInfo.getID(), newschedule);
                //修改重复规则
                changeRepetitionRule(newschedule, scheduleInfo);
                //如果修改后的日程为普通日程且忽略列表内包含日程开始时间则删除该日程
                if (newschedule.getRepetitionRule().getRuleId() == RepetitionRule::RRule_NONE && newschedule.getIgnoreTime().contains(newschedule.getBeginDateTime())) {
                    //删除日程
                    m_DBusManager->DeleteJob(newschedule.getID());
                } else {
                    //更新日程
                    m_DBusManager->UpdateJob(newschedule);
                }

                _restuleBool = true;
            } else if (msgBox.clickButton() == 2) {
                ScheduleDataInfo newschedule;
                m_DBusManager->GetJob(scheduleInfo.getID(), newschedule);
                newschedule.getIgnoreTime().append(scheduleInfo.getBeginDateTime());
                m_DBusManager->UpdateJob(newschedule);
                _restuleBool = true;
            }
        }
    }
    return  _restuleBool;
}

QString CScheduleOperation::queryScheduleStr(const QString &key, const QDateTime &startTime, const QDateTime &endTime)
{
    QString _resultStr;
    m_DBusManager->QueryJobs(key, startTime, endTime, _resultStr);
    return _resultStr;
}

bool CScheduleOperation::queryScheduleInfo(const QString &key, const QDateTime &startTime, const QDateTime &endTime, QMap<QDate, QVector<ScheduleDataInfo> > &info)
{
    return m_DBusManager->QueryJobs(key, startTime, endTime, info);
}

/**
 * @brief CScheduleOperation::deleteOnlyInfo        删除日程 仅删除此日程 不弹框提醒
 * @param scheduleInfo
 */
void CScheduleOperation::deleteOnlyInfo(const ScheduleDataInfo &scheduleInfo)
{
    //如果为纪念日或节日则不处理
    if (scheduleInfo.getType() == 4)
        return;
    //如果为普通日程则删除
    if (scheduleInfo.getRepetitionRule().getRuleId() == RepetitionRule::RRule_NONE) {
        m_DBusManager->DeleteJob(scheduleInfo.getID());
    } else {
        //仅删除此日程
        ScheduleDataInfo newschedule;
        m_DBusManager->GetJob(scheduleInfo.getID(), newschedule);
        newschedule.getIgnoreTime().append(scheduleInfo.getBeginDateTime());
        m_DBusManager->UpdateJob(newschedule);
    }
}

/**
 * @brief CScheduleOperation::ChangeRecurInfo       修改重复日程
 * @param newinfo
 * @param oldinfo
 */
bool CScheduleOperation::changeRecurInfo(const ScheduleDataInfo &newinfo, const ScheduleDataInfo &oldinfo)
{
    bool _result{false};
    //如果为重复类型第一个
    if (newinfo.getRecurID() == 0) {
        emit signalViewtransparentFrame(1);
        CScheduleCtrlDlg msgBox(m_widget);
        msgBox.setText(tr("You are changing a repeating event."));
        msgBox.setInformativeText(
            tr("Do you want to change only this occurrence of the event, or all "
               "occurrences?"));
        msgBox.addPushButton(tr("Cancel", "button"));
        msgBox.addPushButton(tr("All"));
        msgBox.addsuggestButton(tr("Only This Event"));
        msgBox.exec();
        emit signalViewtransparentFrame(0);

        if (msgBox.clickButton() == 0) {
            _result = false;
        } else if (msgBox.clickButton() == 1) {
            //修改所有日程
            ScheduleDataInfo _scheduleDataInfo = newinfo;
            RepetitionRule _rule = _scheduleDataInfo.getRepetitionRule();
            //如果此重复日程只有它一个则将修改为普通日程
            if ((_rule.getRuleType() == 1 && _rule.getEndCount() < 1) ||
                    (_rule.getRuleType() == 2 && _scheduleDataInfo.getBeginDateTime().daysTo(_rule.getEndDate()) < 0)) {
                _rule.setRuleId(RepetitionRule::RRuleID::RRule_NONE);
                _rule.setRuleType(RepetitionRule::RRuleEndType::RRuleType_NEVER);
            }
            _scheduleDataInfo.setRepetitionRule(_rule);
            //TODO 清空忽略日程
            _scheduleDataInfo.getIgnoreTime().clear();
            //更新日程
            _result = m_DBusManager->UpdateJob(_scheduleDataInfo);
        } else if (msgBox.clickButton() == 2) {
            //仅修改此日程
            _result = changeOnlyInfo(newinfo, oldinfo);
        }
    } else {
        emit signalViewtransparentFrame(1);
        CScheduleCtrlDlg msgBox(m_widget);
        msgBox.setText(tr("You are changing a repeating event."));
        msgBox.setInformativeText(
            tr("Do you want to change only this occurrence of the event, or this and "
               "all future occurrences?"));
        msgBox.addPushButton(tr("Cancel", "button"));
        msgBox.addPushButton(tr("All Future Events"));
        msgBox.addsuggestButton(tr("Only This Event"));
        msgBox.exec();
        emit signalViewtransparentFrame(0);

        if (msgBox.clickButton() == 0) {
            _result = false;
        } else if (msgBox.clickButton() == 1) {
            // 根据id获取日程并修改
            ScheduleDataInfo updatescheduleData;
            // 获取原始数据
            m_DBusManager->GetJob(oldinfo.getID(), updatescheduleData);
            //修改重复规则
            changeRepetitionRule(updatescheduleData, newinfo);
            //如果修改后的日程为普通日程且忽略列表内包含日程开始时间则删除该日程
            if (updatescheduleData.getRepetitionRule().getRuleId() == RepetitionRule::RRule_NONE && updatescheduleData.getIgnoreTime().contains(updatescheduleData.getBeginDateTime())) {
                //删除日程
                m_DBusManager->DeleteJob(updatescheduleData.getID());
            } else {
                //更新日程
                m_DBusManager->UpdateJob(updatescheduleData);
            }
            //创建日程
            ScheduleDataInfo newschedule = newinfo;
            //获取重复规则
            RepetitionRule _rule = newschedule.getRepetitionRule();
            if (_rule.getRuleType() == 1) {
                //更新重复规则
                _rule.setEndCount(qAbs(_rule.getEndCount() - newschedule.getRecurID()));
                if (_rule.getEndCount() < 1) {
                    _rule.setRuleId(RepetitionRule::RRuleID::RRule_NONE);
                    _rule.setRuleType(RepetitionRule::RRuleEndType::RRuleType_NEVER);
                }
            }
            newschedule.setRecurID(0);
            newschedule.setID(0);
            newschedule.setRepetitionRule(_rule);
            //创建新日程
            _result = m_DBusManager->CreateJob(newschedule);
        } else if (msgBox.clickButton() == 2) {
            _result = changeOnlyInfo(newinfo, oldinfo);
        }
    }
    return _result;
}

/**
 * @brief CScheduleOperation::ChangeOnlyInfo        修改重复日程,仅修改此日程
 * @param newinfo
 * @param oldinfo
 */
bool CScheduleOperation::changeOnlyInfo(const ScheduleDataInfo &newinfo, const ScheduleDataInfo &oldinfo)
{
    ScheduleDataInfo newschedule = newinfo;
    newschedule.getRepetitionRule().clear();
    newschedule.setRecurID(0);
    newschedule.setID(0);
    newschedule.getIgnoreTime().clear();
    //创建日程
    m_DBusManager->CreateJob(newschedule);
    ScheduleDataInfo updatescheduleData;
    //获取原始信息
    m_DBusManager->GetJob(oldinfo.getID(), updatescheduleData);
    updatescheduleData.getIgnoreTime().append(oldinfo.getBeginDateTime());
    //更新原始信息
    return m_DBusManager->UpdateJob(updatescheduleData);
}

/**
 * @brief CScheduleOperation::changeRepetitionRule      修改日程重复规则
 * @param newinfo
 * @param oldinfo
 */
void CScheduleOperation::changeRepetitionRule(ScheduleDataInfo &newinfo, const ScheduleDataInfo &oldinfo)
{
    switch (newinfo.getRepetitionRule().getRuleType()) {
    case RepetitionRule::RRuleType_FREQ: {
        //如果为结束与次数则修改结束次数
        newinfo.getRepetitionRule().setEndCount(oldinfo.getRecurID() - 1);
        //结束次数为0表示不重复，设置为普通日程
        if (newinfo.getRepetitionRule().getEndCount() < 1) {
            newinfo.getRepetitionRule().setRuleId(RepetitionRule::RRuleID::RRule_NONE);
            newinfo.getRepetitionRule().setRuleType(RepetitionRule::RRuleEndType::RRuleType_NEVER);
        }
        break;
    }
    default: {
        //如果该日程结束类型为永不和结束于日期则修改结束日期
        newinfo.getRepetitionRule().setRuleType(RepetitionRule::RRuleType_DATE);
        newinfo.getRepetitionRule().setEndDate(oldinfo.getBeginDateTime().addDays(-1));
        break;
    }
    }
}
