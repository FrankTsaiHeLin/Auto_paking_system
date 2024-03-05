#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <new>
#include <cmath>
#include <graphics.h>
#include <time.h>

using namespace std;

const float Pi=3.1415926;
int wx=1200, wy=500;
int x_offset=30, y_offset=54;
int wx1=wx+2*x_offset;
int wy1=wy+2*y_offset;
float lapse=0.01;
float wLane=60;

class point
{
	public:
	float x, y;
};

class car
{
	public:
	int id;
	point pCurrent;
	point pNext;
	char tCurrent;//type 'l' for straight line, 'c' for circular move
	char tNext;
	
	float velocity;//=16;//16m per second, 60km per hour
	float acceleration;//=0;
	float rMin;//最小迴轉半徑
	float rMax;
	float aMin;//最小夾角
	float dResponse;//反應距離 
	//point direction;//vector
	//int status=0; //0 for straight line, 1 for arc
	point b[4];
	float length;
	float width;
	int pre;//current position just pass through path[pre]
	int nCorner;
	point *path;
	
	car()
	{
		id=14;
		velocity=5;
		acceleration=5;
		rMax=80;
		rMin=70;
		aMin=2;
		width=wLane/2;
		length=width*2;
		dResponse=rMin*3;
		return;
	}	
	
	car(float v,float a,float l,float w,float m,float d)
	{
		velocity=v;
		acceleration=a;
		rMin=m;
		length=l;
		width=w;
		dResponse=d;
		//direction=d;
		return;
	}
	
	void DrawCar(float x,float y,point d)
	{//length=70, width=30  7:3=2.3:1
		int shape[30]={27,-15,31,-13,35,-8,35,8,31,13,27,15,-22,15,-27,14,-32,13,-35,7,-35,-7,-32,-13,-27,-14,-22,-15,27,-15};
		int window[24]={15,13,1,11,-27,9,-29,0,-27,-9,1,-11,15,-13,-29,-11,-32,-8,-32,8,-29,13,15,13};
		int windshield[14]={17,-12,18,0,17,12,6,10,7,0,6,-10,17,-12};
		int lights[16]={/*r*/27,-15,27,-11,30,-8,35,-8,/*l*/35,8,30,8,27,11,27,15};
		int wheel[32]={/*rf*/20,-15,15,-16,11,-16,6,-15,/*lf*/20,15,15,16,11,16,6,15,/*rr*/-14,-15,-19,-16,-23,-16,-27,-14,/*lr*/-14,15,-19,16,-23,16,-27,14};
		int carshape[32];
		int wLine=2;	
		int i;
		float zoom=4;//zoom
		
		setlinestyle(0,0,wLine);
		//car shape 
		for (i=0;i<30;i=i+2)
		{
			carshape[i]=(int) (x+(d.x*shape[i]-d.y*shape[i+1])*width/70*zoom);
			carshape[i+1]=(int) (y+(d.y*shape[i]+d.x*shape[i+1])*width/70*zoom);	
		}
		drawpoly(15,carshape);
		
		//car window
		for (i=0;i<24;i=i+2)
		{
			carshape[i]=(int) (x+(d.x*window[i]-d.y*window[i+1])*width/70*zoom);
			carshape[i+1]=(int) (y+(d.y*window[i]+d.x*window[i+1])*width/70*zoom);	
		}
		drawpoly(12,carshape);
		
		//car windshield
		for (i=0;i<14;i=i+2)
		{
			carshape[i]=(int) (x+(d.x*windshield[i]-d.y*windshield[i+1])*width/70*zoom);
			carshape[i+1]=(int) (y+(d.y*windshield[i]+d.x*windshield[i+1])*width/70*zoom);	
		}
		drawpoly(7,carshape);
		
		//lights
		for (i=0;i<16;i=i+2)
		{
			carshape[i]=(int) (x+(d.x*lights[i]-d.y*lights[i+1])*width/70*zoom);
			carshape[i+1]=(int) (y+(d.y*lights[i]+d.x*lights[i+1])*width/70*zoom);	
		}
		drawpoly(4,carshape);
		drawpoly(4,carshape+8);
		
		//wheel
		for (i=0;i<32;i=i+2)
		{
			carshape[i]=(int) (x+(d.x*wheel[i]-d.y*wheel[i+1])*width/70*zoom);
			carshape[i+1]=(int) (y+(d.y*wheel[i]+d.x*wheel[i+1])*width/70*zoom);	
		}
		drawpoly(4,carshape);
		drawpoly(4,carshape+8);
		drawpoly(4,carshape+16);
		drawpoly(4,carshape+24);
		
		return;
	}
	
