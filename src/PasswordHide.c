#include<stdio.h>
#include<termios.h>
#include<unistd.h>

int main()
{
	int i = 0;
	char ch;
	char pwd[100];

	printf("Enter your password. Hit ENTER to confirm.\n");
	printf("Password (MAX 100 char):");

	while(1)
	{
/* 
getch() will get the keyboard input. Use ASCII table to compare the char values.
33 is beginning of characters (including special char) and 126 is the end of char.
Linux doesn't have libary for getchar(). So it is defined as new function.
*/
		ch = getch();
		if(ch >= 33 && ch <= 126)
		{
			pwd[i++] = ch;
			printf("* \b");	
		}
/*
8 value is equal to backspace in ASCII table. So it removes the last enter char and i--.
*/
		else if(ch == 8 || ch == 127)
		{
			if(i > 0)
			{
				i--;
				printf("\b \b");		
			}
		}
/*	
If user put other char outside of 33 to 126, it will break. For example: it will break at enter or tab.
*/
		else
		{
			pwd[i] = '\0';
			break;			
		}
	}

	printf("\nPassword:%s\nLength:%d\n", pwd, i);
	return 0;
}

int getch()
{
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
