#include "cpushbutton.h"
#include <DPaletteHelper>
#include <DHiDPIHelper>
#include <DIconButton>
#include <QMouseEvent>
#include <QPainter>
#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>

DWIDGET_USE_NAMESPACE

CPushButton::CPushButton(QWidget *parent) : QPushButton(parent)
{
    setFlat(true);


    QHBoxLayout *layoutAddType = new QHBoxLayout();
    QLabel * lblAddType = new QLabel();
    lblAddType->setFixedSize(100,34);
    lblAddType->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    lblAddType->setText(tr("New event type"));

    lblAddType->setFixedSize(200,34);
    layoutAddType->setSpacing(0);
    layoutAddType->setMargin(0);
    layoutAddType->setAlignment(Qt::AlignLeft);
    DIconButton*  iconBtnAddType = new DIconButton(this);
    iconBtnAddType->setFixedSize(16, 16);
    iconBtnAddType->setFlat(true);
    iconBtnAddType->setIcon(QIcon(DHiDPIHelper::loadNxPixmap(":/resources/icon/create.svg")));
    layoutAddType->setContentsMargins(33,0,0,0);
    layoutAddType->addWidget(iconBtnAddType);
    layoutAddType->addSpacing(5);
    layoutAddType->addWidget(lblAddType);
    setFixedHeight(34);
    setLayout(layoutAddType);
}

void CPushButton::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_pressed = true;
    update();
}

void CPushButton::mouseMoveEvent(QMouseEvent *event)
{
    if (rect().contains(event->pos())) {
        m_pressed = true;
    } else {
        m_pressed = false;
    }
    update();
}

void CPushButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_pressed && rect().contains(event->pos())){
        emit clicked();
    }
    m_pressed = false;
    update();
}

void CPushButton::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);
    DPalette palette = DPaletteHelper::instance()->palette(this);
    QPainter painter(this);
    // 反走样
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    if (m_pressed) {
        //背景设置为高亮色
        painter.setBrush(palette.highlight());
    } else {
        //背景透明
        painter.setBrush(QBrush("#00000000"));
    }

    painter.drawRect(this->rect());
}
