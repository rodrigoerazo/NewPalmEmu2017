#include <unistd.h>
#include <stdio.h>
#include <time.h>

#include "palmwrapper.h"

#include "palmapi.h"

#include "palmdatatypes.h"
#include "eventqueue.h"
#include "sdfilesystem.h"
#include "dataexchange.h"
#include "palmdefines.h"
#include "armv5te.h"

//new list
#include "m68k.h"
#include "resourcelocator.h"
#include "astrology.h"
#include "datamanager.h"
#include "displaydriver.h"
#include "audiodriver.h"
#include "floatlib.h"
#include "stdlib68k.h"


vector<feature> featuretable;

//from mame used for printing apicalls
typedef struct{
	const char *name;
	UWORD trap;
}oocak;

oocak traps[] ={
    { "sysTrapMemInit", 0xA000 },
    { "sysTrapMemInitHeapTable", 0xA001 },
    { "sysTrapMemStoreInit", 0xA002 },
    { "sysTrapMemCardFormat", 0xA003 },
    { "sysTrapMemCardInfo", 0xA004 },
    { "sysTrapMemStoreInfo", 0xA005 },
    { "sysTrapMemStoreSetInfo", 0xA006 },
    { "sysTrapMemNumHeaps", 0xA007 },
    { "sysTrapMemNumRAMHeaps", 0xA008 },
    { "sysTrapMemHeapID", 0xA009 },
    { "sysTrapMemHeapPtr", 0xA00A },
    { "sysTrapMemHeapFreeBytes", 0xA00B },
    { "sysTrapMemHeapSize", 0xA00C },
    { "sysTrapMemHeapFlags", 0xA00D },
    { "sysTrapMemHeapCompact", 0xA00E },
    { "sysTrapMemHeapInit", 0xA00F },
    { "sysTrapMemHeapFreeByOwnerID", 0xA010 },
    { "sysTrapMemChunkNew", 0xA011 },
    { "sysTrapMemChunkFree", 0xA012 },
    { "sysTrapMemPtrNew", 0xA013 },
    { "sysTrapMemPtrRecoverHandle", 0xA014 },
    { "sysTrapMemPtrFlags", 0xA015 },
    { "sysTrapMemPtrSize", 0xA016 },
    { "sysTrapMemPtrOwner", 0xA017 },
    { "sysTrapMemPtrHeapID", 0xA018 },
    { "sysTrapMemPtrCardNo", 0xA019 },
    { "sysTrapMemPtrToLocalID", 0xA01A },
    { "sysTrapMemPtrSetOwner", 0xA01B },
    { "sysTrapMemPtrResize", 0xA01C },
    { "sysTrapMemPtrResetLock", 0xA01D },
    { "sysTrapMemHandleNew", 0xA01E },
    { "sysTrapMemHandleLockCount", 0xA01F },
    { "sysTrapMemHandleToLocalID", 0xA020 },
    { "sysTrapMemHandleLock", 0xA021 },
    { "sysTrapMemHandleUnlock", 0xA022 },
    { "sysTrapMemLocalIDToGlobal", 0xA023 },
    { "sysTrapMemLocalIDKind", 0xA024 },
    { "sysTrapMemLocalIDToPtr", 0xA025 },
    { "sysTrapMemMove", 0xA026 },
    { "sysTrapMemSet", 0xA027 },
    { "sysTrapMemStoreSearch", 0xA028 },
    { "sysTrapSysReserved10Trap1", 0xA029 },
    { "sysTrapMemKernelInit", 0xA02A },
    { "sysTrapMemHandleFree", 0xA02B },
    { "sysTrapMemHandleFlags", 0xA02C },
    { "sysTrapMemHandleSize", 0xA02D },
    { "sysTrapMemHandleOwner", 0xA02E },
    { "sysTrapMemHandleHeapID", 0xA02F },
    { "sysTrapMemHandleDataStorage", 0xA030 },
    { "sysTrapMemHandleCardNo", 0xA031 },
    { "sysTrapMemHandleSetOwner", 0xA032 },
    { "sysTrapMemHandleResize", 0xA033 },
    { "sysTrapMemHandleResetLock", 0xA034 },
    { "sysTrapMemPtrUnlock", 0xA035 },
    { "sysTrapMemLocalIDToLockedPtr", 0xA036 },
    { "sysTrapMemSetDebugMode", 0xA037 },
    { "sysTrapMemHeapScramble", 0xA038 },
    { "sysTrapMemHeapCheck", 0xA039 },
    { "sysTrapMemNumCards", 0xA03A },
    { "sysTrapMemDebugMode", 0xA03B },
    { "sysTrapMemSemaphoreReserve", 0xA03C },
    { "sysTrapMemSemaphoreRelease", 0xA03D },
    { "sysTrapMemHeapDynamic", 0xA03E },
    { "sysTrapMemNVParams", 0xA03F },
    { "sysTrapDmInit", 0xA040 },
    { "sysTrapDmCreateDatabase", 0xA041 },
    { "sysTrapDmDeleteDatabase", 0xA042 },
    { "sysTrapDmNumDatabases", 0xA043 },
    { "sysTrapDmGetDatabase", 0xA044 },
    { "sysTrapDmFindDatabase", 0xA045 },
    { "sysTrapDmDatabaseInfo", 0xA046 },
    { "sysTrapDmSetDatabaseInfo", 0xA047 },
    { "sysTrapDmDatabaseSize", 0xA048 },
    { "sysTrapDmOpenDatabase", 0xA049 },
    { "sysTrapDmCloseDatabase", 0xA04A },
    { "sysTrapDmNextOpenDatabase", 0xA04B },
    { "sysTrapDmOpenDatabaseInfo", 0xA04C },
    { "sysTrapDmResetRecordStates", 0xA04D },
    { "sysTrapDmGetLastErr", 0xA04E },
    { "sysTrapDmNumRecords", 0xA04F },
    { "sysTrapDmRecordInfo", 0xA050 },
    { "sysTrapDmSetRecordInfo", 0xA051 },
    { "sysTrapDmAttachRecord", 0xA052 },
    { "sysTrapDmDetachRecord", 0xA053 },
    { "sysTrapDmMoveRecord", 0xA054 },
    { "sysTrapDmNewRecord", 0xA055 },
    { "sysTrapDmRemoveRecord", 0xA056 },
    { "sysTrapDmDeleteRecord", 0xA057 },
    { "sysTrapDmArchiveRecord", 0xA058 },
    { "sysTrapDmNewHandle", 0xA059 },
    { "sysTrapDmRemoveSecretRecords", 0xA05A },
    { "sysTrapDmQueryRecord", 0xA05B },
    { "sysTrapDmGetRecord", 0xA05C },
    { "sysTrapDmResizeRecord", 0xA05D },
    { "sysTrapDmReleaseRecord", 0xA05E },
    { "sysTrapDmGetResource", 0xA05F },
    { "sysTrapDmGet1Resource", 0xA060 },
    { "sysTrapDmReleaseResource", 0xA061 },
    { "sysTrapDmResizeResource", 0xA062 },
    { "sysTrapDmNextOpenResDatabase", 0xA063 },
    { "sysTrapDmFindResourceType", 0xA064 },
    { "sysTrapDmFindResource", 0xA065 },
    { "sysTrapDmSearchResource", 0xA066 },
    { "sysTrapDmNumResources", 0xA067 },
    { "sysTrapDmResourceInfo", 0xA068 },
    { "sysTrapDmSetResourceInfo", 0xA069 },
    { "sysTrapDmAttachResource", 0xA06A },
    { "sysTrapDmDetachResource", 0xA06B },
    { "sysTrapDmNewResource", 0xA06C },
    { "sysTrapDmRemoveResource", 0xA06D },
    { "sysTrapDmGetResourceIndex", 0xA06E },
    { "sysTrapDmQuickSort", 0xA06F },
    { "sysTrapDmQueryNextInCategory", 0xA070 },
    { "sysTrapDmNumRecordsInCategory", 0xA071 },
    { "sysTrapDmPositionInCategory", 0xA072 },
    { "sysTrapDmSeekRecordInCategory", 0xA073 },
    { "sysTrapDmMoveCategory", 0xA074 },
    { "sysTrapDmOpenDatabaseByTypeCreator", 0xA075 },
    { "sysTrapDmWrite", 0xA076 },
    { "sysTrapDmStrCopy", 0xA077 },
    { "sysTrapDmGetNextDatabaseByTypeCreator", 0xA078 },
    { "sysTrapDmWriteCheck", 0xA079 },
    { "sysTrapDmMoveOpenDBContext", 0xA07A },
    { "sysTrapDmFindRecordByID", 0xA07B },
    { "sysTrapDmGetAppInfoID", 0xA07C },
    { "sysTrapDmFindSortPositionV10", 0xA07D },
    { "sysTrapDmSet", 0xA07E },
    { "sysTrapDmCreateDatabaseFromImage", 0xA07F },
    { "sysTrapDbgSrcMessage", 0xA080 },
    { "sysTrapDbgMessage", 0xA081 },
    { "sysTrapDbgGetMessage", 0xA082 },
    { "sysTrapDbgCommSettings", 0xA083 },
    { "sysTrapErrDisplayFileLineMsg", 0xA084 },
    { "sysTrapErrSetJump", 0xA085 },
    { "sysTrapErrLongJump", 0xA086 },
    { "sysTrapErrThrow", 0xA087 },
    { "sysTrapErrExceptionList", 0xA088 },
    { "sysTrapSysBroadcastActionCode", 0xA089 },
    { "sysTrapSysUnimplemented", 0xA08A },
    { "sysTrapSysColdBoot", 0xA08B },
    { "sysTrapSysReset", 0xA08C },
    { "sysTrapSysDoze", 0xA08D },
    { "sysTrapSysAppLaunch", 0xA08E },
    { "sysTrapSysAppStartup", 0xA08F },
    { "sysTrapSysAppExit", 0xA090 },
    { "sysTrapSysSetA5", 0xA091 },
    { "sysTrapSysSetTrapAddress", 0xA092 },
    { "sysTrapSysGetTrapAddress", 0xA093 },
    { "sysTrapSysTranslateKernelErr", 0xA094 },
    { "sysTrapSysSemaphoreCreate", 0xA095 },
    { "sysTrapSysSemaphoreDelete", 0xA096 },
    { "sysTrapSysSemaphoreWait", 0xA097 },
    { "sysTrapSysSemaphoreSignal", 0xA098 },
    { "sysTrapSysTimerCreate", 0xA099 },
    { "sysTrapSysTimerWrite", 0xA09A },
    { "sysTrapSysTaskCreate", 0xA09B },
    { "sysTrapSysTaskDelete", 0xA09C },
    { "sysTrapSysTaskTrigger", 0xA09D },
    { "sysTrapSysTaskID", 0xA09E },
    { "sysTrapSysTaskUserInfoPtr", 0xA09F },
    { "sysTrapSysTaskDelay", 0xA0A0 },
    { "sysTrapSysTaskSetTermProc", 0xA0A1 },
    { "sysTrapSysUILaunch", 0xA0A2 },
    { "sysTrapSysNewOwnerID", 0xA0A3 },
    { "sysTrapSysSemaphoreSet", 0xA0A4 },
    { "sysTrapSysDisableInts", 0xA0A5 },
    { "sysTrapSysRestoreStatus", 0xA0A6 },
    { "sysTrapSysUIAppSwitch", 0xA0A7 },
    { "sysTrapSysCurAppInfoPV20", 0xA0A8 },
    { "sysTrapSysHandleEvent", 0xA0A9 },
    { "sysTrapSysInit", 0xA0AA },
    { "sysTrapSysQSort", 0xA0AB },
    { "sysTrapSysCurAppDatabase", 0xA0AC },
    { "sysTrapSysFatalAlert", 0xA0AD },
    { "sysTrapSysResSemaphoreCreate", 0xA0AE },
    { "sysTrapSysResSemaphoreDelete", 0xA0AF },
    { "sysTrapSysResSemaphoreReserve", 0xA0B0 },
    { "sysTrapSysResSemaphoreRelease", 0xA0B1 },
    { "sysTrapSysSleep", 0xA0B2 },
    { "sysTrapSysKeyboardDialogV10", 0xA0B3 },
    { "sysTrapSysAppLauncherDialog", 0xA0B4 },
    { "sysTrapSysSetPerformance", 0xA0B5 },
    { "sysTrapSysBatteryInfoV20", 0xA0B6 },
    { "sysTrapSysLibInstall", 0xA0B7 },
    { "sysTrapSysLibRemove", 0xA0B8 },
    { "sysTrapSysLibTblEntry", 0xA0B9 },
    { "sysTrapSysLibFind", 0xA0BA },
    { "sysTrapSysBatteryDialog", 0xA0BB },
    { "sysTrapSysCopyStringResource", 0xA0BC },
    { "sysTrapSysKernelInfo", 0xA0BD },
    { "sysTrapSysLaunchConsole", 0xA0BE },
    { "sysTrapSysTimerDelete", 0xA0BF },
    { "sysTrapSysSetAutoOffTime", 0xA0C0 },
    { "sysTrapSysFormPointerArrayToStrings", 0xA0C1 },
    { "sysTrapSysRandom", 0xA0C2 },
    { "sysTrapSysTaskSwitching", 0xA0C3 },
    { "sysTrapSysTimerRead", 0xA0C4 },
    { "sysTrapStrCopy", 0xA0C5 },
    { "sysTrapStrCat", 0xA0C6 },
    { "sysTrapStrLen", 0xA0C7 },
    { "sysTrapStrCompare", 0xA0C8 },
    { "sysTrapStrIToA", 0xA0C9 },
    { "sysTrapStrCaselessCompare", 0xA0CA },
    { "sysTrapStrIToH", 0xA0CB },
    { "sysTrapStrChr", 0xA0CC },
    { "sysTrapStrStr", 0xA0CD },
    { "sysTrapStrAToI", 0xA0CE },
    { "sysTrapStrToLower", 0xA0CF },
    { "sysTrapSerReceiveISP", 0xA0D0 },
    { "sysTrapSlkOpen", 0xA0D1 },
    { "sysTrapSlkClose", 0xA0D2 },
    { "sysTrapSlkOpenSocket", 0xA0D3 },
    { "sysTrapSlkCloseSocket", 0xA0D4 },
    { "sysTrapSlkSocketRefNum", 0xA0D5 },
    { "sysTrapSlkSocketSetTimeout", 0xA0D6 },
    { "sysTrapSlkFlushSocket", 0xA0D7 },
    { "sysTrapSlkSetSocketListener", 0xA0D8 },
    { "sysTrapSlkSendPacket", 0xA0D9 },
    { "sysTrapSlkReceivePacket", 0xA0DA },
    { "sysTrapSlkSysPktDefaultResponse", 0xA0DB },
    { "sysTrapSlkProcessRPC", 0xA0DC },
    { "sysTrapConPutS", 0xA0DD },
    { "sysTrapConGetS", 0xA0DE },
    { "sysTrapFplInit", 0xA0DF },
    { "sysTrapFplFree", 0xA0E0 },
    { "sysTrapFplFToA", 0xA0E1 },
    { "sysTrapFplAToF", 0xA0E2 },
    { "sysTrapFplBase10Info", 0xA0E3 },
    { "sysTrapFplLongToFloat", 0xA0E4 },
    { "sysTrapFplFloatToLong", 0xA0E5 },
    { "sysTrapFplFloatToULong", 0xA0E6 },
    { "sysTrapFplMul", 0xA0E7 },
    { "sysTrapFplAdd", 0xA0E8 },
    { "sysTrapFplSub", 0xA0E9 },
    { "sysTrapFplDiv", 0xA0EA },
    { "sysTrapWinScreenInit", 0xA0EB },
    { "sysTrapScrCopyRectangle", 0xA0EC },
    { "sysTrapScrDrawChars", 0xA0ED },
    { "sysTrapScrLineRoutine", 0xA0EE },
    { "sysTrapScrRectangleRoutine", 0xA0EF },
    { "sysTrapScrScreenInfo", 0xA0F0 },
    { "sysTrapScrDrawNotify", 0xA0F1 },
    { "sysTrapScrSendUpdateArea", 0xA0F2 },
    { "sysTrapScrCompressScanLine", 0xA0F3 },
    { "sysTrapScrDeCompressScanLine", 0xA0F4 },
    { "sysTrapTimGetSeconds", 0xA0F5 },
    { "sysTrapTimSetSeconds", 0xA0F6 },
    { "sysTrapTimGetTicks", 0xA0F7 },
    { "sysTrapTimInit", 0xA0F8 },
    { "sysTrapTimSetAlarm", 0xA0F9 },
    { "sysTrapTimGetAlarm", 0xA0FA },
    { "sysTrapTimHandleInterrupt", 0xA0FB },
    { "sysTrapTimSecondsToDateTime", 0xA0FC },
    { "sysTrapTimDateTimeToSeconds", 0xA0FD },
    { "sysTrapTimAdjust", 0xA0FE },
    { "sysTrapTimSleep", 0xA0FF },
    { "sysTrapTimWake", 0xA100 },
    { "sysTrapCategoryCreateListV10", 0xA101 },
    { "sysTrapCategoryFreeListV10", 0xA102 },
    { "sysTrapCategoryFind", 0xA103 },
    { "sysTrapCategoryGetName", 0xA104 },
    { "sysTrapCategoryEditV10", 0xA105 },
    { "sysTrapCategorySelectV10", 0xA106 },
    { "sysTrapCategoryGetNext", 0xA107 },
    { "sysTrapCategorySetTriggerLabel", 0xA108 },
    { "sysTrapCategoryTruncateName", 0xA109 },
    { "sysTrapClipboardAddItem", 0xA10A },
    { "sysTrapClipboardCheckIfItemExist", 0xA10B },
    { "sysTrapClipboardGetItem", 0xA10C },
    { "sysTrapCtlDrawControl", 0xA10D },
    { "sysTrapCtlEraseControl", 0xA10E },
    { "sysTrapCtlHideControl", 0xA10F },
    { "sysTrapCtlShowControl", 0xA110 },
    { "sysTrapCtlGetValue", 0xA111 },
    { "sysTrapCtlSetValue", 0xA112 },
    { "sysTrapCtlGetLabel", 0xA113 },
    { "sysTrapCtlSetLabel", 0xA114 },
    { "sysTrapCtlHandleEvent", 0xA115 },
    { "sysTrapCtlHitControl", 0xA116 },
    { "sysTrapCtlSetEnabled", 0xA117 },
    { "sysTrapCtlSetUsable", 0xA118 },
    { "sysTrapCtlEnabled", 0xA119 },
    { "sysTrapEvtInitialize", 0xA11A },
    { "sysTrapEvtAddEventToQueue", 0xA11B },
    { "sysTrapEvtCopyEvent", 0xA11C },
    { "sysTrapEvtGetEvent", 0xA11D },
    { "sysTrapEvtGetPen", 0xA11E },
    { "sysTrapEvtSysInit", 0xA11F },
    { "sysTrapEvtGetSysEvent", 0xA120 },
    { "sysTrapEvtProcessSoftKeyStroke", 0xA121 },
    { "sysTrapEvtGetPenBtnList", 0xA122 },
    { "sysTrapEvtSetPenQueuePtr", 0xA123 },
    { "sysTrapEvtPenQueueSize", 0xA124 },
    { "sysTrapEvtFlushPenQueue", 0xA125 },
    { "sysTrapEvtEnqueuePenPoint", 0xA126 },
    { "sysTrapEvtDequeuePenStrokeInfo", 0xA127 },
    { "sysTrapEvtDequeuePenPoint", 0xA128 },
    { "sysTrapEvtFlushNextPenStroke", 0xA129 },
    { "sysTrapEvtSetKeyQueuePtr", 0xA12A },
    { "sysTrapEvtKeyQueueSize", 0xA12B },
    { "sysTrapEvtFlushKeyQueue", 0xA12C },
    { "sysTrapEvtEnqueueKey", 0xA12D },
    { "sysTrapEvtDequeueKeyEvent", 0xA12E },
    { "sysTrapEvtWakeup", 0xA12F },
    { "sysTrapEvtResetAutoOffTimer", 0xA130 },
    { "sysTrapEvtKeyQueueEmpty", 0xA131 },
    { "sysTrapEvtEnableGraffiti", 0xA132 },
    { "sysTrapFldCopy", 0xA133 },
    { "sysTrapFldCut", 0xA134 },
    { "sysTrapFldDrawField", 0xA135 },
    { "sysTrapFldEraseField", 0xA136 },
    { "sysTrapFldFreeMemory", 0xA137 },
    { "sysTrapFldGetBounds", 0xA138 },
    { "sysTrapFldGetTextPtr", 0xA139 },
    { "sysTrapFldGetSelection", 0xA13A },
    { "sysTrapFldHandleEvent", 0xA13B },
    { "sysTrapFldPaste", 0xA13C },
    { "sysTrapFldRecalculateField", 0xA13D },
    { "sysTrapFldSetBounds", 0xA13E },
    { "sysTrapFldSetText", 0xA13F },
    { "sysTrapFldGetFont", 0xA140 },
    { "sysTrapFldSetFont", 0xA141 },
    { "sysTrapFldSetSelection", 0xA142 },
    { "sysTrapFldGrabFocus", 0xA143 },
    { "sysTrapFldReleaseFocus", 0xA144 },
    { "sysTrapFldGetInsPtPosition", 0xA145 },
    { "sysTrapFldSetInsPtPosition", 0xA146 },
    { "sysTrapFldSetScrollPosition", 0xA147 },
    { "sysTrapFldGetScrollPosition", 0xA148 },
    { "sysTrapFldGetTextHeight", 0xA149 },
    { "sysTrapFldGetTextAllocatedSize", 0xA14A },
    { "sysTrapFldGetTextLength", 0xA14B },
    { "sysTrapFldScrollField", 0xA14C },
    { "sysTrapFldScrollable", 0xA14D },
    { "sysTrapFldGetVisibleLines", 0xA14E },
    { "sysTrapFldGetAttributes", 0xA14F },
    { "sysTrapFldSetAttributes", 0xA150 },
    { "sysTrapFldSendChangeNotification", 0xA151 },
    { "sysTrapFldCalcFieldHeight", 0xA152 },
    { "sysTrapFldGetTextHandle", 0xA153 },
    { "sysTrapFldCompactText", 0xA154 },
    { "sysTrapFldDirty", 0xA155 },
    { "sysTrapFldWordWrap", 0xA156 },
    { "sysTrapFldSetTextAllocatedSize", 0xA157 },
    { "sysTrapFldSetTextHandle", 0xA158 },
    { "sysTrapFldSetTextPtr", 0xA159 },
    { "sysTrapFldGetMaxChars", 0xA15A },
    { "sysTrapFldSetMaxChars", 0xA15B },
    { "sysTrapFldSetUsable", 0xA15C },
    { "sysTrapFldInsert", 0xA15D },
    { "sysTrapFldDelete", 0xA15E },
    { "sysTrapFldUndo", 0xA15F },
    { "sysTrapFldSetDirty", 0xA160 },
    { "sysTrapFldSendHeightChangeNotification", 0xA161 },
    { "sysTrapFldMakeFullyVisible", 0xA162 },
    { "sysTrapFntGetFont", 0xA163 },
    { "sysTrapFntSetFont", 0xA164 },
    { "sysTrapFntGetFontPtr", 0xA165 },
    { "sysTrapFntBaseLine", 0xA166 },
    { "sysTrapFntCharHeight", 0xA167 },
    { "sysTrapFntLineHeight", 0xA168 },
    { "sysTrapFntAverageCharWidth", 0xA169 },
    { "sysTrapFntCharWidth", 0xA16A },
    { "sysTrapFntCharsWidth", 0xA16B },
    { "sysTrapFntDescenderHeight", 0xA16C },
    { "sysTrapFntCharsInWidth", 0xA16D },
    { "sysTrapFntLineWidth", 0xA16E },
    { "sysTrapFrmInitForm", 0xA16F },
    { "sysTrapFrmDeleteForm", 0xA170 },
    { "sysTrapFrmDrawForm", 0xA171 },
    { "sysTrapFrmEraseForm", 0xA172 },
    { "sysTrapFrmGetActiveForm", 0xA173 },
    { "sysTrapFrmSetActiveForm", 0xA174 },
    { "sysTrapFrmGetActiveFormID", 0xA175 },
    { "sysTrapFrmGetUserModifiedState", 0xA176 },
    { "sysTrapFrmSetNotUserModified", 0xA177 },
    { "sysTrapFrmGetFocus", 0xA178 },
    { "sysTrapFrmSetFocus", 0xA179 },
    { "sysTrapFrmHandleEvent", 0xA17A },
    { "sysTrapFrmGetFormBounds", 0xA17B },
    { "sysTrapFrmGetWindowHandle", 0xA17C },
    { "sysTrapFrmGetFormId", 0xA17D },
    { "sysTrapFrmGetFormPtr", 0xA17E },
    { "sysTrapFrmGetNumberOfObjects", 0xA17F },
    { "sysTrapFrmGetObjectIndex", 0xA180 },
    { "sysTrapFrmGetObjectId", 0xA181 },
    { "sysTrapFrmGetObjectType", 0xA182 },
    { "sysTrapFrmGetObjectPtr", 0xA183 },
    { "sysTrapFrmHideObject", 0xA184 },
    { "sysTrapFrmShowObject", 0xA185 },
    { "sysTrapFrmGetObjectPosition", 0xA186 },
    { "sysTrapFrmSetObjectPosition", 0xA187 },
    { "sysTrapFrmGetControlValue", 0xA188 },
    { "sysTrapFrmSetControlValue", 0xA189 },
    { "sysTrapFrmGetControlGroupSelection", 0xA18A },
    { "sysTrapFrmSetControlGroupSelection", 0xA18B },
    { "sysTrapFrmCopyLabel", 0xA18C },
    { "sysTrapFrmSetLabel", 0xA18D },
    { "sysTrapFrmGetLabel", 0xA18E },
    { "sysTrapFrmSetCategoryLabel", 0xA18F },
    { "sysTrapFrmGetTitle", 0xA190 },
    { "sysTrapFrmSetTitle", 0xA191 },
    { "sysTrapFrmAlert", 0xA192 },
    { "sysTrapFrmDoDialog", 0xA193 },
    { "sysTrapFrmCustomAlert", 0xA194 },
    { "sysTrapFrmHelp", 0xA195 },
    { "sysTrapFrmUpdateScrollers", 0xA196 },
    { "sysTrapFrmGetFirstForm", 0xA197 },
    { "sysTrapFrmVisible", 0xA198 },
    { "sysTrapFrmGetObjectBounds", 0xA199 },
    { "sysTrapFrmCopyTitle", 0xA19A },
    { "sysTrapFrmGotoForm", 0xA19B },
    { "sysTrapFrmPopupForm", 0xA19C },
    { "sysTrapFrmUpdateForm", 0xA19D },
    { "sysTrapFrmReturnToForm", 0xA19E },
    { "sysTrapFrmSetEventHandler", 0xA19F },
    { "sysTrapFrmDispatchEvent", 0xA1A0 },
    { "sysTrapFrmCloseAllForms", 0xA1A1 },
    { "sysTrapFrmSaveAllForms", 0xA1A2 },
    { "sysTrapFrmGetGadgetData", 0xA1A3 },
    { "sysTrapFrmSetGadgetData", 0xA1A4 },
    { "sysTrapFrmSetCategoryTrigger", 0xA1A5 },
    { "sysTrapUIInitialize", 0xA1A6 },
    { "sysTrapUIReset", 0xA1A7 },
    { "sysTrapInsPtInitialize", 0xA1A8 },
    { "sysTrapInsPtSetLocation", 0xA1A9 },
    { "sysTrapInsPtGetLocation", 0xA1AA },
    { "sysTrapInsPtEnable", 0xA1AB },
    { "sysTrapInsPtEnabled", 0xA1AC },
    { "sysTrapInsPtSetHeight", 0xA1AD },
    { "sysTrapInsPtGetHeight", 0xA1AE },
    { "sysTrapInsPtCheckBlink", 0xA1AF },
    { "sysTrapLstSetDrawFunction", 0xA1B0 },
    { "sysTrapLstDrawList", 0xA1B1 },
    { "sysTrapLstEraseList", 0xA1B2 },
    { "sysTrapLstGetSelection", 0xA1B3 },
    { "sysTrapLstGetSelectionText", 0xA1B4 },
    { "sysTrapLstHandleEvent", 0xA1B5 },
    { "sysTrapLstSetHeight", 0xA1B6 },
    { "sysTrapLstSetSelection", 0xA1B7 },
    { "sysTrapLstSetListChoices", 0xA1B8 },
    { "sysTrapLstMakeItemVisible", 0xA1B9 },
    { "sysTrapLstGetNumberOfItems", 0xA1BA },
    { "sysTrapLstPopupList", 0xA1BB },
    { "sysTrapLstSetPosition", 0xA1BC },
    { "sysTrapMenuInit", 0xA1BD },
    { "sysTrapMenuDispose", 0xA1BE },
    { "sysTrapMenuHandleEvent", 0xA1BF },
    { "sysTrapMenuDrawMenu", 0xA1C0 },
    { "sysTrapMenuEraseStatus", 0xA1C1 },
    { "sysTrapMenuGetActiveMenu", 0xA1C2 },
    { "sysTrapMenuSetActiveMenu", 0xA1C3 },
    { "sysTrapRctSetRectangle", 0xA1C4 },
    { "sysTrapRctCopyRectangle", 0xA1C5 },
    { "sysTrapRctInsetRectangle", 0xA1C6 },
    { "sysTrapRctOffsetRectangle", 0xA1C7 },
    { "sysTrapRctPtInRectangle", 0xA1C8 },
    { "sysTrapRctGetIntersection", 0xA1C9 },
    { "sysTrapTblDrawTable", 0xA1CA },
    { "sysTrapTblEraseTable", 0xA1CB },
    { "sysTrapTblHandleEvent", 0xA1CC },
    { "sysTrapTblGetItemBounds", 0xA1CD },
    { "sysTrapTblSelectItem", 0xA1CE },
    { "sysTrapTblGetItemInt", 0xA1CF },
    { "sysTrapTblSetItemInt", 0xA1D0 },
    { "sysTrapTblSetItemStyle", 0xA1D1 },
    { "sysTrapTblUnhighlightSelection", 0xA1D2 },
    { "sysTrapTblSetRowUsable", 0xA1D3 },
    { "sysTrapTblGetNumberOfRows", 0xA1D4 },
    { "sysTrapTblSetCustomDrawProcedure", 0xA1D5 },
    { "sysTrapTblSetRowSelectable", 0xA1D6 },
    { "sysTrapTblRowSelectable", 0xA1D7 },
    { "sysTrapTblSetLoadDataProcedure", 0xA1D8 },
    { "sysTrapTblSetSaveDataProcedure", 0xA1D9 },
    { "sysTrapTblGetBounds", 0xA1DA },
    { "sysTrapTblSetRowHeight", 0xA1DB },
    { "sysTrapTblGetColumnWidth", 0xA1DC },
    { "sysTrapTblGetRowID", 0xA1DD },
    { "sysTrapTblSetRowID", 0xA1DE },
    { "sysTrapTblMarkRowInvalid", 0xA1DF },
    { "sysTrapTblMarkTableInvalid", 0xA1E0 },
    { "sysTrapTblGetSelection", 0xA1E1 },
    { "sysTrapTblInsertRow", 0xA1E2 },
    { "sysTrapTblRemoveRow", 0xA1E3 },
    { "sysTrapTblRowInvalid", 0xA1E4 },
    { "sysTrapTblRedrawTable", 0xA1E5 },
    { "sysTrapTblRowUsable", 0xA1E6 },
    { "sysTrapTblReleaseFocus", 0xA1E7 },
    { "sysTrapTblEditing", 0xA1E8 },
    { "sysTrapTblGetCurrentField", 0xA1E9 },
    { "sysTrapTblSetColumnUsable", 0xA1EA },
    { "sysTrapTblGetRowHeight", 0xA1EB },
    { "sysTrapTblSetColumnWidth", 0xA1EC },
    { "sysTrapTblGrabFocus", 0xA1ED },
    { "sysTrapTblSetItemPtr", 0xA1EE },
    { "sysTrapTblFindRowID", 0xA1EF },
    { "sysTrapTblGetLastUsableRow", 0xA1F0 },
    { "sysTrapTblGetColumnSpacing", 0xA1F1 },
    { "sysTrapTblFindRowData", 0xA1F2 },
    { "sysTrapTblGetRowData", 0xA1F3 },
    { "sysTrapTblSetRowData", 0xA1F4 },
    { "sysTrapTblSetColumnSpacing", 0xA1F5 },
    { "sysTrapWinCreateWindow", 0xA1F6 },
    { "sysTrapWinCreateOffscreenWindow", 0xA1F7 },
    { "sysTrapWinDeleteWindow", 0xA1F8 },
    { "sysTrapWinInitializeWindow", 0xA1F9 },
    { "sysTrapWinAddWindow", 0xA1FA },
    { "sysTrapWinRemoveWindow", 0xA1FB },
    { "sysTrapWinSetActiveWindow", 0xA1FC },
    { "sysTrapWinSetDrawWindow", 0xA1FD },
    { "sysTrapWinGetDrawWindow", 0xA1FE },
    { "sysTrapWinGetActiveWindow", 0xA1FF },
    { "sysTrapWinGetDisplayWindow", 0xA200 },
    { "sysTrapWinGetFirstWindow", 0xA201 },
    { "sysTrapWinEnableWindow", 0xA202 },
    { "sysTrapWinDisableWindow", 0xA203 },
    { "sysTrapWinGetWindowFrameRect", 0xA204 },
    { "sysTrapWinDrawWindowFrame", 0xA205 },
    { "sysTrapWinEraseWindow", 0xA206 },
    { "sysTrapWinSaveBits", 0xA207 },
    { "sysTrapWinRestoreBits", 0xA208 },
    { "sysTrapWinCopyRectangle", 0xA209 },
    { "sysTrapWinScrollRectangle", 0xA20A },
    { "sysTrapWinGetDisplayExtent", 0xA20B },
    { "sysTrapWinGetWindowExtent", 0xA20C },
    { "sysTrapWinDisplayToWindowPt", 0xA20D },
    { "sysTrapWinWindowToDisplayPt", 0xA20E },
    { "sysTrapWinGetClip", 0xA20F },
    { "sysTrapWinSetClip", 0xA210 },
    { "sysTrapWinResetClip", 0xA211 },
    { "sysTrapWinClipRectangle", 0xA212 },
    { "sysTrapWinDrawLine", 0xA213 },
    { "sysTrapWinDrawGrayLine", 0xA214 },
    { "sysTrapWinEraseLine", 0xA215 },
    { "sysTrapWinInvertLine", 0xA216 },
    { "sysTrapWinFillLine", 0xA217 },
    { "sysTrapWinDrawRectangle", 0xA218 },
    { "sysTrapWinEraseRectangle", 0xA219 },
    { "sysTrapWinInvertRectangle", 0xA21A },
    { "sysTrapWinDrawRectangleFrame", 0xA21B },
    { "sysTrapWinDrawGrayRectangleFrame", 0xA21C },
    { "sysTrapWinEraseRectangleFrame", 0xA21D },
    { "sysTrapWinInvertRectangleFrame", 0xA21E },
    { "sysTrapWinGetFramesRectangle", 0xA21F },
    { "sysTrapWinDrawChars", 0xA220 },
    { "sysTrapWinEraseChars", 0xA221 },
    { "sysTrapWinInvertChars", 0xA222 },
    { "sysTrapWinGetPattern", 0xA223 },
    { "sysTrapWinSetPattern", 0xA224 },
    { "sysTrapWinSetUnderlineMode", 0xA225 },
    { "sysTrapWinDrawBitmap", 0xA226 },
    { "sysTrapWinModal", 0xA227 },
    { "sysTrapWinGetDrawWindowBounds", 0xA228 },
    { "sysTrapWinFillRectangle", 0xA229 },
    { "sysTrapWinDrawInvertedChars", 0xA22A },
    { "sysTrapPrefOpenPreferenceDBV10", 0xA22B },
    { "sysTrapPrefGetPreferences", 0xA22C },
    { "sysTrapPrefSetPreferences", 0xA22D },
    { "sysTrapPrefGetAppPreferencesV10", 0xA22E },
    { "sysTrapPrefSetAppPreferencesV10", 0xA22F },
    { "sysTrapSndInit", 0xA230 },
    { "sysTrapSndSetDefaultVolume", 0xA231 },
    { "sysTrapSndGetDefaultVolume", 0xA232 },
    { "sysTrapSndDoCmd", 0xA233 },
    { "sysTrapSndPlaySystemSound", 0xA234 },
    { "sysTrapAlmInit", 0xA235 },
    { "sysTrapAlmCancelAll", 0xA236 },
    { "sysTrapAlmAlarmCallback", 0xA237 },
    { "sysTrapAlmSetAlarm", 0xA238 },
    { "sysTrapAlmGetAlarm", 0xA239 },
    { "sysTrapAlmDisplayAlarm", 0xA23A },
    { "sysTrapAlmEnableNotification", 0xA23B },
    { "sysTrapHwrGetRAMMapping", 0xA23C },
    { "sysTrapHwrMemWritable", 0xA23D },
    { "sysTrapHwrMemReadable", 0xA23E },
    { "sysTrapHwrDoze", 0xA23F },
    { "sysTrapHwrSleep", 0xA240 },
    { "sysTrapHwrWake", 0xA241 },
    { "sysTrapHwrSetSystemClock", 0xA242 },
    { "sysTrapHwrSetCPUDutyCycle", 0xA243 },
    { "sysTrapHwrDisplayInit", 0xA244 },
    { "sysTrapHwrDisplaySleep", 0xA245 },
    { "sysTrapHwrTimerInit", 0xA246 },
    { "sysTrapHwrCursorV33", 0xA247 },
    { "sysTrapHwrBatteryLevel", 0xA248 },
    { "sysTrapHwrDelay", 0xA249 },
    { "sysTrapHwrEnableDataWrites", 0xA24A },
    { "sysTrapHwrDisableDataWrites", 0xA24B },
    { "sysTrapHwrLCDBaseAddrV33", 0xA24C },
    { "sysTrapHwrDisplayDrawBootScreen", 0xA24D },
    { "sysTrapHwrTimerSleep", 0xA24E },
    { "sysTrapHwrTimerWake", 0xA24F },
    { "sysTrapHwrDisplayWake", 0xA250 },
    { "sysTrapHwrIRQ1Handler", 0xA251 },
    { "sysTrapHwrIRQ2Handler", 0xA252 },
    { "sysTrapHwrIRQ3Handler", 0xA253 },
    { "sysTrapHwrIRQ4Handler", 0xA254 },
    { "sysTrapHwrIRQ5Handler", 0xA255 },
    { "sysTrapHwrIRQ6Handler", 0xA256 },
    { "sysTrapHwrDockSignals", 0xA257 },
    { "sysTrapHwrPluggedIn", 0xA258 },
    { "sysTrapCrc16CalcBlock", 0xA259 },
    { "sysTrapSelectDayV10", 0xA25A },
    { "sysTrapSelectTimeV33", 0xA25B },
    { "sysTrapDayDrawDaySelector", 0xA25C },
    { "sysTrapDayHandleEvent", 0xA25D },
    { "sysTrapDayDrawDays", 0xA25E },
    { "sysTrapDayOfWeek", 0xA25F },
    { "sysTrapDaysInMonth", 0xA260 },
    { "sysTrapDayOfMonth", 0xA261 },
    { "sysTrapDateDaysToDate", 0xA262 },
    { "sysTrapDateToDays", 0xA263 },
    { "sysTrapDateAdjust", 0xA264 },
    { "sysTrapDateSecondsToDate", 0xA265 },
    { "sysTrapDateToAscii", 0xA266 },
    { "sysTrapDateToDOWDMFormat", 0xA267 },
    { "sysTrapTimeToAscii", 0xA268 },
    { "sysTrapFind", 0xA269 },
    { "sysTrapFindStrInStr", 0xA26A },
    { "sysTrapFindSaveMatch", 0xA26B },
    { "sysTrapFindGetLineBounds", 0xA26C },
    { "sysTrapFindDrawHeader", 0xA26D },
    { "sysTrapPenOpen", 0xA26E },
    { "sysTrapPenClose", 0xA26F },
    { "sysTrapPenGetRawPen", 0xA270 },
    { "sysTrapPenCalibrate", 0xA271 },
    { "sysTrapPenRawToScreen", 0xA272 },
    { "sysTrapPenScreenToRaw", 0xA273 },
    { "sysTrapPenResetCalibration", 0xA274 },
    { "sysTrapPenSleep", 0xA275 },
    { "sysTrapPenWake", 0xA276 },
    { "sysTrapResLoadForm", 0xA277 },
    { "sysTrapResLoadMenu", 0xA278 },
    { "sysTrapFtrInit", 0xA279 },
    { "sysTrapFtrUnregister", 0xA27A },
    { "sysTrapFtrGet", 0xA27B },
    { "sysTrapFtrSet", 0xA27C },
    { "sysTrapFtrGetByIndex", 0xA27D },
    { "sysTrapGrfInit", 0xA27E },
    { "sysTrapGrfFree", 0xA27F },
    { "sysTrapGrfGetState", 0xA280 },
    { "sysTrapGrfSetState", 0xA281 },
    { "sysTrapGrfFlushPoints", 0xA282 },
    { "sysTrapGrfAddPoint", 0xA283 },
    { "sysTrapGrfInitState", 0xA284 },
    { "sysTrapGrfCleanState", 0xA285 },
    { "sysTrapGrfMatch", 0xA286 },
    { "sysTrapGrfGetMacro", 0xA287 },
    { "sysTrapGrfFilterPoints", 0xA288 },
    { "sysTrapGrfGetNumPoints", 0xA289 },
    { "sysTrapGrfGetPoint", 0xA28A },
    { "sysTrapGrfFindBranch", 0xA28B },
    { "sysTrapGrfMatchGlyph", 0xA28C },
    { "sysTrapGrfGetGlyphMapping", 0xA28D },
    { "sysTrapGrfGetMacroName", 0xA28E },
    { "sysTrapGrfDeleteMacro", 0xA28F },
    { "sysTrapGrfAddMacro", 0xA290 },
    { "sysTrapGrfGetAndExpandMacro", 0xA291 },
    { "sysTrapGrfProcessStroke", 0xA292 },
    { "sysTrapGrfFieldChange", 0xA293 },
    { "sysTrapGetCharSortValue", 0xA294 },
    { "sysTrapGetCharAttr", 0xA295 },
    { "sysTrapGetCharCaselessValue", 0xA296 },
    { "sysTrapPwdExists", 0xA297 },
    { "sysTrapPwdVerify", 0xA298 },
    { "sysTrapPwdSet", 0xA299 },
    { "sysTrapPwdRemove", 0xA29A },
    { "sysTrapGsiInitialize", 0xA29B },
    { "sysTrapGsiSetLocation", 0xA29C },
    { "sysTrapGsiEnable", 0xA29D },
    { "sysTrapGsiEnabled", 0xA29E },
    { "sysTrapGsiSetShiftState", 0xA29F },
    { "sysTrapKeyInit", 0xA2A0 },
    { "sysTrapKeyHandleInterrupt", 0xA2A1 },
    { "sysTrapKeyCurrentState", 0xA2A2 },
    { "sysTrapKeyResetDoubleTap", 0xA2A3 },
    { "sysTrapKeyRates", 0xA2A4 },
    { "sysTrapKeySleep", 0xA2A5 },
    { "sysTrapKeyWake", 0xA2A6 },
    { "sysTrapDlkControl", 0xA2A7 },
    { "sysTrapDlkStartServer", 0xA2A8 },
    { "sysTrapDlkGetSyncInfo", 0xA2A9 },
    { "sysTrapDlkSetLogEntry", 0xA2AA },
    { "sysTrapIntlDispatch", 0xA2AB },
    { "sysTrapSysLibLoad", 0xA2AC },
    { "sysTrapSndPlaySmf", 0xA2AD },
    { "sysTrapSndCreateMidiList", 0xA2AE },
    { "sysTrapAbtShowAbout", 0xA2AF },
    { "sysTrapMdmDial", 0xA2B0 },
    { "sysTrapMdmHangUp", 0xA2B1 },
    { "sysTrapDmSearchRecord", 0xA2B2 },
    { "sysTrapSysInsertionSort", 0xA2B3 },
    { "sysTrapDmInsertionSort", 0xA2B4 },
    { "sysTrapLstSetTopItem", 0xA2B5 },
    { "sysTrapSclSetScrollBar", 0xA2B6 },
    { "sysTrapSclDrawScrollBar", 0xA2B7 },
    { "sysTrapSclHandleEvent", 0xA2B8 },
    { "sysTrapSysMailboxCreate", 0xA2B9 },
    { "sysTrapSysMailboxDelete", 0xA2BA },
    { "sysTrapSysMailboxFlush", 0xA2BB },
    { "sysTrapSysMailboxSend", 0xA2BC },
    { "sysTrapSysMailboxWait", 0xA2BD },
    { "sysTrapSysTaskWait", 0xA2BE },
    { "sysTrapSysTaskWake", 0xA2BF },
    { "sysTrapSysTaskWaitClr", 0xA2C0 },
    { "sysTrapSysTaskSuspend", 0xA2C1 },
    { "sysTrapSysTaskResume", 0xA2C2 },
    { "sysTrapCategoryCreateList", 0xA2C3 },
    { "sysTrapCategoryFreeList", 0xA2C4 },
    { "sysTrapCategoryEditV20", 0xA2C5 },
    { "sysTrapCategorySelect", 0xA2C6 },
    { "sysTrapDmDeleteCategory", 0xA2C7 },
    { "sysTrapSysEvGroupCreate", 0xA2C8 },
    { "sysTrapSysEvGroupSignal", 0xA2C9 },
    { "sysTrapSysEvGroupRead", 0xA2CA },
    { "sysTrapSysEvGroupWait", 0xA2CB },
    { "sysTrapEvtEventAvail", 0xA2CC },
    { "sysTrapEvtSysEventAvail", 0xA2CD },
    { "sysTrapStrNCopy", 0xA2CE },
    { "sysTrapKeySetMask", 0xA2CF },
    { "sysTrapSelectDay", 0xA2D0 },
    { "sysTrapPrefGetPreference", 0xA2D1 },
    { "sysTrapPrefSetPreference", 0xA2D2 },
    { "sysTrapPrefGetAppPreferences", 0xA2D3 },
    { "sysTrapPrefSetAppPreferences", 0xA2D4 },
    { "sysTrapFrmPointInTitle", 0xA2D5 },
    { "sysTrapStrNCat", 0xA2D6 },
    { "sysTrapMemCmp", 0xA2D7 },
    { "sysTrapTblSetColumnEditIndicator", 0xA2D8 },
    { "sysTrapFntWordWrap", 0xA2D9 },
    { "sysTrapFldGetScrollValues", 0xA2DA },
    { "sysTrapSysCreateDataBaseList", 0xA2DB },
    { "sysTrapSysCreatePanelList", 0xA2DC },
    { "sysTrapDlkDispatchRequest", 0xA2DD },
    { "sysTrapStrPrintF", 0xA2DE },
    { "sysTrapStrVPrintF", 0xA2DF },
    { "sysTrapPrefOpenPreferenceDB", 0xA2E0 },
    { "sysTrapSysGraffitiReferenceDialog", 0xA2E1 },
    { "sysTrapSysKeyboardDialog", 0xA2E2 },
    { "sysTrapFntWordWrapReverseNLines", 0xA2E3 },
    { "sysTrapFntGetScrollValues", 0xA2E4 },
    { "sysTrapTblSetRowStaticHeight", 0xA2E5 },
    { "sysTrapTblHasScrollBar", 0xA2E6 },
    { "sysTrapSclGetScrollBar", 0xA2E7 },
    { "sysTrapFldGetNumberOfBlankLines", 0xA2E8 },
    { "sysTrapSysTicksPerSecond", 0xA2E9 },
    { "sysTrapHwrBacklightV33", 0xA2EA },
    { "sysTrapDmDatabaseProtect", 0xA2EB },
    { "sysTrapTblSetBounds", 0xA2EC },
    { "sysTrapStrNCompare", 0xA2ED },
    { "sysTrapStrNCaselessCompare", 0xA2EE },
    { "sysTrapPhoneNumberLookup", 0xA2EF },
    { "sysTrapFrmSetMenu", 0xA2F0 },
    { "sysTrapEncDigestMD5", 0xA2F1 },
    { "sysTrapDmFindSortPosition", 0xA2F2 },
    { "sysTrapSysBinarySearch", 0xA2F3 },
    { "sysTrapSysErrString", 0xA2F4 },
    { "sysTrapSysStringByIndex", 0xA2F5 },
    { "sysTrapEvtAddUniqueEventToQueue", 0xA2F6 },
    { "sysTrapStrLocalizeNumber", 0xA2F7 },
    { "sysTrapStrDelocalizeNumber", 0xA2F8 },
    { "sysTrapLocGetNumberSeparators", 0xA2F9 },
    { "sysTrapMenuSetActiveMenuRscID", 0xA2FA },
    { "sysTrapLstScrollList", 0xA2FB },
    { "sysTrapCategoryInitialize", 0xA2FC },
    { "sysTrapEncDigestMD4", 0xA2FD },
    { "sysTrapEncDES", 0xA2FE },
    { "sysTrapLstGetVisibleItems", 0xA2FF },
    { "sysTrapWinSetBounds", 0xA300 },
    { "sysTrapCategorySetName", 0xA301 },
    { "sysTrapFldSetInsertionPoint", 0xA302 },
    { "sysTrapFrmSetObjectBounds", 0xA303 },
    { "sysTrapWinSetColors", 0xA304 },
    { "sysTrapFlpDispatch", 0xA305 },
    { "sysTrapFlpEmDispatch", 0xA306 },
    { "sysTrapExgInit", 0xA307 },
    { "sysTrapExgConnect", 0xA308 },
    { "sysTrapExgPut", 0xA309 },
    { "sysTrapExgGet", 0xA30A },
    { "sysTrapExgAccept", 0xA30B },
    { "sysTrapExgDisconnect", 0xA30C },
    { "sysTrapExgSend", 0xA30D },
    { "sysTrapExgReceive", 0xA30E },
    { "sysTrapExgRegisterData", 0xA30F },
    { "sysTrapExgNotifyReceiveV35", 0xA310 },
    { "sysTrapSysReserved30Trap2", 0xA311 },
    { "sysTrapPrgStartDialogV31", 0xA312 },
    { "sysTrapPrgStopDialog", 0xA313 },
    { "sysTrapPrgUpdateDialog", 0xA314 },
    { "sysTrapPrgHandleEvent", 0xA315 },
    { "sysTrapImcReadFieldNoSemicolon", 0xA316 },
    { "sysTrapImcReadFieldQuotablePrintable", 0xA317 },
    { "sysTrapImcReadPropertyParameter", 0xA318 },
    { "sysTrapImcSkipAllPropertyParameters", 0xA319 },
    { "sysTrapImcReadWhiteSpace", 0xA31A },
    { "sysTrapImcWriteQuotedPrintable", 0xA31B },
    { "sysTrapImcWriteNoSemicolon", 0xA31C },
    { "sysTrapImcStringIsAscii", 0xA31D },
    { "sysTrapTblGetItemFont", 0xA31E },
    { "sysTrapTblSetItemFont", 0xA31F },
    { "sysTrapFontSelect", 0xA320 },
    { "sysTrapFntDefineFont", 0xA321 },
    { "sysTrapCategoryEdit", 0xA322 },
    { "sysTrapSysGetOSVersionString", 0xA323 },
    { "sysTrapSysBatteryInfo", 0xA324 },
    { "sysTrapSysUIBusy", 0xA325 },
    { "sysTrapWinValidateHandle", 0xA326 },
    { "sysTrapFrmValidatePtr", 0xA327 },
    { "sysTrapCtlValidatePointer", 0xA328 },
    { "sysTrapWinMoveWindowAddr", 0xA329 },
    { "sysTrapFrmAddSpaceForObject", 0xA32A },
    { "sysTrapFrmNewForm", 0xA32B },
    { "sysTrapCtlNewControl", 0xA32C },
    { "sysTrapFldNewField", 0xA32D },
    { "sysTrapLstNewList", 0xA32E },
    { "sysTrapFrmNewLabel", 0xA32F },
    { "sysTrapFrmNewBitmap", 0xA330 },
    { "sysTrapFrmNewGadget", 0xA331 },
    { "sysTrapFileOpen", 0xA332 },
    { "sysTrapFileClose", 0xA333 },
    { "sysTrapFileDelete", 0xA334 },
    { "sysTrapFileReadLow", 0xA335 },
    { "sysTrapFileWrite", 0xA336 },
    { "sysTrapFileSeek", 0xA337 },
    { "sysTrapFileTell", 0xA338 },
    { "sysTrapFileTruncate", 0xA339 },
    { "sysTrapFileControl", 0xA33A },
    { "sysTrapFrmActiveState", 0xA33B },
    { "sysTrapSysGetAppInfo", 0xA33C },
    { "sysTrapSysGetStackInfo", 0xA33D },
    { "sysTrapWinScreenMode", 0xA33E },
    { "sysTrapHwrLCDGetDepthV33", 0xA33F },
    { "sysTrapHwrGetROMToken", 0xA340 },
    { "sysTrapDbgControl", 0xA341 },
    { "sysTrapExgDBRead", 0xA342 },
    { "sysTrapExgDBWrite", 0xA343 },
    { "sysTrapHostControl", 0xA344 },
    { "sysTrapFrmRemoveObject", 0xA345 },
    { "sysTrapSysReserved30Trap1", 0xA346 },
    { "sysTrapExpansionDispatch", 0xA347 },
    { "sysTrapFileSystemDispatch", 0xA348 },
    { "sysTrapOEMDispatch", 0xA349 },
    { "sysTrapHwrLCDContrastV33", 0xA34A },
    { "sysTrapSysLCDContrast", 0xA34B },
    { "sysTrapUIContrastAdjust", 0xA34C },
    { "sysTrapHwrDockStatus", 0xA34D },
    { "sysTrapFntWidthToOffset", 0xA34E },
    { "sysTrapSelectOneTime", 0xA34F },
    { "sysTrapWinDrawChar", 0xA350 },
    { "sysTrapWinDrawTruncChars", 0xA351 },
    { "sysTrapSysNotifyInit", 0xA352 },
    { "sysTrapSysNotifyRegister", 0xA353 },
    { "sysTrapSysNotifyUnregister", 0xA354 },
    { "sysTrapSysNotifyBroadcast", 0xA355 },
    { "sysTrapSysNotifyBroadcastDeferred", 0xA356 },
    { "sysTrapSysNotifyDatabaseAdded", 0xA357 },
    { "sysTrapSysNotifyDatabaseRemoved", 0xA358 },
    { "sysTrapSysWantEvent", 0xA359 },
    { "sysTrapFtrPtrNew", 0xA35A },
    { "sysTrapFtrPtrFree", 0xA35B },
    { "sysTrapFtrPtrResize", 0xA35C },
    { "sysTrapSysReserved31Trap1", 0xA35D },
    { "sysTrapHwrNVPrefSet", 0xA35E },
    { "sysTrapHwrNVPrefGet", 0xA35F },
    { "sysTrapFlashInit", 0xA360 },
    { "sysTrapFlashCompress", 0xA361 },
    { "sysTrapFlashErase", 0xA362 },
    { "sysTrapFlashProgram", 0xA363 },
    { "sysTrapAlmTimeChange", 0xA364 },
    { "sysTrapErrAlertCustom", 0xA365 },
    { "sysTrapPrgStartDialog", 0xA366 },
    { "sysTrapSerialDispatch", 0xA367 },
    { "sysTrapHwrBattery", 0xA368 },
    { "sysTrapDmGetDatabaseLockState", 0xA369 },
    { "sysTrapCncGetProfileList", 0xA36A },
    { "sysTrapCncGetProfileInfo", 0xA36B },
    { "sysTrapCncAddProfile", 0xA36C },
    { "sysTrapCncDeleteProfile", 0xA36D },
    { "sysTrapSndPlaySmfResource", 0xA36E },
    { "sysTrapMemPtrDataStorage", 0xA36F },
    { "sysTrapClipboardAppendItem", 0xA370 },
    { "sysTrapWiCmdV32", 0xA371 },
    { "sysTrapHwrDisplayAttributes", 0xA372 },
    { "sysTrapHwrDisplayDoze", 0xA373 },
    { "sysTrapHwrDisplayPalette", 0xA374 },
    { "sysTrapBltFindIndexes", 0xA375 },
    { "sysTrapBmpGetBits", 0xA376 },
    { "sysTrapBltCopyRectangle", 0xA377 },
    { "sysTrapBltDrawChars", 0xA378 },
    { "sysTrapBltLineRoutine", 0xA379 },
    { "sysTrapBltRectangleRoutine", 0xA37A },
    { "sysTrapScrCompress", 0xA37B },
    { "sysTrapScrDecompress", 0xA37C },
    { "sysTrapSysLCDBrightness", 0xA37D },
    { "sysTrapWinPaintChar", 0xA37E },
    { "sysTrapWinPaintChars", 0xA37F },
    { "sysTrapWinPaintBitmap", 0xA380 },
    { "sysTrapWinGetPixel", 0xA381 },
    { "sysTrapWinPaintPixel", 0xA382 },
    { "sysTrapWinDrawPixel", 0xA383 },
    { "sysTrapWinErasePixel", 0xA384 },
    { "sysTrapWinInvertPixel", 0xA385 },
    { "sysTrapWinPaintPixels", 0xA386 },
    { "sysTrapWinPaintLines", 0xA387 },
    { "sysTrapWinPaintLine", 0xA388 },
    { "sysTrapWinPaintRectangle", 0xA389 },
    { "sysTrapWinPaintRectangleFrame", 0xA38A },
    { "sysTrapWinPaintPolygon", 0xA38B },
    { "sysTrapWinDrawPolygon", 0xA38C },
    { "sysTrapWinErasePolygon", 0xA38D },
    { "sysTrapWinInvertPolygon", 0xA38E },
    { "sysTrapWinFillPolygon", 0xA38F },
    { "sysTrapWinPaintArc", 0xA390 },
    { "sysTrapWinDrawArc", 0xA391 },
    { "sysTrapWinEraseArc", 0xA392 },
    { "sysTrapWinInvertArc", 0xA393 },
    { "sysTrapWinFillArc", 0xA394 },
    { "sysTrapWinPushDrawState", 0xA395 },
    { "sysTrapWinPopDrawState", 0xA396 },
    { "sysTrapWinSetDrawMode", 0xA397 },
    { "sysTrapWinSetForeColor", 0xA398 },
    { "sysTrapWinSetBackColor", 0xA399 },
    { "sysTrapWinSetTextColor", 0xA39A },
    { "sysTrapWinGetPatternType", 0xA39B },
    { "sysTrapWinSetPatternType", 0xA39C },
    { "sysTrapWinPalette", 0xA39D },
    { "sysTrapWinRGBToIndex", 0xA39E },
    { "sysTrapWinIndexToRGB", 0xA39F },
    { "sysTrapWinScreenLock", 0xA3A0 },
    { "sysTrapWinScreenUnlock", 0xA3A1 },
    { "sysTrapWinGetBitmap", 0xA3A2 },
    { "sysTrapUIColorInit", 0xA3A3 },
    { "sysTrapUIColorGetTableEntryIndex", 0xA3A4 },
    { "sysTrapUIColorGetTableEntryRGB", 0xA3A5 },
    { "sysTrapUIColorSetTableEntry", 0xA3A6 },
    { "sysTrapUIColorPushTable", 0xA3A7 },
    { "sysTrapUIColorPopTable", 0xA3A8 },
    { "sysTrapCtlNewGraphicControl", 0xA3A9 },
    { "sysTrapTblGetItemPtr", 0xA3AA },
    { "sysTrapUIBrightnessAdjust", 0xA3AB },
    { "sysTrapUIPickColor", 0xA3AC },
    { "sysTrapEvtSetAutoOffTimer", 0xA3AD },
    { "sysTrapTsmDispatch", 0xA3AE },
    { "sysTrapOmDispatch", 0xA3AF },
    { "sysTrapDmOpenDBNoOverlay", 0xA3B0 },
    { "sysTrapDmOpenDBWithLocale", 0xA3B1 },
    { "sysTrapResLoadConstant", 0xA3B2 },
    { "sysTrapHwrPreDebugInit", 0xA3B3 },
    { "sysTrapHwrResetNMI", 0xA3B4 },
    { "sysTrapHwrResetPWM", 0xA3B5 },
    { "sysTrapKeyBootKeys", 0xA3B6 },
    { "sysTrapDbgSerDrvOpen", 0xA3B7 },
    { "sysTrapDbgSerDrvClose", 0xA3B8 },
    { "sysTrapDbgSerDrvControl", 0xA3B9 },
    { "sysTrapDbgSerDrvStatus", 0xA3BA },
    { "sysTrapDbgSerDrvWriteChar", 0xA3BB },
    { "sysTrapDbgSerDrvReadChar", 0xA3BC },
    { "sysTrapHwrPostDebugInit", 0xA3BD },
    { "sysTrapHwrIdentifyFeatures", 0xA3BE },
    { "sysTrapHwrModelSpecificInit", 0xA3BF },
    { "sysTrapHwrModelInitStage2", 0xA3C0 },
    { "sysTrapHwrInterruptsInit", 0xA3C1 },
    { "sysTrapHwrSoundOn", 0xA3C2 },
    { "sysTrapHwrSoundOff", 0xA3C3 },
    { "sysTrapSysKernelClockTick", 0xA3C4 },
    { "sysTrapMenuEraseMenu", 0xA3C5 },
    { "sysTrapSelectTime", 0xA3C6 },
    { "sysTrapMenuCmdBarAddButton", 0xA3C7 },
    { "sysTrapMenuCmdBarGetButtonData", 0xA3C8 },
    { "sysTrapMenuCmdBarDisplay", 0xA3C9 },
    { "sysTrapHwrGetSilkscreenID", 0xA3CA },
    { "sysTrapEvtGetSilkscreenAreaList", 0xA3CB },
    { "sysTrapSysFatalAlertInit", 0xA3CC },
    { "sysTrapDateTemplateToAscii", 0xA3CD },
    { "sysTrapSecVerifyPW", 0xA3CE },
    { "sysTrapSecSelectViewStatus", 0xA3CF },
    { "sysTrapTblSetColumnMasked", 0xA3D0 },
    { "sysTrapTblSetRowMasked", 0xA3D1 },
    { "sysTrapTblRowMasked", 0xA3D2 },
    { "sysTrapFrmCustomResponseAlert", 0xA3D3 },
    { "sysTrapFrmNewGsi", 0xA3D4 },
    { "sysTrapMenuShowItem", 0xA3D5 },
    { "sysTrapMenuHideItem", 0xA3D6 },
    { "sysTrapMenuAddItem", 0xA3D7 },
    { "sysTrapFrmSetGadgetHandler", 0xA3D8 },
    { "sysTrapCtlSetGraphics", 0xA3D9 },
    { "sysTrapCtlGetSliderValues", 0xA3DA },
    { "sysTrapCtlSetSliderValues", 0xA3DB },
    { "sysTrapCtlNewSliderControl", 0xA3DC },
    { "sysTrapBmpCreate", 0xA3DD },
    { "sysTrapBmpDelete", 0xA3DE },
    { "sysTrapBmpCompress", 0xA3DF },
    { "sysTrapBmpGetColortable", 0xA3E0 },
    { "sysTrapBmpSize", 0xA3E1 },
    { "sysTrapBmpBitsSize", 0xA3E2 },
    { "sysTrapBmpColortableSize", 0xA3E3 },
    { "sysTrapWinCreateBitmapWindow", 0xA3E4 },
    { "sysTrapEvtSetNullEventTick", 0xA3E5 },
    { "sysTrapExgDoDialog", 0xA3E6 },
    { "sysTrapSysUICleanup", 0xA3E7 },
    { "sysTrapWinSetForeColorRGB", 0xA3E8 },
    { "sysTrapWinSetBackColorRGB", 0xA3E9 },
    { "sysTrapWinSetTextColorRGB", 0xA3EA },
    { "sysTrapWinGetPixelRGB", 0xA3EB },
    { "sysTrapHighDensityDispatch", 0xA3EC },
    { "sysTrapSysReserved40Trap2", 0xA3ED },
    { "sysTrapSysReserved40Trap3", 0xA3EE },
    { "sysTrapSysReserved40Trap4", 0xA3EF },
    { "sysTrapCncMgrDispatch", 0xA3F0 },
    { "sysTrapSysNotifyBroadcastFromInterrupt", 0xA3F1 },
    { "sysTrapEvtWakeupWithoutNilEvent", 0xA3F2 },
    { "sysTrapStrCompareAscii", 0xA3F3 },
    { "sysTrapAccessorDispatch", 0xA3F4 },
    { "sysTrapBltGetPixel", 0xA3F5 },
    { "sysTrapBltPaintPixel", 0xA3F6 },
    { "sysTrapScrScreenInit", 0xA3F7 },
    { "sysTrapScrUpdateScreenBitmap", 0xA3F8 },
    { "sysTrapScrPalette", 0xA3F9 },
    { "sysTrapScrGetColortable", 0xA3FA },
    { "sysTrapScrGetGrayPat", 0xA3FB },
    { "sysTrapScrScreenLock", 0xA3FC },
    { "sysTrapScrScreenUnlock", 0xA3FD },
    { "sysTrapFntPrvGetFontList", 0xA3FE },
    { "sysTrapExgRegisterDatatype", 0xA3FF },
    { "sysTrapExgNotifyReceive", 0xA400 },
    { "sysTrapExgNotifyGoto", 0xA401 },
    { "sysTrapExgRequest", 0xA402 },
    { "sysTrapExgSetDefaultApplication", 0xA403 },
    { "sysTrapExgGetDefaultApplication", 0xA404 },
    { "sysTrapExgGetTargetApplication", 0xA405 },
    { "sysTrapExgGetRegisteredApplications", 0xA406 },
    { "sysTrapExgGetRegisteredTypes", 0xA407 },
    { "sysTrapExgNotifyPreview", 0xA408 },
    { "sysTrapExgControl", 0xA409 },
    { "sysTrapLmDispatch", 0xA40A },
    { "sysTrapMemGetRomNVParams", 0xA40B },
    { "sysTrapFntWCharWidth", 0xA40C },
    { "sysTrapDmFindDatabaseWithTypeCreator", 0xA40D },
    { "sysTrapSelectTimeZone", 0xA40E },
    { "sysTrapTimeZoneToAscii", 0xA40F },
    { "sysTrapStrNCompareAscii", 0xA410 },
    { "sysTrapTimTimeZoneToUTC", 0xA411 },
    { "sysTrapTimUTCToTimeZone", 0xA412 },
    { "sysTrapPhoneNumberLookupCustom", 0xA413 },
    { "sysTrapHwrDebugSelect", 0xA414 },
    { "sysTrapBltRoundedRectangle", 0xA415 },
    { "sysTrapBltRoundedRectangleFill", 0xA416 },
    { "sysTrapWinPrvInitCanvas", 0xA417 },
    { "sysTrapHwrCalcDynamicHeapSize", 0xA418 },
    { "sysTrapHwrDebuggerEnter", 0xA419 },
    { "sysTrapHwrDebuggerExit", 0xA41A },
    { "sysTrapLstGetTopItem", 0xA41B },
    { "sysTrapHwrModelInitStage3", 0xA41C },
    { "sysTrapAttnIndicatorAllow", 0xA41D },
    { "sysTrapAttnIndicatorAllowed", 0xA41E },
    { "sysTrapAttnIndicatorEnable", 0xA41F },
    { "sysTrapAttnIndicatorEnabled", 0xA420 },
    { "sysTrapAttnIndicatorSetBlinkPattern", 0xA421 },
    { "sysTrapAttnIndicatorGetBlinkPattern", 0xA422 },
    { "sysTrapAttnIndicatorTicksTillNextBlink", 0xA423 },
    { "sysTrapAttnIndicatorCheckBlink", 0xA424 },
    { "sysTrapAttnInitialize", 0xA425 },
    { "sysTrapAttnGetAttention", 0xA426 },
    { "sysTrapAttnUpdate", 0xA427 },
    { "sysTrapAttnForgetIt", 0xA428 },
    { "sysTrapAttnGetCounts", 0xA429 },
    { "sysTrapAttnListOpen", 0xA42A },
    { "sysTrapAttnHandleEvent", 0xA42B },
    { "sysTrapAttnEffectOfEvent", 0xA42C },
    { "sysTrapAttnIterate", 0xA42D },
    { "sysTrapAttnDoSpecialEffects", 0xA42E },
    { "sysTrapAttnDoEmergencySpecialEffects", 0xA42F },
    { "sysTrapAttnAllowClose", 0xA430 },
    { "sysTrapAttnReopen", 0xA431 },
    { "sysTrapAttnEnableNotification", 0xA432 },
    { "sysTrapHwrLEDAttributes", 0xA433 },
    { "sysTrapHwrVibrateAttributes", 0xA434 },
    { "sysTrapSecGetPwdHint", 0xA435 },
    { "sysTrapSecSetPwdHint", 0xA436 },
    { "sysTrapHwrFlashWrite", 0xA437 },
    { "sysTrapKeyboardStatusNew", 0xA438 },
    { "sysTrapKeyboardStatusFree", 0xA439 },
    { "sysTrapKbdSetLayout", 0xA43A },
    { "sysTrapKbdGetLayout", 0xA43B },
    { "sysTrapKbdSetPosition", 0xA43C },
    { "sysTrapKbdGetPosition", 0xA43D },
    { "sysTrapKbdSetShiftState", 0xA43E },
    { "sysTrapKbdGetShiftState", 0xA43F },
    { "sysTrapKbdDraw", 0xA440 },
    { "sysTrapKbdErase", 0xA441 },
    { "sysTrapKbdHandleEvent", 0xA442 },
    { "sysTrapOEMDispatch2", 0xA443 },
    { "sysTrapHwrCustom", 0xA444 },
    { "sysTrapFrmGetActiveField", 0xA445 },
    { "sysTrapSndPlaySmfIrregardless", 0xA446 },
    { "sysTrapSndPlaySmfResourceIrregardless", 0xA447 },
    { "sysTrapSndInterruptSmfIrregardless", 0xA448 },
    { "sysTrapUdaMgrDispatch", 0xA449 },
    { "sysTrapPalmPrivate1", 0xA44A },
    { "sysTrapPalmPrivate2", 0xA44B },
    { "sysTrapPalmPrivate3", 0xA44C },
    { "sysTrapPalmPrivate4", 0xA44D },
    { "sysTrapBmpGetDimensions", 0xA44E },
    { "sysTrapBmpGetBitDepth", 0xA44F },
    { "sysTrapBmpGetNextBitmap", 0xA450 },
    { "sysTrapTblGetNumberOfColumns", 0xA451 },
    { "sysTrapTblGetTopRow", 0xA452 },
    { "sysTrapTblSetSelection", 0xA453 },
    { "sysTrapFrmGetObjectIndexFromPtr", 0xA454 },
    { "sysTrapBmpGetSizes", 0xA455 },
    { "sysTrapWinGetBounds", 0xA456 },
    { "sysTrapBltPaintPixels", 0xA457 },
    { "sysTrapFldSetMaxVisibleLines", 0xA458 },
    { "sysTrapScrDefaultPaletteState", 0xA459 },
    { "sysTrapPceNativeCall", 0xA45A },
    { "sysTrapSndStreamCreate", 0xA45B },
    { "sysTrapSndStreamDelete", 0xA45C },
    { "sysTrapSndStreamStart", 0xA45D },
    { "sysTrapSndStreamPause", 0xA45E },
    { "sysTrapSndStreamStop", 0xA45F },
    { "sysTrapSndStreamSetVolume", 0xA460 },
    { "sysTrapSndStreamGetVolume", 0xA461 },
    { "sysTrapSndPlayResource", 0xA462 },
    { "sysTrapSndStreamSetPan", 0xA463 },
    { "sysTrapSndStreamGetPan", 0xA464 },
    { "sysTrapMultimediaDispatch", 0xA465 },
    { "sysTrapSndStreamCreateExtended", 0xa466 },
    { "sysTrapSndStreamDeviceControl", 0xa467 },
    { "sysTrapBmpCreateVersion3", 0xA468 },
    { "sysTrapECFixedMul", 0xA469 },
    { "sysTrapECFixedDiv", 0xA46A },
    { "sysTrapHALDrawGetSupportedDensity", 0xA46B },
    { "sysTrapHALRedrawInputArea", 0xA46C },
    { "sysTrapGrfBeginStroke", 0xA46D },
    { "sysTrapBmpPrvConvertBitmap", 0xA46E },
    { "sysTrapSysReservedTrap5", 0xA46F },
    { "sysTrapPinsDispatch", 0xA470 },
    { "sysTrapSysReservedTrap1", 0xA471 },
    { "sysTrapSysReservedTrap2", 0xA472 },
    { "sysTrapSysReservedTrap3", 0xA473 },
    { "sysTrapSysReservedTrap4", 0xA474 },
    { "sysTrapLastTrapNumber", 0xA475 }
};

