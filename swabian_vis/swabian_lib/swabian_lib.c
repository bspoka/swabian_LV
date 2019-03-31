#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#define COMMENT_LENGTH 256
#define MIN_BUFFER_LENGTH 1E6 // must always be even

__declspec(dllexport) int __cdecl swabian_convert_rawdata_to_photons(char *raw_filepath, char *output_filepath, int sync_channel,
                                                                     int64_t sync_period_ps);

__declspec(dllexport) int __cdecl write_swabian_ht3v2_header(char *data_comment, int32_t acq_mode, int32_t acq_time,
                                                             int32_t sync_rate, uint64_t num_records, char *data_path);

__declspec(dllexport) int __cdecl swabian_buffer_to_photons(int sync_channel, int64_t sync_period_ps, int64_t *buffer,
                                                            int buffer_length, int first_batch, int64_t *missing_sync_data, int *missing_events,
                                                            int64_t *first_sync_stamp, int64_t *output_buffer, int *output_length);

int get_datafile_size(FILE **ptr, char *raw_filepath);
void insert_into_buffer(int64_t *buffer, int buffer_size, int64_t *addition,
                        int addition_size, int location, int64_t *combined_buffer, int *combined_length);
void print_array(int64_t *array, int array_length, char *array_name);
int find_array_element(int64_t *array, int array_size, int64_t element);
int count_occurence(int64_t *array, int array_size, int64_t element);
void safeFree(void *pntr);


