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
#include "schcedulesearchview.h"
#include "schcedulectrldlg.h"
#include "myschceduleview.h"
#include "schceduledlg.h"
#include "scheduledatamanage.h"
#include "constants.h"

#include <DMessageBox>
#include <DPushButton>
#include <DHiDPIHelper>
#include <DPalette>
#include <DGuiApplicationHelper>

#include <QAction>
#include <QMenu>
#include <QListWidget>
#include <QLabel>
#include <QPainter>
#include <QHBoxLayout>
#include <QStylePainter>
#include <QRect>
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QPainterPath>

DGUI_USE_NAMESPACE
/**
 * @brief CSchceduleSearchItem::CSchceduleSearchItem 构造函数
 * @param parent 父类
 */
CSchceduleSearchItem::CSchceduleSearchItem(QWidget *parent)
    : DLabel(parent)
    , m_rightMenu(new DMenu(this))
{
    //设置对象名称和辅助显示名称
    this->setObjectName("CScheduleDataItem");
    this->setAccessibleName("CScheduleDataItem");
    //新建“编辑”action
    m_editAction = new QAction(tr("Edit"), this);
    //新建“删除”antion
    m_deleteAction = new QAction(tr("Delete"), this);
    //将action的信号和槽进行连接
    connect(m_editAction, SIGNAL(triggered(bool)), this, SLOT(slotEdit()));
    connect(m_deleteAction, SIGNAL(triggered(bool)), this, SLOT(slotDelete()));
    //设置系统主题
    setTheMe(DGuiApplicationHelper::instance()->themeType());
    QObject::connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::paletteTypeChanged,
                     this,
                     &CSchceduleSearchItem::setTheMe);
    m_mouseStatus = M_NONE;
    //设置过滤器
    installEventFilter(this);
}
/**
 * @brief CSchceduleSearchItem::setBackgroundColor 设置背景色
 * @param color1 颜色
 */
void CSchceduleSearchItem::setBackgroundColor(QColor color1)
{
    m_Backgroundcolor = color1;
}
/**
 * @brief CSchceduleSearchItem::setSplitLineColor 设置搜索日程标签中间线的颜色
 * @param color1 颜色
 */
void CSchceduleSearchItem::setSplitLineColor(QColor color1)
{
    m_splitlinecolor = color1;
}
/**
 * @brief CSchceduleSearchItem::setText 设置文字
 * @param tcolor 颜色
 * @param font 字体
 */
void CSchceduleSearchItem::setText(QColor tcolor, QFont font)
{
    m_ttextcolor = tcolor;
    m_tfont = font;
}
/**
 * @brief CSchceduleSearchItem::setTimeC 设置时间颜色
 * @param tcolor 颜色
 * @param font 字体
 */
void CSchceduleSearchItem::setTimeC(QColor tcolor, QFont font)
{
    m_timecolor = tcolor;
    m_timefont = font;
}
/**
 * @brief CSchceduleSearchItem::setData 设置日期
 * @param vScheduleInfo 日程信息
 * @param date 日期
 */
void CSchceduleSearchItem::setData(ScheduleDtailInfo vScheduleInfo, QDate date)
{
    m_ScheduleInfo = vScheduleInfo;
    m_date = date;
    update();
}
/**
 * @brief CSchceduleSearchItem::setRoundtype 设置日程标签的类型
 * @param rtype 日程标签类型
 */
void CSchceduleSearchItem::setRoundtype(int rtype)
{
    //根据日程类型，判断是否要绘制圆角
    m_roundtype = rtype;
    update();
}
/**
 * @brief CSchceduleSearchItem::setTheMe 根据系统主题类型设置颜色
 * @param type 系统主题类型
 */
void CSchceduleSearchItem::setTheMe(int type)
{
    if (type == 2) {
        //深色系统
        //设置点击的item背景色
        m_presscolor.background = CScheduleDataManage::getScheduleDataManage()->getSystemActiveColor();
        m_presscolor.background.setAlphaF(1);
        //设置点击的item时间颜色
        m_presscolor.timeColor = "#FFFFFF";
        m_presscolor.timeColor.setAlphaF(1);
        //设置点击的item文字颜色
        m_presscolor.textColor = "#FFFFFF";
        m_presscolor.textColor.setAlphaF(1);
        //设置hover的item的背景色
        m_hovercolor.background= "#FFFFFF";
        m_hovercolor.background.setAlphaF(0.2);
        //设置hover的item的时间颜色
        m_hovercolor.timeColor = "#6D7C88";
        m_hovercolor.timeColor.setAlphaF(1);
        //设置hover的item的文字颜色
        m_hovercolor.textColor = "#C0C6D4";
        m_hovercolor.textColor.setAlphaF(1);
    } else {
        //浅色和跟随系统
        //设置点击的item背景色
        m_presscolor.background = CScheduleDataManage::getScheduleDataManage()->getSystemActiveColor();
        m_presscolor.background.setAlphaF(1);
        //设置点击的item时间颜色
        m_presscolor.timeColor = "#FFFFFF";
        m_presscolor.timeColor.setAlphaF(1);
        //设置点击的item文字颜色
        m_presscolor.textColor = "#FFFFFF";
        m_presscolor.textColor.setAlphaF(1);
        //设置hover的item的背景色
        m_hovercolor.background= "#000000";
        m_hovercolor.background.setAlphaF(0.2);
        //设置hover的item的时间颜色
        m_hovercolor.timeColor = "#526A7F";
        m_hovercolor.timeColor.setAlphaF(1);
        //设置hover的item的文字颜色
        m_hovercolor.textColor = "#414D68";
        m_hovercolor.textColor.setAlphaF(1);
    }
}
/**
 * @brief CSchceduleSearchItem::slotEdit 编辑日程
 */