const char *lookup_trap(UWORD opcode){
	for(int i = 0; i < (sizeof(traps)/sizeof(traps[0])); i++){
		if (traps[i].trap == opcode)return traps[i].name;
	}
	return nullptr;
}

void ftrget(){
	stacklong(creator);
	stackword(ftrnum);
	stackptr(retval);//edit this address with feature value,ULONG

	bool ftrexists = true;

	//remove switch eventualy!!
	switch(creator){
		case sysFileCSystem:
			switch(ftrnum){
				case sysFtrNumROMVersion://1
					//say where on palm os 5
					//put_long(retval,0x05030300);
					put_long(retval,0x05493028);//V5.49028 (Palm TX version of palm os)
					break;
				case sysFtrNumProcessorID://2
					//hack //find T5 processor type

					put_long(retval,sysFtrNumProcessorXscale);
					break;
				case sysFtrNumOEMDeviceID://21
					/*
					//TnT5 //Tungsten T5
					put_byte(retval,(UBYTE)'T');
					put_byte(retval + 1,(UBYTE)'n');
					put_byte(retval + 2,(UBYTE)'T');
					put_byte(retval + 3,(UBYTE)'5');
					*/

					//D050 //Palm T|X
					put_byte(retval,(UBYTE)'D');
					put_byte(retval + 1,(UBYTE)'0');
					put_byte(retval + 2,(UBYTE)'5');
					put_byte(retval + 3,(UBYTE)'0');
					break;
				case sysFtrNumOEMHALID://22
					/*
					//UNK* //Tungsten T5 //find this
					put_byte(retval,(UBYTE)'T');
					put_byte(retval + 1,(UBYTE)'n');
					put_byte(retval + 2,(UBYTE)'T');
					put_byte(retval + 3,(UBYTE)'5');
					*/

					//P050 //Palm T|X
					put_byte(retval,(UBYTE)'P');
					put_byte(retval + 1,(UBYTE)'0');
					put_byte(retval + 2,(UBYTE)'5');
					put_byte(retval + 3,(UBYTE)'0');
					break;
				case sysFtrNumWinVersion:
					put_long(retval,5);//unsure of value
					break;
				case sysFtrNumHwrMiscFlags://8
					put_word(retval,hwrMiscFlagHasSWContrast |
							 hwrMiscFlagHasCradleDetect |
							 hwrMiscFlagNoRTCBug |
							 hwrMiscFlagHasMbdIrDA |
							 hwrMiscFlagHasBacklight |
							 0xF000/*all hard keys*/);
					break;
				case sysFtrNumOEMCompanyID://20
					/*
					//Tungsten T5 'palm'
					put_byte(retval,(UBYTE)'p');
					put_byte(retval + 1,(UBYTE)'a');
					put_byte(retval + 2,(UBYTE)'l');
					put_byte(retval + 3,(UBYTE)'m');
					*/

					//Palm T|x 'Palm'
					put_byte(retval,(UBYTE)'P');
					put_byte(retval + 1,(UBYTE)'a');
					put_byte(retval + 2,(UBYTE)'l');
					put_byte(retval + 3,(UBYTE)'m');
					break;


				//24,8,27
				//case sysFtrNumVendor://15
				//	break;
				default:
					ftrexists = false;
					break;
			}
			break;
		case navFtrCreator://five way navigator (DPAD)
			if(ftrnum == navFtrVersion)put_long(retval,navVersion);//has DPAD
			else palmabrt();//hack
			break;
		case pinCreator://pen input manager
			if(ftrnum == pinFtrAPIVersion)put_long(retval,pinAPIVersion1_1);
			else palmabrt();//hack
			break;
			/*
		case twFtrCreator://tapwave zodiac identifier
			switch(ftrnum){
				case twFtrAPIVersion:
				case twFtrAPIGlue:
					put_long(retval,TAPWAVE_API_VERSION);
					break;
			}
			break;
			*/
		default:
			ftrexists = false;
			break;
	}

	size_t count,tblsize = featuretable.size();
	inc_for(count,tblsize){
		if(featuretable[count].creator.typen == creator && featuretable[count].id == ftrnum){
			put_long(retval,featuretable[count].value);
			ftrexists = true;
			break;
		}
	}

	ULONG dys = belong(creator);
	if(!ftrexists)
		dbgprintf("Creator:%.4s,Ftrnum:%d,Exists:%s\n",(char*)&dys,ftrnum,(ftrexists ? "True" : "False"));

	if(ftrexists)D0 = errNone;
	else D0 = ftrErrNoSuchFeature;
}

