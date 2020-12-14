#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <ctype.h>
#include "SOIL.h"

GLuint texture; //the array for our texture
GLuint tex_2d,tex_2d2,tex_2d3;
   
int times=0,lines,player=1,rotate=1,xmax,ymax;
int **t;
bool firstTime=true;
int winWidth=15*40;
int winHeight=12*40+55;
float red = 1.0f, green = 1.0f, blue = 1.0f;
float angle = 0.0f,angle1 = 0.0f,angley=0.0f;
float moveF=0.0,moveFy=0.0;
int N ;
int L ;
int K ;
float deltaAngle = 0.0f,deltaAngley = 0.0f;
int xOrigin = -1,yOrigin=-1;
int ri,rj,posi,posj,posiOneMove,posjOneMove,posiScore,posjScore;
int NumberOfMoves=0,NumberOfHammers=0,score;
double fi,fj,fz=0.0f;
bool cylinder=true,camUp=true,moveDown,moveUp,moveRight,moveLeft,oneMove=false;
// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f,lx1=0.0f,lz1=-1.0f,ly=0.0f;
// XZ position of the camera
float xx=40.0f,yy=-220.0f,angleR=0,xx1=50.0f,z,z2=350.0f;
GLUquadricObj *obj = gluNewQuadric();
bool elevator_up=false,endOfGame=false;
int el=0;

using namespace std;

void findPos(){	//vriskei thn thesh, sthn opoia vrisketai o paixths sto paixnidi, ston pinaka twn timwn gia to kathe kouti t[][]
	posi=(8.0*N-fj)/8.0+(fz/30)*N;	//suntetagmenes gia ta kelia tou pinaka
	posj=fi/(80/N);
}

void findNotPossibleMoves(){	//vriskei tis kinhseis pou mporei na kanei kai na mhn kanei o paixths
	findPos();
	int s=8.0*N-(posi-(fz/30)*N)*8.0-4.0,s2=(posj)*(80/N)+(80/N)/2;	//mexri poso konta mporei na plhsiasei o paixths se ena kouti 
	if(posi+1<N*(el+1) && posi-1>=0 && posj+1<N*(el+1) && posj-1>=0){	//an einai sto eswteriko tou lavurinthou
		if(t[posi+1][posj]!=7 && fj<s){
			moveDown=false;
		}
		else{
			moveDown=true;
		}
		if(t[posi-1][posj]!=7 && fj>s){
			moveUp=false;
		}
		else{
			moveUp=true;
		}
		if(t[posi][posj+1]!=7 && fi>s2){
			moveRight=false;
		}
		else{
			moveRight=true;
		}
		if(t[posi][posj-1]!=7 && fi<s2){
			moveLeft=false;
		}
		else{
			moveLeft=true;
		}	
	}
	else if(posj==0 || posj%(N)==0 || posi==0 || posi%(N)==0){	//an einai sta akra tou lavurinthou (sthn arxh tou)
		if(posi==0){
			if(fj>s){
				moveUp=false;
			}
			else{
				moveUp=true;
			}
		}
		else{
			if(fj>s && t[posi-1][posj]!=7){
				moveUp=false;
			}
			else{
				moveUp=true;
			}
		}
		if(t[posi+1][posj]!=7 && fj<s ){
			moveDown=false;
		}
		else{
			moveDown=true;
		}	
		if(t[posi][posj-1]!=7 && fi<s2 && fi<=0){
			moveLeft=false;
		}
		else{
			moveLeft=true;
		}
		if(t[posi][posj+1]!=7 && fi>s2){
			moveRight=false;
		}
		else{
			moveRight=true;
		}
	}	
	else{	//an einai sta akra tou lavurinthou (sto telos tou)
		if(t[posi+1][posj]!=7 && fj<s){
				moveDown=false;
		}
		else{
			moveDown=true;
		}
		if(t[posi-1][posj]!=7 && fj>s){
				moveUp=false;
		}
		else{
			moveUp=true;
		}

		if(t[posi][posj-1]!=7 && fi<s2){
			moveLeft=false;
		}
		else{
			moveLeft=true;
		}
		if(t[posi][posj+1]!=7 && fi>s2){
			moveRight=false;
		}
		else{
			moveRight=true;
		}
	}
}

