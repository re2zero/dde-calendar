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
#include "yearscheduleview.h"

#include "scheduledlg.h"
#include "scheduledatamanage.h"
#include "constants.h"

#include <QPainter>
#include <QRect>
#include <QMouseEvent>

DGUI_USE_NAMESPACE

CYearScheduleView::CYearScheduleView(QWidget *parent)
    : DWidget(parent)
{
    m_textfont.setWeight(QFont::Medium);
    m_textfont.setPixelSize(DDECalendar::FontSizeTwelve);
}

CYearScheduleView::~CYearScheduleView()
{
}

bool YScheduleDateThan(const ScheduleDataInfo &s1, const ScheduleDataInfo &s2)
{
    QDate bdate1 = s1.getBeginDateTime().date();
    QDate edate1 = s1.getEndDateTime().date();
    QDate bdate2 = s2.getBeginDateTime().date();
    QDate edate2 = s2.getEndDateTime().date();

    if (bdate1 != edate1 && bdate2 == edate2) {
        return true;
    } else if (bdate1 == edate1 && bdate2 != edate2) {
        return false;
    } else if (bdate1 != edate1 && bdate2 != edate2) {
        return bdate1 < bdate2;
    } else {
        if (s1.getBeginDateTime() == s2.getBeginDateTime()) {
            return s1.getTitleName() < s2.getTitleName();
        } else {
            return s1.getBeginDateTime() < s2.getBeginDateTime();
        }
    }
}
bool YScheduleDaysThan(const ScheduleDataInfo &s1, const ScheduleDataInfo &s2)
{
    return s1.getBeginDateTime().date().daysTo(s1.getEndDateTime().date()) > s2.getBeginDateTime().date().daysTo(s2.getEndDateTime().date());
}

void CYearScheduleView::setData(QVector<ScheduleDataInfo> &vListData)
{
    QVector<ScheduleDataInfo> valldayListData, vDaylistdata;

    for (int i = 0; i < vListData.count(); i++) {
        if (vListData.at(i).getAllDay()) {
            valldayListData.append(vListData.at(i));
        } else {
            vDaylistdata.append(vListData.at(i));
        }
    }

    std::sort(valldayListData.begin(), valldayListData.end(), YScheduleDaysThan);
    std::sort(valldayListData.begin(), valldayListData.end(), YScheduleDateThan);
    std::sort(vDaylistdata.begin(), vDaylistdata.end(), YScheduleDaysThan);
    std::sort(vDaylistdata.begin(), vDaylistdata.end(), YScheduleDateThan);

    for (int i = 0; i < valldayListData.count(); i++) {
        QVector<ScheduleDataInfo>::iterator iter = valldayListData.begin();
        if (valldayListData.at(i).getType() == DDECalendar::FestivalTypeID) {
            ScheduleDataInfo moveDate;
            moveDate = valldayListData.at(i);
            valldayListData.removeAt(i);
            valldayListData.insert(iter, moveDate);
        }
    }

    m_vlistData.clear();
    m_vlistData.append(valldayListData);
    m_vlistData.append(vDaylistdata);

    if (m_vlistData.size() > DDEYearCalendar::YearScheduleListMaxcount) {
        QVector<ScheduleDataInfo> vTlistData;
        for (int i = 0; i < 4; i++) {
            if (m_vlistData.at(i).getBeginDateTime().date() != m_vlistData.at(i).getEndDateTime().date() && !m_vlistData.at(i).getAllDay()) {
                if (m_vlistData.at(i).getBeginDateTime().date() != m_currentDate) {
                    m_vlistData[i].setAllDay(true);
                }
            }
            vTlistData.append(m_vlistData.at(i));
        }
        ScheduleDataInfo info;
        info.setTitleName("......");
        info.setID(-1);
        vTlistData.append(info);
        m_vlistData = vTlistData;
    }
}

void CYearScheduleView::clearData()
{
    m_vlistData.clear();
    return;
}

