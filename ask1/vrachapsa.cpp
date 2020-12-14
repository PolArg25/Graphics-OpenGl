#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#define TEXTURE_ID_SCISSORS 1
#define TEXTURE_ID_ROCK 2
#define TEXTURE_ID_PAPER 3

using namespace std;

int winWidth = 15*40, winHeight = 12*40+55;
GLint gfPosX1=0,gfPosX2=40,gfPosY1=520,gfPosY2=480,gfDelta=40;
int counter,count=0;
int mx,my,mtimes,th1,th2,th,totalMoves,moves;
bool repeat=true;
float *textureS,*textureR,*textureP;
struct quad{
	int x1;
	int x2;
	int y1;
	int y2;
	float c1,c2,c3;
	int type;
	struct quad *next;
};
int arrayOfQuad[12][15];
struct quad *root;
struct quad *current;
struct quad *first;
struct quad *second;
struct quad *root2;
struct quad *current2;
int score=0;

void initialize(){
	glClearColor(0.6, 0.6, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0,winWidth,0,winHeight);
}

void grid(){
	int i;
	int j;
	for(i=gfPosX1; i<600; i+=gfDelta){
		glBegin(GL_LINES);
			glColor3f(0.0, 0.0, 0.0);
			glVertex2i(i,520);
			glVertex2i(i,40);
		glEnd();
		for(j=gfDelta; j<gfPosY1; j+=gfDelta){		
			glBegin(GL_LINES);
				glColor3f(0.0, 0.0, 0.0);
				glVertex2i(0,j);
				glVertex2i(600,j);
			glEnd();
		}	
	}
}

void image(){
	glBindTexture(GL_TEXTURE_2D, TEXTURE_ID_SCISSORS);
	string line,d=" ";
	float k;
	string t;
	int times=0,p=0;
	ifstream inFileS("scissors.pgm");
	int pos=0;
	textureS=(float *)malloc(1600*sizeof(float));
	while(getline(inFileS,line)){
		if(times>3){	
			while((pos = line.find(d))!= std::string::npos){
				t=line.substr(0,pos);
				k=atoi(t.c_str());
				textureS[p] = k/256.0;
				p++;
				line.erase(0,pos+d.length());		
			}		
		}
		times++;	
	}
	glBindTexture(GL_TEXTURE_2D, TEXTURE_ID_ROCK);
	ifstream inFileR("rock.pgm");
	pos=0;
	p=0;
	times=0;
	textureR=(float *)malloc(1600*sizeof(float));
	while(getline(inFileR,line)){
		if(times>3){	
			while((pos = line.find(d))!= std::string::npos){
				t=line.substr(0,pos);
				k=atoi(t.c_str());
				textureR[p] = k/256.0;
				p++;
				line.erase(0,pos+d.length());		
			}		
		}
		times++;	
	}
	glBindTexture(GL_TEXTURE_2D, TEXTURE_ID_PAPER);
	ifstream inFileP("paper.pgm");
	pos=0;
	p=0;
	times=0;
	textureP=(float *)malloc(1600*sizeof(float));
	while(getline(inFileP,line)){
		if(times>3){	
			while((pos = line.find(d))!= std::string::npos){
				t=line.substr(0,pos);
				k=atoi(t.c_str());
				textureP[p] = k/256.0;
				p++;
				line.erase(0,pos+d.length());			
			}		
		}
		times++;	
	}
}

