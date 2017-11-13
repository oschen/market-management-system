#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define NUM 500
#define MAXBRAND 100
#define MAXID 10
/* struct to define a good */

struct item{
	char brand[MAXBRAND];
	char id[MAXID];
	float in_price;
	float out_price;
	int   storage;
};
/* define a shopping cart */
struct item_node{
	struct item wanted;
	int    amount;
	struct item_node *next;
};

int menu();
void establish();
void dis_all();
void shop_cart();
int  cart_menu();
void add();
void display();
void calculate();
void clear_stdin();
int goodsnum=0;

struct item goods[NUM];
struct item_node *cart;


int main()
{
	printf("***********************************\n");
	printf("*******Welcome to the market*******\n");
	printf("***********************************\n");
	while(1)
	{
		switch(menu())
		{
		case 1:
			establish();break;
		case 2:
			dis_all();break;
		case 3:
			shop_cart();break;
		case 4:
			calculate();break;
		case 5:
			printf("Exit the market!\n");
			exit(0);
		}
	}
	  return 1;
}

int menu()
{
	char str[5];
	int  select,len;
	printf("\n\nHere is the menu\n");
	printf("1.establish the repo\n");
	printf("2.display all goods\n");
	printf("3.begin to shopping\n");
	printf("4.caculate the price\n");
	printf("5. exit\n");
	printf("choose from 1--5:\n");
	while(1)
	{
    //clear_stdin();
		scanf("%s",str);
		clear_stdin();
		select=atoi(str);
		if(select<1||select>5)
			printf("wrong selection:");
		else
			break;

	}
	return select;

}

void dis_all()
{
	int i;
	FILE  *fp;
	/* read from 'goods' file */
	fp=fopen("goods","r");
	if(!fp)
	{
		printf("open good file fail\n");
		exit(-1);
	}
	/*read item from file to good struct array*/
	for(i=0;(fread(goods+i,sizeof(struct item),1,fp))!=0;i++)
	{
		printf("---------------------------------\n");
		printf("display goods:\n");
		printf("%s%7s%7.2f%8d\n",goods[i].id,goods[i].brand,goods[i].out_price,goods[i].storage);

	}
	fclose(fp);
}


void shop_cart()
{
	while(1)
	{
		switch(cart_menu())
		{
		case 1:
			display();break;
		case 2:
			add();break;
		case 3:
			return;
		}
	}
}

int  cart_menu()
{
	char  str[5];
	int   select,len;
	printf("\ncart menu\n");
	printf("-----------------------\n");
	printf("1.display goods in shopping cart\n");
	printf("2.add good to shopping cart\n");
	printf("3.Exit\n");
	printf("-----------------------\n\n");
	while(1)
	{

		scanf("%s",str);
		clear_stdin();
		select=atoi(str);
		if(select<1||select>3)
			printf("wrong selection:");
		else
			break;
	}
	return select;
}

void display()
{
	struct item_node *p=cart;
	if(p==NULL){
		printf("shopping cart is empty\n");
		return ;
	}
	while(p!=NULL){
		printf("----------------------------------\n");
		printf("showing goods in cart\n");
		printf("%10s%20s%7.2f%8d\n",p->wanted.id,p->wanted.brand,p->wanted.out_price,p->amount);
		p=p->next;
	}
}

void add()
{
	FILE *fp;
	int   i,n,len;
	char  str[100];
	char  choice1,choice2;
	struct item_node *p,*p1;
	do
	{
		printf("begin add goods to shopping cart: ");
		scanf("%s",str);
		clear_stdin();
		if((fp=fopen("goods","r"))==NULL){
			printf("open goods file fail\n");
			continue;
		}
		for(i=0;fread(goods+i,sizeof(struct item),1,fp)!=0;i++){
			if((strcmp(goods[i].brand,str)==0||strcmp(goods[i].id,str)==0)&&goods[i].storage!=0){
				printf("this good exist:  \n");
				printf("---------------------\n");
				printf("show good info\n");
				printf("%s%6s%3.2f%4d\n",goods[i].id,goods[i].brand,goods[i].out_price,goods[i].storage);
				printf("how many goods you want to buy: ");
				scanf("%d",&n);
				clear_stdin();
				if(n>goods[i].storage){
					printf("good is more than storage\n");
					break;
				}
				printf("\nadd goods to shopping cart?(Y/N)");
				choice1=getchar();
				if(choice1=='Y'||choice1=='y'){
					p1=(struct item_node*)malloc(sizeof(struct item_node));
					if(p1==NULL){
						printf("malloc fail!\n");
						exit(1);
					}
					p1->amount=n;
					p1->wanted=goods[i];
					p1->next=NULL;
					p=cart;
					if(cart==NULL)
						cart=p1;
					else{
						while(p->next!=NULL)
							p=p->next;
						p1->next=p->next;
						p->next=p1;
					}
				}
				break;
			}
		}
		if(i==NUM)
			printf("good is not found in reposit\n");
		fclose(fp);
		printf("continue to add?(Y/N)");
		choice2=getchar();
	}while(choice2=='Y'||choice2=='y');
}


void  establish(){
	FILE *fp;
  static int i=0;
	char select;
	printf("You want re-initialize the repo from zero[y/n]\n");
	select=getchar();
	if(select=='y' or select=='Y')
	 { i=0;
		 goodsnum=0;
	 }
	printf("begin to initialize the reposit:\n");
	printf("----------------------------\n");
  while(true)
	{
		printf("initialize %d th good\n",i);
		printf("brand: ");
		scanf("%s",goods[i].brand);
		printf("id: ");
		clear_stdin();
		scanf("%s",goods[i].id);
		printf("price to buy in: ");
		clear_stdin();
		scanf("%f",&goods[i].in_price);
		printf("price to sell out: ");
		clear_stdin();
		scanf("%f",&goods[i].out_price);
		printf("store in reposit: ");
		clear_stdin();
		scanf("%d",&goods[i].storage);
		printf("\n");
		goodsnum+=1;
		printf("you want continue to input?[Y/N]\n");
    clear_stdin();
		select=getchar();
		i++;
		if(select!='Y' or goodsnum==NUM)
		   break;

	}
	if((fp=fopen("goods","w"))==NULL){
		printf("open file fail.\n");
		return;
	}
	fwrite(goods,sizeof(struct item),goodsnum,fp);
	fclose(fp);
}

void clear_stdin(void)
{
	while(getchar()!='\n');
}

void calculate()
{
	float total=0,pay;
	struct item_node *p;
	int   i;
	FILE   *fp;
	printf("show goods in shopping cart:  \n");
	display();
	if((fp=fopen("goods","r"))==NULL){
		printf("open file fail:  \n");
		return;
	}
	for(i=0;(fread(goods+i,sizeof(struct item),1,fp))!=0;i++);
	fclose(fp);
	p=cart;
	while(p!=NULL){
		total+=p->wanted.out_price*p->amount;
		for(i=0;strcmp(goods[i].id,p->wanted.id)!=0;i++);
		goods[i].storage-=p->amount;
		p=p->next;
	}
	printf("total price is %7.2f",total);
	printf("\nhow much you want pay: ");
	scanf("%f",&pay);
	clear_stdin();
	printf("pay:         %7.2f    change:          %7.2f",pay,pay-total);
	if((fp=fopen("goods","w"))==NULL){
		printf("open file fail.\n");
		return;
	}
	fwrite(goods,sizeof(struct item),NUM,fp);
	fclose(fp);
}