void CSchceduleSearchItem::slotEdit()
{
    emit signalViewtransparentFrame(1);
    //编辑日程
    CSchceduleDlg dlg(0, this);
    //设置日程信息
    dlg.setData(m_ScheduleInfo);
    //界面
    if (dlg.exec() == DDialog::Accepted) {
        emit signalsEdit(this);
    }
    emit signalViewtransparentFrame(0);
}
/**
 * @brief CSchceduleSearchItem::slotDelete 删除日程
 */
void CSchceduleSearchItem::slotDelete()
{
    emit signalViewtransparentFrame(1);
    //根据是否是重复日程，弹出不同的提示框
    if (m_ScheduleInfo.rpeat == 0) {
        //非重复日程
        CSchceduleCtrlDlg msgBox;
        //设置提示信息
        msgBox.setText(tr("You are deleting an event."));
        msgBox.setInformativeText(tr("Are you sure you want to delete this event?"));
        //设置按钮文字
        msgBox.addPushButton(tr("Cancel"), true);
        msgBox.addWaringButton(tr("Delete"), true);
        msgBox.exec();
        //按钮功能
        if (msgBox.clickButton() == 0) {
            //取消
            emit signalViewtransparentFrame(0);
            return;
        } else if (msgBox.clickButton() == 1) {
            //删除
            CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->deleteScheduleInfoById(m_ScheduleInfo.id);
        }
    } else {
        //重复日程
        if (m_ScheduleInfo.RecurID == 0) {
            //重复日程的第一个日程
            CSchceduleCtrlDlg msgBox;
            //设置提示信息
            msgBox.setText(tr("You are deleting an event."));
            msgBox.setInformativeText(tr("Do you want to delete all occurrences of this event, or only the selected occurrence?"));
            //设置按钮文字
            msgBox.addPushButton(tr("Cancel"));
            msgBox.addPushButton(tr("Delete All"));
            msgBox.addsuggestButton(tr("Delete Only This Event"));
            msgBox.exec();
            //按钮功能
            if (msgBox.clickButton() == 0) {
                //取消
                emit signalViewtransparentFrame(0);
                return;
            } else if (msgBox.clickButton() == 1) {
                //删除所有
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->deleteScheduleInfoById(m_ScheduleInfo.id);
            } else if (msgBox.clickButton() == 2) {
                //仅删除当前日程
                ScheduleDtailInfo newschedule;
                //根据id获取日程信息
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(m_ScheduleInfo.id, newschedule);
                //添加ignore日程
                newschedule.ignore.append(m_ScheduleInfo.beginDateTime);
                //更新日程
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);
            }
        } else {
            //重复日程非第一个日程
            CSchceduleCtrlDlg msgBox;
            //设置提示信息
            msgBox.setText(tr("You are deleting an event."));
            msgBox.setInformativeText(tr("Do you want to delete this and all future occurrences of this event, or only the selected occurrence?"));
            //设置按钮文字
            msgBox.addPushButton(tr("Cancel"));
            msgBox.addPushButton(tr("Delete All Future Events"));
            msgBox.addsuggestButton(tr("Delete Only This Event"));
            msgBox.exec();
            //按钮功能
            if (msgBox.clickButton() == 0) {
                //取消
                emit signalViewtransparentFrame(0);
                return;
            } else if (msgBox.clickButton() == 1) {
                //删除未来所有日程
                ScheduleDtailInfo newschedule;
                //根据id查找日程
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(m_ScheduleInfo.id, newschedule);
                //设置日程结束于日期
                newschedule.enddata.type = 2;
                //设置结束时间
                newschedule.enddata.date = m_ScheduleInfo.beginDateTime.addDays(-1);
                //更新日程
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);
            } else if (msgBox.clickButton() == 2) {
                //仅删除当前日程
                ScheduleDtailInfo newschedule;
                //查找日程
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(m_ScheduleInfo.id, newschedule);
                //添加ignore日程
                newschedule.ignore.append(m_ScheduleInfo.beginDateTime);
                //更新日程
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);
            }
        }
    }
    emit signalViewtransparentFrame(0);
    emit signalsDelete(this);
}
/**
 * @brief CSchceduleSearchItem::slotDoubleEvent 双击日程
 * @param type unused
 */