void elevator(){	//anevainei epipedo o paixths
	findPos();
	if(elevator_up && t[posi+N][posj]==7 && el<L-1){
		el++;
		fz=30*el;
		posi+=N;
		posiOneMove=posi;	//suntetagmenes gia ta kelia tou pinaka
		posjOneMove=posj;
		elevator_up=false;
	}
	else{
		elevator_up=false;
	}
}

void findRandomQuad(){	//vriskei tuxaia mia kenh thesh gia ton paixth
	int ni,nj;
	bool flag=true;	
	while(flag){
		ni=rand()% N ;
		nj=rand()% N ;
		if(t[ni][nj]==7){	//an einai kenh h thesh,dhladh na mhn uparxei kouti
			posiScore=ni;	//suntetagmenes gia ta kelia tou pinaka
			posjScore=nj;
			ri=ni;	//suntetagmenes gia ta kelia tou pinaka
			rj=nj;
			flag=false;
		}
	}
}

void moves(){	//metraei tis kinhseis tou paixth
	findPos();
	if(posiScore!=posi || posjScore!=posj){
		NumberOfMoves++;
		posiScore=posi;
		posjScore=posj;
	}
}

void fallDown(){
	findPos();
	if(posi!=posiOneMove && el>0 || posj!=posjOneMove && el>0){
		oneMove=true;
	}
	if(posi-N>=0 && el>0 && oneMove){
		while(t[posi-N][posj]==7){
			el--;
			fz=30*el;
			posi-=N;
			posiOneMove=posi;
			posjOneMove=posj;
			oneMove=false;
		}
	}	
}

void user(void){
	if(firstTime){	//otan ksekinaei to paixnidi vriskei tuxaia mia kenh thesh gia ton paixth
		findRandomQuad();
		firstTime=false;
	}
	else{	
		elevator();
		fallDown();
	}
	fi=(rj)*(80/N)+(80/N)/2+moveFy;	//oi suntetagmenes thesews x,y sto parathuro(to ri einai gia ton yy' kai to rj gia to xx')
	fj=8.0*N - (ri)*8.0-4.0+moveF;
	cout<<fi<<" "<<fj<<endl;	//(-4.0) kai (+(80/N)/2) gia na topotheththei o paixths sto kentro tou koutiou,moveF kai moveFy gia na proxwraei
	if(cylinder){	//dhmiourgei ton kulindro-paixth
		glColor3f(1.0f,0.5f,0.0f);
		glTranslatef(fi,fj,fz);
		glBegin(GL_POLYGON);
		gluCylinder(obj, 1.0, 1.0, 7.0, 400.0, 500.0);
		glEnd();
		glFlush();
	}
}

void initialize(){
	glMatrixMode(GL_PROJECTION);
	glEnable (GL_DEPTH_TEST); //enable the depth testing
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
}

