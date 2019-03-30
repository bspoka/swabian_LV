#include <stdlib.h>
#include <stdio.h>
#include "picoquantHeaders.h"
#define MAX_PATH_LENGTH 256

int main(int argc, char *argv[])
{

    if (argc < 7)
    {   
        char *error_msg = "Not enough input Arguments... \n (1.data_comment, "
        "2.acq_mode, 3.acq_time, 4.sync_rate, 5.num_records, 6. data_path\n";

        printf("%s", error_msg);
        return EXIT_FAILURE;
    }
    else
    {
        //Copy the first 256 characters of the notes from command line
        char data_comment[COMMENT_LENGTH] = {""};
        int notes_length = strlen(argv[1]);
        int copy_length = COMMENT_LENGTH;
        if (notes_length < COMMENT_LENGTH)
            copy_length = notes_length;
        memcpy(data_comment, argv[1], copy_length);

        //copy the acq mode string
        int32_t acq_mode = atoi(argv[2]);

        //copy the acq_time string
        int32_t acq_time = atoi(argv[3]);

        //copy the sync_rate string
        int32_t sync_rate = atoi(argv[4]);

        //copy the number of records
        char *endptr;
        uint64_t num_records = strtoll(argv[5], &endptr, 10);

        //copy the path string
        char data_path[MAX_PATH_LENGTH] = {""};
        int path_length = strlen(argv[6]);
        copy_length = MAX_PATH_LENGTH;
        if (path_length < MAX_PATH_LENGTH)
            copy_length = path_length;
        memcpy(data_path, argv[6], copy_length);

        HT3v2Header swabian_header;
        swabian_header = generate_swabian_tt20_header(data_comment, acq_mode, acq_time, sync_rate, num_records);

        FILE *outfile;

        // open file for writing
        outfile = fopen(data_path, "w");
        // write struct to file
        fwrite(&swabian_header, sizeof(HT3v2Header), 1, outfile);

        // close file
        fclose(outfile);
    }

    return EXIT_SUCCESS;
}