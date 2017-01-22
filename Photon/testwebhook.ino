#define SPI_SCLK_LOW_TIME 0.004
#define SPI_SCLK_HIGH_TIME 0.004
#define HIGH_VALUE true
#define LOW_VALUE false

//OLED PIN DEFINITIONS

#define SCL_OLED	A3
#define SDA_OLED	A5
#define RST_OLED	A1
#define DC_OLED		A0	
#define CS_OLED		A2

#define SCL_OLED2	D4
#define SDA_OLED2	D2
#define RST_OLED2	D0
#define DC_OLED2	D1	
#define CS_OLED2	D5

#define LED_ONE     1
#define LED_TWO     2

#define delay_ms(x) delay(x)
#define delay_us(x) delay(x/1000)


// Timing Delays
#define SSD1331_DELAYS_HWFILL		(100)
#define SSD1331_DELAYS_HWLINE       (1)

//colors
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

// SSD1331 Commands
#define SSD1331_CMD_DRAWLINE 		0x21
#define SSD1331_CMD_DRAWRECT 		0x22
#define SSD1331_CMD_FILL 			0x26
#define SSD1331_CMD_SETCOLUMN 		0x15
#define SSD1331_CMD_SETROW    		0x75
#define SSD1331_CMD_CONTRASTA 		0x81
#define SSD1331_CMD_CONTRASTB 		0x82
#define SSD1331_CMD_CONTRASTC		0x83
#define SSD1331_CMD_MASTERCURRENT 	0x87
#define SSD1331_CMD_SETREMAP 		0xA0
#define SSD1331_CMD_STARTLINE 		0xA1
#define SSD1331_CMD_DISPLAYOFFSET 	0xA2
#define SSD1331_CMD_NORMALDISPLAY 	0xA4
#define SSD1331_CMD_DISPLAYALLON  	0xA5
#define SSD1331_CMD_DISPLAYALLOFF 	0xA6
#define SSD1331_CMD_INVERTDISPLAY 	0xA7
#define SSD1331_CMD_SETMULTIPLEX  	0xA8
#define SSD1331_CMD_SETMASTER 		0xAD
#define SSD1331_CMD_DISPLAYOFF 		0xAE
#define SSD1331_CMD_DISPLAYON     	0xAF
#define SSD1331_CMD_POWERMODE 		0xB0
#define SSD1331_CMD_PRECHARGE 		0xB1
#define SSD1331_CMD_CLOCKDIV 		0xB3
#define SSD1331_CMD_PRECHARGEA 		0x8A
#define SSD1331_CMD_PRECHARGEB 		0x8B
#define SSD1331_CMD_PRECHARGEC 		0x8C
#define SSD1331_CMD_PRECHARGELEVEL 	0xBB
#define SSD1331_CMD_VCOMH 			0xBE
#define SSD1331_CMD_DRAWRECT 	    0x22

typedef struct weather
{
	uint8_t typeDay;
	int temperature;
	int windSpeed;
}Weather;

Weather	weatherObject[3];

typedef struct news
{
	char headline[51]; //source
	int currentIndex;
}News;

typedef struct time_now
{
	//Minute Hand <x,y> coordinates
	int min;
	
	//Hour Hand <x,y> coordinates
	int hour;
}TimeNow;

void spi_init()
{
	pinMode(CS_OLED, OUTPUT);
	pinMode(DC_OLED, OUTPUT);
	pinMode(RST_OLED, OUTPUT);
	pinMode(SDA_OLED, OUTPUT);
	pinMode(SCL_OLED, OUTPUT);

	pinMode(CS_OLED2, OUTPUT);
	pinMode(DC_OLED2, OUTPUT);
	pinMode(RST_OLED2, OUTPUT);
	pinMode(SDA_OLED2, OUTPUT);
	pinMode(SCL_OLED2, OUTPUT);
}

void write_MOSI(bool high, int ledNo)
{
    if(ledNo == LED_ONE)
	{
        if(high)
	    {
		    digitalWrite(SDA_OLED, HIGH);
    	}
	    else
    	{
	    	digitalWrite(SDA_OLED, LOW);
    	}
    }
    else // LED_TWO
    {
        if(high)
	    {
		    digitalWrite(SDA_OLED2, HIGH);
    	}
	    else
    	{
	    	digitalWrite(SDA_OLED2, LOW);
    	} 
    }
}