	void Parking(point p,point d)//car parking
	{//length=98,width=38  5.25:2.5,2.4:1
		int shape[10]={-35,-20,63,-20,63,20,-35,20,-35,-20};
		int carshape[10];	
		int i;
		float zoom=5.25;//zoom
		
		for (i=0;i<10;i=i+2)
		{
			carshape[i]=(int) (p.x+(d.x*shape[i]-d.y*shape[i+1])*29/80*zoom);
			carshape[i+1]=(int) (p.y+(d.y*shape[i]+d.x*shape[i+1])*29/80*zoom);
		}

		//Parking
		setcolor(15);
		setlinestyle(0,0,1);
		drawpoly(5,carshape);
		setfillstyle(SOLID_FILL,15);
		floodfill((carshape[0]+carshape[2])/2,(carshape[1]+carshape[7])/2,15);
		setfillstyle(SOLID_FILL,0);
		floodfill((carshape[0]+carshape[2])/2,(carshape[1]+carshape[7])/2,14);
		
		return;
	}
	
	void obstacle(point p,point d)//car parking
	{//length=98,width=38  5.25:2.5,2.4:1
		int shape[10]={-35,-19,63,-19,63,19,-35,19,-35,-19};
		int carshape[10];	
		int i;
		float zoom=5.25;//zoom
		
		for (i=0;i<10;i=i+2)
		{
			carshape[i]=(int) (p.x+(d.x*shape[i]-d.y*shape[i+1])*29/80*zoom);
			carshape[i+1]=(int) (p.y+(d.y*shape[i]+d.x*shape[i+1])*29/80*zoom);
		}

		setcolor(14);
		line(0,carshape[1],3000,carshape[1]);
		line(0,carshape[7],3000,carshape[7]);
		
		//color
		setfillstyle(SOLID_FILL,14);
		floodfill(0,(carshape[1]+carshape[7])/2,14);
		
		return;
	}
	
float Dot(point v0,point v1)//v0 dot v1
{
	v0=unitv(v0);
	v1=unitv(v1);
	
	return v0.x*v1.x+v0.y*v1.y;
}

float Cross(point v0,point v1)//v0 cross v1
{
	v0=unitv(v0);
	v1=unitv(v1);
	
	return v0.x*v1.y-v0.y*v1.x;
}

float Angle(point v0,point v1)//center->p0 to center->p1 angle
{
	return acos(Dot(unitv(v0),unitv(v1)));
}

float pdistance(point p0,point p1)//p0~p1 distance
{
	return pow(pow(vector(p0,p1).x,2)+pow(vector(p0,p1).y,2),0.5);
}

float vdistance(point v)//vector distance
{
	return pow(pow(v.x,2)+pow(v.y,2),0.5);
}

point vector(point p0,point p1)//p0->p1
{
	point v;
	
	v.x=p1.x-p0.x;
	v.y=p1.y-p0.y;

	return v;
}

point unitv(point v)//unit vector
{
	float distance;
	
	distance=vdistance(v);
	
	v.x/=distance;
	v.y/=distance;
	
	return v;
}

point normal_vector(int n,point vector)//n=1 inside,n=-1 outside
{
	point v;
	
	v.x=-vector.y*n;
	v.y=vector.x*n;
	
	return v;
}

point central_point(int n,point c,point v)//find center point
{
	point uv;
	point p;
	
	uv=unitv(v);

	p.x=c.x-uv.y*rMin*n;
	p.y=c.y+uv.x*rMin*n;

	return p;
} 

void drive(int direction,point p1, point p2)//straight move
{
	int color=id;
	float distance=pdistance(p1,p2);
	int n=distance/velocity;
	point d;
	
	d.x=unitv(vector(p1,p2)).x*direction;
	d.y=unitv(vector(p1,p2)).y*direction;

	for (int i=0;i<n;i++)
	{
		setcolor(color);
		//DrawCar((p1.x+i*(p2.x-p1.x)/n),(p1.y+i*(p2.y-p1.y)/n),d);
		DrawCar((p1.x+i*(p2.x-p1.x)/n),(p1.y+i*(p2.y-p1.y)/n),d);		
		delay((int)1000*lapse);		
		//if(i%10<9)setcolor(0);
		setcolor(0);
		//DrawCar((p1.x+i*(p2.x-p1.x)/n),(p1.y+i*(p2.y-p1.y)/n),d);
		DrawCar((p1.x+i*(p2.x-p1.x)/n),(p1.y+i*(p2.y-p1.y)/n),d);		
		//putpixel((p1.x+i*(p2.x-p1.x)/n),(p1.y+i*(p2.y-p1.y)/n),color);
	}
	return;	
}

void cdrive(int direction,point p1,point p2,point center)//circular move
{
	int color=id;
	point temp1,temp2,p0;
	point d;
	int n;
	
	temp1=vector(center,p1);
	temp2=vector(center,p2);
	
	float r=vdistance(temp1);
	float distance=pdistance(p1,p2);
	float angle=abs(asin(Cross(temp1,temp2)));
	float angle1=asin(temp1.y/r);
	int sign;


	if(Dot(temp1,temp2) < 0) angle=Pi-angle;
	if(Cross(temp1,temp2) > 0)
		sign=1;
	else
		sign=-1;
	
	if (temp1.x<0) 
		if (angle1>0)
			angle1=Pi-angle1;
		else
			angle1=-(Pi+angle1);
		
	setcolor(color);

	n=r*abs(angle)/velocity;

	p0.x=p1.x;
	p0.y=p1.y;

	for (int i=1;i<n;i++)
	{
		d.x=(cos(sign*i*angle/n+angle1)-cos(sign*(i-1)*angle/n+angle1))*direction;
		d.y=(sin(sign*i*angle/n+angle1)-sin(sign*(i-1)*angle/n+angle1))*direction; 
		d=unitv(d);
		p0.x=center.x+r*cos(sign*i*angle/n+angle1);
		p0.y=center.y+r*sin(sign*i*angle/n+angle1);
		setcolor(color);
		DrawCar(p0.x,p0.y,d);		
		delay((int)1000*lapse);		
		//if(i%10<9)setcolor(0);
		setcolor(0);
		DrawCar(p0.x,p0.y,d);
		//putpixel(p0.x,p0.y,color);
	}

	return;
}

float vChange(float angle)
{
	float wt=4-2*(angle-aMin)/(Pi-aMin);
	return velocity/wt;
}

point cut_point(int dircetion,point p0 ,point center0,point center1)//1 right,-1 left
{
	point p;
	point d;
	float distance;
	float theta;
	
	d=unitv(vector(center0,center1));
	distance=pdistance(center0,center1);
	
	theta=acos(rMin/(distance/2));
	p.x=p0.x+rMin*(d.x*cos(dircetion*theta)-d.y*sin(dircetion*theta));
	p.y=p0.y+rMin*(d.x*sin(dircetion*theta)+d.y*cos(dircetion*theta));
	
	return p;
}

void Cmove(point p0,point v0,int clock0,point p1,point v1,int clock1)
{
	point center0;//dResponse central point
	point center1;//pend central point
	point ptemp;//another p0 point
	point d01;//vector
	point p2,p3,p4,p5;//Turning point
	point p8,p9;//endpoint another
	point p6,p7;//deResponse Reverse point
	float distance01;//distanceij i~j
	float angle;//change velocity angle
	
	//dResponse central point
	if(v0.x>=0)
		if(p1.y>p0.y)
			center0=central_point(1,p0,v0);
		else
			center0=central_point(-1,p0,v0);
	else
		if(p1.y>p0.y)
			center0=central_point(-1,p0,v0);
		else
			center0=central_point(1,p0,v0);
	//pend central point
	if(v1.x>=0)
		if(p1.y>p0.y)
			center1=central_point(-1,p1,v1);
		else
			center1=central_point(1,p1,v1);	
	else
		if(p1.y>p0.y)
			center1=central_point(1,p1,v1);
		else
			center1=central_point(-1,p1,v1);
			
	//dResponse to pend distance
	//distance01=pdistance(center0,center1);
	
	//dResponse to pend vector
	//d01=unitv(vector(center0,center1));
		
	//inside cut point
	p2=cut_point(1,center0,center0,center1);//center0->center1 right
	p3=cut_point(-1,center0,center0,center1);//center0->center1 left
	p4=cut_point(-1,center1,center1,center0);//center1->center0 left
	p5=cut_point(1,center1,center1,center0);//center1->center0 right
	
	//move
	if(p1.y<p0.y)
	{	
		if(v0.x<=0)
		{
			cout<<"01->"<<endl;
			cdrive(-1,p0,p2,center0);
			drive(-1,p2,p5);
			cdrive(-1,p5,p1,center1);
		}
		else
		{
			cout<<"02->"<<endl;
			cdrive(-1,p0,p3,center0);
			drive(-1,p3,p4);
			cdrive(-1,p4,p1,center1);	
		}
	}			
	else
	{	
		if(v0.x>0)
		{
			cout<<"03->"<<endl;
			cdrive(-1,p0,p2,center0);
			drive(-1,p2,p5);
			cdrive(-1,p5,p1,center1);
		}
		else
		{
			cout<<"04->"<<endl;
			cdrive(-1,p0,p3,center0);
			drive(-1,p3,p4);
			cdrive(-1,p4,p1,center1);
		}
	}
	return;
}

void move(point c0,point v0,point c1,point v1)
{	
	point pst,pend,pResponse,ptemp;//central point
	point a1;
	point temp,pT;//turing center point
	point p2,p3;//dResponse point
	point p4,p5,p6,p7;//cut point
	point pTemp0,pTemp1;//Turning point
	point d01;//vector
	float distance01,Rdistance;//distanceij i->j 
	float clock0,clock1;//car turning point & car end direction
	int v;
	
	getch();
	
	if(c0.y+rMax>=c1.y-rMin/3)
		if(v0.y>=0)
		{
			cout<<">";
			a1.x=c0.x-unitv(v0).x*rMin*2;
			a1.y=c0.y-unitv(v0).y*rMin*2;
			v=velocity;
			velocity=3;
			drive(-1,c0,a1);
			velocity=v;
			c0=a1;
		}
		else
		{
			cout<<"<";
			a1.x=c0.x+unitv(v0).x*rMin*2;
			a1.y=c0.y+unitv(v0).y*rMin*2;
			v=velocity;
			velocity=3;
			drive(1,c0,a1);
			velocity=v;
			c0=a1;
		}
	
	//Find central point
	if(c1.x<c0.x)
		if(v0.x>=0)	
			if(v0.y>=0)
				pst=central_point(-1,c0,v0);
			else
				pst=central_point(1,c0,v0);
		else
			if(v0.y>=0)
				pst=central_point(1,c0,v0);
			else
				pst=central_point(-1,c0,v0);	
	else		
		if(v0.x>=0)
			if(v0.y>=0)
				pst=central_point(1,c0,v0);
			else
				pst=central_point(1,c0,v0);
		else
			if(v0.y>=0)
				pst=central_point(1,c0,v0);
			else
				pst=central_point(1,c0,v0);
	
	if(v1.x>=0)//target central point
		if(c1.y>c0.y)
			pend=central_point(-1,c1,v1);
		else
			pend=central_point(1,c1,v1);
	else
		if(c1.y>c0.y)
			pend=central_point(1,c1,v1);
		else
			pend=central_point(-1,c1,v1);
	//------------------------------
	setcolor(15);
	//circle(pst.x,pst.y,rMin);
	//circle(pend.x,pend.y,rMin);
	setcolor(13);
	//circle(pst.x,pst.y,rMax);
	//circle(pend.x,pend.y,rMax);
	//------------------------------
	
	//dResponse center point
	temp.x=pend.x+unitv(v1).x*dResponse;
	temp.y=pend.y;
	
	pResponse.x=pend.x+unitv(v1).x*dResponse;
	if(c1.y>c0.y)
		pResponse.y=pend.y-abs(unitv(v1).x)*2*rMin;
	else
		pResponse.y=pend.y+abs(unitv(v1).x)*2*rMin;
	//turning 
	pT.x=temp.x;
	if(c1.y>c0.y)
		pT.y=temp.y-rMin;
	else
		pT.y=temp.y+rMin;
	//distance p0~p1
	distance01=pdistance(pst,pResponse);
	
	//unit vector pst~pend	
	d01=unitv(vector(pst,pResponse));
	
	//pst turning point
	//left
	pTemp1.x=pst.x-d01.y*rMin;
	pTemp1.y=pst.y+d01.x*rMin;
	
	//right
	pTemp0.x=pst.x+d01.y*rMin;
	pTemp0.y=pst.y-d01.x*rMin;
	
	//pst cut point
	p4=cut_point(1,pst,pst,pResponse);//right
	p7=cut_point(1,pResponse,pResponse,pst);//right
	p5=cut_point(-1,pst,pst,pResponse);//left
	p6=cut_point(-1,pResponse,pResponse,pst);//left
	
	//dResponse point
	//right
	p2.x=pTemp1.x+d01.x*distance01;
	p2.y=pTemp1.y+d01.y*distance01;
	//left 
	p3.x=pTemp0.x+d01.x*distance01;
	p3.y=pTemp0.y+d01.y*distance01;
	
	if(Cross(v0,vector(c0,pst))>=0)//car start direction
		clock0=1;
	else
		clock0=-1;	
	
	if(Cross(v1,vector(c1,pResponse))>=0)//car end direction
		clock1=1;
	else
		clock1=-1;
	
	//------------------------------
	setcolor(3);
	//circle(pResponse.x,pResponse.y,rMin);
	//circle(temp.x,temp.y,rMin);
	//setcolor(12);
	//circle(pResponse.x,pResponse.y,rMax);
	//------------------------------

	if(c0.x>=pResponse.x)
	{
		if(Cross(v0,vector(c0,pst))>=0)//car start direction
		{
			velocity=vChange(Angle(vector(pst,c0),vector(pst,pTemp0))/3);
			if(Cross(vector(pst,pTemp0),vector(pst,c0))>=0)//clockwise or counterclockwise
			{	
				cout<<"1->";
				cdrive(-1,c0,pTemp0,pst);
				
			}
			else
			{
				cout<<"2->";	
				cdrive(1,c0,pTemp0,pst);
			}
			velocity=4;
			drive(1,pTemp0,p3);
			velocity=vChange(Angle(vector(pResponse,p3),vector(pResponse,pT))/2);
			if(Cross(vector(pResponse,pT),vector(pResponse,p3))<0)
				cdrive(1,p3,pT,pResponse);
			else
				cdrive(-1,p3,pT,pResponse);
			Cmove(pT,v1,clock0,c1,v1,clock1);	
		}	
		else
		{	
			velocity=vChange(Angle(vector(pst,c0),vector(pst,p4))/3);
			if(Cross(vector(pst,p4),vector(pst,c0))>=0)//clockwise or counterclockwise
			{
				cout<<"3->";
				cdrive(1,c0,p4,pst);		
			}
			else
			{
				cout<<"4->";
				cdrive(-1,c0,p4,pst);
			}
			velocity=4;
			drive(1,p4,p7);
			velocity=vChange(Angle(vector(pResponse,p7),vector(pResponse,pT))/2);
			if(Cross(vector(pResponse,pT),vector(pResponse,p7))<0)
				cdrive(1,p7,pT,pResponse);
			else
				cdrive(-1,p7,pT,pResponse);
			Cmove(pT,v1,clock0,c1,v1,clock1);
		}	
	}
	else
	{
		if(Cross(v0,vector(c0,pst))>=0)//car start direction
		{
				velocity=vChange(Angle(vector(pst,c0),vector(pst,p5))/3);
				if(Cross(vector(pst,p5),vector(pst,c0))>=0)//clockwise or counterclockwise
			{	
				cout<<"5->";
				cdrive(-1,c0,p5,pst);
			}
			else
			{
				cout<<"6->";	
				cdrive(1,c0,p5,pst);
				
			}
			velocity=4;
			drive(1,p5,p6);
			velocity=vChange(Angle(vector(pResponse,p6),vector(pResponse,pT))/2);
			if(Cross(vector(pResponse,pT),vector(pResponse,p6))<0)
				cdrive(-1,p6,pT,pResponse);
			else
				cdrive(1,p6,pT,pResponse);
			Cmove(pT,v1,clock0,c1,v1,clock1);
		}
		else
		{	
			velocity=vChange(Angle(vector(pst,c0),vector(pst,p5))/3);
			if(Cross(vector(pst,p5),vector(pst,c0))>=0)//clockwise or counterclockwise
			{
				cout<<"7->";
				cdrive(1,c0,p5,pst);		
			}
			else
			{
				cout<<"8->";
				cdrive(-1,c0,p5,pst);
			}
			velocity=4;
			drive(1,p5,p6);
			velocity=vChange(Angle(vector(pResponse,p6),vector(pResponse,pT))/2);
				if(Cross(vector(pResponse,pT),vector(pResponse,p6))<0)
				cdrive(-1,p6,pT,pResponse);
			else
				cdrive(1,p6,pT,pResponse);
			Cmove(pT,v1,clock0,c1,v1,clock1);
		}
	}
	setcolor(15);
	//DrawCar(c0.x,c0.y,unitv(v0));
	return ;
}

void move1(point c0,point v0,point c1,point v1)
{	
	point pst,pend,dResponse;//central point
	point a1;
	point b[4]={966,288,780,288,780,211,966,211};
	point p6,p7;//outside cut point
	point d01,d12,d23;//vector
	point p4,p5;//inside cut point
	point p8,p9;//b[1]->b[2]
	point p10,p11;//b[2]->dResponse
	point p12,p13;//dResponse->pend
	float distance01,distance12,distance23;//distanceij i->j 
	int v;
	
	/*
	getch();
	
	if(c0.y+rMax>=c1.y-rMin/3)
		if(v0.y>=0)
		{
			cout<<">";
			a1.x=c0.x-unitv(v0).x*rMin*2;
			a1.y=c0.y-unitv(v0).y*rMin*2;
			v=velocity;
			velocity=3;
			drive(-1,c0,a1);
			velocity=v;
			c0=a1;
		}
		else
		{
			cout<<"<";
			a1.x=c0.x+unitv(v0).x*rMin*2;
			a1.y=c0.y+unitv(v0).y*rMin*2;
			v=velocity;
			velocity=3;
			drive(1,c0,a1);
			velocity=v;
			c0=a1;
		}
	*/
	
	//Find central point
	if(c1.x<c0.x)
	{
		if(v0.x>=0)	
			if(v0.y>=0)
				pst=central_point(-1,c0,v0);
			else
				pst=central_point(1,c0,v0);
		else
			if(v0.y>=0)
				pst=central_point(1,c0,v0);
			else
				pst=central_point(-1,c0,v0);
	}
	else
	{
		if(v0.x>=0)	
			if(v0.y>=0)
				pst=central_point(1,c0,v0);
			else
				pst=central_point(-1,c0,v0);
		else
			if(v0.y>=0)
				pst=central_point(-1,c0,v0);
			else
				pst=central_point(1,c0,v0);
	}		
	
	if(c1.y>c0.y)//target central point
		pend=central_point(-1,c1,v1);
	else
		pend=central_point(1,c1,v1);

	
	//inside cut point
	p4=cut_point(-1,pst,pst,b[1]);//pst->b[1] right
	p5=cut_point(-1,b[1],b[1],pst);//b[1]->pst right
	
	//outside cut point
	distance01=pdistance(pst,b[1]);//pst->b[1] distance
	
	d01=unitv(vector(pst,b[1]));//pst->b[1] vector
	
	//pst outside cut point
	p6.x=pst.x-d01.y*rMin;
	p6.y=pst.y+d01.x*rMin;
	
	//b[1] outside cut point
	p7.x=b[1].x-d01.y*rMin;
	p7.y=b[1].y+d01.x*rMin;
	
	//b[1]->b[2] outside cut point
	distance12=pdistance(b[1],b[2]);//b[1]->b[2] distance
	
	d12=unitv(vector(b[1],b[2]));//b[1]->b[2] vector
	
	//b[1] outside cut point
	p8.x=b[1].x-d12.y*rMin;
	p8.y=b[1].y+d12.x*rMin;
	
	//b[1] outside cut point
	p9.x=b[2].x-d12.y*rMin;
	p9.y=b[2].y+d12.x*rMin;
	
	/*
	//-------------------------------------
	setcolor(13);
	setlinestyle(0,0,2);
	
	circle(b[1].x,b[1].y,rMin);
	setcolor(14);
	setlinestyle(0,0,1);
	circle(p4.x,p4.y,10);
	circle(p5.x,p5.y,10);
	//-------------------------------------
	*/
	
	//pst vector->b[1]
	if(Cross(v0,vector(c0,pst))>=0)
	{
		velocity=vChange(Angle(vector(pst,p4),vector(pst,c0))/2);
		if(Cross(vector(pst,p4),vector(pst,c0))>=0)
		{
			cdrive(-1,c0,p4,pst);
		}
		else
		{
			cdrive(1,c0,p4,pst);
		}
		velocity+=3;
		drive(1,p4,p5);
		velocity=vChange(Angle(vector(b[1],p5),vector(b[1],p8))/2);
		cdrive(1,p5,p8,b[1]);
	}
	else
	{
		velocity=vChange(Angle(vector(pst,p4),vector(pst,c0))/2);
		if(Cross(vector(pst,p6),vector(pst,c0))>=0)
		{
			cdrive(1,c0,p6,pst);
		}
		else
		{
			cdrive(-1,c0,p6,pst);
		}
		velocity+=3;
		drive(1,p6,p7);
		velocity=vChange(Angle(vector(b[1],p7),vector(b[1],p8))/2);
		cdrive(1,p7,p8,b[1]);
	}
	velocity+=2;
	drive(1,p8,p9);
	
	//b[1]->b[2] outside cut point
	distance23=pdistance(b[2],pend);//b[1]->b[2] distance
	
	d23=unitv(vector(b[2],pend));//b[1]->b[2] vector
	
	//b[1] outside cut point
	p10.x=b[1].x+d12.x*(distance12+rMin);
	p10.y=b[1].y+d12.y*(distance12+rMin);
	
	//dResponse outside cut point
	p11.x=p10.x+d12.y*(distance23+2.3*rMin);
	p11.y=p10.y-d12.x*(distance23+2.3*rMin);
	
	velocity=vChange(Angle(vector(b[2],p9),vector(b[2],p10))/2);
	cdrive(1,p9,p10,b[2]);
	
	velocity+=2;
	drive(1,p10,p11);
	
	dResponse=central_point(1,p11,normal_vector(1,d12));
	
	p12=cut_point(-1,dResponse,dResponse,pend);
	p13=cut_point(-1,pend,pend,dResponse);
	
	velocity=vChange(Angle(vector(dResponse,p11),vector(dResponse,p12))/2);
	cdrive(-1,p11,p12,dResponse);
	drive(-1,p12,p13);
	cdrive(-1,p13,c1,pend);
	
	getch();
	
	return ;
}

};

