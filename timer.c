#include "ch32fun.h"

#define PIN_GATE PC2

int main( void )
{
	SystemInit();
	funGpioInitAll();

	funPinMode( PIN_GATE, GPIO_Speed_10MHz | GPIO_CNF_OUT_PP );

	while(1)
	{
		funDigitalWrite( PIN_GATE, FUN_HIGH );
		Delay_Ms( 500 );
		funDigitalWrite( PIN_GATE, FUN_LOW );
		Delay_Ms( 500 );
	}
}
