#include <stdint.h>
#include "Display.h"
#include "Multiplexer.h"
#include "SysTick.h"
#include "GPIO.h"




// Define active high/low pins
#define DATA_LENGTH		10
#define DISPLAY_NUM		4
#define DISLAY_PIN_NUM				7		// Number of pins
#define OFF		0			// Define active high/low pins
#define  ON		1


// Pins being multiplexed
static const int pins[] = {PORTNUM2PIN(PC,2),PORTNUM2PIN(PC,0),PORTNUM2PIN(PA,2),PORTNUM2PIN(PC,5), PORTNUM2PIN(PC,3), PORTNUM2PIN(PC,7), PORTNUM2PIN(PB,23)};
static const int dotPin = PORTNUM2PIN(PC,9);
static const uint8_t numCode[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x67};
static char data[DATA_LENGTH];
static int muxActivePin;
static int dataIndex; // Index
static int dataCursor;
static int brigthnessLevel;
static int brightnessCounter;






//                Funciones estaticas
//Additional functions
static void write7SegDisplay(uint8_t c);
// Interrupt subroutine called to advance multiplexer
static void displayPISR(void);


void initDisplay(void)
{
	for(int i=0; i < 7; i++){
		pinMode(pins[i],OUTPUT);
		digitalWrite(pins[i],OFF);
	}
	// Register the periodic callback
	sysTickAddCallback(&displayPISR,(float)(1/(float)MUX_FREQUENCY));

}


// Multiplexer initialization

// Interrupt subroutine called to switch multiplexer
void displayPISR(void)
{
	muxActivePin = getActivePin();
	static uint8_t c;
	if((muxActivePin >= 0) & (muxActivePin < dataCursor)){
		c = numCode[data[dataIndex + muxActivePin] - '0'];
	}else{
		c = 0x00;
	}
	write7SegDisplay(c);
}


/*******************************************************************************
 * SERVICES
 ******************************************************************************/
void putchar(char c){
	if(dataIndex < DATA_LENGTH - DISPLAY_NUM){
		data[dataIndex + dataCursor] = c;
		if(dataCursor  + 1 <= DISPLAY_NUM){
			dataCursor ++;
		}else{
			dataIndex ++;
		}
	}

}

void erase(void){
	if(dataIndex > 0){
		dataIndex --;
		data[dataIndex + dataCursor] = 0x00;
	}else if(dataCursor > 0 ){
		dataCursor --;
		data[dataCursor] = 0x00;
	}

}
void dispClear(void){
	for(int i = 0; i < dataIndex + dataCursor;  i++){
		data[i] = 0;
	}
	dataIndex = 0;
	dataCursor = 0;
}


/*******************************************************************************
 * ADDITIONAL FUNCTIONS
 ******************************************************************************/
void write7SegDisplay(uint8_t c){
	static int bitMessi;
	for(int i = 0; i < DISLAY_PIN_NUM; i++){
		bitMessi = (((c) >> i) & 0x01);
		digitalWrite(pins[i], bitMessi);
	}
	if((muxActivePin == 0)){
		bitMessi = 0;
	}
}

int timeCount(int callCount){
	return callCount * 1/MUX_FREQUENCY;
}