int nobstacle;//number of obstacle
int *npoint;//every obstacle point
float **disp;//between start to target point disp
point **Eobstacle;//expansion obstacle point

point inputp();
point inputv();
void initmove(point,point,point,point);
void initobstacle();
void mobilecase1();
void mobilecase2();
void drawpoint(int,point*);
void lsort(int,point*);
void bsort(int,point*,float*);

int main()
{
	car c;
	point start={1000,100},target={500,500};
	point vstart={1,0},vtarget={1,0};
	
	srand (time(NULL));
	initwindow(wx1,wy1);
	
	mobilecase1();
	
	getch();
	closegraph();

	return 0;
}

point inputp()//input p
{
	point p;
	//cout<<"Input point"<<endl<<"x: ";
	//cin>>p.x;
	p.x=rand()%800+200;
	//cout<<"y: ";
	//cin>>p.y;
	p.y=rand()%180+100;
	//p.y=rand()%100+450;
	return p;
}

point inputv()//input v
{
	point v;
	//cout<<"Input vector"<<endl<<"x: ";
	//cin>>v.x;
	v.x=rand()%200-100;
	//cout<<"y: ";
	//cin>>v.y;
	v.y=rand()%200-100;

	return v;
}

void lsort(int n,point *v)
{
	car c;
	point v0={1,0},Vtemp;//v0基準 , Vtemp向量暫存 
	float *list;
	
	list=new float[n];
	
	for(int i=0;i<n;i++)list[i]=0;
	
	for(int i=0;i<n;i++)
	{
		Vtemp.x=(v+i)->x;
		Vtemp.y=(v+i)->y;
		if(Vtemp.y>0)
			list[i]=acos(c.Dot(v0,Vtemp));
		else
			list[i]=acos(c.Dot(v0,Vtemp))*-1+360;
		if(list[i]==360)list[i]=0;//360=0		
	}
	bsort(n,v,list);

	return;
}

