#define HOURS 24
#define SEGMENTS_PER_HOUR 2

uint8_t phase[3][HOURS*SEGMENTS_PER_HOUR]={

{0, 25, 50, 50, 55, 50, 0, 0, 0, 0, 0, 65, 100, 142, 165, 140, 120, 95, 0, 0, 0, 0, 0, 125, 255, 255, 255, 200, 140, 105, 37, 10, 0, 0, 0, 55, 75, 90, 110, 90, 75, 65, 50, 0, 0, 0, 0, 20, },
{0, 30, 0, 0, 0, 0, 20, 57, 73, 90, 110, 102, 100, 87, 60, 0, 0, 30, 120, 170, 200, 225, 250, 255, 255, 155, 90, 0, 0, 17, 37, 75, 100, 110, 110, 90, 75, 50, 0, 0, 0, 0, 0, 25, 40, 45, 45, 40, },
{0, 0, 8, 28, 50, 75, 100, 78, 73, 40, 0, 0, 0, 0, 95, 140, 175, 210, 205, 200, 200, 100, 0, 30, 0, 100, 90, 165, 160, 165, 175, 135, 100, 50, 0, 0, 0, 0, 5, 30, 40, 55, 60, 65, 40, 25, 0, 0, }
};