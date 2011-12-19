#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define LOG(a,b) (log(a)/log(b))
void main()
{
//	unsigned long long a=0x00ffffffffffffff;
//	printf("%lld sizeof:%d\n",a,sizeof(long long int));
/*	long long int x=0x00ffffffffffffff;
	long long int y=0x00fffffffffffe64;//e65 equal
	double res =log(x);
	double res2=log(y);
	res=log(x)+res;
	if(res2<res)
		printf("x big x:%3.15f y:%3.15f\n",res,res2);
	else if(res2==res)
		printf("equal big x:%3.15f y:%3.15f\n",res,res2);
	else
		printf("y big x:%3.15f y:%3.15f\n",res,res2);
*/
/*	FILE *fp=NULL;
	int n=0;
	int i=0;
	int j=0;
	long long int a[20]={0};
	int  b[20][20]={0};
	char ch;

	fp=fopen("xx.log","r+");
	if(fp==NULL)
	{
		printf("fp is Null\n");
		fp=fopen("xx.log","w+");
	}
	else
	{	
		printf("open ok\n");
	}
	i=0;
//cal n
	while(!feof(fp)&&(ch=fgetc(fp))!='\n')
		if(ch==',')
			n++;
	printf("n:%d\n",n);
	
	fseek(fp,0L,0);
		
			for(i=0;i<=n;i++)
			{
				fscanf(fp,"%lld:",&a[i]);
				for(j=0;j<=n;j++)
					fscanf(fp,"%d,",&b[i][j]);
			}
	for(i=0;i<=n;i++)
	{
		printf("%lld:",a[i]);
		for(j=0;j<=n;j++)
			printf("%d ",b[i][j]);
		printf("\n");
	}
	fclose(fp);*/
	int *p=malloc(100*sizeof(int));
	int i=0;
	for(i=0;i<100;i++)
	{
		p[i]=0;
		printf("%d ",p[i]);
	}
	printf("\n");
}
