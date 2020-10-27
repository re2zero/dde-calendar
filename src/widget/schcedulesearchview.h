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
#ifndef SCHCEDULESEARCHVIEW_H
#define SCHCEDULESEARCHVIEW_H

#include "schedulestructs.h"

#include <DLabel>
#include <DPushButton>
#include <DMenu>
#include <DListWidget>

DWIDGET_USE_NAMESPACE
class QVBoxLayout;
class CScheduleListWidget;
class CSchceduleSearchItem;
class CSchceduleSearchDateItem;
/**
 * @brief The CSchceduleSearchView class
 */
class CSchceduleSearchView : public DWidget
{
    Q_OBJECT
public:
    CSchceduleSearchView(QWidget *parent = nullptr);
    ~CSchceduleSearchView() override;
    void setTheMe(int type = 0);
    void clearSearch();
    void setMaxWidth(const int w);
signals:
    void signalsUpdateShcedule(int id = 0);
    void signalDate(QDate date);
    void signalSelectSchedule(const ScheduleDtailInfo &scheduleInfo);
    void signalViewtransparentFrame(int type);
    void signalScheduleHide();
public slots:
    void slotdeleteitem(CSchceduleSearchItem *item);
    void slotedititem(CSchceduleSearchItem *item);
    void slotsetSearch(QString str);
    void slotSelectDate(QDate date);
    void slotSelectSchedule(const ScheduleDtailInfo &scheduleInfo);
protected:
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
private:
    void updateDateShow();
    void createItemWidget(ScheduleDtailInfo info, QDate date, int rtype);
    QListWidgetItem *createItemWidget(QDate date);
private:
    //下拉列表窗
    CScheduleListWidget *m_gradientItemList = nullptr;
    bool                                         m_widgetFlag;
    //日程信息
    QVector<ScheduleDateRangeInfo>               m_vlistData;
    QVector<DLabel *> m_labellist;
    int                                          m_type;
    QDate                                        m_currentDate;
    //背景色
    QColor                m_bBackgroundcolor = "#000000";
    //时间颜色
    QColor                m_btimecolor = "#526A7F";
    //文字颜色
    QColor                m_bttextcolor = "#414D68";
    QColor                m_lBackgroundcolor = Qt::white;
    QColor                m_ltextcolor = "#001A2E";
    QListWidgetItem *m_currentItem = nullptr;
    int m_maxWidth = 200;
};
/**
 * @brief The CScheduleListWidget class
 */
class CScheduleListWidget : public DListWidget
{
    Q_OBJECT
public:
    CScheduleListWidget(QWidget *parent = nullptr);
    ~CScheduleListWidget() override;
signals:
    void signalListWidgetScheduleHide();
protected:
    void mousePressEvent(QMouseEvent *event) override;
};
/**
 * @brief The CSchceduleSearchItem class
 */
class CSchceduleSearchItem : public DLabel
{
    Q_OBJECT
    /**
     * @brief The MouseStatus enum 鼠标状态
     *
     * M_NONE default状态
     * M_PRESS 鼠标点击
     * M_HOVER hover状态
     */
    enum MouseStatus {M_NONE,M_PRESS,M_HOVER};
public:
    explicit CSchceduleSearchItem(QWidget *parent = nullptr);
    void setBackgroundColor(QColor color1);
    void setSplitLineColor(QColor color1);
    void setText(QColor tcolor, QFont font);
    void setTimeC(QColor tcolor, QFont font);
    void setData(ScheduleDtailInfo  vScheduleInfo, QDate date);
    void setRoundtype(int rtype);
    void setTheMe(int type = 0);
    /**
     * @brief getData 获取日程信息
     * @return 日程信息
     */
    const ScheduleDtailInfo &getData() const
    {
        return m_ScheduleInfo;
    }
signals:
    void signalsDelete(CSchceduleSearchItem *item);
    void signalsEdit(CSchceduleSearchItem *item);
    void signalSelectDate(QDate date);
    void signalSelectSchedule(const ScheduleDtailInfo &scheduleInfo);
    void signalViewtransparentFrame(int type);
public slots:
    void slotEdit();
    void slotDelete();
    void slotDoubleEvent(int type = 0);
protected:
    void paintEvent(QPaintEvent *e) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    bool eventFilter(QObject *o, QEvent *e) override;

private:
    /**
     * @brief The ColorStatus struct 颜色种类
     *
     * background 背景色
     * timeColor 时间颜色
     * textColor 文字颜色
     */
    struct ColorStatus {
        QColor   background;
        QColor   timeColor;
        QColor   textColor;
    };
    //日程信息
    ScheduleDtailInfo m_ScheduleInfo;
    //“编辑”
    QAction *m_editAction = nullptr;
    //“删除”
    QAction *m_deleteAction = nullptr;
    //背景色
    QColor                m_Backgroundcolor;
    /**
     * @brief m_presscolor 颜色种类
     *
     * 鼠标点击是显示的颜色
     */
    ColorStatus m_presscolor;
    /**
     * @brief m_hovercolor 颜色种类
     *
     * 鼠标hover状态种类
     */
    ColorStatus m_hovercolor;
    //时间颜色
    QColor                m_timecolor;
    //竖线颜色
    QColor                m_splitlinecolor;
    //时间字体font
    QFont                 m_timefont;
    //文字颜色
    QColor                m_ttextcolor;
    //字体font
    QFont                 m_tfont;
    QDate                 m_date;
    /**
     * @brief m_mouseStatus 鼠标状态
     *
     * 根据鼠标状态显示不同颜色
     */
    MouseStatus m_mouseStatus;
    /**
     * @brief m_rightMenu 菜单
     *
     * 鼠标右键点击搜索日程，弹出菜单
     */
    DMenu *m_rightMenu = nullptr;
    /**
     * @brief m_roundtype 日程标签类型
     *
     * 每天的日程数量不同，圆角显示位置不同
     * 根据类型设置四个角是否为圆角
     */
    int m_roundtype = 1;
    //圆角半径
    const int m_radius = 8;
    const int m_borderframew = 0;
};
/**
 * @brief The CSchceduleSearchDateItem class
 */
class CSchceduleSearchDateItem : public DLabel
{
    Q_OBJECT
public:
    explicit CSchceduleSearchDateItem(QWidget *parent = nullptr);
    void setBackgroundColor(QColor color1);
    void setText(QColor tcolor, QFont font);
    void setDate(QDate  date);
signals:
    void signalLabelScheduleHide();
protected:
    void paintEvent(QPaintEvent *e) override;
    void mousePressEvent(QMouseEvent *event) override;
private:
    //背景色
    QColor                m_Backgroundcolor;
    //文字颜色
    QColor                m_textcolor;
    //字体font
    QFont                 m_font;
    QDate                 m_date;
};
#endif // CSHCEDULEDAYVIEW_H
