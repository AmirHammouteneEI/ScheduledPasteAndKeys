#include "GlobalHotkeyRegisterManager.h"
#include <qt_windows.h>

#include "mainwindow.h"
#include "globals.h"

GlobalHotkeyRegisterManager * GlobalHotkeyRegisterManager::s_instance = nullptr;

GlobalHotkeyRegisterManager *GlobalHotkeyRegisterManager::getInstance()
{
    if(s_instance == nullptr)
    {
        s_instance = new GlobalHotkeyRegisterManager();
    }
    return s_instance;
}

void GlobalHotkeyRegisterManager::destroyInstance()
{
    if (s_instance != nullptr)
    {
        delete s_instance;
        s_instance = nullptr;
    }
}

GlobalHotkeyRegisterManager::GlobalHotkeyRegisterManager()
{
    if (QCoreApplication::instance())
        QCoreApplication::instance()->installNativeEventFilter(this);
}

GlobalHotkeyRegisterManager::~GlobalHotkeyRegisterManager()
{
    unregisterAllHotkeys();
    m_TaskHotkeysMap.clear();

    if (QCoreApplication::instance())
        QCoreApplication::instance()->removeNativeEventFilter(this);
}

int GlobalHotkeyRegisterManager::appendTaskHotKey(const std::shared_ptr<TaskHotKey> &hotkey)
{
    m_TaskHotkeysMap.insert(hotkey->getID(), hotkey);
    return m_TaskHotkeysMap.size();
}

bool GlobalHotkeyRegisterManager::registerHotkey(unsigned int id)
{
    const auto &hotkey = m_TaskHotkeysMap.value(id, nullptr);
    if(!hotkey)
        return false;

    auto keyseq = hotkey->getKeySeq();
    if(keyseq.count() != 1)
        return false;

    auto success = RegisterHotKey(nullptr, id, qtModifiersToWinVirtualModifiers(keyseq[0].keyboardModifiers()), qtKeyToWinVirtualKey(keyseq[0].key()));
    if(success)
        m_registeredHotkeysSet.insert(id);
    return success;
}

bool GlobalHotkeyRegisterManager::unregisterHotkey(unsigned int id)
{
    const auto &hotkey = m_TaskHotkeysMap.value(id, nullptr);
    if(!hotkey)
        return false;

    auto success = UnregisterHotKey(nullptr, id);
    if(success)
        m_registeredHotkeysSet.remove(id);
    return success;
}

void GlobalHotkeyRegisterManager::unregisterAllHotkeys()
{
    for (auto id : std::as_const(m_registeredHotkeysSet))
    {
        const auto &hotkey = m_TaskHotkeysMap.value(id, nullptr);
        if(!hotkey)
            continue;
        UnregisterHotKey(nullptr, id);
    }

    m_registeredHotkeysSet.clear();
}

bool GlobalHotkeyRegisterManager::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result)
{
    if(!G_Parameters::ListenHotkeys)
        return false;
    MSG* winmsg = static_cast<MSG*>(message);
    if(winmsg && winmsg->message == WM_HOTKEY)
    {
        auto id = static_cast<unsigned int>(winmsg->wParam);
        if (m_TaskHotkeysMap.contains(id))
        {
            scheduleTask(id);
            return true;
        }
    }
    return false;
}

void GlobalHotkeyRegisterManager::scheduleTask(unsigned int id)
{
    const auto &hotkey = m_TaskHotkeysMap.value(id, nullptr);
    if(!hotkey)
        return;

    if(hotkey->getType() == TaskHotkeyType::StopAllTasks)
    {
        MainWindow::getInstance()->stopAllTasks();
    }
    else
    {
        MainWindow::getInstance()->autoRun(hotkey->getTaskname(),hotkey->getDelay(),hotkey->getLooptimes());
    }
}

UINT GlobalHotkeyRegisterManager::qtModifiersToWinVirtualModifiers(Qt::KeyboardModifiers mods)
{
    UINT winModifiers = 0;

    if (mods & Qt::ControlModifier)
        winModifiers |= MOD_CONTROL;
    if (mods & Qt::ShiftModifier)
        winModifiers |= MOD_SHIFT;
    if (mods & Qt::AltModifier)
        winModifiers |= MOD_ALT;
    if (mods & Qt::MetaModifier)
        winModifiers |= MOD_WIN;

    return winModifiers;
}

UINT GlobalHotkeyRegisterManager::qtKeyToWinVirtualKey(Qt::Key key)
{
    if((key >= Qt::Key_A && key <= Qt::Key_Z) || (key >= Qt::Key_0 && key <= Qt::Key_9))
        return static_cast<UINT>(key);

    if (key >= Qt::Key_F1 && key <= Qt::Key_F24)
        return VK_F1 + (key - Qt::Key_F1);

    switch (key)
    {
        case Qt::Key_Return:
        case Qt::Key_Enter:
            return VK_RETURN;
        case Qt::Key_Tab:
            return VK_TAB;
        case Qt::Key_Backspace:
            return VK_BACK;
        case Qt::Key_Space:
            return VK_SPACE;
        case Qt::Key_Delete:
            return VK_DELETE;
        case Qt::Key_Insert:
            return VK_INSERT;
        case Qt::Key_Home:
            return VK_HOME;
        case Qt::Key_End:
            return VK_END;
        case Qt::Key_PageUp:
            return VK_PRIOR;
        case Qt::Key_PageDown:
            return VK_NEXT;
        case Qt::Key_Left:
            return VK_LEFT;
        case Qt::Key_Right:
            return VK_RIGHT;
        case Qt::Key_Up:
            return VK_UP;
        case Qt::Key_Down:
            return VK_DOWN;
        case Qt::Key_Escape:
            return VK_ESCAPE;
        default:
            return 0;
    }
}