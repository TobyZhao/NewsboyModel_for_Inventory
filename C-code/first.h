#include <stdlib.h>
#include <math.h>
/****************************************************************
*				parameters						*
*	amount:			amount of order						*
*	bigs:			maximum inventory					*
*	initil: 		initial inventory					*
*	invlev:			dynamic inventory										*
*	nevnts: 		number of events										*
*	next:			next event										*
*	nmnths：	仿真时间（月）									*
*	smalls：	无需订货的库存下限								*
*	npolcy：	策略数目										*
*	nvalue：	需求量											*
*	p1:			每件缺货损失费									*
*	i：			循环变量										*
*	pro[]：		需求概率密度									*
*	mdemdt：	平均需求间隔时间								*
*	setupc：	订货附加费用									*
*	incrmc：	每件订货费										*
*	H：			每件每月保管费									*
*	tne[]：		时间轴，分别记录事件发生的时间					*
/***************************************************************/
int amount,bigs,initil=60,invlev,nevnts=4,next,nmnths=120,smalls,I,npolcy=4;
int nvalue=4,i,p1=5,small1[20],big1[20];
static double pro[99]={0,1.0/6,0.5,0.8330,1};
double mdemdt=0.1,setupc=32,incrmc=3,H=1,z=1,tne[5],price=10,money=0;
double aminus,aplus,tlevent,tordc,time;
double acost[20],ahldc[20],aordc[20],ashrc[20],pri[20];

/****************************************************************
* 函数名： 	drand()												*
* 输入参数： double z											*
* 输出参数： double												*
* 函数功能：产生0~Z的连续随机数							 		*
/***************************************************************/
double drand(double z)
{
	double a=rand()%10000;
	a=a/10000;
	return a*z;
}

/****************************************************************
* 函数名： 	irandi()											*
* 输入参数： int												*
* 输出参数： int												*
* 函数功能：产生随机需求。根据0~1的连续随机数结合分布概率生成	*
/***************************************************************/
int irandi(int z)
{
	int i,n1,res=1;
	double u=drand(z);
	n1=nvalue-1;
	for (i=1;i<=n1;i++)
	{
		if(u>= pro[i])
		{
			res=i+1;
		}

	}
	return res;
}

/****************************************************************
* 函数名： 	unifrm()											*
* 输入参数： double a，double b									*
* 输出参数： double												*
* 函数功能：产生[a,b]连续随机数 								*
/***************************************************************/
double unifrm(double a,double b)
{
	double u,aa;
	u=drand(z);
	aa=a+u*(b-a);
	return aa;

}

/****************************************************************
* 函数名： 	expon()												*
* 输入参数： double rmean										*
* 输出参数： double												*
* 函数功能：生成均值为rmean的指数随机数 						*
/***************************************************************/
double expon( double rmean)
{
	return -1*rmean*log(drand(1));
}

/****************************************************************
* 函数名： 	update()											*
* 输入参数： void												*
* 输出参数： void												*
* 函数功能：计算库存积分量或者缺货积分量，用于计算相关费用		*
/***************************************************************/
void update()
{
	double tsle;
	tsle=time-tlevent;
	tlevent=time;
	if(invlev<0)
	{
		aminus=aminus+(-1*invlev*tsle);
	}

	else if(invlev >0)
		aplus=aplus+(invlev*tsle);
	//printf("update->  aplus=%f,	aminus=%f\n",aplus,aminus);
	return;
}
