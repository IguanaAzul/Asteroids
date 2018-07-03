#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define numeroAsteroides 150
#define radianoParaGraus(radianos) (radianos * (180.0 / M_PI))
#define grausParaRadianos(graus) ((graus * M_PI) / 180.0)
#define PI 3.14159265

int alturaJanela, larguraJanela, tempoAtual = 0;
int placar = 0;
char textoPlacar[100];

#define numeroTirosMax 10000
#define numeroTiros 10
#define tempoVidaTiro 1000
int intervaloDisparo = 150;

int numeroTirosv=numeroTiros;
int tempoVidaTirov = 1000;
int ultimoDisparo = 0;

//Todas as variáveis abaixo se referem aos objetos colocados na tela, posição, orientação, velocidade angular, velocidade linear...

float xNave, yNave, vNMax=2, orientacaoGrausNave = 0, vAngNave, vNave = 0, orientacaoGrausMovimentoNave = 0; // Variáveis referentes à nave
float xTiro[numeroTirosMax], yTiro[numeroTirosMax], orientacaoRadTiro[numeroTirosMax], vTiro[numeroTirosMax]; int aTiro[numeroTirosMax]; // Variáveis referentes aos tiros
int nTiro=0;
float xAsteroide[numeroAsteroides], yAsteroide[numeroAsteroides], orientacaoRadAsteroide[numeroAsteroides], orientacaoGrausAsteroide[numeroAsteroides], vAsteroide[numeroAsteroides], rotAsteroide[numeroAsteroides], vAngAsteroide[numeroAsteroides], rAsteroide[numeroAsteroides];
int nLadosAsteroide[numeroAsteroides], aAsteroide[numeroAsteroides]; // Variáveis referentes aos asteróides
float orientacaoTemp,orientacaoRadTemp, vTemp, vAngTemp;
int imortal = 0;

int k[256]; // Vetor de teclas, 1 para tecla pressionada e 0 para tecla solta
int pausaJogo = 0;

void inicializa(void){
    int i; xNave=0; yNave=0; vNave=0;
    for (i=0; i<256; i++){
        k[i]=0;
    }
    placar = 0;
    for (i=0; i<numeroTirosMax; i++){
        xTiro[i]=7500;
        yTiro[i]=7500;
        vTiro[i]=0;
        aTiro[i]=-1;
    }
    for (i=0; i<numeroAsteroides; i++){
        nLadosAsteroide[i] = rand()%6+5;
        aAsteroide[i]=-1;
        xAsteroide[i]=7500;
        yAsteroide[i]=7500;
        orientacaoGrausAsteroide[i] = rand()%360;
        orientacaoRadAsteroide[i] = grausParaRadianos(orientacaoGrausAsteroide[i]);
        vAsteroide[i] = 0;
        vAngAsteroide[i] = 0;
    }
    for(i=0; i<numeroAsteroides/3; i++){
        aAsteroide[i]=0;
        int j;
        nLadosAsteroide[i] = rand()%6+5;
        j = rand()%2;
        if (j==0){xAsteroide[i] = -(larguraJanela/2) + rand()%(larguraJanela/4);} else {xAsteroide[i] = (larguraJanela/2) - rand()%(larguraJanela/4);};
        j = rand()%2;
        if (j==0){yAsteroide[i] = -(alturaJanela/2)+ rand()%(alturaJanela/4);} else {yAsteroide[i] = (alturaJanela/2) - rand()%(alturaJanela/4);};
        rAsteroide[i] = rand()%7+13;
        orientacaoGrausAsteroide[i] = rand()%360;
        orientacaoRadAsteroide[i] = grausParaRadianos(orientacaoGrausAsteroide[i]);
        vAsteroide[i] = (float)(rand()%20+1)/10;
        vAngAsteroide[i] = (float)(rand()%30+1)/30;
    }
    glClearColor(0, 0, 0, 0);
}
void desenhaNave(void){
    glPushMatrix();
        glTranslatef(xNave, yNave, 0);
        glRotatef(orientacaoGrausNave, 0, 0, 1);
        glColor3f(0.2, 1, .6);
        glBegin(GL_TRIANGLE_FAN);
            glVertex2f( 10,  0);
            glVertex2f(-10,  10);
            glVertex2f(-5,  0);
            glVertex2f(-10, -10);
        glEnd();
    glPopMatrix();
}