int swabian_buffer_to_photons(int sync_channel, int64_t sync_period_ps, int64_t *buffer,
        int buffer_length, int first_batch, int64_t *missing_sync_data, int *missing_events,
         int64_t *first_sync_stamp, int64_t *output_buffer, int *output_length)
{
    //Allocate buffers
    //int64_t *missing_sync_data = NULL;
    //int unpaired_events = 0;
    //for (int ind = 0; ind <= 10; ind++)
        //-----------------------------------------------------------------------------------//
        //combine the newly read buffer with previous stamps that didnt have an associated sync
        int combined_length = 0;
        int *length_ptr = &combined_length;
        int unpaired_events = *missing_events;
        int64_t *combined_buffer = NULL;

        if (unpaired_events > 0)
        {
            combined_buffer = calloc(buffer_length + unpaired_events, sizeof(int64_t));
            insert_into_buffer(buffer, buffer_length, missing_sync_data,
                               unpaired_events, 0, combined_buffer, length_ptr);
        }
        else
        {
            combined_buffer = calloc(buffer_length, sizeof(int64_t));
            insert_into_buffer(buffer, buffer_length, NULL,
                               0, 0, combined_buffer, length_ptr);
        }
        //free(buffer);
        combined_length = *length_ptr;
        //print_array(combined_buffer, combined_length, "combined_buffer:");

        //-----------------------------------------------------------------------------------//
        //look for stamps with no syncs
        unpaired_events = 0;
        for (int offst = 2; offst < combined_length; unpaired_events++)
        {
            //printf("buffer %1llu\n", combined_buffer[combined_length - offst]);
            if (combined_buffer[combined_length - offst] == sync_channel)
                break;
            offst += 2;
        }
        //-----------------------------------------------------------------------------------//

        //-----------------------------------------------------------------------------------//
        //put the stamps with no associated sync into a new array
        int good_buffer_length = 0;
        //missing_sync_data = (int64_t *)calloc(2 * unpaired_events, sizeof(int64_t));
        for (int k = 0; k < 2 * unpaired_events; k++)
        {
            *(missing_sync_data+k) = combined_buffer[combined_length - 2 * unpaired_events + k];
        }
        //print_array(missing_sync_data, 2 * unpaired_events, "missing_sync_data:");
        good_buffer_length = combined_length - 2 * unpaired_events;
        //-----------------------------------------------------------------------------------//

        //-----------------------------------------------------------------------------------//
        //split buffer into channels/timestamps
        int num_events = good_buffer_length / 2;

        int64_t *stamps = (int64_t *)calloc(num_events, sizeof(int64_t));
        int64_t *channels = (int64_t *)calloc(num_events, sizeof(int64_t));

        int ind_channel, ind_stamp, counter;
        for (counter = 0, ind_channel = 0, ind_stamp = 1;
             counter < num_events; ind_channel += 2, ind_stamp += 2, counter++)
        {
            channels[counter] = combined_buffer[ind_channel];
            stamps[counter] = combined_buffer[ind_stamp];
            //printf("channels: %1llu, stamps: %2llu\n", channels[counter], stamps[counter]);
        }
        //-----------------------------------------------------------------------------------//

        //-----------------------------------------------------------------------------------//
        //this only runs on the first iteration and determines whether the sync comes before or
        //after the count and finds the timestamp of the first sync
        if (first_batch == 1)
        {
            int m = find_array_element(channels, num_events, sync_channel);
            if (m == -1)
            {
                //printf("Could not find a sync");
                return EXIT_FAILURE;
            }
            else
                *first_sync_stamp = stamps[m];
        }
        //-----------------------------------------------------------------------------------//

        //-----------------------------------------------------------------------------------//
        //count the number of nonsync and sync events
        int num_sync = count_occurence(channels, num_events, sync_channel);
        int num_nonsync = num_events - num_sync;
        //-----------------------------------------------------------------------------------//

        //-----------------------------------------------------------------------------------//
        //split the stamps into syncs and nonsyncs and nonsync channels
        //sync array are the sync arrival times
        //nonsync array are the detector arrival times
        //nonsync_channels are the channels of the detector timetags
        int64_t *nonsync = (int64_t *)calloc((num_nonsync), sizeof(int64_t));
        int64_t *nonsync_channels = (int64_t *)calloc((num_nonsync), sizeof(int64_t));
        int64_t *sync = (int64_t *)calloc((num_sync), sizeof(int64_t));
        int sync_counter = 0;
        int nonsync_counter = 0;
        int just_found_sync = 0;
        for (int k = 0; k < num_events; k++)
        {
            if (channels[k] != sync_channel) //normal copy detector count
            {
                nonsync[nonsync_counter] = stamps[k];
                nonsync_channels[nonsync_counter] = channels[k];
                just_found_sync = 0;
                //printf("stamps: %1llu\n", nonsync[nonsync_counter]);
                nonsync_counter++;
            }
            else
            {
                //this part skips multiple syncs with no associated detector counts
                //this is an artifact of conditional filtering. if syncs come after counts
                //first sync is kept, otherwise the last sync is kept.
                if (just_found_sync == 1)
                    ;
                else //normal copy sync
                {
                    sync[sync_counter] = stamps[k];
                    sync_counter++;
                }
                just_found_sync = 1;
                //print_array(sync, num_sync, "all_syncs: ");
            }
        }
        sync = realloc(sync, sync_counter * sizeof(int64_t));
        num_sync = sync_counter;
        //print_array(sync, num_sync, "all_syncs: ");
        //print_array(nonsync, num_nonsync, "all_nonsync: ");
        //-----------------------------------------------------------------------------------//

        //-----------------------------------------------------------------------------------//
        //parse the buffer into the output stream and write it to file
        //output stream consists of triples 1.channel 2. sync number 3. sync to count delay
        sync_counter = 0;
        //output_buffer = (int64_t *)calloc((num_nonsync)*3, sizeof(int64_t));
        //print_array(*output_buffer, num_nonsync * 3, "output_buffer");
        for (int l = 0; l < num_nonsync; l++)
        {
            while (nonsync[l] > sync[sync_counter] && sync_counter < num_sync)
                sync_counter++;

            *(output_buffer + 3 * l) = nonsync_channels[l];
            //print_array(*output_buffer, num_nonsync * 3, "output_buffer");
            *(output_buffer + 3 * l + 1) = (sync[sync_counter] - *first_sync_stamp) / sync_period_ps + 1;
            //print_array(*output_buffer, num_nonsync * 3, "output_buffer");
            *(output_buffer + 3 * l + 2) = sync[sync_counter]-nonsync[l];
        }
        //print_array(output_buffer, num_nonsync * 3, "output_buffer");
        //-----------------------------------------------------------------------------------//
        *output_length = (num_nonsync)*3;
        *missing_events = 2 * unpaired_events; 

        free(nonsync);
        free(nonsync_channels);
        free(sync);
        free(stamps);
        free(channels);
        free(combined_buffer);

    return EXIT_SUCCESS;
}

