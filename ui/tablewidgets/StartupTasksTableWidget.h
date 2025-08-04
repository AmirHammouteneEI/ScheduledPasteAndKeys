#ifndef STARTUPTASKSTABLEWIDGET_H
#define STARTUPTASKSTABLEWIDGET_H

#include "ui/CustomPrimaryWidgets.h"
#include "ui/various_dialogs/StartupTaskEditDialog.h"

class StartupTasksTableWidget : public NoFocusCellTableWidget
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