void write_SCLK(bool high, int ledNo)
{
    if(ledNo == LED_ONE)
    {
	    if(high)
	    {
	    	digitalWrite(SCL_OLED, HIGH);
	    }
	    
	    else
	    {
	    	digitalWrite(SCL_OLED, LOW);
	    }
    }
    else
    {
	    if(high)
	    {
	    	digitalWrite(SCL_OLED2, HIGH);
	    }
	    
	    else
	    {
	    	digitalWrite(SCL_OLED2, LOW);
	    }
    }
}

uint8_t SPI_transfer_byte(uint8_t byte_out, int ledNo)
{
	delay_ms(2);
	uint8_t byte_in = 0;
	uint8_t bit;
    if(ledNo == LED_ONE)
    {
	    digitalWrite(CS_OLED, HIGH);
	    delay_ms(10);
	    digitalWrite(CS_OLED, LOW); //cs low
	}
    else
    {
   	    digitalWrite(CS_OLED2, HIGH);
	    delay_ms(10);
	    digitalWrite(CS_OLED2, LOW); //cs low
    }
	for (bit = 0x80; bit; bit >>= 1)
	{
		/* Shift-out a bit to the MOSI line */
		write_MOSI( ( (byte_out & bit) ? HIGH : LOW ) , ledNo);

		/* Delay for at least the peer's setup time */
		delay_us(SPI_SCLK_LOW_TIME); //changed to us from ms
		
		/* Pull the clock line high */
		write_SCLK(HIGH_VALUE, ledNo);

		/* Shift-in a bit from the MISO line */
		//if (read_MISO() == HIGH)
		//byte_in |= bit;

		/* Delay for at least the peer's hold time */
		delay_us(SPI_SCLK_HIGH_TIME); //changed to us from ms

		/* Pull the clock line low */
		write_SCLK(LOW_VALUE, ledNo);
	}

	return byte_in;
}

void fill_color(uint16_t color, int ledNo)
{
	SPI_transfer_byte(SSD1331_CMD_FILL, ledNo);
	SPI_transfer_byte(0x01, ledNo);
	SPI_transfer_byte(SSD1331_CMD_DRAWRECT, ledNo);
	SPI_transfer_byte(0x02 & 0xFF, ledNo);							// Starting column
	SPI_transfer_byte(0x01 & 0xFF, ledNo);							// Starting row
	SPI_transfer_byte((0x02+96-1) & 0xFF, ledNo);	// End column
	SPI_transfer_byte((0x01+64-1) & 0xFF, ledNo);	// End row
	
	// Outline color
	SPI_transfer_byte((uint8_t)((color >> 11) << 1), ledNo);
	SPI_transfer_byte((uint8_t)((color >> 5) & 0x3F), ledNo);
	SPI_transfer_byte((uint8_t)((color << 1) & 0x3F), ledNo);
	// Fill color
	SPI_transfer_byte((uint8_t)((color >> 11) << 1), ledNo);
	SPI_transfer_byte((uint8_t)((color >> 5) & 0x3F), ledNo);
	SPI_transfer_byte((uint8_t)((color << 1) & 0x3F), ledNo);
	
	// Delay while the fill completes
	delay_ms(SSD1331_DELAYS_HWFILL);
}

void draw_line(uint8_t sRow, uint8_t sCol, uint8_t eRow, uint8_t eCol, uint16_t color, int ledNo)
{
	SPI_transfer_byte(SSD1331_CMD_DRAWLINE, ledNo);
	SPI_transfer_byte(sCol, ledNo);
	SPI_transfer_byte(sRow, ledNo);
	SPI_transfer_byte(eCol, ledNo);
	SPI_transfer_byte(eRow, ledNo);
	// SPI_transfer_byte(0x00);
	// SPI_transfer_byte(0x00);
	// SPI_transfer_byte(0x20);
	// SPI_transfer_byte(0x10);
	delay_ms(SSD1331_DELAYS_HWLINE);
	SPI_transfer_byte((uint8_t)((color >> 11) << 1), ledNo);
	SPI_transfer_byte((uint8_t)((color >> 5) & 0x3F), ledNo);
	SPI_transfer_byte((uint8_t)((color << 1) & 0x3F), ledNo);
	delay_ms(SSD1331_DELAYS_HWLINE);
}

