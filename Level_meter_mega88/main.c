/*
 * LevelMeter.c
 *
 * Created: 31/1/2016 8:35:43 μμ
 * Author : George
 //this function uses Timer 1 and PC3 for trigger, PC4 for echo
 
 float getSurfaceDistance() //gives the distance in cm, gives -1 if time out(no obstacle within 5 meters)
 {
	 uint16_t count;
	 float dis;
	 DDRC|=(1<<PC3); //OUTPUT
	 DDRC&=~(1<<PC4); //INPUT
	 //GIVE A PULSE TO THE PC3
	 PORTC|=(1<<PC3);
	 _delay_us(10);
	 PORTC&=~(1<<PC3);
	 
	 while(!(PINC&(1<<PC4))); //wait until echo signal becomes high
	 TCNT1=0; //reset the timer
	 TCCR1B=0x02; //START TIMER(pre scaler - 8)
	 // START TIMER
	 while((PINC&(1<<PC4))&&(TCNT1<60000)); //wait until echo signal becomes low or timeout count=TCNT1;//LOAD TIMER if(count>=60000)
	 
	 return -1.0;
	 
	 TCCR1B=0x00;
	 dis=(count)*(0.0085);
	 return dis;
 }
UltraSonic based load control
Connections:
trigger- pc3
echo- pc4
 
load 1(3 feet)- PB0
load 2 (1.5 feet)-PB1
 
*/
//trigger pc3, echo pc4
#include <avr/io.h>
#define F_CPU 4000000ul
#include <util/delay.h>
#include<math.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include "lcd_lib.h"
#include "ScanKeys.h"
#include <avr/eeprom.h>
#include <util/atomic.h>
//#define max_height 200
#define limit1 PD7
#define limit2 PC0
#define shoot PB0
#define echo PB1
#define backlight PB5 //it was pb6 originally. Changed for using it to xtal physical connection
//#define lcd_pwr PB7
#include "tools.h"
#include <avr/wdt.h>
#include "average.h"
//#include "display.h"
uint8_t EEMEM dimensions[4] = {100,100,100,0}; //x,y,z;r,z;a,b,c
float EEMEM cal[1];
uint8_t *c_system[] = {	"Cartesian","Cylinder","Elliptic"};
enum coord_system  {cartessian, cylinder, elliptic};	
volatile float dis_av=0;
volatile float dis=0.0;
static volatile uint16_t pulse = 0;
static volatile uint8_t i = 0,j;
static volatile uint8_t meas_ready=0;
uint8_t get_front(float * dis)
{
uint16_t count;
//float dis;
//DDRB|=(1<<shoot); //OUTPUT
//PORTB|=(1<<echo);
//DDRB&=~(1<<echo); //INPUT
//GIVE A PULSE TO THE shoot
//_delay_us(20);
PORTB|=(1<<shoot);
_delay_us(15);
//TCNT1=0;
//TCCR1A=0x00;
//TCCR1B=0x00;
//TCCR1C=0x00;
PORTB&=~(1<<shoot);
_delay_ms(120);
//while(!(PINB&(1<<echo)));
//TCCR1A=0x00;
//TCCR1B=0x01; //START TIMER(No prescaling)
// START TIMER
//while((PINB&(1<<echo))&&(TCNT1<60000));
//while(PINB&(1<<echo));
//count=TCNT1;//LOAD TIMER
//TCCR1B=0x00;
 if (pulse>=65000)
	return -1;
 else
//dis=(count/10)*(0.17160);
	*dis=pulse*0.01715;
	//*dis=pulse;
	//TCNT1=0;	
	return 1;
}

uint8_t choose_c_system()
{
	enum coord_system tmp = cartessian;
	uint8_t temp;
	temp=eeprom_read_byte(&dimensions[3]);
	if (temp<0||temp>2)
	{
		temp=tmp;
	}
	_delay_ms(100);
	if (!(Pinkeys & _BV(enter)))
	{
	while (!(Pinkeys & _BV(enter)));
	while (Pinkeys & _BV(enter))
	{
		
		LCDhome();
		LCDGotoXY(0,0);
		LCDstring((c_system[tmp]),sizeof(c_system[tmp])<<2);
		//LCDstring(inttostr(temp),4);
		temp=_key(temp,5);
		temp%=3;
		tmp=temp;
		
	}
	}
	eeprom_update_byte(&dimensions[3],temp);
	tmp=temp;
	return tmp;
}

