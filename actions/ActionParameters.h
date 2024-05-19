#ifndef ACTIONPARAMETERS_H
#define ACTIONPARAMETERS_H

#include <QMetaType>
#include <QStringList>
#include <QMap>
#include <QPair>

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
    QMap<int, QPair<int, QStringList>> m_keysSeqMap;
};

Q_DECLARE_METATYPE(ActionParameters);

#endif // ACTIONPARAMETERS_H
