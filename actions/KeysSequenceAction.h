#ifndef KEYSSEQUENCEACTION_H
#define KEYSSEQUENCEACTION_H

#include "AbstractAction.h"
#include <QStringList>
#include <QMap>
#include <QPair>

typedef QPair<int,QStringList> ReleaseDelayKeysPair;// release delay in milisecond, list of keys as strings
Q_DECLARE_METATYPE(ReleaseDelayKeysPair);

class KeysSequenceAction : public AbstractAction
{
private:
    QMap<int, ReleaseDelayKeysPair> m_keysSeqMap; // pressed time in milisecond, pair of release delay, list of keys
    QString m_sequenceId;
public:
    KeysSequenceAction();
    ~KeysSequenceAction() = default;
    KeysSequenceAction(const KeysSequenceAction & other) = delete;
    KeysSequenceAction& operator=(const KeysSequenceAction & other) = delete;

    void runAction() const override;
    void setParameters(const ActionParameters& param) override;
    KeysSequenceAction *deepCopy() const override;
    ActionParameters generateParameters() const override;

    //TODO friend class KeysSequenceWidget;
};

#endif // KEYSSEQUENCEACTION_H
