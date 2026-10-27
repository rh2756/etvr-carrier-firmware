/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <string.h>
#include "bsp/board_api.h"
#include "tusb.h"

//--------------------------------------------------------------------+
// Device Descriptors
//--------------------------------------------------------------------+

static tusb_desc_device_t const desc_device = {
	.bLength = sizeof(tusb_desc_device_t),
	.bDescriptorType = TUSB_DESC_DEVICE,
	.bcdUSB = 0x0200,

	.bDeviceClass = TUSB_CLASS_MISC,
	.bDeviceSubClass = MISC_SUBCLASS_COMMON,
	.bDeviceProtocol = MISC_PROTOCOL_IAD,

	.idVendor = 0xCafe,
	.idProduct = 0x4000,
	.bcdDevice = 0x0100,

	.iManufacturer = 0x01,
	.iProduct = 0x02,
	.iSerialNumber = 0x03,

	.bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
	.bNumConfigurations = 0x01
};

// Invoked when received GET DEVICE DESCRIPTOR
// Application return pointer to descriptor
uint8_t const *tud_descriptor_device_cb(void) {
	return (uint8_t const *) &desc_device;
}

//--------------------------------------------------------------------+
// Configuration Descriptor
//--------------------------------------------------------------------+

enum {
	ITF_NUM_CDC = 0,
	ITF_NUM_CDC_DATA,
	ITF_NUM_TOTAL
};

#define EPNUM_CDC_NOTIF 0x81
#define EPNUM_CDC_OUT 0x02
#define EPNUM_CDC_IN 0x82

#define CONFIG_TOTAL_LEN (TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN)

static uint8_t const desc_configuration[] = {
	// Config number, interface count, string index, total length, attribute, power in mA
	TUD_CONFIG_DESCRIPTOR(1,ITF_NUM_TOTAL,0,CONFIG_TOTAL_LEN,0x00,500),

	// Interface number, string index, EP notification address and size, EP data address (out, in) and size.
	TUD_CDC_DESCRIPTOR(ITF_NUM_CDC,4,EPNUM_CDC_NOTIF,16,EPNUM_CDC_OUT,EPNUM_CDC_IN,64),
};

// Invoked when received GET CONFIGURATION DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const *tud_descriptor_configuration_cb(uint8_t index) {
	(void) index; // for multiple configurations

	return desc_configuration;
}

//--------------------------------------------------------------------+
// String Descriptors
//--------------------------------------------------------------------+

#define STRING_DESCRIPTOR_MAX_CHARACTERS 32
static const uint16_t STRING_DESCRIPTOR_LANGUAGE_ID = 0x0409; // Supported language is English (0x0409)
static const char *const STRING_DESCRIPTOR_MANUFACTURER = "TinyUSB";
static const char *const STRING_DESCRIPTOR_PRODUCT = "TinyUSB Device";
static const char *const STRING_DESCRIPTOR_SERIAL = "7f3a91c2e8b4065d1a9f73c0b2e6485d";
static const char *const STRING_DESCRIPTOR_CDC_INTERFACE = "TinyUSB CDC";

// String Descriptor Index
enum {
	STRID_LANGID = 0,
	STRID_MANUFACTURER,
	STRID_PRODUCT,
	STRID_SERIAL,
	STRID_CDC_INTERFACE
};

/**
 * @brief Converts an ASCII string literal to UTF-16 encoding and writes it to an output buffer.
 * @param string String literal to be converted.
 * @param length Number of characters to write to the output buffer. Length larger than the output buffer will be ignored.
 * @param output_buffer Output buffer.
 * @param output_buffer_size Output buffer size.
 * @param offset Offset writes to the output buffer by N elements.
 * @returns Number of bytes written to output buffer.
 */
static size_t char_to_utf16(const char *string, size_t length, uint16_t *output_buffer, size_t output_buffer_size, uint8_t offset) {
  size_t num_out_elements = output_buffer_size / 2; // Convert 8 bit to 16 bit.

  // Prevent writes if the offset is larger than the output buffer.
  if (offset > num_out_elements) {
		return 0;
	}

  size_t output_writable_space = num_out_elements - offset;
	size_t characters_to_write = length;

	if (length > output_writable_space) {
		characters_to_write = output_writable_space;
	}

	for (size_t i = 0; i < characters_to_write; i++) {
        output_buffer[offset + i] = (uint16_t) string[i];
    }

	return characters_to_write * 2; // 2x to return in bytes.
}

/**
 * @brief Creates a descriptor header with the first byte being the Length and second byte being the DescriptorType.
 * @param descriptor_type Type of descriptor.
 * @param descriptor_length Size of descriptor payload in bytes.
 * @returns 2 byte descriptor header.
 */
static uint16_t create_descriptor_header(tusb_desc_type_t descriptor_type, size_t descriptor_length) {
	return (descriptor_type << 8) | (descriptor_length + 2); // Add the 2 bytes for the header.
}

static uint16_t descriptor_string[STRING_DESCRIPTOR_MAX_CHARACTERS + 1]; // 1 extra for header.

// Invoked when received GET STRING DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long enough for transfer to complete
uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
	(void) langid;

	// Special case for language ID as its not a string literal.
	if (index == STRID_LANGID) {
		descriptor_string[0] = create_descriptor_header(TUSB_DESC_STRING,2);
		descriptor_string[1] = STRING_DESCRIPTOR_LANGUAGE_ID;

		return descriptor_string;
	}

	char const *string;
	switch (index) {
		case STRID_MANUFACTURER:
			string = STRING_DESCRIPTOR_MANUFACTURER;
			break;
		case STRID_PRODUCT:
			string = STRING_DESCRIPTOR_PRODUCT;
			break;
		case STRID_SERIAL:
			string = STRING_DESCRIPTOR_SERIAL;
			break;
		case STRID_CDC_INTERFACE:
			string = STRING_DESCRIPTOR_CDC_INTERFACE;
			break;
		default:
			return NULL;
	}

	// Put string into the buffer
	size_t descriptor_size = char_to_utf16(string,strlen(string),descriptor_string,sizeof(descriptor_string),1);

	// Add descriptor header
	descriptor_string[0] = create_descriptor_header(TUSB_DESC_STRING,descriptor_size);

	return descriptor_string;
}