void drawCharacter(char character, int sRow, int sCol, int eRow, int eCol, uint16_t color, int ledNo)
{
	int width = eCol - sCol;
	int height = eRow - sRow;

	switch(character)
	{
		case 'a':
		case 'A':
		draw_line(sRow, sCol, sRow, sCol + width, color, ledNo);               // draw top line
		draw_line(sRow, sCol, sRow + height, sCol, color, ledNo);                // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color, ledNo); // draw middle line
		draw_line(sRow, sCol + width, sRow + height, sCol + width, color, ledNo); // draw right line
		break;
		case 'b':
		case 'B':
		draw_line(sRow, sCol, sRow + height, sCol, color, ledNo);                // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color, ledNo); // draw middle line
		draw_line(sRow + height/2, sCol + width, sRow + height, sCol + width, color, ledNo); // draw right line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color, ledNo); // draw bottom line
		break;
		case 'c':
		case 'C':
		draw_line(sRow + height/2, sCol, sRow + height, sCol, color, ledNo);                // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color, ledNo); // draw middle line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color, ledNo); // draw bottom line
		break;
		case 'd':
		case 'D':
		draw_line(sRow + height/2, sCol, sRow + height, sCol, color, ledNo);                // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color, ledNo); // draw middle line
		draw_line(sRow, sCol + width, sRow + height, sCol + width, color, ledNo); // draw right line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color, ledNo); // draw bottom line
		break;
		case 'e':
		case 'E':
		draw_line(sRow, sCol, sRow, sCol + width, color, ledNo);               // draw top line
		draw_line(sRow, sCol, sRow + height, sCol, color, ledNo);                // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color, ledNo); // draw middle line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color, ledNo); // draw bottom line
		break;
		case 'f':
		case 'F':
		draw_line(sRow, sCol, sRow, sCol + width, color, ledNo);               // draw top line
		draw_line(sRow, sCol, sRow + height, sCol, color, ledNo);                // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color, ledNo); // draw middle line
		break;
		case 'g':
		case 'G':
		draw_line(sRow, sCol, sRow, sCol + width, color, ledNo);               // draw top line
		draw_line(sRow, sCol, sRow + height, sCol, color, ledNo);                // draw left line
		draw_line(sRow + height/2, sCol + width/2, sRow + height/2, sCol + width, color, ledNo); // draw middle line
		draw_line(sRow + height/2, sCol + width, sRow + height, sCol + width, color, ledNo); // draw right line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color, ledNo); // draw bottom line
		break;
		case 'h':
		case 'H':
		draw_line(sRow, sCol, sRow + height, sCol, color, ledNo);                // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color, ledNo); // draw middle line
		draw_line(sRow + height/2, sCol + width, sRow + height, sCol + width, color, ledNo); // draw right line
		break;
		case 'i':
		case 'I':
		draw_line(sRow, sCol, sRow, sCol + width, color, ledNo);               // draw top line
		draw_line(sRow, sCol + width/2, sRow + height, sCol + width/2, color, ledNo);                // draw left line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color, ledNo); // draw bottom line
		break;
		case 'j':
		case 'J':
		draw_line(sRow, sCol, sRow, sCol + width, color, ledNo);               // draw top line
		draw_line(sRow + 2*height/3, sCol + width/3, sRow + height, sCol + width/3, color, ledNo);                // draw left line
		draw_line(sRow, sCol + 2*width/3, sRow + height, sCol + 2*width/3, color, ledNo); // draw right line
		draw_line(sRow + height, sCol + width/3, sRow + height, sCol + 2*width/3, color, ledNo); // draw bottom line
		break;
		case 'k':
		case 'K':
		draw_line(sRow, sCol, sRow + height, sCol, color, ledNo);                // draw left line
		draw_line(sRow + height/2, sCol, sRow, sCol + width, color, ledNo); // draw right line
		draw_line(sRow + height/2, sCol, sRow + height, sCol + width, color, ledNo); // draw bottom line
		break;
		case 'l':
		case 'L':
		draw_line(sRow, sCol, sRow + height, sCol, color, ledNo);                // draw left line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color, ledNo); // draw bottom line
		break;
		case 'm':
		case 'M':
		draw_line(sRow, sCol, sRow + height, sCol, color, ledNo);                // draw left line
		draw_line(sRow, sCol + width, sRow + height, sCol + width, color, ledNo); // draw right line
		draw_line(sRow, sCol, sRow + height/2, sCol + width/2, color, ledNo);		// draw this(\) line
		draw_line(sRow + height/2, sCol + width/2, sRow, sCol + width, color, ledNo); // draw this(/) line
		break;
		case 'n':
		case 'N':
		draw_line(sRow, sCol, sRow + height, sCol, color, ledNo);                // draw left line
		draw_line(sRow, sCol + width, sRow + height, sCol + width, color, ledNo); // draw right line
		draw_line(sRow, sCol, sRow + height, sCol + width/2, color, ledNo);		// draw this(\) line
		break;
		case 'o':
		case 'O':
		draw_line(sRow, sCol, sRow, sCol + width, color, ledNo);               // draw top line
		draw_line(sRow, sCol, sRow + height, sCol, color, ledNo);                // draw left line
		draw_line(sRow, sCol + width, sRow + height, sCol + width, color, ledNo); // draw right line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color, ledNo); // draw bottom line
		break;
		case 'p':
		case 'P':
		draw_line(sRow, sCol, sRow, sCol + width, color, ledNo);               // draw top line
		draw_line(sRow, sCol, sRow + height, sCol, color, ledNo);                // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color, ledNo); // draw middle line
		draw_line(sRow, sCol + width, sRow + height/2, sCol + width, color, ledNo); // draw right line
		break;
		case 'q':
		case 'Q':
		draw_line(sRow, sCol, sRow, sCol + width, color, ledNo);               // draw top line
		draw_line(sRow, sCol, sRow + height, sCol, color, ledNo);                // draw left line
		draw_line(sRow + height/2, sCol + width/2, sRow + height, sCol + width, color, ledNo); // draw this(\) line
		draw_line(sRow, sCol + width, sRow + height, sCol + width, color, ledNo); // draw right line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color, ledNo); // draw bottom line
		break;
		case 'r':
		case 'R':
		draw_line(sRow, sCol, sRow, sCol + width, color, ledNo);               // draw top line
		draw_line(sRow, sCol, sRow + height, sCol, color, ledNo);                // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color, ledNo); // draw middle line
		draw_line(sRow, sCol + width, sRow + height/2, sCol + width, color, ledNo); // draw right line
		draw_line(sRow + height/2, sCol + width/2, sRow + height, sCol + width, color, ledNo); // draw this(\) line
		break;
		case 's':
		case 'S':
		draw_line(sRow, sCol, sRow, sCol + width, color, ledNo);               // draw top line
		draw_line(sRow, sCol, sRow + height/2, sCol, color, ledNo);                // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color, ledNo); // draw middle line
		draw_line(sRow + height/2, sCol + width, sRow + height, sCol + width, color, ledNo); // draw right line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color, ledNo); // draw bottom line
		break;
		case 't':
		case 'T':
		draw_line(sRow, sCol, sRow, sCol + width, color, ledNo);               // draw top line
		draw_line(sRow, sCol + width/2, sRow + height, sCol + width/2, color, ledNo);	// draw left line
		break;
		case 'u':
		case 'U':
		draw_line(sRow, sCol, sRow + height, sCol, color, ledNo);                // draw left line
		draw_line(sRow, sCol + width, sRow + height, sCol + width, color, ledNo); // draw right line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color, ledNo); // draw bottom line
		break;
		case 'v':
		case 'V':
		draw_line(sRow, sCol, sRow + height, sCol + width/2, color, ledNo);		// draw this(\) line
		draw_line(sRow + height, sCol + width/2, sRow, sCol + width, color, ledNo); // draw this(/) line
		break;
		case 'w':
		case 'W':
		draw_line(sRow, sCol, sRow + height, sCol, color, ledNo);                // draw left line
		draw_line(sRow, sCol + width, sRow + height, sCol + width, color, ledNo); // draw right line
		draw_line(sRow + height/2, sCol + width/2, sRow + height, sCol + width, color, ledNo); // draw this(\) line
		draw_line(sRow + height, sCol, sRow + height/2, sCol + width/2, color, ledNo); // draw this(/) line
		break;
		case 'x':
		case 'X':
		draw_line(sRow, sCol, sRow + height, sCol + width, color, ledNo);		// draw this(\) line
		draw_line(sRow + height, sCol, sRow, sCol + width, color, ledNo); // draw this(/) line
		break;
		case 'y':
		case 'Y':
		draw_line(sRow, sCol, sRow + height/2, sCol + width/2, color, ledNo);		// draw this(\) line
		draw_line(sRow + height, sCol, sRow, sCol + width, color, ledNo); // draw this(/) line
		break;
		case 'z':
		case 'Z':
		draw_line(sRow, sCol, sRow, sCol + width, color, ledNo);               // draw top line
		draw_line(sRow + height, sCol, sRow, sCol + width, color, ledNo); // draw this(/) line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color, ledNo); // draw bottom line
		break;
	}
}

