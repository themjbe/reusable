// Console IO is a wrapper between the actual in and output and the console code
// In an embedded system, this might interface to a UART driver.

#include "consoleIo.h"
#include <stdio.h>
#include "usbd_cdc_if.h"

char usb_buffer[64] = {EOF};
uint8_t usb_ready = 0;

eConsoleError ConsoleIoInit(void)
{
    return CONSOLE_SUCCESS;
}

eConsoleError ConsoleIoReceive(uint8_t *buffer, const uint32_t bufferLength, uint32_t *readLength)
{
    uint32_t i = 0;
    char ch;
    
    if ( usb_ready == 0 )
    {
        *readLength = 0;
        return CONSOLE_SUCCESS;
    }

    ch = usb_buffer[i];
    while ( ( (char) EOF != ch ) && ( i < bufferLength ) )
    {
        buffer[i] = (uint8_t) usb_buffer[i];
        i++;
        ch = usb_buffer[i];
    }
    *readLength = i;
    usb_ready = 0;

    return CONSOLE_SUCCESS;
}

eConsoleError ConsoleIoSendString(const char *buffer)
{
    static char w_buffer[512];
    static uint16_t w_len;
    uint16_t len = 0;
    while ( buffer[len] != '\0' )
    {
        w_buffer[w_len] = buffer[len];
        len++;
        w_len++;
    }
    if ( CDC_Transmit_FS((uint8_t*)w_buffer, w_len) == USBD_OK )
    {
        w_len = 0;
    }
    return CONSOLE_SUCCESS;
}

