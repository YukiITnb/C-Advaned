#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void loadfile(FILE *fp){
    int edg,ver1,ver2;
    int i,n,m,time;
    fflush(stdin);
    fscanf(fp,"%d",&n);
    printf("%d\t",n);
    fflush(stdin);
    fscanf(fp,"%d",&m);
    printf("%d\n",m);
    while(!feof(fp)){
        fflush(stdin);
        fscanf(fp,"%d",&ver1);
        printf("%d\t",ver1);
        fflush(stdin);
        fscanf(fp,"%d",&ver2);
        printf("%d\t",ver2);
        fflush(stdin);
        fscanf(fp,"%d",&time);
        printf("%d\n",time);
    }
}
int main(){
    int n,m,i,j;
    FILE *fp;
    fp = fopen("dothi.txt","r");
    loadfile(fp);
    fclose(fp);
    return 0;
}
