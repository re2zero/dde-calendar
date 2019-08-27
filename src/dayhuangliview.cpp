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
#include "dayhuangliview.h"
#include <QAction>
#include <QMenu>
#include <QListWidget>
#include <QLabel>
#include <QPainter>
#include <QHBoxLayout>
#include <QStylePainter>
#include <QRect>
#include "dbmanager.h"
#include "schceduledlg.h"
#include <DHiDPIHelper>
CDayHuangLiLabel::CDayHuangLiLabel( QWidget *parent /*= nullptr*/ ): DLabel (parent)
{
    setMargin(0);
}

void CDayHuangLiLabel::setbackgroundColor(QColor backgroundColor)
{
    m_backgroundColor = backgroundColor;
}

void CDayHuangLiLabel::setTextInfo(QColor tcolor, QFont font)
{
    m_textcolor = tcolor;
    m_font = font;
}

void CDayHuangLiLabel::setHuangLiText(QStringList vhuangli, int type)
{
    m_vHuangli = vhuangli;
    m_type = type;
    if (!vhuangli.isEmpty()) {
        QString  str = vhuangli.at(0);
        for (int i = 1; i < vhuangli.count(); i++) {
            str += "." + vhuangli.at(i);
        }
        setToolTip(str);
    } else {
        setToolTip(QString());
    }
    update();
}
void CDayHuangLiLabel::paintEvent( QPaintEvent *e )
{
    int labelwidth = width();
    int labelheight = height();

    QPainter painter(this);
    QRect fillRect = QRect(0, 0, labelwidth, labelheight);
    painter.setRenderHints(QPainter::HighQualityAntialiasing);
    painter.setBrush(QBrush(m_backgroundColor));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(fillRect, 3, 3);
    QPixmap pixmap;
    if (m_type == 0)
        pixmap = DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-yi.svg").scaled(20, 20);
    else {
        pixmap = DHiDPIHelper::loadNxPixmap(":/resources/icon/dde-ji.svg").scaled(20, 20);
    }
    painter.drawPixmap(14, 18, pixmap);
    painter.setFont(m_font);
    painter.setPen(m_textcolor);
    int bw = 44;
    int bh = 18;
    int ss = 14;
    for (int i = 0; i < m_vHuangli.count(); i++) {
        int currentsw = m_vHuangli.at(i).count() * ss;
        if (bw + currentsw + 6 >  labelwidth) {
            painter.drawText(QRect(bw, bh, labelwidth - bw, 21), Qt::AlignLeft, "...");
            break;
        } else {
            painter.drawText(QRect(bw, bh, currentsw, 21), Qt::AlignLeft, m_vHuangli.at(i));
            bw += currentsw + 10;
        }
    }
}


