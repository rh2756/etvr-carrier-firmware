#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define PROTOCOL_VERSION 1
#define PROTOCOL_DELIMITER 0xD5

	typedef struct
	{
		uint8_t protocol_version;
		uint8_t type;
		uint32_t payload;
		uint8_t payload_length;
	} protocol_header_t;

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PROTOCOL_H */