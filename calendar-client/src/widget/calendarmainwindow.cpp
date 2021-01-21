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
#include "calendarmainwindow.h"
#include "widget/yearWidget/yearwindow.h"
#include "widget/monthWidget/monthwindow.h"
#include "dbuscalendar_adaptor.h"
#include "widget/weekWidget/weekwindow.h"
#include "widget/dayWidget/daywindow.h"
#include "scheduledatamanage.h"
#include "myscheduleview.h"
#include "configsettings.h"
#include "shortcut.h"
#include "schedulesearchview.h"
#include "cdynamicicon.h"
#include "constants.h"
#include "scheduledlg.h"

#include <DHiDPIHelper>
#include <DPalette>
#include <DFontSizeManager>
#include <DWidgetUtil>
#include <DTitlebar>
#include <DApplicationHelper>

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QShortcut>
#include <QSizePolicy>
#include <QHBoxLayout>
#include <QWidget>
#include <QMenuBar>
#include <QMouseEvent>

DGUI_USE_NAMESPACE
DWIDGET_USE_NAMESPACE

static const int CalendarMWidth = 860;
static const int CalendarMHeight = 634;

Calendarmainwindow::Calendarmainwindow(QWidget *w)
    : DMainWindow(w)
{
    m_currentdate = QDate::currentDate();
    setContentsMargins(QMargins(0, 0, 0, 0));
    initUI();
    initConnection();
    setMinimumSize(CalendarMWidth, CalendarMHeight);
    setWindowTitle(tr("Calendar"));
    new CalendarAdaptor(this);
    resize(CalendarMWidth, CalendarMHeight);

    QShortcut *shortcut = new QShortcut(this);
    shortcut->setKey(QKeySequence(QLatin1String("Ctrl+F")));
    connect(shortcut, SIGNAL(activated()), this, SLOT(slotSearchEdit()));

    QShortcut *viewshortcut = new QShortcut(this);
    viewshortcut->setKey(QKeySequence(QLatin1String("Ctrl+Shift+/")));
    connect(viewshortcut, SIGNAL(activated()), this, SLOT(onViewShortcut()));

    setTitlebarShadowEnabled(true);
    setFocusPolicy(Qt::ClickFocus);
    //如果为平板模式则使其大小为屏幕大小
    if (DDECalendar::isTable) {
        QDesktopWidget *w = QApplication::desktop();
        setFixedSize(w->size());
    } else {
        QByteArray arrybyte = CConfigSettings::value("base.geometry").toByteArray();
        bool isOk = false;
        int state = CConfigSettings::value("base.state").toInt(&isOk);
        if (!arrybyte.isEmpty() && isOk) {
            Qt::WindowStates winStates = static_cast<Qt::WindowStates>(state);
            setWindowState(winStates);
            if (winStates != Qt::WindowState::WindowMaximized) {
                restoreGeometry(arrybyte);
            }
        }
    }

    Dtk::Widget::moveToCenter(this);
}

Calendarmainwindow::~Calendarmainwindow()
{
    CDynamicIcon::releaseInstance();
}

void Calendarmainwindow::onViewShortcut()
{
    QRect rect = window()->geometry();
    QPoint pos(rect.x() + rect.width() / 2, rect.y() + rect.height() / 2);
    Shortcut sc;
    QStringList shortcutString;
    QString param1 = "-j=" + sc.toStr();
    QString param2 = "-p=" + QString::number(pos.x()) + "," + QString::number(pos.y());
    shortcutString << "-b" << param1 << param2;

    QProcess *shortcutViewProc = new QProcess(this);
    shortcutViewProc->startDetached("killall deepin-shortcut-viewer");
    shortcutViewProc->startDetached("deepin-shortcut-viewer", shortcutString);

    connect(shortcutViewProc, SIGNAL(finished(int)), shortcutViewProc, SLOT(deleteLater()));
}

/**
 * @brief Calendarmainwindow::slotCurrentDateUpdate     更新当前时间
 */
