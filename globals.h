#ifndef GLOBALS_H
#define GLOBALS_H
#include <QString>
#include <QObject>

namespace G_Files {
    inline QString SettingsFilePath = "programSettings.ini";
    inline QString DataFilePath = "programData.ini";
    inline QString SentencesDataCategory = "sentences/";
    inline QString KeysSequencesDataCategory = "keyssequences/";
    inline QString CursorMovementsDataCategory = "cursormovements/";
    inline QString DocumentIdentification_KeyWord = "docType";
    inline QString DocumentIdentification_Value = "ScheduleTask File";
    inline QString DocumentTaskDescription_KeyWord = "description";
    inline QString DocumentActionsArray_KeyWord = "actions";
    inline QString ActionType_KeyWord = "type";
    inline QString ActionPasteType_Value = "paste";
    inline QString ActionWaitType_Value = "wait";
    inline QString ActionSystemCommandeType_Value = "systemcommand";
    inline QString ActionKeysSequenceType_Value = "keyssequence";
    inline QString ActionCursorMovementsType_Value = "cursormovements";
    inline QString ActionRunningOtherTaskType_Value = "runningothertask";
    inline QString ActionContent_KeyWord = "content";
    inline QString ActionContentId_KeyWord = "contentId";
    inline QString ActionPasteTextLoop_KeyWord = "loop";
    inline QString ActionDuration_KeyWord = "duration";
    inline QString ActionKeysSeqMap_KeyWord = "keysmap";
    inline QString ActionKeysSeqId_KeyWord = "keysSeqId";
    inline QString ActionKeysSeqLoop_KeyWord = "loop";
    inline QString ActionSysCommandType_KeyWord = "sysCommandType";
    inline QString ActionSysCommandParam1_KeyWord = "sysCommandParam1";
    inline QString ActionSysCommandParam2_KeyWord = "sysCommandParam2";
    inline QString ActionCursorMovsMap_KeyWord = "cursormovsmap";
    inline QString ActionCursorMovsId_KeyWord = "cursorMovsId";
    inline QString ActionCursorMovsLoop_KeyWord = "loop";
    inline QString ActionCursorMovsOptKeysStroke_KeyWord = "optionalkeysstroke";
    inline QString RunningOtherTaskFilename_KeyWord = "otherTaskName";
    inline QString RunningOtherTaskDelay_KeyWord = "otherTaskDelay";
    inline QString RunningOtherTaskLoops_KeyWord = "otherTaskLoops";
    inline QString TasksFolder = "saved_tasks/";
    inline QString TasksFileExtension = ".scht";
    inline QString StartupTasksParams_KeyWord = "startupTasksParams";
    inline QString SystemStartupRegistry_Path = "HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
    inline QString ProgramNameInRegistry_KeyWord = "Tasket++";
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
}

namespace G_SystemCommands {
    inline QString UndefinedType = "undefined";
    inline QString ShutDownType = "shutdown";
    inline QString RestartType = "restart";
    inline QString LogOffType = "logoff";
    inline QString ChangeAudioVolumeType = "changevolume";
    inline QString ChangeDefaultAudioDeviceType = "changeaudiodevice";
    inline QString KillProcessType = "killprocess";
    inline QString QuitSelfProgramType = "quitselfprogram";
    inline QString CreateFolderType = "createfolder";
    inline QString DeleteFolderType = "deletefolder";
    inline QString CreateFileType = "createfile";
    inline QString DeleteFileType = "deletefile";
    inline QString TakeScreenshotType = "screenshot";
    inline QString PrintActualScreenType = "printscreen";
    inline QString OpenFileType = "openfile";
    inline QString ExecuteProgramType = "executeprogram";
    inline QString OpenUrlType = "openurl";
    inline QString OpenFolderType = "openfolder";
    inline QString CopyFileType = "copyfile";
}

struct G_Parameters {
    static bool AutoScrollTask;
};

#endif // GLOBALS_H
