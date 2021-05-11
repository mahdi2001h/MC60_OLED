#define __CUSTOMER_CODE__
#ifdef __CUSTOMER_CODE__

#include "ql_type.h"
#include "ql_stdlib.h"
#include "ql_trace.h"
#include "ql_gpio.h"
#include "ql_uart.h"
#include "ql_iic.h"
#include "ql_error.h"
#include "oled.h"

///for DEBUG///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SERIAL_RX_BUFFER_LEN 2048
#define DEBUG_PORT UART_PORT1
#define DBG_BUF_LEN 512
static char DBG_BUFFER[DBG_BUF_LEN];
#define APP_DEBUG(FORMAT, ...)                                                                                   \
    {                                                                                                            \
        Ql_memset(DBG_BUFFER, 0, DBG_BUF_LEN);                                                                   \
        Ql_sprintf(DBG_BUFFER, FORMAT, ##__VA_ARGS__);                                                           \
        Ql_UART_Write((Enum_SerialPort)(DEBUG_PORT), (u8 *)(DBG_BUFFER), Ql_strlen((const char *)(DBG_BUFFER))); \
    }
char m_Read_Buffer[SERIAL_RX_BUFFER_LEN];
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void CallBack_UART_Hdlr(Enum_SerialPort port, Enum_UARTEventType msg, bool level, void *customizedPara);

const u8 ant_1[] = {
    0b11111111,
    0b01011010,
    0b00111100,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
};

const u8 ant_2_full[] = {
    0b00000001,
    0b00000001,
    0b00000101,
    0b00000101,
    0b00010101,
    0b00010101,
    0b01010101,
    0b01010101,
};

const u8 ant_low[] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00010000,
    0b00010000,
    0b01010000,
    0b01010000,
};

const u8 play[] = {
    0b00000000,
    0b11000000,
    0b11110000,
    0b11111100,
    0b11111111,
    0b11111100,
    0b11110000,
    0b11000000,
};
const u8 pause[] = {
    0b11100111,
    0b11100111,
    0b11100111,
    0b11100111,
    0b11100111,
    0b11100111,
    0b11100111,
    0b11100111,
};
const u8 stop[] = {
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
};

const u8 bat_1[] = {
    0b11111111,
    0b10000000,
    0b10111111,
    0b10111111,
    0b10111111,
    0b10111111,
    0b10000000,
    0b11111111,
};

const u8 bat_2[] = {
    0b11111100,
    0b00000110,
    0b00000111,
    0b00000111,
    0b00000111,
    0b00000111,
    0b00000110,
    0b11111100,
};

void proc_main_task(s32 taskId)
{
    s32 ret;
    ST_MSG msg;

    // Register & open UART port
    Ql_UART_Register(UART_PORT1, CallBack_UART_Hdlr, NULL);
    Ql_UART_Open(UART_PORT1, 115200, FC_NONE);

        APP_DEBUG("START PROGRAM (SISOOG.COM)\r\n");

    ret = Ql_IIC_Init(0, PINNAME_RI, PINNAME_DCD, TRUE);

    Ql_Sleep(100);
    ret = Ql_IIC_Config(0, TRUE, I2C_ADDRESS, 1500); 

    oledInit();
    oledClear();
    oledFill();
    Ql_Sleep(500);
    oledClear();
    Ql_Sleep(500);
    setFont(Adafruit5x7);

    set2X();
    setCursor(28, 0);
    oledPrint("Sisoog");
    set1X();
    setCursor(0, 3);
    oledPrint("Mahdi.h");

    oledDraw(0, 0, bat_1);
    oledDraw(8, 0, bat_2);

    oledDraw(110, 0, ant_1);
    oledDraw(119, 0, ant_2_full);

    oledDraw(50, 3, play);
    oledDraw(70, 3, pause);
    oledDraw(90, 3, stop);

    while (1)
    {
        Ql_OS_GetMessage(&msg);
        switch (msg.message)
        {
        case 0:
            break;
        default:
            break;
        }
    }
}

static void CallBack_UART_Hdlr(Enum_SerialPort port, Enum_UARTEventType msg, bool level, void *customizedPara)
{
    u32 rdLen = 0;
    s32 ret;

    switch (msg)
    {
    case EVENT_UART_READY_TO_READ:
    {
        Ql_memset(m_Read_Buffer, 0x0, sizeof(m_Read_Buffer));
        rdLen = Ql_UART_Read(port, m_Read_Buffer, sizeof(m_Read_Buffer));

        if (Ql_strstr(m_Read_Buffer, "ok"))
        {
            APP_DEBUG("ok\r\n");
        }
        break;
    }
    }
}
#endif // __CUSTOMER_CODE__