void Calendarmainwindow::slotCurrentDateUpdate()
{
    //获取当前时间
    const QDateTime _currentDate = QDateTime::currentDateTime();
    //设置当前时间
    m_DayWindow->setCurrentDateTime(_currentDate);
    //如果当前日期与动态图标日期不一样则重新生成动态图标
    if (_currentDate.date() != CDynamicIcon::getInstance()->getDate()) {
        CDynamicIcon::getInstance()->setDate(QDate::currentDate());
        CDynamicIcon::getInstance()->setIcon();
        //更新视图数据显示
        for (int i = 0; i < m_stackWidget->count(); ++i) {
            CScheduleBaseWidget *widget = qobject_cast<CScheduleBaseWidget *>(m_stackWidget->widget(i));
            widget->updateData();
        }
        //设置年视图年数据时间显示
        m_yearwindow->setYearData();
        //更新月视图当前周横线绘制
        m_monthWindow->setCurrentDateTime(_currentDate);
    }
}

/**
 * @brief Calendarmainwindow::viewWindow        切换视图
 * @param type                                  视图索引
 * @param showAnimation                         是否显示动画
 */
void Calendarmainwindow::viewWindow(int type, const bool showAnimation)
{
    if (type < 0 || type > m_stackWidget->count()) {
        return;
    }
    if (showAnimation) {
        m_stackWidget->setCurrent(type);
    } else {
        m_stackWidget->setCurrentIndex(type);
    }
    //获取焦点
    m_buttonBox->button(type)->setFocus();
    //设置选中
    m_buttonBox->button(type)->setChecked(true);
    switch (type) {
    case DDECalendar::CalendarYearWindow: {
        //更新界面显示
        m_yearwindow->updateData();
    } break;
    case DDECalendar::CalendarMonthWindow: {
    } break;
    case DDECalendar::CalendarWeekWindow: {
    } break;
    case DDECalendar::CalendarDayWindow: {
        m_DayWindow->setTime();
        m_searchflag = true;
    } break;
    }
    m_priindex = type == 0 ? m_priindex : type;
    CConfigSettings::setOption("base.view", type);
}

void Calendarmainwindow::updateHigh()
{
    int index = m_stackWidget->currentIndex();

    if (index < 0 || index > m_stackWidget->count() - 1) {

        return;
    }

    switch (index) {
    case DDECalendar::CalendarYearWindow: {
    } break;
    case DDECalendar::CalendarMonthWindow: {
    } break;
    case DDECalendar::CalendarWeekWindow: {
        m_weekWindow->updateHigh();
    } break;
    case DDECalendar::CalendarDayWindow: {
        m_DayWindow->updateHigh();
    } break;
    }
}

void Calendarmainwindow::setSearchWidth(int w)
{
    m_contentBackground->setFixedWidth(w);
}

