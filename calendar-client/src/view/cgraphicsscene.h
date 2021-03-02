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
#ifndef CGRAPHICSSCENE_H
#define CGRAPHICSSCENE_H

#include "ckeypressprxy.h"

#include <QGraphicsScene>
/**
 * @brief The CGraphicsScene class
 *
 */
class CGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    enum SceneType { MonthScene,
                     AllDayScene,
                     PartTimeScene
    }; //月场景,全天场景,非全天场景
public:
    explicit CGraphicsScene(QObject *parent = nullptr);
    ~CGraphicsScene() override;
    //设置Scene的第一个focus item
    void setFirstFocusItem(QGraphicsItem *item);
    //获取Scene的第一个focus item
    QGraphicsItem *getFirstFocusItem() const;
    //设置Scene当前focus item
    void setCurrentFocusItem(QGraphicsItem *item);
    //获取Scene当前focus item
    QGraphicsItem *getCurrentFocusItem() const;
    //设置keypress处理
    void setKeyPressPrxy(CKeyPressPrxy *keyPrxy);
    //更新当前item焦点效果显示
    void currentFocusItemUpdate();
    //设置上一页时间
    void setPrePage(const QDate &focusDate);
    //设置下一页时间
    void setNextPage(const QDate &focusDate);
    //获取是否为主动切换
    bool getActiveSwitching() const;
    //设置是否为主动切换
    void setActiveSwitching(bool activeSwitching);
    //获取场景类型
    SceneType getSceneType() const;
    //设置场景类型
    void setSceneType(const SceneType &sceneType);
    void currentItemInit();

    bool getIsShowCurrentItem() const;
    void setIsShowCurrentItem(bool isShowCurrentItem);

protected:
    bool event(QEvent *event) override;
private:
    bool focusInDeal(QEvent *event);
    bool focusOutDeal(QEvent *event);
signals:
    void signalSwitchPrePage(const QDate &focusDate);
    void signalSwitchNextPage(const QDate &focusDate);
    void signalSwitchView(const QDate &focusDate, bool setItemFocus = false);
    //通知另一个视图初始化
    void signalViewFocusInit();
    //切换到日视图
    void signalGotoDayView(const QDate &date);

private:
    QGraphicsItem *firstfocusItem;
    QGraphicsItem *currentFocusItem;
    CKeyPressPrxy *m_keyPrxy;
    bool m_activeSwitching; //是否为主动切换焦点
    SceneType m_sceneType;
    bool m_isShowCurrentItem; //true 当前item获取焦点  false 当前item的下一个item获取焦点
};

#endif // CGRAPHICSSCENE_H