void ftrset(){
	stacklong(creator);
	stackword(ftrnum);
	stacklong(value);

	dbgprintf("FTRSET: Creator:%.4s,Ftrnum:%d,Value:%d\n",(char*)&creator,ftrnum,value);

	size_t count,tblsize = featuretable.size();
	inc_for(count,tblsize){
		if(featuretable[count].creator.typen == creator && featuretable[count].id == ftrnum){
			featuretable[count].value = value;
			D0 = errNone;
			return;
		}
	}

	feature newftr;
	newftr.creator.typen = creator;
	newftr.id = ftrnum;
	newftr.value = value;
	featuretable.push_back(newftr);

	D0 = errNone;
}

void keysetmask(){
	stacklong(newmask);
	D0 = keymask;
	keymask = newmask;
}

TEMPHACK
void keycurrentstate(){
	D0 = BTNTBL;
	TEMPHACK;
	//add key mask if needed
}

void systaskdelay(){
	stacklong(uticks);
	LONG ticks = (LONG)uticks;

	if(ticks < 0)palmabrt();//hack //cant wait less than 0 ticks

	this_thread::sleep_for(palmTicks(ticks));
	D0 = errNone;
}

void syslibfind(){
	stackptr(nameptr);
	stackptr(libnum);//write this value

	dbgprintf("Find lib:%s,Addr:%08x\n",m68kstr(nameptr).c_str(),nameptr);

	//void *meepcheek = get_real_address(nameptr);

	TEMPHACK;//find lib

	//D0 = errNone;
	D0 = sysErrLibNotFound;
}

