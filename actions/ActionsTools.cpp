#include "ActionsTools.h"

ActionsTools::ActionsTools() {}

void ActionsTools::pasteKeystrokeSimulate()
{
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    ip.ki.wVk = VK_CONTROL;
    ip.ki.dwFlags = 0;
    SendInput(1, &ip, sizeof(INPUT));

    ip.ki.wVk = 'V';
    ip.ki.dwFlags = 0;
    SendInput(1, &ip, sizeof(INPUT));

    ip.ki.wVk = 'V';
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));

    ip.ki.wVk = VK_CONTROL;
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));
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
