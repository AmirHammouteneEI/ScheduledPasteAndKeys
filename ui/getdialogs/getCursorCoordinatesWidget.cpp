#include "getCursorCoordinatesWidget.h"
#include <QPushButton>
#include <QApplication>
#include <QScreen>
#include <QPixmap>
#include "actions/ActionsTools.h"

getCursorCoordinatesWidget::getCursorCoordinatesWidget(QWidget *parent)
: QDialog(parent)
{
    QPixmap cursorPix(":/img/crosshair.png");
    cursorPix = cursorPix.scaled(32,32);
    setCursor(QCursor(cursorPix));
    setWindowOpacity(0.4);
    setModal(true);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
}

getCursorCoordinatesWidget::~getCursorCoordinatesWidget()
{
}

void getCursorCoordinatesWidget::showWidget()
{
    auto buttonSender = qobject_cast<QPushButton*>(sender());
    if(buttonSender == nullptr)
        return;

    m_refIndex = buttonSender->property("index").toInt();

    QDialog::show();
    int width = 0;
    int height = 0;
    for(auto &screen : QApplication::screens())
    {
        width+=screen->geometry().width();
        if(height < screen->geometry().height())
            height = screen->geometry().height();
    }
    setGeometry(0,0,width,height);
}

void getCursorCoordinatesWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    auto coordsPair = ActionsTools::getCursorPos();
    emit sendCoordinates(m_refIndex,coordsPair.first,coordsPair.second);
    QDialog::close();
}
