/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     liulibang <liulibang@uniontech.com>
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
#include "colorpickerWidget.h"
#include "tabletconfig.h"

#include <DFontSizeManager>
#include <DFrame>
#include <DListView>
#include <DTitlebar>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

DGUI_USE_NAMESPACE

CColorPickerWidget::CColorPickerWidget(QWidget *parent)
    : DWidget(parent)
    , m_colorLabel(new ColorLabel(this))
    , m_colorSlider(new ColorSlider(this))
    , m_colHexLineEdit(new DLineEdit(this))
    , m_wordLabel(new DLabel(this))
    , m_cancelBtn(new DPushButton(this))
    , m_enterBtn(new DPushButton(this))
{
    initUI();
    moveCentorShow();
    setColorHexLineEdit();
}

CColorPickerWidget::~CColorPickerWidget()
{
}

void CColorPickerWidget::setColorHexLineEdit()
{
    connect(m_colHexLineEdit, &DLineEdit::textChanged, this, [ = ](const QString &) {
        m_colHexLineEdit->setText(m_colHexLineEdit->text().remove('#'));
        m_colHexLineEdit->setText(m_colHexLineEdit->text().toLower());
    });

    m_colHexLineEdit->setText("");
    QRegExp reg("[0-9A-Ea-e]{6}$");
    QValidator *validator = new QRegExpValidator(reg,m_colHexLineEdit->lineEdit());
    m_colHexLineEdit->lineEdit()->setValidator(validator);
    setFocusProxy(m_colHexLineEdit);
}
/**
 * @brief CColorPickerDlg::initUI      初始化
 */
void CColorPickerWidget::initUI()
{
    QFont mlabelTitle;
    QFont mlabelContext;
    mlabelTitle.setWeight(QFont::Bold);
    mlabelContext.setWeight(QFont::Medium);
    setFixedSize(314, 276);
    m_colorLabel->setFixedSize(294, 136);
    m_colorSlider->setFixedSize(294,14);

    connect(m_colorSlider, &ColorSlider::valueChanged, m_colorLabel, [ = ](int val) {
        m_colorLabel->setHue(val);
    });
    connect(m_colorLabel, &ColorLabel::signalpickedColor, this, [ = ](QColor color) {
        this->slotSetColor(color);
    });
    connect(m_colorLabel, &ColorLabel::signalPreViewColor, this, [ = ](QColor color) {
        this->slotUpdateColor(color);
    });

    QVBoxLayout *mLayout = new QVBoxLayout;
    mLayout->setSpacing(12);
    QHBoxLayout *labelLayout = new QHBoxLayout;
    QHBoxLayout *sliderLayout = new QHBoxLayout;
    mLayout->setMargin(10);
    labelLayout->addWidget(m_colorLabel, 0, Qt::AlignBottom | Qt::AlignLeft);//AlignVCenter | AlignHCenter
    mLayout->addLayout(labelLayout);
    sliderLayout->addWidget(m_colorSlider, 0, Qt::AlignBottom | Qt::AlignLeft);//AlignVCenter | AlignHCenter
    mLayout->addLayout(sliderLayout);

    m_wordLabel->setText(tr("Color"));
    m_wordLabel->setFixedWidth(40);
    m_wordLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    m_colHexLineEdit->setFixedSize(QSize(248,36));//248
    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputLayout->setMargin(0);
    inputLayout->setSpacing(6);
    inputLayout->addWidget(m_wordLabel);
    inputLayout->addWidget(m_colHexLineEdit);
    inputLayout->addStretch();
    mLayout->addLayout(inputLayout);
    mLayout->addSpacing(4);

    m_cancelBtn->setText(tr("Cancel"));
    m_cancelBtn->setFixedSize(140, 36);
    m_enterBtn->setText(tr("Save"));
    m_enterBtn->setFixedSize(140, 36);
    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->setMargin(0);
    btnLayout->setSpacing(5);
    btnLayout->addWidget(m_cancelBtn);
    DVerticalLine *line = new DVerticalLine(this);
    line->setObjectName("VLine");
    line->setFixedSize(3,28);
    btnLayout->addWidget(line);
    line->show();
    btnLayout->addWidget(m_enterBtn);
    btnLayout->addStretch();
    mLayout->addLayout(btnLayout);
    mLayout->addStretch();
    this->setLayout(mLayout);

    this->setFocusPolicy(Qt::TabFocus);
    setTabOrder(m_colHexLineEdit,m_cancelBtn);
    setTabOrder(m_cancelBtn,m_enterBtn);
    setTabOrder(m_enterBtn,m_colorSlider);
}

void CColorPickerWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    bool   darkTheme = DGuiApplicationHelper::instance()->themeType()  == 2;
    painter.setBrush(QBrush(darkTheme ? "#282828" : "#ffffff"));
    QPen pen;
    pen.setWidth(1);
    pen.setColor(darkTheme ? QColor("#202020") : "#eeeeee");
    painter.setPen(pen);
    QRect rect = this->rect();
    rect.setWidth(rect.width() - 1);
    rect.setHeight(rect.height() - 1);
    painter.drawRoundedRect(rect, 10, 10);
    QWidget::paintEvent(event);
}

void CColorPickerWidget::moveCentorShow()
{
    //需要获取的顶层窗口
    QWidget *_parentWidget = this;
    QWidget *tmpWidget = nullptr;
    do {
        //获取父类对象，如果为qwiget则赋值否则退出
        tmpWidget = qobject_cast<QWidget *>(_parentWidget->parent());
        if (tmpWidget == nullptr) {
            break;
        } else {
            _parentWidget = tmpWidget;
        }
    } while (_parentWidget != nullptr);
    //获取最顶层窗口的中心坐标
    const QPoint global = _parentWidget->mapToGlobal(_parentWidget->rect().center());
    //居中显示
    move(global.x() - width() / 2, global.y() - height() / 2);
}

void CColorPickerWidget::slotUpdateColor(const QColor &color)
{
    QColor c = color.isValid() ? color : curColor;

    if (color.isValid()) {
        //证明是预览发出信号通知外界

        this->m_colHexLineEdit->setText(c.name());
    }
}

void CColorPickerWidget::slotSetColor(const QColor &c)
{
    bool changed = (c != curColor);

    if (changed) {
        curColor = c;

        slotUpdateColor(c);
    }
}