void CSchceduleSearchItem::slotDoubleEvent(int type)
{
    Q_UNUSED(type);
    //发送删除信号
    emit signalsDelete(this);
}
/**
 * @brief CSchceduleSearchItem::paintEvent 绘制函数
 * @param e unused
 */
void CSchceduleSearchItem::paintEvent( QPaintEvent *e )
{
    Q_UNUSED(e);
    int labelwidth = width();
    int labelheight = height();
    QPainter painter(this);
    //背景色
    QColor bcolor = m_Backgroundcolor;
    //文字颜色
    QColor textcolor = m_ttextcolor;
    //时间颜色
    QColor timecolor = m_timecolor;
    //根据鼠标状态，设置颜色
    switch (m_mouseStatus) {
    case M_NONE: {
        //default color
        bcolor = m_Backgroundcolor;
        textcolor = m_ttextcolor;
        timecolor = m_timecolor;
    }
    break;
    case M_HOVER: {
        //hover color
        bcolor = m_hovercolor.background;
        textcolor = m_hovercolor.textColor;
        timecolor = m_hovercolor.timeColor;
    }
    break;
    case M_PRESS: {
        //press color
        bcolor = m_presscolor.background;
        textcolor = m_presscolor.textColor;
        timecolor = m_presscolor.timeColor;
    }
    break;
    }
    painter.save();
    // 反锯齿;
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(bcolor));
    painter.setPen(Qt::NoPen);
    //绘制路径
    QPainterPath painterPath;
    painterPath.moveTo(m_radius, m_borderframew);
    //根据日程标签类型，判断是绘制圆角还是直角
    //左上角
    if (m_roundtype == 1 || m_roundtype == 3) {
        //左上角为圆角
        painterPath.arcTo(QRect(m_borderframew, m_borderframew, m_radius * 2, m_radius * 2), 90, 90);
    } else {
        //左上角为直角
        painterPath.lineTo(m_borderframew, m_borderframew);
        painterPath.lineTo(m_borderframew, m_radius);
    }
    //左上角到左下角之间的直线
    painterPath.lineTo(0, labelheight - m_radius);
    //左下角
    if (m_roundtype == 1 || m_roundtype == 2) {
        //左下角为圆角
        painterPath.arcTo(QRect(m_borderframew, labelheight - m_radius * 2, m_radius * 2, m_radius * 2), 180, 90);
    } else {
        //左下角为直角
        painterPath.lineTo(m_borderframew, labelheight);
        painterPath.lineTo(m_radius, labelheight);
    }
    //绘制左下角到右下角之间的直线
    painterPath.lineTo(labelwidth - m_radius, labelheight);
    //右下角
    if (m_roundtype == 1 || m_roundtype == 2) {
        //右下角为圆角
        painterPath.arcTo(QRect(labelwidth - m_radius * 2, labelheight - m_radius * 2, m_radius * 2, m_radius * 2), 270, 90);
    } else {
        //右下角为直角
        painterPath.lineTo(labelwidth, labelheight);
        painterPath.lineTo(labelwidth, labelheight - m_radius);
    }
    //右下角到右上角之间的直线
    painterPath.lineTo(labelwidth, m_radius);
    //右上角
    if (m_roundtype == 1 || m_roundtype == 3) {
        //右上角为圆角
        painterPath.arcTo(QRect(labelwidth - m_radius * 2, m_borderframew, m_radius * 2, m_radius * 2), 0, 90);
    } else {
        //右上角为直角
        painterPath.lineTo(labelwidth, m_borderframew);
        painterPath.lineTo(labelwidth - m_radius, m_borderframew);
    }
    //右上角到左上角之间的直线
    painterPath.lineTo(m_radius, m_borderframew);
    painterPath.closeSubpath();
    //绘制路径
    painter.drawPath(painterPath);
    painter.restore();
    //设置时间字体和颜色
    painter.setFont(m_timefont);
    painter.setPen(timecolor);
    //日程开始和结束时间
    QDate begindate  = m_ScheduleInfo.beginDateTime.date();
    QDate enddate = m_ScheduleInfo.endDateTime.date();
    QString datestr;
    //设置时间string
    if (begindate  == enddate) {
        datestr = m_ScheduleInfo.beginDateTime.toString("hh:mm")
                  + "-" + m_ScheduleInfo.endDateTime.toString("hh:mm");
    } else {
        datestr = m_ScheduleInfo.beginDateTime.toString("hh:mm")
                  + "-" + m_ScheduleInfo.endDateTime.toString("hh:mm");
    }
    //字体度量
    QFontMetrics fm1(m_timefont);
    int durationSize = 0;
    //时间string宽度
    durationSize = fm1.horizontalAdvance(datestr);
    int flag = Qt::AlignLeft | Qt::AlignVCenter;
    //全天
    if (m_ScheduleInfo.allday) {
        datestr = tr("All Day");
    }
    //绘制时间
    painter.drawText(QRect(12, 8, durationSize, labelheight - 16), flag, datestr);
    //绘制时间和内容之间的竖线
    painter.save();
    bcolor = m_splitlinecolor;
    QPen pen(bcolor);
    pen.setWidth(2);
    painter.setPen(pen);
    //绘制
    painter.drawLine(durationSize + 17, 0, durationSize + 17, labelheight);
    painter.restore();
    //设置问题的字体和颜色
    painter.setFont(m_tfont);
    painter.setPen(textcolor);
    int tilenameW = labelwidth - 91;
    QFontMetrics fm = painter.fontMetrics();
    QString tStitlename = m_ScheduleInfo.titleName;
    tStitlename.replace("\n", "");
    QString str = tStitlename;
    QString tstr;
    //调整显示的字符
    for (int i = 0; i < str.count(); i++) {
        tstr.append(str.at(i));
        int widthT = fm.width(tstr) + 5;
        if (widthT >= tilenameW) {
            tstr.chop(1);
            break;
        }
    }
    //字符较多，显示...
    if (tstr != str) {
        tstr = tstr + "...";
    }
    //绘制文字
    painter.drawText(QRect(durationSize + 17 + 9, 6, tilenameW, labelheight), Qt::AlignLeft, tstr);
    painter.end();
}
/**
 * @brief CSchceduleSearchItem::contextMenuEvent 设置menu
 * @param event unused
 */