TEMPHACK
void syslibload(){
	stacklong(type);
	stacklong(creator);
	stackptr(result);//write lib start here

	dbgprintf("Type:%.4s,Creator:%.4s\n",(char*)&type,(char*)&creator);

	int lib = numfromtypecreator(0,type,creator);

	if(lib != -1){
		TEMPHACK;//loadlib
		D0 = errNone;
	}
	else{
		put_word(result,sysInvalidRefNum);
		D0 = sysErrLibNotFound;//may be fatal error
	}
}

void sysappstartup(){
	stackptr(appinfoptr);
	stackptr(prevglobalsptr);
	stackptr(appglobalsptr);
	//^^ are pointers to pointers
	put_long(appinfoptr,appcall);
	//put_long(prevglobalsptr,0xFFFFFFFD);//cant be at odd address
	put_long(prevglobalsptr,0x878745FD);//hack/test for platypus
	put_long(appglobalsptr,apps[curapp].thisglobals);

	//there might be more

	D0 = errNone;
}

void sysappexit(){
	stackptr(sysappinfoptr);
	stackptr(prevgblptr);
	stackptr(gblptr);


	dbgprintf("Appinfptr:%08x,Prevgblptr:%08x,Gblptr:%08x\n",sysappinfoptr,prevgblptr,gblptr);
	//palmabrt();//hack

	D0 = errNone;
}

