#ifndef ABSTRACTACTIONWIDGET_H
#define ABSTRACTACTIONWIDGET_H

#include "actions/AbstractAction.h"

#include <QWidget>

class AbstractActionWidget : public QWidget
{
    Q_OBJECT
protected:
    QWidget *m_centralWidget = nullptr;
    AbstractAction *m_action = nullptr;
public:
    explicit AbstractActionWidget(QWidget *parent = nullptr);
    void setRunningState(const QString & state);
    virtual void setAction(AbstractAction *action) = 0;
    virtual void buildWidget() = 0;

signals:
    void removeSelf();
};

#endif // ABSTRACTACTIONWIDGET_H
