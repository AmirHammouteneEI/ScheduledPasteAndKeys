#ifndef ACTIONPARAMETERS_H
#define ACTIONPARAMETERS_H

#include <QMetaType>
#include <QStringList>
#include <QMap>
#include <QPair>
#include <QList>

typedef QPair<int,QStringList> ReleaseDelayKeysPair;// release delay in milisecond, list of keys as strings
typedef QMap<int, ReleaseDelayKeysPair> PressedReleaseDelaysKeysMap;// pressed time in milisecond, pair of release delay, list of keys
Q_DECLARE_METATYPE(ReleaseDelayKeysPair);
Q_DECLARE_METATYPE(PressedReleaseDelaysKeysMap);
typedef QList<int> MovementList;// delays (from previous move) in milisecond, moving time in milisecond, screen x,y (4 elements)
typedef QList<MovementList> CursorMovementsList;// delays in milisecond and its movement (time and x,y)
Q_DECLARE_METATYPE(MovementList);
Q_DECLARE_METATYPE(CursorMovementsList);

class ActionParameters
{
public:
    ActionParameters() = default;
    ~ActionParameters() = default;
    ActionParameters(const ActionParameters &) = default;
    ActionParameters &operator=(const ActionParameters &) = default;

    QString m_dataId;
    QString m_pasteContent;
    long double m_waitDuration; // in seconds
    // map with key = delay of key simulated pressed in milisecond, value = pair of release delay (ms) and list of keys label
    PressedReleaseDelaysKeysMap m_keysSeqMap;
    int m_timesToRun = 1;
    QString m_sysCmdTypeStr;
    QString m_sysCmdParam1;
    QString m_sysCmdParam2;
    CursorMovementsList m_cursorMovementsList;
    QStringList m_cursorMovementsOptionalKeysStroke;
    QString m_taskName;
    int m_delay; // in seconds
};

Q_DECLARE_METATYPE(ActionParameters);

#endif // ACTIONPARAMETERS_H