void CSchceduleSearchItem::contextMenuEvent( QContextMenuEvent *event )
{
    Q_UNUSED(event);
    //节日日程不可编辑删除
    if (m_ScheduleInfo.type.ID == DDECalendar::FestivalTypeID)
        return;
    m_rightMenu->clear();
    //将“编辑”和“删除”添加到menu中
    m_rightMenu->addAction(m_editAction);
    m_rightMenu->addAction(m_deleteAction);
    //鼠标右键点击唤出menu
    m_rightMenu->exec(QCursor::pos());
}
/**
 * @brief CSchceduleSearchItem::mouseDoubleClickEvent 鼠标双击
 * @param event unused
 */
void CSchceduleSearchItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit signalViewtransparentFrame(1);
    //显示“我的日程”
    CMySchceduleView dlg(m_ScheduleInfo, this);
    connect(&dlg, &CMySchceduleView::signalsEditorDelete, this, &CSchceduleSearchItem::slotDoubleEvent);
    dlg.exec();
    disconnect(&dlg, &CMySchceduleView::signalsEditorDelete, this, &CSchceduleSearchItem::slotDoubleEvent);
    emit signalViewtransparentFrame(0);

}
/**
 * @brief CSchceduleSearchItem::mousePressEvent 鼠标单击
 * @param event 判断事件
 */
void CSchceduleSearchItem::mousePressEvent(QMouseEvent *event)
{
    //鼠标左击
    if (event->button() == Qt::LeftButton) {
        //发送选择时间的信号
        emit signalSelectDate(m_date);
        //发送选择日程信息的信号
        emit signalSelectSchedule(m_ScheduleInfo);
    }
}
/**
 * @brief CSchceduleSearchItem::mouseReleaseEvent 鼠标release
 * @param event 判断事件
 */
void CSchceduleSearchItem::mouseReleaseEvent(QMouseEvent *event)
{
    //鼠标左击
    if (event->button() == Qt::LeftButton) {
        m_mouseStatus = M_HOVER;
        update();
    }
}
/**
 * @brief CSchceduleSearchItem::enterEvent enterevent
 * @param event unused
 */
void CSchceduleSearchItem::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    m_mouseStatus = M_HOVER;
    update();
}
/**
 * @brief CSchceduleSearchItem::leaveEvent leaveevent
 * @param event unused
 */
void CSchceduleSearchItem::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    m_mouseStatus = M_NONE;
    update();
}
/**
 * @brief CSchceduleSearchItem::eventFilter 过滤器
 * @param o unused
 * @param e 事件类型
 * @return return
 */
