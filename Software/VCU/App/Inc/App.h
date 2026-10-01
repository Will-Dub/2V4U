#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void App_Init(volatile uint16_t* throttleDmaPtr);
void App_Run(void);

#ifdef __cplusplus
}
#endif