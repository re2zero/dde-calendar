/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
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
#ifndef MONTHDAYVIEW_H
#define MONTHDAYVIEW_H

#include "../touchgestureoperation.h"

#include <DFrame>

#include <QObject>
#include <QDate>

DWIDGET_USE_NAMESPACE

class CMonthWidget;
class CMonthDayView : public DFrame
{
    Q_OBJECT
public:
    explicit CMonthDayView(QWidget *parent = nullptr);
    ~CMonthDayView();
    void setCurrentDate(const QDate date);
    void setRCurrentDate(const QDate date);
    void setTheMe(int type = 0);
    void setsearchfalg(bool flag);
protected:
    void wheelEvent(QWheelEvent *e) override;
    bool event(QEvent *e) override;
signals:
    void signalsSelectDate(QDate date);
    void signalsCurrentDate(QDate date);
    /**
     * @brief signalAngleDelta      发送滚动信号滚动相对量
     * @param delta     滚动相对量
     */
    void signalAngleDelta(int delta);
private:
    /**
     * @brief m_touchGesture        触摸手势处理
     */
    touchGestureOperation m_touchGesture;
    CMonthWidget *m_monthWidget = nullptr;
    QDate                       m_selectDate;
    QDate                       m_days[12];

    int                         m_fixwidth = 200;
    int                         m_realwidth = 100;
    bool                        m_searchfalg = false;
};

class CMonthRect;
class CMonthWidget : public QWidget
{
    Q_OBJECT
public:
    CMonthWidget(QWidget *parent = nullptr);
    ~CMonthWidget() override;
    void setDate(const QDate date[12]);
protected:
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    /**
     * @brief mouseReleaseEvent 鼠标释放事件
     * @param event 鼠标事件
     */
    void mouseReleaseEvent(QMouseEvent *event) override;
    /**
     * @brief mouseMoveEvent 鼠标移动事件，设置hover状态
     * @param event 鼠标事件
     */
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void mousePress(const QPoint &point);
    void updateSize();
    int getMousePosItem(const QPointF &pos);
signals:
    void signalsSelectDate(QDate date);
private:
    QVector<CMonthRect *> m_MonthItem;
    QDate                       m_days[12];
    //触摸状态 0：原始  1：点击  2：移动
    int m_touchState {0};
    //触摸点击坐标
    QPoint m_touchBeginPoint;
};

class CMonthRect
{
public:
    CMonthRect();
    void setDate(const QDate &date);
    QDate getDate()const;
    QRectF rect() const;
    void setRect(const QRectF &rect);
    inline void setRect(qreal x, qreal y, qreal w, qreal h);
    void paintItem(QPainter *painter, const QRectF &rect);
    void setDevicePixelRatio(const qreal pixel);
    static void setTheMe(int type);
    static void setSelectRect(CMonthRect *selectRect);
private:
    QRectF                              m_rect;
    QDate                               m_Date;
    static int                          m_themetype ;
    static qreal                        m_DevicePixelRatio;

    static QColor                       m_defaultTextColor;
    static QColor                       m_backgrounddefaultColor;
    static QColor                       m_currentDayTextColor;
    static QColor                       m_backgroundcurrentDayColor;
    static QColor                       m_fillColor;
    static QFont                        m_dayNumFont;
    static CMonthRect                  *m_SelectRect;
    QColor                              m_selectColor;
};

#endif // MONTDAYVIEW_H