void drawDigit(int digit, int sRow, int sCol, int eRow, int eCol, uint16_t color, int ledNo)
{
	int width  = eCol - sCol;
	int height = eRow - sRow;

	switch(digit)
	{
		case 0:
		draw_line(sRow, sCol, sRow + height, sCol, color, ledNo);                  // draw left line
		draw_line(sRow, sCol, sRow, sCol + width, color, ledNo);                   // draw top line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color, ledNo); // draw bottom line
		draw_line(sRow, sCol + width, sRow + height, sCol + width, color, ledNo);  // draw right line
		break;
		case 1:
		draw_line(sRow, sCol + 2*width/3, sRow + height, sCol + 2*width/3, color, ledNo);
		break;
		case 2:
		draw_line(sRow, sCol, sRow, sCol + width, color, ledNo);                   // draw top line
		draw_line(sRow, sCol + width, sRow + height/2, sCol + width, color, ledNo);  // draw right line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color, ledNo); // draw middle line
		draw_line(sRow + height/2, sCol, sRow + height, sCol, color, ledNo);         // draw left line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color, ledNo); // draw bottom line
		break;
		case 3:
		draw_line(sRow, sCol, sRow, sCol + width, color, ledNo);                   // draw top line
		draw_line(sRow, sCol + width, sRow + height, sCol + width, color, ledNo);  // draw right line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color, ledNo); // draw middle line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color, ledNo); // draw bottom line
		break;
		case 4:
		draw_line(sRow, sCol, sRow + height/2, sCol, color, ledNo);         // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color, ledNo); // draw middle line
		draw_line( sRow + height/3, sCol + 2*width/3, sRow + height, sCol + 2*width/3 , color, ledNo);
		break;
		case 5:
		draw_line(sRow, sCol, sRow, sCol + width, color, ledNo);               // draw top line
		draw_line(sRow, sCol, sRow + height/2, sCol, color, ledNo);                // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color, ledNo); // draw middle line
		draw_line(sRow + height/2, sCol + width, sRow + height, sCol + width, color, ledNo); // draw right line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color, ledNo); // draw bottom line
		break;
		case 6:
		draw_line(sRow, sCol, sRow, sCol + width, color, ledNo);               // draw top line
		draw_line(sRow, sCol, sRow + height, sCol, color, ledNo);                // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color, ledNo); // draw middle line
		draw_line(sRow + height/2, sCol + width, sRow + height, sCol + width, color, ledNo); // draw right line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color, ledNo); // draw bottom line
		break;
		case 7:
		draw_line(sRow, sCol, sRow, sCol + width, color, ledNo);               // draw top line
		draw_line(sRow, sCol + width, sRow + height, sCol + 2*width/3, color, ledNo); // draw right line
		break;
		case 8:
		draw_line(sRow, sCol, sRow, sCol + width, color, ledNo);               // draw top line
		draw_line(sRow, sCol, sRow + height, sCol, color, ledNo);                // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color, ledNo); // draw middle line
		draw_line(sRow, sCol + width, sRow + height, sCol + width, color, ledNo); // draw right line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color, ledNo); // draw bottom line
		break;
		case 9:
		draw_line(sRow, sCol, sRow, sCol + width, color, ledNo);               // draw top line
		draw_line(sRow, sCol, sRow + height/2, sCol, color, ledNo);                // draw left line
		draw_line(sRow + height/2, sCol, sRow + height/2, sCol + width, color, ledNo); // draw middle line
		draw_line(sRow, sCol + width, sRow + height, sCol + width, color, ledNo); // draw right line
		draw_line(sRow + height, sCol, sRow + height, sCol + width, color, ledNo); // draw bottom line
		break;
	}
}

