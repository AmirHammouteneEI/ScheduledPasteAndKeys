#ifndef ACTIONSTOOLS_H
#define ACTIONSTOOLS_H

#include <qt_windows.h>
#include <winuser.h>
#include <QString>
#include "actions/ActionParameters.h"

class ActionsTools
{
public:
    ActionsTools();
    ~ActionsTools() = default;

    static void pasteKeystrokeSimulate();
    static void keyStokeSimulate(const QString & keyAsStr, DWORD typeOfPress);
    static void mouseStokeSimulate(const QString & keyAsStr, DWORD typeOfPress);
    static DWORD mouseFlagPressReleaseConversion(WORD keyInputWord, DWORD typeOfPress);
    static void setClipboard(const QString & str);
    static PressedReleaseDelaysKeysMap fromStandardQMapToKeysSeqMap(const QMap<QString,QVariant> &standardMap);
    static QString fromKeysSeqMapToPrintedString(const PressedReleaseDelaysKeysMap &map);
    static QMap<QString, WORD> m_keysStrToInputWordMap;
    static QString fromCursorMovsMapToPrintedString(const DelaysMovementsMap &map);
    static DelaysMovementsMap fromStandardQMapToCursorMovsMap(const QMap<QString,QVariant> &standardMap);
};

#endif // ACTIONSTOOLS_H
