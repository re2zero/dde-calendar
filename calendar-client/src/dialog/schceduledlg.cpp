/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     kirigaya <kirigaya@mkacg.com>
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
#include "schceduledlg.h"
#include "schcedulectrldlg.h"
#include "scheduledatamanage.h"
#include "timeedit.h"
#include "constants.h"

#include <DHiDPIHelper>
#include <DMessageBox>
#include <DPalette>
#include <DFontSizeManager>

#include <QCalendarWidget>
#include <QHBoxLayout>
#include <QIcon>
#include <QIntValidator>
#include <QMessageBox>
#include <QShortcut>
#include <QTextBlock>
#include <QToolButton>
#include <QVBoxLayout>
#include <QKeyEvent>

DGUI_USE_NAMESPACE
/**
 * @brief CSchceduleDlg::CSchceduleDlg 构造函数
 * @param type 1为新建日程，0为编辑日程
 * @param parent 父类
 * @param isAllDay 是否是全天日程
 */
CSchceduleDlg::CSchceduleDlg(int type, QWidget *parent, const bool isAllDay)
    : DDialog(parent)
    , m_createAllDay(isAllDay)
{
    //设置中心边距
    setContentsMargins(0, 0, 0, 0);
    //新建or编辑
    m_type = type;
    //初始化界面
    initUI();
    //初始化信号连接
    initConnection();
    //编辑or新建
    if (type == 1) {
        //设置title文字
        m_titleLabel->setText(tr("New Event"));
        //设置日程开始时间
        m_beginDateEdit->setDate(QDate::currentDate());
        //设置小时
        int hours = QTime::currentTime().hour();
        //设置分钟，以15分钟为单位
        int minnutes = QTime::currentTime().minute() % DDECalendar::QuarterOfAnhourWithMinute;
        //以15分钟为单位，向后取整
        if (minnutes != 0) {
            minnutes = QTime::currentTime().minute() / DDECalendar::QuarterOfAnhourWithMinute * DDECalendar::QuarterOfAnhourWithMinute + DDECalendar::QuarterOfAnhourWithMinute;
        }
        //显示日程开始时间
        m_beginTimeEdit->setTime(QTime(hours, minnutes));
        //设置日程结束日期
        m_endDateEdit->setDate(QDate::currentDate());
        //设置日程结束时间为一小时后
        m_endTimeEdit->setTime(QTime(hours, minnutes).addSecs(3600));
    } else {
        //编辑日程
        m_titleLabel->setText(tr("Edit Event"));
    }

    setFocusPolicy(Qt::WheelFocus);
    setFixedSize(438, 470);
}
/**
 * @brief CSchceduleDlg::~CSchceduleDlg 析构函数
 */
CSchceduleDlg::~CSchceduleDlg()
{
    emit signalViewtransparentFrame(0);
}
/**
 * @brief CSchceduleDlg::setData 设置日程信息
 * @param info 日程信息
 */
void CSchceduleDlg::setData(const ScheduleDtailInfo &info)
{
    //日程信息
    m_scheduleDtailInfo = info;
    //设置日程类型
    m_typeComBox->setCurrentIndex(info.type.ID - 1);
    //设置日程详情
    m_textEdit->setPlainText(info.titleName);
    //设置日程开始日期
    m_beginDateEdit->setDate(info.beginDateTime.date());
    //设置日程开始时间
    m_beginTimeEdit->setTime(info.beginDateTime.time());
    //设置日程结束日期
    m_endDateEdit->setDate(info.endDateTime.date());
    //设置日程结束时间
    m_endTimeEdit->setTime(info.endDateTime.time());
    //是否全天
    m_allDayCheckbox->setChecked(info.allday);
    //设置结束于日期的最早时间
    m_endRepeatDate->setMinimumDate(info.beginDateTime.date());
    //设置日程开始时间为当前时间
    m_currentDate = info.beginDateTime;
    //日程结束时间
    m_EndDate = info.endDateTime;
    //根据是否为全天，改变状态
    slotallDayStateChanged(info.allday);
    //更新提醒界面
    initRmindRpeatUI();
}
/**
 * @brief CSchceduleDlg::setDate 设置日期
 * @param date 日期
 */
void CSchceduleDlg::setDate(const QDateTime &date)
{
    m_currentDate = date;
    int hours = date.time().hour();
    int minnutes = 0;
    //以15分钟为单位，向后取整
    if (date.date() == QDate::currentDate()) {
        //今天
        minnutes = (date.time().minute() / DDECalendar::QuarterOfAnhourWithMinute + 1) * DDECalendar::QuarterOfAnhourWithMinute;
    } else {
        //不是今天
        int minnutes = date.time().minute() % DDECalendar::QuarterOfAnhourWithMinute;
        if (minnutes != 0) {
            minnutes = (date.time().minute() / DDECalendar::QuarterOfAnhourWithMinute + 1) * DDECalendar::QuarterOfAnhourWithMinute;
        }
    }
    //+1小时
    if (minnutes == 60) {
        //+1天
        if (hours + 1 == 24) {
            m_currentDate.setTime(QTime(0, 0));
            m_currentDate = m_currentDate.addDays(1);
        } else {
            m_currentDate.setTime(QTime(hours + 1, 0));
        }
    } else {
        //设置时间
        m_currentDate.setTime(QTime(hours, minnutes));
    }
    //DDateEdit显示日程开始时间
    m_beginDateEdit->setDate(m_currentDate.date());
    m_beginTimeEdit->setTime(m_currentDate.time());
    //设置结束时间
    QDateTime datetime = m_currentDate.addSecs(3600);
    m_EndDate = datetime;
    //DDateEdit显示日程结束时间
    m_endDateEdit->setDate(datetime.date());
    m_endTimeEdit->setTime(datetime.time());
    //设置重复日程结束于日期的最早时间
    m_endRepeatDate->setMinimumDate(date.date());
}
/**
 * @brief CSchceduleDlg::getScheduleData 获取日程信息
 * @return 日程信息
 */
ScheduleDtailInfo CSchceduleDlg::getScheduleData()
{
    return m_scheduleDtailInfo;
}
/**
 * @brief CSchceduleDlg::setAllDay 设置全天标志
 * @param flag 是否全天
 */
void CSchceduleDlg::setAllDay(bool flag)
{
    //DCheckBox设置是否checked
    m_allDayCheckbox->setChecked(flag);
}
/**
 * @brief CSchceduleDlg::slotCancelBt "取消"按钮功能
 * @param buttonIndex 按钮索引
 * @param buttonName 按钮名称
 */
void CSchceduleDlg::slotCancelBt(int buttonIndex, QString buttonName)
{
    //如果不是“取消”按钮，return
    if (buttonIndex != 0 && buttonName != "Cancel")
        return;

    reject();
}
/**
 * @brief CSchceduleDlg::slotOkBt "保存”按钮功能
 * @param buttonIndex 按钮索引
 * @param buttonName 按钮名称
 */
