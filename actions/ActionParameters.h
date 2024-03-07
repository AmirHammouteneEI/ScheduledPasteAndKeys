#ifndef ACTIONPARAMETERS_H
#define ACTIONPARAMETERS_H

#include <QMetaType>

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
};

Q_DECLARE_METATYPE(ActionParameters);

#endif // ACTIONPARAMETERS_H
