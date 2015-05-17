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
#define AJ_MODULE MODEL1_SERVICE

#include <stdio.h>
#include <aj_debug.h>
 
#include "alljoyn.h"

#include "model1.h"

uint8_t dbgMODEL1_SERVICE = 0;

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
#define BASIC_SERVICE_SETPOINT             AJ_APP_MESSAGE_ID(0, 0, 0)
#define BASIC_SERVICE_CURRENT_TEMP         AJ_APP_MESSAGE_ID(0, 0, 1)
#define BASIC_SERVICE_SOFT_CURRENT_LIMIT   AJ_APP_MESSAGE_ID(0, 0, 2)
#define BASIC_SERVICE_CURRENT_DRAW_INSTANT AJ_APP_MESSAGE_ID(0, 0, 3)
#define BASIC_SERVICE_TIME_ODOMETER        AJ_APP_MESSAGE_ID(0, 0, 4)
#define BASIC_SERVICE_CURRENT_ODOMETER     AJ_APP_MESSAGE_ID(0, 0, 5)

int main(int argc, char **argv)
{
    AJ_Status status = AJ_OK;
    AJ_BusAttachment bus;
    uint8_t connected = FALSE;
    uint32_t sessionId = 0;

    /* One time initialization before calling any other AllJoyn APIs. */
    AJ_Initialize();

    /* This is for debug purposes and is optional. */
    AJ_PrintXML(AppObjects);

    AJ_RegisterObjects(AppObjects, NULL);

    while (TRUE) {
        AJ_Message msg;

        if (!connected) {
            status = AJ_StartService(&bus,
                                     NULL,
                                     CONNECT_TIMEOUT,
                                     FALSE,
                                     ServicePort,
                                     ServiceName,
                                     AJ_NAME_REQ_DO_NOT_QUEUE,
                                     NULL);

            if (status != AJ_OK) {
                continue;
            }

            AJ_InfoPrintf(("StartService returned %d, session_id=%u\n", status, sessionId));
            connected = TRUE;
        }

        status = AJ_UnmarshalMsg(&bus, &msg, UNMARSHAL_TIMEOUT);

        if (AJ_ERR_TIMEOUT == status) {
            continue;
        }

        if (AJ_OK == status) {
            switch (msg.msgId) {
            case AJ_METHOD_ACCEPT_SESSION:
                {
                    uint16_t port;
                    char* joiner;
                    AJ_UnmarshalArgs(&msg, "qus", &port, &sessionId, &joiner);
                    status = AJ_BusReplyAcceptSession(&msg, TRUE);
                    AJ_InfoPrintf(("Accepted session session_id=%u joiner=%s\n", sessionId, joiner));
                }
                break;

            case BASIC_SERVICE_SETPOINT:
                {
                    uint8_t setPoint = 40;
                    AJ_Message reply;
                    AJ_UnmarshalArgs(&msg, "y", &setPoint);
                    AJ_AlwaysPrintf(("Setting set point to: %d F.\n", setPoint));
                    /* Make set setpoint call */
                    AJ_MarshalReplyMsg(&msg, &reply);
                    AJ_InfoPrintf(("Set target water temperature returned %d, session_id=%u\n", status, sessionId));
                    status = AJ_DeliverMsg(&reply);                
                }
                break;

            case BASIC_SERVICE_CURRENT_TEMP:
                {
                    uint8_t currentTemp = 42;
                    AJ_Message reply;
                    AJ_Arg replyArg;
                    AJ_MarshalReplyMsg(&msg, &reply);
                    AJ_AlwaysPrintf(("Getting current temperature: %d F.\n", currentTemp));                    
                    /* Get the current temperature */
                    AJ_InitArg(&replyArg, AJ_ARG_BYTE, 0, (void *)&currentTemp, 0);
                    AJ_MarshalArg(&reply, &replyArg);
                    AJ_InfoPrintf(("Asked for  current temperature returned %d, session_id=%u\n", status, sessionId));
                    status = AJ_DeliverMsg(&reply);                
                }
                break;
                
            case BASIC_SERVICE_SOFT_CURRENT_LIMIT:
                {
                    uint8_t currentLimit = 10;
                    AJ_Message reply;
                    AJ_UnmarshalArgs(&msg, "y", &currentLimit);
                    AJ_AlwaysPrintf(("Setting soft current limit to: %d amps.\n", currentLimit));
                    /* Actually set the set point! */
                    AJ_MarshalReplyMsg(&msg, &reply);
                    AJ_InfoPrintf(("Setting soft current limit: returned %d, session_id=%u\n", status, sessionId));
                    status = AJ_DeliverMsg(&reply);                
                }
                break;
                
            case BASIC_SERVICE_CURRENT_DRAW_INSTANT:
                {
                    uint8_t currentUsedNow = 0;
                    AJ_Message reply;
                    AJ_Arg replyArg;
                    AJ_MarshalReplyMsg(&msg, &reply);
                    AJ_AlwaysPrintf(("Getting instantaneous current draw: %d amps.\n", currentUsedNow));
                    /* Get current draw */
                    AJ_InitArg(&replyArg, AJ_ARG_BYTE, 0, (void *)&currentUsedNow, 0);
                    AJ_MarshalArg(&reply, &replyArg);
                    AJ_InfoPrintf(("Asked for instant current draw value: returned %d, session_id=%u\n", status, sessionId));
                    status = AJ_DeliverMsg(&reply);                
                }
                break;
                
            case BASIC_SERVICE_TIME_ODOMETER:
                {
                    int timeRunning = 0;
                    AJ_Message reply;
                    AJ_Arg replyArg;
                    AJ_MarshalReplyMsg(&msg, &reply);
                    AJ_AlwaysPrintf(("Getting time odometer: %d seconds.\n", timeRunning));
                    /* Get current time odometer */
                    AJ_InitArg(&replyArg, AJ_ARG_INT32, 0, (void *)&timeRunning, 0);
                    AJ_MarshalArg(&reply, &replyArg);
                    AJ_InfoPrintf(("Asked for time odometer value: returned %d, session_id=%u\n", status, sessionId));
                    status = AJ_DeliverMsg(&reply);                
                }
                break;
                
            case BASIC_SERVICE_CURRENT_ODOMETER:
                {
                    int ampsUsed = 0;
                    AJ_Message reply;
                    AJ_Arg replyArg;
                    AJ_MarshalReplyMsg(&msg, &reply);
                    AJ_AlwaysPrintf(("Getting current odometer: %d amps.\n", ampsUsed));
                    /* Get current time odometer */
                    AJ_InitArg(&replyArg, AJ_ARG_INT32, 0, (void *)&ampsUsed, 0);
                    AJ_MarshalArg(&reply, &replyArg);
                    AJ_InfoPrintf(("Asked for current odometer value: returned %d, session_id=%u\n", status, sessionId));
                    status = AJ_DeliverMsg(&reply);                
                }
                break;
                
            case AJ_SIGNAL_SESSION_LOST_WITH_REASON:
                /* Session was lost so return error to force a disconnect. */
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

        if ((status == AJ_ERR_SESSION_LOST || status == AJ_ERR_READ)) {
            AJ_AlwaysPrintf(("AllJoyn disconnect.\n"));
            AJ_Disconnect(&bus);
            connected = FALSE;

            /* Sleep a little while before trying to reconnect. */
            AJ_Sleep(SLEEP_TIME);
        }
    }

    AJ_AlwaysPrintf(("Basic service exiting with status %d.\n", status));

    return status;
}
