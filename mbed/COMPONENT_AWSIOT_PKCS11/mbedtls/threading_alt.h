/*
 * AWS IoT Over-the-air Update v2.0.0 (Release Candidate)
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef THREADING_ALT_H
#define THREADING_ALT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* This header file cannot include C++ stuff to be included by mbedtls without error.
 * To meet this, rtos::Mutex cannot be used for memory requirement calculation. Instead,
 * estimate this memory requirement to accommodate rtos::Mutex. This must be checked in
 * threading_alt.cpp which can include rtos::Mutex. */
#define THREADING_MUTEX_BLOCK_SIZE          64

/**
 * \brief   struct for MBEDTLS_THREADING_ALT
 */
struct mbedtls_threading_mutex
{
    uint64_t        mutex_block[(THREADING_MUTEX_BLOCK_SIZE + 7) / 8];
    void *          mutex;
};

typedef struct mbedtls_threading_mutex  mbedtls_threading_mutex_t;

/**
 * \brief   Initialize mbedtls_threading_mutex_t statically
 */
#define MUTEX_INIT  = { .mutex = NULL }

/**
 * \brief   functions for MBEDTLS_THREADING_ALT
 *
 * All these functions are expected to work or the result will be undefined.
 */
void threading_mutex_init_mbed(mbedtls_threading_mutex_t *mutex);
void threading_mutex_free_mbed(mbedtls_threading_mutex_t *mutex);
int threading_mutex_lock_mbed(mbedtls_threading_mutex_t *mutex);
int threading_mutex_unlock_mbed(mbedtls_threading_mutex_t *mutex);

#ifdef __cplusplus
}
#endif

#endif /* ifndef THREADING_ALT_H */