bool CSchceduleSearchItem::eventFilter(QObject *o, QEvent *e)
{
    Q_UNUSED(o);
    //判断事件类型
    if (e->type() == QEvent::MouseButtonPress) {
        QMouseEvent *m_press = dynamic_cast<QMouseEvent *>(e);
        if (m_press->button() == Qt::LeftButton) {
            //左击
            m_mouseStatus = M_PRESS;
        }
    }
    update();
    return false;
}
/**
 * @brief CSchceduleSearchView::CSchceduleSearchView 构造函数
 * @param parent 父类
 */
CSchceduleSearchView::CSchceduleSearchView(QWidget *parent)
    : DWidget(parent)
{
    //垂直布局
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    //搜索的日程列表
    m_gradientItemList = new CScheduleListWidget(parent);
    m_gradientItemList->setAlternatingRowColors(true);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_gradientItemList);
    // set default row
    m_gradientItemList->setCurrentRow(0);
    //设置layout
    setLayout(layout);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    //设置背景色透明度
    m_bBackgroundcolor.setAlphaF(0.03);
    m_gradientItemList->setLineWidth(0);
    m_labellist.clear();
    //隐藏日程列表
    connect(m_gradientItemList,
            &CScheduleListWidget::signalListWidgetScheduleHide,
            this,
            &CSchceduleSearchView::signalScheduleHide);
}
/**
 * @brief CSchceduleSearchView::~CSchceduleSearchView 析构函数
 */
CSchceduleSearchView::~CSchceduleSearchView()
{
    for (int i = 0; i < m_gradientItemList->count(); i++) {
        QListWidgetItem *item11 = m_gradientItemList->takeItem(i);
        m_gradientItemList->removeItemWidget(item11);
        delete  item11;
        item11 = nullptr;
    }
    m_gradientItemList->clear();
}
/**
 * @brief CSchceduleSearchView::setTheMe 根据系统主题类型设置颜色
 * @param type 系统主题类型
 */
void CSchceduleSearchView::setTheMe(int type)
{
    if (type == 0 || type == 1) {
        //浅色或跟随系统
        //背景色
        m_bBackgroundcolor = "#000000";
        m_bBackgroundcolor.setAlphaF(0.03);
        //时间颜色
        m_btimecolor = "#526A7F";
        //问题颜色
        m_bttextcolor = "#414D68";
        m_lBackgroundcolor = Qt::white;
        m_ltextcolor = "#001A2E";
    } else if (type == 2) {
        //神色主题
        m_bBackgroundcolor = "#FFFFFF";
        m_bBackgroundcolor.setAlphaF(0.05);
        m_btimecolor = "#6D7C88";
        m_bttextcolor = "#C0C6D4";
        m_lBackgroundcolor = "#FFFFFF";
        m_lBackgroundcolor.setAlphaF(0.0);
        m_ltextcolor = "#C0C6D4";
    }
    updateDateShow();
}
/**
 * @brief CSchceduleSearchView::clearSearch 清空搜索
 */
void CSchceduleSearchView::clearSearch()
{
    m_vlistData.clear();

    for (int i = 0; i < m_gradientItemList->count(); i++) {
        QListWidgetItem *item11 = m_gradientItemList->takeItem(i);
        m_gradientItemList->removeItemWidget(item11);
        delete  item11;
        item11 = nullptr;
    }
    m_labellist.clear();
    m_gradientItemList->clear();
    QVector<ScheduleDateRangeInfo> vScheduleInfo;
    CScheduleDataManage::getScheduleDataManage()->setSearchResult(vScheduleInfo);
}
/**
 * @brief CSchceduleSearchView::setMaxWidth 设置最大宽度
 * @param w 宽度
 */
void CSchceduleSearchView::setMaxWidth(const int w)
{
    m_maxWidth = w;
}
/**
 * @brief CSchceduleSearchView::updateDateShow 更新显示
 */
