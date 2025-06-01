#ifndef _menu_h_
#define _menu_h_

#include "main.h"

// Structure representing a menu state
typedef struct
{
    unsigned char state;   // State identifier (from menu_state_id_t)
    int16_t step;          // Step number or index in the menu
    uint16_t flag;         // Associated flag (from menu_flag_t)
    char* text;            // Display text for the menu item
} menu_state_t;

// Enumeration for state machine states
typedef enum {
    ST_NONE = 0,   // No state
    ST_INFO,       // Information state
    ST_SENSOR,     // Sensor state
    ST_CONV,       // Conversion state
    ST_ACT,        // Actuator state
    ST_MPU         // MPU mode state
} menu_state_id_t;

// Enumeration for menu flags (bitmask)
typedef enum {
    FLAG_NONE   = 0x0000, // No flag
    FLAG_INFO   = 0x0001, // Info flag
    FLAG_SENSOR = 0x0002, // Sensor flag
    FLAG_CONV   = 0x0004, // Conversion flag
    FLAG_ACT    = 0x0008, // Actuator flag
    FLAG_MPU    = 0x0010  // MPU flag
} menu_flag_t;

// Enumeration for state machine on/off flags
typedef enum {
    FLAG_SM_ON   = 0x0000, // State machine ON
    FLAG_SM_OFF  = 0x0001  // State machine OFF
} sm_flag_t;

// Array of menu states with their properties
const menu_state_t menu_state[] = {
    // STATE        STEP    FLAG            TEXT
    {ST_INFO,      0,      FLAG_INFO,      "Infos"},
    {ST_SENSOR,    1,      FLAG_SENSOR,    "Sensor ADXL345"},
    {ST_CONV,      2,      FLAG_CONV,      "Conversion"},
    {ST_ACT,       3,      FLAG_ACT,       "Actuator"},
    {ST_MPU,       4,      FLAG_MPU,       "MPU Mode"},
    {0,            0,      0,              NULL}, // End marker
};

#endif