int swabian_convert_rawdata_to_photons(char *raw_filepath, char *output_filepath, int sync_channel,
                                       int64_t sync_period_ps)
{
    FILE *input_file = NULL;
    FILE *output_file = NULL;
    //read raw data file and determine its size
    int data_length = get_datafile_size(&input_file, raw_filepath);
    int buffer_length = MIN_BUFFER_LENGTH;
    if (data_length < MIN_BUFFER_LENGTH)
        buffer_length = data_length;

    int last_batch_length = data_length % buffer_length;
    int num_loops = (last_batch_length > 0) ? (data_length / buffer_length + 1) : (data_length / buffer_length);
    if (num_loops == 1)
        last_batch_length = buffer_length;

    //Allocate buffers
    int64_t *missing_sync_data = NULL;
    int64_t first_sync_stamp = 0;
    output_file = fopen(output_filepath, "wb");
    if (output_file == NULL)
        return EXIT_FAILURE;

    int unpaired_events = 0;
    //for (int ind = 0; ind <= 10; ind++)
    for (int ind = 0; ind < num_loops; ind++)
    {
        //-----------------------------------------------------------------------------------//
        //buffer always starts with channel first followed by timestamp
        if (ind == num_loops - 1)
            buffer_length = last_batch_length;

        int64_t *read_buffer = (int64_t *)calloc(buffer_length, sizeof(int64_t));
        fread(read_buffer, sizeof(int64_t), buffer_length, input_file); //read into buffer
        //print_array(read_buffer, buffer_length, "read_buffer:");
        //-----------------------------------------------------------------------------------//

        //-----------------------------------------------------------------------------------//
        //combine the newly read buffer with previous stamps that didnt have an associated sync
        int combined_length = 0;
        int *length_ptr = &combined_length;
        int64_t *combined_buffer = NULL;
        if (unpaired_events > 0)
        {
            combined_buffer = calloc(buffer_length + 2 * unpaired_events, sizeof(int64_t));
            insert_into_buffer(read_buffer, buffer_length, missing_sync_data,
                               2 * unpaired_events, 0, combined_buffer, length_ptr);
            free(missing_sync_data);
        }
        else
        {
            combined_buffer = calloc(buffer_length, sizeof(int64_t));
            insert_into_buffer(read_buffer, buffer_length, NULL,
                               0, 0, combined_buffer, length_ptr);
        }
        free(read_buffer);
        combined_length = *length_ptr;
        //print_array(combined_buffer, combined_length, "combined_buffer:");

        //-----------------------------------------------------------------------------------//
        //look for stamps with no syncs
        unpaired_events = 0;
        for (int offst = 2; offst < combined_length; unpaired_events++)
        {
            //printf("buffer %1llu\n", combined_buffer[combined_length - offst]);
            if (combined_buffer[combined_length - offst] == sync_channel)
                break;
            offst += 2;
        }
        //-----------------------------------------------------------------------------------//

        //-----------------------------------------------------------------------------------//
        //put the stamps with no associated sync into a new array
        int good_buffer_length = 0;
        missing_sync_data = (int64_t *)calloc(2 * unpaired_events, sizeof(int64_t));
        for (int k = 0; k < 2 * unpaired_events; k++)
        {
            missing_sync_data[k] = combined_buffer[combined_length - 2 * unpaired_events + k];
        }
        //print_array(missing_sync_data, 2 * unpaired_events, "missing_sync_data:");
        good_buffer_length = combined_length - 2 * unpaired_events;
        //-----------------------------------------------------------------------------------//

        //-----------------------------------------------------------------------------------//
        //split buffer into channels/timestamps
        int num_events = good_buffer_length / 2;

        int64_t *stamps = (int64_t *)calloc(num_events, sizeof(int64_t));
        int64_t *channels = (int64_t *)calloc(num_events, sizeof(int64_t));

        int ind_channel, ind_stamp, counter;
        for (counter = 0, ind_channel = 0, ind_stamp = 1;
             counter < num_events; ind_channel += 2, ind_stamp += 2, counter++)
        {
            channels[counter] = combined_buffer[ind_channel];
            stamps[counter] = combined_buffer[ind_stamp];
            //printf("channels: %1llu, stamps: %2llu\n", channels[counter], stamps[counter]);
        }
        //-----------------------------------------------------------------------------------//

        //-----------------------------------------------------------------------------------//
        //this only runs on the first iteration and determines whether the sync comes before or
        //after the count and finds the timestamp of the first sync
        if (ind == 0)
        {
            int m = find_array_element(channels, num_events, sync_channel);
            if (m == -1)
            {
                //printf("Could not find a sync");
                return EXIT_FAILURE;
            }
            else
                first_sync_stamp = stamps[m];
        }
        //-----------------------------------------------------------------------------------//

        //-----------------------------------------------------------------------------------//
        //count the number of nonsync and sync events
        int num_sync = count_occurence(channels, num_events, sync_channel);
        int num_nonsync = num_events - num_sync;
        //-----------------------------------------------------------------------------------//

        //-----------------------------------------------------------------------------------//
        //split the stamps into syncs and nonsyncs and nonsync channels
        //sync array are the sync arrival times
        //nonsync array are the detector arrival times
        //nonsync_channels are the channels of the detector timetags
        int64_t *nonsync = (int64_t *)calloc((num_nonsync), sizeof(int64_t));
        int64_t *nonsync_channels = (int64_t *)calloc((num_nonsync), sizeof(int64_t));
        int64_t *sync = (int64_t *)calloc((num_sync), sizeof(int64_t));
        int sync_counter = 0;
        int nonsync_counter = 0;
        int just_found_sync = 0;
        for (int k = 0; k < num_events; k++)
        {
            if (channels[k] != sync_channel) //normal copy detector count
            {
                nonsync[nonsync_counter] = stamps[k];
                nonsync_channels[nonsync_counter] = channels[k];
                just_found_sync = 0;
                //printf("stamps: %1llu\n", nonsync[nonsync_counter]);
                nonsync_counter++;
            }
            else
            {
                //this part skips multiple syncs with no associated detector counts
                //this is an artifact of conditional filtering. if syncs come after counts
                //first sync is kept, otherwise the last sync is kept.
                if (just_found_sync == 1)
                    ;
                else //normal copy sync
                {
                    sync[sync_counter] = stamps[k];
                    sync_counter++;
                }
                just_found_sync = 1;
                //print_array(sync, num_sync, "all_syncs: ");
            }
        }
        sync = realloc(sync, sync_counter * sizeof(int64_t));
        num_sync = sync_counter;
        //print_array(sync, num_sync, "all_syncs: ");
        //-----------------------------------------------------------------------------------//

        //-----------------------------------------------------------------------------------//
        //parse the buffer into the output stream and write it to file
        //output stream consists of triples 1.channel 2. sync number 3. sync to count delay
        sync_counter = 0;
        int64_t *output_buffer = (int64_t *)calloc((num_nonsync)*3, sizeof(int64_t));
        for (int l = 0; l < num_nonsync; l++)
        {
            while (nonsync[l] > sync[sync_counter] && sync_counter < num_sync)
                sync_counter++;

            output_buffer[3 * l] = nonsync_channels[l];
            output_buffer[3 * l + 1] = (sync[sync_counter] - first_sync_stamp) / sync_period_ps + 1;
            output_buffer[3 * l + 2] = (int64_t)llabs((int64_t)nonsync[l] - ((int64_t)sync[sync_counter] - (int64_t)sync_period_ps));
        }
        //print_array(output_buffer, num_nonsync * 3, "output_buffer");
        fwrite(output_buffer, sizeof(int64_t), (num_nonsync)*3, output_file);
        //-----------------------------------------------------------------------------------//

        free(output_buffer);
        free(nonsync);
        free(nonsync_channels);
        free(sync);
        free(stamps);
        free(channels);
        free(combined_buffer);
    }

    fclose(output_file);
    fclose(input_file);
    return EXIT_SUCCESS;
}


