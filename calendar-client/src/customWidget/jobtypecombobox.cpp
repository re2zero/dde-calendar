#include "jobtypecombobox.h"

#include <QLayout>
#include <QPainter>
#include <QAbstractItemView>

JobTypeComboBox::JobTypeComboBox(QWidget *parent) : DComboBox(parent)
{
    initUI();
    //不启用自动匹配
    setAutoCompletion(false);
    //设置不接受回车键插入
    setInsertPolicy(QComboBox::NoInsert);
}

JobTypeComboBox::~JobTypeComboBox()
{
    return;
}

int JobTypeComboBox::getCurrentJobTypeNo()
{
    return m_lstJobType[this->currentIndex()].getJobTypeNo();
}

void JobTypeComboBox::setCurrentJobTypeNo(int strJobTypeNo)
{
    for (int i = 0; i < m_lstJobType.size(); i++) {
        if (strJobTypeNo == m_lstJobType[i].getJobTypeNo()) {
            this->setCurrentIndex(i);
            break;
        }
    }
    return;
}

bool JobTypeComboBox::updateJobType()
{
    QString strColorHex;
    QString strJobType;

    m_lstJobType = JobTypeInfoManager::instance()->getJobTypeList();

    clear(); //更新前先清空原有列表
    for (int i = 0; i < m_lstJobType.size(); i++) {
        strColorHex = m_lstJobType[i].getColorHex();
        strJobType = m_lstJobType[i].getJobTypeName();

        if (strColorHex.isEmpty() || strJobType.isEmpty()) {
            continue;
        }
        addJobTypeItem(i, m_lstJobType[i].getColorHex(), m_lstJobType[i].getJobTypeName());
    }
    return true;
}
void JobTypeComboBox::addJobTypeItem(int idx, QString strColorHex, QString strJobType)
{
    QSize size(16, 16);
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.setBrush(QColor(strColorHex));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(0, 0, 16, 16, 4, 4);

    insertItem(idx, QIcon(pixmap), tr(strJobType.toLocal8Bit()));
}

void JobTypeComboBox::initUI()
{
    setEditable(false);
    setIconSize(QSize(16, 16));
    view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void JobTypeComboBox::addCustomWidget(QFrame *viewContainer)
{
    if (viewContainer) {
        if(nullptr == m_customWidget){
            //获取原控件布局
            QBoxLayout *layout = qobject_cast<QBoxLayout *>(viewContainer->layout());

            //自定义控件
            QVBoxLayout *hLayout = new QVBoxLayout;
            DPushButton *splitter = new DPushButton(this);
            CPushButton *btnAdd = new CPushButton();
            splitter->setFixedHeight(2);
            hLayout->addWidget(splitter);
            hLayout->addWidget(btnAdd);
            hLayout->setContentsMargins(0, 0, 0, 0);
            hLayout->setSpacing(0);
            m_customWidget = new QWidget(this);
            m_customWidget->setFixedHeight(35);
            m_customWidget->setLayout(hLayout);
            //添加自定义控件到最后
            layout->insertWidget(-1, m_customWidget);
            viewContainer->setFixedHeight(viewContainer->height() + m_customWidget->height());
            //重置高度
            connect(btnAdd, &QPushButton::clicked, this, &JobTypeComboBox::slotBtnAddItemClicked);
        }
    }
}

void JobTypeComboBox::showPopup()
{
    //重置icon大小
    setIconSize(QSize(16, 16));
    DComboBox::showPopup();
    setEditable(false);

    //获取下拉视图容器
    QFrame *viewContainer = findChild<QFrame *>();

    if (m_lstJobType.size() < 20) {
        //添加自定义布局
        addCustomWidget(viewContainer);
    }
}

void JobTypeComboBox::slotBtnAddItemClicked()
{
    JobTypeComboBox::hidePopup();
    setIconSize(QSize(0, 0));
    setEditable(true);
    setCurrentText("");
    emit signalAddTypeBtnClicked();
    return;
}