void syscurappdatabase(){
	stackptr(cardnoptr);//UWORD
	stackptr(localidptr);//ULONG

	put_word(cardnoptr,0);
	put_long(localidptr,curapp + dmOpenRefOffset);

	D0 = errNone;
}

//1 tick = 100 microseconds
void systickspersecond(){
	D0 = TICKSPERSECOND;
}

//found real palm os sysrandom from palm password cracker
ULONG seed;
void sysrandom(){
	stacklong(newseed);

	//option to patch sysrandom() to use real random numbers
	//instead of the palm os sequential modification algorithm.
	if(truerandom){
		D0 = (randomnumber() & 0x7fff);
		return;
	}

	if(newseed != 0)seed = newseed;
	seed = (seed * 22695477) + 1;
	D0 = ((seed >> 16) & 0x7fff);
}

void syscopystringresource(){
	stackptr(chrptr);
	stackword(id);
	CPTR resptr = getappresource(id,'tSTR');
	size_t_68k strsize = strlen68k(resptr) + 1;//+1 to count the null terminator
	memcpy68k(chrptr,resptr,strsize);
	//no return value
}

TEMPHACK
//params are ignored
void sysgetappinfo(){
	stackptr(uiappptr);//pointer to SysAppInfoPtr (pointer to pointer)
	stackptr(actioncodeappptr);//pointer to SysAppInfoPtr (pointer to pointer)

	TEMPHACK;
	//dont know what to do
	//(just returning the sysappinfo of the running app)
	A0 = appcall;
}