void bsort(int n,point *v,float *list)
{
	point Vtemp;
	float ftemp;
	if(n<=0)return;
	for(int i=0;i<n-1;i++)
	{
		if(*(list+i)>*(list+i+1))//not*  is 位址 
		{
			ftemp=*(list+i);
			*(list+i)=*(list+i+1);
			*(list+i+1)=ftemp;
			
			Vtemp.x=(v+i)->x;
			Vtemp.y=(v+i)->y;
			(v+i)->x=(v+i+1)->x;
			(v+i)->y=(v+i+1)->y;
			(v+i+1)->x=Vtemp.x;
			(v+i+1)->y=Vtemp.y;
		}
	}
	bsort(n-1,v,list);
	
	return;
}

void drawpoint(int n_points,point *p)
{
	int *points;
	
	points=new int[n_points*2];
	
	for(int i=0,j=0;i<n_points,j<n_points*2;i++,j+=2)
	{
		points[j]=(p+i)->x;
		points[j+1]=(p+i)->y;
		//cout<<(p+i)->x<<" "<<(p+i)->y<<endl;
	}
	drawpoly(n_points,points);
	
	return ;
}

void mobilecase1()
{
	car c;
	point cst,cend;//car source and target point
	point vst,vend;//car source and target vector
	point temp;
	//source
	cst=inputp();//source point
	//cst.x=100;
	//cst.y=100;
	vst=inputv();//source vector
	//vst.x=1;
	//vst.y=0;
	//target
	//target point
	cend=inputp();
	//cend.x=1000;
	cend.y=500;

	//target vector
	if(vst.x>0)
		if(cend.x<cst.x)
			vend.x=-1;
		else
		 	vend.x=1;
	else
		if(cend.x<cst.x)
			vend.x=-1;
		else
		 	vend.x=1;
	vend.y=0;

	//Parking
	setcolor(15);
	c.obstacle(cend,c.unitv(vend));//side parking obstacle
	c.Parking(cend,c.unitv(vend));//target parking
	//move
	//initmap();
	c.move(cst,vst,cend,vend);
	
	return ;
}

