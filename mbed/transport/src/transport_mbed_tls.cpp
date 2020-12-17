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

/* Standard includes. */
#include <assert.h>
#include <string.h>

/* Transport includes. */
#include "transport_mbed_tls.h"

int32_t Mbed_Tls_Connect( NetworkContext_t * pNetworkContext,
                          const ServerInfo_t * pServerInfo,
                          const CredentialInfo_t * pCredentialInfo,
                          uint32_t sendTimeoutMs,
                          uint32_t recvTimeoutMs )
{
    /* Validate parameters. */
    if (pNetworkContext == NULL) {
        LogError(("Parameter check failed: pNetworkContext is NULL."));
        return -1;
    } else if (pServerInfo == NULL || pServerInfo->hostname == NULL) {
        LogError(("Parameter check failed: pServerInfo or its hostname is NULL."));
        return -1;
    }

    /* Default network interface */
    auto net = NetworkInterface::get_default_instance();
    if (net == NULL) {
        LogError(("Default network interface is NULL."));
        return -1;
    }

    /* Cast 'pNetworkContext' to 'TlsNetworkContext_t *' for following net socket operations */
    auto *ctx_ = static_cast<TlsNetworkContext_t *>(static_cast<void *>(pNetworkContext));

    /* Save send/recv timeout */
    ctx_->sendTimeoutMs = sendTimeoutMs;
    ctx_->recvTimeoutMs = recvTimeoutMs;

    /* Check un-closed connection */
    if (ctx_->socket) {
        LogWarn(("Socket control block un-closed but reused for new connection. Close previous connection first."));
        ctx_->socket->~TLSSocket();
        ctx_->socket = NULL;
    }

    /* Construct socket */
    ctx_->socket = new (ctx_->socketBlock) TLSSocket;

    nsapi_size_or_error_t rc;
    SocketAddress sockaddr;

    /* AWS IoT requires devices to send the Server Name Indication (SNI)
     * extension to the Transport Layer Security (TLS) protocol and provide
     * the complete endpoint address in the host_name field. Details about
     *
     * This call will direct to mbedtls_ssl_set_hostname(...), which will also
     * set the SNI extension if enabled.
     *
     * For details regarding SNI, refer to:
     * https://www.globalsign.com/en/blog/what-is-server-name-indication
     */
    ctx_->socket->set_hostname(pServerInfo->hostname);

    /* Configure root CA into TLSSocket */
    if (pCredentialInfo && pCredentialInfo->rootCA) {
        rc = ctx_->socket->set_root_ca_cert(pCredentialInfo->rootCA);
        if (rc != 0) {
            LogError(("Error: configure root CA into TLSSocket: %d", rc));
            goto cleanup;
        }
    }

    /* Configure client certificate/private key into TLSSocket */
    if (pCredentialInfo && pCredentialInfo->clientCrt && pCredentialInfo->clientKey) {
        rc = ctx_->socket->set_client_cert_key(pCredentialInfo->clientCrt, pCredentialInfo->clientKey);
        if (rc != 0) {
            LogError(("Error: configure client certificate/private key into TLSSocket: %d", rc));
            goto cleanup;
        }
    }

    /* Set ALPN extension */
    if (pCredentialInfo && pCredentialInfo->alpnProtos) {
        mbedtls_ssl_conf_alpn_protocols(ctx_->socket->get_ssl_config(), pCredentialInfo->alpnProtos);
    }

    /* Set 'host':'port' into 'sockaddr': */
    /* Translate 'host' to IP address */
    rc = net->gethostbyname(pServerInfo->hostname, &sockaddr);
    if (rc != 0) {
        LogError(("Network interface gethostbyname(%s) failed with %d", pServerInfo->hostname, rc));
        goto cleanup;
    }
    /* Set port into 'sockaddr' */
    sockaddr.set_port(pServerInfo->port);

    /* Open network socket */
    rc = ctx_->socket->open(net);
    if (rc != 0) {
        LogError(("TLS socket open failed with %d", rc));
        return -1;
    }

    /* Connect to remote peer */
    rc = ctx_->socket->connect(sockaddr);
    if (rc != 0) {
        LogError(("TLS socket connect failed with %d", rc));
        return -1;
    }

cleanup:

    if (rc != 0) {
        Mbed_Tls_Disconnect(pNetworkContext);
        return -1;
    } else {
        return 0;
    }
}

