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
#ifndef MYSCHCEDULEVIEW_H
#define MYSCHCEDULEVIEW_H

#include "schedulestructs.h"

#include <DDialog>
#include <DPushButton>
#include <DSuggestButton>
#include <DTextEdit>

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>

DWIDGET_USE_NAMESPACE
/**
 * @brief The CMySchceduleView class
 */
class CMySchceduleView : public DDialog
{
    Q_OBJECT
public:
    explicit CMySchceduleView(const ScheduleDtailInfo &schduleInfo,QWidget *parent = nullptr);
    ~CMySchceduleView() override;
    /**
     * @brief getSchedules 获取日程信息
     * @return 日程信息
     */
    ScheduleDtailInfo getSchedules()
    {
        return  m_scheduleInfo;
    }
signals:
    void signalsEditorDelete(int type = 0);
    void signalViewtransparentFrame(int type);
public slots:
    void slotEditBt(int buttonIndex, QString buttonName);
    void slotDeleteBt(int buttonIndex, QString buttonName);
private:
    void initUI();
    void initConnection();
    void AutoFeed(QString text);
    void setLabelTextColor(const int type);
    void setPaletteTextColor(QWidget *widget,QColor textColor);
protected:
    void showEvent(QShowEvent *event) override;
    bool eventFilter(QObject *o, QEvent *e) override;
private:
    /**
     * @brief m_schceduleLabel label
     *
     * 日程详情label
     */
    QLabel  *m_schceduleLabel = nullptr;
    /**
     * @brief m_timeLabel label
     *
     * 日程时间label
     */
    QLabel  *m_timeLabel = nullptr;
    /**
     * @brief m_editBt button
     *
     * “编辑”按钮
     */
    DSuggestButton *m_editBt = nullptr;
    /**
     * @brief m_deleteBt button
     *
     * "删除“按钮
     */
    DPushButton *m_deleteBt = nullptr;
    /**
     * @brief m_okBt button
     *
     * ok按钮
     */
    DPushButton *m_okBt = nullptr;
    /**
     * @brief m_icon label
     *
     * 图标
     */
    QLabel *m_icon = nullptr;
    /**
     * @brief m_Title label
     *
     * title
     */
    QLabel *m_Title = nullptr;
    //日程
    ScheduleDtailInfo m_scheduleInfo;
    /**
     * @brief area 滚动视图区域
     *
     * 日程详情区域
     */
    QScrollArea *area = nullptr;
    /**
     * @brief labelF font
     *
     * 字体
     */
    QFont labelF;
};

#endif // MYSCHCEDULEVIEW_H