void CSchceduleDlg::slotOkBt(int buttonIndex, QString buttonName)
{
    //如果不是“保存”按钮，return
    if (buttonIndex != 1 && buttonName != "Save")
        return;
    //系统主题类型
    int themetype = CScheduleDataManage::getScheduleDataManage()->getTheme();
    //日程信息
    ScheduleDtailInfo scheduleDtailInfo = m_scheduleDtailInfo;
    QDateTime beginDateTime, endDateTime;
    //日程开始时间和日期
    beginDateTime.setDate(m_beginDateEdit->date());
    beginDateTime.setTime(m_beginTimeEdit->getTime());
    //日程结束时间和日期
    endDateTime.setDate(m_endDateEdit->date());
    endDateTime.setTime(m_endTimeEdit->getTime());
    //设置日程详情
    if (m_textEdit->toPlainText().isEmpty()) {
        //如果没有输入，使用默认的占位符作为日程详情
        scheduleDtailInfo.titleName = m_textEdit->placeholderText();
    } else {
        //设置textedit内容作为日程详情
        scheduleDtailInfo.titleName = m_textEdit->toPlainText();
    }
    //日程详情为空，return
    if (scheduleDtailInfo.titleName.isEmpty()) {
        return;
    }
    //日程开始时间小于结束时间，显示提示信息，return
    if (beginDateTime > endDateTime) {
        // new dialog
        DDialog *prompt = new DDialog(this);
        //设置icon
        prompt->setIcon(QIcon(":/resources/icon/warning.svg"));
        //设置提醒文字
        prompt->setMessage(tr("End time must be greater than start time"));
        //设置windowflags
        prompt->setWindowFlags(prompt->windowFlags() | Qt::WindowStaysOnTopHint);
        //添加button
        prompt->addButton(tr("OK"), true, DDialog::ButtonNormal);
        //展示
        prompt->exec();
        return;
    }
    //新建日程，设置id
    if (m_type == 1)
        scheduleDtailInfo.id = 0;
    //设置全天标志
    scheduleDtailInfo.allday = m_allDayCheckbox->isChecked();
    //是否提醒
    if (m_rmindCombox->currentIndex() == 0)
        //为0,不提醒
        scheduleDtailInfo.remind = false;
    else {
        //其他值，提醒
        scheduleDtailInfo.remind = true;
    }
    //根据是否是全天日程，设置不同的提醒内容
    if (scheduleDtailInfo.allday) {
        //全天
        if (scheduleDtailInfo.remind) {
            //重复日程，默认为9点
            scheduleDtailInfo.remindData.time = QTime(9, 0);
            //提醒类型
            if (m_rmindCombox->currentIndex() == 1) {
                //日程开始时提醒
                scheduleDtailInfo.remindData.n = DDECalendar::OnStartDay;
            } else if (m_rmindCombox->currentIndex() == 2) {
                //日程开始前一天提醒
                scheduleDtailInfo.remindData.n = DDECalendar::OneDayBeforeWithDay;
            } else if (m_rmindCombox->currentIndex() == 3) {
                //日程开始前两天提醒
                scheduleDtailInfo.remindData.n = DDECalendar::TwoDayBeforeWithDay;
            } else if (m_rmindCombox->currentIndex() == 4) {
                //日程开始前一周提醒
                scheduleDtailInfo.remindData.n = DDECalendar::OneWeekBeforeWithDay;
            }
        }
    } else {
        //非全天
        if (scheduleDtailInfo.remind) {
            //重复日程，重复类型
            if (m_rmindCombox->currentIndex() == 1) {
                //日程开始时提醒
                scheduleDtailInfo.remindData.n = DDECalendar::AtTimeOfEvent;
            } else if (m_rmindCombox->currentIndex() == 2) {
                //日程开始前15分钟提醒
                scheduleDtailInfo.remindData.n = DDECalendar::FifteenMinutesBefore;
            } else if (m_rmindCombox->currentIndex() == 3) {
                //日程开始前30分钟提醒
                scheduleDtailInfo.remindData.n = DDECalendar::ThirtyMinutesBefore;
            } else if (m_rmindCombox->currentIndex() == 4) {
                //日程开始前1小时提醒
                scheduleDtailInfo.remindData.n = DDECalendar::OneHourBefore;
            } else if (m_rmindCombox->currentIndex() == 5) {
                //日程开始前一天提醒
                scheduleDtailInfo.remindData.n = DDECalendar::OneDayBeforeWithMinutes;
            } else if (m_rmindCombox->currentIndex() == 6) {
                //日程开始前两天提醒
                scheduleDtailInfo.remindData.n = DDECalendar::TwoDayBeforeWithMinutes;
            } else if (m_rmindCombox->currentIndex() == 7) {
                //日程开始前一周提醒
                scheduleDtailInfo.remindData.n = DDECalendar::OneWeekBeforeWithMinutes;
            }
        }
    }
    //日程重复类型
    scheduleDtailInfo.rpeat = m_beginrepeatCombox->currentIndex();
    //设置重复日程结束于
    if (scheduleDtailInfo.rpeat != 0) {
        //设置结束重复的类型
        scheduleDtailInfo.enddata.type = m_endrepeatCombox->currentIndex();
        //结束于类型
        if (m_endrepeatCombox->currentIndex() == 1) {
            //结束于次数
            if (m_endrepeattimes->text().isEmpty()) {
                return;
            }
            //设置重复的次数
            scheduleDtailInfo.enddata.tcount = m_endrepeattimes->text().toInt();
        } else if (m_endrepeatCombox->currentIndex() == 2) {
            //结束与日期
            QDateTime endrpeattime = beginDateTime;
            endrpeattime.setDate(m_endRepeatDate->date());
            //结束于日期不得早于日程开始日期
            if (beginDateTime > endrpeattime) {
                return;
            }
            //设置结束重复的日期
            scheduleDtailInfo.enddata.date = endrpeattime;
        }
    }
    //获取日程类型
    CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->GetType(
        m_typeComBox->currentIndex() + 1, scheduleDtailInfo.type);
    //设置日程开始结束时间
    scheduleDtailInfo.beginDateTime = beginDateTime;
    scheduleDtailInfo.endDateTime = endDateTime;
    //新建日程or编辑日程
    if (m_type == 1) {
        //新建日程
        CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->addSchedule(
            scheduleDtailInfo);
    } else if (m_type == 0) {
        //编辑日程
        if (m_scheduleDtailInfo.rpeat == 0 &&
                m_scheduleDtailInfo.rpeat == scheduleDtailInfo.rpeat) {
            //非重复日程，更新日程
            CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(
                scheduleDtailInfo);
        } else {
            //重复日程
            if (m_scheduleDtailInfo.allday != scheduleDtailInfo.allday) {
                //修改日程的全天标志
                CSchceduleCtrlDlg msgBox;
                //设置提示语
                msgBox.setText(
                    tr("All occurrences of a repeating event must have the same all-day status."));
                msgBox.setInformativeText(tr("Do you want to change all occurrences?"));
                //设置按钮文字
                msgBox.addPushButton(tr("Cancel"), true);
                msgBox.addWaringButton(tr("Change All"), true);
                msgBox.exec();
                //按钮对应的功能
                if (msgBox.clickButton() == 0) {
                    //取消
                    return;
                } else if (msgBox.clickButton() == 1) {
                    //修改所有
                    CScheduleDataManage::getScheduleDataManage()
                    ->getscheduleDataCtrl()
                    ->updateScheduleInfo(scheduleDtailInfo);
                }
            } else if (m_scheduleDtailInfo.rpeat != scheduleDtailInfo.rpeat) {
                //修改日程的重复类型
                CSchceduleCtrlDlg msgBox;
                //设置提醒信息
                msgBox.setText(tr("You are changing the repeating rule of this event."));
                msgBox.setInformativeText(tr("Do you want to change all occurrences?"));
                //设置按钮文字
                msgBox.addPushButton(tr("Cancel"), true);
                msgBox.addWaringButton(tr("Change All"), true);
                msgBox.exec();
                //按钮对应的功能
                if (msgBox.clickButton() == 0) {
                    //取消
                    return;
                } else if (msgBox.clickButton() == 1) {
                    //修改所有
                    CScheduleDataManage::getScheduleDataManage()
                    ->getscheduleDataCtrl()
                    ->updateScheduleInfo(scheduleDtailInfo);
                }
            } else {
                //修改重复日程
                ChangeRecurInfo(this, scheduleDtailInfo, m_scheduleDtailInfo, themetype);
            }
        }
    }
    accept();
    emit  signalScheduleUpdate();
}
/**
 * @brief CSchceduleDlg::slotTextChange 更改日程详情
 */
