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

#ifndef TRANSPORT_MBED_TCP_H
#define TRANSPORT_MBED_TCP_H

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
 * Sockets. */
#ifndef LIBRARY_LOG_NAME
    #define LIBRARY_LOG_NAME     "Mbed_TCP_Sockets"
#endif
#ifndef LIBRARY_LOG_LEVEL
    #define LIBRARY_LOG_LEVEL    LOG_ERROR
#endif

#include "logging_stack.h"

/************ End of logging configuration ****************/

/**
 * @brief Transport includes.
 */
#include "transport_mbed_base.h"

/**
 * @brief Derived NetworkStruct for TCP
 */
struct TcpNetworkContext : public NetworkContext
{
    TcpNetworkContext() :
        socket(NULL)
    {
    }

    uint64_t                socketBlock[(sizeof(TCPSocket) + 7) / 8];
    TCPSocket *             socket;
}; 
typedef struct TcpNetworkContext TcpNetworkContext_t;

/**
 * @brief Establish TCP connection to server.
 *
 * @param[out] pNetworkContext The output parameter to return the created network context.
 * @param[in] pServerInfo Server connection info.
 * @param[in] sendTimeout Timeout for socket send.
 * @param[in] recvTimeout Timeout for socket recv.
 *
 * @note A timeout of 0 means infinite timeout.
 *
 * @return #SOCKETS_SUCCESS if successful;
 * #SOCKETS_INVALID_PARAMETER, #SOCKETS_DNS_FAILURE, #SOCKETS_CONNECT_FAILURE on error.
 */
int32_t Mbed_Tcp_Connect( NetworkContext_t * pNetworkContext,
                          const ServerInfo_t * pServerInfo,
                          uint32_t sendTimeoutMs,
                          uint32_t recvTimeoutMs );

/**
 * @brief Close TCP connection to server.
 *
 * @param[in] pNetworkContext The network context to close the connection.
 *
 * @return #SOCKETS_SUCCESS if successful; #SOCKETS_INVALID_PARAMETER on error.
 */
int32_t Mbed_Tcp_Disconnect( NetworkContext_t * pNetworkContext );

/**
 * @brief Receives data over an established TCP connection.
 *
 * This can be used as #TransportInterface.recv function to receive data over
 * the network.
 *
 * @param[in] pNetworkContext The network context created using Mbed_Tcp_Connect API.
 * @param[out] pBuffer Buffer to receive network data into.
 * @param[in] bytesToRecv Number of bytes requested from the network.
 *
 * @return Number of bytes received if successful; negative value on error.
 */
int32_t Mbed_Tcp_Recv( NetworkContext_t * pNetworkContext,
                        void * pBuffer,
                        size_t bytesToRecv );

/**
 * @brief Sends data over an established TCP connection.
 *
 * This can be used as the #TransportInterface.send function to send data
 * over the network.
 *
 * @param[in] pNetworkContext The network context created using Mbed_Tcp_Connect API.
 * @param[in] pBuffer Buffer containing the bytes to send over the network.
 * @param[in] bytesToSend Number of bytes to send over the network.
 *
 * @return Number of bytes sent if successful; negative value on error.
 */
int32_t Mbed_Tcp_Send( NetworkContext_t * pNetworkContext,
                        const void * pBuffer,
                        size_t bytesToSend );

#ifdef __cplusplus
}
#endif

#endif /* ifndef TRANSPORT_MBED_TCP_H */
