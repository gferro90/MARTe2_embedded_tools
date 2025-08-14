/*
 * HandleDatabase.c
 *
 *  Created on: 06 giu 2018
 *      Author: pc
 */

#include "HandleDatabase.h"
#include <stdio.h>
#include <string.h>
#define QUOTE(x) QUOTE_1(x)
#define QUOTE_1(x) #x
#include QUOTE(_HAL_H)


struct HandleNode handleDatabase[64];
unsigned int numberOfHandles = 0u;
static volatile unsigned long long hrtCounter = 0ull;

void* GetHandle(const char *handleName) {
    unsigned int i = 0u;
    for (i = 0u; i < numberOfHandles; i++) {
        if (strcmp(handleDatabase[i].handleName, handleName) == 0) {
            return (void*) handleDatabase[i].handle;
        }
    }
    return NULL;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == __TIMER__NAME__) {
        hrtCounter++;
    }
}

unsigned long long GetCounter(){
    return hrtCounter;
}