void CSchceduleSearchView::updateDateShow()
{
    m_currentItem = nullptr;

    for (int i = 0; i < m_gradientItemList->count(); i++) {
        QListWidgetItem *item11 = m_gradientItemList->takeItem(i);
        m_gradientItemList->removeItemWidget(item11);
        delete  item11;
        item11 = nullptr;
    }
    m_gradientItemList->clear();
    m_labellist.clear();
    //找最近日程
    QDate tcurrentdata = QDate::currentDate();
    QVector<ScheduleDateRangeInfo> m_showData;
    ScheduleDateRangeInfo showData;
    qint64 offset = 1000;
    QDate topdate = tcurrentdata;
    //过滤日程
    for (int i = 0; i < m_vlistData.size(); ++i) {
        qint64 d = qAbs(m_vlistData.at(i).date.daysTo(tcurrentdata));
        showData.date = m_vlistData.at(i).date;

        for (int j = 0 ; j < m_vlistData.at(i).vData.size(); ++j) {
            if (m_vlistData.at(i).vData.at(j).beginDateTime.date() == m_vlistData.at(i).date) {
                showData.vData.append(m_vlistData.at(i).vData.at(j));
            }
        }
        if (showData.vData.count() > 0) {
            if ( d<offset) {
                offset = d;
                topdate = showData.date;
            }
            m_showData.append(showData);
        }
        showData.vData.clear();
    }
    tcurrentdata = topdate;
    //展示日程
    for (int i = 0; i < m_showData.size(); ++i) {
        QListWidgetItem *titem = createItemWidget(m_showData[i].date);

        if (m_showData[i].date == tcurrentdata) {
            m_currentItem = titem;
        }
        if (m_showData.at(i).vData.isEmpty()) continue;
        if (m_showData.at(i).vData.count() == 1) {
            createItemWidget(m_showData.at(i).vData.at(0), m_showData[i].date, 1);
        } else if (m_showData.at(i).vData.count() == 2) {
            createItemWidget(m_showData.at(i).vData.at(0), m_showData[i].date, 3);
            createItemWidget(m_showData.at(i).vData.at(1), m_showData[i].date, 2);
        } else {
            createItemWidget(m_showData.at(i).vData.at(0), m_showData[i].date, 3);
            for (int j = 1; j < m_showData.at(i).vData.count() - 1; j++) {
                createItemWidget(m_showData.at(i).vData.at(j), m_showData[i].date, 0);
            }
            createItemWidget(m_showData.at(i).vData.at(m_showData.at(i).vData.count() - 1), m_showData[i].date, 2);
        }
    }
    if (m_gradientItemList->count() == 0) {
        QListWidgetItem *listItem = new QListWidgetItem(m_gradientItemList);
        DLabel *gwi = new DLabel();
        QFont font;
        font.setPixelSize(DDECalendar::FontSizeTwenty);
        gwi->setAlignment(Qt::AlignCenter );
        DPalette daypa = gwi->palette();
        QColor textcolor = DPalette::ToolTipText;
        int themtype = CScheduleDataManage::getScheduleDataManage()->getTheme();
        if (themtype == 2) {
            textcolor = "#C0C6D4";
            textcolor.setAlphaF(0.4);
        } else {
            textcolor = "#000000";
            textcolor.setAlphaF(0.3);
        }
        daypa.setColor(DPalette::WindowText, textcolor);
        daypa.setColor(DPalette::Window, m_lBackgroundcolor);
        gwi->setPalette(daypa);
        gwi->setForegroundRole(DPalette::WindowText);
        gwi->setFont(font);
        gwi->move(this->width() - 70, this->height() - 196);
        gwi->setText(tr("No search results"));
        gwi->setFixedSize(m_maxWidth - 20, 450);
        listItem->setSizeHint(QSize(m_maxWidth, 450)); //每次改变Item的高度
        listItem->setFlags(Qt::ItemIsTristate );
        m_gradientItemList->addItem(listItem);
        m_gradientItemList->setItemWidget(listItem, gwi);
        m_labellist.append(gwi);
    }
    if (m_currentItem != nullptr) {
        m_gradientItemList->scrollToItem(m_currentItem, QAbstractItemView::PositionAtTop);
    }

}
/**
 * @brief CSchceduleSearchView::createItemWidget 新建日程标签
 * @param info 日程信息
 * @param date 日期
 * @param rtype 日程标签类型
 */
void CSchceduleSearchView::createItemWidget(ScheduleDtailInfo info, QDate date, int rtype)
{
    ScheduleDtailInfo &gd = info;
    CSchedulesColor gdcolor = CScheduleDataManage::getScheduleDataManage()->getScheduleColorByType(gd.type.ID);
    //SchceduleSearchItem
    CSchceduleSearchItem *gwi = new CSchceduleSearchItem();
    QFont font;
    font.setPixelSize(DDECalendar::FontSizeFourteen);
    font.setWeight(QFont::Normal);
    gwi->setBackgroundColor(m_bBackgroundcolor);
    QColor scolor = gdcolor.Purecolor;
    scolor.setAlphaF(1.0);
    gwi->setSplitLineColor(gdcolor.splitColor);
    gwi->setText(m_bttextcolor, font);
    font.setPixelSize(DDECalendar::FontSizeTwelve);
    //设置SchceduleSearchItem
    gwi->setTimeC(m_btimecolor, font);
    gwi->setFixedSize(m_maxWidth - 20, 35);
    gwi->setData(gd, date);
    gwi->setRoundtype(rtype);
    connect(gwi, &CSchceduleSearchItem::signalsDelete, this, &CSchceduleSearchView::slotdeleteitem);
    connect(gwi, &CSchceduleSearchItem::signalsEdit, this, &CSchceduleSearchView::slotedititem);
    connect(gwi, &CSchceduleSearchItem::signalSelectDate, this, &CSchceduleSearchView::slotSelectDate);
    connect(gwi, &CSchceduleSearchItem::signalSelectSchedule, this, &CSchceduleSearchView::slotSelectSchedule);
    connect(gwi, &CSchceduleSearchItem::signalViewtransparentFrame, this, &CSchceduleSearchView::signalViewtransparentFrame);

    QListWidgetItem *listItem = new QListWidgetItem;
    listItem->setSizeHint(QSize(m_maxWidth - 5, 36)); //每次改变Item的高度
    listItem->setFlags(Qt::ItemIsTristate );
    m_gradientItemList->addItem(listItem);
    m_gradientItemList->setItemWidget(listItem, gwi);
    m_labellist.append(gwi);
}
/**
 * @brief CSchceduleSearchView::createItemWidget 新建日程item
 * @param date 日期
 * @return  QListWidgetItem
 */
