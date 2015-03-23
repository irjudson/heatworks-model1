/******************************************************************************
 * Copyright (c) 2015, Ivan R. Judson. All rights reserved.
 *
 *    Permission to use, copy, modify, and/or distribute this software for any
 *    purpose with or without fee is hereby granted, provided that the above
 *    copyright notice and this permission notice appear in all copies.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 ******************************************************************************/
 #define AJ_MODULE MODEL1_CLIENT

#include <stdio.h>
#include <stdlib.h>
#include <aj_debug.h>
#include <alljoyn.h>

#include "model1.h"

uint8_t dbgMODEL1_CLIENT = 0;

/*
 * The value of the arguments are the indices of the
 * object path in AppObjects (above), interface in sampleInterfaces (above), and
 * member indices in the interface.
 * The first index is 1 because the first entry in sampleInterface is the interface name.
 * This makes the first index (index 0 of the methods) the second string in
 * sampleInterface[].
 *
 * See also aj_introspect.h
 */
#define BASIC_CLIENT_SETPOINT             AJ_PRX_MESSAGE_ID(0, 0, 0)
#define BASIC_CLIENT_CURRENT_TEMP         AJ_PRX_MESSAGE_ID(0, 0, 1)
#define BASIC_CLIENT_SOFT_CURRENT_LIMIT   AJ_PRX_MESSAGE_ID(0, 0, 2)
#define BASIC_CLIENT_CURRENT_DRAW_INSTANT AJ_PRX_MESSAGE_ID(0, 0, 3)
#define BASIC_CLIENT_TIME_ODOMETER        AJ_PRX_MESSAGE_ID(0, 0, 4)
#define BASIC_CLIENT_CURRENT_ODOMETER     AJ_PRX_MESSAGE_ID(0, 0, 5)

void SetPoint(AJ_BusAttachment *bus, uint32_t sessionId, uint8_t setPoint) 
{
    AJ_Status status = AJ_OK;
    AJ_Message msg;
    status = AJ_MarshalMethodCall(bus, &msg, BASIC_CLIENT_SETPOINT, fullServiceName, sessionId, 0, METHOD_TIMEOUT);
    if (status == AJ_OK) {
        status = AJ_MarshalArgs(&msg, "y", setPoint);
    }
    if (status == AJ_OK) {
        status = AJ_DeliverMsg(&msg);
    }
    AJ_InfoPrintf(("SetSetPoint() resulted in a status of 0x%04x.\n", status));
}

void GetCurrentTemp(AJ_BusAttachment *bus, uint32_t sessionId) 
{
    AJ_Status status = AJ_OK;
    AJ_Message msg;
    status = AJ_MarshalMethodCall(bus, &msg, BASIC_CLIENT_CURRENT_TEMP, fullServiceName, sessionId, 0, METHOD_TIMEOUT);
    if (status == AJ_OK) {
        status = AJ_DeliverMsg(&msg);
    }
    AJ_InfoPrintf(("GetCurrentTemp() resulted in a status of 0x%04x.\n", status));
}

void SetSoftCurrentLimit(AJ_BusAttachment *bus, uint32_t sessionId, uint8_t currentLimit) 
{
    AJ_Status status = AJ_OK;
    AJ_Message msg;
    status = AJ_MarshalMethodCall(bus, &msg, BASIC_CLIENT_SOFT_CURRENT_LIMIT, fullServiceName, sessionId, 0, METHOD_TIMEOUT);
    if (status == AJ_OK) {
        status = AJ_MarshalArgs(&msg, "y", currentLimit);
    }
    if (status == AJ_OK) {
        status = AJ_DeliverMsg(&msg);
    }
    AJ_InfoPrintf(("SetSoftCurrentLimit() resulted in a status of 0x%04x.\n", status));
}

void GetCurrentDraw(AJ_BusAttachment *bus, uint32_t sessionId) 
{
    AJ_Status status = AJ_OK;
    AJ_Message msg;
    status = AJ_MarshalMethodCall(bus, &msg, BASIC_CLIENT_CURRENT_DRAW_INSTANT, fullServiceName, sessionId, 0, METHOD_TIMEOUT);
    if (status == AJ_OK) {
        status = AJ_DeliverMsg(&msg);
    }
    AJ_InfoPrintf(("GetCurrentDraw() resulted in a status of 0x%04x.\n", status));
}

void GetTimeOdometer(AJ_BusAttachment *bus, uint32_t sessionId) 
{
    AJ_Status status = AJ_OK;
    AJ_Message msg;
    status = AJ_MarshalMethodCall(bus, &msg, BASIC_CLIENT_TIME_ODOMETER, fullServiceName, sessionId, 0, METHOD_TIMEOUT);
    if (status == AJ_OK) {
        status = AJ_DeliverMsg(&msg);
    }
    AJ_InfoPrintf(("GetTimeOdometer() resulted in a status of 0x%04x.\n", status));
}

void GetCurrentOdometer(AJ_BusAttachment *bus, uint32_t sessionId) 
{
    AJ_Status status = AJ_OK;
    AJ_Message msg;
    status = AJ_MarshalMethodCall(bus, &msg, BASIC_CLIENT_CURRENT_ODOMETER, fullServiceName, sessionId, 0, METHOD_TIMEOUT);
    if (status == AJ_OK) {
        status = AJ_DeliverMsg(&msg);
    }
    AJ_InfoPrintf(("GetCurrentOdometer() resulted in a status of 0x%04x.\n", status));
}

