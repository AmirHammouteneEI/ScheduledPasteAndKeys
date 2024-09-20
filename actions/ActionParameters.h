#ifndef ACTIONPARAMETERS_H
#define ACTIONPARAMETERS_H

#include <QMetaType>
#include <QStringList>
#include <QMap>
#include <QPair>

typedef QPair<int,QStringList> ReleaseDelayKeysPair;// release delay in milisecond, list of keys as strings
typedef QMap<int, ReleaseDelayKeysPair> PressedReleaseDelaysKeysMap;// pressed time in milisecond, pair of release delay, list of keys
Q_DECLARE_METATYPE(ReleaseDelayKeysPair);
Q_DECLARE_METATYPE(PressedReleaseDelaysKeysMap);
typedef QPair<int,int> CoordinatesPair;// screen x,y
typedef QPair<int,CoordinatesPair> MovementPair;// time in milisecond for the movement and its coordinate
typedef QMap<int, MovementPair> DelaysMovementsMap;// delays in milisecond and its movement (time and x,y)
Q_DECLARE_METATYPE(CoordinatesPair);
Q_DECLARE_METATYPE(MovementPair);
Q_DECLARE_METATYPE(DelaysMovementsMap);

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
    DelaysMovementsMap m_cursorMovementsMap;
};

Q_DECLARE_METATYPE(ActionParameters);

#endif // ACTIONPARAMETERS_H