void Calendarmainwindow::slotTheme(int type)
{
    if (type == 0) {
        type = DGuiApplicationHelper::instance()->themeType();
    }

    if (type == 1) {

        DPalette pl = m_yearButton->palette();
        pl.setColor(DPalette::ButtonText, QColor("#414D68"));
        pl.setColor(DPalette::Light, QColor("#E6E6E6"));
        pl.setColor(DPalette::Dark, QColor("#E3E3E3"));
        QColor sbcolor("#000000");
        sbcolor.setAlphaF(0.08);
        pl.setColor(DPalette::Shadow, sbcolor);
        m_yearButton->setPalette(pl);
        m_monthButton->setPalette(pl);
        m_weekButton->setPalette(pl);
        m_dayButton->setPalette(pl);

        DPalette anipa = m_contentBackground->palette();
        anipa.setColor(DPalette::Background, "#F8F8F8");
        m_contentBackground->setPalette(anipa);
        m_contentBackground->setBackgroundRole(DPalette::Background);

        DPalette tframepa = m_transparentFrame->palette();
        QColor tColor = "#FFFFFF";
        tColor.setAlphaF(0.3);
        tframepa.setColor(DPalette::Background, tColor);
        m_transparentFrame->setPalette(tframepa);
        m_transparentFrame->setBackgroundRole(DPalette::Background);

    } else {
        DPalette pl = m_yearButton->palette();
        pl.setColor(DPalette::ButtonText, QColor("#C0C6D4"));
        pl.setColor(DPalette::Light, QColor("#484848"));
        pl.setColor(DPalette::Dark, QColor("#414141"));
        QColor sbcolor("#000000");
        sbcolor.setAlphaF(0.08);
        pl.setColor(DPalette::Shadow, sbcolor);
        m_yearButton->setPalette(pl);
        m_monthButton->setPalette(pl);
        m_weekButton->setPalette(pl);
        m_dayButton->setPalette(pl);
        DPalette anipa = m_contentBackground->palette();
        anipa.setColor(DPalette::Background, "#252525");
        m_contentBackground->setPalette(anipa);
        m_contentBackground->setBackgroundRole(DPalette::Background);

        DPalette tframepa = m_transparentFrame->palette();
        QColor tColor = "#000000";
        tColor.setAlphaF(0.3);
        tframepa.setColor(DPalette::Background, tColor);
        m_transparentFrame->setPalette(tframepa);
        m_transparentFrame->setBackgroundRole(DPalette::Background);
    }
    CScheduleDataManage::getScheduleDataManage()->setTheMe(type);
    m_yearwindow->setTheMe(type);
    m_monthWindow->setTheMe(type);
    m_weekWindow->setTheMe(type);
    m_DayWindow->setTheMe(type);
    m_scheduleSearchView->setTheMe(type);
}

void Calendarmainwindow::OpenSchedule(QString job)
{
    if (job.isEmpty())
        return;
    ScheduleDataInfo out;
    out = ScheduleDataInfo::JsonStrToSchedule(job);
    m_dayButton->setFocus();
    m_dayButton->setChecked(true);
    //切换到日视图
    m_stackWidget->setCurrentIndex(DDECalendar::CalendarDayWindow);
    //设置选择时间
    m_DayWindow->setSelectDate(out.getBeginDateTime().date());
    //更新界面显示
    m_DayWindow->updateData();
    //设置非全天时间定位位置
    m_DayWindow->setTime(out.getBeginDateTime().time());
    //弹出编辑对话框
    CMyScheduleView dlg(out, this);
    dlg.exec();
    slotWUpdateShcedule();
}

void Calendarmainwindow::ActiveWindow()
{
    raise();
}

