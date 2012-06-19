#include <stdlib.h>
#include "main.h"
#include <stdio.h>
#include "plasma.h"


#ifdef __AVR__
#include <avr/pgmspace.h>
#else
typedef uint16_t prog_uint16_t;
typedef uint8_t prog_uint8_t;
#endif


static prog_uint8_t sin_table[256] = 
{
	0,0,1,2,3,3,4,5,6,7,7,8,9,10,11,11,12,13,14,15,15,16,17,18,19,19,20,21,22,23,23,24,25,26,26,
	27,28,29,30,30,31,32,33,33,34,35,36,36,37,38,39,40,40,41,42,43,43,44,45,46,46,47,48,48,49,50,
	51,51,52,53,54,54,55,56,56,57,58,59,59,60,61,61,62,63,64,64,65,66,66,67,68,68,69,70,70,71,72,
	72,73,74,74,75,76,76,77,77,78,79,79,80,81,81,82,82,83,84,84,85,85,86,87,87,88,88,89,89,90,91,
	91,92,92,93,93,94,94,95,95,96,97,97,98,98,99,99,100,100,101,101,102,102,103,103,103,104,104,
	105,105,106,106,107,107,107,108,108,109,109,110,110,110,111,111,112,112,112,113,113,113,114,
	114,114,115,115,116,116,116,117,117,117,117,118,118,118,119,119,119,120,120,120,120,121,121,
	121,121,122,122,122,122,123,123,123,123,123,124,124,124,124,124,124,125,125,125,125,125,125,
	126,126,126,126,126,126,126,126,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,
	127,127,127,127,127,127,127,127,127
};

// input 0..1023 ; output 0..255
static uint8_t sini(uint16_t x) 
{
	x = x & 1023;

#ifdef __AVR__
	if(x < 256) return pgm_read_byte(&sin_table[x])+128;
	if(x < 512) return pgm_read_byte(&sin_table[511-x])+128;
	if(x < 768) return 127-pgm_read_byte(&sin_table[x-512]);
	return 127-pgm_read_byte(&sin_table[1023-x]); 
#else
	if(x < 256) return sin_table[x]+128;
	if(x < 512) return sin_table[511-x]+128;
	if(x < 768) return 127-sin_table[x-512];
	return 127-sin_table[1023-x]; 
#endif

}


static uint16_t nr, ng, nb;

static uint16_t h;
static uint16_t h1;
static uint16_t h2;
static uint16_t s=255;

