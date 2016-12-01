//---------------------------------------------------------------------------
#ifndef FileOperationProgressH
#define FileOperationProgressH
//---------------------------------------------------------------------------
#include "Configuration.h"
#include "CopyParam.h"
#include "Exceptions.h"
#include <vector>
//---------------------------------------------------------------------------
class TFileOperationProgressType;
enum TFileOperation { foNone, foCopy, foMove, foDelete, foSetProperties,
  foRename, foCustomCommand, foCalculateSize, foRemoteMove, foRemoteCopy,
  foGetProperties, foCalculateChecksum, foLock, foUnlock };
// csCancelTransfer and csRemoteAbort are used with SCP only
enum TCancelStatus { csContinue = 0, csCancelFile, csCancel, csCancelTransfer, csRemoteAbort };
enum TBatchOverwrite { boNo, boAll, boNone, boOlder, boAlternateResume, boAppend, boResume };
typedef void __fastcall (__closure *TFileOperationProgressEvent)
  (TFileOperationProgressType & ProgressData);
typedef void __fastcall (__closure *TFileOperationFinished)
  (TFileOperation Operation, TOperationSide Side, bool Temp,
    const UnicodeString & FileName, bool Success, TOnceDoneOperation & OnceDoneOperation);
//---------------------------------------------------------------------------
class TFileOperationProgressType
{
private:
  TFileOperation FOperation;
  TOperationSide FSide;
  UnicodeString FFileName;
  UnicodeString FFullFileName;
  UnicodeString FDirectory;
  bool FAsciiTransfer;
  bool FTransferingFile;
  bool FTemp;
  __int64 FLocalSize;
  __int64 FLocallyUsed;
  __int64 FTransferSize;
  __int64 FTransferredSize;
  __int64 FSkippedSize;
  bool FInProgress;
  bool FFileInProgress;
  TCancelStatus FCancel;
  int FCount;
  TDateTime FStartTime;
  __int64 FTotalTransferred;
  __int64 FTotalSkipped;
  __int64 FTotalSize;
  TBatchOverwrite FBatchOverwrite;
  bool FSkipToAll;
  unsigned long FCPSLimit;
  bool FTotalSizeSet;
  bool FSuspended;

  // when it was last time suspended (to calculate suspend time in Resume())
  unsigned int FSuspendTime;
  // when current file was started being transferred
  TDateTime FFileStartTime;
  int FFilesFinished;
  TFileOperationProgressEvent FOnProgress;
  TFileOperationFinished FOnFinished;
  bool FReset;
  unsigned int FLastSecond;
  unsigned long FRemainingCPS;
  bool FCounterSet;
  std::vector<unsigned long> FTicks;
  std::vector<__int64> FTotalTransferredThen;

protected:
  void __fastcall ClearTransfer();
  inline void __fastcall DoProgress();
  int __fastcall OperationProgress();

public:
  // common data
  __property TFileOperation Operation = { read = FOperation };
  // on what side if operation being processed (local/remote), source of copy
  __property TOperationSide Side = { read = FSide };
  __property UnicodeString FileName =  { read = FFileName };
  __property UnicodeString FullFileName = { read = FFullFileName };
  __property UnicodeString Directory = { read = FDirectory };
  __property bool AsciiTransfer = { read = FAsciiTransfer };
  // Can be true with SCP protocol only
  __property bool TransferingFile = { read = FTransferingFile };
  __property bool Temp = { read = FTemp };

  // file size to read/write
  __property __int64 LocalSize = { read = FLocalSize };
  __property __int64 LocallyUsed = { read = FLocallyUsed };
  __property __int64 TransferSize = { read = FTransferSize };
  __property __int64 TransferredSize = { read = FTransferredSize };
  __property __int64 SkippedSize = { read = FSkippedSize };
  __property bool InProgress = { read = FInProgress };
  __property bool FileInProgress = { read = FFileInProgress };
  __property TCancelStatus Cancel = { read = FCancel };
  __property int Count = { read = FCount };
  // when operation started
  __property TDateTime StartTime = { read = FStartTime };
  // bytes transferred
  __property __int64 TotalTransferred = { read = FTotalTransferred };
  __property __int64 TotalSkipped = { read = FTotalSkipped };
  __property __int64 TotalSize = { read = FTotalSize };