void set_cylinder_dimensions()
{
	uint8_t temp;
	_delay_ms(100);
	while (!(Pinkeys & _BV(enter)));
	temp=eeprom_read_byte(&dimensions[0]);
	while (Pinkeys & _BV(enter))
	{
		
		LCDhome();
		LCDGotoXY(0,0);
		LCDstring("Radial ",7);
		LCDstring(inttostr(temp),4);
		temp=_key(temp,5);
		
	}
	eeprom_update_byte(&dimensions[0],temp);
	_delay_ms(100);
	while (!(Pinkeys & _BV(enter)));
	temp=eeprom_read_byte(&dimensions[1]);
	while (Pinkeys & _BV(enter))
	{
		
		LCDhome();
		LCDGotoXY(0,0);
		LCDstring("Height ",7);
		LCDstring(inttostr(temp),4);
		temp=_key(temp,5);
		
	}
	eeprom_update_byte(&dimensions[1],temp);
	while (!(Pinkeys & _BV(enter)));
}

void set_Cartesian_dimensions()
{
	uint8_t temp;
	_delay_ms(100);
	while (!(Pinkeys & _BV(enter)));
	temp=eeprom_read_byte(&dimensions[0]);
	while (Pinkeys & _BV(enter))
	{
		
		LCDhome();
		LCDGotoXY(0,0);
		LCDstring("Width ",6);
		LCDstring(inttostr(temp),4);
		temp=_key(temp,5);
		
	}
	eeprom_update_byte(&dimensions[0],temp);
	_delay_ms(100);
	while (!(Pinkeys & _BV(enter)));
	temp=eeprom_read_byte(&dimensions[1]);
	while (Pinkeys & _BV(enter))
	{
		
		LCDhome();
		LCDGotoXY(0,0);
		LCDstring("Depth ",6);
		LCDstring(inttostr(temp),4);
		temp=_key(temp,5);
		
	}
	eeprom_update_byte(&dimensions[1],temp);
	_delay_ms(100);
	while (!(Pinkeys & _BV(enter)));
	temp=eeprom_read_byte(&dimensions[2]);
	while (Pinkeys & _BV(enter))
	{
		
		LCDhome();
		LCDGotoXY(0,0);
		LCDstring("Height ",7);
		LCDstring(inttostr(temp),4);
		temp=_key(temp,5);
		
	}
	eeprom_update_byte(&dimensions[2],temp);
	while (!(Pinkeys & _BV(enter)));
}

void set_Elliptic_dimensions()
{
	uint8_t temp;
	_delay_ms(100);
	while (!(Pinkeys & _BV(enter)));
	temp=eeprom_read_byte(&dimensions[0]);
	while (Pinkeys & _BV(enter))
	{
		
		LCDhome();
		LCDGotoXY(0,0);
		LCDstring("Axis A ",6);
		LCDstring(inttostr(temp),4);
		temp=_key(temp,5);
		
	}
	eeprom_update_byte(&dimensions[0],temp);
	_delay_ms(100);
	while (!(Pinkeys & _BV(enter)));
	temp=eeprom_read_byte(&dimensions[1]);
	while (Pinkeys & _BV(enter))
	{
		
		LCDhome();
		LCDGotoXY(0,0);
		LCDstring("Axis B ",6);
		LCDstring(inttostr(temp),4);
		temp=_key(temp,5);
		
	}
	eeprom_update_byte(&dimensions[1],temp);
	_delay_ms(100);
	while (!(Pinkeys & _BV(enter)));
	temp=eeprom_read_byte(&dimensions[2]);
	while (Pinkeys & _BV(enter))
	{
		
		LCDhome();
		LCDGotoXY(0,0);
		LCDstring("Height ",7);
		LCDstring(inttostr(temp),4);
		temp=_key(temp,5);
		
	}
	eeprom_update_byte(&dimensions[2],temp);
	while (!(Pinkeys & _BV(enter)));
}

void get_cartesian_dim()
{
uint8_t temp = 0;
uint8_t temp1;	
//while (!(Pinkeys & _BV(enter)));
	while (Pinkeys & _BV(enter))
	{
		LCDhome();
		LCDGotoXY(0,1);
		if (temp == 0)
		LCDstring("Width ",6);
		else if (temp == 1)
		LCDstring("Depth ",6);
		else if (temp == 2)
		LCDstring("Height ",7);
		temp1=eeprom_read_byte(&dimensions[temp]);
		LCDstring(inttostr(temp1),4);
		LCDstring("   ",3); //clear the rest
		temp=_key(temp,5);
		temp%=3;
	}	
}

