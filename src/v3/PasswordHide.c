#include<stdio.h>
#include <termios.h>
#include <unistd.h>

int main()
{	
	char str[20],c=' ';
	int i=0;
	
	printf("\n Enter the password [max length 10] : ");
	
	while (i<=9)
	{
	    str[i]=getch();
	    c=str[i];
	    if(c==13) break;
	    else printf("*");
	    i++;
	}
	
	str[i]='\0';
	i=0;
	printf("\n");
	printf("\n Your password is : %s",str);
	return 0;
}


int getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