void CSchceduleDlg::slotTextChange()
{
    //带游标的文字副本
    QTextCursor cursor = m_textEdit->textCursor();
    //纯文本
    QString textContent = m_textEdit->toPlainText();
    //文本字数
    int length = textContent.count();
    QString tStitlename = textContent;

    if (tStitlename.contains("\n")) {
        //将”\n"转换为“”
        tStitlename.replace("\n", "");
        m_textEdit->setText(tStitlename);
        cursor.movePosition(QTextCursor::End);
        m_textEdit->setTextCursor(cursor);
        return;
    }
    // 最大字符数
    int maxLength = 256;
    //如果字符数大于最大字符数
    if (length > maxLength) {
        m_textEdit->setText(m_context);
        cursor.movePosition(QTextCursor::End);
        m_textEdit->setTextCursor(cursor);
    }
    m_context = m_textEdit->toPlainText();
}
/**
 * @brief CSchceduleDlg::slotendrepeatTextchange 重复日程重复次数改变
 */
void CSchceduleDlg::slotendrepeatTextchange()
{
    //获取“保存”按钮
    QAbstractButton *m_OkBt = getButton(1);
    //设置按钮状态
    if (m_endrepeattimes->text().isEmpty())
        //如果重复次数为空，“保存”按钮失能
        m_OkBt->setEnabled(false);
    else
        //“保存”按钮使能
        m_OkBt->setEnabled(true);
}
/**
 * @brief CSchceduleDlg::slotBDateEidtInfo 日程开始日期改变
 * @param date 日期
 */
void CSchceduleDlg::slotBDateEidtInfo(const QDate &date)
{
    //设置日程开始日期
    m_beginDateEdit->setDate(date);
    //重复日程最早结束于date
    m_endRepeatDate->setMinimumDate(date);
    //结束日期最早为date
    m_endDateEdit->setMinimumDate(date);
    //设置日程开始时间和日期
    QDateTime beginDateTime, endDateTime;
    beginDateTime.setDate(m_beginDateEdit->date());
    beginDateTime.setTime(m_beginTimeEdit->getTime());
    //设置日程结束时间和日期
    endDateTime.setDate(m_endDateEdit->date());
    endDateTime.setTime(m_endTimeEdit->getTime());
    //结束时间早于开始时间，设置结束时间为开始时间+1小时
    if (endDateTime < beginDateTime) {
        m_endTimeEdit->setTime(m_beginTimeEdit->getTime().addSecs(3600));
    }
}
/**
 * @brief CSchceduleDlg::slotallDayStateChanged 全天状态改变
 * @param state 状态
 */
void CSchceduleDlg::slotallDayStateChanged(int state)
{
    //清楚combox
    m_rmindCombox->clear();
    //设置combox内容
    if (!state) {
        //非全天日程
        //从不提醒
        m_rmindCombox->addItem(tr("Never"));
        //日程开始时提醒
        m_rmindCombox->addItem(tr("At time of event"));
        //日程开始前15分钟提醒
        m_rmindCombox->addItem(tr("15 minutes before"));
        //日程开始前30分钟提醒
        m_rmindCombox->addItem(tr("30 minutes before"));
        //日程开始前一小时提醒
        m_rmindCombox->addItem(tr("1 hour before"));
        //日程开始前一天提醒
        m_rmindCombox->addItem(tr("1 day before"));
        //日程开始前两天提醒
        m_rmindCombox->addItem(tr("2 days before"));
        //日程开始前一周提醒
        m_rmindCombox->addItem(tr("1 week before"));
        m_beginTimeEdit->setVisible(true);
        m_endTimeEdit->setVisible(true);
        //新建日程or编辑日程
        if (m_type == 0) {
            //编辑日程
            //设置日程开始时间
            m_beginDateEdit->setDate(m_scheduleDtailInfo.beginDateTime.date());
            m_beginTimeEdit->setTime(m_scheduleDtailInfo.beginDateTime.time());
            //设置日程结束时间
            m_endDateEdit->setDate(m_scheduleDtailInfo.endDateTime.date());
            m_endTimeEdit->setTime(m_scheduleDtailInfo.endDateTime.time());
        } else {
            //新建日程
            //设置日程开始时间
            m_beginDateEdit->setDate(m_currentDate.date());
            m_beginTimeEdit->setTime(m_currentDate.time());
            //设置日程结束时间
            m_endDateEdit->setDate(m_EndDate.date());
            m_endTimeEdit->setTime(m_EndDate.time());
        }
    } else {
        //全天日程
        //从不提醒
        m_rmindCombox->addItem(tr("Never"));
        //日程开始当天9点提醒
        m_rmindCombox->addItem(tr("On start day (9:00 AM)"));
        //日程开始前一天提醒
        m_rmindCombox->addItem(tr("1 day before"));
        //日程开始前两天提醒
        m_rmindCombox->addItem(tr("2 days before"));
        //日程开始前一周提醒
        m_rmindCombox->addItem(tr("1 week before"));
        m_beginTimeEdit->setVisible(false);
        m_endTimeEdit->setVisible(false);
        //新建日程or编辑日程
        if (m_type == 0) {
            //编辑日程
            //设置日程开始时间
            m_beginDateEdit->setDate(m_scheduleDtailInfo.beginDateTime.date());
            m_beginTimeEdit->setTime(QTime(0, 0));
            //设置日程结束时间
            m_endDateEdit->setDate(m_scheduleDtailInfo.endDateTime.date());
            m_endTimeEdit->setTime(QTime(23, 59));
        } else {
            //新建日程
            //设置日程开始时间
            m_beginDateEdit->setDate(m_currentDate.date());
            m_endDateEdit->setDate(m_EndDate.date());
            //设置日程结束时间
            m_beginTimeEdit->setTime(QTime(0, 0));
            m_endTimeEdit->setTime(QTime(23, 59));
            //默认日程开始当天9点提醒
            m_rmindCombox->setCurrentIndex(2);
        }
    }
}
/**
 * @brief CSchceduleDlg::slotbRpeatactivated 根据重复类型设置大小
 * @param index 索引值
 */
void CSchceduleDlg::slotbRpeatactivated(int index)
{
    if (index > 0) {
        //重复日程
        m_endrepeatWidget->setVisible(true);
        //设置页面大小
        setFixedSize(438, 506);
    } else {
        //非重复日程
        m_endrepeatWidget->setVisible(false);
        setFixedSize(438, 470);
    }
}
/**
 * @brief CSchceduleDlg::sloteRpeatactivated 重复结束于
 * @param index 索引值
 */
void CSchceduleDlg::sloteRpeatactivated(int index)
{
    if (index == 0) {
        //永不
        //设置结束于次数失能
        m_endrepeattimesWidget->setVisible(false);
        //设置结束于日期使能
        m_endRepeatDate->setVisible(false);
    } else if (index == 1) {
        //结束于次数
        m_endrepeattimesWidget->setVisible(true);
        m_endRepeatDate->setVisible(false);
    } else {
        //结束与日期
        m_endrepeattimesWidget->setVisible(false);
        m_endRepeatDate->setVisible(true);
    }
}
/**
 * @brief CSchceduleDlg::eventFilter 过滤器
 * @param obj 事件对象
 * @param pEvent 事件类型
 * @return 过滤掉事件，返回true;否则返回假。
 */
