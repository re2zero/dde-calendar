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

#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace DDECalendar {
    static const int HeaderItemWidth = 80;
    static const int HeaderItemHeight = 40;

    static const int CellWidth = 106;
    static const int CellHeight = 70;

    static const int CellHighlightWidth = 80;
    static const int CellHighlightHeight = 70;
}
namespace DDEYearCalendar {
    static const int YHeaderItemWidth = 20;
    static const int YHeaderItemHeight = 16;

    static const int YCellWidth = 26;
    static const int YCellHeight = 20;

    static const int YCellHighlightWidth = 26;
    static const int YCellHighlightHeight = 20;
    static const int Y_MLableHeight = 24;
    static const int Y_YLableHeight = 45;
    static const int Y_MWindowWidth = 202;
    static const int Y_MWindowHeight = 162;
}
namespace DDEMonthCalendar {
    static const int MHeaderItemWidth = 60;
    static const int MHeaderItemHeight = 33;
    static const int MEventsItemHeight = 36;

    static const int MCellWidth = 120;
    static const int MCellHeight = 74;

    static const int MCellHighlightWidth = 120;
    static const int MCellHighlightHeight = 74;

    static const int MWeekCellWidth = 120;
    static const int MWeekCellHeight = 54;

    static const int MDayCellWidth = 36;
    static const int MDayCellHeight = 36;

    static const int M_YLableHeight = 36;
    static const int M_YLunatLabelWindth = 66;
    static const int M_YLunatLabelHeight = 20;

    static const int MTodayWindth = 100;
    static const int MTodayHeight = 36;

    static const int M_MWindowWidth = 202;
    static const int M_MWindowHeight = 162;


}
enum Week {
    Monday    = 6,
    Tuesday   = 5,
    Wednesday = 4,
    Thursday  = 3,
    Friday    = 2,
    Saturday  = 1,
    Sunday    = 0,
};

#endif // CONSTANTS_H
