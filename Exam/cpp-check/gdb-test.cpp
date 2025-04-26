#include <stdio.h>
#include <unistd.h>

int main(int argc,char * argv[])
{
    char * pStr="helloworld\n";
    printf("%s\n",pStr);
    pStr[1] = 'y';
    
    return 0; 
}
