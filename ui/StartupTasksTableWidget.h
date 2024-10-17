#ifndef STARTUPTASKSTABLEWIDGET_H
#define STARTUPTASKSTABLEWIDGET_H

#include <QTableWidget>

#include "ui/StartupTaskEditDialog.h"

class StartupTasksTableWidget : public QTableWidget
{
    Q_OBJECT
    StartupTaskEditDialog *m_startupTaskEditDialog;
    bool doesEntryExistsInRegistry(const QString & taskParam);
    void addEntryToRegistry(const QString & taskParam);
    void deleteEntryFromRegistry(const QString & taskParam);
public:
    explicit StartupTasksTableWidget(QWidget *parent = nullptr);
    ~StartupTasksTableWidget() = default;
    void refresh();
    void saveInSettings();
private slots:
    void checkBoxToggled(bool val);
public slots:
    void addStartupTaskReceived();
    void removeStartupTaskReceived();
    void editFromDialogReceived();
};

#endif // STARTUPTASKSTABLEWIDGET_H
