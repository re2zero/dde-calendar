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
#include "ctitlewidget.h"

#include "constants.h"

#include <DFontSizeManager>

#include <QHBoxLayout>

CTitleWidget::CTitleWidget(QWidget *parent)
    : QWidget(parent)
{
    m_buttonBox = new CButtonBox(this);
    m_buttonBox->setObjectName("ButtonBox");
    m_buttonBox->setAccessibleName("ButtonBox");
    m_buttonBox->setAccessibleDescription("Year, month, week, day button box");

    DButtonBoxButton *m_yearButton = new DButtonBoxButton(tr("Y"), this);
    //设置年辅助技术显示名称
    m_yearButton->setObjectName("YearButton");
    m_yearButton->setAccessibleName("YearButton");
    m_yearButton->setFocusPolicy(Qt::TabFocus);

    QFont viewfont;
    viewfont.setWeight(QFont::Medium);
    viewfont.setPixelSize(DDECalendar::FontSizeFourteen);

    m_yearButton->setFixedSize(50, 36);
    DButtonBoxButton *m_monthButton = new DButtonBoxButton(tr("M"), this);
    //设置月辅助技术显示名称
    m_monthButton->setObjectName("MonthButton");
    m_monthButton->setAccessibleName("MonthButton");
    m_monthButton->setFixedSize(50, 36);
    m_monthButton->setFocusPolicy(Qt::TabFocus);
    DButtonBoxButton *m_weekButton = new DButtonBoxButton(tr("W"), this);
    //设置周辅助技术显示名称
    m_weekButton->setObjectName("WeekButton");
    m_weekButton->setAccessibleName("WeekButton");
    m_weekButton->setFixedSize(50, 36);
    m_weekButton->setFocusPolicy(Qt::TabFocus);
    DButtonBoxButton *m_dayButton = new DButtonBoxButton(tr("D"), this);
    //设置日辅助技术显示名称
    m_dayButton->setObjectName("DayButton");
    m_dayButton->setAccessibleName("DayButton");
    m_dayButton->setFixedSize(50, 36);
    m_dayButton->setFocusPolicy(Qt::TabFocus);
    m_yearButton->setFont(viewfont);
    m_monthButton->setFont(viewfont);
    m_weekButton->setFont(viewfont);
    m_dayButton->setFont(viewfont);

    QList<DButtonBoxButton *> btlist;
    btlist.append(m_yearButton);
    btlist.append(m_monthButton);
    btlist.append(m_weekButton);
    btlist.append(m_dayButton);
    m_buttonBox->setButtonList(btlist, true);

    m_buttonBox->setId(m_yearButton, DDECalendar::CalendarYearWindow);
    m_buttonBox->setId(m_monthButton, DDECalendar::CalendarMonthWindow);
    m_buttonBox->setId(m_weekButton, DDECalendar::CalendarWeekWindow);
    m_buttonBox->setId(m_dayButton, DDECalendar::CalendarDayWindow);
    m_buttonBox->setFixedSize(200, 36);

    m_searchEdit = new DSearchEdit(this);
    //设置搜索框辅助技术显示名称
    m_searchEdit->setObjectName("SearchEdit");
    m_searchEdit->setAccessibleName("SearchEdit");
    DFontSizeManager::instance()->bind(m_searchEdit, DFontSizeManager::T6);
    m_searchEdit->setFixedHeight(36);
    //    m_searchEdit->setMinimumWidth(240);
    //    m_searchEdit->setMaximumWidth(354);
    m_searchEdit->setFont(viewfont);

    //新建日程快捷按钮
    m_newScheduleBtn = new DIconButton(this);
    DStyle style;
    m_newScheduleBtn->setFixedSize(36, 36);
    //设置+
    m_newScheduleBtn->setIcon(style.standardIcon(DStyle::SP_IncreaseElement));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_buttonBox, Qt::AlignLeft);
    layout->addStretch();
    layout->addWidget(m_searchEdit, Qt::AlignCenter);
    layout->addStretch();
    layout->addWidget(m_newScheduleBtn, Qt::AlignRight);
    this->setLayout(layout);
    //设置焦点代理为buttonBox
    setFocusProxy(m_buttonBox);
}

DButtonBox *CTitleWidget::buttonBox() const
{
    return m_buttonBox;
}

DSearchEdit *CTitleWidget::searchEdit() const
{
    return m_searchEdit;
}

DIconButton *CTitleWidget::newScheduleBtn() const
{
    return m_newScheduleBtn;
}

void CTitleWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    int padding = qMax(m_buttonBox->width(), m_newScheduleBtn->width());
    //更加widget宽度设置搜索框宽度
    int searchWidth = width() - 2 * padding;
    //参考其他应用设置宽度范围为 240～354
    if (searchWidth < 240) {
        searchWidth = 240;
    } else if (searchWidth > 354) {
        searchWidth = 354;
    }
    m_searchEdit->setFixedWidth(searchWidth);
}