void sysnotifyregister(){
	stackword(cardno);//dummy
	stacklong(localid);//dmopenref format
	stacklong(notifytype);
	stackptr(callback);
	stackbyte(upriority);
	stackptr(userdataptr);
	BYTE priority = (BYTE)upriority;
	localid -= dmOpenRefOffset;

	if(cardno != 0)palmabrt();//hack

	dbgprintf("NotificationType:%.4s,ASnum:%08x,DBnum:%d,Callback:%08x\n",(char*)&notifytype,notifytype,localid,callback);

	switch(notifytype){
		//safe to ignore
		case sysNotifySyncFinishEvent:
		case sysNotifyAppLaunchingEvent:
			break;

		//unknown
		default:
			palmabrt();//hack
			break;
	}

	D0 = errNone;
}


void timgetticks(){
	chrono::high_resolution_clock::time_point current = chrono::high_resolution_clock::now();
	ULONG abovezero = (current - starttime) / palmTicks(1);
	chrono::high_resolution_clock::duration belowzero = (current - starttime) % palmTicks(1);
	totalticks += abovezero;
	starttime = current - belowzero;//preserve partial ticks
	D0 = totalticks;
}

void timgetseconds(){
	time_t now = time(nullptr);
	D0 = now + 2082844800;//unix to palm os timestamp offset (seconds from 1904-1970)
}

