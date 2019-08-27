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

#include <QWidget>
#include <DDialog>
DWIDGET_USE_NAMESPACE

class VerticalScrollText;
class VerticalScroll;
#include <QTime>
class CCalendarTimeEidt : public QWidget
{
    Q_OBJECT

public:
    CCalendarTimeEidt(QWidget *parent);
    ~CCalendarTimeEidt();
    void setTime(QTime time);
    QString  getCrrentAMorPm();
    int      getHour();
    int      getMin();
signals:
    void currentTValueChanged(QString value);
    void currentHValueChanged(int value);
    void currentMValueChanged(int value);
protected:
    void focusOutEvent(QFocusEvent *event);
private:
    void initUI();
    void initConnection();
private:
    VerticalScrollText         *m_AmorPmW;
    VerticalScroll             *m_HourScroll;
    VerticalScroll             *m_MinScroll;
    QTime                      m_time;
};
