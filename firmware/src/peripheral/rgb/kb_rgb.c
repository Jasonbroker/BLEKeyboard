#include "kb_rgb.h"
#include "i2c.h"

static nrfx_twi_t* twi_channel;

void rgb3741_twi_init()
{
    twi_channel = shared_i2c_init(3, 4);
    if (twi_channel == NULL)
        APP_ERROR_HANDLER(1);
}