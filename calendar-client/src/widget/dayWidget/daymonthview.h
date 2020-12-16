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
#ifndef DAYMONTHVIEW_H
#define DAYMONTHVIEW_H
#include "customframe.h"
#include "src/dbusdatastruct.h"

#include <DWidget>
#include <DImageButton>
#include <DPushButton>
#include <DFrame>
#include <DIconButton>

#include <QList>
#include <QDate>
#include <QStyleOption>
#include <QGridLayout>
#include <QVBoxLayout>

DWIDGET_USE_NAMESPACE
class QLabel;
class CDayHuangLiLabel;
class CTodayButton;
class CDayMonthView: public CustomFrame
{
    Q_OBJECT

public:
    explicit CDayMonthView(QWidget *parent = nullptr);
    ~CDayMonthView() override;
    //设置显示时间,选择时间和当前时间
    void setShowDate(const QVector<QDate> &showDate, const QDate &selectDate, const QDate &currentDate);
    void setLunarVisible(bool visible);
    void setTheMe(int type = 0);
    void setSearchFlag(bool flag);
    //设置黄历信息
    void setHuangliInfo(const CaHuangLiDayInfo &huangliInfo);
    void setHasScheduleFlag(const QVector<bool> &hasScheduleFlag);
signals:
    //判断是否为拖拽
    void signalIsDragging(bool &isDragging);
    //选择时间改变信号
    void signalChangeSelectDate(const QDate &date);
public slots:
    //点击月显示时间修改选择时间
    void cellClicked(QWidget *cell);
    //设置选择项修改选择时间
    void setSelectedCell(int index);
    //上一个月
    void slotprev();
    //下一个月
    void slotnext();
    //返回当前时间
    void slottoday();
private:
    const QString getCellDayNum(int pos);
    const QDate getCellDate(int pos);
    void paintCell(QWidget *cell);
    bool eventFilter(QObject *o, QEvent *e) override;
    void initUI();
    void initConnection();
    void updateDateShow();
    void updateDateLunarDay();
private slots:

private:
    void changeSelectDate(const QDate &date);
protected:
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void paintEvent(QPaintEvent *e) override;
private:
    DIconButton      *m_prevButton = nullptr;
    DIconButton      *m_nextButton = nullptr;
    CTodayButton *m_today = nullptr;
    CustomFrame *m_currentMouth = nullptr;
    CustomFrame *m_currentDay = nullptr;
    CustomFrame *m_currentYear = nullptr;
    CustomFrame *m_currentWeek = nullptr;
    CustomFrame *m_currentLuna = nullptr;
    CDayHuangLiLabel *m_yiLabel = nullptr;
    CDayHuangLiLabel *m_jiLabel = nullptr;
    QList<QWidget *> m_cellList;
    QVector<QDate> m_showDays;
    QDate m_selectDate;
    QDate m_currentDate;
    int m_selectedCell = 0;
    QFont m_dayNumFont;

    QColor m_topBorderColor = Qt::red;
    QColor m_backgroundCircleColor = "#2ca7f8";
    QColor m_defaultTextColor = Qt::black;
    QColor m_currentDayTextColor = "#2ca7f8";
    QColor m_weekendsTextColor = Qt::black;
    QColor m_selectedTextColor = Qt::white;
    QColor m_festivalTextColor = Qt::black;
    QColor m_notCurrentTextColor = "#b2b2b2";

    int m_firstWeekDay;
    bool m_huanglistate = true;
    QGridLayout *m_gridLayout = nullptr;
    int cellwidth = 20;
    int cellheight = 20;
    QVBoxLayout *m_hhLayout = nullptr;
    QVBoxLayout *m_upLayout = nullptr;
    DHorizontalLine *m_splitline = nullptr;
    QVBoxLayout *m_yidownLayout = nullptr;
    QVBoxLayout *m_jidownLayout = nullptr;
    QStringList m_weeklist;
    CaHuangLiDayInfo    m_huangliInfo;

    int                   m_themetype  = 1;
    const int m_radius = 8;
    bool m_searchflag = false;
    QColor m_ceventColor = "#FF5D00";
    QVector<bool> m_vlineflag; //节假日和日程标识
};

#endif // YEARVIEW_H
