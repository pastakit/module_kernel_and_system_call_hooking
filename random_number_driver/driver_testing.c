#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> // for open 
#include <unistd.h> // for read

#define DEVICE "/dev/random_number_driver" // define c device

int main(){
	int i,f;
	char buff[100];
	f=open(DEVICE,O_RDONLY); // 
	if(f ==-1)
	{
		printf("ERROR Open");
		exit(-1);
	}
	read(f,buff,sizeof(buff));
	printf("So ngau nhien doc duoc tu device la:%s\n",buff);
	return 0;
}