void dlkgetsyncinfo(){
	CPTR ssdateptr = poplonglink();
	CPTR lastsdateptr = poplonglink();
	CPTR syncstateptr = poplonglink();
	CPTR usernameptr = poplonglink();
	CPTR logptr = poplonglink();
	CPTR loglengthptr = poplonglink();

	if(ssdateptr){
		dbgprintf("Why syncdate?\n");
	}
	if(lastsdateptr){
		dbgprintf("Why llamaptr?\n");
	}
	putbyteifvptr(syncstateptr,dlkSyncStateNeverSynced);
	if(usernameptr){
		writestring(usernameptr,username);
	}
	if(logptr){
		dbgprintf("Why logptr?\n");
	}
	putlongifvptr(loglengthptr,0);
}

void locgetnumberseparators(){
	stackbyte(localization);//unused
	stackptr(thousandsep);
	stackptr(decimalsep);

	put_byte(thousandsep,',');
	put_byte(decimalsep,'.');

	//no return value
}

void errexceptionlist(){
	A0 = appexceptionlist;
}

bool hastextclipbank = true;//use dynalloc chunk for clipboard
CPTR textclipbank = 0;
void clipboardgetitem(){
	stackbyte(cliptype);
	stackptr(lengthptr);//UWORD length in bytes of clipboard item

	switch(cliptype){
		case clipboardText:{
				if(!hastextclipbank)textclipbank = getnewlinearchunks(1) << 16;//64k clipboard
				string curclip = getclipboard();
				put_word(lengthptr,curclip.size());
				writestring(textclipbank,curclip);
				A0 = textclipbank;
				return;
			}
			break;
		case clipboardBitmap:
			palmabrt();//hack
			break;
		default:
			palmabrt();//hack
			break;
	}

}


/*
typedef struct _tagEmulState {
	UInt32	unknown;
	UInt32	d[8];		// 0x04
	UInt32	a[7];		// 0x24
	UInt32	usp;		// 0x40
	UInt32	pc;			// 0x44
	UInt16	sr;
	UInt16	pad;
	UInt32	more[33];
} EmulStateType, *EmulStateRef;
*/

//switch to ARM instruction set
bool hasemulstate = true;
CPTR emulstate;
void pcenativecall(){
	stackptr(callptr);
	stackptr(datablock);

	dbgprintf("Trying to run arm stuff.\n");

	//palmabrt();//hack

	//stackptr(data1);
	//stackptr(data2);
	//dbgprintf("RandomData1:%08x,RandomData2:%08x\n",data1,data2);

	/*
	//must be an api
	if(callptr > 0x1000){

	}
	*/

	dbgprintf("Call:%08x,DataBlock:%08x\n",callptr,datablock);

	//printregs();
	//palmabrt();//hack

	//r0-r3 are used as function arguments in arm (with aapcs)

	/*
	pushstackARM(emulstate);
	pushstackARM(datablock);
	pushstackARM(CALL68KFUNC);
	*/

	setregARM(0,emulstate);
	setregARM(1,datablock);
	setregARM(2,CALL68KFUNC);


	callARM(callptr);

	//arm passes and returns values in regesters not on the stack
	//CPTR armreturn = popstackARM();
	CPTR armreturn = getregARM(0);//arm r0

	//Palm OS API Reference
	/*This value is placed in both the A0 and D0
	 * registers in the emulated 68k CPU,
	 * allowing PceNativeCall to support both pointer
	 * and immediate return value conventions.*/
	D0 = armreturn;
	A0 = armreturn;
}


#include "bsod.h"