bool CSchceduleDlg::eventFilter(QObject *obj, QEvent *pEvent)
{
    if (obj == m_textEdit) {
        if (pEvent->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(pEvent);
            if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter) {
                return true;
            }
            if (keyEvent->key() == Qt::Key_Tab)
                return true;
        }
        if (m_type == 1) {
            if (pEvent->type() == QEvent::FocusIn) {
                //清空编辑框默认占位符
                m_textEdit->setPlaceholderText("");
            } else if (pEvent->type() == QEvent::FocusOut) {
                //设置编辑框默认占位符
                m_textEdit->setPlaceholderText(tr("New event"));
            }
        }
    }
    return QDialog::eventFilter(obj, pEvent);
}
/**
 * @brief CSchceduleDlg::showEvent 展示
 * @param event unused
 */
void CSchceduleDlg::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    emit signalViewtransparentFrame(1);
}
/**
 * @brief CSchceduleDlg::changeEvent 字体大小改变事件
 * @param event unused
 */
void CSchceduleDlg::changeEvent(QEvent *event)
{
    Q_UNUSED(event);
    QFont mlabelF;
    mlabelF.setWeight(QFont::Medium);
    //设置“类型”的最大宽度，多余文字显示...
    QFontMetrics fontWidth_typeLabel(mlabelF);
    QString str_typelabel = fontWidth_typeLabel.elidedText(tr("Type:"), Qt::ElideRight, DDECalendar::NewSchceduleLabelWidth);
    m_typeLabel->setText(str_typelabel);
    //设置“内容”的最大宽度，多余文字显示...
    QFontMetrics fontWidth_contentlabel(mlabelF);
    QString str_contentlabel = fontWidth_contentlabel.elidedText(tr("Description:"), Qt::ElideRight, DDECalendar::NewSchceduleLabelWidth);
    m_contentLabel->setText(str_contentlabel);
    //设置“全天”的最大宽度，多余文字显示...
    QFontMetrics fontWidth_allDayLabel(mlabelF);
    QString str_allDayLabel = fontWidth_allDayLabel.elidedText(tr("All Day:"), Qt::ElideRight, DDECalendar::NewSchceduleLabelWidth);
    m_adllDayLabel->setText(str_allDayLabel);
    //设置“开始时间”的最大宽度，多余文字显示...
    QFontMetrics fontWidth_beginTimeLabel(mlabelF);
    QString str_beginTimeLabel = fontWidth_beginTimeLabel.elidedText(tr("Starts:"), Qt::ElideRight, DDECalendar::NewSchceduleLabelWidth);
    m_beginTimeLabel->setText(str_beginTimeLabel);
    //设置“结束时间”的最大宽度，多余文字显示...
    QFontMetrics fontWidth_endTimeLabel(mlabelF);
    QString str_endTimeLabel = fontWidth_endTimeLabel.elidedText(tr("Ends:"), Qt::ElideRight, DDECalendar::NewSchceduleLabelWidth);
    m_endTimeLabel->setText(str_endTimeLabel);
    //设置“提醒”的最大宽度，多余文字显示...
    QFontMetrics fontWidth_remindSetLabel(mlabelF);
    QString str_remindSetLabel = fontWidth_remindSetLabel.elidedText(tr("Remind Me:"), Qt::ElideRight, DDECalendar::NewSchceduleLabelWidth);
    m_remindSetLabel->setText(str_remindSetLabel);
    //设置“重复”的最大宽度，多余文字显示...
    QFontMetrics fontWidth_beginRepeatLabel(mlabelF);
    QString str_beginRepeatLabel = fontWidth_beginRepeatLabel.elidedText(tr("Repeat:"), Qt::ElideRight, DDECalendar::NewSchceduleLabelWidth);
    m_beginrepeatLabel->setText(str_beginRepeatLabel);
    //设置“结束重复”的最大宽度，多余文字显示...
    QFontMetrics fontWidth_endrepeatLabel(mlabelF);
    QString str_endrepeatLabel = fontWidth_endrepeatLabel.elidedText(tr("End Repeat:"), Qt::ElideRight, DDECalendar::NewSchceduleLabelWidth);
    m_endrepeatLabel->setText(str_endrepeatLabel);
}
/**
 * @brief CSchceduleDlg::initUI 初始化界面
 */
