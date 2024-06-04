#include "KeysSequenceAction.h"
#include "actions/ActionsTools.h"

KeysSequenceAction::KeysSequenceAction() : AbstractAction()
{
    m_type = ActionType::KeysSequence;
}

void KeysSequenceAction::runAction() const
{
    int timesToRun = m_timesToRun;

    begin:

    auto it = m_keysStrokeTimeline.begin();
    while(it != m_keysStrokeTimeline.end())
    {
        int time = it.key();
        auto vecOfStrokes = it.value();
        if(vecOfStrokes.size() < 2)
            continue;

        for(auto& pressStr : vecOfStrokes[0])
            ActionsTools::keyStokeSimulate(pressStr, 0);

        for(auto& releaseStr : vecOfStrokes[1])
            ActionsTools::keyStokeSimulate(releaseStr, KEYEVENTF_KEYUP);
        ++it;
        if(it != m_keysStrokeTimeline.end())
            Sleep(it.key() - time);
    }

    Sleep(100);

    --timesToRun;
    if(timesToRun > 0)
        goto begin;
}

void KeysSequenceAction::setParameters(const ActionParameters &param)
{
    m_keysSeqMap = param.m_keysSeqMap;
    m_sequenceId = param.m_dataId;
    m_timesToRun = param.m_timesToRun;
}

KeysSequenceAction *KeysSequenceAction::deepCopy() const
{
    KeysSequenceAction *actToReturn = new KeysSequenceAction();
    actToReturn->m_keysSeqMap = m_keysSeqMap;
    actToReturn->m_sequenceId = m_sequenceId;
    actToReturn->m_keysStrokeTimeline = m_keysStrokeTimeline;
    actToReturn->m_timesToRun = m_timesToRun;
    actToReturn->m_refID = m_ID;
    return actToReturn;
}

ActionParameters KeysSequenceAction::generateParameters() const
{
    ActionParameters param;
    param.m_keysSeqMap = m_keysSeqMap;
    param.m_dataId = m_sequenceId;
    param.m_timesToRun = m_timesToRun;
    return param;
}

void KeysSequenceAction::generateTimeline()
{
    m_keysStrokeTimeline.clear();
    for(auto [key,val] : m_keysSeqMap.asKeyValueRange())
    {
        int releaseKey = key + val.first; // time + release after time

        if(m_keysStrokeTimeline.contains(key)) // in that case, we append keys in the press stroke keys list
        {
            auto vecOfStrokes = m_keysStrokeTimeline.value(key);
            if(vecOfStrokes.size() == 2)
            {
                vecOfStrokes[0].append(val.second);
                m_keysStrokeTimeline.insert(key, vecOfStrokes);
            }
        }
        else // else we create the entry with the pressed keys
        {
            QVector<QStringList> vecToInsert;
            vecToInsert.append(val.second);
            vecToInsert.append(QStringList());
            m_keysStrokeTimeline.insert(key,vecToInsert);
        }

        if(m_keysStrokeTimeline.contains(releaseKey)) // in that case, we append keys in the release stroke keys list
        {
            auto vecOfStrokes = m_keysStrokeTimeline.value(releaseKey);
            if(vecOfStrokes.size() == 2)
            {
                vecOfStrokes[1].append(val.second);
                m_keysStrokeTimeline.insert(releaseKey, vecOfStrokes);
            }
        }
        else // else we create the entry with the release keys
        {
            QVector<QStringList> vecToInsert;
            vecToInsert.append(QStringList());
            vecToInsert.append(val.second);
            m_keysStrokeTimeline.insert(releaseKey,vecToInsert);
        }
    }
}

int KeysSequenceAction::computeOneExecutionDuration()
{
    int returnedValue = 0;

    for(auto [key,val] : m_keysSeqMap.asKeyValueRange())
    {
        int thisKeyStops = key + val.first;
        if(thisKeyStops > returnedValue)
            returnedValue = thisKeyStops;
    }

    return returnedValue;
}
