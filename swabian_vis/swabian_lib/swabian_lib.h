	  //==============================================================================
//
// Title:		picoquant_corr.h
// Purpose:		A short description of the interface.
//
// Created on:	9/29/2017 at 9:40:29 AM by bspoka.
// Copyright:	Microsoft. All Rights Reserved.
//
//==============================================================================

#ifndef __swabian_lib_H__
#define __swabian_lib_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include "cvidef.h"
#include <stdint.h> 
#include <ansi_c.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>  
#include <string.h>
		
//==============================================================================
// Constants

//==============================================================================
// Types

int swabian_convert_rawdata_to_photons(char *raw_filepath, char *output_filepath, int sync_channel,
                                       uint64_t sync_period_ps);

int write_swabian_ht3v2_header(char *data_comment, int32_t acq_mode, int32_t acq_time,
                               int32_t sync_rate, uint64_t num_records, char *data_path);

int swabian_buffer_to_photons(int sync_channel, uint64_t sync_period_ps, uint64_t *buffer,
        int buffer_length, int first_batch, uint64_t *missing_sync_data, int *missing_events,
         uint64_t *first_sync_stamp, uint64_t *output_buffer, int *output_length);

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __picoquant_corr_H__ */
