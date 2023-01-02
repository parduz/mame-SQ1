// license:BSD-3-Clause
// copyright-holders:Parduz
/*
    Ensoniq LCD dotmatrix Displays
    derived by Parduz from the VFD Emulation by R. Belmont
*/
#include "emu.h"
#include "esqlcd.h"
// #include "sq1.lh" // Moved in esq5505 - Parduz

#define VERBOSE 1
#include "logmacro.h"

DEFINE_DEVICE_TYPE(ESQ2X16_SQ1, esq2x16_sq1_device, "esq2x16_sq1", "Ensoniq 2x16 VFD (SQ-1 variant)")

// --- SQ1 - Parduz --------------------------------------------------------------------------------------------------------------------------
#if DEBUG_LCD	// defined in the h file
 #define LCD_LOG	LOG
 const char CurrPageName[4][10] = {
 	{'B','l','i','n','k',' ',' ',' ',' ', 0 },
 	{'U','n','k','n','o','w','n',' ',' ', 0 },
 	{'N','o','r','m','a','l',' ',' ',' ', 0 },
 	{'-','-','-','-','-','-','-','-','-', 0 }
 };
 #define CurrPageStr(pIndex) &CurrPageName[(pIndex)][0]

 const char CurrPageSHRT[4][10] = {
 	{'B','l',' ',' ',' ',' ',' ',' ',' ', 0 },
 	{' ',' ',' ',' ','_','_',' ',' ',' ', 0 },
 	{' ',' ',' ',' ',' ',' ',' ','N','o', 0 },
 	{'-','-','-','-','-','-','-','-','-', 0 }
 };
 #define CurrPageShort(pIndex) &CurrPageSHRT[(pIndex)][0]

 #define LOCATION_INFO m_lcdCursor.Pos,CurrPageShort(m_lcdCursor.Attr)
#else
 #define LCD_LOG(...)
#endif


/*! \file font5x7.h \brief Graphic LCD Font (Ascii Characters). */
//*****************************************************************************
//
// File Name    : 'font5x7.h'
// Title        : Graphic LCD Font (Ascii Charaters)
// Author       : Pascal Stang
// Date         : 10/19/2001
// Revised      : 10/19/2001
// Version      : 0.1
// Target MCU   : Atmel AVR
// Editor Tabs  : 4
//
//*****************************************************************************
// standard ascii 5x7 font
// defines ascii characters 0x20-0x7F (32-127)