void Calendarmainwindow::RaiseWindow()
{
    raise();
}
void Calendarmainwindow::initUI()
{
    //设置主窗口辅助技术显示名称和描述
    this->setObjectName("MainWindow");
    this->setAccessibleName("MainWindow");
    this->setAccessibleDescription("This is the main window");
    m_currentDateUpdateTimer = new QTimer(this);
    //1.5秒更新当前时间
    m_currentDateUpdateTimer->start(1500);

    QFrame *titleframe = new QFrame(this);
    titleframe->setAccessibleName("TitleFrame");
    titleframe->setObjectName("titleframe");
    titleframe->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    CDynamicIcon::getInstance()->setTitlebar(this->titlebar());
    CDynamicIcon::getInstance()->setIcon();

    QStringList titlelist;
    titlelist << tr("Y") << tr("M") << tr("W") << tr("D");
    m_buttonBox = new DButtonBox(this);
    m_buttonBox->setObjectName("ButtonBox");
    m_buttonBox->setAccessibleName("ButtonBox");
    m_buttonBox->setAccessibleDescription("Year, month, week, day button box");

    m_yearButton = new DButtonBoxButton(tr("Y"), this);
    //设置年辅助技术显示名称
    m_yearButton->setObjectName("YearButton");
    m_yearButton->setAccessibleName("YearButton");

    QFont viewfont;
    viewfont.setWeight(QFont::Medium);
    viewfont.setPixelSize(DDECalendar::FontSizeFourteen);

    m_yearButton->setFixedSize(50, 36);
    m_monthButton = new DButtonBoxButton(tr("M"), this);
    //设置月辅助技术显示名称
    m_monthButton->setObjectName("MonthButton");
    m_monthButton->setAccessibleName("MonthButton");
    m_monthButton->setFixedSize(50, 36);
    m_weekButton = new DButtonBoxButton(tr("W"), this);
    //设置周辅助技术显示名称
    m_weekButton->setObjectName("WeekButton");
    m_weekButton->setAccessibleName("WeekButton");
    m_weekButton->setFixedSize(50, 36);
    m_dayButton = new DButtonBoxButton(tr("D"), this);
    //设置日辅助技术显示名称
    m_dayButton->setObjectName("DayButton");
    m_dayButton->setAccessibleName("DayButton");
    m_dayButton->setFixedSize(50, 36);
    m_yearButton->setFont(viewfont);
    m_monthButton->setFont(viewfont);
    m_weekButton->setFont(viewfont);
    m_dayButton->setFont(viewfont);

    QList<DButtonBoxButton *> btlist;
    btlist.append(m_yearButton);
    btlist.append(m_monthButton);
    btlist.append(m_weekButton);
    btlist.append(m_dayButton);
    m_buttonBox->setButtonList(btlist, true);

    m_buttonBox->setId(m_yearButton, DDECalendar::CalendarYearWindow);
    m_buttonBox->setId(m_monthButton, DDECalendar::CalendarMonthWindow);
    m_buttonBox->setId(m_weekButton, DDECalendar::CalendarWeekWindow);
    m_buttonBox->setId(m_dayButton, DDECalendar::CalendarDayWindow);
    m_buttonBox->setFixedSize(200, 36);

    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->setMargin(0);
    titleLayout->setSpacing(0);
    titleLayout->addSpacing(8);
    titleLayout->addWidget(m_buttonBox);
    titleLayout->addSpacing(52);
    m_searchEdit = new DSearchEdit(this);
    //设置搜索框辅助技术显示名称
    m_searchEdit->setObjectName("SearchEdit");
    m_searchEdit->setAccessibleName("SearchEdit");
    DFontSizeManager::instance()->bind(m_searchEdit, DFontSizeManager::T6);
    m_searchEdit->setFixedHeight(36);
    m_searchEdit->setMinimumWidth(240);
    m_searchEdit->setMaximumWidth(354);
    m_searchEdit->setFont(viewfont);
    titleLayout->addStretch();
    titleframe->setLayout(titleLayout);

    DTitlebar *titlebar = this->titlebar();
    titlebar->setObjectName("TitleBar");
    titlebar->setAccessibleName("TitleBar");
    titlebar->setFixedHeight(50);
    titlebar->addWidget(titleframe, Qt::AlignLeft | Qt::AlignVCenter);
    titlebar->setCustomWidget(m_searchEdit, true);
    //新建日程快捷按钮
    m_newScheduleBtn = new DToolButton(this);
    DStyle style;
    m_newScheduleBtn->setFixedSize(36, 36);
    //设置+
    m_newScheduleBtn->setIcon(style.standardIcon(DStyle::SP_IncreaseElement));
    titlebar->addWidget(m_newScheduleBtn, Qt::AlignRight);

    m_stackWidget = new AnimationStackedWidget();
    m_stackWidget->setObjectName("StackedWidget");
    m_stackWidget->setAccessibleName("StackedWidget");
    m_stackWidget->setAccessibleDescription("Container for year window, month window, week window and day window");
    m_stackWidget->setContentsMargins(0, 0, 0, 0);
    m_stackWidget->setDuration(250);
    createview();
    QHBoxLayout *tmainLayout = new QHBoxLayout;
    tmainLayout->setMargin(0);
    tmainLayout->setSpacing(0);
    tmainLayout->setContentsMargins(0, 0, 0, 0);
    tmainLayout->addWidget(m_stackWidget);
    m_contentBackground = new DFrame;
    m_contentBackground->setAccessibleName("ScheduleSearchWidgetBackgroundFrame");
    m_contentBackground->setObjectName("ScheduleSearchWidgetBackgroundFrame");
    m_contentBackground->setContentsMargins(0, 0, 0, 0);
    DPalette anipa = m_contentBackground->palette();
    anipa.setColor(DPalette::Background, "#F8F8F8");
    m_contentBackground->setAutoFillBackground(true);
    m_contentBackground->setPalette(anipa);

    m_scheduleSearchView = new CScheduleSearchView(this);
    m_scheduleSearchView->setObjectName("ScheduleSearchWidget");
    m_scheduleSearchView->setAccessibleName("ScheduleSearchWidget");
    m_scheduleSearchView->setAccessibleDescription("Window showing search results");

    QVBoxLayout *ssLayout = new QVBoxLayout;
    ssLayout->setMargin(0);
    ssLayout->setSpacing(0);
    ssLayout->setContentsMargins(0, 10, 0, 10);
    ssLayout->addWidget(m_scheduleSearchView);
    m_contentBackground->setLayout(ssLayout);
    tmainLayout->addWidget(m_contentBackground);
    m_contentBackground->setVisible(false);

    DWidget *maincentralWidget = new DWidget(this);
    maincentralWidget->setAccessibleName("mainCentralWidget");

    maincentralWidget->setLayout(tmainLayout);

    setCentralWidget(maincentralWidget);
    m_yearButton->setFocus();
    m_yearButton->setChecked(true);
    m_transparentFrame = new DFrame(this);
    m_transparentFrame->setAutoFillBackground(true);
    m_transparentFrame->hide();

    m_animation = new QPropertyAnimation(this);
}

