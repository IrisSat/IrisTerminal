
#include "utilities.h"

int getCharLocation(char * str, char c)
{
    int position = -1;
    int i;
    for(i=0; i < strlen(str); i++)
    {
        if(str[i] == c)
            return i;
    }
    return position;
}

void substring(char * str, char * substr1, int start, int end)
{
    printf("Substring: begin\n");
    int len = strlen(str);
    int substrlen = end - start + 1;
    if(len <= 0 || start > end || start > len || end > len)
        return;
    printf("Start = %d, End = %d, Length = %d\n",start,end,len);
    char substr[substrlen];
    int i;
    for(i=start; i < end; i++)
    {
        char c = str[i];
        printf("[%d] - %c\n",i,c);
        substr[i] = str[i];
        //substr += str[i];
        //strncat(substr,&c,1);
        //sprintf(substr,"%s%c",substr,str[i]);
    }
    for(i=0; i < substrlen; i++){
        printf("%c",substr[i]);
    }
    printf("\n");
}