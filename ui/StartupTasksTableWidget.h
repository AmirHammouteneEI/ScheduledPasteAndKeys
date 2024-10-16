#ifndef STARTUPTASKSTABLEWIDGET_H
#define STARTUPTASKSTABLEWIDGET_H

#include <QTableWidget>

#include "ui/StartupTaskEditDialog.h"

class StartupTasksTableWidget : public QTableWidget
{
    Q_OBJECT
    StartupTaskEditDialog *m_startupTaskEditDialog;
public:
    explicit StartupTasksTableWidget(QWidget *parent = nullptr);
    ~StartupTasksTableWidget() = default;
    void refresh();
    void saveInSettings();
public slots:
    void addStartupTaskReceived();
    void removeStartupTaskReceived();
    void editFromDialogReceived();
};

#endif // STARTUPTASKSTABLEWIDGET_H