void Calendarmainwindow::initConnection()
{
    connect(m_stackWidget, &AnimationStackedWidget::signalIsFinished, this, &Calendarmainwindow::slotSetButtonBox);
    connect(m_buttonBox, &DButtonBox::buttonClicked, this, &Calendarmainwindow::slotstackWClicked);
    connect(m_searchEdit, &DSearchEdit::returnPressed, this, &Calendarmainwindow::slotSreturnPressed);
    connect(m_searchEdit, &DSearchEdit::textChanged, this, &Calendarmainwindow::slotStextChanged);
    connect(m_searchEdit, &DSearchEdit::focusChanged, this, &Calendarmainwindow::slotStextfocusChanged);
    //监听当前应用主题切换事件
    connect(DGuiApplicationHelper::instance(), &DGuiApplicationHelper::themeTypeChanged, this, &Calendarmainwindow::slotTheme);
    //编辑搜索日程刷新界面
    connect(m_scheduleSearchView, &CScheduleSearchView::signalSelectSchedule, this, &Calendarmainwindow::slotSearchSelectSchedule);
    connect(m_scheduleSearchView, &CScheduleSearchView::signalScheduleHide, this, &Calendarmainwindow::setScheduleHide);
    //界面弹出对话框设置背景阴影
    connect(m_scheduleSearchView, &CScheduleSearchView::signalViewtransparentFrame, this, &Calendarmainwindow::slotViewtransparentFrame);
    connect(m_yearwindow, &CYearWindow::signalViewtransparentFrame, this, &Calendarmainwindow::slotViewtransparentFrame);
    connect(m_monthWindow, &CMonthWindow::signalViewtransparentFrame, this, &Calendarmainwindow::slotViewtransparentFrame);
    connect(m_weekWindow, &CWeekWindow::signalViewtransparentFrame, this, &Calendarmainwindow::slotViewtransparentFrame);
    connect(m_DayWindow, &CDayWindow::signalViewtransparentFrame, this, &Calendarmainwindow::slotViewtransparentFrame);
    //更新当前时间
    connect(m_currentDateUpdateTimer, &QTimer::timeout, this, &Calendarmainwindow::slotCurrentDateUpdate);
    //切换视图
    connect(m_yearwindow, &CYearWindow::signalSwitchView, this, &Calendarmainwindow::slotSwitchView);
    connect(m_monthWindow, &CMonthWindow::signalSwitchView, this, &Calendarmainwindow::slotSwitchView);
    connect(m_weekWindow, &CWeekWindow::signalSwitchView, this, &Calendarmainwindow::slotSwitchView);
    //按钮关联新建日程
    connect(m_newScheduleBtn, &DToolButton::clicked, this, &Calendarmainwindow::slotNewSchedule);
}

