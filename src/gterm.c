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
		write(STDOUT_FILENO, ALT_BUFF, sizeof(ALT_BUFF));
	}
	else if((set_flags & TERM_OG_BUFFER) && !(set_flags & TERM_ALT_BUFFER))
	{
		//	Prints the ansi code to return to the main buffer.
		//	Also prints the null terminator, if that matters
		write(STDOUT_FILENO, OG_BUFF, sizeof(OG_BUFF));
	}

	if(set_flags & TERM_CLEAR)
	{
		//	Prints the ansi code to clear the terminal.
		//	Also prints the null terminator, if that matters
		write(STDOUT_FILENO, CLR_SCRN, sizeof(CLR_SCRN));
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