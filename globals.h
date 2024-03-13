#ifndef GLOBALS_H
#define GLOBALS_H
#include <QString>
#include <QObject>

namespace G_Files {
    inline QString SettingsFilePath = "programSettings.ini";
    inline QString DataFilePath = "programData.ini";
    inline QString SentencesDataCategory = "sentences/";
    inline QString DocumentIdentification_KeyWord = "docType";
    inline QString DocumentIdentification_Value = "ScheduleTask File";
    inline QString DocumentTaskName_KeyWord = "taskName";
    inline QString DocumentActionsArray_KeyWord = "actions";
    inline QString ActionType_KeyWord = "type";
    inline QString ActionPasteType_Value = "paste";
    inline QString ActionWaitType_Value = "wait";
    inline QString ActionContent_KeyWord = "content";
    inline QString ActionContentId_KeyWord = "contentId";
    inline QString ActionDuration_KeyWord = "duration";
}

namespace G_Sentences {
    static QString AlreadyExists() {return QObject::tr("A task file with the chosen name already exists, do you confirm that you would like to overwrite this one ?");}
    static QString NoFileSelected() {return QObject::tr("No file has been selected from the table of saved files above. Please select one.");}
    static QString OperationInterference() {return QObject::tr("Some system or user action interferes with this operation, sorry for the inconvenience.");}
    static QString FileParsingError() {return QObject::tr("The file is not in the correct format for this software : parse error.");}
    static QString NoSetenceSelected() {return QObject::tr("No sentence has been selected. Please select one.");}
}
#endif // GLOBALS_H
