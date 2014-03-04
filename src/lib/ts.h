/* vim: set tabstop=8 shiftwidth=8: */
//=============================================================================
// Name: ts.h
// Purpose: analyse ts stream
// To build: gcc -std-c99 -c ts.c
//=============================================================================

#ifndef _TS_H
#define _TS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "list.h"

#define INFO_LEN_MAX                    (0x0001 << 10) // pow(2, 10)
#define SERVER_STR_MAX                  (188) // max length of server string

//=============================================================================
// Struct definition:
//=============================================================================
typedef struct _ts_error_t
{
        // First priority: necessary for de-codability (basic monitoring)
        int TS_sync_loss; // 1.1
        int Sync_byte_error; // 1.2
        int PAT_error; // 1.3
        int PAT_error_2; // 1.3a
        int Continuity_count_error; // 1.4
        int PMT_error; // 1.5
        int PMT_error_2; // 1.5a
        int PID_error; // 1.6

        // Second priority: recommended for continuous or periodic monitoring
        int Transport_error; // 2.1
        int CRC_error; // 2.2
        // int PCR_error; 2.3 == (2.3a | 2.3b), invalid for new implementations!
        int PCR_repetition_error; // 2.3a
        int PCR_discontinuity_indicator_error; // 2.3b
        int PCR_accuracy_error; // 2.4
        int PTS_error; // 2.5
        int CAT_error; // 2.6

        // Third priority: application dependant monitoring
        int NIT_error; // 3.1
        int NIT_actual_error; // 3.1a
        int NIT_other_error; // 3.1b
        int SI_repetition_error; // 3.2
        int Buffer_error; // 3.3
        int Unreferenced_PID; // 3.4
        int Unreferenced_PID_2; // 3.4a
        int SDT_error; // 3.5
        int SDT_actual_error; // 3.5a
        int SDT_other_error; // 3.5b
        int EIT_error; // 3.6
        int EIT_actual_error; // 3.6a
        int EIT_other_error; // 3.6b
        int EIT_PF_error; // 3.6c
        int RST_error; // 3.7
        int TDT_error; // 3.8
        int Empty_buffer_error; // 3.9
        int Data_delay_error; // 3.10
}
ts_error_t; // TR 101 290 V1.2.1 2001-05

typedef struct _ts_psi_t
{
        uint8_t table_id; // TABLE_ID_TABLE
        uint8_t section_syntax_indicator; // 1-bit
        uint8_t private_indicator; // 1-bit
        uint8_t reserved0; // 2-bit
        uint16_t section_length; // 12-bit
        uint16_t table_id_extension; // transport_stream_id or program_number
        uint8_t reserved1; // 2-bit
        uint8_t version_number; // 5-bit
        uint8_t current_next_indicator; // 1-bit
        uint8_t section_number;
        uint8_t last_section_number;

        int check_CRC; // some table do not need to check CRC_32
        int type; // index of item in PID_TYPE[]
}
ts_psi_t;

typedef struct _ts_section_t
{
        // for list
        NODE *next;
        NODE *prev;
        uint32_t key; // copy section_number as key

        uint8_t section_number;
        uint16_t section_length;
        uint8_t data[4096];
}
ts_section_t; // unit of section list

typedef struct _ts_table_t
{
        // for list
        NODE *next;
        NODE *prev;
        uint32_t key; // copy table_id as key

        uint8_t table_id; // 0x00~0xFF except 0x02
        uint8_t version_number;
        uint8_t last_section_number;
        LIST section_list; // ts_section_t
}
ts_table_t; // unit of PSI/SI table list

typedef struct _ts_prog_t
{
        // for list
        NODE *next;
        NODE *prev;
        uint32_t key; // copy program_number as key

        // PMT section, 
        int is_parsed;
        ts_table_t table; // table_id = 0x02

        // program information
        uint16_t PMT_PID; // 13-bit
        uint16_t PCR_PID; // 13-bit
        uint16_t program_number;
        int program_info_len;
        uint8_t program_info[INFO_LEN_MAX];

        // service information
        uint8_t service_name_len;
        uint8_t service_name[SERVER_STR_MAX];
        uint8_t service_provider_len;
        uint8_t service_provider[SERVER_STR_MAX];

        // tracks
        LIST track_list; // ts_track_t, track list of this program

        // for STC calc
        uint64_t ADDa; // PCR packet a: packet address
        int64_t PCRa; // PCR packet a: PCR value
        uint64_t ADDb; // PCR packet b: packet address
        int64_t PCRb; // PCR packet b: PCR value
        int STC_sync; // true: PCRa and PCRb OK, STC can be calc
        uint64_t interval;
}
ts_prog_t; // unit of prog list

