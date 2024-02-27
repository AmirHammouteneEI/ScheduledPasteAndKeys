#ifndef ACTIONWIDGETSMANAGER_H
#define ACTIONWIDGETSMANAGER_H

#include "ui/actionwidgets/AbstractActionWidget.h"

#include <QObject>
#include <QVBoxLayout>

class ActionWidgetsManager : public QObject
{
    Q_OBJECT
    QVBoxLayout *m_layout = nullptr;
    QList<AbstractActionWidget*> m_actionWidgetsList;
public:
    explicit ActionWidgetsManager(QVBoxLayout *parent = nullptr);
    int appendWidget(AbstractActionWidget* actionWidget);

signals:
};

#endif // ACTIONWIDGETSMANAGER_H