void drawTimeMenu(int hour, int mins, int ledNo)
{
	// dimensions: 96 * 64
	//drawDigit(int digit, int sRow, int sCol, int eRow, int eCol)
	drawDigit(hour/10, 10, 10, 54, 19, RED, ledNo);
	drawDigit(hour%10, 10, 32, 54, 41, RED, ledNo);
	draw_line(25,47,27,47, RED, ledNo);
	draw_line(37,47,39,47, RED, ledNo);
	drawDigit(mins/10, 10, 54, 54, 63, RED, ledNo);
	drawDigit(mins%10, 10, 76, 54, 85, RED, ledNo);
}

void initializeOLED(int ledNo)
{
    if(ledNo == LED_ONE)
    {
        //reset
	    delay_ms(200);
	    digitalWrite(RST_OLED, HIGH); //reset high
	    delay_ms(500);

	    digitalWrite(RST_OLED, LOW); //reset low
	    delay_ms(500);
	    digitalWrite(RST_OLED, HIGH); //reset high
	    delay_ms(500);
	    //port_pin_set_output_level(RST_OLED, false); //reset high
	    //delay_ms(500);
	    digitalWrite(DC_OLED, LOW); //DC LOW COMMAND MODE
	    delay_ms(200);
	}
    else
    {
	    //reset
	    delay_ms(200);
	    digitalWrite(RST_OLED2, HIGH); //reset high
	    delay_ms(500);

	    digitalWrite(RST_OLED2, LOW); //reset low
	    delay_ms(500);
	    digitalWrite(RST_OLED2, HIGH); //reset high
	    delay_ms(500);
	    //port_pin_set_output_level(RST_OLED, false); //reset high
	    //delay_ms(500);
	    digitalWrite(DC_OLED2, LOW); //DC LOW COMMAND MODE
	    delay_ms(200);
	}	
	
	//SPI_transfer_byte(0X25, ledNo);  	// 0xAE
	SPI_transfer_byte(SSD1331_CMD_DISPLAYOFF, ledNo);  	// 0xAE
	SPI_transfer_byte(SSD1331_CMD_SETREMAP, ledNo); 	// 0xA0
	SPI_transfer_byte(0x72, ledNo); 	// rgb color
	
	SPI_transfer_byte(SSD1331_CMD_STARTLINE, ledNo); 	// 0xA1
	SPI_transfer_byte(0x0, ledNo);
	SPI_transfer_byte(SSD1331_CMD_DISPLAYOFFSET, ledNo); 	// 0xA2
	SPI_transfer_byte(0x0, ledNo);
	SPI_transfer_byte(SSD1331_CMD_NORMALDISPLAY, ledNo);  	// 0xA4
	SPI_transfer_byte(SSD1331_CMD_SETMULTIPLEX, ledNo); 	// 0xA8
	SPI_transfer_byte(0x3F, ledNo);  			// 0x3F 1/64 duty
	SPI_transfer_byte(SSD1331_CMD_SETMASTER, ledNo);  	// 0xAD
	SPI_transfer_byte(0x8E, ledNo);
	SPI_transfer_byte(SSD1331_CMD_POWERMODE, ledNo);  	// 0xB0
	SPI_transfer_byte(0x0B, ledNo);
	SPI_transfer_byte(SSD1331_CMD_PRECHARGE, ledNo);  	// 0xB1
	SPI_transfer_byte(0x31, ledNo);
	SPI_transfer_byte(SSD1331_CMD_CLOCKDIV, ledNo);  	// 0xB3
	SPI_transfer_byte(0xF0, ledNo);  // 7:4 = Oscillator Frequency, 3:0 = CLK Div Ratio (A[3:0]+1 = 1..16)
	SPI_transfer_byte(SSD1331_CMD_PRECHARGEA, ledNo);  	// 0x8A
	SPI_transfer_byte(0x64, ledNo);
	SPI_transfer_byte(SSD1331_CMD_PRECHARGEB, ledNo);  	// 0x8B
	SPI_transfer_byte(0x78, ledNo);
	SPI_transfer_byte(SSD1331_CMD_PRECHARGEA, ledNo);  	// 0x8C
	SPI_transfer_byte(0x64, ledNo);
	SPI_transfer_byte(SSD1331_CMD_PRECHARGELEVEL, ledNo);  	// 0xBB
	SPI_transfer_byte(0x3A, ledNo);
	SPI_transfer_byte(SSD1331_CMD_VCOMH, ledNo);  		// 0xBE
	SPI_transfer_byte(0x3E, ledNo);
	SPI_transfer_byte(SSD1331_CMD_MASTERCURRENT, ledNo);  	// 0x87
	SPI_transfer_byte(0x06, ledNo);
	SPI_transfer_byte(SSD1331_CMD_CONTRASTA, ledNo);  	// 0x81
	SPI_transfer_byte(0x91, ledNo);
	SPI_transfer_byte(SSD1331_CMD_CONTRASTB, ledNo);  	// 0x82
	SPI_transfer_byte(0x50, ledNo);
	SPI_transfer_byte(SSD1331_CMD_CONTRASTC, ledNo);  	// 0x83
	SPI_transfer_byte(0x7D, ledNo);
	SPI_transfer_byte(SSD1331_CMD_DISPLAYON, ledNo);	//--turn on oled panel
	//SPI_transfer_byte(0X25, ledNo);  	// 0xAE
	//SPI_transfer_byte(0X00);  	// 0xAE
	//SPI_transfer_byte(0X00);  	// 0xAE
	//SPI_transfer_byte(0X10);  	// 0xAE
	//SPI_transfer_byte(0X10);  	// 0xAE
	
	fill_color(BLACK, ledNo);
}

