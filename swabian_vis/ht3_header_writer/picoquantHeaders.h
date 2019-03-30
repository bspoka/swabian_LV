#include "stdint.h" 
#include <string.h>
#define COMMENT_LENGTH 256

//Picoquant .ht3 version 2 header type definition
typedef struct
{
    //Ascii File Header
    char Ident[16];
    char FormatVersion[6];
    char CreatorName[18];
    char CreatorVersion[12];
    char FileTime[18];
    char CRLF[2];
    char Comment[COMMENT_LENGTH];

    //Binary File header
    int32_t NumberOfCurves;
    int32_t BitsPerRecord;
    int32_t ActiveCurve;
    int32_t MeasurementMode;
    int32_t SubMode;
    int32_t Binning;
    double Resolution;
    int32_t Offset;
    int32_t Tacq;
    uint32_t StopAt;
    int32_t StopOnOvfl;
    int32_t Restart;
    int32_t DispLinLog;
    int32_t DispTimeAxisFrom;
    int32_t DispTimeAxisTo;
    int32_t DispCountAxisFrom;
    int32_t DispCountAxisTo;
    int32_t DispCurveMapTo[8];
    int32_t DispCurveShow[8];
    float ParamStart[3];
    float ParamStep[3];
    float ParamEnd[3];
    int32_t RepeatMode;
    int32_t RepeatsPerCurve;
    int32_t Repaobjime;
    int32_t RepeatWaiobjime;
    char ScriptName[20];

    //Hardware Info Header
    char HardwareIdent[16];
    char HardwarePartNo[8];
    int32_t HardwareSerial;
    int32_t nModulesPresent;
    int32_t ModelCode[10];
    int32_t VersionCode[10];
    double BaseResolution;
    uint64_t InputsEnabled;
    int32_t InpChansPresent;
    int32_t RefClockSource;
    int32_t ExtDevices;
    int32_t MarkerSeobjings;
    int32_t SyncDivider;
    int32_t SyncCFDLevel;
    int32_t SyncCFDZeroCross;
    int32_t SyncOffset;

    //Channels Info Header
    int32_t InputModuleIndex[8];
    int32_t InputCFDLevel[8];
    int32_t InputCFDZeroCross[8];
    int32_t InputOffset[8];

    //time_tagginh header
    int32_t InputRate[8];
    int32_t SyncRate;
    int32_t StopAfter;
    int32_t StopReason;
    int32_t ImgHdrSize;
    uint64_t nRecords;
    int32_t ImgHdr;
} HT3v2Header;

HT3v2Header generate_swabian_tt20_header(char *Comment, int32_t mode, int32_t acq_time,int32_t sync_rate,
    uint64_t num_records)
{
    HT3v2Header swabian_header;
    char empty_space[sizeof(swabian_header.Ident)] = {""};
    memcpy(swabian_header.Ident, empty_space, sizeof(empty_space));

    char CreatorName[sizeof(swabian_header.CreatorName)] = {"Bawendi"};
    memcpy(swabian_header.CreatorName, CreatorName, sizeof(CreatorName));

    char CreatorVersion[sizeof(swabian_header.CreatorVersion)] = {""};
    memcpy(swabian_header.CreatorVersion, CreatorVersion, sizeof(CreatorVersion));

    char FormatVersion[sizeof(swabian_header.FormatVersion)] = {"2.0"};
    memcpy(swabian_header.FormatVersion, FormatVersion, sizeof(FormatVersion));

    char FileTime[sizeof(swabian_header.FileTime)] = {""};
    memcpy(swabian_header.FileTime, FileTime, sizeof(FileTime));

    char CRLF[sizeof(swabian_header.CRLF)] = {""};
    memcpy(swabian_header.CRLF, CRLF, sizeof(CRLF));

    memcpy(swabian_header.Comment, Comment, sizeof(swabian_header.Comment));

    swabian_header.NumberOfCurves = 0;
    swabian_header.BitsPerRecord = 0;
    swabian_header.ActiveCurve = 0;
    swabian_header.MeasurementMode = mode;
    swabian_header.SubMode = 0;
    swabian_header.Binning = 0;
    swabian_header.Resolution = 34;
    swabian_header.Offset = 0;
    swabian_header.Tacq = acq_time;
    swabian_header.StopAt = 0;
    swabian_header.StopOnOvfl = 0;
    swabian_header.Restart = 0;
    swabian_header.DispLinLog = 0;
    swabian_header.DispTimeAxisFrom = 0;
    swabian_header.DispTimeAxisTo = 0;
    swabian_header.DispCountAxisFrom = 0;
    swabian_header.DispCountAxisTo = 0;
    memset(swabian_header.DispCurveMapTo,0,sizeof(swabian_header.DispCurveMapTo));
    memset(swabian_header.DispCurveShow,0,sizeof(swabian_header.DispCurveShow));

    memset(swabian_header.ParamStart, 0, sizeof(swabian_header.ParamStart));
    memset(swabian_header.ParamStep, 0, sizeof(swabian_header.ParamStep));
    memset(swabian_header.ParamEnd, 0, sizeof(swabian_header.ParamEnd));
    swabian_header.RepeatMode = 0;
    swabian_header.RepeatsPerCurve = 0;
    swabian_header.Repaobjime = 0;
    swabian_header.RepeatWaiobjime = 0;
    char ScriptName[sizeof(swabian_header.ScriptName)] = {""};
    memcpy(swabian_header.ScriptName, ScriptName, sizeof(ScriptName));

    //Hardware Info Header
    char HardwareIdent[sizeof(swabian_header.HardwareIdent)] = {"1740000JG3"};
    memcpy(swabian_header.HardwareIdent, HardwareIdent, sizeof(HardwareIdent));

    char HardwarePartNo[sizeof(swabian_header.HardwarePartNo)] = {"TT20"};
    memcpy(swabian_header.HardwarePartNo, HardwarePartNo, sizeof(HardwarePartNo));

    swabian_header.HardwareSerial = 0;
    swabian_header.nModulesPresent = 1;
    memset(swabian_header.ModelCode, 0, sizeof(swabian_header.ModelCode));
    memset(swabian_header.VersionCode, 0, sizeof(swabian_header.VersionCode));
    swabian_header.BaseResolution = 3;
    swabian_header.InputsEnabled = 8;
    swabian_header.InpChansPresent = 8;
    swabian_header.RefClockSource = 0;
    swabian_header.ExtDevices = 0;
    swabian_header.MarkerSeobjings = 0;
    swabian_header.SyncDivider = 0;
    swabian_header.SyncCFDLevel = 0;
    swabian_header.SyncCFDZeroCross = 0;
    swabian_header.SyncOffset = 0;

    //Channels Info Header
    memset(swabian_header.InputModuleIndex, 0, sizeof(swabian_header.InputModuleIndex));
    memset(swabian_header.InputCFDLevel, 0, sizeof(swabian_header.InputCFDLevel));
    memset(swabian_header.InputCFDZeroCross, 0, sizeof(swabian_header.InputCFDZeroCross));
    memset(swabian_header.InputOffset, 0, sizeof(swabian_header.InputOffset));

    //time_tagginh header
    memset(swabian_header.InputRate, 0, sizeof(swabian_header.InputRate));
    swabian_header.SyncRate = sync_rate;
    swabian_header.StopAfter = 0;
    swabian_header.StopReason = 0;
    swabian_header.ImgHdrSize = 0;
    swabian_header.nRecords = num_records;
    swabian_header.ImgHdr = 0;

    return swabian_header;
}

