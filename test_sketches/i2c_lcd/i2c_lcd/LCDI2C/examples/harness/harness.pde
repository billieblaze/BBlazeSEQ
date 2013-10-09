// example use of LCDI2C library with the web4robot.com i2C lcd display
// Plot bargraphs showing the current A/D readings
// Sample Code by Ran Talbott

#include <Wire.h>
#include <LCDI2C.h>

// Candidates for addition to the library


#define VERSION_STRING "harness V0.5"

// Subtest durations (in milliseconds)
#define BANNER_DURATION 2000
#define HORIZONTAL_DURATION 30000
#define VERTICAL_DURATION 30000
#define NUM_SUBTESTS 4
#define TESTID_BANNER	0
#define TESTID_H_BAR	1
#define TESTID_H_LINE	2
#define TESTID_V_BAR	3

long Runtime[NUM_SUBTESTS] = {
			BANNER_DURATION, 
			HORIZONTAL_DURATION, 
			HORIZONTAL_DURATION, 
			VERTICAL_DURATION
			} ;

// Assume 16 character lcd

#define MINIMUM_LCD_ROWS    1
#define MAXIMUM_LCD_ROWS    4
#define DEFAULT_LCD_ROWS    2
#define MINIMUM_LCD_COLUMNS 16
#define MAXIMUM_LCD_COLUMNS 40
#define DEFAULT_LCD_COLUMNS 16

byte Num_rows = DEFAULT_LCD_ROWS;
byte Num_cols = DEFAULT_LCD_COLUMNS;

byte Lcd_brightness;
byte Lcd_contrast;

#define LCD_I2C_ADDRESS 0x4C

LCDI2C lcd = LCDI2C(Num_rows,Num_cols, LCD_I2C_ADDRESS,0);   // Number of lines, columns and i2c address of the display,display type

int X_delay = 0;	// Used to determine by experimentation how much to delay in various places 

#define SER_STATE_WAIT_CMD		1
#define SER_STATE_WAIT_INDEX	2
#define SER_STATE_WAIT_DIGIT	3

#define SER_CMD_SET_ROWS		'h'
#define SER_CMD_SET_COLS		'w'
#define SER_CMD_SET_CONTRAST	'c'
#define SER_CMD_SET_BL_BRITE	'b'
#define SER_CMD_SET_RUNTIME		'r'
#define SER_CMD_SET_D_SOURCE	's'
#define SER_CMD_SET_D_VALUE		'd'
#define SER_CMD_SET_X_DELAY		'x'
#define SER_CMD_GET_VERSION		'v'
#define SER_CMD_GET_FW_VERSION	'f'
#define SER_CMD_GET_TIME		't'
#define SER_CMD_PRINT_ALL		'a'

#define SERIAL_TIMEOUT_MS	10000

byte Serial_cmd;
byte Serial_state = SER_STATE_WAIT_CMD;
byte Serial_index;	// Which element of an array of parameters to set/get
long Serial_operand;
const char Hex_tab[17] = "0123456789ABCDEF";


void setup()
{ 

	Serial.begin(9600);
  lcd.init();                          // Init the display, clears the display

}


void loop()
{
	do_banner();
	Fancy_Clear();
	if (Serial.available())
		process_config_command();
	do_horizontal_bar();
	Fancy_Clear();
	if (Serial.available())
		process_config_command();
	do_horizontal_line();
	Fancy_Clear();
	if (Serial.available())
		process_config_command();
	do_vertical();
	Fancy_Clear();
}


	/*
	 * Display a simple text message
	 */
void do_banner()
{
	delay(X_delay);
	lcd.printstr(VERSION_STRING);
//	print_str(VERSION_STRING);
	delay(Runtime[TESTID_BANNER]);
}

	/*
	 * Do up to 4 rows of bar graphs displaying the readings on the 
	 * corresponding A/D channels
	 */
