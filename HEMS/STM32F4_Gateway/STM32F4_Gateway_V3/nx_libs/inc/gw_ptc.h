#ifndef GW_PTC_H
#define GW_PTC_H

#include "main.h"

/* -------------------- COMMAND -------------------- */
#define STATREQC        0x1A        // -- Status Request COMMAND      
#define STATREPC        0x2A        // -- Status Report COMMAND     
#define EVENREPC        0x3A        // -- Event Report COMMAND     
#define CONFMANC        0x4A        // -- Configuration Management COMMAND   
#define DATAREPC        0x5A        // -- Data Report COMMAND                
#define LOGMANIC        0x6A        // -- Log Manipulation COMMAND    
#define NETMAINC        0x7A        // -- Network Maintenance COMMAND    
#define RMDCTRLC        0x8A        // -- Remote Device Control COMMAND    
#define CLRLISTC        0x9A        // -- Clear List COMMAND
#define FMWUPLDC        0xFA        // -- Firmware Upload COMMAND (Reserved)   
                   
/* -------------------- ACKNOWLEDGE -------------------- */
#define STATREQA        0x1B        // -- Status Request ACKNOWLEDGE
#define STATREPA        0x2B        // -- Status Report ACKNOWLEDGE
#define EVENREPA        0x3B        // -- Event Report ACKNOWLEDGE
#define CONFMANA        0x4B        // -- Configuration Management ACKNOWLEDGE
#define DATAREPA        0x5B        // -- Data Report ACKNOWLEDGE
#define LOGMANIA        0x6B        // -- Log Manipulation ACKNOWLEDGE
#define NETMAINA        0x7B        // -- Network Maintenance ACKNOWLEDGE
#define RMDCTRLA        0x8B        // -- Remote Device Control ACKNOWLEDGE        
#define CLRLISTA        0x9B        // -- Clear List ACKNOWLEDGE
#define PROTOERROR      0xEB        // -- Protocol Error Feedback 
#define FMWUPLDA        0xFB        // -- Firmware Upload ACKNOWLEDGE (Reserved)



/* --------------- NETWORK MAINTENANCE COMMAND --------------- */
#define HEARTBEAT       (int8_t)0x11        // Heartbeat
#define JOINREQ         (int8_t)0x30        // Join Request
/* --------------- DATA REPORT COMMAND --------------- */
// - Record Type
#define RLT             (int8_t)0x00        // Real time record
#define LOG             (int8_t)0x01        // Logged record
#define SMART_PLUG      (int8_t)0x20        // Fixed for Smart_Plug
#define SMART_THERMO    (int8_t)0x21        // Fixed for Smart_Thermostat
// - Data ID
#define SEN1            (int8_t)0x01        // Sensor1
#define SEN2            (int8_t)0x02        // Sensor2
#define SEN3            (int8_t)0x03        // Sensor3
#define SEN4            (int8_t)0x04        // Sensor4
#define SEN5            (int8_t)0x05        // Sensor5
// - Data Type
#define TEMP            (int8_t)0x01        // Temperature (C)
#define PRES            (int8_t)0x02        // Pressure
#define HUMI            (int8_t)0x03        // Humidity
#define VOLT            (int8_t)0x10        // Voltage (V)
#define AMP             (int8_t)0x11        // Ampere (mA)
#define PWR_KW          (int8_t)0x12        // Power (KW)
#define KWH             (int8_t)0x13        // Energy (KWH)

/* --------------- PROTOCOL ERROR FEEDBACK ACK --------------- */
// - Error ID
#define INVALID_LEN         (int8_t)0x10    // Invalid Length
#define INVALID_FTYPE       (int8_t)0x20    // Invalid Frame type
#define INVALID_PARAM       (int8_t)0x30    // Invalid Parameter
#define PARAM_OUT_OF_RANGE  (int8_t)0x40    // Parameter Out of Range
#define END_NOT_READY       (int8_t)0x50    // End Point not Ready
#define UNKNOWN_ERR         (int8_t)0xFF    // Unknown Error


// -------------------- HEADER -------------------- //
#define PROTO_VERSION   (int8_t)0x30

// DATASET use in Data Report Cmd Packet
typedef struct {
    char dataID;
    char dataType;
    float value;
}DATASET;



void gw_proc_xbee_api(uint8_t *value, uint16_t len);
int xbee_checksum(char buf[],int len);
void gw_proc_pkt(uint8_t frame_type,uint8_t *value, uint16_t len);
int gw_proc_zig_receiv(char *buf, int len);
int gw_sendProtocolErrorFeedbackAck(char frameID, char errorID) ;
int8_t gw_sendPacket(char *recvAddr, char *data, int dataLen, int timeout);
int gw_ims_ptc(uint8_t *value,uint16_t len);
int gw_add_edv(uint8_t *sn);
int gw_select_edv(uint8_t *sn);
int gw_chk_dup_edv(uint8_t *sn);
//int gw_join_ack(uint8_t *edv_adr);
int8_t gw_join_ack(uint8_t *val , uint16_t len);
int gw_Heart_ack(uint8_t *edv_adr);
int gw_Read_DataReport(uint8_t *edv_adr);
int gw_Read_Status(uint8_t *edv_adr);

int8_t gw_dat_report_parser(uint8_t *val);
int8_t gw_status_parser(uint8_t *val);
int8_t gw_event_parser(uint8_t *val);
int8_t gw_remote_parser(uint8_t *val);

extern client_tb_t client_tb;
#endif 