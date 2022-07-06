#include <stdio.h>
#include <csp/csp.h>

#include "telemetry.h"

void CdhTelemetryHandler(csp_conn_t * conn, csp_packet_t * packet);
void PayloadTelemetryHandler(csp_conn_t * conn, csp_packet_t * packet);