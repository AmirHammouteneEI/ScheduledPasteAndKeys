#include "WaitWidget.h"
#include "actions/WaitAction.h"

#include <QGridLayout>
#include <QPushButton>

WaitWidget::WaitWidget(QWidget *parent)
    : AbstractActionWidget{parent}
{

}

void WaitWidget::buildWidget()
{

    if(m_centralWidget == nullptr)
        return;

    auto pasteaction =  dynamic_cast<WaitAction*>(m_action);

    QString durationStr = "ERROR on access duration";
    if(pasteaction != nullptr)
        durationStr = QString::number(pasteaction->m_duration);

    auto gridLayout = new QGridLayout(m_centralWidget);
    auto mainButton = new QPushButton(tr("Wait for ") + durationStr + tr(" secs"),m_centralWidget); //TODO icon
    mainButton->setToolTip(durationStr+tr(" seconds"));

    gridLayout->addItem(new QSpacerItem(5,5,QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding),0,0);
    gridLayout->addWidget(mainButton,1,1,Qt::AlignCenter | Qt::AlignHCenter);
    gridLayout->addItem(new QSpacerItem(5,5,QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding),2,2);
}
