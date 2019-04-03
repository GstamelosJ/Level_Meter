
#include "average.h"

void Average(float *M) {
	#define LM_SIZE 10
	static float LM[LM_SIZE];      // LastMeasurements
	static uint8_t index = 0;
	static float sum = 0;
	static uint8_t count = 0;

	// keep sum updated to improve speed.
	sum -= LM[index];
	LM[index] = *M;
	sum += LM[index];
	index++;
	index = index % LM_SIZE;
	if (count < LM_SIZE) count++;
	*M=sum / count;
	//return sum / count;
}


/*uint16_t filter(uint16_t M, uint16_t std_variation)
{
	#define LM_SIZE 20
	static uint16_t LastM[LM_SIZE]; //last measurements
	static uint8_t countmax=0, countmin=0, indx = 0;
	static uint32_t sum = 0;
	static uint16_t max=0,max2=0, min=0, min2 =0;
	float result;
	LastM[indx] = M;
	if (M>max) max=M;
	//else if (M==max) countmax++;
	if ((M>max2) &&(max>max2)) max2=M;
	if (M<min) min=M;
	//else if (M==min) countmin++;
	if ((M<min2) && (min<min2)) min2=M;
	sum += LastM[indx];
	indx++;
	if (indx==LM_SIZE)
	{
		indx=0;
		if ((max - min)<std_variation)
		{
			result= sum/LM_SIZE;
		}
		else if ((max2-min2)<std_variation)
		{
			result= (sum - max -min)/(LM_SIZE-2);
			
		}
		else 
		result= min;
		sum=0;
		max=0;
		max2=0;
		min=0;
		min2=0;
		countmax=0;
		countmin=0;
		return result;	
		
	}
	else return 0;
	
	
	
	
}
*/