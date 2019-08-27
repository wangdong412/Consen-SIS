// HTML Help Context IDs 
// 
// To create this file: 
// 1. Run batch file, makehelp.bat. 
// 2. Copy Device_Help.hm Device_Help.h 
// 3. Use regex (Lemmy/vi/sed) with :1,$s/H/#define H/g 
 
// Commands (ID_* and IDM_*) 
#define HIDM_FILE                               0x107D1
#define HIDM_FILE_NEW                           0x107D2
#define HIDM_FILE_OPEN                          0x107D3
#define HIDM_FILE_SAVE                          0x107D4
#define HIDM_FILE_SAVE_AS                       0x107D5
#define HIDM_FILE_PRINT                         0x107D6
#define HIDM_FILE_PRINT_PREVIEW                 0x107D7
#define HIDM_FILE_PRINT_SETUP                   0x107D8
#define HIDM_FILE_MRU                           0x107D9
#define HIDM_FILE_EXIT                          0x107DA
#define HIDM_EDIT                               0x107E5
#define HIDM_EDIT_UNDO                          0x107E6
#define HIDM_EDIT_REDO                          0x107E7
#define HIDM_EDIT_CUT                           0x107E8
#define HIDM_EDIT_COPY                          0x107E9
#define HIDM_EDIT_PASTE                         0x107EA
#define HIDM_EDIT_FIND                          0x107EB
#define HIDM_DEVICE                             0x107EF
#define HIDM_DEVICE_PASSWORD                    0x107F0
#define HIDM_DEVICE_LOGOUT                      0x107F1
#define HIDM_DEVICE_CONNECT                     0x107F2
#define HIDM_DEVICE_RETRIEVE                    0x107F3
#define HIDM_DEVICE_SEND                        0x107F4
#define HIDM_DEVICE_COMPARE                     0x107F5
#define HIDM_DEVICE_SIMULATE                    0x107F6
#define HIDM_DEVICE_COMM_SETUP                  0x107F7
#define HIDM_DEVICE_SELECT_APP_TYPE             0x107F8
#define HIDM_MONITOR                            0x10803
#define HIDM_MONITOR_START                      0x10804
#define HIDM_MONITOR_STOP                       0x10805
#define HIDM_MONITOR_RECORD                     0x10806
#define HIDM_MONITOR_SETUP                      0x10807
#define HIDM_VIEW                               0x1080D
#define HIDM_VIEW_TOOLBAR                       0x1080E
#define HIDM_VIEW_STATUSBAR                     0x1080F
#define HIDM_VIEW_WORKSPACE                     0x10810
#define HIDM_VIEW_MESSAGES                      0x10811
#define HIDM_VIEW_CONFIGURATION                 0x10812
#define HIDM_VIEW_STARTUP                       0x10813
#define HIDM_VIEW_MONITOR                       0x10814
#define HIDM_VIEW_TREND                         0x10815
#define HIDM_VIEW_TUNING                        0x10816
#define HIDM_VIEW_ALIASES                       0x10817
#define HIDM_VIEW_FONT                          0x10818
#define HIDM_VIEW_REFRESH                       0x10819
#define HIDM_TREND                              0x10821
#define HIDM_TREND_X_AXIS                       0x10822
#define HIDM_TREND_Y1_AXIS                      0x10823
#define HIDM_TREND_Y2_AXIS                      0x10824
#define HIDM_TREND_PROPERTIES                   0x10825
#define HIDM_HELP                               0x1082B
#define HIDM_HELP_HELP                          0x1082C
#define HIDM_HELP_ABOUT                         0x1082D
 
// Prompts (IDP_*) 
#define HIDP_APPTYPE_UNKNOWN                    0x3F007
#define HIDP_LIBFILE_NOTFOUND                   0x3F009
#define HIDP_LIBFILE_INVALID                    0x3F00A
#define HIDP_SIM310SV_EXE_ERROR                 0x3F01D

 
// Resources (IDR_*) 
#define HIDR_MAINFORM                           0x203E9
#define HIDR_ABOUTFORM                          0x203EA
#define HIDR_COMMSETTINGS                       0x203EB
#define HIDR_FACEPLATE                          0x203EC
#define HIDR_PASSWORD                           0x203ED
#define HIDR_MONITOR                            0x203EE
#define HIDR_PROGRESS                           0x203EF
#define HIDR_SETTINGTREE                        0x203F0
#define HIDR_SETTINGGRID                        0x203F1
#define HIDR_STARTUP                            0x203F2
#define HIDR_SPLASH                             0x203F3
#define HIDR_TREND                              0x203F4
#define HIDR_TAGSGRID                           0x203F5
#define HIDR_TUNING                             0x203F6
#define HIDR_ALARM                              0X204F7
#define HIDR_MENUANIMATION                      0x200AB
#define HIDR_FILEID                             0x200D3
#define HIDR_OPENFILTER                         0x200D4
#define HIDR_SAVEASFILTER                       0x200D5
#define HIDR_SAVEFAILED                         0x200D6
#define HIDR_SAVEREADONLY                       0x200D7

 
// Dialogs (IDD_*) 
 