  __property TBatchOverwrite BatchOverwrite = { read = FBatchOverwrite };
  __property bool SkipToAll = { read = FSkipToAll };
  __property unsigned long CPSLimit = { read = FCPSLimit };

  __property bool TotalSizeSet = { read = FTotalSizeSet };

  __property bool Suspended = { read = FSuspended };

  __fastcall TFileOperationProgressType();
  __fastcall TFileOperationProgressType(
    TFileOperationProgressEvent AOnProgress, TFileOperationFinished AOnFinished);
  __fastcall ~TFileOperationProgressType();
  void __fastcall AssignButKeepSuspendState(const TFileOperationProgressType & Other);
  void __fastcall AddLocallyUsed(__int64 ASize);
  void __fastcall AddTransferred(__int64 ASize, bool AddToTotals = true);
  void __fastcall AddResumed(__int64 ASize);
  void __fastcall AddSkippedFileSize(__int64 ASize);
  void __fastcall Clear();
  unsigned int __fastcall CPS();
  void __fastcall Finish(UnicodeString FileName, bool Success,
    TOnceDoneOperation & OnceDoneOperation);
  void __fastcall Progress();
  unsigned long __fastcall LocalBlockSize();
  bool __fastcall IsLocallyDone();
  bool __fastcall IsTransferDone();
  void __fastcall SetFile(UnicodeString AFileName, bool AFileInProgress = true);
  void __fastcall SetFileInProgress();
  unsigned long __fastcall TransferBlockSize();
  unsigned long __fastcall AdjustToCPSLimit(unsigned long Size);
  void __fastcall ThrottleToCPSLimit(unsigned long Size);
  static unsigned long __fastcall StaticBlockSize();
  void __fastcall Reset();
  void __fastcall Resume();
  void __fastcall SetLocalSize(__int64 ASize);
  void __fastcall SetAsciiTransfer(bool AAsciiTransfer);
  void __fastcall SetTransferSize(__int64 ASize);
  void __fastcall ChangeTransferSize(__int64 ASize);
  void __fastcall RollbackTransfer();
  void __fastcall SetTotalSize(__int64 ASize);
  void __fastcall Start(TFileOperation AOperation, TOperationSide ASide, int ACount);
  void __fastcall Start(TFileOperation AOperation,
    TOperationSide ASide, int ACount, bool ATemp, const UnicodeString ADirectory,
    unsigned long ACPSLimit);
  void __fastcall Stop();
  void __fastcall Suspend();
  // whole operation
  TDateTime __fastcall TimeElapsed();
  // only current file
  TDateTime __fastcall TimeExpected();
  TDateTime __fastcall TotalTimeExpected();
  TDateTime __fastcall TotalTimeLeft();
  int __fastcall TransferProgress();
  int __fastcall OverallProgress();
  int __fastcall TotalTransferProgress();
  void __fastcall SetSpeedCounters();
  void __fastcall SetTransferingFile(bool ATransferingFile);
  void __fastcall SetCancel(TCancelStatus ACancel);
  void __fastcall SetCancelAtLeast(TCancelStatus ACancel);
  bool __fastcall ClearCancelFile();
  void __fastcall SetCPSLimit(unsigned long ACPSLimit);
  void __fastcall SetBatchOverwrite(TBatchOverwrite ABatchOverwrite);
  void __fastcall SetSkipToAll();
};
//---------------------------------------------------------------------------
class TSuspendFileOperationProgress
{
public:
  __fastcall TSuspendFileOperationProgress(TFileOperationProgressType * OperationProgress)
  {
    FOperationProgress = OperationProgress;
    if (FOperationProgress != NULL)
    {
      FOperationProgress->Suspend();
    }
  }

  __fastcall ~TSuspendFileOperationProgress()
  {
    if (FOperationProgress != NULL)
    {
      FOperationProgress->Resume();
    }
  }

private:
  TFileOperationProgressType * FOperationProgress;
};
//---------------------------------------------------------------------------
#endif