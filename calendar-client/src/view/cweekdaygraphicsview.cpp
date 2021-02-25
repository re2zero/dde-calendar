/*
   * Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
   *
   * Author:     chenhaifeng <chenhaifeng@uniontech.com>
   *
   * Maintainer: chenhaifeng <chenhaifeng@uniontech.com>
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
#include "cweekdaygraphicsview.h"

#include "constants.h"
#include "schedulecoormanage.h"
#include "cweekdayscenetabkeydeal.h"
#include "ckeyenabledeal.h"
#include "ckeyleftdeal.h"
#include "ckeyrightdeal.h"

CWeekDayGraphicsview::CWeekDayGraphicsview(QWidget *parent, ViewPosition viewPos, ViewType viewtype)
    : DragInfoGraphicsView(parent)
    , m_viewPos(viewPos)
    , m_viewType(viewtype)
    , m_coorManage(new CScheduleCoorManage)
    , m_rightmagin(0)
{
    createBackgroundItem();
    m_Scene->setFirstFocusItem(m_backgroundItem.first());
    //添加键盘事件处理
    CKeyPressPrxy *m_keyPrxy = new CKeyPressPrxy();
    m_keyPrxy->addkeyPressDeal(new CWeekDaySceneTabKeyDeal(m_Scene));
    m_keyPrxy->addkeyPressDeal(new CKeyEnableDeal(m_Scene));
    m_keyPrxy->addkeyPressDeal(new CKeyLeftDeal(m_Scene));
    m_keyPrxy->addkeyPressDeal(new CKeyRightDeal(m_Scene));
    m_Scene->setKeyPressPrxy(m_keyPrxy);
    connect(m_Scene, &CGraphicsScene::signalSwitchView, this, &CWeekDayGraphicsview::slotSwitchView);
    connect(m_Scene, &CGraphicsScene::signalViewFocusInit, this, &CWeekDayGraphicsview::signalViewFocusInit);
}

CWeekDayGraphicsview::~CWeekDayGraphicsview()
{
    delete m_coorManage;
    m_coorManage = nullptr;
}

void CWeekDayGraphicsview::setRange(int w, int h, QDate begindate, QDate enddate, int rightmagin)
{
    m_MoveDate.setDate(begindate.addMonths(-2));
    m_beginDate = begindate;
    m_endDate = enddate;
    if (m_viewPos == WeekPos) {
        w -= 2;
    } else {
        w = w - rightmagin - 2;
    }
    setBackgroundDate();
    m_coorManage->setRange(w, h, begindate, enddate, rightmagin);
    setSceneRect(0, 0, w, h);
    m_rightmagin = rightmagin;
}

void CWeekDayGraphicsview::setRange(QDate begin, QDate end)
{
    m_MoveDate.setDate(begin.addMonths(-2));
    m_beginDate = begin;
    m_endDate = end;
    setBackgroundDate();
    getCoorManage()->setDateRange(begin, end);
    this->scene()->update();
}

void CWeekDayGraphicsview::setTheMe(int type)
{
    for (int i = 0; i < m_backgroundItem.size(); ++i) {
        m_backgroundItem.at(i)->setTheMe(type);
    }
    DragInfoGraphicsView::setTheMe(type);
    this->viewport()->update();
}

CScheduleCoorManage *CWeekDayGraphicsview::getCoorManage() const
{
    return m_coorManage;
}

void CWeekDayGraphicsview::setCurrentFocusItem(const QDate &focusDate)
{
    qint64 offset = m_backgroundItem.first()->getDate().daysTo(focusDate);
    if (offset >= 0 && offset < m_backgroundItem.size()) {
        m_Scene->setCurrentFocusItem(m_backgroundItem.at(static_cast<int>(offset)));
    } else {
        qWarning() << "set CurrentFocusItem Error,offset:" << offset << ",focusDate:" << focusDate << ",firstDate:" << m_backgroundItem.first()->getDate();
    }
}

void CWeekDayGraphicsview::setSceneRect(qreal x, qreal y, qreal w, qreal h)
{
    m_Scene->setSceneRect(x, y, w, h);
    const qreal backgroundItemHeight = h;
    const qreal backgroundItemWidth = w / m_backgroundItem.size();
    for (int i = 0; i < m_backgroundItem.size(); ++i) {
        m_backgroundItem.at(i)->setData(m_beginDate.addDays(i));
        m_backgroundItem.at(i)->setRect(x + backgroundItemWidth * i, 0, backgroundItemWidth, backgroundItemHeight);
    }
}

void CWeekDayGraphicsview::createBackgroundItem()
{
    if (m_viewPos == DayPos) {
        //日视图
        CWeekDayBackgroundItem *backgroundItem = new CWeekDayBackgroundItem();
        backgroundItem->setZValue(-1);
        m_backgroundItem.append(backgroundItem);
        m_Scene->addItem(backgroundItem);
    } else {
        //周视图
        for (int i = 0; i < DDEWeekCalendar::AFewDaysofWeek; ++i) {
            CWeekDayBackgroundItem *item = new CWeekDayBackgroundItem();
            item->setZValue(-1);
            if (m_backgroundItem.size() > 0) {
                //设置对应左右和下一个                            关系
                m_backgroundItem.last()->setNextFocusItem(item);
                m_backgroundItem.last()->setRightItem(item);
                item->setLeftItem(m_backgroundItem.last());
            }
            //设置背景直接分隔符
            item->setDrawDividingLine(true);
            //如果为非全天则设置背景焦点获取显示
            if (m_viewType == PartTimeView) {
                item->setShowFocus(true);
            }
            //设置编号
            item->setBackgroundNum(i);
            m_backgroundItem.append(item);
            m_Scene->addItem(item);
        }
    }
}

/**
 * @brief CWeekDayGraphicsview::setBackgroundDate       设置背景时间
 */
void CWeekDayGraphicsview::setBackgroundDate()
{
    for (int i = 0; i < m_backgroundItem.size(); ++i) {
        m_backgroundItem.at(i)->setData(m_beginDate.addDays(i));
    }
}

void CWeekDayGraphicsview::slotSwitchView(const QDate &focusDate)
{
    if (m_viewType == ALLDayView) {
        emit signaleSwitchToView(focusDate, PartTimeView);
    } else {
        emit signaleSwitchToView(focusDate, ALLDayView);
    }
}

void CWeekDayGraphicsview::slotViewInit()
{
    m_Scene->currentItemInit();
}
