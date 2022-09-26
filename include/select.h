#include "gterm.h"
#include "color.h"
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

//prints a set of selections
void printSelections(int count,int high,char* color,char** selections);

int makeSelection(int count,char* color,char** selections);

int makeSelectionInt(int min, int max, char* color);

int makeSelectionComplex(int count,int len,char* color,...);
