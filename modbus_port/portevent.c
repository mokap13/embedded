/*
  * FreeModbus Libary: LPC214X Port
  * Copyright (C) 2007 Tiago Prado Lone <tiago@maxwellbohr.com.br>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portevent.c,v 1.1 2007/04/24 23:15:18 wolti Exp $
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Variables ----------------------------------------*/
// static eMBEventType eQueuedEvent;
// static BOOL     xEventInQueue;
static QueueHandle_t xQueueHdl;
/* ----------------------- Start implementation -----------------------------*/
BOOL
xMBPortEventInit( void )
{
    BOOL            bStatus = FALSE;
    if( 0 != ( xQueueHdl = xQueueCreate( 1, sizeof( eMBEventType ) ) ) )
    {
        bStatus = TRUE;
    }
    return bStatus;
}

BOOL
xMBPortEventPost( eMBEventType eEvent )
{
    BOOL            bStatus = TRUE;
    if( bMBPortIsWithinException(  ) )
    {
        ( void )xQueueSendFromISR( xQueueHdl, ( const void * )&eEvent, pdFALSE );
    }
    else
    {
        ( void )xQueueSend( xQueueHdl, ( const void * )&eEvent, pdFALSE );
    }

    return bStatus;
}

BOOL
xMBPortEventGet( eMBEventType * peEvent )
{
    BOOL            xEventHappened = FALSE;
    
        if( pdTRUE == xQueueReceive( xQueueHdl, peEvent, portTICK_PERIOD_MS * 50 ) )
        {
            xEventHappened = TRUE;
        }
        return xEventHappened;
}
