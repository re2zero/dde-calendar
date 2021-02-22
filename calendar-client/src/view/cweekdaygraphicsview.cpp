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
#include "cscenetabkeydeal.h"
#include "ckeyenabledeal.h"
#include "ckeyleftdeal.h"
#include "ckeyrightdeal.h"

CWeekDayGraphicsview::CWeekDayGraphicsview(QWidget *parent, ViewType edittype)
    : DragInfoGraphicsView(parent)
    , m_viewType(edittype)
    , m_coorManage(new CScheduleCoorManage)
    , m_rightmagin(0)
{
    createBackgroundItem();
    m_Scene->setFirstFocusItem(m_backgroundItem.first());
    //添加键盘事件处理
    CKeyPressPrxy *m_keyPrxy = new CKeyPressPrxy();
    m_keyPrxy->addkeyPressDeal(new CSceneTabKeyDeal(m_Scene));
    m_keyPrxy->addkeyPressDeal(new CKeyEnableDeal(m_Scene));
    m_keyPrxy->addkeyPressDeal(new CKeyLeftDeal(m_Scene));
    m_keyPrxy->addkeyPressDeal(new CKeyRightDeal(m_Scene));
    m_Scene->setKeyPressPrxy(m_keyPrxy);
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
    if (m_viewType == WeekView) {
        w -= 2;
    } else {
        w = w - rightmagin - 2;
    }

    m_coorManage->setRange(w, h, begindate, enddate, rightmagin);
    setSceneRect(0, 0, w, h);
    m_rightmagin = rightmagin;
}

void CWeekDayGraphicsview::setRange(QDate begin, QDate end)
{
    m_MoveDate.setDate(begin.addMonths(-2));
    m_beginDate = begin;
    m_endDate = end;
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
    if (m_viewType == DayView) {
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
            //设置编号
            item->setDrawDividingLine(true);
            //TODO 临时
            item->setShowFocus(true);
            item->setBackgroundNum(i);
            m_backgroundItem.append(item);
            m_Scene->addItem(item);
        }
    }
}
