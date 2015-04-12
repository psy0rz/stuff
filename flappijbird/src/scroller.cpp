#include "Arduino.h"
#include "PropFont.h"
#include "LedControlMS.h"

#define DISPLAYS 1

//returns false on abort
bool scrolltext(LedControl & lc, char *msg, int wait, int abort_pin=0)
{

	int curcharix = 0;
	int curcharbit = 0;
	int curcharixsave = 0;
	int curcharbitsave = 0;
	int curcharixsave2 = 0;
	int curcharbitsave2 = 0;
	char curchar;
	int msgsize=strlen(msg);

	int abort_state=digitalRead(abort_pin);

	int i,j,k;
    while(abort_pin==0 || digitalRead(abort_pin)==abort_state)
	{
		curcharixsave2 = curcharix;
		curcharbitsave2 = curcharbit;

		for (i=DISPLAYS-1;i>=0;i--) // Loop through our 1 display
		{
			for (j=0;j<8;j++) // Set up rows on current  display
			{      
				byte outputbyte = 0;
                                curchar = msg[curcharix];
				curcharixsave = curcharix;
				curcharbitsave = curcharbit;
				for (k=7;k>=0;k--) // Copy over data for 8 columns to current row and send it to current display
				{
					// This byte is the bitmap of the current character for the current row
					byte currentcharbits = pgm_read_byte(&Font8x5[((curchar-32)*8)+j]);
					if (currentcharbits & (1<<curcharbit))
						outputbyte |= (1<<k);

					// advance the current character bit of current character
					curcharbit ++;

					if (curcharbit > pgm_read_byte(&lentbl_S[curchar-32])) // we are past the end of this character, so advance.
					{
						curcharbit = 0;
						curcharix += 1;          
						if (curcharix+1 > msgsize) curcharix=0;
						curchar = msg[curcharix];
					}
				}

				lc.setColumn(i, 7-j, outputbyte);

				if (j != 7) // if this is not the last row, roll back advancement, if it is, leave the counters advanced.
				{
					curcharix = curcharixsave;
					curcharbit = curcharbitsave;
				}

			}

		}

		curcharix = curcharixsave2;
		curcharbit = curcharbitsave2;

		curchar = msg[curcharix];

		// advance the current character bit of current character
		curcharbit ++;

		if (curcharbit > pgm_read_byte(&lentbl_S[curchar-32])) // we are past the end of this character, so advance.
		{
			curcharbit = 0;
			curcharix += 1;
			if (curcharix+1 >= msgsize)  {
				return(true);
			}
			curchar = msg[curcharix];

		}

		delay(wait);
	}

	lc.clearDisplay(0);
	return(false);
}