static prog_uint16_t table[] = { 0,1,2,2,3,4,5,5,6,7,8,9,9,10,11,12,13,13,14,15,16,17,17,18,19,20,21,21,22,23,24,25,25,26,27,28,28,29,30,31,32,32,33,34,35,36,36,37,38,39,40,40,41,42,43,44,44,45,46,47,48,48,49,50,51,51,52,53,54,55,55,56,57,58,59,59,60,61,62,63,63,64,65,66,67,67,68,69,70,70,71,72,73,74,74,75,76,77,78,78,79,80,81,82,82,83,84,85,86,86,87,88,89,89,90,91,92,93,93,94,95,96,97,97,98,99,100,101,101,102,103,104,104,105,106,107,108,108,109,110,111,112,112,113,114,115,116,116,117,118,119,119,120,121,122,123,123,124,125,126,127,127,128,129,130,131,131,132,133,134,134,135,136,137,138,138,139,140,141,142,142,143,144,145,146,146,147,148,149,150,150,151,152,153,153,154,155,156,157,157,158,159,160,161,161,162,163,164,165,165,166,167,168,168,169,170,171,172,172,173,174,175,176,176,177,178,179,180,180,181,182,183,183,184,185,186,187,187,188,189,190,191,191,192,193,194,195,195,196,197,198,199,199,200,201,202,202,203,204,205,206,206,207,208,209,210,210,211,212,213,214,214,215,216,217,218,218,219,220,
	221,221,222,223,224,225,225,226,227,228,229,229,230,231,232,233,233,234,235,236,237,237,238,239,240,240,241,242,243,244,244,245,246,247,248,248,249,250,251,252,252,253,254,255,256,256,257,258,259,260,260,261,262,263,264,264,265,266,267,268,268,269,270,271,272,272,273,274,275,276,276,277,278,279,280,280,281,282,283,284,284,285,286,287,288,288,289,290,291,292,292,293,294,295,296,296,297,298,299,300,300,301,302,303,304,304,305,306,307,308,308,309,310,311,312,312,313,314,315,316,316,317,318,319,320,320,321,322,323,324,324,325,326,327,328,329,329,330,331,332,333,333,334,335,336,337,337,338,339,340,341,341,342,343,344,345,346,346,347,348,349,350,350,351,352,353,354,355,355,356,357,358,359,359,360,361,362,363,363,364,365,366,367,368,368,369,370,371,372,373,373,374,375,376,377,377,378,379,380,381,382,382,383,384,385,386,387,387,388,389,390,391,391,392,393,394,395,396,396,397,398,399,400,401,401,402,403,404,405,406,406,407,408,409,410,411,412,412,413,414,415,416,417,417,418,
	419,420,421,422,422,423,424,425,426,427,428,428,429,430,431,432,433,433,434,435,436,437,438,439,439,440,441,442,443,444,445,445,446,447,448,449,450,451,451,452,453,454,455,456,457,457,458,459,460,461,462,463,463,464,465,466,467,468,469,470,470,471,472,473,474,475,476,477,477,478,479,480,481,482,483,484,484,485,486,487,488,489,490,491,492,492,493,494,495,496,497,498,499,500,501,501,502,503,504,505,506,507,508,509,510,510,511,512,513,514,515,516,517,518,519,520,520,521,522,523,524,525,526,527,528,529,530,531,531,532,533,534,535,536,537,538,539,540,541,542,543,544,545,545,546,547,548,549,550,551,552,553,554,555,556,557,558,559,560,561,562,563,563,564,565,566,567,568,569,570,571,572,573,574,575,576,577,578,579,580,581,582,583,584,585,586,587,588,589,590,591,592,593,594,595,596,597,598,599,600,601,602,603,604,605,606,607,608,609,610,611,612,613,614,615,616,617,618,619,620,621,622,623,624,626,627,628,629,630,631,632,633,634,635,636,637,638,639,640,641,642,644,645,646,647,
	648,649,650,651,652,653,654,655,657,658,659,660,661,662,663,664,665,666,668,669,670,671,672,673,674,675,677,678,679,680,681,682,683,685,686,687,688,689,690,692,693,694,695,696,697,699,700,701,702,703,704,706,707,708,709,710,712,713,714,715,716,718,719,720,721,723,724,725,726,727,729,730,731,732,734,735,736,737,739,740,741,742,744,745,746,748,749,750,751,753,754,755,757,758,759,761,762,763,764,766,767,768,770,771,772,774,775,776,778,779,781,782,783,785,786,787,789,790,792,793,794,796,797,799,800,801,803,804,806,807,808,810,811,813,814,816,817,819,820,822,823,824,826,827,829,830,832,833,835,836,838,839,841,842,844,846,847,849,850,852,853,855,856,858,860,861,863,864,866,867,869,871,872,874,875,877,879,880,882,884,885,887,889,890,892,894,895,897,899,900,902,904,905,907,909,911,912,914,916,918,919,921,923,925,926,928,930,932,934,935,937,939,941,943,944,946,948,950,952,954,956,957,959,961,963,965,967,969,971,973,975,977,979,981,982,984,986,988,990,992,994,996,998,1000,1002,
	1005,1007,1009,1011,1013,1015,1017,1019,1021,1023};

static void hsv_to_rgb(void)	
{
	uint16_t R,G,B;			

	uint32_t i,f;
	uint16_t p, q, t;

	if( s == 0 ) 
	{
		nr=1023;										// Auf 8 Bit RGB skalieren, nacher nur noch kopieren
		ng=1023;
		nb=1023;	
		return;
	}

	i = (((uint32_t)h) * 6) & 0xFF0000;
	f = ((((uint32_t)h) * 6) - i)>>8;
	i >>= 16;
	p = 65535 - s * 256;
	q = 65535 - s * f;
	t = 65535 - s * (256 - f);

	switch( i )
	{
		case 0:
			R = 65535; G = t; B = p; break;
		case 1:
			R = q; G = 65535; B = p; break;
		case 2:
			R = p; G = 65535; B = t; break;
		case 3:
			R = p; G = q; B = 65535; break;			
		case 4:
			R = t; G = p; B = 65535; break;
		default:								
			R = 65535; G = p; B = q; break;				// case 5:					
	}

	#ifdef __AVR__
	nr=pgm_read_word(&table[R>>6]);
	ng=pgm_read_word(&table[G>>6]);
	nb=pgm_read_word(&table[B>>6]);
	#else
	nr=table[R>>6];
	ng=table[G>>6];
	nb=table[B>>6];
	#endif


}

