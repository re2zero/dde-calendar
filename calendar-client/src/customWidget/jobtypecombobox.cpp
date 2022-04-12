#include "jobtypecombobox.h"

#include <DLineEdit>
#include <DHiDPIHelper>

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
    for (int i = 0;i<m_lstJobType.size();i++) {
        if(strJobTypeNo == m_lstJobType[i].getJobTypeNo()){
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
    qInfo() << "m_lstJobType.size():" << m_lstJobType.size();

    for (int i = 0;i < m_lstJobType.size();i++) {
        strColorHex = m_lstJobType[i].getColorHex();
        strJobType = m_lstJobType[i].getJobTypeName();

        if(strColorHex.isEmpty() || strJobType.isEmpty()){
            continue;
        }
        addJobTypeItem(i, m_lstJobType[i].getColorHex(),m_lstJobType[i].getJobTypeName());
    }
    return true;
}
void JobTypeComboBox::addJobTypeItem(int idx, QString strColorHex, QString strJobType)
{
    QSize size(16, 16);
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing );
    painter.setBrush(QColor(strColorHex));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(0, 0, 16, 16, 4, 4);

    insertItem(idx, QIcon(pixmap),tr(strJobType.toLocal8Bit()));//
}
void JobTypeComboBox::initUI()
{
    setEditable(false);
    setIconSize(QSize(16,16));
    view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void JobTypeComboBox::showPopup()
{
    //重置icon大小
    setIconSize(QSize(16,16));
    DComboBox::showPopup();

    setEditable(false);
    //获取下拉视图容器
    QFrame *viewContainer = findChild<QFrame *>();
    if (viewContainer) {
        QAbstractItemView *view = viewContainer->findChild<QAbstractItemView *>();

        if((m_btnAdd == nullptr) && view){
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
            m_btnAdd = new DPushButton();
            m_btnAdd->setFixedHeight(34);
            m_btnAdd->setLayout(layoutAddType);

            int index = viewContainer->layout()->indexOf(view);
            QBoxLayout *layout = qobject_cast<QBoxLayout *>(viewContainer->layout());
            layout->insertWidget(index+1,m_btnAdd);
            viewContainer->setFixedHeight(viewContainer->height() + 34 + 10);

            connect(m_btnAdd, &QPushButton::clicked, this, &JobTypeComboBox::slotBtnAddItemClicked);
        }
        if(m_btnAdd) {
            bool darkTheme = DGuiApplicationHelper::instance()->themeType()  == 2;
            if(darkTheme){
                m_btnAdd->setBackgroundRole(QPalette::Base);
                m_btnAdd->setFlat(true);
            }
            else {
                //m_btnAdd->setStyleSheet("border:none;background:#fff;");
            }
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

