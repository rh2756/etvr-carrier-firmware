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

#define CFG_TUSB_DEBUG 0

// MCU Options
#define BOARD_TUD_RHPORT 0
#define CFG_TUSB_MCU OPT_MCU_SAMD11
#define CFG_TUSB_OS OPT_OS_NONE

// Device Stack Options
#define CFG_TUD_ENABLED 1
#define CFG_TUD_MAX_SPEED OPT_MODE_FULL_SPEED // USB Full Speed (12 Mbps) should be enough for this application.
#define CFG_TUD_ENDPOINT0_SIZE 64 // Size of the control transfers buffer.

// USB Class Options
#define CFG_TUD_CDC 1
#define CFG_TUD_CDC_NOTIFY 1 // Enable use of notification endpoint

// CDC FIFO size of TX and RX
#define CFG_TUD_CDC_RX_BUFSIZE (TUD_OPT_HIGH_SPEED ? 512 : 64)
#define CFG_TUD_CDC_TX_BUFSIZE (TUD_OPT_HIGH_SPEED ? 512 : 64)

// CDC Endpoint transfer buffer size, default to max bulk packet size (HS 512, FS 64). Larger is faster.
// Larger RX_EPSIZE requires CFG_TUD_CDC_RX_NEED_ZLP = 1 and host ZLP support
#define CFG_TUD_CDC_RX_EPSIZE (TUD_OPT_HIGH_SPEED ? 512 : 64)
#define CFG_TUD_CDC_TX_EPSIZE (TUD_OPT_HIGH_SPEED ? 512 : 64)