void CSchceduleDlg::initUI()
{
    //设置对象名称和辅助显示名称
    this->setObjectName("ScheduleEditDialog");
    this->setAccessibleName("ScheduleEditDialog");
    //在点击任何对话框上的按钮后不关闭对话框，保证关闭子窗口时不被一起关掉
    setOnButtonClickedClose(false);
    //设置title
    m_titleLabel = new QLabel(this);
    QFont titlelabelF;
    titlelabelF.setWeight(QFont::DemiBold);
    titlelabelF.setPixelSize(17);
    m_titleLabel->setFixedSize(148, 51);
    m_titleLabel->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);
    m_titleLabel->move(145, 0);
    m_titleLabel->setFont(titlelabelF);
    setSpacing(0);
    QFont mlabelF;
    mlabelF.setWeight(QFont::Medium);
    //main布局
    QVBoxLayout *maintlayout = new QVBoxLayout;
    maintlayout->setMargin(0);
    maintlayout->setSpacing(10);
    //“类型”布局
    QHBoxLayout *typelayout = new QHBoxLayout;
    typelayout->setSpacing(0);
    typelayout->setMargin(0);
    //“类型”label
    m_typeLabel = new QLabel();
    m_typeLabel->setToolTip(tr("Type"));
    DFontSizeManager::instance()->bind(m_typeLabel, DFontSizeManager::T6);
    QFontMetrics fontWidth_typeLabel(mlabelF);
    QString str_typelabel = fontWidth_typeLabel.elidedText(tr("Type:"), Qt::ElideRight, DDECalendar::NewSchceduleLabelWidth);
    m_typeLabel->setText(str_typelabel);
    m_typeLabel->setFont(mlabelF);
    m_typeLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_typeLabel->setFixedSize(78, 36);
    //“类型”combox
    m_typeComBox = new DComboBox();
    //设置对象名称和辅助显示名称
    m_typeComBox->setObjectName("ScheduleTypeCombobox");
    m_typeComBox->setAccessibleName("ScheduleTypeCombobox");
    m_typeComBox->setFixedSize(319, 36);
    m_typeComBox->setIconSize(QSize(24, 24));
    m_typeComBox->insertItem(0,
                             QIcon(DHiDPIHelper::loadNxPixmap(":/resources/icon/icon_type_work.svg")
                                   .scaled(QSize(24, 24) * devicePixelRatioF())),
                             tr("Work"));
    m_typeComBox->insertItem(1,
                             QIcon(DHiDPIHelper::loadNxPixmap(":/resources/icon/icon_type_life.svg")
                                   .scaled(QSize(24, 24) * devicePixelRatioF())),
                             tr("Life"));
    m_typeComBox->insertItem(
        2,
        QIcon(DHiDPIHelper::loadNxPixmap(":/resources/icon/icon_type_other.svg")
              .scaled(QSize(24, 24) * devicePixelRatioF())),
        tr("Other"));
    //将“类型”label添加到“类型”布局
    typelayout->addWidget(m_typeLabel);
    //将“类型”combox添加到“类型”布局
    typelayout->addWidget(m_typeComBox);
    //将“类型”布局添加到main布局
    maintlayout->addLayout(typelayout);
    //“内容”布局
    QHBoxLayout *contentLabellayout = new QHBoxLayout;
    contentLabellayout->setSpacing(0);
    contentLabellayout->setMargin(0);
    //“内容”label布局
    QVBoxLayout *conttelabellayout = new QVBoxLayout;
    conttelabellayout->setSpacing(0);
    conttelabellayout->setMargin(0);
    //“内容”label
    m_contentLabel = new QLabel();
    DFontSizeManager::instance()->bind(m_contentLabel, DFontSizeManager::T6);
    QFontMetrics fontWidth_contentlabel(mlabelF);
    QString str_contentlabel = fontWidth_contentlabel.elidedText(tr("Description:"), Qt::ElideRight, DDECalendar::NewSchceduleLabelWidth);
    m_contentLabel->setText(str_contentlabel);
    m_contentLabel->setFont(mlabelF);
    m_contentLabel->setToolTip(tr("Description"));
    m_contentLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_contentLabel->setFixedWidth(78);
    //将“内容”label添加到"内容“label布局中
    conttelabellayout->addWidget(m_contentLabel);
    conttelabellayout->addStretch();
    //new textedit
    m_textEdit = new DTextEdit(this);
    //设置对象名称和辅助显示名称
    m_textEdit->setObjectName("ScheduleTitleEdit");
    m_textEdit->setAccessibleName("ScheduleTitleEdit");
    m_textEdit->setFixedSize(319, 86);
    m_textEdit->setAcceptRichText(false);
    //设置占位符内容
    m_textEdit->setPlaceholderText(tr("New Event"));
    //设置关联控件，用于QTextEdit控件捕获MouseButtonPress等事件
    QWidget *mpContentWidget = m_textEdit->viewport();
    //设置事件过滤器
    m_textEdit->installEventFilter(this);
    //设置过滤器
    mpContentWidget->installEventFilter(this);
    //将”内容“label布局添加到”内容“布局
    contentLabellayout->addLayout(conttelabellayout);
    //将textedit添加到”内容“布局
    contentLabellayout->addWidget(m_textEdit);
    //将“内容”布局添加到main布局
    maintlayout->addLayout(contentLabellayout);
    //“全天”布局
    QHBoxLayout *alldayLabellayout = new QHBoxLayout;
    alldayLabellayout->setSpacing(0);
    alldayLabellayout->setMargin(0);
    //“全天”label
    m_adllDayLabel = new QLabel();
    m_adllDayLabel->setToolTip(tr("All Day"));
    DFontSizeManager::instance()->bind(m_adllDayLabel, DFontSizeManager::T6);
    QFontMetrics fontWidth_allDayLabel(mlabelF);
    QString str_allDayLabel = fontWidth_allDayLabel.elidedText(tr("All Day:"), Qt::ElideRight, DDECalendar::NewSchceduleLabelWidth);
    m_adllDayLabel->setText(str_allDayLabel);
    m_adllDayLabel->setFont(mlabelF);
    m_adllDayLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_adllDayLabel->setFixedWidth(78);
    //”全天“checkbox
    m_allDayCheckbox = new DCheckBox(this);
    //设置对象名称和辅助显示名称
    m_allDayCheckbox->setObjectName("AllDayCheckBox");
    m_allDayCheckbox->setAccessibleName("AllDayCheckBox");
    //将”全天“label添加到”全天“布局中
    alldayLabellayout->addWidget(m_adllDayLabel);
    //将”全天“checkbox添加到”全天“布局中
    alldayLabellayout->addWidget(m_allDayCheckbox);
    //将“全天”添加到main布局中
    maintlayout->addLayout(alldayLabellayout);
    //“开始时间”布局
    QHBoxLayout *beginLabellayout = new QHBoxLayout;
    beginLabellayout->setSpacing(0);
    beginLabellayout->setMargin(0);
    //“开始时间”label
    m_beginTimeLabel = new QLabel();
    m_beginTimeLabel->setToolTip(tr("Starts"));
    DFontSizeManager::instance()->bind(m_beginTimeLabel, DFontSizeManager::T6);
    QFontMetrics fontWidth_beginTimeLabel(mlabelF);
    QString str_beginTimeLabel = fontWidth_beginTimeLabel.elidedText(tr("Starts:"), Qt::ElideRight, DDECalendar::NewSchceduleLabelWidth);
    m_beginTimeLabel->setText(str_beginTimeLabel);
    m_beginTimeLabel->setFont(mlabelF);
    m_beginTimeLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_beginTimeLabel->setFixedSize(78, 36);
    //“开始时间”dateedit
    m_beginDateEdit = new QDateEdit(this);
    //设置对象名称和辅助显示名称
    m_beginDateEdit->setObjectName("ScheduleBeginDateEdit");
    m_beginDateEdit->setAccessibleName("ScheduleBeginDateEdit");
    m_beginDateEdit->setFixedSize(175, 36);
    //“开始时间”timeedit
    m_beginTimeEdit = new CTimeEdit(this);
    //设置对象名称和辅助显示名称
    m_beginTimeEdit->setObjectName("ScheduleBeginTimeEdit");
    m_beginTimeEdit->setAccessibleName("ScheduleBeginTimeEdit");
    m_beginTimeEdit->setFixedSize(141, 36);
    m_beginDateEdit->setCalendarPopup(true);
    m_beginDateEdit->setDisplayFormat("yyyy-MM-dd");
    //将“开始时间”label添加到“开始时间”布局中
    beginLabellayout->addWidget(m_beginTimeLabel);
    //将“开始时间”dateedit添加到“开始时间”布局中
    beginLabellayout->addWidget(m_beginDateEdit);
    beginLabellayout->addSpacing(8);
    //将“开始时间”timeedit添加到“开始时间”布局中
    beginLabellayout->addWidget(m_beginTimeEdit);
    beginLabellayout->addStretch();
    //将“开始时间”布局添加到main布局中
    maintlayout->addLayout(beginLabellayout);
    //“结束时间”布局
    QHBoxLayout *enQLabellayout = new QHBoxLayout;
    enQLabellayout->setSpacing(0);
    enQLabellayout->setMargin(0);
    //“结束时间”label
    m_endTimeLabel = new QLabel();
    m_endTimeLabel->setToolTip(tr("Ends"));
    DFontSizeManager::instance()->bind(m_endTimeLabel, DFontSizeManager::T6);
    QFontMetrics fontWidth_endTimeLabel(mlabelF);
    QString str_endTimeLabel = fontWidth_endTimeLabel.elidedText(tr("Ends:"), Qt::ElideRight, DDECalendar::NewSchceduleLabelWidth);
    m_endTimeLabel->setText(str_endTimeLabel);
    m_endTimeLabel->setFont(mlabelF);
    m_endTimeLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_endTimeLabel->setFixedSize(78, 36);
    //“结束时间”dateedit
    m_endDateEdit = new QDateEdit(this);
    //设置对象名称和辅助显示名称
    m_endDateEdit->setObjectName("ScheduleEndDateEdit");
    m_endDateEdit->setAccessibleName("ScheduleEndDateEdit");
    m_endDateEdit->setFixedSize(175, 36);
    //“结束时间”timeedit
    m_endTimeEdit = new CTimeEdit(this);
    //设置对象名称和辅助显示名称
    m_endTimeEdit->setObjectName("ScheduleEndTimeEdit");
    m_endTimeEdit->setAccessibleName("ScheduleEndTimeEdit");
    m_endTimeEdit->setFixedSize(141, 36);
    m_endDateEdit->setCalendarPopup(true);
    m_endDateEdit->setDisplayFormat("yyyy-MM-dd");
    //将“结束时间”label添加到“结束时间”布局中
    enQLabellayout->addWidget(m_endTimeLabel);
    //将“结束时间”dateedit添加到“结束时间”布局中
    enQLabellayout->addWidget(m_endDateEdit);
    enQLabellayout->addSpacing(8);
    //将“结束时间”timeedit添加到“结束时间”布局中
    enQLabellayout->addWidget(m_endTimeEdit);
    enQLabellayout->addStretch();
    //将“结束时间”布局添加到main布局中
    maintlayout->addLayout(enQLabellayout);
    //“提醒”布局
    QHBoxLayout *rminQLabellayout = new QHBoxLayout;
    rminQLabellayout->setSpacing(0);
    rminQLabellayout->setMargin(0);
    //“提醒”label
    m_remindSetLabel = new QLabel();
    DFontSizeManager::instance()->bind(m_remindSetLabel, DFontSizeManager::T6);
    QFontMetrics fontWidth_remindSetLabel(mlabelF);
    QString str_remindSetLabel = fontWidth_remindSetLabel.elidedText(tr("Remind Me:"), Qt::ElideRight, DDECalendar::NewSchceduleLabelWidth);
    m_remindSetLabel->setToolTip(tr("Remind Me"));
    m_remindSetLabel->setText(str_remindSetLabel);
    m_remindSetLabel->setFont(mlabelF);
    m_remindSetLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_remindSetLabel->setFixedWidth(78);
    //“提醒”combox
    m_rmindCombox = new DComboBox();
    //设置对象名称和辅助显示名称
    m_rmindCombox->setObjectName("RmindComboBox");
    m_rmindCombox->setAccessibleName("RmindComboBox");
    m_rmindCombox->setFixedSize(175, 36);
    m_rmindCombox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    //将“提醒”label添加到“提醒”布局
    rminQLabellayout->addWidget(m_remindSetLabel);
    //将“提醒”combox添加到“提醒”布局
    rminQLabellayout->addWidget(m_rmindCombox);
    rminQLabellayout->addStretch();
    //将“提醒”布局添加到main布局中
    maintlayout->addLayout(rminQLabellayout);
    //“重复”布局
    QHBoxLayout *repeatLabellayout = new QHBoxLayout;
    repeatLabellayout->setSpacing(0);
    repeatLabellayout->setMargin(0);
    //“重复”label
    m_beginrepeatLabel = new QLabel();
    m_beginrepeatLabel->setToolTip(tr("Repeat"));
    DFontSizeManager::instance()->bind(m_beginrepeatLabel, DFontSizeManager::T6);
    QFontMetrics fontWidth_beginRepeatLabel(mlabelF);
    QString str_beginRepeatLabel = fontWidth_beginRepeatLabel.elidedText(tr("Repeat:"), Qt::ElideRight, DDECalendar::NewSchceduleLabelWidth);
    m_beginrepeatLabel->setText(str_beginRepeatLabel);
    m_beginrepeatLabel->setFont(mlabelF);
    m_beginrepeatLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_beginrepeatLabel->setFixedWidth(78);
    //“重复”combox
    m_beginrepeatCombox = new DComboBox();
    //设置对象名称和辅助显示名称
    m_beginrepeatCombox->setObjectName("BeginRepeatComboBox");
    m_beginrepeatCombox->setAccessibleName("BeginRepeatComboBox");
    m_beginrepeatCombox->setFixedSize(175, 36);
    m_beginrepeatCombox->addItem(tr("Never"));
    m_beginrepeatCombox->addItem(tr("Daily"));
    m_beginrepeatCombox->addItem(tr("Weekdays"));
    m_beginrepeatCombox->addItem(tr("Weekly"));
    m_beginrepeatCombox->addItem(tr("Monthly"));
    m_beginrepeatCombox->addItem(tr("Yearly"));
    repeatLabellayout->addWidget(m_beginrepeatLabel);
    repeatLabellayout->addWidget(m_beginrepeatCombox);
    repeatLabellayout->addStretch();
    maintlayout->addLayout(repeatLabellayout);
    //“结束重复”布局
    QHBoxLayout *endrepeatLabellayout = new QHBoxLayout;
    endrepeatLabellayout->setSpacing(0);
    endrepeatLabellayout->setMargin(0);
    //“结束重复”label
    m_endrepeatLabel = new QLabel();
    DFontSizeManager::instance()->bind(m_endrepeatLabel, DFontSizeManager::T6);
    QFontMetrics fontWidth_endrepeatLabel(mlabelF);
    QString str_endrepeatLabel = fontWidth_endrepeatLabel.elidedText(tr("End Repeat:"), Qt::ElideRight, DDECalendar::NewSchceduleLabelWidth);
    m_endrepeatLabel->setToolTip(tr("End Repeat"));
    m_endrepeatLabel->setText(str_endrepeatLabel);
    m_endrepeatLabel->setFont(mlabelF);
    m_endrepeatLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_endrepeatLabel->setFixedWidth(78);
    //“结束重复”combox
    m_endrepeatCombox = new DComboBox();
    //设置对象名称和辅助显示名称
    m_endrepeatCombox->setObjectName("EndRepeatComboBox");
    m_endrepeatCombox->setAccessibleName("EndRepeatComboBox");
    m_endrepeatCombox->setFixedSize(175, 36);
    m_endrepeatCombox->addItem(tr("Never"));
    m_endrepeatCombox->addItem(tr("After"));
    m_endrepeatCombox->addItem(tr("On"));
    endrepeatLabellayout->addWidget(m_endrepeatLabel);
    endrepeatLabellayout->addWidget(m_endrepeatCombox);
    endrepeatLabellayout->addSpacing(8);
    //结束于次数布局
    QHBoxLayout *endrepeattimeslayout = new QHBoxLayout;
    endrepeattimeslayout->setSpacing(0);
    endrepeattimeslayout->setMargin(0);
    endrepeattimeslayout->setContentsMargins(10, 0, 0, 0);
    //结束于次数lineedit
    m_endrepeattimes = new DLineEdit(this);
    //设置对象名称和辅助显示名称
    m_endrepeattimes->setObjectName("EndRepeatTimeEidt");
    m_endrepeattimes->setAccessibleName("EndRepeatTimeEidt");
    m_endrepeattimes->setFixedSize(71, 36);
    m_endrepeattimes->setText(QString::number(10));
    m_endrepeattimes->setClearButtonEnabled(false);
    QRegExp rx("^[1-9]\\d{0,2}$");
    QValidator *validator = new QRegExpValidator(rx, this);
    m_endrepeattimes->lineEdit()->setValidator(validator);
    //“次数”label
    m_endrepeattimesLabel = new QLabel(tr("time(s)"));
    m_endrepeattimesLabel->setFont(mlabelF);
    m_endrepeattimesLabel->setFixedHeight(36);
    //将结束于次数lineedit添加到结束于次数布局
    endrepeattimeslayout->addWidget(m_endrepeattimes);
    //将“次数”label添加到结束于次数布局
    endrepeattimeslayout->addWidget(m_endrepeattimesLabel);
    //结束于次数widget
    m_endrepeattimesWidget = new DWidget;
    //设置对象名称和辅助显示名称
    m_endrepeattimesWidget->setObjectName("EndRepeatTimeWidget");
    m_endrepeattimesWidget->setAccessibleName("EndRepeatTimeWidget");
    m_endrepeattimesWidget->setLayout(endrepeattimeslayout);
    m_endrepeattimesWidget->setVisible(false);
    //将结束于次数widget添加到“结束重复”布局
    endrepeatLabellayout->addWidget(m_endrepeattimesWidget);
    //结束重复于日期
    m_endRepeatDate = new DDateEdit;
    //设置对象名称和辅助显示名称
    m_endRepeatDate->setObjectName("EndRepeatDateEdit");
    m_endRepeatDate->setAccessibleName("EndRepeatDateEdit");
    m_endRepeatDate->setCalendarPopup(true);
    m_endRepeatDate->setFixedSize(141, 36);
    m_endRepeatDate->setDate(QDate::currentDate());
    m_endRepeatDate->setDisplayFormat("yyyy-MM-dd");
    m_endRepeatDate->setCurrentSectionIndex(2);
    QFont enddatefont;
    enddatefont.setWeight(QFont::Medium);
    m_endRepeatDate->setFont(enddatefont);
    //将结束重复于日期添加到“结束重复”布局
    endrepeatLabellayout->addWidget(m_endRepeatDate);
    endrepeatLabellayout->addStretch();
    m_endRepeatDate->setVisible(false);
    //“结束重复”widget
    m_endrepeatWidget = new DWidget;
    //设置对象名称和辅助显示名称
    m_endrepeatWidget->setObjectName("EndRepeatDateWidget");
    m_endrepeatWidget->setAccessibleName("EndRepeatDateWidget");
    //将“结束重复”布局添加到“结束重复”widget
    m_endrepeatWidget->setLayout(endrepeatLabellayout);
    //将“结束重复”widget添加到main布局
    maintlayout->addWidget(m_endrepeatWidget);
    m_endrepeatWidget->setVisible(false);
    //添加button
    addButton(tr("Cancel"));
    addButton(tr("Save"), false, DDialog::ButtonRecommend);
    //设置button大小
    for (int i = 0; i < buttonCount(); i++) {
        QAbstractButton *button = getButton(i);
        button->setFixedSize(189, 36);
    }
    m_gwi = new DFrame(this);
    m_gwi->setFrameShape(QFrame::NoFrame);
    m_gwi->setLayout(maintlayout);
    DPalette anipa = m_gwi->palette();
    QColor color = "#F8F8F8";
    color.setAlphaF(0.0);
    anipa.setColor(DPalette::Background, color);
    addContent(m_gwi, Qt::AlignCenter);
    //初始化dateedit
    initDateEdit();

    if (m_type == 1)
        slotallDayStateChanged(0);
    setFocus();
}
/**
 * @brief CSchceduleDlg::initConnection 初始化信号连接
 */