typedef struct _ts_track_t
{
        // for list
        NODE *next;
        NODE *prev;
        uint32_t key; // copy PID as key

        // PID
        uint16_t PID; // 13-bit
        int type; // PID type index

        // stream_type
        int stream_type;
        const char *sdes; // stream type short description
        const char *ldes; // stream type long description

        // es_info
        int es_info_len;
        uint8_t es_info[INFO_LEN_MAX];

        // for PTS/DTS mark
        int64_t PTS; // last PTS
        int64_t DTS; // last DTS
}
ts_track_t; // unit of track list

typedef struct _ts_pid_t
{
        // for list
        NODE *next;
        NODE *prev;
        uint32_t key; // copy PID as key

        // PID
        uint16_t PID; // 13-bit
        int type; // PID type index
        const char *sdes; // PID type short description
        const char *ldes; // PID type long description

        // only for PID with PSI/SI
        int section_idx; // to index data in section
        uint8_t section[4416]; // (184*24=4416), PSI/SI|private <= 1024|4096
        uint32_t CRC_32;
        uint32_t CRC_32_calc;

        // relative pointer
        ts_prog_t *prog; // should be prog0 if does not belong to any program
        ts_track_t *track; // should be NULL if not video or audio packet

        // for continuity_counter check
        int is_CC_sync;
        uint8_t CC; // 4-bit

        // for statistic
        uint32_t cnt; // packet received from last PCR
        uint32_t lcnt; // packet received from PCRa to PCRb

        int64_t STC; // last STC

        // file for ES dump
        FILE *fd;
}
ts_pid_t; // unit of pid list

typedef struct _ts_rslt_t
{
        // error
        ts_error_t err;

        // PSI/SI table
        ts_psi_t psi;
        int is_psi_parsed;
        int is_psi_si;
        int has_section;
        LIST table_list; // ts_table_t, PSI/SI table except PMT

        // TS information
        uint16_t transport_stream_id;
        LIST prog_list; // ts_prog_t, program list
        LIST pid_list; // ts_pid_t, pid list
        ts_prog_t *prog0; // first program in this stream

        // information about current packet
        uint64_t cnt; // count of this packet, start from 0
        uint64_t addr; // address of this packet, cnt * pkt_size
        uint8_t line[256]; // current TS packet, 188-byte or 204-byte

        uint16_t concerned_pid; // used for PSI parsing
        uint16_t pid;

        ts_pid_t *pids; // point to item in pid_list

        // PMT, PCR, VID and AUD has timestamp according to its PCR
        // other PID has timestamp according to the PCR in the 1st program
        int64_t STC; // System Time Clock of this packet
        int64_t STC_base;
        int16_t STC_ext;

        // for bit-rate statistic
        int64_t aim_interval; // appointed interval
        int64_t interval; // time passed from last rate calc
        uint64_t sys_cnt; // system packet count
        uint64_t psi_cnt; // psi-si packet count
        uint64_t nul_cnt; // empty packet count

        // for rate calc
        int has_rate; // new bit-rate ready
        int64_t last_interval; // interval from PCRa to PCRb
        uint64_t last_sys_cnt; // system packet count from PCRa to PCRb
        uint64_t last_psi_cnt; // psi-si packet count from PCRa to PCRb
        uint64_t last_nul_cnt; // empty packet count from PCRa to PCRb

        int CC_wait;
        int CC_find;
        int CC_lost; // lost != 0 means CC wrong

        int has_PCR;
        int64_t PCR;
        int64_t PCR_base;
        int16_t PCR_ext;
        int64_t PCR_interval; // PCR packet arrive time interval
        int64_t PCR_continuity; // PCR value interval
        int64_t PCR_jitter;

        int has_PTS;
        int64_t PTS;
        int64_t PTS_interval;
        int64_t PTS_minus_STC;

        int has_DTS;
        int64_t DTS;
        int64_t DTS_interval;
        int64_t DTS_minus_STC;

        // PES data info in this TS
        uint16_t PES_len;
        uint8_t *PES_buf;

        // ES data info in this TS
        uint16_t ES_len;
        uint8_t *ES_buf;
}
ts_rslt_t; // parse result

//============================================================================
// public function declaration
//============================================================================
int tsCreate(ts_rslt_t **rslt);
int tsDelete(int id);
int tsParseTS(int id, void *pkt, int size);
int tsParseOther(int id);

#ifdef __cplusplus
}
#endif

#endif // _TS_H

/*****************************************************************************
 * End
 ****************************************************************************/
