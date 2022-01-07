
/*
 *  LED configuration
 * 
 *    |3|     |4|
 * 
 *    |2| |7| |5|
 * 
 *    |1|     |6|
 *
 */


/* LED Pins */
#define LED_PIN1 1  // LED 1 and 4
#define LED_PIN2 3  // LED 2 and 5
#define LED_PIN3 2  // LED 3 and 6
#define LED_PIN4 0  // LED 7

/* Pin du capteur de secousse */
#define SENSOR_PIN A2

/* Temps durant lequel le résultat reste affiché (en millisecondes) */
#define RESULT_TIMEOUT 5000


#include <avr/sleep.h>


void setup()
{
	// LED pins are outputs
	DDRB |= _BV(LED_PIN1) | _BV(LED_PIN2) | _BV(LED_PIN3) | _BV(LED_PIN4);

	// Sensor pin is an input
	DDRB &= ~_BV(SENSOR_PIN);

	// All outputs are low, internal pull-ups are disconnected
	PORTB = 0x00;

	// Sleep mode is enabled and set to power-down
	MCUCR |= _BV(SE);
	MCUCR |= _BV(SM1);
	MCUCR &= ~_BV(SM0);

	// Initialize random generator
	randomSeed(analogRead(SENSOR_PIN));

	// Be a dice, do your thing ;)
	animateDice();

	// Enter sleep mode indefinitely
	sleep_cpu();
}


void loop() {}


void animateDice() 
{
	char al = 22 + random(5);
	float as = 1.6 + (random(41) / 100);

	for (char i = 1; i < al; i++)
	{
		showNumber(getRandomNumber(), pow(i, as));
		delay(50);
	}

	showNumber(getRandomNumber(), RESULT_TIMEOUT);
}


/* Returns a number from 1 to 6 which is always different from the previous one */
char getRandomNumber()
{
	static char previous = 0;
	char number;
	
	do {
		number = random(1, 7);
	} while (number == previous);
	
	previous = number;
	
	return number;
}


/* Shows "number" on the dice for "duration" milliseconds */
void showNumber(char number, unsigned int duration)
{
	switch (number)
	{
		case 1: PORTB |= _BV(LED_PIN4); break;
		case 2: PORTB |= _BV(LED_PIN1); break;
		case 3: PORTB |= _BV(LED_PIN1) | _BV(LED_PIN4); break;
		case 4: PORTB |= _BV(LED_PIN1) | _BV(LED_PIN3); break;
		case 5: PORTB |= _BV(LED_PIN1) | _BV(LED_PIN3) | _BV(LED_PIN4); break;
		case 6: PORTB |= _BV(LED_PIN1) | _BV(LED_PIN2) | _BV(LED_PIN3); break;
		default: return;
	}
	
	delay(duration);
	
	PORTB &= ~(_BV(LED_PIN1) | _BV(LED_PIN2) | _BV(LED_PIN3) | _BV(LED_PIN4));
}