void do_horizontal_bar()
{
	int i, adc, pixels, my_rows;
	long started_at = millis();

	lcd.init_bargraph(LCDI2C_HORIZONTAL_BAR_GRAPH);
	my_rows = min(Num_rows, 4);		// Can't do more than 4 A/D pins
	while ((millis() - started_at < Runtime[TESTID_H_BAR])
				&& !Serial.available())
		{
		for (i = 0; i < my_rows; i++)
			{
			adc = analogRead(i);
			pixels = map(adc, 0, 1023, 0, (5 * Num_cols) -1);
			lcd.draw_horizontal_graph(i, 0, Num_cols, pixels);
			delay(50);
			}
		}
}

	/*
	 * Do up to 4 rows of horizontal "line" graphs displaying the readings on the 
	 * corresponding A/D channels
	 */
void do_horizontal_line()
{
	int adc;
	byte i, j, pixels, col, previous_col[4], my_rows;
	long started_at = millis();

		// Load fake values into the "previous_col" array,  which remembers what
		// column the line was placed in on the previous pass,  so we don't
		// have to clear the entire row.  This is harmless,
		// because the screen was cleared before this routine is called,
		// but could be bad if done in a real app where the screen had
		// text on it along with the graph.
	for (i = 0; i < 4; i++)
		previous_col[i] = 0;
	lcd.init_bargraph(LCDI2C_HORIZONTAL_LINE_GRAPH);
	my_rows = min(Num_rows, 4);		// Can't do more than 4 A/D pins
	while ((millis() - started_at < Runtime[TESTID_H_LINE])
				&& !Serial.available())
		{
		for (i = 0; i < my_rows; i++)
			{
			adc = analogRead(i);
			pixels = map(adc, 0, 1023, 0, (5 * Num_cols) -1);
			col = pixels / 5;
			if (col != previous_col[i])
				{
					// Clear the previous graph.  Do this only if we're
					// going to plot to a different column,  because erasing
					// a character just before it's re-written makes it
					// flicker unpleasantly.
				lcd.setCursor(i, previous_col[i]);
				lcd.print(" ");
				delay(10);
				previous_col[i] = col;
				}
					// Add 1 to the last parameter because the controller numbers
					// pixel columns from 1 to 5,  instead of 0 to 4.
			lcd.draw_horizontal_graph(i, col, 1, (pixels % 5) + 1);
			delay(50);
			}
		}
}

	/*
	 * Do 4 columns of vertical bar graphs displaying the readings on the 
	 * corresponding A/D channels
	 */
void do_vertical()
{
	int i, adc, pixels, my_rows;
	long started_at = millis();

	lcd.init_bargraph(LCDI2C_VERTICAL_BAR_GRAPH);
	while ((millis() - started_at < Runtime[TESTID_V_BAR])
				&& !Serial.available())
		{
		for (i = 0; i < 4; i++)
			{
			adc = analogRead(i);
			pixels = map(adc, 0, 1023, 0, (8 * Num_rows) - 1);
			lcd.draw_vertical_graph(Num_rows - 1, i * 4, Num_rows, pixels);
			delay(50);
			}
		}
}



// Clear the screen by zig-zagging back and forth across it

void Fancy_Clear(){
	int i, j;
	for (i = 0; i < Num_rows; i++)
		{
		if (i & 1)
			for (j = 0; j < Num_cols; j++)
				{
				lcd.setCursor(i, j);
				lcd.print(" ");
				delay(10);
				}
		  else
			for (j = (Num_cols - 1); j >= 0; j--)
				{
				lcd.setCursor(i, j);
				lcd.print(" ");
				delay(10);
				}
		}
	lcd.setCursor(0, 0);
}


void print_CRLF()
	{
	Serial.print('\r', BYTE);
	Serial.print('\n', BYTE);
	}

void print_long(long arg)
	{
	char buff[8];
	byte i, j;

	for (i = 7; i >= 0; i--)
		{
		j = arg % 10;
		buff[i] = Hex_tab[j];
		arg = arg / 10;
		if (arg == 0)
			break;
		}
	for (; i < 8; i++)
		Serial.print(buff[i], BYTE);
	}

/*
 * Update the configuration setting if the new value is valid.
 * Send back an error message if it's not.
 */
