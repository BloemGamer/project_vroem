#pragma once
#include <stdint.h>


class Blue_Tooth
{
private:
    char bluetooth_string[100] = { 0 };
public:
    char bluetooth_read_char(void);
    const char* bluetooth_read_string(void);
    void bluetooth_send_int64(int64_t value); 
};