void desenhaAsteroide(int n){
    glPushMatrix();
            int i;
            glColor3f(.6+(float)imortal/3+imortal*(float)(rand()%100)/100, .6-(float)imortal/3+imortal*(float)(rand()%100)/100, .6-(float)imortal/3+imortal*(float)(rand()%100)/100);
            glTranslatef(xAsteroide[n], yAsteroide[n], 0);//Isso define o centro do polígono
            glRotatef(rotAsteroide[n], 0, 0, 1);
            glBegin(GL_TRIANGLE_FAN);
            for (i=nLadosAsteroide[n]; i>0; i--){
                glVertex3f(rAsteroide[n]*cos(i*2*PI/nLadosAsteroide[n]),rAsteroide[n]*sin(i*2*PI/nLadosAsteroide[n]), 0); //E isso os vértices
            }
        glEnd();
    glPopMatrix();
}


void desenhaTiro (int n){
    glPushMatrix();
        glTranslatef(xTiro[n], yTiro[n], 0);
        glColor3f(1, .3, .1);
        glBegin(GL_TRIANGLE_STRIP);
            glVertex2f( 2,  0);
            glVertex2f(0,  2);
            glVertex2f(0, -2);
            glVertex2f(-2,  0);
        glEnd();
    glPopMatrix();
}


void desenhaTexto( float x, float y, int r, int g, int b, const char *string ) {
	int j = strlen( string );
	glColor3f( r, g, b );
	glRasterPos2f( x, y );
	int i;
	for( i = 0; i < j; i++ ) {
		glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, string[i] );
	}
}
void desenhaHUD(){
    sprintf(textoPlacar, "%d", placar);
    desenhaTexto(-larguraJanela/2+40,alturaJanela/2-40,1,1,1,textoPlacar);
}


void desenhaCena(void){
    glClear(GL_COLOR_BUFFER_BIT);
    desenhaNave();
    int n;
    for(n=0; n<numeroAsteroides; n++){
        desenhaAsteroide(n);
    }
    for(n=0; n<numeroTirosv; n++){
        desenhaTiro(n);
    }
    desenhaHUD();
    glutSwapBuffers();
}


int colidiu(float x1, float y1, float x2, float y2, float distancia2) {
    return ((pow(x1-x2,2)+pow(y1-y2,2)) < pow((distancia2),2));
}


void detectaColisoes(void){
    int i, j;
    for (i=0; i<numeroAsteroides; i++){
        if(colidiu(xAsteroide[i], yAsteroide[i] , xNave, yNave, rAsteroide[i]+9) & !imortal){
            inicializa();
        }
    }
    for (i=0; i<numeroAsteroides; i++){
        for (j=i; j<numeroAsteroides; j++){
            if (i==j) continue; //
            if (colidiu(xAsteroide[i], yAsteroide[i] , xAsteroide[j], yAsteroide[j], rAsteroide[i]+rAsteroide[j])){
                orientacaoTemp = orientacaoGrausAsteroide[i];
                orientacaoRadTemp = orientacaoRadAsteroide[i];
                vAngTemp = vAngAsteroide[i];

		float mi = pow(rAsteroide[i], 3), mj = pow(rAsteroide[j], 3);

                vTemp = (vAsteroide[i]*mi+vAsteroide[j]*mj+mi*(vAsteroide[i]-vAsteroide[j]))/(mi+mj);

                vAsteroide[i]=(vAsteroide[i]*mi+vAsteroide[j]*mj+mj*(vAsteroide[j]-vAsteroide[j]))/(mi+mj);
                orientacaoGrausAsteroide[i]=orientacaoGrausAsteroide[j];
                orientacaoRadAsteroide[i]=orientacaoRadAsteroide[j];
                vAngAsteroide[i]=vAngAsteroide[j];

                vAsteroide[j]=vTemp;
                orientacaoGrausAsteroide[j]=orientacaoTemp;
                orientacaoRadAsteroide[j]=orientacaoRadTemp;
                vAngAsteroide[j]=vAngTemp;

		xAsteroide[i]+=cos(orientacaoRadAsteroide[j])*vAsteroide[j];
		yAsteroide[i]+=sin(orientacaoRadAsteroide[j])*vAsteroide[j];
		xAsteroide[j]+=cos(orientacaoRadAsteroide[i])*vAsteroide[j];
		yAsteroide[j]+=sin(orientacaoRadAsteroide[i])*vAsteroide[j];

            }
        }
    }
    int proximoBB(){
        int i;
        for(i=numeroAsteroides; i>0; i--){
            if (aAsteroide[i] == -1)
                return i;
        }
        return -1;
    }
    for (i=0; i<numeroAsteroides; i++){
        for (j=0; j<numeroTirosv; j++){
            if (aTiro[j] != -1 && colidiu(xAsteroide[i], yAsteroide[i], xTiro[j], yTiro[j], rAsteroide[i])){
                    placar += 2;
				    xTiro[j]=7500;
				    yTiro[j]=7500;
				    vTiro[j]=0;
				    aTiro[j]=-1;
                    if (aAsteroide[i]==0){
                        int m;
                        for(m=0; m<2; m++){
                            int b = proximoBB();
                            if (b != -1){
                                vAsteroide[b] = (float)(rand()%20+1)/5;
                                vAngAsteroide[b] = (float)(rand()%30+1)/5;
                                rAsteroide[b] = rAsteroide[i]/2;
                                orientacaoGrausAsteroide[b] = rand()%360;
                                orientacaoRadAsteroide[b] = grausParaRadianos(orientacaoGrausAsteroide[i]);
                                aAsteroide[b]=2;
                                xAsteroide[b] = xAsteroide[i]+pow(-1,m)*cos(orientacaoRadAsteroide[b])*vAsteroide[b]*rAsteroide[b];
                                yAsteroide[b] = yAsteroide[i]+pow(-1,m)*sin(orientacaoRadAsteroide[b])*vAsteroide[b]*rAsteroide[b];
                            }
                        }
                    }
                    xAsteroide[i] = 7500; 
                    yAsteroide[i] = 7500;
                    vAsteroide[i] = 0;
                    aAsteroide[i]=-1;
            }
        }
    }
}

