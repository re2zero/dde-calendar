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
#ifndef MYSCHEDULEVIEW_H
#define MYSCHEDULEVIEW_H

#include "src/scheduledatainfo.h"
#include "dcalendarddialog.h"

#include <DTextEdit>

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>

DWIDGET_USE_NAMESPACE
/**
 * @brief The CMyScheduleView class
 * 日程概括对话框
 */
class CMyScheduleView : public DCalendarDDialog
{
    Q_OBJECT
public:
    explicit CMyScheduleView(const ScheduleDataInfo &schduleInfo, QWidget *parent = nullptr);
    ~CMyScheduleView() override;
    ScheduleDataInfo getSchedules()
    {
        return  m_scheduleInfo;
    }
signals:
    void signalsEditorDelete(int type = 0);
    void signalViewtransparentFrame(int type);
public slots:
    //按钮点击事件
    void slotBtClick(int buttonIndex, QString buttonName);
private:
    //界面初始化
    void initUI();
    void initConnection();
    //字体改变更改界面显示
    void AutoFeed(QString text);
    //设置label文字颜色
    void setLabelTextColor(const int type);
    //设置调色板颜色
    void setPaletteTextColor(QWidget *widget, QColor textColor);
protected:
    void showEvent(QShowEvent *event) override;
    bool eventFilter(QObject *o, QEvent *e) override;
private:
    QLabel *m_scheduleLabel = nullptr;
    QLabel *m_timeLabel = nullptr;
    QLabel *m_Title = nullptr;
    ScheduleDataInfo m_scheduleInfo; //日程
    QScrollArea *area = nullptr;
    QFont labelF;
};

#endif // MYSCHEDULEVIEW_H
