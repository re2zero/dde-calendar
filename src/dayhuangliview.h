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
#ifndef DAYHUANGLILABEL_H
#define DAYHUANGLILABEL_H

#include <DLabel>

DWIDGET_USE_NAMESPACE
class CDayHuangLiLabel : public DLabel
{
    Q_OBJECT

public:
    explicit CDayHuangLiLabel(QWidget *parent = nullptr);
    void setbackgroundColor(QColor backgroundColor);
    void setTextInfo(QColor tcolor, QFont font);
    void setHuangLiText(QStringList vhuangli, int type = 0);
protected:
    void paintEvent(QPaintEvent *e) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QStringList           m_vHuangli;
    QColor                m_backgroundColor;
    QColor                m_textcolor;
    QFont                 m_font;
    int                   m_type  = 0;
    int                   m_leftMagin = 14;
    int                   m_topMagin = 18;
};

#endif // DAYHUANGLILABEL_H
