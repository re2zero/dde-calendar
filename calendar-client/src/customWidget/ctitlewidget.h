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
#ifndef CTITLEWIDGET_H
#define CTITLEWIDGET_H

#include "cbuttonbox.h"

#include <QWidget>
#include <DSearchEdit>
#include <DIconButton>

DWIDGET_USE_NAMESPACE

class CTitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CTitleWidget(QWidget *parent = nullptr);

    DButtonBox *buttonBox() const;

    DSearchEdit *searchEdit() const;

    DIconButton *newScheduleBtn() const;

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *o, QEvent *e) override;
signals:
    void signalSetButtonFocus();
    void signalSearchFocusSwitch();
public slots:
private:
    CButtonBox *m_buttonBox {};
    DSearchEdit *m_searchEdit {};
    DIconButton *m_newScheduleBtn {}; //全局的新建日程按钮
};

#endif // CTITLEWIDGET_H