void CYearScheduleView::showWindow()
{
    if (m_vlistData.isEmpty()) {
        setFixedSize(130, 45);
    } else {
        updateDateShow();
    }
}

void CYearScheduleView::setTheMe(int type)
{
    if (type == 0 || type == 1) {
        m_btimecolor = "#414D68";
        m_btimecolor.setAlphaF(0.7);
        m_bttextcolor = "#414D68";
    } else if (type == 2) {
        m_btimecolor = "#C0C6D4";
        m_btimecolor.setAlphaF(0.7);
        m_bttextcolor = "#C0C6D4";
    }
}

void CYearScheduleView::setCurrentDate(QDate cdate)
{
    m_currentDate = cdate;
}

QDate CYearScheduleView::getCurrentDate()
{
    return m_currentDate;
}

void CYearScheduleView::setTimeFormat(QString format)
{
    m_timeFormat = format;
    update();
}

void CYearScheduleView::updateDateShow()
{
    int sviewNum = 0;
    if (!m_vlistData.isEmpty()) {
        if (m_vlistData.size() > DDEYearCalendar::YearScheduleListMaxcount) {
            sviewNum = DDEYearCalendar::YearScheduleListMaxcount;
        } else {
            sviewNum = m_vlistData.size();
        }
    }
    if (!m_vlistData.isEmpty())
        setFixedSize(240, 45 + (sviewNum - 1) * 29);
    update();

    return;
}

void CYearScheduleView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 反锯齿;
    if (m_vlistData.isEmpty()) {
        paintItem(painter);
    } else {
        for (int i = 0; i < m_vlistData.size(); ++i) {
            paintItem(painter, m_vlistData.at(i), i);
        }
    }
}

void CYearScheduleView::paintItem(QPainter &painter, ScheduleDataInfo info, int index)
{
    int labelwidth = width() - 30;
    int bheight = index * 29 + 10;
    int labelheight = 28;
    ScheduleDataInfo &gd = info;
    CSchedulesColor gdcolor = CScheduleDataManage::getScheduleDataManage()->getScheduleColorByType(gd.getType());



    if (gd.getID() == -1) {
        QString str = "...";

        painter.save();
        painter.setPen(m_btimecolor);
        painter.setFont(m_textfont);
        painter.drawText(QRect(25, bheight, labelwidth - 80, labelheight - 2), Qt::AlignLeft | Qt::AlignVCenter, str);
        painter.restore();
    } else {
        if (info.getID() != -1) {
            //圆点m_solocolor
            painter.save();
            painter.setBrush(QBrush(gdcolor.splitColor));
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(QRect(25, bheight + (labelheight - 8) / 2, 8, 8));
            painter.restore();
        }

        QString str;
        //左边文字
        painter.save();
        painter.setPen(m_bttextcolor);
        painter.setFont(m_textfont);
        QFontMetrics fm = painter.fontMetrics();
        QString tStitlename = gd.getTitleName();
        tStitlename.replace("\n", "");
        str = tStitlename;
        int tilenameW = labelwidth - 80;
        QString tstr;

        for (int i = 0; i < str.count(); i++) {
            tstr.append(str.at(i));
            int widthT = fm.width(tstr) + 5;
            if (widthT >= tilenameW) {
                tstr.chop(1);
                break;
            }
        }
        if (tstr != str) {
            tstr = tstr + "...";
        }
        painter.drawText(QRect(41, bheight, tilenameW, labelheight - 2), Qt::AlignLeft | Qt::AlignVCenter, tstr);

        painter.restore();

        if (info.getID() != -1) {
            //右边时间
            painter.save();
            painter.setPen(m_btimecolor);
            painter.setFont(m_textfont);
            if (info.getAllDay()) {
                str = tr("All Day");
            } else {
                if (m_currentDate > info.getBeginDateTime().date()) {
                    str = tr("All Day");
                } else {
                    str = info.getBeginDateTime().time().toString(m_timeFormat);
                }
            }
            painter.drawText(QRect(width() - 70, bheight, 57, labelheight - 2), Qt::AlignRight | Qt::AlignVCenter, str);
            painter.restore();
        }
    }
}