void CSchceduleDlg::initConnection()
{
    QObject::connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
                     this,
                     &CSchceduleDlg::setTheMe);
    connect(this, &DDialog::buttonClicked, this, &CSchceduleDlg::slotCancelBt);
    connect(this, &DDialog::buttonClicked, this, &CSchceduleDlg::slotOkBt);
    connect(m_textEdit, &DTextEdit::textChanged, this, &CSchceduleDlg::slotTextChange);
    connect(m_endrepeattimes, &DLineEdit::textChanged, this, &CSchceduleDlg::slotendrepeatTextchange);

    connect(m_allDayCheckbox, &DCheckBox::stateChanged, this,
            &CSchceduleDlg::slotallDayStateChanged);
    connect(m_beginrepeatCombox, QOverload<int>::of(&QComboBox::activated), this,
            &CSchceduleDlg::slotbRpeatactivated);
    connect(m_endrepeatCombox, QOverload<int>::of(&QComboBox::activated), this,
            &CSchceduleDlg::sloteRpeatactivated);
    connect(m_beginDateEdit, &DDateEdit::userDateChanged, this, &CSchceduleDlg::slotBDateEidtInfo);
    //快捷键
    QShortcut *shortcut = new QShortcut(this);
    shortcut->setKey(QKeySequence(QLatin1String("ESC")));
    connect(shortcut, SIGNAL(activated()), this, SLOT(close()));
}
/**
 * @brief CSchceduleDlg::initDateEdit 初始化dateedit
 */
