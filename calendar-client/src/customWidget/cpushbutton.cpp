#include "cpushbutton.h"
#include <DPaletteHelper>
#include <DHiDPIHelper>
#include <QMouseEvent>
#include <QPainter>
#include <QLabel>
#include <QHBoxLayout>
#include <DIconButton>
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
    pressed = true;
    update();
}

void CPushButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (pressed && rect().contains(event->pos())){
        emit clicked();
    }
    pressed = false;
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

    if (pressed) {
        painter.setBrush(palette.highlight());
    } else {
        painter.setBrush(QBrush("#00000000"));
    }

    painter.drawRect(this->rect());
}