void CYearScheduleView::paintItem(QPainter &painter)
{
    //左边文字
    painter.save();
    painter.setPen(m_bttextcolor);
    painter.setFont(m_textfont);
    QString tStitlename = tr("No event");
    painter.drawText(QRect(0, 0, width(), height()), Qt::AlignCenter, tStitlename);
    painter.restore();
}

CYearScheduleOutView::CYearScheduleOutView(QWidget *parent)
    : DArrowRectangle(DArrowRectangle::ArrowLeft, parent)
{
    //如果dtk版本为5.3以上则使用新接口
#if (DTK_VERSION > DTK_VERSION_CHECK(5, 3, 0, 0))
    //设置显示圆角
    setRadiusArrowStyleEnable(true);
    //设置圆角
    setRadius(DARROWRECT::DRADIUS);
#endif
    yearscheduleview = new CYearScheduleView();
    this->setContent(yearscheduleview);
}

void CYearScheduleOutView::setData(QVector<ScheduleDataInfo> &vListData)
{
    list_count = vListData.size();
    yearscheduleview->setData(vListData);
    yearscheduleview->showWindow();
    scheduleinfoList = yearscheduleview->getlistdate();
}

void CYearScheduleOutView::clearData()
{
    yearscheduleview->clearData();
}

void CYearScheduleOutView::setTheMe(int type)
{
    yearscheduleview->setTheMe(type);
    //根据主题设备不一样的背景色
    if (type == 2) {
        setBackgroundColor(DBlurEffectWidget::DarkColor);
    } else {
        setBackgroundColor(DBlurEffectWidget::LightColor);
    }
}

void CYearScheduleOutView::setCurrentDate(QDate cdate)
{
    currentdate = cdate;
    yearscheduleview->setCurrentDate(cdate);
}

/**
 * @brief SchecduleRemindWidget::setDirection       设置箭头方向
 * @param value
 */
void CYearScheduleOutView::setDirection(DArrowRectangle::ArrowDirection value)
{
    //设置箭头方向
    this->setArrowDirection(value);
    //设置内容窗口
    this->setContent(yearscheduleview);
}

void CYearScheduleOutView::setTimeFormat(QString format)
{
    yearscheduleview->setTimeFormat(format);
}

void CYearScheduleOutView::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    QPoint pos = QCursor::pos();
    pos = this->mapFromGlobal(pos);
    QVector<QRect> rect_press;
    int listShow = 0;

    if (!scheduleinfoList.isEmpty()) {
        if (scheduleinfoList.size() < DDEYearCalendar::YearScheduleListMaxcount)
            listShow = scheduleinfoList.size();
        else
            listShow = DDEYearCalendar::YearScheduleListMaxcount;
    }
    for (int i = 0; i < listShow; i++) {
        if (this->arrowDirection() == DArrowRectangle::ArrowLeft)
            rect_press.append(QRect(35, 20 + i * 30, width() - 50, 20));
        else
            rect_press.append(QRect(20, 20 + i * 30, width() - 50, 20));
    }
    for (int i = 0; i < listShow; i++) {
        if (rect_press.at(i).contains(pos)) {
            if (i > 3 && list_count > DDEYearCalendar::YearScheduleListMaxcount) {
                emit signalsViewSelectDate(currentdate);
                this->hide();
                //跳转到周视图
            } else {
                //如果日程类型不为节假日或纪念日则显示编辑框
                if (scheduleinfoList.at(i).getType() != DDECalendar::FestivalTypeID) {
                    emit signalViewtransparentFrame(1);
                    //因为提示框会消失，所以设置CScheduleDlg的父类为主窗口
                    CScheduleDlg dlg(0, qobject_cast<QWidget *>(this->parent()));
                    dlg.setData(scheduleinfoList.at(i));
                    dlg.exec();
                    emit signalViewtransparentFrame(0);
                }
            }
        }
    }
}