void getDay(char *day , int dayNum)
{
	
	if(dayNum == 1)
	{
		day[0] = 'M';
		day[1] = 'O';
		day[2] = 'N';
		day[3] = '\0';
	}
	else if(dayNum == 2)
	{
		day[0] = 'T';
		day[1] = 'U';
		day[2] = 'E';
		day[3] = '\0';
	}
	else if(dayNum == 3)
	{
		day[0] = 'W';
		day[1] = 'E';
		day[2] = 'D';
		day[3] = '\0';
	}
	else if(dayNum == 4)
	{
		day[0] = 'T';
		day[1] = 'H';
		day[2] = 'U';
		day[3] = '\0';
	}
	else if(dayNum == 5)
	{
		day[0] = 'F';
		day[1] = 'R';
		day[2] = 'I';
		day[3] = '\0';
	}
	else if(dayNum == 6)
	{
		day[0] = 'S';
		day[1] = 'A';
		day[2] = 'T';
		day[3] = '\0';
	}
	else if(dayNum == 7)
	{
		day[0] = 'S';
		day[1] = 'U';
		day[2] = 'N';
		day[3] = '\0';
	}
}	


void drawWeatherInfo(int ledNo)
{
    fill_color(BLACK, ledNo);
	int i = 0;
	char day[4];
	//day[0] = 'W';
	//day[1] = 'E';
	//day[2] = 'D';
	//day[3] = '\0';
	int increment = 0;
	//prints day
	drawCharacter(day[0],0 + increment,2 ,7 + increment,10,RED, ledNo);
	drawCharacter(day[1],9 + increment,2 ,16 + increment,10,RED, ledNo);
	drawCharacter(day[2],18 + increment,2 ,25 + increment,10,RED, ledNo);
	
	//prints temp
	drawCharacter('O',0 + increment,37,3 + increment,40,RED, ledNo);
	//drawCharacter('E',0 + increment,36,6 + increment,40,RED);
	drawCharacter('F',0 + increment,42,6 + increment,48,RED, ledNo);
	//drawCharacter('P',0 + increment,48,6 + increment,52,RED);
	
	int temperature = weatherObject[i].temperature;
	if(temperature < 0)
	{
		draw_line(15 + increment,31,15 + increment,34,CYAN, ledNo);
		temperature = temperature*(-1);
	}
	int tempOnes = temperature % 10;
	int tempTens = (temperature/10) % 10;
	//draw_line(0,0,63,95,GREEN);	// The perfect diagonal

	drawDigit(tempTens,10 + increment,36,20 + increment,39,CYAN, ledNo);
	drawDigit(tempOnes,10 + increment,41,20 + increment,44,CYAN, ledNo);

	drawCharacter('M',0 + increment,76,6 + increment,80,RED, ledNo);
	drawCharacter('P',0 + increment,82,6 + increment,86,RED, ledNo);
	drawCharacter('H',0 + increment,88,6 + increment,92,RED, ledNo);
	
	int windSpeed = weatherObject[i].windSpeed;
	if(windSpeed < 0)
	{
		draw_line(15 + increment,71,15 + increment,74,CYAN, ledNo);
		windSpeed = windSpeed*(-1);
	}
	int windOnes = windSpeed % 10;
	int windTens = (windSpeed / 10) % 10;

	drawDigit(windTens,10 + increment,76,20 + increment,79,CYAN, ledNo);
	drawDigit(windOnes,10 + increment,81,20 + increment,84,CYAN, ledNo);
	
	increment += 36;
}