void mobilecase2()
{
	car c;
	point cst,cend;//car source and target point
	point vst,vend;//car source and target vector
	point temp;
	//source
	//cst=inputp();//source point
	//cst.x-=100;
	//cst.y+=350;
	cst.x=rand()%300+100;
	cst.y=rand()%100+480;
	vst=inputv();//source vector
	//vst.x=1;
	//vst.y=0;
	//target
	//target point
	//cend=inputp();
	//cend.x=1000;
	cend.x=300;
	cend.y=250;
	temp.x=900;
	temp.y=250;

	//target vector
	vend.x=-1;
	vend.y=0;
	
	//Parking
	
	c.obstacle(cend,c.unitv(vend));//side parking obstacle
	setcolor(15);
	c.Parking(cend,c.unitv(vend));//target parking
	c.Parking(temp,c.unitv(vend));//space
	
	//c.DrawCar(cst.x,cst.y,c.unitv(vst));
	//getch();
	
	//move
	c.move1(cst,vst,cend,vend);
	
	return ;
}

void initobstacle()
{
	car c;
	int nobject=5;//move object point
	float Xdisp=50,Ydisp=0;
	
	point object[nobject];//object point
	
	point **obstacle;//original obstacle point
	point *Nobstacle;//normal vector obstacle
	point *Vobstacle;//vector obstacle
	
	//point Nobstacle[7];//normal vector obstacle
	//point Vobstacle[7];//vector obstacle
	//point Eobstacle[8];//expansion obstacle point
	
	//object point
	object[0].x=0;
	object[0].y=50;
	object[1].x=50;
	object[1].y=0;
	object[2].x=0;
	object[2].y=-50;
	object[3].x=-50;
	object[3].y=0;
	object[4].x=object[0].x;
	object[4].y=object[0].y;
	
	//input obstacle point
	FILE *fp;
	fp=fopen("obstacle.txt","r");//call txt
	if(!fp)return;
	
	fscanf(fp,"%d",&nobstacle);//the number of obstacle
	
	obstacle=new point*[nobstacle];//empty the number of obstacle
	Eobstacle=new point*[nobstacle];
	disp=new float*[nobstacle];
	npoint=new int[nobstacle];//empty the number of obstacle's point
	
	for(int i=0;i<nobstacle;i++)
	{
		fscanf(fp,"%d",&npoint[i]);//the number of obstacle's point
		npoint[i]++;
		obstacle[i]=new point[npoint[i]];//empty the number of obstacle's point
		Eobstacle[i]=new point[npoint[i]];//empty the number of espansion obstacle's point
		disp[i]=new float[npoint[i]];
		for(int j=0;j<npoint[i]-1;j++)
		{
			fscanf(fp,"%f %f",&obstacle[i][j].x,&obstacle[i][j].y);//input obstacle's point
		}
		obstacle[i][npoint[i]-1].x=obstacle[i][0].x;
		obstacle[i][npoint[i]-1].y=obstacle[i][0].y;
		
		//expansion coculation N&V
		Nobstacle=new point[nobject+npoint[i]-2];
		Vobstacle=new point[nobject+npoint[i]-2];
		
		//object normal vector
		for(int j=0;j<nobject-1;j++)Nobstacle[j]=c.normal_vector(1,c.vector(object[j],object[j+1]));
		
		//obstacle normal vector
		for(int j=0;j<npoint[i]-1;j++)Nobstacle[nobject-1+j]=c.normal_vector(-1,c.vector(obstacle[i][j],obstacle[i][j+1]));

		//sort normal vector
		lsort(nobject+npoint[i]-2,Nobstacle);
		
		//expansion obstacle vector 
		for(int j=0;j<nobject+npoint[i]-2;j++)Vobstacle[j]=c.normal_vector(1,Nobstacle[j]);
		
		//expansion obstacle point
		Eobstacle[i][0].x=obstacle[i][0].x+Xdisp;
		Eobstacle[i][0].y=obstacle[i][0].y+Ydisp;
		for(int j=0;j<nobject+npoint[i]-2;j++)
		{
			Eobstacle[i][j+1].x=Eobstacle[i][j].x+Vobstacle[j].x;
			Eobstacle[i][j+1].y=Eobstacle[i][j].y+Vobstacle[j].y;
		}
		Eobstacle[i][nobject+npoint[i]-2]=Eobstacle[i][0];

		/*
		//draw expansion obstacle
		setcolor(9);
		drawpoint(nobject+npoint[i]-1,Eobstacle[i]);
		*/
				
		//draw obstacle
		setcolor(14);
		drawpoint(npoint[i],obstacle[i]);

	}
	
	return ;
}