void CSchceduleDlg::initDateEdit()
{
    //设置dateedit最早和最晚时间
    m_beginDateEdit->setMinimumDate(QDate(DDECalendar::QueryEarliestYear, 1, 1)); // 0天
    m_beginDateEdit->setMaximumDate(QDate(DDECalendar::QueryLatestYear, 12, 31));
    m_endDateEdit->setMinimumDate(QDate(DDECalendar::QueryEarliestYear, 1, 1)); // 0天
    m_endDateEdit->setMaximumDate(QDate(DDECalendar::QueryLatestYear, 12, 31));
    return;
}
/**
 * @brief CSchceduleDlg::initRmindRpeatUI 初始化提醒界面
 */
void CSchceduleDlg::initRmindRpeatUI()
{
    if (m_scheduleDtailInfo.allday) {
        //全天
        if (m_scheduleDtailInfo.remind) {
            //有提醒
            if (m_scheduleDtailInfo.remindData.n == DDECalendar::OnStartDay) {
                m_rmindCombox->setCurrentIndex(1);
            } else if (m_scheduleDtailInfo.remindData.n == DDECalendar::OneDayBeforeWithDay) {
                m_rmindCombox->setCurrentIndex(2);
            } else if (m_scheduleDtailInfo.remindData.n == DDECalendar::TwoDayBeforeWithDay) {
                m_rmindCombox->setCurrentIndex(3);
            } else if (m_scheduleDtailInfo.remindData.n == DDECalendar::OneWeekBeforeWithDay) {
                m_rmindCombox->setCurrentIndex(4);
            }
        } else {
            //没有提醒
            m_rmindCombox->setCurrentIndex(0);
        }
    } else {
        //非全天
        if (m_scheduleDtailInfo.remind) {
            //有提醒
            if (m_scheduleDtailInfo.remindData.n == DDECalendar::AtTimeOfEvent) {
                m_rmindCombox->setCurrentIndex(1);
            } else if (m_scheduleDtailInfo.remindData.n == DDECalendar::FifteenMinutesBefore) {
                m_rmindCombox->setCurrentIndex(2);
            } else if (m_scheduleDtailInfo.remindData.n == DDECalendar::ThirtyMinutesBefore) {
                m_rmindCombox->setCurrentIndex(3);
            } else if (m_scheduleDtailInfo.remindData.n == DDECalendar::OneHourBefore) {
                m_rmindCombox->setCurrentIndex(4);
            } else if (m_scheduleDtailInfo.remindData.n == DDECalendar::OneDayBeforeWithMinutes) {
                m_rmindCombox->setCurrentIndex(5);
            } else if (m_scheduleDtailInfo.remindData.n == DDECalendar::TwoDayBeforeWithMinutes) {
                m_rmindCombox->setCurrentIndex(6);
            } else if (m_scheduleDtailInfo.remindData.n == DDECalendar::OneWeekBeforeWithMinutes) {
                m_rmindCombox->setCurrentIndex(7);
            }
        } else {
            //没有提醒
            m_rmindCombox->setCurrentIndex(0);
        }
    }
    //设置dialog大小
    slotbRpeatactivated(m_scheduleDtailInfo.rpeat);
    //设置重复类型
    m_beginrepeatCombox->setCurrentIndex(m_scheduleDtailInfo.rpeat);
    //是否显示结束重复
    if (m_scheduleDtailInfo.rpeat != 0) {
        //显示结束重复
        if (m_scheduleDtailInfo.enddata.type == 0) {
            //永不
            m_endrepeatCombox->setCurrentIndex(0);
        } else if (m_scheduleDtailInfo.enddata.type == 1) {
            //结束重复于次数
            m_endrepeatCombox->setCurrentIndex(1);
            m_endrepeattimes->setText(QString::number(m_scheduleDtailInfo.enddata.tcount));
        } else if (m_scheduleDtailInfo.enddata.type == 2) {
            //结束重复于日期
            m_endrepeatCombox->setCurrentIndex(2);
            m_endRepeatDate->setDate(m_scheduleDtailInfo.enddata.date.date());
        }
        m_endrepeatWidget->show();
        sloteRpeatactivated(m_scheduleDtailInfo.enddata.type);
    } else {
        //结束重复隐藏
        m_endrepeatWidget->hide();
    }
}