void grid(void) {
	int i=0;
	int j,ep,q=0,cond,numberOfRows=N*L;
	float p=0,y,x,k;
	z=(L/2)*30; //otan peristrefetai o lavirinthos h cam na einai sthn mesh
	//Set the camera
	cylinder=true;
	if(rotate%2==0){	//h camera peristrefetai gurw apo ton lavurintho 
		glRotatef(angleR, 0.0f ,angleR , 0.0f);	
			gluLookAt(	xx, yy, z,
				xx+lx1, yy+100.0f,  z+lz1,
				0.0f, 1.0f,  0.0f);
			
		if(angleR==360){
			angleR=0;
		}
		if(angleR<=90){			
			angleR++;
			xx-=3.11f;
			yy+=3.22f;
		}
		else if(angleR<=180){
			angleR++;
			xx+=3.11f;
			yy+=3.22f;	
		}
		else if(angleR<=270){
			angleR++;
			xx+=3.11f;
			yy-=3.22f;	
		}
		else if(angleR<=360){
			angleR++;
			xx-=3.11f;
			yy-=3.22f;	
		}
	}
	else{
		if(player%2==0){	//h camera emfanizetai mesa sto paixnidi kai deixnei ekei pou koitaei o paixths(fps)
		cylinder=false;
		gluLookAt(	fi, fj,fz+2.0,
				fi+lx,fj-20.0+lz,  fz+2.0f+ly,
				0.0f, 0.0f,  1.0f);
		
		}	
		else{			//h camera deixnei ton lavurintho apo panw 
			gluLookAt(	xx1, 50.0f, z2,
				xx1+lx1, 50.0F,  z2+lz1,
				0.0f, 1.0f,  0.0f);
		}
	}
	for(ep=0;ep<1;ep++){	//dhmiourgei ola ta epipeda
	
		q=80*N;//kathe fora pou allazei to N oi kathetes grammes tou plegmatos na ftanoun ta oria twn orizontiwn grammwn
		x=0;
		while(x<=80){	//dhmiourgei tis kathetes grammes tou plegmatos kathe epipedou
			glBegin(GL_LINES);
				glColor3f(0.0f, 0.0f, 0.0f);
				glVertex3f(x, 0.0f,30*ep);     //10*ep: gia na exoun diafora +10 ta epipeda metaksu tous
	  			glVertex3f(x, q/10.0,30*ep);
			glEnd();
		
			x+=80/N; //gia na exei kathetes grammes pou na isapexoun metaksu tous kai gia na ginoun N sthles 
		
		}
		xmax=x;
		y=0;
		for(j=0; j<=N;j++){	//dhmiourgei tis orizonties grammes tou plegmatos kathe epipedou
			glBegin(GL_LINES);
				glColor3f(0.0f, 0.0f, 0.0f);
				glVertex3f(80.0f,y,30*ep);
				glVertex3f(0.0f,y,30*ep);
			glEnd();
			

			y+=8.0;
		}
		ymax=y-8.0;
		y=ymax;	

		for(i;i<numberOfRows;i++){    
			x=0;
			for(j=0;j<N;j++){
				if(t[i][j]<4 || t[i][j]==7){	//vriskei ti xrwma einai to kouti
					if(t[i][j]==2){
		    				glColor3f(0.0f,1.0f,0.0f); 
					}   
					else if(t[i][j]==1){
		    				glColor3f(1.0f,0.0f,0.0f);
					} 
					else if(t[i][j]==3){
		    				glColor3f(0.0f,0.0f,1.0f); 
					}
					else{
		    				x=x+80/N;
						continue;
					}
					glBegin(GL_QUADS);	//dhmiourgei to xrwmatisto kouti
						glVertex3f( x, y, 30*ep);
						glVertex3f( x, y-8.0,30*ep);
						glVertex3f(x+80/N, y-8.0,30*ep);
						glVertex3f( x+80/N, y, 30*ep);

						glVertex3f( x, y,30*ep+10);
						glVertex3f( x, y-8.0, 30*ep+10);
						glVertex3f(x+80/N, y-8.0f, 30*ep+10);
						glVertex3f(x+80/N, y,30*ep+10);
		
						glVertex3f( x, y,30*ep);
						glVertex3f( x+80/N, y, 30*ep);
						glVertex3f(x+80/N, y, 30*ep+10);
						glVertex3f(x, y,30*ep+10);
		
						glVertex3f( x, y-8.0,30*ep);
						glVertex3f( x+80/N, y-8.0, 30*ep);
						glVertex3f(x+80/N, y-8.0, 30*ep+10);
						glVertex3f(x, y-8.0,30*ep+10);
		
						glVertex3f( x+80/N, y,30*ep);
						glVertex3f( x+80/N, y-8.0, 30*ep);
						glVertex3f(x+80/N, y-8.0, 30*ep+10);
						glVertex3f(x+80/N, y,30*ep+10);
		
						glVertex3f( x, y,30*ep);
						glVertex3f( x, y-8.0, 30*ep);
						glVertex3f(x, y-8.0, 30*ep+10);
						glVertex3f(x, y,30*ep+10);
					glEnd(); 
					x=x+80/N;
				}
			
				else{	//vriskei poio eidos toixou einai to kouti
					glColor3f(1.0f, 1.0f, 1.0f);
					if(t[i][j]==4){
	    					glEnable(GL_TEXTURE_2D);
    						glBindTexture(GL_TEXTURE_2D, tex_2d);

						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					}   
					else if(t[i][j]==5){
		    				glEnable(GL_TEXTURE_2D);
						glBindTexture(GL_TEXTURE_2D, tex_2d2);

						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
						glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					} 
					else{
	    					glEnable(GL_TEXTURE_2D);
    						glBindTexture(GL_TEXTURE_2D, tex_2d3);

					   	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
					}
					glBegin(GL_QUADS); //dhmiourgei to kouti toixo
						glTexCoord2f(1.0f, 0.0f); glVertex3f( x, y, 30*ep);
						glTexCoord2f(0.0f, 0.0f); glVertex3f( x, y-8.0,30*ep); 
						glTexCoord2f(0.0f, 1.0f); glVertex3f(x+80/N, y-8.0,30*ep);
						glTexCoord2f(1.0f, 1.0f); glVertex3f( x+80/N, y, 30*ep);
		
		
						glTexCoord2f(1.0f, 0.0f); glVertex3f( x, y,30*ep+10);
						glTexCoord2f(0.0f, 0.0f); glVertex3f( x, y-8.0, 30*ep+10);
						glTexCoord2f(0.0f, 1.0f); glVertex3f(x+80/N, y-8.0f, 30*ep+10);
						glTexCoord2f(1.0f, 1.0f); glVertex3f(x+80/N, y,30*ep+10);
		
						glTexCoord2f(1.0f, 0.0f); glVertex3f( x, y,30*ep);
						glTexCoord2f(0.0f, 0.0f); glVertex3f( x+80/N, y, 30*ep);
						glTexCoord2f(0.0f, 1.0f); glVertex3f(x+80/N, y, 30*ep+10);
						glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y,30*ep+10);
		
						glTexCoord2f(1.0f, 0.0f); glVertex3f( x, y-8.0,30*ep);
						glTexCoord2f(0.0f, 0.0f); glVertex3f( x+80/N, y-8.0, 30*ep);
						glTexCoord2f(0.0f, 1.0f); glVertex3f(x+80/N, y-8.0, 30*ep+10);
						glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y-8.0,30*ep+10);
		
						glTexCoord2f(1.0f, 0.0f); glVertex3f( x+80/N, y,30*ep);
						glTexCoord2f(0.0f, 0.0f); glVertex3f( x+80/N, y-8.0, 30*ep);
						glTexCoord2f(0.0f, 1.0f); glVertex3f(x+80/N, y-8.0, 30*ep+10);
						glTexCoord2f(1.0f, 1.0f); glVertex3f(x+80/N, y,30*ep+10);
		
						glTexCoord2f(1.0f, 0.0f); glVertex3f( x, y,30*ep);
						glTexCoord2f(0.0f, 0.0f); glVertex3f( x, y-8.0, 30*ep);
						glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y-8.0, 30*ep+10);
						glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y,30*ep+10);
					glEnd();
					glDisable(GL_TEXTURE_2D); 
					x=x+80/N;
				}

			}  	 
			y=y-8.0;
			cond=i+1;
			if(cond % N==0 ){//molis ftasei sthn N grammh na paei ksana sthn 1h grammh tou panw epipedou kai to i na sunexisei ston pinaka pou exoume kataxwrhmenes tis times
				i++;
				break;
			}	
		}
	}
	glFlush();

}