void initmove(point p0,point v0,point p1,point v1)
{
	if(p0.x==p1.x && p0.y==p1.y)return;
	
	car c;
	point Vjudge;//p0 -> p1 vector 
	point Vtemp;//p0 ->any obstacle point
	int temp,temp1;//compare variable
	int t=0,t1=0;//L,R point number
	int *R,*L;//Lside or Rside point number
	float theta;//Vjudge and Vtemp theta
	float X[nobstacle]={0};//one obstacle any point all distance
	float Lside=0.0,Rside=0.0;//L<0  R>0 
	float m;//start to end line's slope 
	float D;//judge lRside  R>0,L<0
	
	Vjudge=c.unitv(c.vector(p0,p1));//p0 -> p1 unit vector
	m=(p1.y-p0.y)/(p1.x-p0.x);//Slope 
	
	//load distacne of every obstacle point
	for(int i=0;i<nobstacle;i++)
	{
		for(int j=0;j<(npoint[temp]-1)*2;j++)
		{
			//load every obstacle point
			Vtemp=c.vector(p0,Eobstacle[i][j]);
			theta=c.Angle(Vjudge,c.unitv(Vtemp));
			disp[i][j]=c.vdistance(Vtemp)*sin(theta);
			X[i]+=disp[i][j];
		}
		//average
		X[i]/=npoint[i];		
	}
	
	//find the recent obstacle
	for(int i=0;i<nobstacle-1;i++)
		if(X[i]<X[i+1])
			temp=i;
		else
			temp=i+1;


	//judge Rside or Lside
	for(int i=0;i<(npoint[temp]-1)*2;i++)
	{
		D=m*(Eobstacle[temp][i].x-p0.x)-Eobstacle[temp][i].y+p0.y;
		if(D>0)
		{
			Rside+=disp[temp][i];
			t++;
		}	
		else
		{
			Lside+=disp[temp][i];
			t1++;
		}			
	}
	//average
	Rside/=(float)t;
	Lside/=(float)t1;
	
	cout<<Rside<<endl<<Lside;
	
	R=new int[t];
	L=new int[t1];
	
	//record R,L point
	for(int i=0,j=0,k=0;i<(npoint[temp]-1)*2;i++)
	{
		D=m*(Eobstacle[temp][i].x-p0.x)-Eobstacle[temp][i].y+p0.y;
		if(D>0)
			R[j++]=i;
		else
			L[k++]=i;		
	}
	
	//find recent obstacle's recent point
	if(Rside<Lside)//smaller
	{
		temp1=R[0];
		for(int i=1;i<t;i++)
			if(c.pdistance(p0,Eobstacle[temp][temp1]) >= c.pdistance(p0,Eobstacle[temp][R[i]]))
				temp1=R[i];
	}
	else
	{
		temp1=L[0];
		for(int i=1;i<t1;i++)
			if(c.pdistance(p0,Eobstacle[temp][temp1]) >= c.pdistance(p0,Eobstacle[temp][L[i]]))
				temp1=L[i];
	}     
	//for(int i=0;i<t;i++)             
	//	circle(Eobstacle[temp][R[i]].x,Eobstacle[temp][R[i]].y,20);
	
	setcolor(14);
	circle(p0.x,p0.y,20);
	circle(Eobstacle[temp][temp1].x,Eobstacle[temp][temp1].y,20);
	circle(p1.x,p1.y,20);
	
	c.drive(1,p0,Eobstacle[temp][temp1]);
	setcolor(14);
	line(p0.x,p0.y,p1.x,p1.y);
	
	//c.move();
	//initmove(Eobstacle[temp][temp1],v0,p1,v1);

	return;
}