void drawText(const char *text,int length, int x, int y){
	glMatrixMode(GL_PROJECTION);
	double *matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(0, 800, 0, 600, -5, 5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(x, y);
	for(int i=0; i<length; i++){
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
}

void redisplay(){
	glClear (GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
  	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	bool flag=true,flag2;
	while(flag){
		flag=false;
		current=root;
		for(int i=0;i<15;i++){	
			for(int j=0;j<12;j++){
				flag2=false;
				if(current->type==0){
					current->type=rand()% 5 + 1;
					flag2=true;
				}
				arrayOfQuad[j][i]=current->type;
				if(current->type==1){
	  				glBindTexture(GL_TEXTURE_2D, TEXTURE_ID_SCISSORS);
	  				glRasterPos2d(current->x1,current->y2);
	  				glDrawPixels(40,40,GL_LUMINANCE,GL_FLOAT,textureS);
				}
				else if(current->type==2){
					glBindTexture(GL_TEXTURE_2D, TEXTURE_ID_ROCK);
	  				glRasterPos2d(current->x1,current->y2);
	  				glDrawPixels(40,40,GL_LUMINANCE,GL_FLOAT,textureR);
				}
				else if(current->type==3){
					glBindTexture(GL_TEXTURE_2D, TEXTURE_ID_PAPER);
	 				glRasterPos2d(current->x1,current->y2);
	  				glDrawPixels(40,40,GL_LUMINANCE,GL_FLOAT,textureP);
				}
				else if(current->type==4){
				glBegin(GL_QUADS);
						glColor3f(1.0f, 0.0f , 0.0f);	
						glVertex2i(current->x1,current->y1);
						glVertex2i(current->x2, current->y1);
						glVertex2i(current->x2, current->y2);
						glVertex2i(current->x1, current->y2);
					glEnd();	
				}
				else{
					glBegin(GL_QUADS);
						glColor3f(0.0f, 0.0f , 1.0f);	
						glVertex2i(current->x1,current->y1);
						glVertex2i(current->x2, current->y1);
						glVertex2i(current->x2, current->y2);
						glVertex2i(current->x1, current->y2);
					glEnd();
				}
				
				if(j-1>=0 && flag2){//ama pesei petra panw se psalidi tote katastrefetai to psalidi(to flag2 mpainei mono otan dhmiourgeite ena neo tetragwno)
					if(arrayOfQuad[j][i]==1){
						if(arrayOfQuad[j-1][i]==3){
							arrayOfQuad[j-1][i]=0;
							score+=2;
							flag=true;					
						}
					}
					else if(arrayOfQuad[j][i]==2){
						if(arrayOfQuad[j-1][i]==1){
							arrayOfQuad[j-1][i]=0;
							score+=2;
							flag=true;					
						}
					}
					else if(arrayOfQuad[j][i]==3){
						if(arrayOfQuad[j-1][i]==1){
							arrayOfQuad[j-1][i]=0;
							score+=2;
							flag=true;					
						}
					}
				}
				if(j+1<=11 && flag2){
					if(arrayOfQuad[j][i]==1){
						if(arrayOfQuad[j+1][i]==3){
							arrayOfQuad[j+1][i]=0;
							score+=2;
							flag=true;					
						}
					}
					else if(arrayOfQuad[j][i]==2){
						if(arrayOfQuad[j+1][i]==1){
							arrayOfQuad[j+1][i]=0;
							score+=2;
							flag=true;					
						}
					}
					else if(arrayOfQuad[j][i]==3){
						if(arrayOfQuad[j+1][i]==1){
							arrayOfQuad[j+1][i]=0;
							score+=2;
							flag=true;					
						}
					}
				}
				if(i+1<=14 && flag2){
					if(arrayOfQuad[j][i]==1){
						if(arrayOfQuad[j][i+1]==3){
							arrayOfQuad[j][i+1]=0;
							score+=2;
							flag=true;					
						}
					}
					else if(arrayOfQuad[j][i]==2){
						if(arrayOfQuad[j][i+1]==1){
							arrayOfQuad[j][i+1]=0;
							score+=2;
							flag=true;					
						}
					}
					else if(arrayOfQuad[j][i]==3){
						if(arrayOfQuad[j][i+1]==1){
							arrayOfQuad[j][i+1]=0;
							score+=2;
							flag=true;					
						}
					}
				}
				if(i-1>=0 && flag2){
					if(arrayOfQuad[j][i]==1){
						if(arrayOfQuad[j][i-1]==3){
							arrayOfQuad[j][i-1]=0;
							score+=2;
							flag=true;					
						}
					}
					else if(arrayOfQuad[j][i]==2){
						if(arrayOfQuad[j][i-1]==1){
							arrayOfQuad[j][i-1]=0;
							score+=2;
							flag=true;					
						}
					}
					else if(arrayOfQuad[j][i]==3){
						if(arrayOfQuad[j][i-1]==1){
							arrayOfQuad[j][i-1]=0;
							score+=2;
							flag=true;					
						}
					}
				}
				if(flag){
					current2=root;
					for(int k=0;k<15;k++){
						for(int l=0;l<12;l++){
							current2->type=arrayOfQuad[k][l];
							current2=current2->next;
						}
					}
					break;
				}
				current=current->next;
			}
			if(flag){
				break;
			}		
		}
	}
	string data1;
	stringstream ss;
	ss << score;
	data1=ss.str();
	std::string text2;
	text2 = "SCORE: ";
	drawText(text2.data(),text2.size(), 500, 20);
	glLoadIdentity();
	drawText(data1.data(),text2.size(), 600, 20);
	glFlush();	
}

void fallOfQuad(){
	bool flag;
	for(int j=0;j<15;j++){
		flag=true;
		while(flag){
			for(int i=0;i<12;i++){//gia ta koutia pou einai apo panw
				if(arrayOfQuad[i][j]==0 && i-1>=0){
					if(arrayOfQuad[i-1][j]!=0){
						arrayOfQuad[i][j]=arrayOfQuad[i-1][j];
						arrayOfQuad[i-1][j]=0;
					}
				}
			}
			flag=false;
			for(int i=0;i<12;i++){
				if(arrayOfQuad[i][j]==0 && i-1>=0){
					if(arrayOfQuad[i-1][j]!=0){
						flag=true;
						break;
					}
				}
			}
		}
	}
}


void threeOfAKind(){
	int k,l,m,n,m2,type;
	counter=5;
	repeat=true;
	while(repeat && counter!=0){
		repeat=false;
		for(int j=0;j<15;j++){
			for(int i=0;i<10;i++){	
				if(arrayOfQuad[i][j]==arrayOfQuad[i+1][j] && arrayOfQuad[i][j]==arrayOfQuad[i+2][j]  && arrayOfQuad[i][j]!=0){
					type=arrayOfQuad[i][j];
					score+=10;
					if(type==4 || type==5){
						arrayOfQuad[i][j]=0;
						arrayOfQuad[i+1][j]=0;
						arrayOfQuad[i+2][j]=0;
					}
					else{
						if(i-3>=0 && i+5<=11){
							k=i-3;
							l=i+5;
						}
						else if(i-2==0){
							k=i-2;
							l=i+5;
						}
						else if(i-1==0){
							k=i-1;
							l=i+5;
						}
						else if(i==0){
							k=i;
							l=i+5;
						}
						else if(i+4==11){
							k=i-3;
							l=i+4;
						}
						else if(i+3==11){
							k=i-3;
							l=i+3;
						}
						else {
							k=i-3;
							l=i+2;
						}
						if(j-3>=0 && j+5<=14){
							m=j-3;
							n=j+3;
						}
						else if(j-2==0){
							m=j-2;
							n=j+3;
						}
						else if(j-1==0){
							m=j-1;
							n=j+3;
						}
						else if(j==0){
							m=j;
							n=j+3;
						}
						else if(j+2==14){
							m=j-3;
							n=j+2;
						}
						else if(j+1==14){
							m=j-3;
							n=j+1;
						}
						else {
							m=j-3;
							n=j;
						}
						for(k;k<l+1;k++){
							m2=m;
							for(m2;m2<n+1;m2++){
								if(k+3==i || k+2==i || k-5==i || k-4==i || m2+3==j || m2+2==j || m2-3==j || m2-2==j){//apostash 2 kai 3
									if(type==1){
										if(arrayOfQuad[k][m2]==3){
											arrayOfQuad[k][m2]=0;
											score+=3;
										}
									}
									if(type==2){
										if(arrayOfQuad[k][m2]==1){
											arrayOfQuad[k][m2]=0;
											score+=3;
										}
									}
									if(type==3){
										if(arrayOfQuad[k][m2]==2){
											arrayOfQuad[k][m2]=0;
											score+=3;
										}
									}
								}
								else if(m2==j && k==i || m2==j && k-1==i || m2==j && k-2==i){//apostash 0
									arrayOfQuad[k][m2]=0;
								}
								else{//apostash 1
									if(type==1){
										if(arrayOfQuad[k][m2]==3 || arrayOfQuad[k][m2]==4 || arrayOfQuad[k][m2]==5){
											arrayOfQuad[k][m2]=0;
											score+=2;
										}
										else{
											score-=1;
										}
									}
									if(type==2){
										if(arrayOfQuad[k][m2]==1 || arrayOfQuad[k][m2]==4 || arrayOfQuad[k][m2]==5){
											arrayOfQuad[k][m2]=0;
											score+=2;
										}
										else{
											score-=1;
										}
									}
									if(type==3){
										if(arrayOfQuad[k][m2]==2 || arrayOfQuad[k][m2]==4 || arrayOfQuad[k][m2]==5){
											arrayOfQuad[k][m2]=0;
											score+=2;
										}
										else{
											score-=1;
										}
									}
								}
								
							
							}
						}
					}
					fallOfQuad();
					current=root;
					for(int i=0;i<15;i++){
						for(int j=0;j<12;j++){
							current->type=arrayOfQuad[j][i];
							current=current->next;
						}
					}
					redisplay();
					counter--;
					repeat=true;
					break;
				}
				
			}
			if(repeat){//gia na vroume prwta stis kathetes grammes prwta kai kat deuteron stis orizonties
				break;
			}
		}
		if(repeat==false){
			for(int i=0;i<12;i++){
				for(int j=0;j<13;j++){	
					if(arrayOfQuad[i][j]==arrayOfQuad[i][j+1] && arrayOfQuad[i][j]==arrayOfQuad[i][j+2] && arrayOfQuad[i][j]!=0){
						score+=10;
						type=arrayOfQuad[i][j];
						if(type==4 || type==5){
							arrayOfQuad[i][j]=0;
							arrayOfQuad[i][j+1]=0;
							arrayOfQuad[i][j+2]=0;
						}
						else{
							if(j-3>=0 && j+5<=14){
								k=j-3;
								l=j+5;
							}
							else if(j-2==0){
								k=j-2;
								l=j+5;
							}
							else if(j-1==0){
								k=j-1;
								l=j+5;
							}
							else if(j==0){
								k=j;
								l=j+5;
							}
							else if(j+4==14){
								k=j-3;
								l=j+4;
							}
							else if(j+3==14){
								k=j-3;
								l=j+3;
							}
							else {
								k=j-3;
								l=j+2;
							}
							if(i-3>=0 && i+5<=11){
								m=i-3;
								n=i+3;
							}
							else if(i-2==0){
								m=i-2;
								n=i+3;
							}
							else if(i-1==0){
								m=i-1;
								n=i+3;
							}
							else if(i==0){
								m=i;
								n=i+3;
							}
							else if(i+2==11){
								m=i-3;
								n=i+2;
							}
							else if(i+1==11){
								m=i-3;
								n=i+1;
							}
							else {
								m=i-3;
								n=i;
							}
							for(m;m<n+1;m++){
								m2=k;
								for(m2;m2<l+1;m2++){
									if(m+3==i || m+2==i || m-3==i || m-2==i || m2+3==j || m2+2==j || m2-5==j || m2-4==j){
										if(type==1){
											if(arrayOfQuad[m][m2]==3){
												arrayOfQuad[m][m2]=0;
												score+=3;
											}
										}
										if(type==2){
											if(arrayOfQuad[m][m2]==1){
												arrayOfQuad[m][m2]=0;
												score+=3;
											}
										}
										if(type==3){
											if(arrayOfQuad[m][m2]==1){
												arrayOfQuad[m][m2]=0;
												score+=3;
											}
										}
									}
									else if(m2==j && m==i || m2-1==j && m==i || m2-2==j && m==i){
										arrayOfQuad[m][m2]=0;
									}
									else{
										if(type==1){
											if(arrayOfQuad[m][m2]==3 || arrayOfQuad[m][m2]==4 || arrayOfQuad[m][m2]==5){
												arrayOfQuad[m][m2]=0;
												score+=2;
											}
											else{
												score-=1;
											}
										}
										if(type==2){
											if(arrayOfQuad[m][m2]==1 || arrayOfQuad[m][m2]==4 || arrayOfQuad[m][m2]==5){
												arrayOfQuad[m][m2]=0;
												score+=2;
											}
											else{
												score-=1;
											}
										}
										if(type==3){
											if(arrayOfQuad[m][m2]==2 || arrayOfQuad[m][m2]==4 || arrayOfQuad[m][m2]==5){
												arrayOfQuad[m][m2]=0;
												score+=2;
											}
											else{
												score-=1;
											}
										}
									}
								}
							}
						}
						fallOfQuad();
						current=root;
						for(int i=0;i<15;i++){
							for(int j=0;j<12;j++){
								current->type=arrayOfQuad[j][i];
								current=current->next;
							}
						}
						redisplay();
						counter--;
						repeat=true;
						break;
					}
				
				}
				if(repeat){
					break;
				}
			}
		}
	}
}



void start(){
	glMatrixMode(GL_MODELVIEW);
  	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	int n;
	mtimes=0;
	moves=totalMoves;
	glClear (GL_COLOR_BUFFER_BIT);
	current=root;
	for(int i=0;i<15;i++){	
		for(int j=0;j<12;j++){
			n=rand()% 5 + 1;
			if(n==1){
				current->type=1;
  				current->c1=1.0f;
				current->c2=1.0f;
				current->c3=1.0f;
				current->x1=gfPosX1;
				current->x2=gfPosX2;
				current->y1=gfPosY1;
				current->y2=gfPosY2;
				arrayOfQuad[j][i]=current->type;
				current->next=(quad *)malloc(sizeof(struct quad));
				current=current->next;
				current->next=0;
				glBindTexture(GL_TEXTURE_2D, TEXTURE_ID_SCISSORS);
  				glRasterPos2d(gfPosX1,gfPosY2);
  				glDrawPixels(40,40,GL_LUMINANCE,GL_FLOAT,textureS);
				
			}
			else if(n==2){
				current->type=2;
				current->c1=1.0f;
				current->c2=1.0f;
				current->c3=1.0f;
				current->x1=gfPosX1;
				current->x2=gfPosX2;
				current->y1=gfPosY1;
				current->y2=gfPosY2;
				arrayOfQuad[j][i]=current->type;
				current->next=(quad *)malloc(sizeof(struct quad));
				current=current->next;
				current->next=0;
				glBindTexture(GL_TEXTURE_2D, TEXTURE_ID_ROCK);
  				glRasterPos2d(gfPosX1,gfPosY2);
  				glDrawPixels(40,40,GL_LUMINANCE,GL_FLOAT,textureR);
				
			}
			else if(n==3){
				current->type=3;
				current->c1=1.0f;
				current->c2=1.0f;
				current->c3=1.0f;
				current->x1=gfPosX1;
				current->x2=gfPosX2;
				current->y1=gfPosY1;
				current->y2=gfPosY2;
				arrayOfQuad[j][i]=current->type;
				current->next=(quad *)malloc(sizeof(struct quad));
				current=current->next;
				current->next=0;
				glBindTexture(GL_TEXTURE_2D, TEXTURE_ID_PAPER);
 				glRasterPos2d(gfPosX1,gfPosY2);
  				glDrawPixels(40,40,GL_LUMINANCE,GL_FLOAT,textureP);
				
			}
			else if(n==4){
				current->type=4;
				glBegin(GL_QUADS);
					glColor3f(1.0f, 0.0f , 0.0f);	
					glVertex2i(gfPosX1,gfPosY1);
					glVertex2i(gfPosX2, gfPosY1);
					glVertex2i(gfPosX2, gfPosY2);
					glVertex2i(gfPosX1, gfPosY2);
				glEnd();
				current->x1=gfPosX1;
				current->x2=gfPosX2;
				current->y1=gfPosY1;
				current->y2=gfPosY2;
				current->c1=1.0f;
				current->c2=0.0f;
				current->c3=0.0f;
				arrayOfQuad[j][i]=current->type;
				current->next=(quad *)malloc(sizeof(struct quad));
				current=current->next;
				current->next=0;
			}
			else {
				current->type=5;
				glBegin(GL_QUADS);
					glColor3f(0.0f, 0.0f , 1.0f);	
					glVertex2i(gfPosX1,gfPosY1);
					glVertex2i(gfPosX2, gfPosY1);
					glVertex2i(gfPosX2, gfPosY2);
					glVertex2i(gfPosX1, gfPosY2);
				glEnd();
				current->x1=gfPosX1;
				current->x2=gfPosX2;
				current->y1=gfPosY1;
				current->y2=gfPosY2;
				current->c1=0.0f;
				current->c2=0.0f;
				current->c3=1.0f;
				arrayOfQuad[j][i]=current->type;
				current->next=(quad *)malloc(sizeof(struct quad));
				current=current->next;
				current->next=0;
			}
			gfPosY1 -= gfDelta;
			gfPosY2 -= gfDelta;
		}
		gfPosY1 = 520;
		gfPosY2 = 480;
		gfPosX1 += gfDelta;
		gfPosX2 += gfDelta;
	}
	gfPosX1 = 0;
	gfPosX2 = 40;
	threeOfAKind();	
	glPixelStoref(GL_PACK_ALIGNMENT,1);
  	glPopMatrix();
  	glFlush();
}	

void display(){
	count++;
	grid();
	std::string text;
	text = "PRESS b TO START OR ESC TO EXIT. ";
	drawText(text.data(),text.size(), 30, 0);
	if(count==1){
		std::string text2;
		text2 = "SCORE: 0 ";
		drawText(text2.data(),text2.size(), 500, 20);
	}
	glLoadIdentity();
	glutPostRedisplay();
	glFlush();
}

void display2(){
	image();
	start();	
	glFlush();
}
void keyboardFunc(unsigned char key,int x,int y){
	switch (key){
		case 27:
			exit(0);
		case 98:
			score=0;
			display2();		
	}	
}

void exchange(){
	float color1,color2,color3;
	current=root;
	th=1;
	while(current->next!=0){
		if(first->type==4 && second->type==5 || first->type==5 && second->type==4){
			if(th==th1){
					current->type=second->type;
				
			}
			if(th==th2){
					current->type=first->type;
			}
			current=current->next;
			th++;
		}
		else if(first->type!=4 && second->type!=5 || first->type!=5 && second->type!=4){
			if(th==th1){
				current->type=second->type;
			}
			if(th==th2){
				current->type=first->type;
			}
			current=current->next;
			th++;
		}
		else{
			if(first->type<4){			
				if(th==th1){
					current->type=second->type;
				}
				if(th==th2){
					current->type=first->type;

				}
				current=current->next;
				th++;
			}
			else{
				if(th==th1){
					current->type=second->type;
				}
				if(th==th2){
					current->type=first->type;
				}
				current=current->next;
				th++;
			}
		}
	}
	current=root;
	for(int i=0;i<15;i++){
		for(int j=0;j<12;j++){
			arrayOfQuad[j][i]=current->type;
			current=current->next;
		}
	}	
	glutPostRedisplay();
	threeOfAKind();
	redisplay();
}

void move(int mx,int my){	
	int k=0;
	int th=1;
	current=root;
	while(k==0){	
		if(current->x1 < mx && current->x2 > mx && current->y1 > my && current->y2 < my){
			k=1;
			break;
		}
		th++;
		current=current->next;
	}
	mtimes++;
	if(mtimes==1){
		th1=th;
		first->type=current->type;
		first->x1=current->x1;
		first->x2=current->x2;
		first->y1=current->y1;
		first->y2=current->y2;
		if(current->type==4 || current->type==5){
			first->c1=current->c1;
			first->c2=current->c2;
			first->c3=current->c3;
		}
	}
	else{
		th2=th;
		second->type=current->type;
		second->x1=current->x1;
		second->x2=current->x2;
		second->y1=current->y1;
		second->y2=current->y2;
		if(current->type==4 || current->type==5){
			second->c1=current->c1;
			second->c2=current->c2;
			second->c3=current->c3;
		}
		if(first->x1==second->x1 && first->x2==second->x2 && first->y1==second->y1 && first->y2==second->y2){
			mtimes=0;
		}
		else{
			if(first->x1+40==second->x2 && first->y1==second->y2 && moves!=0 || first->x1+40==second->x2 && first->y2==second->y1 && moves!=0 || first->y1-40==second->y2 && first->x1==second->x2 && moves!=0 || first->y1-40==second->y2 && first->x2==second->x1 && moves!=0){
			
				exchange();
				mtimes=0;
				th1=0;
				th2=0;	
				moves--;
			}
			if(moves==0){
				std::string text;
				text = "GAME OVER. ";
				drawText(text.data(),text.size(), 30, 20);
			
			}
		}
	}
}

void mousebutton(int button,int state,int x,int y){
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		mx=x;
		my=winHeight-y;
		move(mx,my);
	}	
}

void reshape(GLsizei w, GLsizei h) {
   if(h == 0)
		h = 1;
	float ratio = 1.0* w / h;

	

}

int main (int argc, char** argv){
	cout<<"EISHGAGE ARITHMO KINHSEWN: ";
	cin>>totalMoves;
	root=(quad *)malloc(sizeof(struct quad));
	first=(quad *)malloc(sizeof(struct quad));
	second=(quad *)malloc(sizeof(struct quad));
	root->next=0;
	current=root;
	glutInit (&argc, argv);
	srand(time(NULL));
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); 
	glutInitWindowPosition (150, 150);
	glutInitWindowSize (winWidth, winHeight);
	glutCreateWindow ("vrachapsa");
	initialize();
	glutReshapeFunc(reshape);
	glutDisplayFunc (display);
	glutKeyboardFunc (keyboardFunc);
	glutMouseFunc(mousebutton);
	glutMainLoop();
	free(first);
	free(second);
	free(root);
	free(current);
	free(root2);
	free(current2);	
	return 0;
}
