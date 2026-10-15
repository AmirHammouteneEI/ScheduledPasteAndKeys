#ifndef GLOBALS_H
#define GLOBALS_H
#include <QString>
#include <QObject>

namespace G_Files {
    inline const QString SettingsFilePath = "programSettings.ini";
    inline const QString DataFilePath = "programData.ini";
    inline const QString SentencesDataCategory = "sentences/";
    inline const QString KeysSequencesDataCategory = "keyssequences/";
    inline const QString CursorMovementsDataCategory = "cursormovements/";
    inline const QString DocumentIdentification_KeyWord = "docType";
    inline const QString DocumentIdentification_Value = "ScheduleTask File";
    inline const QString DocumentTaskDescription_KeyWord = "description";
    inline const QString DocumentActionsArray_KeyWord = "actions";
    inline const QString ActionType_KeyWord = "type";
    inline const QString ActionPasteType_Value = "paste";
    inline const QString ActionWaitType_Value = "wait";
    inline const QString ActionSystemCommandeType_Value = "systemcommand";
    inline const QString ActionKeysSequenceType_Value = "keyssequence";
    inline const QString ActionCursorMovementsType_Value = "cursormovements";
    inline const QString ActionRunningOtherTaskType_Value = "runningothertask";
    inline const QString ActionContent_KeyWord = "content";
    inline const QString ActionContentId_KeyWord = "contentId";
    inline const QString ActionPasteTextLoop_KeyWord = "loop";
    inline const QString ActionDuration_KeyWord = "duration";
    inline const QString ActionKeysSeqMap_KeyWord = "keysmap";
    inline const QString ActionKeysSeqId_KeyWord = "keysSeqId";
    inline const QString ActionKeysSeqLoop_KeyWord = "loop";
    inline const QString ActionSysCommandType_KeyWord = "sysCommandType";
    inline const QString ActionSysCommandParam1_KeyWord = "sysCommandParam1";
    inline const QString ActionSysCommandParam2_KeyWord = "sysCommandParam2";
    inline const QString ActionCursorMovsMap_KeyWord = "cursormovsmap";
    inline const QString ActionCursorMovsId_KeyWord = "cursorMovsId";
    inline const QString ActionCursorMovsLoop_KeyWord = "loop";
    inline const QString ActionCursorMovsOptKeysStroke_KeyWord = "optionalkeysstroke";
    inline const QString RunningOtherTaskFilename_KeyWord = "otherTaskName";
    inline const QString RunningOtherTaskDelay_KeyWord = "otherTaskDelay";
    inline const QString RunningOtherTaskLoops_KeyWord = "otherTaskLoops";
    inline const QString TasksFolder = "saved_tasks/";
    inline const QString TasksFileExtension = ".scht";
    inline const QString StartupTasksParams_KeyWord = "startupTasksParams";
    inline const QString SystemStartupRegistry_Path = "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
    inline const QString ProgramNameInRegistry_KeyWord = "Tasket++";
}

namespace G_Sentences {
    static QString AlreadyExists() {return QObject::tr("A task file with the chosen name already exists, do you confirm that you would like to overwrite this one ?");}
    static QString NoFileSelected() {return QObject::tr("No file has been selected from the table of saved files above. Please select one.");}
    static QString OperationInterference() {return QObject::tr("Some system or user action interferes with this operation, sorry for the inconvenience.");}
    static QString ForbiddenCharacter() {return QObject::tr("The filename contains some forbidden characters (<>:/\"|?*\\), please change it.");}
    static QString FileParsingError() {return QObject::tr("The file is not in the correct format for this software : parse error.");}
    static QString NoSetenceSelected() {return QObject::tr("No sentence has been selected. Please select one.");}
    static QString NoKeysSequenceSelected() {return QObject::tr("No keys sequence has been selected. Please select one.");}
    static QString NoCursorMovementsSelected() {return QObject::tr("No cursor movements set has been selected. Please select one.");}
    static QString NoStartupTaskSelected() {return QObject::tr("No startup task has been selected. Please select one.");}
    inline const QString AutoRenameFileOption = "Auto-rename if exists";
    inline const QString EraseFileOption = "Erase if exists";
}

namespace G_SystemCommands {
    inline const QString UndefinedType = "undefined";
    inline const QString ShutDownType = "shutdown";
    inline const QString RestartType = "restart";
    inline const QString LogOffType = "logoff";
    inline const QString ChangeAudioVolumeType = "changevolume";
    inline const QString ChangeDefaultAudioDeviceType = "changeaudiodevice";
    inline const QString KillProcessType = "killprocess";
    inline const QString QuitSelfProgramType = "quitselfprogram";
    inline const QString CreateFolderType = "createfolder";
    inline const QString DeleteFolderType = "deletefolder";
    inline const QString CreateFileType = "createfile";
    inline const QString DeleteFileType = "deletefile";
    inline const QString TakeScreenshotType = "screenshot";
    inline const QString PrintActualScreenType = "printscreen";
    inline const QString OpenFileType = "openfile";
    inline const QString ExecuteProgramType = "executeprogram";
    inline const QString OpenUrlType = "openurl";
    inline const QString OpenFolderType = "openfolder";
    inline const QString CopyFileType = "copyfile";
}

struct G_Parameters {
    static bool AutoScrollTask;
};

#endif // GLOBALS_H