int get_datafile_size(FILE **ptr, char *raw_filepath)
{
    int64_t record;
    int data_length = 0;

    *ptr = fopen(raw_filepath, "rb");
    if (ptr == NULL)
    {
        //printf("Error opening raw data file");
        return EXIT_FAILURE;
    }
    while (fread(&record, sizeof(int64_t), 1, *ptr))
        data_length++;
    //printf("File size is: %1d records\n", data_length);
    fseek(*ptr, 0, SEEK_SET);

    return data_length;
}

void insert_into_buffer(int64_t *buffer, int buffer_size, int64_t *addition,
                        int addition_size, int location, int64_t *combined_buffer, int *combined_length)
{
    int counter = 0;
    if (location == 0)
    {
        if (addition == NULL)
            ;
        else
        {
            for (int i = 0; i < addition_size; i++)
            {
                combined_buffer[counter] = addition[i];
                counter++;
            }
        }
        for (int j = 0; j < buffer_size; j++)
        {
            combined_buffer[counter] = buffer[j];
            counter++;
        }
    }
    else
    {
        for (int j = 0; j < buffer_size; j++)
        {
            combined_buffer[counter] = buffer[j];
            counter++;
        }
        if (addition == NULL)
            ;
        else
        {
            for (int i = 0; i < addition_size; i++)
            {
                combined_buffer[counter] = addition[i];
                counter++;
            }
        }
    }
    *combined_length = buffer_size + addition_size;
}

