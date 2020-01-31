#include <stdio.h>

unsigned char x = 0, y = 0, z = 0, a = 1;

int main()
{


	while(1)
	{


		unsigned char t = x ^ (x >> 1);
		x=y;
		y=z;
		z=a;
		a = z ^ t ^ ( z >> 3) ^ (t << 1);

		printf("%i\n", a);

	}


}
