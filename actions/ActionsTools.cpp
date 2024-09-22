#include "ActionsTools.h"
#include <QStringList>
#include <QVariant>

QMap<QString, WORD> ActionsTools::m_keysStrToInputWordMap = { {"CONTROL_LEFT", VK_LCONTROL},{"CONTROL_RIGHT", VK_RCONTROL},{"SHIFT_LEFT", VK_LSHIFT},{"SHIFT_RIGHT", VK_RSHIFT},
    {"ALT_LEFT", VK_LMENU},{"ALT_GR", VK_RMENU},{"WINDOWS", VK_LWIN},{"FN", 0xFF},{"LEFT_MOUSE", MOUSEEVENTF_LEFTDOWN},{"RIGHT_MOUSE", MOUSEEVENTF_RIGHTDOWN},{"MIDDLE_MOUSE", MOUSEEVENTF_MIDDLEDOWN},
    {"XBUTTON_1", MOUSEEVENTF_XDOWN},{"XBUTTON_2", MOUSEEVENTF_XDOWN},
    {"A", 0x41},{"B", 0x42},{"C", 0x43},{"D", 0x44},{"E", 0x45},{"F", 0x46},{"G", 0x47},{"H", 0x48},{"I", 0x49},{"J", 0x4A},{"K", 0x4B},{"L", 0x4C},
    {"M", 0x4D},{"N", 0x4E},{"O", 0x4F},{"P", 0x50},{"Q", 0x51},{"R", 0x52},{"S", 0x53},{"T", 0x54},{"U", 0x55},{"V", 0x56},{"W", 0x57},{"X", 0x58},
    {"Y", 0x59},{"Z", 0x5A},{"0", 0x30},{"1", 0x31},{"2", 0x32},{"3", 0x33},{"4", 0x34},{"5", 0x35},{"6", 0x36},{"7", 0x37},{"8", 0x38},{"9", 0x39},
    {"NUMPAD_0", VK_NUMPAD0},{"NUMPAD_1", VK_NUMPAD1},{"NUMPAD_2", VK_NUMPAD2},{"NUMPAD_3", VK_NUMPAD3},{"NUMPAD_4", VK_NUMPAD4},{"NUMPAD_5", VK_NUMPAD5},
    {"NUMPAD_6", VK_NUMPAD6},{"NUMPAD_7", VK_NUMPAD7},{"NUMPAD_8", VK_NUMPAD8},{"NUMPAD_9", VK_NUMPAD9},{"+", VK_ADD},{"-", VK_SUBTRACT},{"*", VK_MULTIPLY},
    {"/", VK_DIVIDE},{".", VK_DECIMAL},{",", VK_SEPARATOR},{"NUMPAD_ENTER", VK_RETURN},{"NUM_LOCK", VK_NUMLOCK},
    {"SPACEBAR", VK_SPACE},{"ENTER", VK_RETURN},{"BACKSPACE", VK_BACK},{"DELETE", VK_DELETE},{"INSERT", VK_INSERT},{"ESCAPE", VK_ESCAPE	},
    {"PRINT_SCREEN", VK_SNAPSHOT},{"PRINT", VK_PRINT},{"F1", VK_F1},{"F2", VK_F2},{"F3", VK_F3},{"F4", VK_F4},{"F5", VK_F5},{"F6", VK_F6},{"F7", VK_F7},{"F8", VK_F8},{"F9", VK_F9},{"F10", VK_F10},{"F11", VK_F11},{"F12", VK_F12}
    ,{"PLAYPAUSE_MEDIA", VK_MEDIA_PLAY_PAUSE},{"STOP_MEDIA", VK_MEDIA_STOP},{"VOLUME_UP", VK_VOLUME_UP},{"VOLUME_DOWN", VK_VOLUME_DOWN},
    {"VOLUME_MUTE", VK_VOLUME_MUTE},{"NEXT_TRACK", VK_MEDIA_NEXT_TRACK},{"PREVIOUS_TRACK", VK_MEDIA_PREV_TRACK},{"CAPS_LOCK", VK_CAPITAL},{"SCROLL_LOCK", VK_SCROLL},
    {"UP_ARROW", VK_UP},{"DOWN_ARROW", VK_DOWN},{"LEFT_ARROW", VK_LEFT},{"RIGHT_ARROW", VK_RIGHT},{"HOME", VK_HOME},{"END", VK_END},
    {"PAGE_UP", VK_PRIOR},{"PAGE_DOWN", VK_NEXT},{"TAB", VK_TAB},
    {";:", VK_OEM_1},{"/?", VK_OEM_2},{"`~", VK_OEM_3},{"[{", VK_OEM_4},{"\\|", VK_OEM_5},{"]}", VK_OEM_6},{"'\"", VK_OEM_7},{"!§", VK_OEM_8},{"<>", VK_OEM_102},
    {"$£", VK_OEM_1},{":/", VK_OEM_2},{"ù%", VK_OEM_3},{")°", VK_OEM_4},{"*µ", VK_OEM_5},{"^¨", VK_OEM_6},{"²", VK_OEM_7},
};

ActionsTools::ActionsTools()
{

}

