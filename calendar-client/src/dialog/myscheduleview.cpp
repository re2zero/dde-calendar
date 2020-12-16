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
#include "myscheduleview.h"
#include "scheduledlg.h"
#include "scheduledatamanage.h"
#include "schedulectrldlg.h"
#include "cdynamicicon.h"
#include "constants.h"
#include "cscheduleoperation.h"

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
CMyScheduleView::CMyScheduleView(const ScheduleDataInfo &schduleInfo, QWidget *parent)
    : DDialog(parent)
{
    setContentsMargins(0, 0, 0, 0);
    m_scheduleInfo = schduleInfo;
    initUI();
    initConnection();
    //根据主题type设置颜色
    setLabelTextColor(DGuiApplicationHelper::instance()->themeType());
    setFixedSize(380, 160);
    AutoFeed(m_scheduleInfo.getTitleName());

    if (m_scheduleInfo.getType() == DDECalendar::FestivalTypeID) {
        m_timeLabel->setText(m_scheduleInfo.getBeginDateTime().toString(("yyyy-MM-dd")));
    } else {
        m_timeLabel->setText(m_scheduleInfo.getBeginDateTime().toString("yyyy-MM-dd hh:mm") + " ~ " + m_scheduleInfo.getEndDateTime().toString("yyyy-MM-dd hh:mm"));
    }
}

CMyScheduleView::~CMyScheduleView()
{
    emit signalViewtransparentFrame(0);
}

/**
 * @brief CMyScheduleView::AutoFeed     字体改变更改界面显示
 * @param text
 */
void CMyScheduleView::AutoFeed(QString text)
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

    if (titlewidth < 330) {
        strList.append(strText);
        resultStr += strText;
    } else {
        for (int i = 0; i < strText.count(); i++) {
            str += strText.at(i);

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

    if (strList.count() * h + 140 > 240) {
        area->setFixedHeight(100);
        setFixedHeight(250);
    } else {
        area->setFixedHeight(strList.count() * h);
        setFixedHeight(strList.count() * h + 152);
    }

    m_scheduleLabel->setText(resultStr);
}

/**
 * @brief setLabelTextColor     设置label文字颜色
 * @param type  主题type
 */
void CMyScheduleView::setLabelTextColor(const int type)
{
    //标题显示颜色
    QColor titleColor;
    //日程显示颜色
    QColor scheduleTitleColor;
    //时间显示颜色
    QColor timeColor;
    if (type == 2) {
        titleColor = "#C0C6D4";
        scheduleTitleColor = "#FFFFFF";
        timeColor = "#FFFFFF";
        timeColor.setAlphaF(0.7);
    } else {
        titleColor = "#001A2E";
        scheduleTitleColor = "#000000";
        scheduleTitleColor.setAlphaF(0.9);
        timeColor = "#000000";
        timeColor.setAlphaF(0.7);
    }
    //设置颜色
    setPaletteTextColor(m_Title, titleColor);
    setPaletteTextColor(m_scheduleLabel, scheduleTitleColor);
    setPaletteTextColor(m_timeLabel, timeColor);
}

/**
 * @brief setPaletteTextColor   设置调色板颜色
 * @param widget                需要设置的widget
 * @param textColor             显示颜色
 */
void CMyScheduleView::setPaletteTextColor(QWidget *widget, QColor textColor)
{
    //如果为空指针则退出
    if (nullptr == widget)
        return;
    DPalette palette = widget->palette();
    //设置文字显示颜色
    palette.setColor(DPalette::WindowText, textColor);
    widget->setPalette(palette);
}

void CMyScheduleView::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    emit signalViewtransparentFrame(1);
}

bool CMyScheduleView::eventFilter(QObject *o, QEvent *e)
{
    Q_UNUSED(o);

    if (e->type() == QEvent::FontChange) {
        AutoFeed(m_scheduleInfo.getTitleName());
    }

    return false;
}

/**
 * @brief CMyScheduleView::slotBtClick      按钮点击事件
 * @param buttonIndex
 * @param buttonName
 */
void CMyScheduleView::slotBtClick(int buttonIndex, QString buttonName)
{
    Q_UNUSED(buttonName);
    if (buttonIndex == 0) {
        //删除日程
        CScheduleOperation _scheduleOpertion(this);
        if (_scheduleOpertion.deleteSchedule(m_scheduleInfo)) {
            accept();
        };
        return;
    }
    if (buttonIndex == 1) {
        //编辑日程
        CScheduleDlg dlg(0, this);
        dlg.setData(m_scheduleInfo);
        if (dlg.exec() == DDialog::Accepted) {
            accept();
        }
        return;
    }
}

/**
 * @brief CMyScheduleView::initUI       界面初始化
 */
void CMyScheduleView::initUI()
{
    //在点击任何对话框上的按钮后不关闭对话框，保证关闭子窗口时不被一起关掉
    setOnButtonClickedClose(false);

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

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    area = new QScrollArea(this);
    area->setFrameShape(QFrame::NoFrame);
    area->setFixedWidth(363);
    area->setBackgroundRole(QPalette::Background);
    area->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    area->setWidgetResizable(true);
    area->setAlignment(Qt::AlignCenter);

    m_scheduleLabel = new QLabel(this);
    m_scheduleLabel->setTextFormat(Qt::PlainText); //纯文本格式
    m_scheduleLabel->installEventFilter(this);
    m_scheduleLabel->setFixedWidth(330);
    m_scheduleLabel->setAlignment(Qt::AlignCenter);
    DFontSizeManager::instance()->bind(m_scheduleLabel, DFontSizeManager::T6);
    labelF.setWeight(QFont::Medium);
    m_scheduleLabel->setFont(labelF);

    area->setWidget(m_scheduleLabel);
    mainLayout->addWidget(area);

    m_timeLabel = new DLabel(this);
    m_timeLabel->setFixedHeight(26);
    m_timeLabel->setAlignment(Qt::AlignCenter);
    QFont timeFont;
    timeFont.setWeight(QFont::Normal);
    m_timeLabel->setFont(timeFont);
    mainLayout->addSpacing(6);
    mainLayout->addWidget(m_timeLabel);

    if (m_scheduleInfo.getType() == DDECalendar::FestivalTypeID) {
        addButton(tr("OK"), false, DDialog::ButtonNormal);
        QAbstractButton *button_ok = getButton(0);
        button_ok->setFixedSize(360, 36);
    } else {
        addButton(tr("Delete"), false, DDialog::ButtonNormal);
        addButton(tr("Edit"), false, DDialog::ButtonRecommend);
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

void CMyScheduleView::initConnection()
{
    //关联主题改变事件
    QObject::connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged,
                     this,
                     &CMyScheduleView::setLabelTextColor);
    if (m_scheduleInfo.getType() == DDECalendar::FestivalTypeID) {
        connect(this, &DDialog::buttonClicked, this, &CMyScheduleView::close);
    } else {
        connect(this, &DDialog::buttonClicked, this, &CMyScheduleView::slotBtClick);
    }

    QShortcut *shortcut = new QShortcut(this);
    shortcut->setKey(QKeySequence(QLatin1String("ESC")));
    connect(shortcut, SIGNAL(activated()), this, SLOT(close()));
}