void draw_rectangle(uint8_t sRow, uint8_t sCol, uint8_t eRow, uint8_t eCol, uint16_t colorBorder, uint16_t colorFill, int ledNo)
{
	SPI_transfer_byte(SSD1331_CMD_DRAWRECT, ledNo);
	SPI_transfer_byte(sCol, ledNo);
	SPI_transfer_byte(sRow, ledNo);
	SPI_transfer_byte(eCol, ledNo);
	SPI_transfer_byte(eRow, ledNo);
	// SPI_transfer_byte(0x00, ledNo);
	// SPI_transfer_byte(0x00, ledNo);
	// SPI_transfer_byte(0x20, ledNo);
	// SPI_transfer_byte(0x10, ledNo);
	delay_ms(SSD1331_DELAYS_HWLINE);
	SPI_transfer_byte((uint8_t)((colorBorder >> 11) << 1), ledNo);
	SPI_transfer_byte((uint8_t)((colorBorder >> 5) & 0x3F), ledNo);
	SPI_transfer_byte((uint8_t)((colorBorder << 1) & 0x3F), ledNo);
	SPI_transfer_byte((uint8_t)((colorFill >> 11) << 1), ledNo);
	SPI_transfer_byte((uint8_t)((colorFill >> 5) & 0x3F), ledNo);
	SPI_transfer_byte((uint8_t)((colorFill << 1) & 0x3F), ledNo);
	delay_ms(SSD1331_DELAYS_HWLINE);
}