/**
 * @brief Calendarmainwindow::createview        创建视图
 */
void Calendarmainwindow::createview()
{
    m_yearwindow = new CYearWindow(this);
    m_yearwindow->updateData();
    m_stackWidget->addWidget(m_yearwindow);
    m_monthWindow = new CMonthWindow(this);
    m_monthWindow->updateData();
    m_stackWidget->addWidget(m_monthWindow);

    m_weekWindow  = new CWeekWindow(this);
    m_weekWindow->updateData();
    m_stackWidget->addWidget(m_weekWindow);

    m_DayWindow = new CDayWindow;
    m_DayWindow->updateData();
    m_stackWidget->addWidget(m_DayWindow);
}

/**
 * @brief Calendarmainwindow::setScheduleHide       隐藏提示框
 */
void Calendarmainwindow::setScheduleHide()
{
    m_yearwindow->slotSetScheduleHide();
    m_monthWindow->slotScheduleHide();
    m_weekWindow->slotScheduleHide();
    m_DayWindow->slotScheduleHide();
}

void Calendarmainwindow::resizeEvent(QResizeEvent *event)
{
    m_transparentFrame->resize(width(), height() - 50);
    m_scheduleSearchViewMaxWidth = qRound(0.2325 * width() + 0.5);
    m_scheduleSearchView->setMaxWidth(m_scheduleSearchViewMaxWidth);
    setSearchWidth(m_scheduleSearchViewMaxWidth);
    setScheduleHide();
    DMainWindow::resizeEvent(event);
    CConfigSettings::setOption("base.geometry", saveGeometry());
    CConfigSettings::setOption("base.state", int(windowState()));
}

/**
 * @brief Calendarmainwindow::slotstackWClicked 点击按钮切换视图
 * @param bt
 */
void Calendarmainwindow::slotstackWClicked(QAbstractButton *bt)
{
    m_buttonBox->setEnabled(false);
    setScheduleHide();
    int index = m_buttonBox->id(bt);
    viewWindow(index, true);
}

void Calendarmainwindow::slotWUpdateShcedule()
{
    if (m_opensearchflag && !m_searchEdit->text().isEmpty()) {
        m_scheduleSearchView->slotsetSearch(m_searchEdit->text());
    }
    updateHigh();
    return;
}

void Calendarmainwindow::slotSreturnPressed()
{
    if (!m_opensearchflag && !m_searchEdit->text().isEmpty()) {
        m_opensearchflag = true;
        m_contentBackground->setVisible(true);
    }
    m_scheduleSearchView->slotsetSearch(m_searchEdit->text());
    updateHigh();
}

void Calendarmainwindow::slotStextChanged()
{
    if (!m_searchEdit->text().isEmpty()) {
        m_yearwindow->setSearchWFlag(true);
        m_weekWindow->setSearchWFlag(true);
        m_monthWindow->setSearchWFlag(true);
        m_DayWindow->setSearchWFlag(true);
    } else {
        m_scheduleSearchView->clearSearch();
        m_yearwindow->setSearchWFlag(false);
        m_monthWindow->setSearchWFlag(false);
        m_weekWindow->setSearchWFlag(false);
        m_DayWindow->setSearchWFlag(false);
        m_contentBackground->setVisible(false);
        m_opensearchflag = false;
    }
    updateHigh();
}

/**
 * @brief Calendarmainwindow::slotStextfocusChanged     搜索框有焦点时隐藏提示框
 * @param onFocus
 */
void Calendarmainwindow::slotStextfocusChanged(bool onFocus)
{
    if (onFocus) {
        setScheduleHide();
    }
}

void Calendarmainwindow::slotSearchEdit()
{
    m_searchEdit->lineEdit()->setFocus();
}

/**
 * @brief Calendarmainwindow::slotSearchSelectSchedule  单击搜索日程动画设置
 * @param scheduleInfo
 */
