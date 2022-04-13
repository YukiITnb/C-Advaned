#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void menu(){
    printf("\n=========Menu=========\n");
    printf("1. Ma tran ke\n");
    printf("2. Danh sach ke\n");
    printf("3. \n");
    printf("4. \n");
    printf("5. Quit\n");
    printf("Choose: ");
}

int main()
{
   int choice;
   int n,m,i,j;
   do{
       menu();
       scanf("%d",&choice);
       
   }while(choice!=5);
   return 0;
   
}