void change_setting()
	{
	byte rc;
	
	print_CRLF();

	switch (Serial_cmd)
		{
		case SER_CMD_SET_ROWS:
				if (Serial_operand < MINIMUM_LCD_ROWS)
					{
					Serial.print("Too small\r\n");
					return;
					}
				if (Serial_operand > MAXIMUM_LCD_ROWS)
					{
					Serial.print("Too big\r\n");
					return;
					}
				Num_rows = Serial_operand;
				break;
		case SER_CMD_SET_COLS:
				if (Serial_operand < MINIMUM_LCD_COLUMNS)
					{
					Serial.print("Too small\r\n");
					return;
					}
				if (Serial_operand > MAXIMUM_LCD_COLUMNS)
					{
					Serial.print("Too big\r\n");
					return;
					}
				Num_cols = Serial_operand;
				break;
		case SER_CMD_SET_BL_BRITE:
				if (Serial_operand < LCDI2C_MIN_BRIGHTNESS)
					{
					Serial.print("Too small\r\n");
					return;
					}
				if (Serial_operand > LCDI2C_MAX_BRIGHTNESS)
					{
					Serial.print("Too big\r\n");
					return;
					}
				if ((rc = lcd.set_backlight_brightness(Serial_operand)) != 0)
					{
					Serial.print("Error ");
					Serial.print(rc);
					Serial.print("\r\n");
					return;
					}
				Lcd_brightness = Serial_operand;
				break;
		case SER_CMD_SET_CONTRAST:
				if (Serial_operand < LCDI2C_MIN_CONTRAST)
					{
					Serial.print("Too small\r\n");
					return;
					}
				if (Serial_operand > LCDI2C_MAX_CONTRAST)
					{
					Serial.print("Too big\r\n");
					return;
					}
				if ((rc = lcd.set_contrast(Serial_operand)) != 0)
					{
					Serial.print("Error ");
					Serial.print(rc);
					Serial.print("\r\n");
					return;
					}
				Lcd_contrast = Serial_operand;
				break;
		case SER_CMD_SET_RUNTIME:
				Runtime[Serial_index] = Serial_operand * 1000;
				break;
		case SER_CMD_SET_X_DELAY:
				X_delay = Serial_operand;
				break;
		case SER_CMD_SET_D_SOURCE:
		case SER_CMD_SET_D_VALUE:
				Serial.print("Not implemented yet\r\n");
				return;
		}
		// Configuration changed.  Save it and report back.
	Serial.print("OK\r\n");
	}

	
/*
 * Display the value of the queried configuration setting.
 */
void print_setting()
	{
	int i;	// Avoid bogus sign extension

	switch (Serial_cmd)
		{
		case SER_CMD_GET_VERSION:
				Serial.print(VERSION_STRING);
				break;
			// This command doesn't send any response over the serial port
		case SER_CMD_GET_FW_VERSION:
				lcd.clear();
				lcd.command(0x06);
                                delay(5000);  // Give user time to read it
				break;
		case SER_CMD_SET_ROWS:
				Serial.print("Rows=");
				Serial.print(Num_rows);
				break;
		case SER_CMD_SET_COLS:
				Serial.print("Width=");
				Serial.print(Num_cols);
				break;
		case SER_CMD_SET_CONTRAST:
				Serial.print("Contrast=");
				i = Lcd_contrast;
				Serial.print(i);
				break;
		case SER_CMD_SET_BL_BRITE:
				Serial.print("Brightness=");
				i = Lcd_brightness;
				Serial.print(i);
				break;
		case SER_CMD_SET_RUNTIME:
				Serial.print("Runtime[");
				i = Serial_index;
				Serial.print(i);
				Serial.print("]=");
				i = Runtime[Serial_index] / 1000;
				Serial.print(i);
				break;
		case SER_CMD_GET_TIME:
				Serial.print("Ticks=");
				print_long(millis());
				break;
		case SER_CMD_SET_X_DELAY:
				Serial.print("X_delay=");
				Serial.print(X_delay);
				break;
		}
	print_CRLF();
	}

/*
 * Display all of the configuration settings.
 */
void print_all_settings()
	{
	byte i;

	print_CRLF();
	Serial_cmd = SER_CMD_GET_VERSION;
	print_setting();
	Serial_cmd = SER_CMD_SET_ROWS;
	print_setting();
	Serial_cmd = SER_CMD_SET_COLS;
	print_setting();
	Serial_cmd = SER_CMD_GET_TIME;
	print_setting();
	Serial_cmd = SER_CMD_SET_BL_BRITE;
	print_setting();
	Serial_cmd = SER_CMD_SET_CONTRAST;
	print_setting();
	Serial_cmd = SER_CMD_SET_X_DELAY;
	print_setting();
	Serial_cmd = SER_CMD_SET_RUNTIME;
	for (i = 0; i < NUM_SUBTESTS; i++)
		print_setting();
	}