static unsigned char const Font5x7[DOTCHARS][DOTCOLS] = {
#if DEBUG_LCD
	{0x00, 0x00, 0x08, 0x00, 0x00}, // _Undef_      0x00 - dots for debug purposes
	{0x00, 0x00, 0x14, 0x00, 0x00}, // _Undef_      0x01 - dots for debug purposes
	{0x00, 0x00, 0x22, 0x00, 0x00}, // _Undef_      0x02
	{0x00, 0x00, 0x33, 0x00, 0x00}, // _Undef_      0x03
	{0x04, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x04
	{0x05, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x05
	{0x06, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x06
	{0x07, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x07
#else
	{0x00, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x00
	{0x01, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x01
	{0x02, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x02
	{0x03, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x03
	{0x04, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x04
	{0x05, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x05
	{0x06, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x06
	{0x07, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x07
#endif
	{0x20, 0x70, 0x3F, 0x00, 0x00}, // Croma        0x08
	{0x20, 0x70, 0x3F, 0x02, 0x0C}, // Croma        0x09
	{0x20, 0x70, 0x3F, 0x05, 0x0A}, // Croma        0x0A
	{0x20, 0x70, 0x3F, 0x15, 0x2A}, // Croma        0x0B
	{0x20, 0x50, 0x50, 0x3F, 0x00}, // Croma        0x0C
	{0x0D, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x0D
	{0x0E, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x0E
	{0x0F, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x0F
	{0x10, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x10
	{0x11, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x11
	{0x12, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x12
	{0x13, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x13
	{0x14, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x14
	{0x15, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x15
	{0x16, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x16
	{0x17, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x17
	{0x18, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x18
	{0x19, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x19
	{0x1A, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x1A
	{0x1B, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x1B
	{0x1C, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x1C
	{0x1D, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x1D
	{0x1E, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x1E
	{0x1F, 0x00, 0x00, 0x00, 0x00}, // _Undef_      0x1F
	{0x00, 0x00, 0x00, 0x00, 0x00}, // (space)      0x20
	{0x00, 0x00, 0x5F, 0x00, 0x00}, // !            0x21
	{0x00, 0x07, 0x00, 0x07, 0x00}, // "            0x22
	{0x14, 0x7F, 0x14, 0x7F, 0x14}, // #            0x23
	{0x24, 0x2A, 0x7F, 0x2A, 0x12}, // $            0x24
	{0x23, 0x13, 0x08, 0x64, 0x62}, // %            0x25
	{0x36, 0x49, 0x55, 0x22, 0x50}, // &            0x26
	{0x00, 0x05, 0x03, 0x00, 0x00}, // '            0x27
	{0x00, 0x1C, 0x22, 0x41, 0x00}, // (            0x28
	{0x00, 0x41, 0x22, 0x1C, 0x00}, // )            0x29
	{0x08, 0x2A, 0x1C, 0x2A, 0x08}, // *            0x2A
	{0x08, 0x08, 0x3E, 0x08, 0x08}, // +            0x2B
	{0x00, 0x50, 0x30, 0x00, 0x00}, // ,            0x2C
	{0x08, 0x08, 0x08, 0x08, 0x08}, // -            0x2D
	{0x00, 0x60, 0x60, 0x00, 0x00}, // .            0x2E
	{0x20, 0x10, 0x08, 0x04, 0x02}, // /            0x2F
	{0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0            0x30
	{0x00, 0x42, 0x7F, 0x40, 0x00}, // 1            0x31
	{0x42, 0x61, 0x51, 0x49, 0x46}, // 2            0x32
	{0x21, 0x41, 0x45, 0x4B, 0x31}, // 3            0x33
	{0x18, 0x14, 0x12, 0x7F, 0x10}, // 4            0x34
	{0x27, 0x45, 0x45, 0x45, 0x39}, // 5            0x35
	{0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6            0x36
	{0x01, 0x71, 0x09, 0x05, 0x03}, // 7            0x37
	{0x36, 0x49, 0x49, 0x49, 0x36}, // 8            0x38
	{0x06, 0x49, 0x49, 0x29, 0x1E}, // 9            0x39
	{0x00, 0x36, 0x36, 0x00, 0x00}, // :            0x3A
	{0x00, 0x56, 0x36, 0x00, 0x00}, // ;            0x3B
	{0x00, 0x08, 0x14, 0x22, 0x41}, // <            0x3C
	{0x14, 0x14, 0x14, 0x14, 0x14}, // =            0x3D
	{0x41, 0x22, 0x14, 0x08, 0x00}, // >            0x3E
	{0x02, 0x01, 0x51, 0x09, 0x06}, // ?            0x3F
	{0x32, 0x49, 0x79, 0x41, 0x3E}, // @            0x40
	{0x7E, 0x11, 0x11, 0x11, 0x7E}, // A            0x41
	{0x7F, 0x49, 0x49, 0x49, 0x36}, // B            0x42
	{0x3E, 0x41, 0x41, 0x41, 0x22}, // C            0x43
	{0x7F, 0x41, 0x41, 0x22, 0x1C}, // D            0x44
	{0x7F, 0x49, 0x49, 0x49, 0x41}, // E            0x45
	{0x7F, 0x09, 0x09, 0x01, 0x01}, // F            0x46
	{0x3E, 0x41, 0x41, 0x51, 0x32}, // G            0x47
	{0x7F, 0x08, 0x08, 0x08, 0x7F}, // H            0x48
	{0x00, 0x41, 0x7F, 0x41, 0x00}, // I            0x49
	{0x20, 0x40, 0x41, 0x3F, 0x01}, // J            0x4A
	{0x7F, 0x08, 0x14, 0x22, 0x41}, // K            0x4B
	{0x7F, 0x40, 0x40, 0x40, 0x40}, // L            0x4C
	{0x7F, 0x02, 0x04, 0x02, 0x7F}, // M            0x4D
	{0x7F, 0x04, 0x08, 0x10, 0x7F}, // N            0x4E
	{0x3E, 0x41, 0x41, 0x41, 0x3E}, // O            0x4F
	{0x7F, 0x09, 0x09, 0x09, 0x06}, // P            0x50
	{0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q            0x51
	{0x7F, 0x09, 0x19, 0x29, 0x46}, // R            0x52
	{0x46, 0x49, 0x49, 0x49, 0x31}, // S            0x53
	{0x01, 0x01, 0x7F, 0x01, 0x01}, // T            0x54
	{0x3F, 0x40, 0x40, 0x40, 0x3F}, // U            0x55
	{0x1F, 0x20, 0x40, 0x20, 0x1F}, // V            0x56
	{0x7F, 0x20, 0x18, 0x20, 0x7F}, // W            0x57
	{0x63, 0x14, 0x08, 0x14, 0x63}, // X            0x58
	{0x03, 0x04, 0x78, 0x04, 0x03}, // Y            0x59
	{0x61, 0x51, 0x49, 0x45, 0x43}, // Z            0x5A
	{0x00, 0x00, 0x7F, 0x41, 0x41}, // [            0x5B
	{0x02, 0x04, 0x08, 0x10, 0x20}, // \            0x5C
	{0x41, 0x41, 0x7F, 0x00, 0x00}, // ]            0x5D
	{0x04, 0x02, 0x01, 0x02, 0x04}, // ^            0x5E
	{0x40, 0x40, 0x40, 0x40, 0x40}, // _            0x5F
	{0x00, 0x01, 0x02, 0x04, 0x00}, // `            0x60
	{0x20, 0x54, 0x54, 0x54, 0x78}, // a            0x61
	{0x7F, 0x48, 0x44, 0x44, 0x38}, // b            0x62
	{0x38, 0x44, 0x44, 0x44, 0x20}, // c            0x63
	{0x38, 0x44, 0x44, 0x48, 0x7F}, // d            0x64
	{0x38, 0x54, 0x54, 0x54, 0x18}, // e            0x65
	{0x08, 0x7E, 0x09, 0x01, 0x02}, // f            0x66
	{0x08, 0x14, 0x54, 0x54, 0x3C}, // g            0x67
	{0x7F, 0x08, 0x04, 0x04, 0x78}, // h            0x68
	{0x00, 0x44, 0x7D, 0x40, 0x00}, // i            0x69
	{0x20, 0x40, 0x44, 0x3D, 0x00}, // j            0x6A
	{0x00, 0x7F, 0x10, 0x28, 0x44}, // k            0x6B
	{0x00, 0x41, 0x7F, 0x40, 0x00}, // l            0x6C
	{0x7C, 0x04, 0x18, 0x04, 0x78}, // m            0x6D
	{0x7C, 0x08, 0x04, 0x04, 0x78}, // n            0x6E
	{0x38, 0x44, 0x44, 0x44, 0x38}, // o            0x6F
	{0x7C, 0x14, 0x14, 0x14, 0x08}, // p            0x70
	{0x08, 0x14, 0x14, 0x18, 0x7C}, // q            0x71
	{0x7C, 0x08, 0x04, 0x04, 0x08}, // r            0x72
	{0x48, 0x54, 0x54, 0x54, 0x20}, // s            0x73
	{0x04, 0x3F, 0x44, 0x40, 0x20}, // t            0x74
	{0x3C, 0x40, 0x40, 0x20, 0x7C}, // u            0x75
	{0x1C, 0x20, 0x40, 0x20, 0x1C}, // v            0x76
	{0x3C, 0x40, 0x30, 0x40, 0x3C}, // w            0x77
	{0x44, 0x28, 0x10, 0x28, 0x44}, // x            0x78
	{0x0C, 0x50, 0x50, 0x50, 0x3C}, // y            0x79
	{0x44, 0x64, 0x54, 0x4C, 0x44}, // z            0x7A
	{0x00, 0x08, 0x36, 0x41, 0x00}, // {            0x7B
	{0x00, 0x00, 0x7F, 0x00, 0x00}, // |            0x7C
	{0x00, 0x41, 0x36, 0x08, 0x00}, // }            0x7D
	{0x08, 0x08, 0x2A, 0x1C, 0x08}, // ->           0x7E
	{0x08, 0x1C, 0x2A, 0x08, 0x08}  // <-           0x7F
};


// (more or less) Known commands
#define LCD_CMD_GOTOPOS		0x87
#define LCD_CMD_SAVEPOS		0x88
#define LCD_CMD_RESTPOS		0x89
#define LCD_CMD_LEDOFF		0x8D
#define LCD_CMD_LEDON		0x8E
#define LCD_CMD_LEDBLNK		0x8F
#define LCD_CMD_CHAR_BLNK	0x90
#define LCD_CMD_CHAR_UNK	0x91
#define LCD_CMD_CHAR_NORM	0x92
#define LCD_CMD_LCD_CLR		0x8C
#define LCD_CMD_ATTR_CLR	0x98



//--------------------------------------------------------------------------------------------------------------------------------------------
void esq2x16_sq1_device::device_add_mconfig(machine_config &config)
{
	//config.set_default_layout(layout_sq1); // Moved in esq5505 - Parduz
}

//--------------------------------------------------------------------------------------------------------------------------------------------
void esq2x16_sq1_device::write_char(int data)
{
	int DisplayCode = data;
	int LedState;

	// Non-ASCII codes that needs to be treated as ASCII characters
	if (
		data == 0x08 ||
		data == 0x09 ||
		data == 0x0A ||
		data == 0x0B ||
		data == 0x0C
	) data = '^';  // musical notes

	// 0x00: // Unknown Code, sent in the beginning routine
	// 0x02: // Unknown Code, sent in the beginning routine
	// 0x0E: // Unknown Code, sent in the beginning routine
	// 0x12: // Unknown Code, sent in the beginning routine

	// Resolve here 2-Bytes commands: the command was saved previously
	switch (m_lcd_command) {
		case 0:
			// No current command.
			break;

		case LCD_CMD_GOTOPOS:
			// Go To
			LCD_LOG("LCD %02X: Go To pos %02d              - pos=%02d [%s]\n", m_lcd_command, DisplayCode, LOCATION_INFO);
			lcdCursor_Move(DisplayCode);
			m_lcd_command = -1;
			break;

		case LCD_CMD_SAVEPOS:
			// Save Cursor position - The second byte should be to switch cursor visibility
			if (DisplayCode) {
				LCD_LOG("LCD %02X: Save Pos. %02d ############ - pos=%02d [%s]\n", m_lcd_command, m_lcdCursor.Pos, LOCATION_INFO);
				m_lcdCursor.BlinkType = 0xFFFF;	// Dunno what to write here.
				m_lcdCursor.BlinkPos = m_lcdCursor.Pos;
				m_lcdCursor.BlinkSavedPos = m_lcdCursor.Pos;
				update_display();
			}else{
				LCD_LOG("LCD %02X: Save.                       - pos=%02d [%s]\n", m_lcd_command, LOCATION_INFO);
				m_lcdCursor.BlinkType = 0x0000;	// Dunno what to write here.
				// Save screen to buffer ...? This command have to do something with buffering
				lcdScreen_Save();
				lcdCursor_Save();
			}
			m_lcd_command = -1;
			break;

		case LCD_CMD_RESTPOS:
			// Restore Cursor position - The second byte should be to switch cursor visibility
			if (DisplayCode) {
				LCD_LOG("LCD %02X: Restore Pos. %02d ######### - pos=%02d [%s]\n", m_lcd_command, m_lcdCursor.SavedPos, LOCATION_INFO);
				// Set cursor visible ... ?
				m_lcdCursor.BlinkType = 0xFFFF;	// Dunno what to write here.
				m_lcdCursor.Pos = m_lcdCursor.BlinkPos = m_lcdCursor.BlinkSavedPos;
				update_display();
			}else{
				LCD_LOG("LCD %02X: Restore.                    - pos=%02d [%s]\n", m_lcd_command, LOCATION_INFO);
				m_lcdCursor.BlinkType = 0;	// Dunno what to write here.
				// Restore screen from buffer ... ? This command have to do something with buffering
				lcdScreen_Restore();
				lcdScreen_ResetAttr();
				lcdCursor_Restore();
			}
			Blink_Reset(true);
			m_lcd_command = -1;
			break;

		case LCD_CMD_LEDOFF:
		case LCD_CMD_LEDON:
		case LCD_CMD_LEDBLNK:
			// LED OFF, ON, BLINK, ????
			LedState = m_lcd_command & 0x03;
			if (
			  DisplayCode >= 16 || // Out of bounds
			  DisplayCode == 6  || // non-existent
			  DisplayCode == 7  || // non-existent
			  DisplayCode == 14 || // non-existent
			  DisplayCode == 15    // non-existent
			)
			{
				LCD_LOG("\n!!!!!!!!!!!! LCD %02X: Led %02d doesn't exist       - pos=%02d [%s]\n", m_lcd_command, DisplayCode, LOCATION_INFO);
			}
			else
			{
				LCD_LOG("LCD %02X: Set LED %02d %s (%d) - pos=%02d [%s]\n", m_lcd_command, DisplayCode, ( LedState==2?" OFF ":LedState==1?" ON  ":LedState==0?"BLINK":"?????" ), LedState, LOCATION_INFO);
				//m_leds[DisplayCode] = LedState;
				m_ledsStatus[DisplayCode] = LedState;
				update_leds();
			}
			m_lcd_command = -1;
			break;

		case 0x85:	// Set the visible cursor
			m_lcdCursor.BlinkType = (m_lcdCursor.BlinkType & 0xFF00) | (DisplayCode & 0x00FF); // Just guessing, here....
			if (DisplayCode != 0x10) {
				LCD_LOG("LCD %02X: Cursor set to %04X ###### - pos=%02d [%s]\n", m_lcd_command, m_lcdCursor.BlinkType, LOCATION_INFO);
			}else{
				LCD_LOG("LCD %02X: Cursor set to %04X        - pos=%02d [%s]\n", m_lcd_command, m_lcdCursor.BlinkType, LOCATION_INFO);
			}
			m_lcd_command = -1;
			break;

		case 0x86:	// Set the visible cursor
			m_lcdCursor.BlinkType = (m_lcdCursor.BlinkType & 0x00FF) | (DisplayCode*256 & 0xFF00); // Just guessing, here....
			if (DisplayCode != 0x01) {
				LCD_LOG("LCD %02X: Cursor set to %04X ###### - pos=%02d [%s]\n", m_lcd_command, m_lcdCursor.BlinkType, LOCATION_INFO);
			}else{
				LCD_LOG("LCD %02X: Cursor set to %04X        - pos=%02d [%s]\n", m_lcd_command, m_lcdCursor.BlinkType, LOCATION_INFO);
			}
			m_lcd_command = -1;
			break;

		default:
			LCD_LOG("\n!!!!!!!!!!!! LCD: Unknown 2-Bytes Command:%02X-%02X - pos=%02d [%s]\n", m_lcd_command, DisplayCode, LOCATION_INFO);
			m_lcd_command = 0;
			return;
	}

	if (m_lcd_command == -1) {
		// the command has been found and executed/saved.
		// Update the display if needed
		if (lcd_is_dirty) {
			update_display();
		}
		m_lcd_command = 0;
		return;
	}

	// We're here only if m_lcd_command was 0 (so, the previous switch() did nothing)
	if ((data >= 0x20) && (data <= 0x7f))
	{
		// Write this character on the display
		LCD_LOG("LCD %02X:                     \"%c\"   - pos=%02d [%s]\n", DisplayCode, data, LOCATION_INFO);
		if (lcd_write(DisplayCode))
		{
			update_display();
		}
		lcdCursor_Inc();
		return;
	}

	// Data wasn't a charachter. Is it a command?
	if (DisplayCode >= 0x80)
	{
		switch (DisplayCode) {
			//---- Known 2-bytes commands ------

			case LCD_CMD_GOTOPOS:  // Go To

			case LCD_CMD_SAVEPOS:  // Save Cursor Position & set visibility
			case LCD_CMD_RESTPOS:  // Restore Cursor Position & set visibility
				// I think that those two commands could also do some copy/swapping/whatever operation on the
				// whole screen. When the display is missing non-biinking parts is 'cause i don't get
				// what should we do here.
				// Parduz.

				// LEDs OFF,ON and BLINK - 2 bytes command
			case LCD_CMD_LEDOFF:
			case LCD_CMD_LEDON:
			case LCD_CMD_LEDBLNK:

			case 0x85:	// Set the visible cursor
			case 0x86:	// Set the visible curtor
				// These two commands are related to the visible cursor, only appearing as 0x85-0x10 and 0x86-0x01.
				// What they are supposed to do? In the end, on the SQ1 seems that setting the cursor visible from the 
				// save/restore position commands is enough.
				// Parduz.

			case 0x95:
				// 2-bytes command with unknown meaning.
				// Appears only with the Wave Delay Time parameter (the only one with a 0~250 range) 

				m_lcd_command = DisplayCode; // Save the command, wait for the next byte
				return;

			//---- Guessed 1-byte commands ------
			// "Attribute" selectors: the next chars will be written with this attributes.
			case LCD_CMD_CHAR_BLNK : // Blink
			case LCD_CMD_CHAR_UNK  : // ??
			case LCD_CMD_CHAR_NORM : // Normal
				// I "sense" that these commands SHOULD do something else, like buffering the screen, or rendering it.
				// Cannot find exactly what, though.
				// Parduz

				LCD_LOG("LCD %02X: %s charachters    - pos=%02d [%s]\n", DisplayCode, CurrPageStr(DisplayCode & 0x03), LOCATION_INFO);

				//m_lcdCursor.Attr = DisplayCode & 0x03;
				// i prefer to have "0" as normal, and "2" as blink
				lcdCursor_Attr ( (DisplayCode & 0x03)==0?chBlink:(DisplayCode & 0x03)==1?chUnknown:chNormal );
					// It seems to work this way....
				if (m_lcdCursor.Attr==chBlink)
					lcdScreen_ResetAttr();
				else if (m_lcdCursor.Attr==chUnknown)
					Blink_Reset(true);

				m_lcd_command = -1;
				break;

			case LCD_CMD_ATTR_CLR:
				// Set all attributes to "Normal" ... maybe.
				LCD_LOG("\n             LCD %02X: Attributes Clear          - pos=%02d [%s]\n", DisplayCode, LOCATION_INFO);
				lcdScreen_ResetAttr();
				m_lcd_command = -1;
				break;

			case LCD_CMD_LCD_CLR:
				// Zero everything.
				LCD_LOG("\n             LCD %02X: Lcd Clear                 - pos=%02d [%s]\n", DisplayCode, LOCATION_INFO);
				lcd_reset();
				m_lcd_command = -1;
				break;

			// We know we don't know these commands:
			// All of them appears only in the beginning routine ("notes coregraphy")
			case 0x80:
			case 0x81:
			case 0x83:
			case 0x93:
			case 0x94:
			case 0xFF:
				LCD_LOG("LCD %02X: Command with unknown use  - pos=%02d [%s]\n", LOCATION_INFO);
				m_lcd_command = -1;
				break;

			default:
				LCD_LOG("\n!!!!!!!!!!!! LCD %02X: Unknown Command !!!!!!!!! - pos=%02d [%s]\n", DisplayCode, LOCATION_INFO);
				m_lcd_command = 0;
				return;
		}

		if (m_lcd_command == -1) {
			// the command has been found and executed.
			// Update the display if needed
			if (lcd_is_dirty) {
				update_display();
			}
			m_lcd_command = 0;
			return;
		}

	}
	else
	{
		LCD_LOG("LCD: Unknown LCD Code: %04X       - pos=%02d [%s]\n", data, LOCATION_INFO);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------
void esq2x16_sq1_device::update_leds()
{
#if DEBUG_LCD
 #define MANAGED_LED_STATE(st)	(st)
#else
 #define MANAGED_LED_STATE(st)	(m_blinker_on?0:1)
#endif

	int iLed=0;
	for (auto &led : m_leds) {
		if (iLed>=0 && iLed<NUMLEDS) {
			switch (m_ledsStatus[iLed]) {
				case 0:
					led = MANAGED_LED_STATE(2);
					break;
				case 1:
					led = 1;
					break;
				case 2:
					led = 0;
					break;
				case 3:
					led = MANAGED_LED_STATE(3);
					break;
			}
			++iLed;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------
void esq2x16_sq1_device::update_display()
{
	uint8_t	curr_char, blink_char, attr;
	bool	blinkit = false;

	for (int pos = 0; pos < LCDCHARS; pos++)
	{
		blinkit = false;
		curr_char = m_lcdScreen.Chars[pos];
		attr  = m_lcdScreen.Attributes[pos];
		if (
		  curr_char>=0x20 // Chars below are "special" and don't blinks
		  &&
		  attr == chBlink
		){
			blinkit = true;
			blink_char = 0;	// There's two types of blink: with or without visible cursor
		}
		// There's to types of blink: with or without visible cursor
		// if the cursor is visible, blinkit
		if (m_lcdCursor.BlinkType && m_lcdCursor.BlinkPos == pos) {
			blinkit = true;
			blink_char = 0x5F; // "_"
		}
		// If it is time to blink, and we have to do it, then blink it
		if (blinkit && m_blinker_on) {
			curr_char = blink_char;
		}

		// Now we can draw the char we've found.
		for (int row = 0; row < DOTROWS; row++)
			m_lcdPix[pos*DOTROWS + row] = m_RenderedFont[curr_char].dotLine[row];
	}
#if DEBUG_LCD
	update_debug();
#endif

	lcd_is_dirty = false;
}

//--------------------------------------------------------------------------------------------------------------------------------------------
#if DEBUG_LCD

void esq2x16_sq1_device::update_debug()
{
	// for debug purposes, draw all the data of the screen
	int page, pos, row;
	uint8_t DotRow, lcdCharIndex, charAttr;
	for (pos = 0; pos < LCDCHARS; pos++)
	{
		lcdCharIndex = m_lcdScreen.Chars[pos];
		charAttr     = m_lcdScreen.Attributes[pos] & 0x03;	// 0x03 just to be sure
		for (page=0; page<chTOTAL; page++) {
			for (row = 0; row < DOTROWS; row++)
			{
				DotRow = (page == charAttr)? m_RenderedFont[lcdCharIndex].dotLine[row] : 0;
				m_lcdPixDbg[page][pos*8 + row] = DotRow;
			}
			// Show on row 8 the actual cursor position
			DotRow = (m_lcdCursor.Pos == pos)?0x1F:0;
			m_lcdPixDbg[page][pos*8 + 8] = DotRow;
		}

		// buffer
		lcdCharIndex = m_lcdBackBuffer.Chars[pos];
		for (row = 0; row < DOTROWS; row++)
			m_lcdPixDbg[chTOTAL][pos*8 + row] = m_RenderedFont[lcdCharIndex].dotLine[row];
		// Show on row 8 the saved cursor position
		DotRow = (m_lcdCursor.SavedPos == pos)?0x1F:0;
		m_lcdPixDbg[chTOTAL][pos*8 + 8] = DotRow;
	}


}
#endif
//--------------------------------------------------------------------------------------------------------------------------------------------
void esq2x16_sq1_device::lcd_reset()
{
	m_lcd_command = 0;

	lcdCursor_Reset();
	memset(&m_lcdScreen,     0,sizeof(sLcdPage));
	memset(&m_lcdBackBuffer, 0,sizeof(sLcdPage));
}

//--------------------------------------------------------------------------------------------------------------------------------------------
void esq2x16_sq1_device::Blink_Reset(const bool Restart)
{
	m_blinker_on = true;
	if (Restart) {
		blinker_timer->adjust(attotime::zero, 0, attotime::from_msec(300));
//		printf("Blink RESET\n");
	}else{
//		printf("Blink STOP\n");
	}
	update_leds();
	update_display();
}

//--------------------------------------------------------------------------------------------------------------------------------------------
void esq2x16_sq1_device::Blink_Restart()
{
	blinker_timer->adjust(attotime::zero, 0, attotime::from_msec(300));
//	printf("Blink RESTART\n");
}

//--------------------------------------------------------------------------------------------------------------------------------------------
bool esq2x16_sq1_device::lcd_write(const char NewChar)
{
	bool result = false;
	if (m_lcdCursor.Pos<LCDCHARS)
	{
		if (
		  m_lcdScreen.Chars[m_lcdCursor.Pos] != NewChar
		  ||
		  m_lcdScreen.Attributes[m_lcdCursor.Pos] != m_lcdCursor.Attr
		)
		{
			m_lcdScreen.Chars[m_lcdCursor.Pos] = NewChar;
			m_lcdScreen.Attributes[m_lcdCursor.Pos] = m_lcdCursor.Attr;
			lcd_is_dirty = true;
			result = true;
		}
	}
	return result;
}

//--------------------------------------------------------------------------------------------------------------------------------------------
















//--------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------
esq2x16_sq1_device::esq2x16_sq1_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock) :
	esqvfd_device(mconfig, ESQ2X16_SQ1, tag, owner, clock, make_dimensions<2, 16>(*this)),
	blinker_timer(nullptr), m_blinker_on(false),
#if DEBUG_LCD
	m_lcdPixDbg(*this, "pgDbg_%u%03u", 1U, 0U),	// pg_n000 is referred in the layout file
#endif
	m_lcdPix(*this, "pg_1%03u", 0U),	// pg_n000 is referred in the layout file
	m_leds(*this, "rLed_%02u", 0U)			// rLed_n is referred in the layout file
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------
void esq2x16_sq1_device::device_start()
{
	esqvfd_device::device_start();
	m_lcdPix.resolve();
	m_leds.resolve();

	PreRenderFonts();

	blinker_timer = timer_alloc(FUNC(esq2x16_sq1_device::blinker_tick), this);
	blinker_timer->enable();
#if DEBUG_LCD
	m_lcdPixDbg.resolve();
#endif
}
//--------------------------------------------------------------------------------------------------------------------------------------------
void esq2x16_sq1_device::device_reset()
{
	lcdCursor_Reset();
	memset(&m_lcdScreen     , 0, sizeof(sLcdPage)   );
	memset(&m_lcdBackBuffer , 0, sizeof(sLcdPage)   );
	memset(&m_ledsStatus    , 0, sizeof(m_ledsStatus) );
	m_lcd_command = 0;
	for (auto &led : m_leds) led = 0;
	m_blinker_on = false;
	blinker_timer->enable();
}

//--------------------------------------------------------------------------------------------------------------------------------------------
//  blink_tick - update blinking LEDs / LCD Chars
//--------------------------------------------------------------------------------------------------------------------------------------------
TIMER_CALLBACK_MEMBER(esq2x16_sq1_device::blinker_tick)
{
//	printf("Time! %s \n", (m_blinker_on?"+":"-") );
	m_blinker_on = ! m_blinker_on;
	update_leds();
	update_display();
}

//--------------------------------------------------------------------------------------------------------------------------------------------
void esq2x16_sq1_device::PreRenderFonts()
{
	// Pre-render the dotmatrix chars
	char lcdCharRow;
	for (int nChar=0; nChar<DOTCHARS; nChar++) {
		// stealed from tecnbras.cpp and modified
		for (int rr = 0; rr < 8; rr++)
		{
			lcdCharRow = 0x1F & rotate_lcd_char(nChar,rr);
			m_RenderedFont[nChar].dotLine[rr] = lcdCharRow;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------
char esq2x16_sq1_device::rotate_lcd_char(const uint8_t lcdCharIndex, const int charRow)
{
	char lcdCharRow = 0;
	for (int cc=0; cc<DOTCOLS; cc++){
		lcdCharRow |= BIT(Font5x7[lcdCharIndex][cc], charRow) ? (1 << (cc)) : 0;
	}
	return lcdCharRow;
}
