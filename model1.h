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

#define CONNECT_ATTEMPTS    10

/* All times are expressed in milliseconds. */
#define CONNECT_TIMEOUT     (1000 * 60)
#define UNMARSHAL_TIMEOUT   (1000 * 5)
#define SLEEP_TIME          (1000 * 2)
#define METHOD_TIMEOUT      (100 * 10)

static const char ServiceName[] = "com.myheatworks.model1";
static const char ServicePath[] = "/control";
static const uint16_t ServicePort = 25;

/*
 * Buffer to hold the full service name. This buffer must be big enough to hold
 * a possible 255 characters plus a null terminator (256 bytes)
 */
static char fullServiceName[AJ_MAX_SERVICE_NAME_SIZE];

/**
 * The interface name followed by the method signatures.
 *
 * See also aj_introspect.h
 */
static const char* const sampleInterface[] = {
    "com.myheatworks.model1",          /* The first entry is the interface name. */
    "?setPoint temp<y",                /* Set the water temperature */
    "?currentTemp temp>y",             /* Get the water temperature setting */
    "?softCurrentLimit current<y",     /* Set the soft current limit */
    "?currentDrawInstant current>y",   /* Get the current draw at this instant */
    "?timeOdometerValue time>i",       /* Get the number of seconds the unit has been running. */
    "?currentOdometerValue current>i", /* Get the integral of amps where dt = 4 seconds */
    NULL
};

/**
 * A NULL terminated collection of all interfaces.
 */
static const AJ_InterfaceDescription sampleInterfaces[] = {
    sampleInterface,
    NULL
};

/**
 * Objects implemented by the application. The first member in the AJ_Object structure is the path.
 * The second is the collection of all interfaces at that path.
 */
static const AJ_Object AppObjects[] = {
    { ServicePath, sampleInterfaces },
    { NULL }
};