void drawPastTenTransactions(int *transactions, int noTransactions, int ledNo)
{
    int maxVal = 0, minVal = 0;
    for(int i = 0; i < noTransactions; ++i)
    {
        if(transactions[i] > maxVal) maxVal = transactions[i];
        if(transactions[i] < minVal) minVal = transactions[i];
    }

	fill_color(BLACK, ledNo);
    draw_line(2, 2, 62, 2, WHITE, ledNo);
    draw_line(31, 2, 31, 94, WHITE, ledNo);

    if(noTransactions == 0) return;

    int totalWidth = 94 - 2;
    int width = totalWidth / noTransactions;

    int startIdx = 3;
    for(int i = 0; i < noTransactions; ++i)
    {
        if(transactions[i] < 0)
            draw_rectangle(startIdx, 32, startIdx + (startIdx + width), 32 + ( minVal / transactions[i] ), RED, RED, ledNo);
        else
            draw_rectangle(2, (startIdx - maxVal / transactions[i]) , 
                                (maxVal / transactions[i]) ,( startIdx + width ), GREEN, GREEN, ledNo);

        startIdx += width;
    }
}

void weatherHandler(const char *event, const char *data)
{
    int temp = 0;
    int windSpeed = 0;
    int i = 0;
    
    Particle.publish("temp", data);

    while(data[i] != '#')
    {
        temp = temp * 10 + data[i] - '0';    
        i++;
    }
    i++;
    
    while(data[i] != '#')
    {
        windSpeed = windSpeed * 10 + data[i] - '0';    
        i++;
    }
    i++;
    
    while(data[i] != '#')
    {
        windSpeed = windSpeed * 10 + data[i] - '0';    
        i++;
    }
    
    i++;
    
    weatherObject[0].temperature = temp;
    weatherObject[0].windSpeed = windSpeed;
    
    drawWeatherInfo(LED_TWO);
}

void dateTimeHandler(const char *event, const char *data)
{
    Particle.publish("temp", data);
}

void capitalOneHandler(const char *event, const char *data)
{
    Particle.publish("temp", data);
}

void setup()
{
    spi_init();
    initializeOLED(LED_ONE);
    initializeOLED(LED_TWO);
    
    Particle.subscribe("hook-response/weather", weatherHandler, MY_DEVICES);
    //Particle.subscribe("hook-response/dateTime", dateTimeHandler, MY_DEVICES);
    //Particle.subscribe("hook-response/capitalOne", capitalOneHandler, MY_DEVICES);
    //Particle.subscribe("hook-response/stocks", weatherHandler, MY_DEVICES);
    
    int transacts[] = {1,2,3,4,5,6,7,8,9,11};
    int number_transacts = 10;
    
    //drawPastTenTransactions(transacts, number_transacts);
	
	for(int i = 0; i < 3; i++)
	{
	    weatherObject[i].typeDay = 2;
	    weatherObject[i].windSpeed = 23;
	}
	
	//delay_ms(60000);
	
    fill_color(BLACK, LED_ONE);
    drawTimeMenu(88,88, LED_ONE);
    drawWeatherInfo(LED_TWO);
}

void loop()
{
    String temperature = String(random(60, 80));
    //Particle.publish("temp", temperature, PRIVATE);
    Particle.publish("weather");
    //Particle.publish("dateTime");
    //Particle.publish("capitalOne");
    delay(30000);
    //Particle.publish("stocks");    TODO:: ADD STOCKS
}