uint8_t tick(void) {
	static uint16_t a = 0;
	static uint8_t zap = 0;

	uint8_t x, y;

	uint8_t sin1 = sini(a+512);
	uint8_t sin2 = sini(a*2);
	uint16_t sin3 = sini(a*3)<<10;
	
	
	ssd1351SetCursor(0, 0);
	SET_DC; 
	CLR_D7;
	CLR_D6;

	

	for(y = 0; y < LED_HEIGHT; y++) 
	{
		uint16_t y_part = 32*sini(sin2+y*10)  + sin3;

		for(x = 0; x < LED_WIDTH; x++) 
		{
			h = 32*sini(sin1+x*(10+(sini(a*8)>>4)))+ y_part;
			h1 = 32*sini(sin1+x*20)+ y_part;
			h2 = 32*sini(sin1+x*10)+ y_part;
			//hsv_to_rgb();
			//setLedXY(x, y, nr>>2,ng>>2,nb>>2);
			//
			//
			uint8_t r = sini((h>>4)+sini(a)*3);
			uint8_t g = sini((h1>>4)+sini(a*2));
			uint8_t b = sini((h2>>4)+sini(a*3)*2);

	//		setLedXY(x, y, r,g,b);
#ifdef C262K
	//		ssd1351SendByte( (r>>2), 0 );


	uint16_t port_r = port2_table[r>>2];
	uint16_t port_g = port2_table[g>>2];
	uint16_t port_b = port2_table[b>>2];

	SET_E;
	LPC_GPIO2->DATA = port_r;
	__ASM volatile ("nop");
	CLR_E;
	SET_E;
	LPC_GPIO2->DATA = port_g;
	__ASM volatile ("nop");
	CLR_E;
	SET_E;
	LPC_GPIO2->DATA = port_b;
	__ASM volatile ("nop");
	CLR_E;

/*	CLR_D7;

	SET_E;
	LPC_GPIO0->MASKED_ACCESS[((1<<6)|(1<<7))]=r<<4;
	LPC_GPIO1->MASKED_ACCESS[((1<<3)|(1<<4)|(1<<5))]=r>>2;
	LPC_GPIO2->MASKED_ACCESS[(1<<3)]=r>>1;
	CLR_E;
*/
/*	SET_E;
	LPC_GPIO0->MASKED_ACCESS[((1<<6)|(1<<7))]=r<<6;
	LPC_GPIO1->MASKED_ACCESS[((1<<3)|(1<<4)|(1<<5))]=r>>2;
	LPC_GPIO2->MASKED_ACCESS[(1<<3)]=r>>1;

  __ASM volatile ("nop");
	CLR_E;
  __ASM volatile ("nop");
	SET_E;
	LPC_GPIO0->MASKED_ACCESS[((1<<6)|(1<<7))]=g<<6;
	LPC_GPIO1->MASKED_ACCESS[((1<<3)|(1<<4)|(1<<5))]=g>>2;
	LPC_GPIO2->MASKED_ACCESS[(1<<3)]=g>>1;

  __ASM volatile ("nop");
	CLR_E;
  __ASM volatile ("nop");
	SET_E;
	LPC_GPIO0->MASKED_ACCESS[((1<<6)|(1<<7))]=b<<6;
	LPC_GPIO1->MASKED_ACCESS[((1<<3)|(1<<4)|(1<<5))]=b>>2;
	LPC_GPIO2->MASKED_ACCESS[(1<<3)]=b>>1;

  __ASM volatile ("nop");
	CLR_E;
  __ASM volatile ("nop");

		//	ssd1351SendByte( (g>>2), 0 );
			
		//	ssd1351SendByte( (b>>2), 0 );
*/
#endif
#ifndef C262K
//			ssd1351SendByte( (r&0xF8) | (g>>5), 0 );
//			ssd1351SendByte( (b>>3) | ((g>>2)<<5), 0 );
#endif
/*			setLedXY(x*2, y*2, r,g,b);
			setLedXY(x*2, y*2+1, r,g,b);
			setLedXY(x*2+1, y*2, r,g,b);
			setLedXY(x*2+1, y*2+1, r,g,b);*/
			//setLedXY(x, y, h>>2,0,0);
		}
	}
	a++;
	zap++;
	if(a==1024)
	{
		a=0;
	}
	return 0;
}





