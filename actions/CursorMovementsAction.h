#ifndef CURSORMOVEMENTSACTION_H
#define CURSORMOVEMENTSACTION_H

#include "AbstractAction.h"

class CursorMovementsAction : public AbstractAction
{
    Q_OBJECT
private:
    CursorMovementsList m_cursorMovementsList;
    QString m_movementsId;
    int m_timesToRun = 1;
    QStringList m_cursorMovementsOptionalKeysStroke;
public:
    CursorMovementsAction();
    ~CursorMovementsAction() = default;
    CursorMovementsAction(const CursorMovementsAction & other) = delete;
    CursorMovementsAction& operator=(const CursorMovementsAction & other) = delete;

    void runAction() override;
    void setParameters(const ActionParameters& param) override;
    std::shared_ptr<AbstractAction> deepCopy() const override;
    ActionParameters generateParameters() const override;

    void optionalProcesses() override {}
    int computeOneExecutionDuration(); // in miliseconds

    friend class CursorMovementsWidget;
};

#endif // CURSORMOVEMENTSACTION_H