void ActionsTools::pasteKeystrokeSimulate()
{
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;
    ip.ki.dwFlags = 0;

    ip.ki.wVk = VK_CONTROL;
    SendInput(ip.type, &ip, sizeof(INPUT));

    ip.ki.wVk = 'V';
    SendInput(ip.type, &ip, sizeof(INPUT));

    Sleep(100);

    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(ip.type, &ip, sizeof(INPUT));

    ip.ki.wVk = VK_CONTROL;
    SendInput(ip.type, &ip, sizeof(INPUT));

    Sleep(100);
}

void ActionsTools::keyStokeSimulate(const QString &keyAsStr, DWORD typeOfPress)
{
    if(!m_keysStrToInputWordMap.contains(keyAsStr))
        return;

    if(keyAsStr=="LEFT_MOUSE" || keyAsStr=="RIGHT_MOUSE" || keyAsStr=="MIDDLE_MOUSE" || keyAsStr=="XBUTTON_1" || keyAsStr=="XBUTTON_2")
    {
        mouseStokeSimulate(keyAsStr,typeOfPress);
        return;
    }

    INPUT ip;
    auto wVkCode = m_keysStrToInputWordMap.value(keyAsStr,VK_RETURN);
    ip.type = INPUT_KEYBOARD;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;
    ip.ki.dwFlags = typeOfPress;
    ip.ki.wVk = wVkCode;
    ip.ki.wScan = MapVirtualKeyW(wVkCode, MAPVK_VK_TO_VSC_EX);;
    SendInput(1, &ip, sizeof(INPUT));
}

void ActionsTools::mouseStokeSimulate(const QString &keyAsStr, DWORD typeOfPress)
{
    DWORD mouseData = 0;
    if(keyAsStr=="XBUTTON_1")
        mouseData=XBUTTON1;
    if(keyAsStr=="XBUTTON_2")
        mouseData=XBUTTON2;

    INPUT ip;
    ip.type = INPUT_MOUSE;
    ip.mi.dx = 0;
    ip.mi.dy = 0;
    ip.mi.mouseData = mouseData;
    ip.mi.time = 0;
    ip.mi.dwExtraInfo = 0;
    ip.mi.dwFlags = mouseFlagPressReleaseConversion(m_keysStrToInputWordMap.value(keyAsStr,VK_RETURN),typeOfPress);;
    SendInput(1, &ip, sizeof(INPUT));
}

DWORD ActionsTools::mouseFlagPressReleaseConversion(WORD keyInputWord, DWORD typeOfPress)
{
    if(typeOfPress == 0)
        return keyInputWord;

    switch (keyInputWord)
    {
    case MOUSEEVENTF_RIGHTDOWN:
        return MOUSEEVENTF_RIGHTUP;
        break;
    case MOUSEEVENTF_MIDDLEDOWN:
        return MOUSEEVENTF_MIDDLEUP;
        break;
    case MOUSEEVENTF_XDOWN:
        return MOUSEEVENTF_XUP;
        break;
    default:
        return MOUSEEVENTF_LEFTUP;
        break;
    }
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

PressedReleaseDelaysKeysMap ActionsTools::fromStandardQMapToKeysSeqMap(const QMap<QString, QVariant> &standardMap)
{
    PressedReleaseDelaysKeysMap keysMap;
    for(auto [key,val] : standardMap.asKeyValueRange())
    {
        ReleaseDelayKeysPair pair = val.value<ReleaseDelayKeysPair>();
        keysMap.insert(key.toInt(),pair);
    }

    return keysMap;
}

QString ActionsTools::fromKeysSeqMapToPrintedString(const PressedReleaseDelaysKeysMap &map)
{
    QStringList contentList;
    for(auto [key,val] : map.asKeyValueRange())
    {
        QString oneLineInfo = QString::number(key)+" ms (release after "+
          QString::number(val.first)+" ms) : ";
        oneLineInfo+= val.second.join("+");
        contentList.append(oneLineInfo);
    }

    return contentList.join("\n");
}

QString ActionsTools::fromCursorMovsMapToPrintedString(const CursorMovementsList &map)
{
    QStringList contentList;
    for(auto el : map)
    {
        if(el.size() < 4)
            continue;
        QString oneLineInfo = QString::number(el[0])+" ms delay (after previous move), during "+
                              QString::number(el[1])+" ms : ";
        oneLineInfo+= "["+QString::number(el[2])+","+QString::number(el[3])+"]";
        contentList.append(oneLineInfo);
    }

    return contentList.join("\n");
}

CursorMovementsList ActionsTools::fromStandardQMapToCursorMovsMap(const QList<QVariant> &standardList)
{
    CursorMovementsList curmovsList;
    for(auto &el : standardList)
    {
        MovementList movlist = el.value<MovementList>();
        if(movlist.size() >= 4)
            curmovsList.append(movlist);
    }

    return curmovsList;
}

void ActionsTools::moveCursorSimulate(int xPos, int yPos, int time)
{
    POINT origin;
    bool successGet = GetCursorPos(&origin);
    if(time<= 0 || !successGet)
    {
        SetCursorPos(xPos,yPos);
        return;
    }

    auto timesToMove = time / s_cursorFrequency;
    int movedTimes = 1;
    while(movedTimes < timesToMove)
    {
        int curX = (movedTimes*(xPos-origin.x) + timesToMove*origin.x)/timesToMove;
        int curY = (movedTimes*(yPos-origin.y) + timesToMove*origin.y)/timesToMove;
        SetCursorPos(curX,curY);
        Sleep(s_cursorFrequency);
        ++movedTimes;
    }
    SetCursorPos(xPos,yPos);
}
