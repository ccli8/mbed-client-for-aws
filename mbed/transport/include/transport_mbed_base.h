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

#ifndef TRANSPORT_MBED_BASE_H
#define TRANSPORT_MBED_BASE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Transport includes. */
#include "transport_interface.h"

/**
 * @brief NetworkContext as base.
 */
struct NetworkContext
{
    NetworkContext() :
        sendTimeoutMs(0),
        recvTimeoutMs(0)
    {
    }

    uint32_t    sendTimeoutMs;
    uint32_t    recvTimeoutMs;
};

/**
 * @brief The format for remote server host and port on this system.
 */
typedef struct {
    const char *hostname;
    uint16_t port;
} ServerInfo_t;

#ifdef __cplusplus
}
#endif

#endif /* ifndef IOT_PLATFORM_TYPES_TEMPLATE_H_ */
