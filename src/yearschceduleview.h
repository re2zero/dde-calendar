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
#ifndef YEARSCHCEDULEVIEW_H
#define YEARSCHCEDULEVIEW_H

#include <DLabel>
#include <DPushButton>
#include "schedulestructs.h"
#include <DListWidget>
#include <DWidget>
DWIDGET_USE_NAMESPACE
class QVBoxLayout;
class CYearSchceduleItem;
class CYearSchceduleView : public DWidget
{
    Q_OBJECT

public:
    CYearSchceduleView(QWidget *parent = nullptr);
    ~CYearSchceduleView();
    void setSoloDay(QString soloday);
    void setData(QVector<ScheduleDtailInfo> &vListData);
    void clearData();
    void showWindow();
    void setTheMe(int type = 0);
    void setDtype(int type, int arrowheight);
private:
    void updateDateShow();
    void createItemWidget(ScheduleDtailInfo info, int type = 0);
protected:
    void paintEvent(QPaintEvent *event);
private:
    DListWidget                                 *m_gradientItemList; //下拉列表窗
    bool                                         m_widgetFlag;
    QVector<ScheduleDtailInfo>                   m_vlistData;
    QString                                      m_soloDay;
    QVector<DLabel *>                             m_labellist;
    int                                          m_type;
    QDate                                        m_currentDate;
    QColor                m_bBackgroundcolor = "#000000";
    QColor                m_btimecolor = "#526A7F";
    QColor                m_bttextcolor = "#414D68";
    QColor                m_lBackgroundcolor = Qt::white;
    QColor                m_ltextcolor = "#001A2E";
    QColor                m_solocolor = "#001A2E";
    QColor                m_TBcolor = "#001A2E";
    int                   m_dtype = 3;
    int                   m_arrowheight = 0;
    QColor                m_borderColor = "#000000";
};

class CYearSchceduleItem : public DLabel
{
    Q_OBJECT

public:
    explicit CYearSchceduleItem(QWidget *parent = nullptr);
    void setBackgroundColor(QColor color1);
    void setStateColor(QColor color1);
    void setText(QColor tcolor, QFont font);
    void setTimeC(QColor tcolor, QFont font);
    void setData(ScheduleDtailInfo  vScheduleInfo);
protected:
    void paintEvent ( QPaintEvent *e);
private:
    ScheduleDtailInfo          m_ScheduleInfo;
    QColor                m_Backgroundcolor;
    QColor                m_timecolor;
    QColor                m_Statecolor;
    QFont                 m_timefont;
    QColor                m_ttextcolor;
    QFont                 m_tfont;
};
#endif // CSHCEDULEDAYVIEW_H

