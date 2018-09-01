#include "Keyboard.h"
#include "GPIO.h"
#include "Systick.h"
#include "Multiplexer.h"
#include "CircularBuffer.h"

// Keyboard size
#define NCOLS 3U
#define NROWS 4U
// Define pins
#define COL0 PORTNUM2PIN(PB, 19)
#define COL1 PORTNUM2PIN(PC, 17)
#define COL2 PORTNUM2PIN(PB, 18)
// Define active high/low keys
#define PRESSED 1
#define UNPRESSED 0

#define BUFFER_SIZE 5


// Interrupt subroutine called to multiplex the keyboard
static void keyboardPISR(void);


static int COLS[] = {COL0,COL1,COL2};
static int kbStatus[NROWS][NCOLS];
static char kbChars[NROWS][NCOLS] = {{'1','2','3'},
									 {'4','5','6'},
									 {'7','8','9'},
									 {'*','0','#'}};

NEW_CIRCULAR_BUFFER(buffer,BUFFER_SIZE,sizeof(KeyboardEvent));


void initKeyboard(void)
{
	// Set inputs
	for(int i=0; i<NCOLS; i++)
		pinMode(COLS[i],INPUT_PULLDOWN);
	// Set all keys unpressed
	for(int r=0; r<NROWS; r++)
		for(int c=0; c<NCOLS; c++)
			kbStatus[r][c]=UNPRESSED;

	sysTickAddCallback(&keyboardPISR,(float)(1/(float)MUX_FREQUENCY));
}


static void keyboardPISR(void)
{
	// Active row
	int r = getActivePin();
	// Read columns
	for(int c=0; c<NCOLS; c++)
	{
		uint8_t currStatus = digitalRead(COLS[c]);
		if(currStatus != kbStatus[r][c])
		{
			KeyboardEvent ev = {.charCode = kbChars[r][c],
						.row=r ,
						.col=c};

			if(currStatus==PRESSED)
				ev.type=KB_KEY_DOWN;
			else
				ev.type=KB_KEY_UP;

			push(&buffer,&ev);
			kbStatus[r][c] = currStatus;
		}
	}

}

KeyboardEvent getKeyboardEvent()
{
	KeyboardEvent ev;
	if(pop(&buffer,&ev)==false)
		ev.type=KB_NO_EVT;
	return ev;
}

char getchar()
{
	KeyboardEvent ev;
	// Ignore all KB_KEY_UP events
	do{
		if(pop(&buffer,&ev)==false) // QUE DEVOLVER SI NO HAY NADA EN EL BUFFER?
			break;
	}
	while(ev.type!=KB_KEY_DOWN);

	return ev.charCode;
}

bool keyboardHit()
{
	return !isEmpty(&buffer);
}

void keyboardFlush()
{
	flush(&buffer);
}

uint8_t keyboardCount()
{
	return numel(&buffer);
}

