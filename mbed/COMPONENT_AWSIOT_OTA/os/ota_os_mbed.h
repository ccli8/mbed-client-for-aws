/*
 * Copyright (c) 2020, Nuvoton Technology Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file ota_os_mbed.h
 * @brief Function declarations for the example OTA OS Functional interface for
 * Mbed.
 */

#ifndef _OTA_OS_MBED_H_
#define _OTA_OS_MBED_H_

/* Mbed includes. */
#include "mbed.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************/
/******* DO NOT CHANGE the following order ********/
/**************************************************/

/* Logging related header files are required to be included in the following order:
 * 1. Include the header file "logging_levels.h".
 * 2. Define LIBRARY_LOG_NAME and  LIBRARY_LOG_LEVEL.
 * 3. Include the header file "logging_stack.h".
 */

/* Include header that defines log levels. */
#include "logging_levels.h"

/* Logging configuration for the transport interface implementation which uses
 * TLSSocket. */
#ifndef LIBRARY_LOG_NAME
    #define LIBRARY_LOG_NAME     "Mbed_OTA_OS"
#endif
#ifndef LIBRARY_LOG_LEVEL
    #define LIBRARY_LOG_LEVEL    LOG_ERROR
#endif

#include "logging_stack.h"

/************ End of logging configuration ****************/

/* OTA library interface include.
 *
 * struct OtaTimerInterface has field name 'delete' which is C++ reserved word. To include
 * this header file in C++ source file, use the C++ bugfix version instead.
 */
/* OtaTimerInterface_t (ota_os_interface.h) has member 'delete' which is C++ reserved
 * keyword. Try get around it. */
#define delete  delete_
#include "ota_os_interface.h"
#undef delete

/**
 * @brief Initialize the OTA events.
 *
 * This function initializes the OTA events mechanism for Mbed platforms.
 *
 * @param[pEventCtx]     Pointer to the OTA event context.
 *
 * @return               OtaOsStatus_t, OtaOsSuccess if success , other error code on failure.
 */
OtaOsStatus_t Mbed_OtaInitEvent( OtaEventContext_t * pEventCtx );

/**
 * @brief Sends an OTA event.
 *
 * This function sends an event to OTA library event handler for Mbed platforms.
 *
 * @param[pEventCtx]     Pointer to the OTA event context.
 *
 * @param[pEventMsg]     Event to be sent to the OTA handler.
 *
 * @param[timeout]       The maximum amount of time (msec) the task should block.
 *
 * @return               OtaOsStatus_t, OtaOsSuccess if success , other error code on failure.
 */
OtaOsStatus_t Mbed_OtaSendEvent( OtaEventContext_t * pEventCtx,
                                  const void * pEventMsg,
                                  unsigned int timeout );

/**
 * @brief Receive an OTA event.
 *
 * This function receives next event from the pending OTA events for Mbed platforms.
 *
 * @param[pEventCtx]     Pointer to the OTA event context.
 *
 * @param[pEventMsg]     Pointer to store message.
 *
 * @param[timeout]       The maximum amount of time the task should block.
 *
 * @return               OtaOsStatus_t, OtaOsSuccess if success , other error code on failure.
 */
OtaOsStatus_t Mbed_OtaReceiveEvent( OtaEventContext_t * pEventCtx,
                                     void * pEventMsg,
                                     uint32_t timeout );

/**
 * @brief Deinitialize the OTA Events mechanism.
 *
 * This function deinitialize the OTA events mechanism and frees any resources
 * used on Mbed platforms.
 *
 * @param[pEventCtx]     Pointer to the OTA event context.
 *
 * @return               OtaOsStatus_t, OtaOsSuccess if success , other error code on failure.
 */
OtaOsStatus_t Mbed_OtaDeinitEvent( OtaEventContext_t * pEventCtx );


/**
 * @brief Start timer.
 *
 * This function starts the timer or resets it if it is already started for Mbed platforms.
 *
 * @param[otaTimerId]       Timer ID of type otaTimerId_t.
 *
 * @param[pTimerName]       Timer name.
 *
 * @param[timeout]          Timeout for the timer.
 *
 * @param[callback]         Callback to be called when timer expires.
 *
 * @return                  OtaOsStatus_t, OtaOsSuccess if success , other error code on failure.
 */
OtaOsStatus_t Mbed_OtaStartTimer( OtaTimerId_t otaTimerId,
                                   const char * const pTimerName,
                                   const uint32_t timeout,
                                   OtaTimerCallback_t callback );

/**
 * @brief Stop timer.
 *
 * This function stops the timer fro Mbed platforms.
 *
 * @param[otaTimerId]     Timer ID of type otaTimerId_t.
 *
 * @return                OtaOsStatus_t, OtaOsSuccess if success , other error code on failure.
 */
OtaOsStatus_t Mbed_OtaStopTimer( OtaTimerId_t otaTimerId );

/**
 * @brief Delete a timer.
 *
 * This function deletes a timer for Mbed platforms.
 *
 * @param[otaTimerId]       Timer ID of type otaTimerId_t.
 *
 * @return                  OtaOsStatus_t, OtaOsSuccess if success , other error code on failure.
 */
OtaOsStatus_t Mbed_OtaDeleteTimer( OtaTimerId_t otaTimerId );

/**
 * @brief Allocate memory.
 *
 * This function allocates the requested memory and returns a pointer to it using standard
 * C library malloc.
 *
 * @param[size]        This is the size of the memory block, in bytes..
 *
 * @return             This function returns a pointer to the allocated memory, or NULL if
 *                     the request fails.
 */

void * STDC_Malloc( size_t size );

/**
 * @brief Free memory.
 *
 * This function deallocates the memory previously allocated by a call to allocation
 * function of type OtaMalloc_t and uses standard C library free.
 *
 * @param[ptr]         This is the pointer to a memory block previously allocated with function
 *                     of type OtaMalloc_t. If a null pointer is passed as argument, no action occurs.
 *
 * @return             None.
 */

void STDC_Free( void * ptr );

#ifdef __cplusplus
}
#endif

#endif /* ifndef _OTA_OS_MBED_H_ */