//apis (and call to ARM)
bool emulateapi(int api){

	if(currentactivewindow == nullptr_68k)palmabrt();//hack


	//to catch errors without printing every api call
	char fakestring[100];//100 is way overestimateing
	sprintf(fakestring,"Trap At:%#010x,Api:%s,0x%04x\n",MC68000_getpc(),lookup_trap(api),api);
	lasttrap = fakestring;
	//*/

	if(api != sysTrapMemMove && api != sysTrapMemSet && api != sysTrapDmWrite && api != sysTrapFlpEmDispatch && api != sysTrapTimGetTicks)dbgprintf("Trap At:%#010x,Api:%s,0x%04x\n",MC68000_getpc(),lookup_trap(api),api);
	//dbgprintf("Trap At:%#010x,Api:%s,0x%04x\n",MC68000_getpc(),lookup_trap(api),api);

	LINK(SP);//No longer in every api call
	switch(api){
		/*
		//things apps should not even be calling
		case sysTrapSysSetTrapAddress:
		case sysTrapSysGetTrapAddress:
		case sysTrapDbgSerDrvReadChar:
			break;
		*/
		case sysTrapErrDisplayFileLineMsg:{
				stackptr(fname);
				stackword(line);
				stackptr(errstr);
				dbgprintf("Palm says:%s",m68kstr(fname).c_str());
				dbgprintf(" %d ",line);
				dbgprintf("%s\n",m68kstr(errstr).c_str());
			}
			return true;

		case sysTrapStrPrintF:{
				stackptr(outstring);
				stackptr(formatstring);
				stackptr(args);
				dbgprintf("OutPtr:%08x,InPtr:%08x,ArgPtr:%08x\n",outstring,formatstring,args);
				string str = m68kstr(formatstring);
				dbgprintf("Start:%s,ArgPtr:%08x\n",str.c_str(),args);
				writestring(outstring,str);
				//D0 = sprintf68k(outstring,formatstring,args);
				palmabrt();
			}
			return true;

		case sysTrapErrSetJump:

			TEMPHACK;
			//implement

			//stackregs(regs);

			//change in jump value
			//backup regs |D3-D7,PC,A2-A7|

			//D0 = returnvaluefromlongjump;
			return true;
		case sysTrapErrLongJump:

			palmabrt();//hack

			//stackregs(regs);

			//change in jump value
			//put regs back

			//no return value
			return true;

		//graphics apis
		API(sysTrapWinCreateOffscreenWindow,wincreateoffscreenwindow());
		API(sysTrapWinCreateBitmapWindow,wincreatebitmapwindow());
		API(sysTrapWinDeleteWindow,windeletewindow());
		API(sysTrapWinSetDrawWindow,winsetdrawwindow());
		API(sysTrapWinEraseRectangle,wineraserectangle());
		API(sysTrapWinDrawBitmap,windrawbitmap());
		API(sysTrapWinScreenMode,winscreenmode());
		API(sysTrapWinGetDrawWindow,wingetdrawwindow());
		API(sysTrapWinGetBitmap,wingetbitmap());
		API(sysTrapWinSetForeColor,winsetforecolor());
		API(sysTrapWinSetBackColor,winsetbackcolor());
		API(sysTrapWinSetTextColor,winsettextcolor());
		API(sysTrapWinDrawPixel,windrawpixel());
		API(sysTrapWinGetDisplayWindow,wingetdisplaywindow());
		API(sysTrapWinSetActiveWindow,winsetactivewindow());
		API(sysTrapWinSetClip,winsetclip());
		API(sysTrapWinDrawChars,windrawchars());
		API(sysTrapWinResetClip,winresetclip());
		API(sysTrapWinCopyRectangle,wincopyrectangle());
		API(sysTrapWinPalette,winpalette());
		API(sysTrapWinRGBToIndex,winrgbtoindex());
		API(sysTrapWinPushDrawState,winpushdrawstate());
		API(sysTrapWinPopDrawState,winpopdrawstate());
		API(sysTrapWinDrawRectangle,windrawrectangle());
		API(sysTrapWinDrawRectangleFrame,windrawrectangleframe());
		API(sysTrapWinScreenLock,winscreenlock());
		API(sysTrapWinScreenUnlock,winscreenunlock());
		API(sysTrapWinGetDisplayExtent,wingetdisplayextent());
		API(sysTrapWinEraseWindow,winerasewindow());
		API(sysTrapWinDrawLine,windrawline());
		API(sysTrapWinErasePixel,winerasepixel());
		API(sysTrapWinPaintBitmap,winpaintbitmap());

		//uicolor
		API(sysTrapUIColorGetTableEntryIndex,uicolorgettableentryindex());
		API(sysTrapUIColorGetTableEntryRGB,uicolorgettableentryrgb());
		API(sysTrapUIColorSetTableEntry,uicolortablesettableentry());

		//field
		API(sysTrapFldSetUsable,fldsetuseable());
		API(sysTrapFldEraseField,flderasefield());
		API(sysTrapFldSetTextPtr,fldsettextptr());
		API(sysTrapFldSetTextHandle,fldsettexthandle());
		API(sysTrapFldDrawField,flddrawfield());
		API(sysTrapFldGetTextPtr,fldgettextptr());
		API(sysTrapFldGetTextHandle,fldgettexthandle());

		//controls
		API(sysTrapCtlSetUsable,ctlsetusable());

		//bitmaps
		API(sysTrapBmpGetBits,bmpgetbits());
		API(sysTrapBmpBitsSize,bmpbitssize());
		API(sysTrapBmpCreate,bmpcreate());
		API(sysTrapBmpGetDimensions,bmpgetdimensions());

		//font
		API(sysTrapFntSetFont,fntsetfont());
		API(sysTrapFntCharsWidth,fntcharswidth());
		API(sysTrapFntGetFontPtr,fntgetfontptr());

		//forms
		API(sysTrapFrmAlert,frmalert());
		API(sysTrapFrmCustomAlert,frmcustomalert());
		API(sysTrapFrmGotoForm,frmgotoform());
		API(sysTrapFrmGetFormPtr,frmgetformptr());
		API(sysTrapFrmGetWindowHandle,frmgetwindowhandle());
		API(sysTrapFrmDispatchEvent,frmdispatchevent());
		API(sysTrapFrmInitForm,frminitform());
		API(sysTrapFrmDeleteForm,frmdeleteform());
		API(sysTrapFrmCloseAllForms,frmcloseallforms());
		API(sysTrapFrmSetActiveForm,frmsetactiveform());
		API(sysTrapFrmSetEventHandler,frmseteventhandler());
		API(sysTrapFrmGetActiveForm,frmgetactiveform());
		API(sysTrapFrmGetActiveFormID,frmgetactiveformid());
		API(sysTrapFrmSetTitle,frmsettitle());
		//put on display!!
		API(sysTrapFrmDrawForm,frmdrawform());
		API(sysTrapFrmGetObjectIndex,frmgetobjectindex());
		API(sysTrapFrmSetGadgetHandler,frmsetgadgethandler());
		API(sysTrapFrmGetObjectPtr,frmgetobjectptr());
		API(sysTrapFrmHideObject,frmhideobject());
		API(sysTrapFrmSetFocus,frmsetfocus());
		API(sysTrapFrmSetControlValue,frmsetcontrolvalue());

		//menu
		API(sysTrapMenuHandleEvent,menuhandleevent());

		//misc gui
		API(sysTrapRctSetRectangle,rctsetrectangle());
		API(sysTrapRctPtInRectangle,rctptinrectangle());

		//buttons
		API(sysTrapKeyCurrentState,keycurrentstate());
		API(sysTrapKeySetMask,keysetmask());
		API(sysTrapEvtGetPenBtnList,evtgetpenbuttonlist());

		//time
		API(sysTrapTimGetTicks,timgetticks());
		API(sysTrapTimGetSeconds,timgetseconds());
		API(sysTrapSysTicksPerSecond,systickspersecond());
		API(sysTrapSysTaskDelay,systaskdelay());

		//system
		API(sysTrapSysAppStartup,sysappstartup());
		API(sysTrapSysLibFind,syslibfind());
		API(sysTrapSysLibLoad,syslibload());
		API(sysTrapSysRandom,sysrandom());
		API(sysTrapSysHandleEvent,syshandleevent());
		API(sysTrapSysCopyStringResource,syscopystringresource());
		API(sysTrapSysCurAppDatabase,syscurappdatabase());
		API(sysTrapSysAppExit,sysappexit());//some games use this to kill the window manager
		API(sysTrapSysGetAppInfo,sysgetappinfo());
		API(sysTrapSysNotifyRegister,sysnotifyregister());

		//feature manager
		API(sysTrapFtrGet,ftrget());
		API(sysTrapFtrSet,ftrset());

		//event queue
		API(sysTrapEvtGetEvent,evtgetevent());
		API(sysTrapEvtResetAutoOffTimer,evtresetautoofftimer());
		API(sysTrapEvtEnqueueKey,evtenqueuekey());
		API(sysTrapEvtAddEventToQueue,evtaddeventtoqueue());
		API(sysTrapEvtAddUniqueEventToQueue,evtadduniqueeventtoqueue());

		//memory
		API(sysTrapMemSemaphoreReserve,memsemaphorereserve());
		API(sysTrapMemSemaphoreRelease,memsemaphorerelease());
		API(sysTrapMemMove,memmoveAPI());
		API(sysTrapMemSet,memsetAPI());
		//memory subset (pointers)
		API(sysTrapMemPtrNew,memptrnew());
		API(sysTrapMemPtrUnlock,memptrunlock());
		API(sysTrapMemPtrRecoverHandle,memptrrecoverhandle());
		API(sysTrapMemPtrHeapID,memptrheapid());
		//memory subset (handles)
		API(sysTrapMemHandleNew,memhandlenew());
		API(sysTrapMemHandleFree,memhandlefree());
		API(sysTrapMemHandleLock,memhandlelock());
		API(sysTrapMemHandleUnlock,memhandleunlock());
		API(sysTrapMemHandleSize,memhandlesize());
		//memory subset (chunks)
		API(sysTrapMemChunkNew,memchunknew());
		API(sysTrapMemChunkFree,memchunkfree());
		API(sysTrapMemHeapFreeBytes,memheapfreebytes());
		API(sysTrapMemCardInfo,memcardinfo());
		API(sysTrapMemNumCards,D0 = 1);//hack
		API(sysTrapMemNumHeaps,D0 = 3);//hack
		API(sysTrapMemNumRAMHeaps,D0 = 2);//hack //dynamic and storage
		API(sysTrapMemHeapID,memheapid());
		API(sysTrapMemHeapDynamic,memheapdynamic());
		API(sysTrapMemHeapCompact,memheapcompact());

		//preferences
		API(sysTrapPrefGetAppPreferencesV10,prefgetapppreferencesv10());
		API(sysTrapPrefSetAppPreferences,prefsetapppreferences());
		API(sysTrapPrefGetAppPreferences,prefgetapppreferences());
		API(sysTrapPrefGetPreferences,prefgetpreferences());
		API(sysTrapPrefGetPreference,prefgetpreference());

		//string
		API(sysTrapStrCopy,strcopyAPI());
		API(sysTrapStrLen,strlenAPI());
		API(sysTrapStrNCompare,strncompare());
		API(sysTrapStrNCopy,strncopy());
		API(sysTrapStrCompare,strcompare());
		API(sysTrapStrChr,strchr());
		API(sysTrapStrIToA,stritoa());

		//data manager
		//get
		API(sysTrapDmFindDatabase,dmfinddatabase());
		API(sysTrapDmFindRecordByID,dmfindrecordbyid());
		API(sysTrapDmGetNextDatabaseByTypeCreator,dmgetnextdatabasebytypecreator());
		API(sysTrapDmGetRecord,dmgetrecord());
		API(sysTrapDmGetResource,dmgetresource());
		API(sysTrapDmGet1Resource,dmget1resource());
		API(sysTrapDmGetResourceIndex,dmgetresourceindex());
		API(sysTrapDmFindResource,dmfindresource());
		API(sysTrapDmNextOpenResDatabase,dmnextopenresdatabase());
		API(sysTrapDmSearchResource,dmsearchresource());
		//create
		API(sysTrapDmCreateDatabase,dmcreatedatabase());
		API(sysTrapDmNewHandle,dmnewhandle());
		API(sysTrapDmNewRecord,dmnewrecord());
		API(sysTrapDmNewResource,dmnewresource());
		//modify
		API(sysTrapDmAttachRecord,dmattachrecord());
		API(sysTrapDmReleaseRecord,dmreleaserecord());
		API(sysTrapDmRemoveRecord,dmremoverecord());
		API(sysTrapDmSetDatabaseInfo,dmsetdatabaseinfo());
		API(sysTrapDmWrite,dmwrite());
		API(sysTrapDmWriteCheck,dmwritecheck());
		API(sysTrapDmResetRecordStates,dmresetrecordstates());
		//get current state
		API(sysTrapDmDatabaseInfo,dmdatabaseinfo());
		API(sysTrapDmOpenDatabaseInfo,dmopendatabaseinfo());
		API(sysTrapDmNumRecords,dmnumrecords());
		//open and close
		API(sysTrapDmOpenDatabase,dmopendatabase());
		API(sysTrapDmOpenDatabaseByTypeCreator,dmopendatabasebytypecreator());
		API(sysTrapDmCloseDatabase,dmclosedatabase());
		//misc
		API(sysTrapDmQueryRecord,dmqueryrecord());
		API(sysTrapDmReleaseResource,dmreleaseresource());

		//sound
		API(sysTrapSndGetDefaultVolume,sndgetdefaultvolume());
		API(sysTrapSndDoCmd,snddocmd());
		API(sysTrapSndStreamSetVolume,sndstreamsetvolume());

		//sysTrap****Dispatch (switch statement for real apis)
		//high density displays
		API(sysTrapHighDensityDispatch,highdensitydispatch());
		//floating point math
		API(sysTrapFlpDispatch,flpdispatch());//Float Library trap
		API(sysTrapFlpEmDispatch,flpemdispatch());//FPU emulator trap
		//other
		API(sysTrapPinsDispatch,pinsdispatch());

		//misc
		API(sysTrapHostControl,D0 = 0);
		API(sysTrapLocGetNumberSeparators,locgetnumberseparators());
		API(sysTrapErrExceptionList,errexceptionlist());
		API(sysTrapClipboardGetItem,clipboardgetitem());
		API(sysTrapDlkGetSyncInfo,dlkgetsyncinfo());

		//tapwave zodiac api
		API(sysTrapOEMDispatch2,tapwavedispatch());

		//switch to ARM instruction set
		API(sysTrapPceNativeCall,pcenativecall());

		//filesystem
		API(sysTrapFileSystemDispatch,filesystemdispatch());

		default:
			dbgprintf("Trap At:%#010x,Api:%s,0x%04x\n",MC68000_getpc(),lookup_trap(api),api);
			char meep[100];
			sprintf(meep,"Trap At:%#010x,Api:%s,0x%04x",MC68000_getpc(),lookup_trap(api),api);
			string output = meep;
			output += " Your freekum leaks wonderfly";
			//showBSOD("Your freekum leaks wonderfly");
			showBSOD(output);
			palmabrt();
			return false;
	}
	//0xA460 sound related,sysTrapSndPlayResource,sysTrapWinEraseWindow,sysTrapLstSetSelection
	//sysTrapSysStringByIndex,sysTrapFntGetFont,sysTrapFrmCopyLabel,sysTrapLstSetListChoices,
	//sysTrapFntCharHeight

	//(*apilist[api & 0xFFF])();
	//dbgprintf("Trap At:%#010x,Api:%s,0x%04x\n",MC68000_getpc(),lookup_trap(api),api);
	return false;
}