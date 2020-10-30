#include <stdio.h>
#include <string.h>

/*function for password encrypt*/
void encrypt(char password[],int key)
{
    unsigned int i;
    for(i=0;i<strlen(password);++i)
    {
        password[i] = password[i] - key;
    }
}

/*function for password decrypt*/
void decrypt(char password[],int key)
{
    unsigned int i;
    for(i=0;i<strlen(password);++i)
    {
        password[i] = password[i] + key;
    }
}
int main()
{
    char password[20] ;
    printf("Enter the password: ");
    scanf("%s",password);
    printf("Passwrod = %s\n",password);
    
    /*
    shifting the user input char in ASCII table according to the key.
    10 in decimal, it will shift 10 places in ASCII table.
    Can change into any value key as user wants.
    */
    encrypt(password, 10);
    printf("Encrypted value = %s\n",password);
    decrypt(password, 10);
    printf("Decrypted value = %s\n",password);
    return 0;
}