int main(int argc, char **argv)
{
    AJ_Status status = AJ_OK;
    AJ_BusAttachment bus;
    uint8_t connected = FALSE;
    uint8_t done = FALSE;
    uint32_t sessionId = 0;

    /*
     * One time initialization before calling any other AllJoyn APIs
     */
    AJ_Initialize();
    AJ_PrintXML(AppObjects);
    AJ_RegisterObjects(NULL, AppObjects);

    while (!done) {
        AJ_Message msg;

        if (!connected) {
            status = AJ_StartClientByName(&bus,
                                          NULL,
                                          CONNECT_TIMEOUT,
                                          FALSE,
                                          ServiceName,
                                          ServicePort,
                                          &sessionId,
                                          NULL,
                                          fullServiceName);

            if (status == AJ_OK) {
                AJ_InfoPrintf(("StartClient returned %d, sessionId=%u.\n", status, sessionId));
                connected = TRUE;
                SetPoint(&bus, sessionId, 50);
                GetCurrentTemp(&bus, sessionId);
                SetSoftCurrentLimit(&bus, sessionId, 48);
                GetCurrentDraw(&bus, sessionId);
                GetTimeOdometer(&bus, sessionId);
                GetCurrentOdometer(&bus, sessionId);
            } else {
                AJ_InfoPrintf(("StartClient returned 0x%04x.\n", status));
                break;
            }
        }

        status = AJ_UnmarshalMsg(&bus, &msg, UNMARSHAL_TIMEOUT);

        if (AJ_ERR_TIMEOUT == status) {
            continue;
        }

        if (AJ_OK == status) {
            switch (msg.msgId) {
            case AJ_REPLY_ID(BASIC_CLIENT_CURRENT_TEMP):
                {
                    AJ_Arg arg;
                    status = AJ_UnmarshalArg(&msg, &arg);
                    if (AJ_OK == status) {
                        AJ_AlwaysPrintf(("'%s.%s' (path='%s') returned '%d'.\n", fullServiceName, "currentTemp",
                                         ServicePath, (int)*(arg.val.v_byte)));
                    } else {
                        AJ_InfoPrintf(("AJ_UnmarshalArg() returned status %d.\n", status));
                        GetCurrentTemp(&bus, sessionId);
                    }
                }
            break;
            case AJ_REPLY_ID(BASIC_CLIENT_CURRENT_DRAW_INSTANT):
                {
                    AJ_Arg arg;
                    status = AJ_UnmarshalArg(&msg, &arg);
                    if (AJ_OK == status) {
                        AJ_AlwaysPrintf(("'%s.%s' (path='%s') returned '%d'.\n", fullServiceName, "currentDrawInstant",
                                         ServicePath, (int)*(arg.val.v_byte)));
                    } else {
                        AJ_InfoPrintf(("AJ_UnmarshalArg() returned status %d.\n", status));
                        GetCurrentTemp(&bus, sessionId);
                    }
                }
            break;
            case AJ_REPLY_ID(BASIC_CLIENT_TIME_ODOMETER):
                {
                    AJ_Arg arg;
                    status = AJ_UnmarshalArg(&msg, &arg);
                    if (AJ_OK == status) {
                        AJ_AlwaysPrintf(("'%s.%s' (path='%s') returned '%d'.\n", fullServiceName, "timeOdometerValue",
                                         ServicePath, (int)*(arg.val.v_int32)));
                    } else {
                        AJ_InfoPrintf(("AJ_UnmarshalArg() returned status %d.\n", status));
                        GetCurrentTemp(&bus, sessionId);
                    }
                }
            break;
            case AJ_REPLY_ID(BASIC_CLIENT_CURRENT_ODOMETER):
                {
                    AJ_Arg arg;
                    status = AJ_UnmarshalArg(&msg, &arg);
                    if (AJ_OK == status) {
                        AJ_AlwaysPrintf(("'%s.%s' (path='%s') returned '%d'.\n", fullServiceName, "currentOdometerValue",
                                         ServicePath, (int)*(arg.val.v_int32)));
                    } else {
                        AJ_InfoPrintf(("AJ_UnmarshalArg() returned status %d.\n", status));
                        GetCurrentTemp(&bus, sessionId);
                    }
                }
            break;
            case AJ_SIGNAL_SESSION_LOST_WITH_REASON:
                /* A session was lost so return error to force a disconnect. */
                {
                    uint32_t id, reason;
                    AJ_UnmarshalArgs(&msg, "uu", &id, &reason);
                    AJ_AlwaysPrintf(("Session lost. ID = %u, reason = %u", id, reason));
                }
                status = AJ_ERR_SESSION_LOST;
                break;
            default:
                /* Pass to the built-in handlers. */
                status = AJ_BusHandleBusMessage(&msg);
                break;
            }
        }

        /* Messages MUST be discarded to free resources. */
        AJ_CloseMsg(&msg);

        if (status == AJ_ERR_SESSION_LOST) {
            AJ_AlwaysPrintf(("AllJoyn disconnect.\n"));
            AJ_Disconnect(&bus);
            exit(0);
        }
    }

    AJ_AlwaysPrintf(("Basic client exiting with status %d.\n", status));

    return status;
}

