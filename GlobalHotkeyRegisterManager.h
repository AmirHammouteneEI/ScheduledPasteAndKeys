#ifndef GLOBALHOTKEYREGISTERMANAGER_H
#define GLOBALHOTKEYREGISTERMANAGER_H

#include <QAbstractNativeEventFilter>
#include <TaskHotKey.h>
#include <QMap>
#include <QSet>

class GlobalHotkeyRegisterManager : public QAbstractNativeEventFilter
{
    QMap<unsigned int, std::shared_ptr<TaskHotKey>> m_TaskHotkeysMap;
    void scheduleTask(unsigned int id);
    GlobalHotkeyRegisterManager();
    static GlobalHotkeyRegisterManager* s_instance;
public:
    static GlobalHotkeyRegisterManager * getInstance();
    ~GlobalHotkeyRegisterManager();
    GlobalHotkeyRegisterManager(GlobalHotkeyRegisterManager &other) = delete;
    void operator=(const GlobalHotkeyRegisterManager &) = delete;
    static void destroyInstance();

    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override;

    int appendTaskHotKey(const std::shared_ptr<TaskHotKey> & hotkey);
    bool registerHotkey(unsigned int id);
    bool unregisterHotkey(unsigned int id);
    QSet<unsigned int> m_registeredHotkeysSet;

    void unregisterAllHotkeys();

    static UINT qtModifiersToWinVirtualModifiers(Qt::KeyboardModifiers mods);
    static UINT qtKeyToWinVirtualKey(Qt::Key key);
};

#endif // GLOBALHOTKEYREGISTERMANAGER_H