QListWidgetItem *CSchceduleSearchView::createItemWidget(QDate date)
{
    CSchceduleSearchDateItem *gwi = new CSchceduleSearchDateItem();
    QFont font;
    font.setWeight(QFont::Medium);
    font.setPixelSize(DDECalendar::FontSizeSixteen);
    gwi->setBackgroundColor(m_lBackgroundcolor);
    gwi->setText(m_ltextcolor, font);
    //设置活动色
    if (date == QDate::currentDate()) {
        int themtype = CScheduleDataManage::getScheduleDataManage()->getTheme();
        if (themtype == 2) {
            gwi->setText(CScheduleDataManage::getScheduleDataManage()->getSystemActiveColor(), font);
        } else {
            gwi->setText(CScheduleDataManage::getScheduleDataManage()->getSystemActiveColor(), font);
        }

    }
    gwi->setFixedSize(m_maxWidth - 20, 35);
    gwi->setDate(date);
    connect(gwi,
            &CSchceduleSearchDateItem::signalLabelScheduleHide,
            this,
            &CSchceduleSearchView::signalScheduleHide);
    QListWidgetItem *listItem = new QListWidgetItem;
    listItem->setSizeHint(QSize(m_maxWidth - 5, 36)); //每次改变Item的高度
    listItem->setFlags(Qt::ItemIsTristate );
    m_gradientItemList->addItem(listItem);
    m_gradientItemList->setItemWidget(listItem, gwi);
    m_labellist.append(gwi);
    return listItem;
}
/**
 * @brief CSchceduleSearchView::slotdeleteitem 删除item
 * @param item 日程item
 */
void CSchceduleSearchView::slotdeleteitem( CSchceduleSearchItem *item )
{
    emit signalsUpdateShcedule(item->getData().id);
    updateDateShow();
    update();
}
/**
 * @brief CSchceduleSearchView::slotedititem 编辑日程
 * @param item 日程item
 */
void CSchceduleSearchView::slotedititem(CSchceduleSearchItem *item)
{
    emit signalsUpdateShcedule(item->getData().id);
}
/**
 * @brief CSchceduleSearchView::slotsetSearch 搜索
 * @param str 日程string
 */
void CSchceduleSearchView::slotsetSearch(QString str)
{
    if (str.isEmpty()) return;
    QDateTime date = QDateTime::currentDateTime();
    QDateTime bdate = date.addMonths(-6);

    if (!bdate.isValid()) {
        QDateTime tdate = date;
        tdate.setDate(QDate(date.date().year(), date.date().month(), 1));
        bdate = tdate.addMonths(-6);
    }
    //搜索最晚时间为半年后
    QDateTime edate = date.addMonths(6);

    if (!edate.isValid()) {
        QDateTime tdate = date;
        tdate.setDate(QDate(date.date().year(), date.date().month(), 1));
        edate = tdate.addMonths(7);
        edate = edate.addDays(-1);
    }
    QVector<ScheduleDateRangeInfo> vScheduleInfo;
    m_vlistData.clear();
    //搜索日程
    CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->queryScheduleInfo(str, bdate, edate, vScheduleInfo);
    m_vlistData = vScheduleInfo;
    CScheduleDataManage::getScheduleDataManage()->setSearchResult(vScheduleInfo);
    updateDateShow();
}
/**
 * @brief CSchceduleSearchView::slotSelectDate 搜索date
 * @param date
 */
void CSchceduleSearchView::slotSelectDate(QDate date)
{
    emit signalDate(date);
}
/**
 * @brief CSchceduleSearchView::slotSelectSchedule 选择日程
 * @param scheduleInfo 日程信息
 */
void CSchceduleSearchView::slotSelectSchedule(const ScheduleDtailInfo &scheduleInfo)
{
    emit signalSelectSchedule(scheduleInfo);
}
/**
 * @brief CSchceduleSearchView::resizeEvent 窗口大小调整
 * @param event 窗口大小调整事件
 */
