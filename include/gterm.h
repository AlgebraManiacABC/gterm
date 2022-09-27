#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <stdio.h>

#define TERM_NO_MASK	0b000000000
#define TERM_BUFFERED	0b000000001
#define TERM_UNBUFFERED	0b000000010
#define TERM_ECHO_ON	0b000000100
#define TERM_ECHO_OFF	0b000001000
#define TERM_CURSOR_ON	0b000010000
#define TERM_CURSOR_OFF	0b000100000
#define TERM_CLEAR		0b001000000
#define TERM_ALT_BUFFER	0b010000000
#define TERM_OG_BUFFER	0b100000000

#define ESC '\x1b'
#define CSI "\x1b["

#define UP CSI"A"
#define DOWN CSI"B"
#define RIGHT CSI"C"
#define LEFT CSI"D"
#define HOME CSI"H"

#define DIR_UP 'A'
#define DIR_DOWN 'B'
#define DIR_RIGHT 'C'
#define DIR_LEFT 'D'

#define A_UP 'A'
#define A_DOWN 'B'
#define A_RIGHT 'C'
#define A_LEFT 'D'

//	Clears the screen when printed
#define CLR_SCRN "\x1b[2J"
//	Sets the terminal to the alternate buffer when printed
#define ALT_BUFF "\x1b[?1049h"
//	Returns the terminal to the original buffer when printed
#define OG_BUFF  "\x1b[?1049l"
//	Hides the cursor when printed
#define CURSOR_OFF "\x1b[?25l"
//	Shows the cursor when printed
#define CURSOR_ON "\x1b[?25h"

typedef unsigned int Uint32;

typedef struct winsize win;

/**
 * Tells if there are characters waiting to be read 
 * 
 * @return byte count in buffer
 */
int kbhit();

/**
 * Alters the terminal depending on set bits in set_flags:
 * 
 * set_flags can be any of the following OR'd together (with exceptions):
 * 
 * - TERM_UNBUFFERED: input will immediately be sent (not compatible with TERM_BUFFERED)
 * - TERM_BUFFERED: input will be sent only at newlines or end of buffer (not compatible with TERM_UNBUFFERED)
 * - TERM_ECHO_ON: input will echo (not compatible with TERM_ECHO_OFF)
 * - TERM_ECHO_OFF: input will not be echoed (not compatible with TERM_ECHO_ON)
 * - TERM_ALT_BUFFER: sets the terminal to the alternate buffer (not compatible with TERM_OG_BUFFER)
 * - TERM_OG_BUFFER: returns to the original buffer (not compatible with TERM_ALT_BUFFER)
 * - TERM_CURSOR_OFF: hides the cursor (not compatible with TERM_CURSOR_ON)
 * - TERM_CURSOR_ON: shows the cursor (not compatible with TERM_CURSOR_OFF)
 * - TERM_CLEAR: clears the screen
 */
void setTerm(Uint32 set_flags);

/**
 * Moves the cursor in the given direction n times 
 * 
 * @param dir one of the four directional definitions: DIR_UP, DIR_DOWN, DIR_RIGHT, or DIR_LEFT
 * @param n the number of characters to move across in the given direction
 */
void moveCursor(char dir, int n);

/**
 * Instantly moves the cursor to the given row/column 
 * 
 * @param row cursor's destination row (top-most row is 1)
 * @param col cursor's destination column (left-most column is 1)
 */
void teleportCursor(int row, int col);

/**
 * Gets the current terminal size and stores it within w
 * 
 * @param w the winsize struct (win) to be filled with terminal size info
 * 
 * @return 0 on success, -1 on failure
 */
int getWinsize(win * w);

/**
 * Sets the current terminal size 
 * 
 * @param rows how tall the terminal should be
 * @param cols how wide the terminal should be
 * 
 * @return 0 on success, -1 on fail
 */
int setWinsize(int rows, int cols);