/**
 * @brief CSchceduleDlg::setTheMe 设置系统主题类型
 * @param type 系统主题类型
 */
void CSchceduleDlg::setTheMe(const int type)
{
    //日程标题编辑框文字颜色
    QColor titleColor;
    if (type == 2) {
        titleColor = "#C0C6D4";
    } else {
        titleColor = "#414D68";
    }
    DPalette pa = m_textEdit->palette();
    //设置颜色
    pa.setColor(DPalette::Text, titleColor);
    m_textEdit->setPalette(pa);
}

/**
 * @brief CSchceduleDlg::ChangeRecurInfo 修改重复日程
 * @param parent 父类
 * @param newinfo new日程信息
 * @param oldinfo old日程信息
 * @param m_themetype 系统主题类型
 */
void CSchceduleDlg::ChangeRecurInfo(QWidget *parent, const ScheduleDtailInfo &newinfo, const ScheduleDtailInfo &oldinfo, int m_themetype)
{
    Q_UNUSED(m_themetype);
    Q_UNUSED(parent);
    //修改的重复日程是否为第一个
    if (newinfo.RecurID == 0) {
        //修改第一个重复日程
        CSchceduleCtrlDlg msgBox;
        //设置提示语
        msgBox.setText(tr("You are changing a repeating event."));
        msgBox.setInformativeText(
            tr("Do you want to change only this occurrence of the event, or all "
               "occurrences?"));
        //添加按钮设置文字
        msgBox.addPushButton(tr("Cancel"));
        msgBox.addPushButton(tr("All"));
        msgBox.addsuggestButton(tr("Only This Event"));
        msgBox.exec();
        //按钮对应功能
        if (msgBox.clickButton() == 0) {
            //取消
            return;
        } else if (msgBox.clickButton() == 1) {
            //修改所有
            ScheduleDtailInfo scheduleDtailInfo = newinfo;
            //重复类型设置为“永不”的情况
            if (scheduleDtailInfo.enddata.type == 1 && scheduleDtailInfo.enddata.tcount < 1) {
                scheduleDtailInfo.enddata.type = 0;
            } else if (scheduleDtailInfo.enddata.type == 2 &&
                       scheduleDtailInfo.beginDateTime.daysTo(
                           scheduleDtailInfo.enddata.date) < 0) {
                scheduleDtailInfo.enddata.type = 0;
                scheduleDtailInfo.rpeat = 0;
            }
            //更新日程
            CScheduleDataManage::getScheduleDataManage()
            ->getscheduleDataCtrl()
            ->updateScheduleInfo(scheduleDtailInfo);
        } else if (msgBox.clickButton() == 2) {
            //仅修改当前日程
            ChangeOnlyInfo(newinfo, oldinfo);
        }
    } else {
        //修改非第一个重复日程
        CSchceduleCtrlDlg msgBox;
        //设置提示信息
        msgBox.setText(tr("You are changing a repeating event."));
        msgBox.setInformativeText(
            tr("Do you want to change only this occurrence of the event, or this and "
               "all future occurrences?"));
        //设置按钮文字
        msgBox.addPushButton(tr("Cancel"));
        msgBox.addPushButton(tr("All Future Events"));
        msgBox.addsuggestButton(tr("Only This Event"));
        msgBox.exec();
        //按钮对应功能
        if (msgBox.clickButton() == 0) {
            //取消
            return;
        } else if (msgBox.clickButton() == 1) {
            //修改将来所有
            ScheduleDtailInfo newschedule = newinfo;
            //设置此日程为重复日程的第一个
            newschedule.RecurID = 0;
            newschedule.id = 0;
            if (newschedule.enddata.type == 1) {
                //重复日程重复的次数
                newschedule.enddata.tcount = qAbs(newinfo.enddata.tcount - newinfo.RecurID);
                if (newschedule.enddata.tcount < 1) {
                    //如果次数小于1,则设置为永不
                    newschedule.enddata.type = 0;
                    newschedule.rpeat = 0;
                }
            }
            //添加日程
            CScheduleDataManage::getScheduleDataManage()
            ->getscheduleDataCtrl()
            ->addSchedule(newschedule);
            ScheduleDtailInfo updatescheduleData;
            //根据id获取点击日程
            CScheduleDataManage::getScheduleDataManage()
            ->getscheduleDataCtrl()
            ->getScheduleInfoById(oldinfo.id, updatescheduleData);
            if (updatescheduleData.enddata.type == 1) {
                //重复日程重复的次数
                updatescheduleData.enddata.tcount = newinfo.RecurID - 1;
                if (updatescheduleData.enddata.tcount < 1) {
                    //如果重复的次数小于1,则设置为永不
                    updatescheduleData.enddata.type = 0;
                    updatescheduleData.rpeat = 0;
                }
            } else {
                //如果结束类型为永不或结束于日期
                updatescheduleData.enddata.type = 2;
                updatescheduleData.enddata.date =
                    oldinfo.beginDateTime.addDays(-1);
            }
            //更新日程
            CScheduleDataManage::getScheduleDataManage()
            ->getscheduleDataCtrl()
            ->updateScheduleInfo(updatescheduleData);
        } else if (msgBox.clickButton() == 2) {
            //仅修改当前日程
            ChangeOnlyInfo(newinfo, oldinfo);
        }
    }
}
/**
 * @brief CSchceduleDlg::ChangeOnlyInfo 仅修改当期日程信息
 * @param newinfo 新的日程信息
 * @param oldinfo 旧的日程信息
 */
void CSchceduleDlg::ChangeOnlyInfo(const ScheduleDtailInfo &newinfo, const ScheduleDtailInfo &oldinfo)
{
    ScheduleDtailInfo newschedule = newinfo;
    //不设置重复
    newschedule.rpeat = 0;
    //设置为第一个日程
    newschedule.RecurID = 0;
    newschedule.id = 0;
    newschedule.ignore.clear();
    //新建日程
    CScheduleDataManage::getScheduleDataManage()
    ->getscheduleDataCtrl()
    ->addSchedule(newschedule);
    ScheduleDtailInfo updatescheduleData;
    //根据id，获取日程
    CScheduleDataManage::getScheduleDataManage()
    ->getscheduleDataCtrl()
    ->getScheduleInfoById(oldinfo.id, updatescheduleData);
    //忽略当前的日程
    updatescheduleData.ignore.append(oldinfo.beginDateTime);
    //更新日程
    CScheduleDataManage::getScheduleDataManage()
    ->getscheduleDataCtrl()
    ->updateScheduleInfo(updatescheduleData);
}