void CSchceduleSearchView::resizeEvent(QResizeEvent *event)
{
    for (int i = 0; i < m_gradientItemList->count(); i++) {
        QListWidgetItem *item11 = m_gradientItemList->item(i);
        item11->setSizeHint(QSize(m_maxWidth - 5, 36)); //每次改变Item的高度
    }
    for (int i = 0; i < m_labellist.count(); i++) {
        m_labellist.at(i)->setFixedSize(m_maxWidth - 20, 35);
        m_labellist.at(i)->update();
    }
    if (m_gradientItemList->count() == 1) {
        QListWidgetItem *item11 = m_gradientItemList->item(0);
        item11->setSizeHint(QSize(m_maxWidth, qRound(height() * 0.7978))); //每次改变Item的高度
        m_labellist.at(0)->setFixedSize(m_maxWidth, qRound(height() * 0.7978));
        m_labellist.at(0)->update();
    }
    if (m_gradientItemList->count() >1) {
        if (m_currentItem != nullptr) {
            m_gradientItemList->scrollToItem(m_currentItem, QAbstractItemView::PositionAtTop);
        }
    }
    DWidget::resizeEvent(event);
}
/**
 * @brief CSchceduleSearchView::mousePressEvent 鼠标点击
 * @param event 鼠标事件
 */
void CSchceduleSearchView::mousePressEvent(QMouseEvent *event)
{
    DWidget::mousePressEvent(event);
}
/**
 * @brief CSchceduleSearchDateItem::CSchceduleSearchDateItem 搜索dateitem
 * @param parent
 */
CSchceduleSearchDateItem::CSchceduleSearchDateItem(QWidget *parent)
    : DLabel(parent)
{
    //设置对象名称和辅助显示名称
    this->setObjectName("CScheduleDateItem");
    this->setAccessibleName("CScheduleDateItem");
    setAutoFillBackground(true);
}
/**
 * @brief CSchceduleSearchDateItem::setBackgroundColor 设置背景色
 * @param color1 颜色
 */
void CSchceduleSearchDateItem::setBackgroundColor(QColor color1)
{
    m_Backgroundcolor = color1;
}
/**
 * @brief CSchceduleSearchDateItem::setText 设置文字字体
 * @param tcolor 字体颜色
 * @param font 字体
 */
void CSchceduleSearchDateItem::setText(QColor tcolor, QFont font)
{
    m_textcolor = tcolor;
    m_font = font;
}
/**
 * @brief CSchceduleSearchDateItem::setDate 设置date
 * @param date date
 */
void CSchceduleSearchDateItem::setDate(QDate date)
{
    m_date = date;
}
/**
 * @brief CSchceduleSearchDateItem::paintEvent 绘制函数
 * @param e unused
 */
void CSchceduleSearchDateItem::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    int labelwidth = width();
    int labelheight = height();
    //配置painter
    QPainter painter(this);
    QRect fillRect = QRect(0, 0, labelwidth, labelheight);
    painter.setRenderHints(QPainter::HighQualityAntialiasing);
    painter.setBrush(QBrush(m_Backgroundcolor));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(fillRect, 3, 3);
    //设置字体和颜色
    painter.setFont(m_font);
    painter.setPen(m_textcolor);

    QLocale locale;
    QString datestr;
    //根据系统语言设置时间显示
    if (locale.language() == QLocale::Chinese) {
        datestr = QString::number(m_date.year()) + tr("Y") + QString::number(m_date.month()) + tr("M") + QString::number(m_date.day()) + tr("D");
    } else {
        datestr = m_date.toString("yyyy/M/d");
    }
    painter.drawText(QRect(12, 8, labelwidth, labelheight - 8), Qt::AlignLeft, datestr);
    painter.end();
}
/**
 * @brief CSchceduleSearchDateItem::mousePressEvent 鼠标点击
 * @param event unused
 */
void CSchceduleSearchDateItem::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit signalLabelScheduleHide();
}
/**
 * @brief CScheduleListWidget::CScheduleListWidget 构造函数
 * @param parent 父类
 */
CScheduleListWidget::CScheduleListWidget(QWidget *parent)
    : DListWidget (parent)
{
    //设置对象名称和辅助显示名称
    this->setObjectName("CScheduleListWidget");
    this->setAccessibleName("CScheduleListWidget");
}
/**
 * @brief CScheduleListWidget::~CScheduleListWidget 析构函数
 */
CScheduleListWidget::~CScheduleListWidget()
{

}
/**
 * @brief CScheduleListWidget::mousePressEvent 鼠标点击
 * @param event 鼠标事件
 */
void CScheduleListWidget::mousePressEvent(QMouseEvent *event)
{
    DListWidget::mousePressEvent(event);
    emit signalListWidgetScheduleHide();
}
