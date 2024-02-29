#include "PasteWidget.h"
#include "actions/PasteAction.h"

#include <QGridLayout>
#include <QPushButton>

PasteWidget::PasteWidget(QWidget *parent)
    : AbstractActionWidget{parent}
{

}

void PasteWidget::buildWidget()
{
    if(m_centralWidget == nullptr)
        return;

    auto pasteaction =  dynamic_cast<PasteAction*>(m_action);

    QString content = "ERROR on access action";
    if(pasteaction != nullptr)
        content = pasteaction->m_content;

    auto gridLayout = new QGridLayout(m_centralWidget);
    auto mainButton = new QPushButton("Paste text ",m_centralWidget); //TODO title is the #id of the text + Icon
    mainButton->setToolTip(content);

    gridLayout->addItem(new QSpacerItem(5,5,QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding),0,0);
    gridLayout->addWidget(mainButton,1,1,Qt::AlignCenter | Qt::AlignHCenter);
    gridLayout->addItem(new QSpacerItem(5,5,QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding),2,2);
}
