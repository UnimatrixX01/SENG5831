/* SENG5831 Homework 5.  Motor encoder and serial communications.             */
/*                                                                            */
/* The purpose of this program is to perform the following actions:           */
/* while (not_button_pushed) {                                                */
/*    move motor 2 rotations at medium speed using encoder                    */
/*    reverse direction                                                       */
/* }                                                                          */
/* Every 250ms, the program will also output the encoder values to a          */
/* connected computer.  Since this is a blocking operation, a computer must   */
/* be connected and reading from the serial port or the program will not      */
/* continue.                                                                  */
/*                                                                            */
/* @author Stephen Atterbury                                                  */
/* @date   03/04/2013                                                         */

#include <pololu/orangutan.h>
#include <string.h>
#include <stdio.h>

int main()
{
	const long baud_rate = 115200;      /* Serial baud rate.                   */
	const int wheel_resolution = 64;    /* Encoder counts per wheel rotation.  */
	const int motor_speed = 128;        /* Maximum speed of the motors.        */
	int direction_modifier = 1;         /* Attempt to compensate for           */
	                                    /* reverse-polarity motor connections. */
	char buffer[80];                    /* Temporary buffer.                   */
	unsigned long last_ms;              /* Last serial message.                */

	/**** Begin Function Definition ****/

	/* Initialize the encoders.  M1 values are bogus and can be ignored.       */

	encoders_init(IO_B4, IO_B3, IO_C1, IO_C0);
	encoders_get_counts_and_reset_m2();

	/* Initialize serial communication.                                        */

	serial_set_baud_rate(USB_COMM, baud_rate);
	serial_set_mode(USB_COMM, SERIAL_AUTOMATIC);
	last_ms = get_ms();

	strcpy(buffer, "Starting!\n");
	serial_send_blocking(USB_COMM, buffer, strlen(buffer));

	/* And start the motor spinning.                                           */

	set_m2_speed(motor_speed);

	/* Loop until the user presses any of the three buttons on the board.      */

	while (!get_single_debounced_button_press(ANY_BUTTON)) {
		int encoder_count = encoders_get_counts_m2();

		if (encoders_check_error_m2()) {
			strcpy(buffer, "Encoder error!\n");
			serial_send_blocking(USB_COMM, buffer, strlen(buffer));
		}

		/* If the wheel has turned an extra 1/4 turn past our thresholds, the   */
		/* motor is probably hooked up in reverse.  Compensate for it.          */

		if (direction_modifier > 0 &&
		    (encoder_count >= (2*wheel_resolution + wheel_resolution/4) ||
		     encoder_count < -(wheel_resolution/4))) {

			/* Stop the motor while we send the message over the serial line     */

			set_m2_speed(0);
			strcpy(buffer, "Detected reverse polarity\n");
			serial_send_blocking(USB_COMM, buffer, strlen(buffer));


			direction_modifier = -1;
			set_m2_speed(-motor_speed);
		}

		/* Check to see if it's time to reverse the direction of the motor.     */

		if (encoder_count >= 2*wheel_resolution) {
			set_m2_speed(-motor_speed*direction_modifier);

		}
		else if (encoder_count < 0) {
			set_m2_speed(motor_speed*direction_modifier);
		}

		/* Ocassionally print out the encoder value.                            */

		if (get_ms() - last_ms > 250) {
			sprintf(buffer, "%d\n", encoder_count);
			serial_send_blocking(USB_COMM, buffer, strlen(buffer));

			last_ms = get_ms();
		}
	}

	/* Stop the motors                                                         */

	set_m2_speed(0);
	strcpy(buffer, "Button pressed!\n");
	serial_send_blocking(USB_COMM, buffer, strlen(buffer));

	return 0;
}
