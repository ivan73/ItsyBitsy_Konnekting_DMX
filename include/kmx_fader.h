/*
 * LEDFader.h
 *
 *  Created on: Sep 24, 2013
 *      Author: jeremy
 */ 

 #include "Arduino.h"
 #include "LXSAMD21DMX.h"

#ifndef KMX_FADER_H_
#define KMX_FADER_H_

// The minimum time (milliseconds) the program will wait between LED adjustments
// adjust this to modify performance.
#define MIN_INTERVAL 20

class kmx_fader {
	public:
	// Who likes dealing with function pointers? (Ok, I do, but no one else does)
	typedef uint8_t (*curve_function)(uint8_t);
	private:
	unsigned int _channel;
	unsigned long last_step_time;
	unsigned int interval;
	uint8_t color;
	uint8_t to_color;
	uint8_t last_value;
	unsigned int duration;
	float percent_done;
	curve_function curve;

	public:
	
	// Create a new LED Fader for a pin
	kmx_fader(unsigned int DMX_channel = 0);

	// Set the DMX-Channel that the LED is connected to
	void set_channel(unsigned int DMX_channel);
	uint8_t get_channel();

	// Set an LED to an absolute PWM value
	void set_value(int pwm);

	// Get the current LED PWM value
	uint8_t get_value();

	// Get the PWM value we're fading to
	uint8_t get_target_value();
	
	// Set curve to transform output
	void set_curve(curve_function);
	
	// Get the current curve function pointer
	curve_function get_curve();

	// Fade an LED to a PWM value over a duration of time (milliseconds)
	void fade(uint8_t pwm, unsigned int time);


	// Returns TRUE if there is an active fade process
	bool is_fading();

	// Stop the current fade where it's at
	void stop_fade();

	// Update the LEDs along the fade
	// Returns TRUE if a fade is still in process
	bool update();

	// Decrease the current fading speed by a number of milliseconds
	void slower(int by_seconds);

	// Increase the current fading speed by a number of milliseconds
	void faster(int by_seconds);

	// Returns how much of the fade is complete in a percentage between 0 - 100
	uint8_t get_progress();
};


#endif /* KMX_FADER_H_ */