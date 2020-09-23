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
#include "myschceduleview.h"
#include "schceduledlg.h"
#include "scheduledatamanage.h"
#include "schcedulectrldlg.h"
#include "cdynamicicon.h"
#include "constants.h"

#include <DMessageBox>
#include <DPushButton>
#include <DHiDPIHelper>
#include <DPalette>
#include <DFontSizeManager>
#include <DLabel>

#include <QShortcut>
#include <QVBoxLayout>
#include <QPainter>
#include <QtMath>

DGUI_USE_NAMESPACE
/**
 * @brief CMySchceduleView::CMySchceduleView 构造函数
 * @param schduleInfo 日程信息
 * @param parent 父类
 */
CMySchceduleView::CMySchceduleView(const ScheduleDtailInfo &schduleInfo, QWidget *parent)
    : DDialog(parent)
{
    //设置边距
    setContentsMargins(0, 0, 0, 0);
    m_scheduleInfo = schduleInfo;
    //初始化界面
    initUI();
    //初始化connect
    initConnection();
    //根据主题type设置颜色
    setLabelTextColor(DGuiApplicationHelper::instance()->themeType());
    //设置大小
    setFixedSize(380, 160);
    //设置日程title
    AutoFeed(m_scheduleInfo.titleName);
    //设置日程时间显示
    if (m_scheduleInfo.type.ID == DDECalendar::FestivalTypeID) {
        //节日日程
        m_timeLabel->setText(m_scheduleInfo.beginDateTime.toString(("yyyy-MM-dd")));
    } else {
        //普通日程
        m_timeLabel->setText(m_scheduleInfo.beginDateTime.toString("yyyy-MM-dd hh:mm") + " ~ " + m_scheduleInfo.endDateTime.toString("yyyy-MM-dd hh:mm"));
    }
}
/**
 * @brief CMySchceduleView::~CMySchceduleView 析构函数
 */
CMySchceduleView::~CMySchceduleView()
{
    emit signalViewtransparentFrame(0);
}
/**
 * @brief CMySchceduleView::AutoFeed 设置日程详情
 * @param text 日程详情
 */
void CMySchceduleView::AutoFeed(QString text)
{
    QString strText = text;
    QString resultStr = nullptr;
    QFont labelF;
    labelF.setWeight(QFont::Medium);
    labelF = DFontSizeManager::instance()->get(DFontSizeManager::T6, labelF);
    QFontMetrics fm(labelF);
    int titlewidth = fm.width(strText);
    QStringList strList;
    QString str;
    int h = fm.height();
    strList.clear();
    //处理title text
    if (titlewidth < 330) {
        strList.append(strText);
        resultStr += strText;
    } else {
        for (int i = 0; i < strText.count(); i++) {
            str += strText.at(i);
            //字符串的宽度超过文本框的宽度，进行换行处理
            if (fm.width(str) > 330) {
                str.remove(str.count() - 1, 1);
                strList.append(str);
                resultStr += str + "\n";
                str.clear();
                --i;
            }
        }
        strList.append(str);
        resultStr += str;
    }
    //设置高度
    if (strList.count() * h + 140 > 240) {
        area->setFixedHeight(100);
        setFixedHeight(250);
    } else {
        area->setFixedHeight(strList.count() * h);
        setFixedHeight(strList.count() * h + 152);
    }
    //设置text
    m_schceduleLabel->setText(resultStr);
}

/**
 * @brief setLabelTextColor     设置label文字颜色
 * @param type  主题type
 */
void CMySchceduleView::setLabelTextColor(const int type)
{
    //标题显示颜色
    QColor titleColor;
    //日程显示颜色
    QColor scheduleTitleColor;
    //时间显示颜色
    QColor timeColor;
    //根据系统主题类型设置颜色
    if (type == 2) {
        //系统主题类型为深色
        titleColor = "#C0C6D4";
        scheduleTitleColor = "#FFFFFF";
        timeColor = "#FFFFFF";
        timeColor.setAlphaF(0.7);
    } else {
        //系统主题类型为浅色或跟随系统
        titleColor = "#001A2E";
        scheduleTitleColor = "#000000";
        scheduleTitleColor.setAlphaF(0.9);
        timeColor = "#000000";
        timeColor.setAlphaF(0.7);
    }
    //设置颜色
    setPaletteTextColor(m_Title, titleColor);
    setPaletteTextColor(m_schceduleLabel, scheduleTitleColor);
    setPaletteTextColor(m_timeLabel, timeColor);
}

/**
 * @brief setPaletteTextColor   设置调色板颜色
 * @param widget    需要设置的widget
 * @param textColor     显示颜色
 */
void CMySchceduleView::setPaletteTextColor(QWidget *widget, QColor textColor)
{
    //如果为空指针则退出
    if (nullptr == widget)
        return;
    DPalette palette = widget->palette();
    //设置文字显示颜色
    palette.setColor(DPalette::WindowText, textColor);
    widget->setPalette(palette);
}

