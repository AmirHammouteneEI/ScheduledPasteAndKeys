#ifndef KEYSSEQUENCEACTION_H
#define KEYSSEQUENCEACTION_H

#include "AbstractAction.h"

class KeysSequenceAction : public AbstractAction
{
    Q_OBJECT
private:
    PressedReleaseDelaysKeysMap m_keysSeqMap;
    QString m_sequenceId;
    QMap<int, QVector<QStringList>> m_keysStrokeTimeline; // Timeline of keys to stroke, each time contain 2 vectors : [0] press keys list [1] released keys list
    int m_timesToRun = 1;
public:
    KeysSequenceAction();
    ~KeysSequenceAction() = default;
    KeysSequenceAction(const KeysSequenceAction & other) = delete;
    KeysSequenceAction& operator=(const KeysSequenceAction & other) = delete;

    void runAction() override;
    void setParameters(const ActionParameters& param) override;
    std::shared_ptr<AbstractAction> deepCopy() const override;
    ActionParameters generateParameters() const override;

    void generateTimeline();
    void optionalProcesses() override {generateTimeline();}
    int computeOneExecutionDuration(); // in miliseconds

    friend class KeysSequenceWidget;
};

#endif // KEYSSEQUENCEACTION_H
