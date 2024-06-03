#ifndef KEYSSEQUENCEACTION_H
#define KEYSSEQUENCEACTION_H

#include "AbstractAction.h"
#include <QStringList>
#include <QMap>
#include <QPair>

class KeysSequenceAction : public AbstractAction
{
private:
    PressedReleaseDelaysKeysMap m_keysSeqMap;
    QString m_sequenceId;
    QMap<int, QVector<QStringList>> m_keysStrokeTimeline; // Timeline of keys to stroke, each time contain 2 vectors : [0] press keys list [1] released keys list
public:
    KeysSequenceAction();
    ~KeysSequenceAction() = default;
    KeysSequenceAction(const KeysSequenceAction & other) = delete;
    KeysSequenceAction& operator=(const KeysSequenceAction & other) = delete;

    void runAction() const override;
    void setParameters(const ActionParameters& param) override;
    KeysSequenceAction *deepCopy() const override;
    ActionParameters generateParameters() const override;

    void generateTimeline();
    void optionalProcesses() override {generateTimeline();}

    friend class KeysSequenceWidget;
};

#endif // KEYSSEQUENCEACTION_H
