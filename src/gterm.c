#include "gterm.h"

int kbhit()
{
	int bytesWaiting;
	ioctl(STDIN_FILENO, FIONREAD, &bytesWaiting);
	return bytesWaiting;
}

void setTerm(Uint32 set_flags)
{
	//	Each bit tells setTerm to do "something"
	//	No set bits means do nothing
	if(set_flags == TERM_NO_MASK) return;

	struct termios term;
	tcgetattr(STDIN_FILENO, &term);

	if ((set_flags & TERM_BUFFERED) && !(set_flags & TERM_UNBUFFERED))
	{
		term.c_lflag |= ICANON;
	}
	else if ((set_flags & TERM_UNBUFFERED) && !(set_flags & TERM_BUFFERED))
	{
		term.c_lflag &= ~ICANON;
	}

	if ((set_flags & TERM_ECHO_ON) && !(set_flags & TERM_ECHO_OFF))
	{
		term.c_lflag |= ECHO;
	}
	else if ((set_flags & TERM_ECHO_OFF) && !(set_flags & TERM_ECHO_ON))
	{
		term.c_lflag &= ~ECHO;
	}

	if((set_flags & TERM_ALT_BUFFER) && !(set_flags & TERM_OG_BUFFER))
	{
		//	Prints the ansi code to switch to the alternate buffer.
		//	Also prints the null terminator, if that matters
		fwrite(ALT_BUFF,sizeof(char),sizeof(ALT_BUFF)-1,stdout);
	}
	else if((set_flags & TERM_OG_BUFFER) && !(set_flags & TERM_ALT_BUFFER))
	{
		//	Prints the ansi code to return to the main buffer.
		//	Also prints the null terminator, if that matters
		fwrite(OG_BUFF,sizeof(char),sizeof(OG_BUFF)-1,stdout);
	}

	if((set_flags & TERM_CURSOR_OFF) && !(set_flags & TERM_CURSOR_ON))
	{
		//	Prints the ansi code to hide the cursor.
		//	Also prints the null terminator, if that matters
		fwrite(CURSOR_OFF,sizeof(char),sizeof(CURSOR_OFF)-1,stdout);
	}
	else if((set_flags & TERM_CURSOR_ON) && !(set_flags & TERM_CURSOR_OFF))
	{
		//	Prints the ansi code to show the cursor.
		//	Also prints the null terminator, if that matters
		fwrite(CURSOR_ON,sizeof(char),sizeof(CURSOR_ON)-1,stdout);
	}

	if(set_flags & TERM_CLEAR)
	{
		//	Prints the ansi code to clear the terminal.
		//	Also prints the null terminator, if that matters
		fwrite(CLR_SCRN,sizeof(char),sizeof(CLR_SCRN)-1,stdout);
	}

	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void moveCursor(char dir, int n)
{
	printf(CSI"%d%c",n,dir);
}

void teleportCursor(int row, int col)
{
	printf(CSI"%d;%dH", row, col);
}

// man ioctl_tty

int getWinsize(win *w)
{
	return ioctl(STDIN_FILENO, TIOCGWINSZ, w);
}

int setWinsize(int rows, int cols)
{
	win w;
	w.ws_col = cols;
	w.ws_row = rows;
	return ioctl(STDIN_FILENO, TIOCSWINSZ, &w);
}