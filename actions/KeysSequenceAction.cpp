#include "KeysSequenceAction.h"

KeysSequenceAction::KeysSequenceAction() : AbstractAction()
{
    m_type = ActionType::KeysSequence;
}

void KeysSequenceAction::runAction() const
{

}

void KeysSequenceAction::setParameters(const ActionParameters &param)
{
    m_keysSeqMap = param.m_keysSeqMap;
    m_sequenceId = param.m_dataId;
}

KeysSequenceAction *KeysSequenceAction::deepCopy() const
{
    KeysSequenceAction *actToReturn = new KeysSequenceAction();
    actToReturn->m_keysSeqMap = m_keysSeqMap;
    actToReturn->m_sequenceId = m_sequenceId;
    actToReturn->m_refID = m_ID;
    return actToReturn;
}

ActionParameters KeysSequenceAction::generateParameters() const
{
    ActionParameters param;
    param.m_keysSeqMap = m_keysSeqMap;
    param.m_dataId = m_sequenceId;
    return param;
}