byte set_index(byte c)
{
	switch (Serial_cmd)
		{
		case SER_CMD_SET_RUNTIME:
				if (c < '0')
					return LCDI2C_VALUE_OUT_OF_RANGE;
				c -= '0';
				if (c >= NUM_SUBTESTS)
					return LCDI2C_VALUE_OUT_OF_RANGE;
				break;
		case SER_CMD_SET_D_SOURCE:
		case SER_CMD_SET_D_VALUE:
				return LCDI2C_VALUE_OUT_OF_RANGE;	// Not done yet
				break;
		}
	return 0;
}



/*
 * When a serial command comes in,  suspend normal processing,  and
 * read the entire command.  To avoid having the system get hung by
 * noise on the serial port or other errors, exit this routine if
 * invalid characters are received,  a command is complete,  or
 * no serial data comes in for SERIAL_TIMEOUT_MS mS.
 */
void process_config_command()
	{
	byte c;
	long last_read_time;

	c = Serial.read();
	last_read_time = millis();
	while (1)
		{
			// Echo the character,  if printable
		if ((c >= ' ') && (c < 0x80))
			Serial.print(c, BYTE);
		switch(Serial_state)
			{
			case SER_STATE_WAIT_CMD:
					switch (c)
						{
						case SER_CMD_PRINT_ALL:
								print_all_settings();
								Serial_state = SER_STATE_WAIT_CMD;
								return;
						case SER_CMD_GET_VERSION:
						case SER_CMD_GET_FW_VERSION:
						case SER_CMD_GET_TIME:
								Serial_cmd = c;
								print_setting();
								Serial_state = SER_STATE_WAIT_CMD;
								return;
						case SER_CMD_SET_ROWS:
						case SER_CMD_SET_COLS:
						case SER_CMD_SET_CONTRAST:
						case SER_CMD_SET_BL_BRITE:
						case SER_CMD_SET_X_DELAY:
								Serial_state = SER_STATE_WAIT_DIGIT;
								Serial_cmd = c;
								Serial_operand = 0;
								break;
						case SER_CMD_SET_RUNTIME:
						case SER_CMD_SET_D_VALUE:
								Serial_state = SER_STATE_WAIT_INDEX;
								Serial_cmd = c;
								Serial_operand = 0;
								break;
						default:
									// Invalid command
								print_CRLF();
								Serial.print("????");
								print_CRLF();
								Serial_state = SER_STATE_WAIT_CMD;
								return;
						}
					break;
			case SER_STATE_WAIT_INDEX:
					if (set_index(c))
						{
							// Invalid value
						print_CRLF();
						Serial.print("????");
						print_CRLF();
						Serial_state = SER_STATE_WAIT_CMD;
						return;
						}
					  else
						{
						Serial_index = c - '0';
						Serial_state = SER_STATE_WAIT_DIGIT;
						}
					break;
			case SER_STATE_WAIT_DIGIT:
					if (c == '?')
						{
						print_CRLF();
						print_setting();
						Serial_state = SER_STATE_WAIT_CMD;
						return;
						}
					if ((c == '\r') || (c == '\n'))
						{
						change_setting();
						Serial_state = SER_STATE_WAIT_CMD;
						return;
						}
					if ((c >= '0') && (c <= '9'))
						{
						Serial_operand *= 10;
						Serial_operand += (c - '0');
						}
					else
						{	// Invalid character
						print_CRLF();
						Serial.print("????");
						print_CRLF();
						Serial_state = SER_STATE_WAIT_CMD;
						}
					break;
			}
			// Wait for the next character
		while (((millis() - last_read_time) < SERIAL_TIMEOUT_MS) && !Serial.available());
		if (Serial.available())
			{
			c = Serial.read();
			last_read_time = millis();
			}
		  else
			{	// Too long between characters.  Reset the command engine
			Serial.print("\r\nTimeout\r\n");
			Serial_state = SER_STATE_WAIT_CMD;
			return;
			}
		}
	}

// Candidates for addition to the library

