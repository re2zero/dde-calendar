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

#include <DComboBox>
#include <DLineEdit>
#include <DListWidget>

#include <QTime>
#include <QTimeEdit>

DWIDGET_USE_NAMESPACE

class TimeEdit : public QTimeEdit
{
    Q_OBJECT
public:
    TimeEdit(QWidget *parent = nullptr);
    QLineEdit *getLineEdit();
signals:
    void signalFocusOut();
protected:
    void focusOutEvent(QFocusEvent *event) override;
};

class CTimeEdit : public DComboBox
{
    Q_OBJECT
public:
    explicit CTimeEdit(QWidget *parent = nullptr);
    ~CTimeEdit() override;
    void setTime(QTime time);
    QTime getTime();
    //根据当前编辑框设置下拉选中item
    void setSelectItem();
public slots:
    void setTimeFormat(int value);
signals:
    void signalFocusOut();
private:
    void initUI();
    void initConnection();
protected:
    void showPopup() override;
    void focusInEvent(QFocusEvent *event) override;
private:
    QString m_timeFormat = "hh:mm";
    int m_timeFormatValue = 0;
    QTime m_time;
    QStringList m_strList;
    TimeEdit *m_timeEdit = nullptr;
};