int32_t Mbed_Tls_Disconnect( NetworkContext_t * pNetworkContext )
{
    /* Validate parameters. */
    if (pNetworkContext == NULL) {
        LogError(("Parameter check failed: pNetworkContext is NULL."));
        return -1;
    }

    /* Cast 'pNetworkContext' to 'TlsNetworkContext_t *' for following net socket operations */
    auto *ctx_ = static_cast<TlsNetworkContext_t *>(static_cast<void *>(pNetworkContext));

    /* Validate socket */
    if (ctx_->socket == NULL) {
        LogError(("Parameter check failed: socket is NULL."));
        return -1;
    }

    /* Destruct socket */
    ctx_->socket->~TLSSocket();
    ctx_->socket = NULL;

    return 0;
}

int32_t Mbed_Tls_Recv( NetworkContext_t * pNetworkContext,
                       void * pBuffer,
                       size_t bytesToRecv )
{
   assert( pNetworkContext != NULL );
    assert( pBuffer != NULL );
    assert( bytesToRecv > 0 );

    /* Cast 'pNetworkContext' to 'TlsNetworkContext_t *' for following net socket operations */
    auto *ctx_ = static_cast<TlsNetworkContext_t *>(static_cast<void *>(pNetworkContext));

    /* Validate socket */
    if (ctx_->socket == NULL) {
        LogError(("Parameter check failed: socket is NULL."));
        return -1;
    }

    /* Configure timeout in ms. */
    ctx_->socket->set_timeout(ctx_->recvTimeoutMs);

    /* Invoke socket recv() and then cast return code */
    nsapi_size_or_error_t rc = ctx_->socket->recv(pBuffer, bytesToRecv);
    if (rc > 0) {
        return rc;
    } else if (rc == 0) {
        /* Peer has closed the connection. Treat as an error. */
        LogError(("No more buffered receive data and peer has closed the connection."));
        return -1;
    } else if (rc == NSAPI_ERROR_WOULD_BLOCK) {
        if (ctx_->recvTimeoutMs) {
            // If the error code represents a timeout, then the return
            // code should be translated to zero so that the caller
            // can retry the read operation.
            return 0;
        } else {
            return 0;
        }
    } else {
        LogError(("Socket recv(%d), timeout(%d) failed with %d", bytesToRecv, ctx_->sendTimeoutMs, rc));
        return -1;
    }
}

int32_t Mbed_Tls_Send( NetworkContext_t * pNetworkContext,
                       const void * pBuffer,
                       size_t bytesToSend )
{
    assert( pNetworkContext != NULL );
    assert( pBuffer != NULL );
    assert( bytesToSend > 0 );

    /* Cast 'pNetworkContext' to 'TlsNetworkContext_t *' for following net socket operations */
    auto *ctx_ = static_cast<TlsNetworkContext_t *>(static_cast<void *>(pNetworkContext));

    /* Validate socket */
    if (ctx_->socket == NULL) {
        LogError(("Parameter check failed: socket is NULL."));
        return -1;
    }

    /* Configure timeout in ms. */
    ctx_->socket->set_timeout(ctx_->sendTimeoutMs);

    /* Invoke socket send() and then cast return code */
    nsapi_size_or_error_t rc = ctx_->socket->send(pBuffer, bytesToSend);
    if (rc > 0) {
        return rc;
    } else if (rc == 0) {
        /* FIXME: Unclear definition with send zero in Mbed OS Socket. Treat as error */
        LogError(("Unclear definition with send zero in Mbed OS Socket. Treat as error."));
        return -1;
    } else if (rc == NSAPI_ERROR_WOULD_BLOCK) {
        if (ctx_->sendTimeoutMs) {
            // If the error code represents a timeout, then the return
            // code should be translated to zero so that the caller
            // can retry the write operation.
            return 0;
        } else {
            return 0;
        }
    } else {
        LogError(("Socket send(%d), timeout(%d) failed with %d", bytesToSend, ctx_->sendTimeoutMs, rc));
        return -1;
    }
}
