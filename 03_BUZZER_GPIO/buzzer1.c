#include <LPC214x.H>
int main()
{
		  int i,j;
		  IO1DIR|=(1<<25);
		  IO1SET|=(1<<25);
		  for(i=1;i<=10000;i++)
		  	for(j=1;j<=20000;j++);
		  IO1CLR|=(1<<25);
		  for(i=1;i<=10000;i++)
		  	for(j=1;j<=20000;j++);

		  IO1SET|=(1<<25);
		  for(i=1;i<=10000;i++)
		  	for(j=1;j<=20000;j++);
		  IO1CLR|=(1<<25);
		  for(i=1;i<=10000;i++)
		  	for(j=1;j<=20000;j++);
}