void atualizaPosicaoAsteroide(int i){
    xAsteroide[i] += cos(orientacaoRadAsteroide[i])*vAsteroide[i];
    yAsteroide[i] += sin(orientacaoRadAsteroide[i])*vAsteroide[i];
    if (xAsteroide[i] >= larguraJanela/2 && xAsteroide[i]!=7500){
            xAsteroide[i] = xAsteroide[i] - larguraJanela;
    };
    if (yAsteroide[i] >= alturaJanela/2 && yAsteroide[i]!=7500){
            yAsteroide[i] = yAsteroide[i] - alturaJanela;
    };
    if (xAsteroide[i] <= -larguraJanela/2 && xAsteroide[i]!=7500){
            xAsteroide[i] = xAsteroide[i] + larguraJanela;
    };
    if (yAsteroide[i] <= -alturaJanela/2 && yAsteroide[i]!=7500){
            yAsteroide[i] = yAsteroide[i] + alturaJanela;
    };
    rotAsteroide[i] += vAngAsteroide[i];
}


void atualizaPosicaoTiro(int i){
        xTiro[i] += cos(orientacaoRadTiro[i])*vTiro[i];
        yTiro[i] += sin(orientacaoRadTiro[i])*vTiro[i];
        if (xTiro[i] >= larguraJanela/2 && xTiro[i]!=7500){
                xTiro[i] = xTiro[i] - larguraJanela;
                aTiro[i]+=1;
        };
        if (yTiro[i] >= alturaJanela/2 && yTiro[i]!=7500){
                yTiro[i] = yTiro[i] - alturaJanela;
                aTiro[i]+=1;
        };
        if (xTiro[i] <= -larguraJanela/2 && xTiro[i]!=7500){
                xTiro[i] = xTiro[i] + larguraJanela;
                aTiro[i]+=1;
        };
        if (yTiro[i] <= -alturaJanela/2 && yTiro[i]!=7500){
                yTiro[i] = yTiro[i] + alturaJanela;
                aTiro[i]+=1;
        };
        if(aTiro[i]!=-1 && (tempoAtual-aTiro[i])>tempoVidaTiro*pow(10, imortal)){
            xTiro[i]=7500;
            yTiro[i]=7500;
            vTiro[i]=0;
            aTiro[i]=-1;
        }
}

