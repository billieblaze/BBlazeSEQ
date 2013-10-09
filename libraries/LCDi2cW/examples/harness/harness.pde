// example use of LCDI2C library with the web4robot.com i2C lcd display
// Plot bargraphs showing the current A/D readings
// Sample Code by Ran Talbott / Edited by Dale Wentz for library verison 1.0

// Only runs on the web4robot display

#include <Wire.h>
#include <LCDi2cW.h>

#define VERSION_STRING "Harness V0.6"

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

LCDi2cW lcd = LCDi2cW(Num_rows,Num_cols, LCD_I2C_ADDRESS,0);   // Number of lines, columns and i2c address of the display,display type

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
	
	do_horizontal_bar();
	Fancy_Clear();

	do_horizontal_line();
	Fancy_Clear();

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