void display(){
	//std::string text;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glPushMatrix();
	grid();
	user();
	moves();
	if(endOfGame){
		if(el+1==L){
			score=(N*N)-(NumberOfMoves*10)-(NumberOfHammers*50);
			cout<<"score: "<<score<<endl;
		}
		else{
			score=0;
		}
		
		//text = "SCORE: 0 ";
		//drawText(text.data(),text.size(), 300, 20);
		//glLoadIdentity();
	}
	glutSwapBuffers(); //swap the buffers
	glPopMatrix();
	glFlush();
}

void reshape(int w, int h) {
	if(h == 0)
		h = 1;
	float ratio = 1.0* w / h;

	
	glMatrixMode(GL_PROJECTION);	// Use the Projection Matrix

        
	glLoadIdentity();	// Reset Matrix

	
	glViewport(0, 0, w, h);	// Set the viewport to be the entire window

	
	gluPerspective(45,ratio,1,1000);	// Set the correct perspective.

	
	glMatrixMode(GL_MODELVIEW);	// Get Back to the Modelview
}

void keyboardFunc(unsigned char key,int x,int y){
	if(endOfGame==false){	//otan den exei teleiwsei to paixnidi
		switch (key){
			case 120:	//Otan patame to koumpi "x"
				endOfGame=true;
				break;
			case 88:	//Otan patame to koumpi "X"
				endOfGame=true;
				break;
			case 82:	//Otan patame to koumpi "R"
				rotate++;
				break;
			case 114:	//Otan patame to koumpi "r"
				rotate++;
				break;
			case 101:	//Otan patame to koumpi "e"
				if(el+1==L){
					endOfGame=true;
				
				}
				break;
			case 69:	//Otan patame to koumpi "E"
				if(el+1==L){
					endOfGame=true;
				}
				break;
			case 72:	//Otan patame to koumpi "H"
				findPos();
			
				if(lx<=13.0 && lx>-13.0 && lz<-23.0 && lz>=-30.0 && t[posi+1][posj]<4 && K>0 && posi%(N-1)!=0|| lx==0.0 && lz==-1.0 && t[posi+1][posj]<4 && K>0 && posi%(N-1)!=0){		
				t[posi+1][posj]=7;
				NumberOfHammers++;
				K--;
				}
				if(posi-1>=0){
					if(lx<=5.0 && lx>-4.0 && lz<=30.0 && lz>29.5  && t[posi-1][posj]<4 && K>0 ||lx==0.0 && lz==-1.0 && t[posi-1][posj]<4 && K>0 ){
					t[posi-1][posj]=7;
					NumberOfHammers++;
					K--;
					}
				}
				if(posj-1>=0){
					if(lx<=-5.0 && lx>-29.0 && lz<=29.0 && lz>-6.5 && lx!=0.0 && lz!=-1.0 && t[posi][posj-1]<4 && K>0){
					t[posi][posj-1]=7;
					NumberOfHammers++;
					K--;
					}
				}
			
				if(lx<=27.0 && lx>-12.0 && lz<=27.0 && lz>-12.0 && lx!=0.0 && lz!=-1.0 && t[posi][posj+1]<4 && K>0 && posj%(N-1)!=0){
				t[posi][posj+1]=7;
				NumberOfHammers++;
				K--;
				}
				break;
			case 104:	//Otan patame to koumpi "h"
				findPos();
			
				if(lx<=13.0 && lx>-13.0 && lz<-23.0 && lz>=-30.0 && t[posi+1][posj]<4 && K>0 && posi%(N-1)!=0|| lx==0.0 && lz==-1.0 && t[posi+1][posj]<4 && K>0 && posi%(N-1)!=0){		
				t[posi+1][posj]=7;
				NumberOfHammers++;
				K--;
				}
				if(posi-1>=0){
					if(lx<=5.0 && lx>-4.0 && lz<=30.0 && lz>29.5  && t[posi-1][posj]<4 && K>0 ||lx==0.0 && lz==-1.0 && t[posi-1][posj]<4 && K>0 ){
					t[posi-1][posj]=7;
					NumberOfHammers++;
					K--;
					}
				}
				if(posj-1>=0){
					if(lx<=-5.0 && lx>-29.0 && lz<=29.0 && lz>-6.5 && lx!=0.0 && lz!=-1.0 && t[posi][posj-1]<4 && K>0){
					t[posi][posj-1]=7;
					NumberOfHammers++;
					K--;
					}
				}
			
				if(lx<=27.0 && lx>-12.0 && lz<=27.0 && lz>-12.0 && lx!=0.0 && lz!=-1.0 && t[posi][posj+1]<4 && K>0 && posj%(N-1)!=0){
				t[posi][posj+1]=7;
				NumberOfHammers++;
				K--;
				}
				break;
			
			case 87:	//Otan patame to koumpi "W"
				findPos();
				findNotPossibleMoves();
				if(moveDown && lx<=13.0 && lx>-13.0 && lz<-23.0 && lz>=-30.0 || moveDown && lx==0.0 && lz==-1.0){
				moveF-=0.1;
				}
				else if(moveUp && lx<=5.0 && lx>-4.0 && lz<=30.0 && lz>29.5 || moveUp && lx==0.0 && lz==-1.0 ){
				moveF+=0.1;
				}
				else if(moveLeft && lx<=-5.0 && lx>-29.0 && lz<=29.0 && lz>-6.5 && lx!=0.0 && lz!=-1.0){
				moveFy-=0.1;
				}
				else if(moveRight && lx<=27.0 && lx>-12.0 && lz<=27.0 && lz>-12.0 && lx!=0.0 && lz!=-1.0){
				moveFy+=0.1;
				}
				break;
			case 119:	//Otan patame to koumpi "w"
			
				findPos();
				findNotPossibleMoves();
				if(moveDown && lx<=13.0 && lx>-13.0 && lz<-23.0 && lz>=-30.0 || moveDown && lx==0.0 && lz==-1.0){
				moveF-=0.1;
				}
				else if(moveUp && lx<=5.0 && lx>-4.0 && lz<=30.0 && lz>29.5 || moveUp && lx==0.0 && lz==-1.0){
			
				moveF+=0.1;
				}
				else if(moveLeft && lx<=-5.0 && lx>-29.0 && lz<=29.0 && lz>-6.5 && lx!=0.0 && lz!=-1.0){
				moveFy-=0.1;
				}
				else if(moveRight && lx<=27.0 && lx>-12.0 && lz<=27.0 && lz>-12.0 && lx!=0.0 && lz!=-1.0){
				moveFy+=0.1;
				}
				break;
			case 83:	//Otan patame to koumpi "S"
				findPos();
				findNotPossibleMoves();
				if(moveUp && lx<=13.0 && lx>-13.0 && lz<-23.0 && lz>=-30.0 || moveUp && lx==0.0 && lz==-1.0){
				moveF+=0.1;
				}
				else if(moveDown && lx<=5.0 && lx>-4.0 && lz<=30.0 && lz>29.5 || moveDown && lx==0.0 && lz==-1.0){
				moveF-=0.1;
				}
				else if(moveRight && lx<=-5.0 && lx>-29.0 && lz<=29.0 && lz>-6.5 && lx!=0.0 && lz!=-1.0){
				moveFy+=0.1;
				}
				else if(moveLeft && lx<=27.0 && lx>-12.0 && lz<=27.0 && lz>-12.0 && lx!=0.0 && lz!=-1.0){
				moveFy-=0.1;
				}
				break;
			case 115:	//Otan patame to koumpi "s"
				findPos();
				findNotPossibleMoves();
				if(moveUp && lx<=13.0 && lx>-13.0 && lz<-23.0 && lz>=-30.0 || moveUp && lx==0.0 && lz==-1.0){
				moveF+=0.1;
				}
				else if(moveDown && lx<=5.0 && lx>-4.0 && lz<=30.0 && lz>29.5 || moveDown && lx==0.0 && lz==-1.0){
				moveF-=0.1;
				}
				else if(moveRight && lx<=-5.0 && lx>-29.0 && lz<=29.0 && lz>-6.5 && lx!=0.0 && lz!=-1.0){
				moveFy+=0.1;
				}
				else if(moveLeft && lx<=27.0 && lx>-12.0 && lz<=27.0 && lz>-12.0 && lx!=0.0 && lz!=-1.0){
				moveFy-=0.1;
				}
				break;
			case 32:	//Otan patame to koumpi "space"
				if(el+1==L){
					endOfGame=true;
				}
				else{
					elevator_up=true;
				}
				break;
		
			case 27:	//Otan patame to koumpi "esc"
				exit(0);
			case 86:	//Otan patame to koumpi "V"
				player++;
				break;
			case 118:	//Otan patame to koumpi "v"
				player++;
				break;
		}
	}
	else{	//otan exei teleiwsei to paixnidi
		switch (key){
			case 27:	//Otan patame to koumpi "esc"
				exit(0);
			case 86:	//Otan patame to koumpi "V"
				player++;
				break;
			case 118:	//Otan patame to koumpi "v"
				player++;
				break;
		}
	}	
}
void openfile(void){	//diavazei to arxeio file.maz
	string line,d=" ",letter;
	ifstream inFileS("file.maz");
	int pos=0,pos2=0,c=0,flag;
	int i=0,j=0;
	char *cstr,*cstr2,*p;
	while(getline(inFileS,line)){
		if(times==0){	//diavazei to L apo to arxeio file.maz
			pos = line.find(d);
			letter= line.substr(0,pos);
			cstr=new char [letter.length()+1];
			strcpy(cstr,letter.c_str());
			p=strtok(cstr,"=");
			p=strtok(cstr," ");
			p+=2;
			L=atoi(p);
			delete[] cstr;
		}
		else if(times==1){	//diavazei to N apo to arxeio file.maz
			pos = line.find(d);
			letter= line.substr(0,pos);
			cstr=new char [letter.length()+1];
			strcpy(cstr,letter.c_str());
			p=strtok(cstr,"=");
			p=strtok(cstr," ");
			p+=2;
			N=atoi(p);
			delete[] cstr;
			lines=N*L;
			//dhmiourgei ton pinaka t[lines][N]
			t=(int **)malloc(sizeof(int *)*lines);
			t[0]=(int *)malloc(sizeof(int)*N*lines);
			for(int k=0;k<lines;k++){
				t[k]=(*t + N * k);
			}
			
		}
		if(times==2){	//diavazei to K apo to arxeio file.maz
			pos = line.find(d);
			letter= line.substr(0,pos);
			cstr=new char [letter.length()+1];
			strcpy(cstr,letter.c_str());
			p=strtok(cstr,"=");
			p=strtok(cstr," ");
			p+=2;
			K=atoi(p);
			delete[] cstr;
		}
		else if(times>3){
			flag=0;	
			//vazei ston pinaka t[][] times analoga me to ti diavazei apo to arxeio file.maz
			while((pos = line.find(d))!= std::string::npos){
				letter= line.substr(0,pos);
				if(letter.compare("G")==0){
					t[i][j] = 2;
					j++;
					flag=1;
				}
				else if(letter.compare("R")==0){
					t[i][j] = 1;				
					j++;
					flag=1;
				}
				else if(letter.compare("B")==0){
					t[i][j] = 3;
					j++;
					flag=1;
				}
				else if(letter.compare("T1")==0){
					t[i][j] = 4;
					j++;
					flag=1;
				}
				else if(letter.compare("T2")==0){
					t[i][j] = 5;
					j++;
					flag=1;
				}
				else if(letter.compare("T3")==0){
					t[i][j] = 6;
					j++;
					flag=1;
				}
				else if(letter.compare("E")==0){
					t[i][j] = 7;
					j++;
					flag=1;
				}	
				line.erase(0,pos+d.length());
				

				if(j==N-1){
					line.erase(line.end()-1,line.end());
					if(line.compare("G")==0){
						t[i][j] = 2;
						j++;
						flag=1;
					}
					else if(line.compare("R")==0){
						t[i][j] = 1;				
						j++;
						flag=1;		
					}
					else if(line.compare("B")==0){
						t[i][j] = 3;
						j++;
						flag=1;
					}
					else if(line.compare("T1")==0){
						t[i][j] = 4;
						j++;
						flag=1;
					}
					else if(line.compare("T2")==0){
						t[i][j] = 5;
						j++;
						flag=1;
					}
					else if(line.compare("T3")==0){
						t[i][j] = 6;
						j++;
						flag=1;
					}
					else if(line.compare("E")==0){
						t[i][j] = 7;
						j++;
						flag=1;
					}
				}
				
			}
			if(flag==1){
				i++;
				j=0;
			}		
		}
		times++;	
	}
}