void atualiza(int idx){   
    if(pausaJogo == 0){
        tempoAtual = glutGet(GLUT_ELAPSED_TIME) ;
        float orientacaoRadNave = grausParaRadianos(orientacaoGrausNave);
        float orientacaoRadMovimentoNave = grausParaRadianos(orientacaoGrausMovimentoNave);
        detectaColisoes();
        if (xNave >= larguraJanela/2){xNave = xNave - larguraJanela;};
        if (yNave >= alturaJanela/2){yNave = yNave - alturaJanela;};
        if (xNave <= -larguraJanela/2){xNave = xNave + larguraJanela;};
        if (yNave <= -alturaJanela/2){yNave = yNave + alturaJanela;};
        xNave += cos(orientacaoRadMovimentoNave)*vNave;
        yNave += sin(orientacaoRadMovimentoNave)*vNave;
        orientacaoGrausNave += vAngNave;

        if (k['w']==1 || k['W']==1){
            if (vNave<vNMax){vNave+=0.1;};
            orientacaoGrausMovimentoNave = orientacaoGrausNave;
        }
        if (k['w']==0 && k['W']==0){
            if (vNave>0.005){vNave-=0.005;};
        }
        if (k['s']==1 || k['S']==1){
            if (vNave>0.03){vNave-=0.03;};
        }
        if (k['a']==1 || k['A']==1){
            vAngNave = 3;
        }
        if (k['d']==1 || k['D']==1){
            vAngNave = -3;
        }
        if (k['a']==0 && k['A']==0 && k['d']==0 && k['D']==0){
            vAngNave = 0;
        }

        int proximoTiro(){
        int i = 0;
        for(i=0; i<numeroTirosv; i++){
            if (aTiro[i] == -1)
                return i;
        }
        return -1;
        }
        if (k[' ']==1 && (tempoAtual - ultimoDisparo) > intervaloDisparo){
            ultimoDisparo = tempoAtual;
            nTiro = proximoTiro();
            if (nTiro != -1){
                vTiro[nTiro]=5*pow(vNave, imortal)+vNave*imortal;
                xTiro[nTiro] = xNave;
                yTiro[nTiro] = yNave;
                aTiro[nTiro] = ultimoDisparo;
                orientacaoRadTiro[nTiro] = orientacaoRadNave;
            }
        }
        int i;
        // Atualizando posição tiros
        for(i=0; i<numeroTirosv; i++){
            atualizaPosicaoTiro(i);
        }
        // Atualizando posição asteroides
        for(i=0; i<numeroAsteroides; i++){
            atualizaPosicaoAsteroide(i);
        }
    }
    glutPostRedisplay();
    glutTimerFunc(17, atualiza, 0);
}

void redimensiona(int w, int h){
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-w/2, w/2, -h/2, h/2, -1, 1);//proporções do mundo e coordenadas do jogo e de todo o código, xmin, xmax, ymin, ymax, zmin(padrão = -1), zmax(padrão = 1);
   alturaJanela = h;
   larguraJanela = w;
   glMatrixMode(GL_MODELVIEW);glBegin(GL_TRIANGLE_STRIP);
   glLoadIdentity();
}

void pressiona(unsigned char key,int x, int y){
    k[key]=1;
    switch(key){
        case 27:
            exit(0);
            break;
	case 'i'|'I':
	    if (imortal==0) {
		imortal=1; 
		placar-=10000000;
	    	numeroTirosv=numeroTirosMax; tempoVidaTirov=10000; intervaloDisparo=15; vNMax=15;
		inicializa();
	    } 
	    else {
		imortal=0;
	    	numeroTirosv=numeroTiros; tempoVidaTirov=1000; intervaloDisparo=150; vNMax=2;
		inicializa();
	    }
	break;
	case 'p'|'P':
	    if (pausaJogo == 0){
	    	pausaJogo=1;
	    }
	    else{
	        pausaJogo = 0;
	    }
	break;
    }
}

void solta(unsigned char key,int x, int y){
    k[key]=0;
}

int main(int argc, char **argv){
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    larguraJanela = 1280;
    alturaJanela = 720;
    glutInitWindowSize(larguraJanela,alturaJanela);
    glutInitWindowPosition(50, 50);

    glutCreateWindow("Asteroids");
    glutDisplayFunc(desenhaCena);
    glutReshapeFunc(redimensiona);
    glutKeyboardFunc(pressiona);
    glutKeyboardUpFunc(solta);
    glutTimerFunc(0, atualiza, 0);
    inicializa();

    glutMainLoop();
    return 0;
}
