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
#pragma once

#include "ccustomtimeedit.h"

#include <DComboBox>
#include <DLineEdit>
#include <DListWidget>

#include <QTime>
#include <QTimeEdit>

DWIDGET_USE_NAMESPACE

class CTimeEdit : public DComboBox
{
    Q_OBJECT
public:
    explicit CTimeEdit(QWidget *parent = nullptr);
    ~CTimeEdit() override;
    void setTime(QTime time);
    QTime getTime();
public slots:
    void setTimeFormat(int value);
    //焦点获取效果绘制
    void slotFocusDraw(bool showFocus);
signals:
    void signalFocusOut();
private:
    void initUI();
    void initConnection();
protected:
    void showPopup() override;
    void focusInEvent(QFocusEvent *event) override;
    void paintEvent(QPaintEvent *e) override;

private:
    QString m_timeFormat = "hh:mm";
    int m_timeFormatValue = 0;
    QTime m_time;
    QStringList m_strList;
    CCustomTimeEdit *m_timeEdit = nullptr;
    bool m_hasFocus;
};
