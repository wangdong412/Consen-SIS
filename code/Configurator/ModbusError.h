// ==========================================================================
// Error codes
// ==========================================================================

// Modbus Server Error Codes
#define EMB_BASE                        0xC700
#define EMB_FUNCTION                    (EMB_BASE + 1)
#define EMB_DATAADDRESS                 (EMB_BASE + 2)
#define EMB_DATAVALUE                   (EMB_BASE + 3)
#define EMB_FAILURE                     (EMB_BASE + 4)
#define EMB_ACKNOWLEDGE                 (EMB_BASE + 5)
#define EMB_BUSYREJECTED                (EMB_BASE + 6)
#define EMB_NEGACKNOWLEDGE              (EMB_BASE + 7)
#define EMB_MEMORYPARITY                (EMB_BASE + 8)
#define EMB_CRC                         (EMB_BASE + 9)
#define EMB_TIMEOUT                     (EMB_BASE + 10)
#define EMB_UNK_EXCEPTION               (EMB_BASE + 11)

// Modbus Client Error Codes
#define EMC_UNKNOWN                     (EMB_BASE + 21)
#define EMC_MESSAGE                     (EMB_BASE + 22)
#define EMC_CRC                         (EMB_BASE + 23)
#define EMC_TIMEOUT                     (EMB_BASE + 24)
#define EMC_DATAVALUE                   (EMB_BASE + 25)
#define EMC_BADALIAS                    (EMB_BASE + 26)
#define EMC_BUFFER                      (EMB_BASE + 27)
#define EMC_WRITE                       (EMB_BASE + 28)

// Modbus Device Error Codes
#define EMD_ALIAS_MISSING               (EMB_BASE + 30)
#define EMD_PRODUCT_NUMBER              (EMB_BASE + 31)
#define EMD_PRODUCT_REVISION            (EMB_BASE + 32)

// Modbus Tcp  Error Codes
#define EMT_TRANSACTION_ERROR           (EMB_BASE + 41)
#define EMT_PROTOCAL_ERROR              (EMB_BASE + 42)
#define EMT_LENGTH_ERROR                (EMB_BASE + 43)


// --------------------------------------------------------------------------
// Modbus HRESULTs
// --------------------------------------------------------------------------

#define HR_MODBUS(code)                 MAKE_HRESULT(SEVERITY_ERROR, FACILITY_MODBUS, code)
#define FACILITY_MODBUS                 202

// Modbus HRESULT Codes
#define E_MODBUS_BASE                   HR_MODBUS(EMB_BASE)
#define E_MODBUS_FUNCTION               HR_MODBUS(EMB_FUNCTION)
#define E_MODBUS_DATAADDRESS            HR_MODBUS(EMB_DATAADDRESS)
#define E_MODBUS_DATAVALUE              HR_MODBUS(EMB_DATAVALUE)
#define E_MODBUS_FAILURE                HR_MODBUS(EMB_FAILURE)
#define E_MODBUS_ACKNOWLEDGE            HR_MODBUS(EMB_ACKNOWLEDGE)
#define E_MODBUS_BUSYREJECTED           HR_MODBUS(EMB_BUSYREJECTED)
#define E_MODBUS_NEGACKNOWLEDGE         HR_MODBUS(EMB_NEGACKNOWLEDGE)
#define E_MODBUS_MEMORYPARITY           HR_MODBUS(EMB_MEMORYPARITY)
#define E_MODBUS_CRC                    HR_MODBUS(EMB_CRC)
#define E_MODBUS_TIMEOUT                HR_MODBUS(EMB_TIMEOUT)
#define E_MODBUS_UNK_EXCEPTION          HR_MODBUS(EMB_UNK_EXCEPTION)

#define E_CLIENT_UNKNOWN                HR_MODBUS(EMC_UNKNOWN)
#define E_CLIENT_MESSAGE                HR_MODBUS(EMC_MESSAGE)
#define E_CLIENT_CRC                    HR_MODBUS(EMC_CRC)
#define E_CLIENT_TIMEOUT                HR_MODBUS(EMC_TIMEOUT)
#define E_CLIENT_DATAVALUE              HR_MODBUS(EMC_DATAVALUE)
#define E_CLIENT_BADALIAS               HR_MODBUS(EMC_BADALIAS)
#define E_CLIENT_BUFFER                 HR_MODBUS(EMC_BUFFER)
#define E_CLIENT_WRITE                  HR_MODBUS(EMC_WRITE)

#define E_DEVICE_ALIAS_MISSING          HR_MODBUS(EMD_ALIAS_MISSING)
#define E_DEVICE_PRODUCT_NUMBER         HR_MODBUS(EMD_PRODUCT_NUMBER)
#define E_DEVICE_PRODUCT_REVISION       HR_MODBUS(EMD_PRODUCT_REVISION)

#define E_EMT_TRANSACTION_ERROR         HR_MODBUS(EMT_TRANSACTION_ERROR)
#define E_EMT_PROTOCAL_ERROR            HR_MODBUS(EMT_PROTOCAL_ERROR)
#define E_EMT_LENGTH_ERROR              HR_MODBUS(EMT_LENGTH_ERROR)