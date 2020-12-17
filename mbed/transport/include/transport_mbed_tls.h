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

#ifndef TRANSPORT_MBED_TLS_H
#define TRANSPORT_MBED_TLS_H

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
    #define LIBRARY_LOG_NAME     "Mbed_TLS_Sockets"
#endif
#ifndef LIBRARY_LOG_LEVEL
    #define LIBRARY_LOG_LEVEL    LOG_ERROR
#endif

#include "logging_stack.h"

/************ End of logging configuration ****************/

/* Transport includes. */
#include "transport_mbed_base.h"

/**
 * @brief Derived NetworkStruct for TLS
 */
struct TlsNetworkContext : public NetworkContext
{
    TlsNetworkContext() :
        socket(NULL)
    {
    }

    uint64_t                socketBlock[(sizeof(TLSSocket) + 7) / 8];
    TLSSocket *             socket;
};
typedef struct TlsNetworkContext TlsNetworkContext_t;

/* The format for network credentials on this system. */
typedef struct {
    const char *rootCA;
    const char *clientCrt;
    const char *clientKey;
} CredentialInfo_t;

/**
 * @brief Sets up a TLS session on top of a TCP connection.
 *
 * @param[out] pNetworkContext The output parameter to return the created network context.
 * @param[in] pServerInfo Server connection info.
 * @param[in] pCredentialInfo Credentials for the TLS connection.
 * @param[in] sendTimeoutMs Timeout for transport send.
 * @param[in] recvTimeoutMs Timeout for transport recv.
 *
 * @note A timeout of 0 means infinite timeout.
 *
 * @return 0 on success; -1 on failure
 */
int32_t Mbed_Tls_Connect( NetworkContext_t * pNetworkContext,
                          const ServerInfo_t * pServerInfo,
                          const CredentialInfo_t * pCredentialInfo,
                          uint32_t sendTimeoutMs,
                          uint32_t recvTimeoutMs );

/**
 * @brief Closes a TLS session on top of a TCP connection.
 *
 * @param[out] pNetworkContext The output parameter to end the TLS session and
 * clean the created network context.
 *
 * @return 0 on success; -1 on failure
 */
int32_t Mbed_Tls_Disconnect( NetworkContext_t * pNetworkContext );

/**
 * @brief Receives data over an established TLS session.
 *
 * This can be used as #TransportInterface.recv function for receiving data
 * from the network.
 *
 * @param[in] pNetworkContext The network context created using Mbed_Tls_Connect API.
 * @param[out] pBuffer Buffer to receive network data into.
 * @param[in] bytesToRecv Number of bytes requested from the network.
 *
 * @return Number of bytes received if successful; negative value to indicate failure.
 * A return value of zero represents that the receive operation can be retried.
 */
int32_t Mbed_Tls_Recv( NetworkContext_t * pNetworkContext,
                      void * pBuffer,
                      size_t bytesToRecv );

/**
 * @brief Sends data over an established TLS session.
 *
 * This can be used as the #TransportInterface.send function to send data
 * over the network.
 *
 * @param[in] pNetworkContext The network context created using Mbed_Tls_Connect API.
 * @param[in] pBuffer Buffer containing the bytes to send over the network stack.
 * @param[in] bytesToSend Number of bytes to send over the network.
 *
 * @return Number of bytes sent if successful; negative value on error.
 *
 * @note This function does not return zero value because it cannot be retried
 * on send operation failure.
 */
int32_t Mbed_Tls_Send( NetworkContext_t * pNetworkContext,
                      const void * pBuffer,
                      size_t bytesToSend );

#ifdef __cplusplus
}
#endif

#endif /* ifndef TRANSPORT_MBED_TLS_H */
