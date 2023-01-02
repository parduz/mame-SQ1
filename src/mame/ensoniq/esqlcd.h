// license:BSD-3-Clause
// copyright-holders:R. Belmont, Parduz
#ifndef MAME_MACHINE_ESQLCD_H
#define MAME_MACHINE_ESQLCD_H

#pragma once

#include "esqvfd.h"


// --- SQ1 - Parduz --------------------------------------------------------------------------------------------------------------------------
#define DEBUG_LCD 0

class esq2x16_sq1_device : public esqvfd_device {

#define DOTCHARS	0x80	// How many different chars available
#define DOTROWS		7		// Number of rows of dots per char (5x7)
#define DOTCOLS		5		// Number of cols of dots per char (5x7)
#define LCDROWS		2
#define LCDCOLS		16
#define LCDCHARS	LCDROWS*LCDCOLS
#define NUMLEDS		16

public:
	esq2x16_sq1_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	virtual void write_char(int data) override;    // This here is more a "ExecuteCommand" ...- Parduz
	virtual void update_display() override;

	TIMER_CALLBACK_MEMBER(blinker_tick);           // The blinker timer callback function.

private:
	// ---- DOT CHARS -----------------------------------------------------------------------------
	// A pre-rendered dotmatrix character.
	// The 8th byte is useful only for debugging, right now it shows where the
	// virtual cursor is.
	struct sDotChar {
		uint8_t dotLine[DOTROWS+1];            // Bytes containing pre-rendered dot-matrix chars
	};
	sDotChar    m_RenderedFont[DOTCHARS]{};    // Pre-rendered dotmatrix chars
	void        PreRenderFonts();
	static char rotate_lcd_char(const uint8_t lcdCharIndex, const int charRow);
	// --------------------------------------------------------------------------------------------

	// ---- VIRTUAL CURSOR ------------------------------------------------------------------------
	// Attributes of each character on the display. The only one i'm sure about is the blinking.
	// Dunno what should be the difference between the other two, neither what the LCD should do
	// when the command to switch between the two arrives (but i think it should actually DO something)
	enum  eCharAttr {
		chNormal     ,
		chUnknown    ,
		chBlink      ,                  // Makes the characters blink

		chTOTAL                         // Useful for the check on loops ;)
	};
	struct slcdCursor{
		uint8_t    Pos;                 // Current writing position
		uint8_t    SavedPos;            // Current saved position (?)
		uint8_t    Attr;                // Current char attribute: new char will have this attr value.
		uint8_t    BlinkPos;            // Visible blinking cursor: position
		uint8_t    BlinkSavedPos;       // Visible blinking cursor: saver position (?)
		uint16_t   BlinkType;           // Visible blinking cursor: type of cursor (?)
	};
	slcdCursor  m_lcdCursor;
	inline void    lcdCursor_Move       (const uint8_t NewPos)                  {m_lcdCursor.Pos = NewPos;                    };
	inline void    lcdCursor_Attr       (const uint8_t NewAttr)                 {m_lcdCursor.Attr = NewAttr;                  };
	inline void    lcdCursor_Inc        ()                                      {++m_lcdCursor.Pos;                           };
	inline void    lcdCursor_Save       ()                                      {m_lcdCursor.SavedPos = m_lcdCursor.Pos;      };
	inline void    lcdCursor_Restore    ()                                      {m_lcdCursor.Pos = m_lcdCursor.SavedPos;      };
	inline void    lcdCursor_Reset      ()                                      {memset(&m_lcdCursor, 0, sizeof(slcdCursor)); };

	// ---- VIRTUAL SCREEN ------------------------------------------------------------------------
	struct sLcdPage {
		uint8_t    Chars[LCDCHARS];
		uint8_t    Attributes[LCDCHARS];
	};
	sLcdPage       m_lcdScreen;         // This is what you see on the screen
	sLcdPage       m_lcdBackBuffer;     // A save/restore buffer of the Screen. Actually ignores the Attributes.
	inline void    lcdScreen_Save       ()                                      {memcpy(&m_lcdBackBuffer.Chars , &m_lcdScreen.Chars    , LCDCHARS); };
	inline void    lcdScreen_Restore    ()                                      {memcpy(&m_lcdScreen.Chars     , &m_lcdBackBuffer.Chars, LCDCHARS); };
	inline void    lcdScreen_ResetAttr  ()                                      {memset(&m_lcdScreen.Attributes, chNormal, LCDCHARS);               };


	bool           lcd_write            (const char NewChar);   // Writes a single character in the screen
	void           lcd_reset            ();                     // Zeroes everything.
	bool           lcd_is_dirty = false;// Flag true when the LCD needs an update. Pretty much ALWAYS.
	int            m_lcd_command = 0;   // The command sent to the panel

	// ---- PANEL LEDS ----------------------------------------------------------------------------
	uint8_t     m_ledsStatus[NUMLEDS]{};  // Internal status of LEDs: 2=off, 1=on, 0=blink, 3=???
	void        update_leds();

	// ---- BLINKER TIMER -------------------------------------------------------------------------
	emu_timer      *blinker_timer;      // The blinker timer
	bool           m_blinker_on;        // Status of the blinker
	void           Blink_Reset          (const bool Restart=true);
	void           Blink_Restart        ();

#if DEBUG_LCD
	void           update_debug();      // updates the debug displays on the LAYout
#endif



protected:
	virtual void device_add_mconfig(machine_config &config) override;
	virtual void device_start() override;
	virtual void device_reset() override;

#if DEBUG_LCD
	output_finder<chTOTAL+1,LCDCHARS*(DOTROWS+1)> m_lcdPixDbg;   // Showing the other pages as LCDs
#endif
	output_finder<LCDCHARS*DOTROWS> m_lcdPix;              // The dot-matrix "outputs" for the LAYout
	output_finder<NUMLEDS>          m_leds;                // panel LEDs for the LAYout


};
DECLARE_DEVICE_TYPE(ESQ2X16_SQ1, esq2x16_sq1_device)
#endif // MAME_MACHINE_ESQLCD_H