/**
 * @brief CMySchceduleView::showEvent 展示
 * @param event
 */
void CMySchceduleView::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    emit signalViewtransparentFrame(1);
}
/**
 * @brief CMySchceduleView::eventFilter 过滤器
 * @param o 过滤widget
 * @param e 过滤事件
 * @return false
 */
bool CMySchceduleView::eventFilter(QObject *o, QEvent *e)
{
    Q_UNUSED(o);
    //字体大小改变事件
    if (e->type() == QEvent::FontChange) {
        //重新设置日程详情
        AutoFeed(m_scheduleInfo.titleName);
    }

    return false;
}
/**
 * @brief CMySchceduleView::slotEditBt 编辑按钮
 * @param buttonIndex 按钮id
 * @param buttonName 修改内容
 */
void CMySchceduleView::slotEditBt(int buttonIndex, QString buttonName)
{
    //不是编辑按钮，return
    if (buttonIndex != 1 && buttonName != "Edit")
        return;
    //设置日程信息
    CSchceduleDlg dlg(0, this);
    dlg.setData(m_scheduleInfo);
    //弹出编辑窗口
    if (dlg.exec() == DDialog::Accepted) {
        accept();
        emit signalsEditorDelete(1);
    }
}
/**
 * @brief CMySchceduleView::slotDeleteBt 删除按钮
 * @param buttonIndex 按钮id
 * @param buttonName 日程详情
 */
void CMySchceduleView::slotDeleteBt(int buttonIndex, QString buttonName)
{
    //不是删除按钮，return
    if (buttonIndex != 0 && buttonName != "Delete")
        return;
    //不同按钮不同功能
    if (m_scheduleInfo.rpeat == 0) {
        CSchceduleCtrlDlg msgBox;
        //设置提示性文字
        msgBox.setText(tr("You are deleting an event."));
        msgBox.setInformativeText(tr("Are you sure you want to delete this event?"));
        //设置按钮文字
        msgBox.addPushButton(tr("Cancel"), true);
        msgBox.addWaringButton(tr("Delete"), true);
        msgBox.exec();
        //是否删除
        if (msgBox.clickButton() == 0) {
            //取消
            return;
        } else if (msgBox.clickButton() == 1) {
            //删除
            CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->deleteScheduleInfoById(m_scheduleInfo.id);
        } else {
            return;
        }
    } else {
        if (m_scheduleInfo.RecurID == 0) {
            CSchceduleCtrlDlg msgBox;
            //设置提示性文字
            msgBox.setText(tr("You are deleting an event."));
            msgBox.setInformativeText(tr("Do you want to delete all occurrences of this event, or only the selected occurrence?"));
            //设置按钮文字
            msgBox.addPushButton(tr("Cancel"));
            msgBox.addPushButton(tr("Delete All"));
            msgBox.addWaringButton(tr("Delete Only This Event"));
            msgBox.exec();
            //各个按钮功能
            if (msgBox.clickButton() == 0) {
                //取消
                return;
            } else if (msgBox.clickButton() == 1) {
                //删除所有
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->deleteScheduleInfoById(m_scheduleInfo.id);
            } else if (msgBox.clickButton() == 2) {
                //仅删除这个日程
                ScheduleDtailInfo newschedule;
                //根据id查找点击日程
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(m_scheduleInfo.id, newschedule);
                //忽略需要删除的日程
                newschedule.ignore.append(m_scheduleInfo.beginDateTime);
                //更新日程信息
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);
            } else {
                return;
            }
        } else {
            CSchceduleCtrlDlg msgBox;
            //设置提示性文字
            msgBox.setText(tr("You are deleting an event."));
            msgBox.setInformativeText(tr("Do you want to delete this and all future occurrences of this event, or only the selected occurrence?"));
            //设置按钮文字
            msgBox.addPushButton(tr("Cancel"));
            msgBox.addPushButton(tr("Delete All Future Events"));
            msgBox.addWaringButton(tr("Delete Only This Event"));
            msgBox.exec();
            //各个按钮功能
            if (msgBox.clickButton() == 0) {
                //取消
                return;
            } else if (msgBox.clickButton() == 1) {
                //删除未来所有
                ScheduleDtailInfo newschedule;
                //根据id查找点击的日程
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(m_scheduleInfo.id, newschedule);
                //设置日程结束重复于的类型
                newschedule.enddata.type = 2;
                //设置重复日程结束日期
                newschedule.enddata.date = m_scheduleInfo.beginDateTime.addDays(-1);
                //更新日程信息
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);
            } else if (msgBox.clickButton() == 2) {
                //仅删除这个日程
                ScheduleDtailInfo newschedule;
                //根据id查找点击的日程
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->getScheduleInfoById(m_scheduleInfo.id, newschedule);
                //忽略这个日程
                newschedule.ignore.append(m_scheduleInfo.beginDateTime);
                //更新日程信息
                CScheduleDataManage::getScheduleDataManage()->getscheduleDataCtrl()->updateScheduleInfo(newschedule);
            } else {
                return;
            }
        }
    }
    accept();
    emit signalsEditorDelete(1);
}
/**
 * @brief CMySchceduleView::initUI 初始化界面
 */