void print_array(int64_t *array, int array_length, char *array_name)
{
    int name_length = strlen(array_name);
    char format[] = " %1llu\n";
    int format_length = strlen(format);
    char *messg = malloc(sizeof(char) * (format_length + name_length));
    strcpy(messg, array_name);
    strcat(messg, format);
    for (int ind = 0; ind < array_length; ind++)
        printf(messg, array[ind]);
}

int find_array_element(int64_t *array, int array_size, int64_t element)
{
    for (int ind = 0; ind < array_size; ind++)
    {
        if (array[ind] == element)
            return ind;
    }
    return -1;
}

int count_occurence(int64_t *array, int array_size, int64_t element)
{
    int counter = 0;
    for (int ind = 0; ind < array_size; ind++)
    {
        if (array[ind] == element)
            counter++;
    }
    return counter;
}

void safeFree(void *pntr)
{
    if (pntr != NULL)
        free(pntr);
}

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

HT3v2Header generate_swabian_tt20_header(char *Comment, int32_t mode, int32_t acq_time, int32_t sync_rate,
                                         int64_t num_records)
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
    memset(swabian_header.DispCurveMapTo, 0, sizeof(swabian_header.DispCurveMapTo));
    memset(swabian_header.DispCurveShow, 0, sizeof(swabian_header.DispCurveShow));

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

int write_swabian_ht3v2_header(char *data_comment, int32_t acq_mode, int32_t acq_time,
                               int32_t sync_rate, uint64_t num_records, char *data_path)
{
    HT3v2Header swabian_header;
    swabian_header = generate_swabian_tt20_header(data_comment, acq_mode, acq_time, sync_rate, num_records);

    FILE *outfile = NULL;

    // open file for writing
    outfile = fopen(data_path, "w");
    if (outfile == NULL)
        return EXIT_FAILURE;
    // write struct to file
    fwrite(&swabian_header, sizeof(HT3v2Header), 1, outfile);

    // close file
    fclose(outfile);
    return EXIT_SUCCESS;
}