void loadTexture(){	//fortwnei tis eikones 
	tex_2d = SOIL_load_OGL_texture
    (
    	"T1.png",
    	SOIL_LOAD_AUTO,
    	SOIL_CREATE_NEW_ID,
    	SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );
	tex_2d2 = SOIL_load_OGL_texture
    (
    	"T2.png",
    	SOIL_LOAD_AUTO,
    	SOIL_CREATE_NEW_ID,
    	SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );
	tex_2d3 = SOIL_load_OGL_texture
    (
    	"T3.png",
    	SOIL_LOAD_AUTO,
    	SOIL_CREATE_NEW_ID,
    	SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );
}

void mouseMove(int x, int y) { 	
         if (xOrigin >= 0) {	// otan einai pathmeno to aristero koumpi

		
		deltaAngle = (x - xOrigin) * 0.001f;

		lx = 30*sin(angle + deltaAngle);	// allazei thn kateuthunsh ths cameras
		lz = -30*cos(angle + deltaAngle);
	}
	 if (yOrigin >=0){	// otan einai pathmeno to deksi koumpi
		deltaAngley = (y - yOrigin) * 0.001f;		
		ly = -150*sin(angley + deltaAngley);
	}
}

void mouseButton(int button, int state, int x, int y) {
	
	if (button == GLUT_LEFT_BUTTON) {	// ksekinaei na gurnaei h camera otan patame to deksi 'h to aristero koumpi tou pontikiou

		
		if (state == GLUT_UP) {	// otan einai pathmeno to aristero koumpi
			angle += deltaAngle;
			
			xOrigin = -1;
		}
		else  {// state = GLUT_DOWN
		
			xOrigin = x;
		}
	
	}
	if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_UP) {	// otan einai pathmeno to deksi koumpi
			angley += deltaAngley;
			
			yOrigin = -1;
		}
		else{
			yOrigin = y;
		}
	}
}

int main (int argc, char** argv){
	srand(time(NULL));
	glutInit (&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); 
	glutInitWindowPosition(150, 150);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Maze");
	openfile();
	loadTexture();
	initialize();
	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(display);


	glutKeyboardFunc (keyboardFunc);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);
	glutMainLoop();
	return 0;
}