void CMySchceduleView::initUI()
{
    //在点击任何对话框上的按钮后不关闭对话框，保证关闭子窗口时不被一起关掉
    setOnButtonClickedClose(false);
    //title label
    m_Title = new QLabel(this);
    m_Title->setFixedSize(108, 51);
    m_Title->setAlignment(Qt::AlignCenter);
    QFont titleFont;
    //设置字重
    titleFont.setWeight(QFont::Bold);
    m_Title->setFont(titleFont);
    DFontSizeManager::instance()->bind(m_Title, DFontSizeManager::T5);
    //设置日期图标
    QIcon t_icon(CDynamicIcon::getInstance()->getPixmap());
    setIcon(t_icon);
    QFont labelTitle;
    labelTitle.setWeight(QFont::DemiBold);
    m_Title->setFont(labelTitle);
    m_Title->setText(tr("My Event"));
    m_Title->move(137, 0);
    //main布局
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    //带滚动条的区域
    area = new QScrollArea(this);
    area->setFrameShape(QFrame::NoFrame);
    area->setFixedWidth(363);
    area->setBackgroundRole(QPalette::Background);
    area->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    area->setWidgetResizable(true);
    //设置对齐方式
    area->setAlignment(Qt::AlignCenter);
    //日程label
    m_schceduleLabel = new QLabel(this);
    m_schceduleLabel->setTextFormat(Qt::PlainText);//纯文本格式
    //设置过滤器
    m_schceduleLabel->installEventFilter(this);
    m_schceduleLabel->setFixedWidth(330);
    m_schceduleLabel->setAlignment(Qt::AlignCenter);
    //设置字体大小
    DFontSizeManager::instance()->bind(m_schceduleLabel, DFontSizeManager::T6);
    labelF.setWeight(QFont::Medium);
    m_schceduleLabel->setFont(labelF);
    //将日程label添加到area
    area->setWidget(m_schceduleLabel);
    //将area添加到main布局
    mainLayout->addWidget(area);
    //时间label
    m_timeLabel = new DLabel(this);
    m_timeLabel->setFixedHeight(26);
    m_timeLabel->setAlignment(Qt::AlignCenter);
    QFont timeFont;
    timeFont.setWeight(QFont::Normal);
    m_timeLabel->setFont(timeFont);
    mainLayout->addSpacing(6);
    //将时间label添加到main布局
    mainLayout->addWidget(m_timeLabel);
    //设置按钮
    if (m_scheduleInfo.type.ID == DDECalendar::FestivalTypeID) {
        //节假日
        addButton(tr("OK"), false, DDialog::ButtonNormal);
        //添加ok按钮
        QAbstractButton *button_ok = getButton(0);
        button_ok->setFixedSize(360, 36);
    } else {
        //非节假日，添加“删除”和“编辑”按钮
        addButton(tr("Delete"), false, DDialog::ButtonNormal);
        addButton(tr("Edit"), false, DDialog::ButtonRecommend);
        //设置按钮大小
        for (int i = 0; i < buttonCount(); i++) {
            QAbstractButton *button = getButton(i);
            button->setFixedSize(165, 36);
        }
    }

    //这种中心铺满的weiget，显示日程标题和时间的控件
    DWidget *centerWidget = new DWidget(this);
    centerWidget->setLayout(mainLayout);
    //获取widget的调色板
    DPalette centerWidgetPalette = centerWidget->palette();
    //设置背景色为透明
    centerWidgetPalette.setColor(DPalette::Background, Qt::transparent);
    centerWidget->setPalette(centerWidgetPalette);
    //添加窗口为剧中对齐
    addContent(centerWidget, Qt::AlignCenter);
}

void CMySchceduleView::initConnection()
{
    //关联主题改变事件
    QObject::connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
                     this,
                     &CMySchceduleView::setLabelTextColor);
    //按钮点击
    if (m_scheduleInfo.type.ID == DDECalendar::FestivalTypeID) {
        //节日日程按钮
        connect(this, &DDialog::buttonClicked, this, &CMySchceduleView::close);
    } else {
        //普通日程按钮
        connect(this, &DDialog::buttonClicked, this, &CMySchceduleView::slotEditBt);
        connect(this, &DDialog::buttonClicked, this, &CMySchceduleView::slotDeleteBt);
    }
    //快捷键
    QShortcut *shortcut = new QShortcut(this);
    shortcut->setKey(QKeySequence(QLatin1String("ESC")));
    connect(shortcut, SIGNAL(activated()), this, SLOT(close()));
}