// Frame Controls (IDW_*) 
 
// Configuration Items Controls (IDX_*) 
#define HIDX_INOUT                              0x40BB9
#define HIDX_INOUT_AI                           0x40BBA
#define HIDX_INOUT_AO                           0x40BBB
#define HIDX_INOUT_DI                           0x40BBC
#define HIDX_INOUT_DO                           0x40BBD
#define HIDX_CONTROL                            0x40BC2
#define HIDX_TUNING                             0x40BE0
#define HIDX_CONTROL_PERIOD                     0X40C05     // Added by wangdong begin
#define HIDX_CONTROL_PV                         0X40C06
#define HIDX_CONTROL_SP                         0X40C07
#define HIDX_CONTROL_ERROR                      0X40C08
#define HIDX_PID_CONTROLLER_OUTPUT              0X40C09
#define HIDX_DRIVE                              0X40C10     // Added by wangdong end
/*  TS320
#define HIDX_INOUT_SPD                          0x40BBE
#define HIDX_COMMUNICATION                      0x40BBF
#define HIDX_CONTROL                            0x40BC2
#define HIDX_CONTROL_TURBINE                    0x40BC3
#define HIDX_CONTROL_TURBINE_GENERAL            0x40BC4
#define HIDX_CONTROL_TURBINE_STARTUP            0x40BC5
#define HIDX_CONTROL_TURBINE_BREAK              0x40BC6
#define HIDX_CONTROL_TURBINE_CRITICAL           0x40BC7
#define HIDX_CONTROL_TURBINE_GOVERNOR           0x40BC8
#define HIDX_CONTROL_TURBINE_NOZZLES            0x40BC9
#define HIDX_CONTROL_SPEED                      0x40BCA
#define HIDX_CONTROL_SPEED_GENERAL              0x40BCB
#define HIDX_CONTROL_SPEED_SWITCHES             0x40BCC
#define HIDX_CONTROL_EXTRACTION                 0x40BCD
#define HIDX_CONTROL_STANDALONE                 0x40BCE
#define HIDX_DROOP                              0x40BD6
#define HIDX_DROOP_SPEED                        0x40BD7
#define HIDX_DROOP_EXTRACTION                   0x40BD8
#define HIDX_DROOP_INLET_PRESSURE               0x40BD9
#define HIDX_DROOP_PROCESS                      0x40BDA
#define HIDX_DROOP_BACK_PRESSURE                0x40BDB
#define HIDX_TUNING                             0x40BE0
#define HIDX_TUNING_START                       0x40BE1
#define HIDX_TUNING_RUN                         0x40BE2
#define HIDX_TUNING_DROOP                       0x40BE3
#define HIDX_TUNING_EXTRACTION                  0x40BE4
#define HIDX_TUNING_INLET_PRESSURE              0x40BE5
#define HIDX_TUNING_STANDALONE                  0x40BE6
#define HIDX_TUNING_PROCESS                     0x40BE7
#define HIDX_TUNING_V1_LIMIT                    0x40BE8
#define HIDX_TUNING_ISOCHRONOUS                 0x40BEA
#define HIDX_TUNING_MEGAWATT                    0x40BEB
#define HIDX_TUNING_LOAD_LIMIT                  0x40BEC
#define HIDX_TUNING_BACK_PRESSURE               0x40BED
#define HIDX_MECHANICAL_DRIVE                   0x40BF4
#define HIDX_MECH_PROCESS                       0x40BF5
#define HIDX_MECH_V1_LIMIT                      0x40BF6
#define HIDX_MECH_INLET_PRESSURE                0x40BF7
#define HIDX_GENERATOR                          0x40BFE
#define HIDX_GEN_MEGAWATT                       0x40BFF
#define HIDX_GEN_MEGAWATT_GENERAL               0x40C00
#define HIDX_GEN_MEGAWATT_LOAD_LIMIT            0x40C01
#define HIDX_GEN_SYNCHRONIZATION                0x40C02
#define HIDX_GEN_INLET_PRESSURE                 0x40C03
#define HIDX_GEN_BACK_PRESSURE                  0x40C04
*/

