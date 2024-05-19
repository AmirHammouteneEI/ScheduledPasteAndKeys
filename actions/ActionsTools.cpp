#include "ActionsTools.h"

INPUT ActionsTools::m_ip;

ActionsTools::ActionsTools() {}

void ActionsTools::pasteKeystrokeSimulate()
{
    m_ip.type = INPUT_KEYBOARD;
    m_ip.ki.wScan = 0;
    m_ip.ki.time = 0;
    m_ip.ki.dwExtraInfo = 0;
    m_ip.ki.dwFlags = 0;

    m_ip.ki.wVk = VK_CONTROL;
    SendInput(1, &m_ip, sizeof(INPUT));

    m_ip.ki.wVk = 'V';
    SendInput(1, &m_ip, sizeof(INPUT));

    m_ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &m_ip, sizeof(INPUT));

    m_ip.ki.wVk = VK_CONTROL;
    SendInput(1, &m_ip, sizeof(INPUT));
}

void ActionsTools::setClipboard(const QString &str)
{
    if(OpenClipboard(NULL))
    {
        HGLOBAL clipbuffer;
        EmptyClipboard();
        clipbuffer = GlobalAlloc(GMEM_MOVEABLE, str.size()+1);
        memcpy(GlobalLock(clipbuffer), str.toLatin1(), str.size()+1);
        GlobalUnlock(clipbuffer);
        SetClipboardData(CF_TEXT,clipbuffer);
        CloseClipboard();
    }
}
