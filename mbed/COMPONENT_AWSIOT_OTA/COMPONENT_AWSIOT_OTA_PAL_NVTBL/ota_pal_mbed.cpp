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

/* OTA PAL implementation for Mbed platform. */

#include "mbed.h"
#include "ota_pal_mbed.h"

/* Set up external block device */
#if defined(TARGET_NUMAKER_PFM_M487) || defined(TARGET_NUMAKER_IOT_M487)

#include "FlashIAPBlockDevice.h"
#include "SPIFBlockDevice.h"

/* Flash IAP for application area */
//FlashIAPBlockDevice flashiap_app(APPLICATION_ADDR, APPLICATION_SIZE);

/* Flash IAP for boot area */
//FlashIAPBlockDevice flashiap_boot(MBED_CONF_APP_BOOT_BASE_ADDRESS, MBED_CONF_APP_BOOT_SIZE);

/* We needn't write-protect and hold functions. Configure /WP and /HOLD pins to high. */
DigitalOut onboard_spi_wp(PC_5, 1);
DigitalOut onboard_spi_hold(PC_4, 1);

/* External SPI flash where to place firmware candidate */
SPIFBlockDevice spif(MBED_CONF_SPIF_DRIVER_SPI_MOSI,
                     MBED_CONF_SPIF_DRIVER_SPI_MISO,
                     MBED_CONF_SPIF_DRIVER_SPI_CLK,
                     MBED_CONF_SPIF_DRIVER_SPI_CS);

#endif

/**
 * @brief Specify the OTA signature algorithm we support on this platform.
 *
 * @note `OTA_JsonFileSignatureKey` is an extern variable declared but not defined in OTA library. This variable shall be defined in OTA platform abstraction layer implementation.
 */
const char OTA_JsonFileSignatureKey[ OTA_FILE_SIG_KEY_STR_MAX_LENGTH ] = "sig-sha256-ecdsa";

OtaPalStatus_t otaPal_Abort( OtaFileContext_t * const C )
{
    /* TODO */
    return OTA_PAL_COMBINE_ERR( OtaPalUninitialized, 0 );
}

OtaPalStatus_t otaPal_CreateFileForRx( OtaFileContext_t * const C )
{
    /* TODO */
    return OTA_PAL_COMBINE_ERR( OtaPalUninitialized, 0 );
}

OtaPalStatus_t otaPal_CloseFile( OtaFileContext_t * const C )
{
    /* TODO */
    return OTA_PAL_COMBINE_ERR( OtaPalUninitialized, 0 );
}

int16_t otaPal_WriteBlock( OtaFileContext_t * const C,
                           uint32_t ulOffset,
                           uint8_t * const pcData,
                           uint32_t ulBlockSize )
{
    /* TODO */
    return -1;
}

OtaPalStatus_t otaPal_ActivateNewImage( OtaFileContext_t * const C )
{
    /* TODO */
    return OTA_PAL_COMBINE_ERR( OtaPalUninitialized, 0 );
}

OtaPalStatus_t otaPal_SetPlatformImageState( OtaFileContext_t * const C,
                                             OtaImageState_t eState )
{
    /* TODO */
    return OTA_PAL_COMBINE_ERR( OtaPalUninitialized, 0 );
}

OtaPalStatus_t otaPal_ResetDevice( OtaFileContext_t * const C )
{
    /* TODO */
    return OTA_PAL_COMBINE_ERR( OtaPalUninitialized, 0 );
}

OtaPalImageState_t otaPal_GetPlatformImageState( OtaFileContext_t * const C )
{
    /* TODO */
    return OtaPalImageStateInvalid;
}
