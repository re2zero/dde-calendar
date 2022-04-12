#include "jobtypecombobox.h"

#include <DLineEdit>

#include <QWidget>
#include <QLayout>
#include <QPainter>
#include <QEvent>
#include <QFocusEvent>
#include <QAbstractItemView>

JobTypeComboBox::JobTypeComboBox(QWidget *parent) : DComboBox(parent)
{
    initUI();
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

    insertItem(idx, QIcon(pixmap), tr(strJobType.toLocal8Bit())); //
}
void JobTypeComboBox::initUI()
{
    setEditable(false);
    setIconSize(QSize(16, 16));
    view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void JobTypeComboBox::showPopup()
{
    //重置icon大小
    setIconSize(QSize(16, 16));
    DComboBox::showPopup();

    setEditable(false);

    if (m_lstJobType.size() >= 20) {
        return;
    }

    //获取下拉视图容器
    QFrame *viewContainer = findChild<QFrame *>();
    if (viewContainer) {
        QAbstractItemView *view = viewContainer->findChild<QAbstractItemView *>();

        if((m_btnAdd == nullptr) && view){
            m_btnAdd = new CPushButton();

            int index = viewContainer->layout()->indexOf(view);
            QBoxLayout *layout = qobject_cast<QBoxLayout *>(viewContainer->layout());
            layout->insertWidget(index + 1, m_btnAdd);
            viewContainer->setFixedHeight(viewContainer->height() + 34 + 10);

            connect(m_btnAdd, &QPushButton::clicked, this, &JobTypeComboBox::slotBtnAddItemClicked);
        }
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

