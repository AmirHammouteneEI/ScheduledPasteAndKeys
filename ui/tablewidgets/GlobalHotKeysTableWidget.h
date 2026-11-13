#ifndef GLOBALHOTKEYSTABLEWIDGET_H
#define GLOBALHOTKEYSTABLEWIDGET_H

#include "ui/CustomPrimaryWidgets.h"
#include "ui/various_dialogs/GlobalHotKeyEditDialog.h"

class GlobalHotKeysTableWidget : public NoFocusCellTableWidget
{
    Q_OBJECT
    GlobalHotKeyEditDialog *m_hotkeyEditDialog = nullptr;
    bool doesHotKeyIsRegistered(const QString & hotkeyParam);
    bool registerHotKey(const QString & hotkeyParam);
    bool unregisterHotKey(const QString & hotkeyParam);
    QMap<QString, unsigned int> m_hotkeyParamsToObjectIdMap;
public:
    explicit GlobalHotKeysTableWidget(QWidget *parent = nullptr);
    ~GlobalHotKeysTableWidget() = default;
    void refresh();
    void saveInSettings();
private slots:
    void checkBoxToggled(bool val);
    void onHotkeyButtonPushed();
public slots:
    void addHotkeyReceived();
    void removeHotkeyReceived();
    void editFromDialogReceived();
};

#endif // GLOBALHOTKEYSTABLEWIDGET_H
