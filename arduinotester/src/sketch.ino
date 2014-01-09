//simple arduino selftester:
//connect the pins shown below all together and connect that "centerpoint" with one 2k2 resistor to gnd.
//we use the internal pullup resistors to do the actual testing.

//note that analog pins also can be digital output pins!
const byte output_pins[]={2,3,4,5,6,7,8,9,10,11,12, A0,A1,A2,A3};
const byte digital_input_pins[]={2,3,4,5,6,7,8,9,10,11,12};
const byte analog_input_pins[]={A0,A1,A2,A3};

const word analog_pullup_value=58;
const word analog_open_value=0; // high impedance
const word analog_high_value=1012;
const word analog_low_value=23;

int failed=0;

//reset all pins to input without pullup
void reset()
{
	//to be sure we traverse all arrays, in case of typos . (we dont want to blow up a port while testing ;)
	for (int i=0; i< sizeof(digital_input_pins); i++)
	{
		pinMode(digital_input_pins[i], INPUT);
	}
	for (int i=0; i< sizeof(analog_input_pins); i++)
	{
		pinMode(analog_input_pins[i], INPUT);
	}
	for (int i=0; i< sizeof(output_pins); i++)
	{
		pinMode(output_pins[i], INPUT);
	}
}



void settle()
{
	delay(10);
}

void setup()
{
  Serial.begin(9600);

}

void print_analog_header()
{
	for (byte a=0; a<sizeof(analog_input_pins); a++)
	{
		Serial.print("\t");
		Serial.print(analog_input_pins[a]);
		Serial.print(":");
	}
}

void print_analog_values(int skip, int compare)
{
	for (byte a=0; a<sizeof(analog_input_pins); a++)
	{
		Serial.print("\t");
		if (analog_input_pins[a]==skip)
		{
			Serial.print("-");
		}
		else
		{
			int value=analogRead(analog_input_pins[a]);
			Serial.print(value);

			if (abs(value-compare)>10)
			{
				failed++;
				Serial.print("!");
			}
		}
	}
}



void print_digital_header()
{
	for (byte a=0; a<sizeof(digital_input_pins); a++)
	{
		Serial.print("\t");
		Serial.print(digital_input_pins[a]);
		Serial.print(":");
	}

}

void print_digital_values(int skip, byte compare)
{
	for (byte a=0; a<sizeof(digital_input_pins); a++)
	{
		Serial.print("\t");
		if (digital_input_pins[a]==skip)
		{
			Serial.print("-");
		}
		else
		{
			byte value=digitalRead(digital_input_pins[a]);
			Serial.print(value);

			if (value!=compare)
			{
				failed++;
				Serial.print("!");
			}
		}
	}
}

void loop()
{

	Serial.println();
	Serial.println("Starting test:");
	reset();

	Serial.println("All ports high impedance:");
	print_digital_header();
	print_analog_header();
	Serial.println();
	print_digital_values(-1,0);
	print_analog_values(-1,analog_open_value);
	Serial.println();

	Serial.println("Enable pullups one by one:");
	print_digital_header();
	print_analog_header();
	Serial.println();
	for (byte p=0; p<sizeof(output_pins); p++)
	{
		reset();
		//since the port is configured as INPUT, this will activate the pullup resitor
		digitalWrite(output_pins[p], HIGH);
		settle();

		Serial.print(output_pins[p]);
		Serial.print(":");
		print_digital_values(output_pins[p],0);
		print_analog_values(output_pins[p],analog_pullup_value);
		Serial.println();
	}

	Serial.println("Output high one by one:");
	print_digital_header();
	print_analog_header();
	Serial.println();
	for (byte p=0; p<sizeof(output_pins); p++)
	{
		reset();
		pinMode(output_pins[p], OUTPUT);
		digitalWrite(output_pins[p], HIGH);
		settle();

		Serial.print(output_pins[p]);
		Serial.print(":");
		print_digital_values(output_pins[p],1);
		print_analog_values(output_pins[p],analog_high_value);
		Serial.println();
	}

	Serial.println("Output low one by one:");
	print_digital_header();
	print_analog_header();
	Serial.println();
	for (byte p=0; p<sizeof(output_pins); p++)
	{
		reset();
		//this results in all pins pulling up
		for (int i=0; i< sizeof(output_pins); i++)
		{
			digitalWrite(output_pins[i], HIGH);
		}
		//now set the tested pin down as real output:
		digitalWrite(output_pins[p], LOW);
		pinMode(output_pins[p], OUTPUT);
		settle();

		Serial.print(output_pins[p]);
		Serial.print(":");
		print_digital_values(output_pins[p],0);
		print_analog_values(output_pins[p],analog_low_value);
		Serial.println();
	}

	if (failed)
		Serial.println("*FAILED*");
	else
		Serial.println("*PASSED*");

    delay(60000);
}