void get_cylinder_dim()
{
uint8_t temp = 0;
uint8_t temp1;	
//while (!(Pinkeys & _BV(enter)));
	while (Pinkeys & _BV(enter))
	{
		LCDhome();
		LCDGotoXY(0,1);
		if (temp == 0)
		LCDstring("Randial ",8);
		else if (temp == 1)
		LCDstring("Height ",7);
		temp1=eeprom_read_byte(&dimensions[temp]);
		LCDstring(inttostr(temp1),4);
		LCDstring("   ",3); //clear the rest
		temp=_key(temp,5);
		temp%=2;
	}	
}

//initialize watchdog
void WDT_Init(void)
{
	//disable interrupts
	cli();
	//reset watchdog
	wdt_reset();
	//set up WDT interrupt
	WDTCSR = (1<<WDCE)|(1<<WDE);
	//Start watchdog timer with 4s prescaller
	//WDTCSR = (1<<WDIE)|(1<<WDE)|(1<<WDP3);
	WDTCSR = (1<<WDIE) | (0<<WDP2) | (1<<WDP3);
	//_delay_ms(500);
	sei();
}

int main()
{
uint8_t tmp=0;
uint32_t temp;
uint8_t float2string[17];
float temp1=0.0;
float cal_coef=1.0;
uint8_t string[5]="Volu " ;
uint8_t yes_no=1;
enum coord_system coord;
//LCDinit();
//PORTB|=_BV(lcd_pwr);
LCDinit();
LCDclr();
init_keys();
DDRB|=(1<<shoot); //OUTPUT
DDRC|=(1<<limit2);
DDRD|=(1<<limit1);
DDRB|=(1<<backlight);
PORTB|=(1<<backlight);
float lit_cm=0;
uint8_t max_height=0;
//PORTB=0x00;
TCCR1A = 0;
TCCR1C = 0;
PORTB|=(1<<echo);
DDRB&=~(1<<echo); //INPUT
//EIMSK |= _BV(INT0);  //Enable INT0
//EICRA |= _BV(ISC00); //Trigger on falling edge of INT0
//sei();
	coord = choose_c_system();
	switch (coord) 
	{
		case cartessian:
		if (!(Pinkeys & _BV(enter)))
		{
			while (yes_no)
			{
				LCDclr();
				set_Cartesian_dimensions();
				while(Pinkeys & _BV(enter))
				{
					//LCDclr();
					LCDhome();
					LCDGotoXY(0,0);
					LCDstring("Ok? +/- (YES|NO)",16);
					temp=yes_no; //save the original status og yes_no
					yes_no=_key(yes_no,5);
					yes_no%=2;
					//if (!(yes_no==temp))
					//{
						LCDGotoXY(0,1);
						switch(yes_no)
						{
							case 1:
							LCDstring("NO ",3);
							break;
							case 0:
							LCDstring("YES",3);
							break;
						}
					//}
					
				}
				
			}
		}
		tmp = eeprom_read_byte(&dimensions[0]);
		temp = eeprom_read_byte(&dimensions[1]);
		lit_cm = (temp*tmp)/100;
		max_height = eeprom_read_byte(&dimensions[2]);
		break;
		case cylinder:
		if (!(Pinkeys & _BV(enter)))
		{
			while (yes_no)
			{
				LCDclr();
				set_cylinder_dimensions();
				while(Pinkeys & _BV(enter))
				{
					//LCDclr();
					LCDhome();
					LCDGotoXY(0,0);
					LCDstring("Ok? +/- (YES|NO)",16);
					temp=yes_no; //save the original status of yes_no
					yes_no=_key(yes_no,5);
					yes_no%=2;
					//if (!(yes_no==temp))
					//{
					LCDGotoXY(0,1);
					switch(yes_no)
					{
						case 1:
						LCDstring("NO ",3);
						break;
						case 0:
						LCDstring("YES",3);
						break;
					}
					//}
					
				}
				
			}
			
		}
		
		tmp = eeprom_read_byte(&dimensions[0]);
		lit_cm = (tmp*tmp)/100;
		lit_cm*=3.14;
		max_height = eeprom_read_byte(&dimensions[1]);
		break;
		case elliptic:
		if (!(Pinkeys & _BV(enter)))
		set_Elliptic_dimensions();
		tmp = eeprom_read_byte(&dimensions[0]);
		temp = eeprom_read_byte(&dimensions[1]);		
		lit_cm = (tmp*temp*12.56)/3000;
		max_height = eeprom_read_byte(&dimensions[2]);
		break;
	}

temp=0;
tmp=0;
uint8_t countr=500;
uint8_t sensor_counter;
//float dis=0;
uint16_t dis_int;
//uint8_t str_buf[15];
uint8_t sample=4;
set_sleep_mode(SLEEP_MODE_PWR_DOWN);
PCICR &= ~_BV(PCIE1);
PCMSK1|= _BV(PCINT9);
PCICR|= _BV(PCIE0);
PCMSK0|= _BV(PCINT1);
sei();
//WDT_Init();
if(!(Pinkeys&_BV(up)))
{
	while(!(Pinkeys&_BV(up)));
	LCDhome();
	LCDGotoXY(0,0);
	LCDstring("Cal.? +yes -No",14);
	while ((Pinkeys & _BV(up))&&(Pinkeys & _BV(down)));
	_delay_ms(120);
	if(!(Pinkeys & _BV(up)))
	{
		LCDGotoXY(0,0);
		LCDstring("set a target at",15);
		LCDGotoXY(0,1);
		LCDstring("0.5m&press enter",16);
		while(Pinkeys & _BV(enter));
		_delay_ms(120);
		LCDclr();
		LCDGotoXY(0,0);
		LCDstring("cal. in progress",16);
		LCDGotoXY(0,1);
		for(j=0;j<=20;j++)
		{
			if(meas_ready)
			{
				get_front(&dis);
				dis_av=dis;
				Average(&dis_av);
				meas_ready=0;
			}
			
			//LCDstring(".",1);
			LCDGotoXY(0,1);
			LCDprogressBar(j, 20, 16);
			_delay_ms(200);
						
		}
		LCDclr();
		LCDGotoXY(0,0);
		LCDstring("Dist= ",6);
		dtostrf(dis_av,4,3,float2string);
		LCDstring(float2string,strlen(float2string));
		LCDGotoXY(0,1);
		LCDstring("Cal_coef= ",10);
		cal_coef=dis_av/50.0;
		dtostrf(cal_coef,4,3,float2string);
		LCDstring(float2string,strlen(float2string));
		eeprom_update_float(&cal[0],cal_coef);
					
	}
	else if(!(Pinkeys&_BV(down)))
	{
		cal_coef=eeprom_read_float(&cal[0]);
		LCDGotoXY(0,1);
		LCDstring("Cal_coef= ",10);
		dtostrf(cal_coef,4,3,float2string);
		LCDstring(float2string,strlen(float2string));
	}
	
	_delay_ms(2000);
}
while(1)
	{	//max_height=200;
		sensor_counter++;
		if (countr) countr--;
		
		else// (!countr)
		{
			//sleep_enable();
			//WDT_Init();
			
			PORTB&=~(1<<backlight);
			PCICR|= _BV(PCIE1);
			PCMSK1|= _BV(PCINT9);
			sei();
			
			sleep_mode();
			cli();
			sleep_disable();
			PCICR &= ~_BV(PCIE1);
			PCMSK1 &= _BV(PCINT9);
			sei();
		}
		/*if (sample--)	
		{	
			dis+=get_front();
		}
		else
		{
			sample=4;
			dis_av=dis/sample;
			dis=0;
			
		}*/
		//dis=get_front();
		//get_front(&dis);
		//LCDclr();
		LCDhome();
		LCDGotoXY(0,0);
		//if ((get_front(&dis)!=-1)&& sensor_counter==3)
		if((get_front(&dis)!=-1)&& meas_ready)
		{
			//get_front(&dis);
			meas_ready=0;
			sensor_counter=0;
			dis_av=dis;
			Average(&dis_av);
			dis_av*=cal_coef;
			//while (dis_int==0) 
			//{dis_int=filter((uint16_t)(dis*100),150); 
				//get_front(&dis);}
			//dis_av=dis_int/100.0;
			//LCDGotoXY(0,1);
			//LCDstring(string,5);
			//LCDstring("test",4);
			if (coord==elliptic)
				temp1 = lit_cm*(max_height-dis_av)/20.0;
			else
				temp1 = lit_cm*(max_height-dis_av)/10.0;
			//LCDstring(inttostr(temp),4);
			//LCDstring(inttostr(dis),4);
			//LCDsendChar('e');
			if(dis_av>(max_height*3/4) && dis_av<(max_height*5/6))//tank emptier than 3/4 of max 
			{
				PORTD|=(1<<limit1);
			}
			else if (dis_av <(max_height*2/3))
			{
				PORTD&=~(1<<limit1);
			}
			
			if(dis_av>(max_height*7/8))//tank emptier than 7/8 of max
			{
				PORTD&=~(1<<limit1);
				_delay_ms(100);
				PORTC|=(1<<limit2);
				LCDGotoXY(12,1);
				LCDstring("!!! ",4);
				LCDGotoXY(0,0);
			}
			else if (dis_av <(max_height*5/6))
			{
				//while(i);
				PORTC&=~(1<<limit2);
				LCDGotoXY(12,1);
				LCDstring("    ",4);
				LCDGotoXY(0,0);
			}
		
		
		//dis++;
		//if (dis_av>max_height)
		//	dis_av=max_height;
		LCDprogressBar(max_height-dis_av, max_height, 16);
			
		if (!(Pinkeys &_BV(up))||!(Pinkeys &_BV(down))||!(Pinkeys &_BV(enter)))
			{
				countr=500;
				PORTB|=(1<<backlight);
			}
		if (!(Pinkeys &_BV(up))||!(Pinkeys &_BV(down)))
		{
			tmp=_key(tmp,1);
			tmp%=4;
			while (!(Pinkeys &_BV(up))||!(Pinkeys &_BV(down)));
			
		}
				LCDGotoXY(0,1);			
		if (tmp==1)
		{
		LCDstring("Volu ",5);
		}
		else if (tmp==0)
		{
			LCDstring("Dist ",5);
			temp1 = dis_av;
		}
		else if (tmp==2)
		{
			LCDstring("Level:",6);
			temp1 = max_height-dis_av;
		}
		else if (tmp==3)
		{	
			LCDclr();
			LCDhome();
			LCDGotoXY(0,0);
			LCDstring("Dim of ",7);
			switch (coord) 
			{
				case cartessian:
				LCDstring("Cart:",5);
				get_cartesian_dim();
				break;
				case cylinder:
				LCDstring("Cylinder:",9);
				get_cylinder_dim();
				break;
				
			}
			LCDclr();
			tmp=0;
		}
		else continue;
		if (temp1)
		//sprintf(float2string,"%0.2f",temp1);
		dtostrf(temp1,4,3,float2string);
		LCDstring(float2string,strlen(float2string));
		//LCDstring(inttostr((uint16_t)dis),4);
		
		//LCDstring(dtostrf(temp,5,1,str_buf),4);
		if (tmp==1)
		{
			LCDstring(" lt ",5);
		}
		else if (tmp==0)
		{
			LCDstring(" cm  ",5);
			
		}
		else if (tmp==2)
		{
			LCDstring(" cm  ",5);
			
		}
		
		
		
			
		}
		/*else
		{
			LCDhome();
			LCDclr();
			LCDGotoXY(0,0);
			LCDstring("Error!",6);
		}*/
		_delay_ms(20);//0.1 sec delay to avoid echoes?
	}
}