void Calendarmainwindow::slotSearchSelectSchedule(const ScheduleDataInfo &scheduleInfo)
{
    //获取当前视图编号
    CScheduleBaseWidget *_showWidget = dynamic_cast<CScheduleBaseWidget *>(m_stackWidget->currentWidget());
    if (_showWidget) {
        //如果日程开始时间年份与选择时间年份不一样则切换年份显示
        bool changeYear = _showWidget->getSelectDate().year() != scheduleInfo.getBeginDateTime().date().year();
        //设置选择时间
        if (_showWidget->setSelectDate(scheduleInfo.getBeginDateTime().date(), changeYear)) {
            //更新显示数据
            _showWidget->updateData();
            //设置年份信息显示
            _showWidget->setYearData();
            //延迟150毫秒设置选中动画
            QTimer::singleShot(150, this, [ = ] {
                _showWidget->setSelectSearchScheduleInfo(scheduleInfo);
            });
        }
        //如果当前界面不为年试图则更新年视图数据
        if (_showWidget != m_yearwindow) {
            m_yearwindow->updateData();
        }
    }
}

/**
 * @brief Calendarmainwindow::slotViewtransparentFrame      添加视图阴影
 * @param isShow
 */
void Calendarmainwindow::slotViewtransparentFrame(const bool isShow)
{
    static int showFrameCount = 0;
    if (isShow) {
        m_transparentFrame->resize(width(), height() - 50);
        m_transparentFrame->move(0, 50);
        m_transparentFrame->show();
        ++showFrameCount;
    } else {
        if (showFrameCount == 1)
            m_transparentFrame->hide();
        --showFrameCount;
    }
    //获取当前视图
    int index = m_stackWidget->currentIndex();
    //设置焦点
    switch (index) {
    case DDECalendar::CalendarYearWindow: {
        m_yearwindow->setFocus();
    } break;
    case DDECalendar::CalendarMonthWindow: {
        m_monthWindow->setFocus();
    } break;
    case DDECalendar::CalendarWeekWindow: {
        m_weekWindow->setFocus();
    } break;
    case DDECalendar::CalendarDayWindow: {
        m_DayWindow->setFocus();
    } break;
    }
}

/**
 * @brief Calendarmainwindow::slotSetButtonBox      启用buttonbox
 */
void Calendarmainwindow::slotSetButtonBox()
{
    m_buttonBox->setEnabled(true);
}

/**
 * @brief Calendarmainwindow::slotSwitchView        切换视图
 * @param viewIndex         切换类型
 */
void Calendarmainwindow::slotSwitchView(const int viewIndex)
{
    // 0:跳转上一个视图  1：月视图  2：周视图  3:日视图
    switch (viewIndex) {
    case 0: {
        if (m_priindex == DDECalendar::CalendarYearWindow)
            m_priindex = DDECalendar::CalendarDayWindow;
        viewWindow(m_priindex);
        break;
    }
    case 1: {
        viewWindow(DDECalendar::CalendarMonthWindow);
        break;
    }
    case 2: {
        viewWindow(DDECalendar::CalendarWeekWindow);
        break;
    }
    case 3: {
        viewWindow(DDECalendar::CalendarDayWindow);
        break;
    }
    default:
        break;
    }
}

/**
 * @brief Calendarmainwindow::slotNewSchedule       全局新建日程
 */
void Calendarmainwindow::slotNewSchedule()
{
    //设置日程开始时间
    QDateTime _beginTime(m_yearwindow->getSelectDate(), QTime::currentTime());
    //新建日程对话框
    CScheduleDlg _scheduleDig(1, this, false);
    //设置开始时间
    _scheduleDig.setDate(_beginTime);
    _scheduleDig.exec();
}

void Calendarmainwindow::mouseMoveEvent(QMouseEvent *event)
{
    //如果为平板模式则不可移动
    if (DDECalendar::isTable) {
        Q_UNUSED(event);
    } else {
        DMainWindow::mouseMoveEvent(event);
        setScheduleHide();
    }
}

void Calendarmainwindow::changeEvent(QEvent *event)
{
    DMainWindow::changeEvent(event);
    if (event->type() == QEvent::ActivationChange) {
        setScheduleHide();
    }
}

void Calendarmainwindow::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    setScheduleHide();
}

