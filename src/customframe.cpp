#include "customframe.h"
#include <QPainter>
#include <DPalette>
DGUI_USE_NAMESPACE
CustomFrame::CustomFrame(QWidget *parent): QFrame (parent)
{
    m_font.setFamily("SourceHanSansSC");
    m_font.setWeight(QFont::Medium);
    m_font.setPixelSize(14);
    this->setAttribute(Qt::WA_TranslucentBackground);//设置窗口背景透明
    this->setWindowFlags(Qt::FramelessWindowHint);   //设置无边框窗口
    setContentsMargins(0, 0, 0, 0);
}

void CustomFrame::setBColor(QColor normalC)
{
    m_bnormalColor = normalC;
}

void CustomFrame::setRoundState(bool lstate, bool tstate, bool rstate, bool bstate)
{
    m_lstate = lstate;
    m_tstate = tstate;
    m_rstate = rstate;
    m_bstate = bstate;
}

void CustomFrame::setTextStr(QFont font, QColor tc, QString strc, int flag)
{
    m_font = font;
    m_tnormalColor = tc;
    m_text = strc;
    m_textflag = flag;
}

void CustomFrame::setTextStr(QString strc)
{
    m_text = strc;
    update();
}

void CustomFrame::setTextColor(QColor tc)
{
    m_tnormalColor = tc;
    update();
}

void CustomFrame::setTextFont(QFont font)
{
    m_font = font;
}

void CustomFrame::setTextAlign(int flag)
{
    m_textflag = flag;
}

void CustomFrame::setRadius(int radius)
{
    m_radius = radius;
}

void CustomFrame::paintEvent(QPaintEvent *e)
{
    int labelwidth = width();
    int labelheight = height();

    QPainter painter(this);
    QRect fillRect = QRect(0, 0, labelwidth, labelheight);
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    painter.setBrush(QBrush(m_bnormalColor));
    painter.setPen(Qt::NoPen);
    QPainterPath painterPath;
    painterPath.moveTo(m_radius, 0);
    if (m_lstate) {
        painterPath.arcTo(QRect(0, 0, m_radius * 2, m_radius * 2), 90, 90);
    } else {
        painterPath.lineTo(0, 0);
        painterPath.lineTo(0, m_radius);
    }
    painterPath.lineTo(0, labelheight - m_radius);
    if (m_bstate) {
        painterPath.arcTo(QRect(0, labelheight - m_radius * 2, m_radius * 2, m_radius * 2), 180, 90);
    } else {
        painterPath.lineTo(0, labelheight);
        painterPath.lineTo(m_radius, labelheight);
    }
    painterPath.lineTo(labelwidth - m_radius, labelheight);
    if (m_rstate) {
        painterPath.arcTo(QRect(labelwidth - m_radius * 2, labelheight - m_radius * 2, m_radius * 2, m_radius * 2), 270, 90);
    } else {
        painterPath.lineTo(labelwidth, labelheight);
        painterPath.lineTo(labelwidth, labelheight - m_radius);
    }
    painterPath.lineTo(labelwidth, m_radius);
    //painterPath.moveTo(labelwidth, m_radius);
    if (m_tstate) {

        painterPath.arcTo(QRect(labelwidth - m_radius * 2, 0, m_radius * 2, m_radius * 2), 0, 90);

    } else {
        painterPath.lineTo(labelwidth, 0);
        painterPath.lineTo(labelwidth - m_radius, 0);
    }
    painterPath.lineTo(m_radius, 0);
    painterPath.closeSubpath();
    painter.drawPath(painterPath);
    painter.restore();

    if (!m_text.isEmpty()) {
        painter.save();
        painter.setRenderHints(QPainter::HighQualityAntialiasing);
        painter.setFont(m_font);
        painter.setPen(m_tnormalColor);
        //painter.setPen(Qt::NoPen);
        painter.drawText(fillRect, m_textflag, m_text);
        painter.restore();
    }
    QWidget::paintEvent(e);
}