ISR(PCINT1_vect)
{
	//cli();
	//PCICR &= ~_BV(PCIE1);
	//PCMSK1 = 0;
	;
}

/*ISR(WDT_vect)
{
	//cli();
	sleep_disable();
	uint8_t max_height = eeprom_read_byte(&dimensions[2]);
	//float dis;
	
	//while(!(dis_av=(filter(get_front(),150)/100))) ;
	//while (dis==0) {dis=filter((uint16_t)(dis*100),150); get_front(&dis);}
		//	dis_av=dis/100.0;
	//get_front(&dis_av);
	if(dis_av >(max_height*3/4))//tank emptier than 3/4 of max
	{
		PORTD|=(1<<limit1);
	}
	else if (dis_av <(max_height*2/3))
	{
		PORTD&=~(1<<limit1);
	}
	
	if(dis_av >(max_height*7/8))//tank emptier than 7/8 of max
	{
		PORTC|=(1<<limit2);
	}
	else if (dis_av <(max_height*6/7))
	{
		PORTC&=~(1<<limit2);
	}
	sleep_enable();
	//sei();
	
}*/

ISR(PCINT0_vect)//interrupt service routine when there is a change in logic level

{
	cli();
	if (i==1)//when logic from HIGH to LOW

	{
		TCCR1B=0;//disabling counter

		//pulse=(TCNT1H<<8);//count memory is updated to integer
		//pulse|=TCNT1L;
		pulse=TCNT1;
		TCNT1=0;//resetting the counter memory

		i=0;
		meas_ready=1;
	}
	else if (i==0)//when logic change from LOW to HIGH

	{
		//pulse=TCNT1;
		TCCR1B|=(1<<CS10);//enabling counter
		i=1;
		//pulse=TCNT1;
		

	}
	sei();

}