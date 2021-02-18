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
#include "ckeyenabledeal.h"
#include "cgraphicsscene.h"
#include "scheduledlg.h"
#include "graphicsItem/cscenebackgrounditem.h"
#include "graphicsItem/draginfoitem.h"
#include "myscheduleview.h"

#include <QGraphicsView>

CKeyEnableDeal::CKeyEnableDeal(QGraphicsScene *scene)
    : CKeyPressDealBase(Qt::Key_Return, scene)
{
}

/**
 * @brief CKeyEnableDeal::focusItemDeal     焦点项处理
 * @param item
 * @param scene
 * @return
 */
bool CKeyEnableDeal::focusItemDeal(CSceneBackgroundItem *item, CGraphicsScene *scene)
{
    bool result = false;
    CFocusItem *focusItem = item->getFocusItem();
    if (focusItem != nullptr) {
        result = true;
        QWidget *parentWidget = scene->views().at(0);
        switch (focusItem->getItemType()) {
        case CFocusItem::CBACK: {
            CSceneBackgroundItem *backgroundItem = dynamic_cast<CSceneBackgroundItem *>(focusItem);
            createSchedule(backgroundItem->getDate(), parentWidget);
        } break;
        case CFocusItem::CITEM: {
            DragInfoItem *scheduleItem = dynamic_cast<DragInfoItem *>(focusItem);
            CMyScheduleView dlg(scheduleItem->getData(), parentWidget);
            dlg.exec();
        } break;
        default:
            break;
        }
    }
    return result;
}

/**
 * @brief CKeyEnableDeal::createSchedule        创建日程
 * @param createDate
 * @param parent
 */
void CKeyEnableDeal::createSchedule(const QDate &createDate, QWidget *parent)
{
    CScheduleDlg dlg(1, parent);
    QDateTime setDate(createDate, QTime(0, 0, 0));
    dlg.setDate(setDate);
    dlg.setAllDay(true);
    dlg.exec();
}
