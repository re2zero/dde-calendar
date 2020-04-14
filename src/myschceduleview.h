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

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <DDialog>
#include <DPushButton>
#include "schedulestructs.h"
DWIDGET_USE_NAMESPACE

class CMySchceduleView : public DDialog
{
    Q_OBJECT
public:
    explicit CMySchceduleView(const ScheduleDtailInfo &schduleInfo,QWidget *parent = nullptr);
    ~CMySchceduleView() Q_DECL_OVERRIDE;
    ScheduleDtailInfo getSchedules()
    {
        return  m_scheduleInfo;
    }
signals:
    void signalsEditorDelete(int type = 0);
    void signalViewtransparentFrame(int type);
public slots:
    void slotEditBt();
    void slotDeleteBt();
private:
    void initUI();
    void initConnection();
    void AutoFeed(QString text);
protected:
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
private:
    QLabel                           *m_schceduleLabel = nullptr;
    QLabel                           *m_timeLabel = nullptr;
    DPushButton                      *m_editBt = nullptr;
    DPushButton                      *m_deleteBt = nullptr;
    DPushButton                      *m_okBt = nullptr;
    QLabel                           *m_icon;
    QLabel                           *m_Title;
    ScheduleDtailInfo                     m_scheduleInfo;//日程

};

#endif // MYSCHCEDULEVIEW_H
