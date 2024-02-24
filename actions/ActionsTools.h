#ifndef ACTIONSTOOLS_H
#define ACTIONSTOOLS_H

#include <qt_windows.h>
#include <winuser.h>
#include <QString>

class ActionsTools
{
public:
    ActionsTools();

    static void pasteKeystrokeSimulate();
    static void setClipboard(const QString & str);
};

#endif // ACTIONSTOOLS_H
