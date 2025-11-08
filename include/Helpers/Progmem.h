#pragma once

#include "Helpers/Platforms.h"

#ifndef PROGMEM
#define PROGMEM
#define F(s) s
#define PSTR(s) s
#define FPSTR(s) s
#define __FlashStringHelper char
#define pgm_read_byte(addr) *(addr)
#endif
