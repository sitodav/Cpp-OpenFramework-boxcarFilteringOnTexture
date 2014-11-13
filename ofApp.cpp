#include "ofApp.h"

void ofApp::setup(){

	text.allocate(800,800,GL_RGBA); //alloco oggetto texture per essere 800x800 di tipo rgba, quindi sarà 800x800x4
	
	img.loadImage("stars.png"); //di default la legge rgb
	unsigned char * pixs_daimmagine= img.getPixels(); //quindi sarà un array monodim di w*h*3 (3 bytes a canale)
	//se la texture fosse stata GL_RGB avrei potuto passare direttamente pixs_daimmagine al metodo loadData della texture
	//ma siccome nella texture c'e' anche l'alpha channel, devo costruire appositamente io un array 
	//alloco questo array
	int w=img.getWidth(), h=img.getHeight();
	pixs_pertexture= new unsigned char [w*h*4]; //4 perchè c'e' l'alpha
	for(int i=0;i<h;i++)
		for(int j=0;j<w;j++){
			ofColor colPixImm=img.getColor(j,i); //prendo colore pixel,nb che vuole prima la x e poi la y
			pixs_pertexture[(i*w+j)*4+0]= colPixImm.r; //i riga * ncolonne + icolonna , tutto per 4, ci posiziona sulla quatrupla relativa a quel pixel, e poi accedo ai 4 canali
			pixs_pertexture[(i*w+j)*4+1]= colPixImm.g;
			pixs_pertexture[(i*w+j)*4+2]= colPixImm.b;
			//pixs_pertexture[(i*w+j)*4+3]= (unsigned char)ofRandom(0,255); //random per l'alpha channel
			pixs_pertexture[(i*w+j)*4+3]=255;
		}

	//creato l'array lo passo alla texture
	text.loadData(pixs_pertexture,800,800,GL_RGBA);
	//abilito alpha blending, altrimenti non usa l'alpha channel
	ofEnableAlphaBlending();



}


void ofApp::update(){
	if(text_toupdate){
		text_toupdate=false;
		text.loadData(pixs_pertexture,800,800,GL_RGBA);
	}
}

void ofApp::keyPressed(int key){
	if(key!=(int)'a')
		return;
	//running average
	//NB: non si possono tirare fuori i pixel dall'oggetto texture !
	//BISOGNA LAVORARE SEMPRE SULL'ARRAY DI APPOGGIO E RIFARE IL LOAD OGNI VOLTA !

	//per semplicità, non considero pixel di bordo
	int w=img.getWidth();
	int h=img.getHeight();
	float mean;
	int ind_mono;
	//la media è fatta con i quattro-connessi
	for(int i=1;i<h-1;i++)
		for(int j=1;j<w-1;j++){
			mean=0.0f;
			//RICORDA SEMPRE CHE DEVI ACCEDERE AD UN VETTORE MONODIMENSIONALE, CON 4 ELEMENTI (CANALI) PER OGNI PIXEL
			//se io ho l'indice dell'elemento (della quadrupla), sul vettore monodimensionale che è
			ind_mono=(i*w+j)*4; //(qui ci stanno i 4 canali del pixel)
			//il pixel quattroconnesso della riga superiore è dato da ind_mono - 4*w
			//quello riga sottostante sarà ind_mono+4*w
			//il quattro connesso sarà ind_mono -4, e +4 per quello a destra, 
			//e poi si considera l'offset del singolo char per uno dei 4 canali
			 
			//QUINDI:
			//nei 3 canali separati

			//red:
			mean+=pixs_pertexture[ind_mono-4*w + 0] ; //vicino superiore
			mean+=pixs_pertexture[ind_mono+4*w + 0] ; //vicino sottostante
			mean+=pixs_pertexture[ind_mono+4 + 0]; //vicino dx
			mean+=pixs_pertexture[ind_mono-4 + 0]; //vicino sx
			mean+=pixs_pertexture[ind_mono + 0]; //proprio il pixel
			//assegno la media
			pixs_pertexture[ind_mono+ 0]=mean/5;

			//e lo stesso per green e blu e l'alpha(offset di 1 per il green, 2 per il blu)
			//l'alpha in realtà non lo tocco perchè è uguale per tutti i pixel
			mean=0.0f;
			mean+=pixs_pertexture[ind_mono-4*w + 1] ; //vicino superiore
			mean+=pixs_pertexture[ind_mono+4*w + 1] ; //vicino sottostante
			mean+=pixs_pertexture[ind_mono+4 + 1]; //vicino dx
			mean+=pixs_pertexture[ind_mono-4 + 1]; //vicino sx
			mean+=pixs_pertexture[ind_mono +1];
			pixs_pertexture[ind_mono+ 1]=mean/5;

			mean=0.0f;
			mean+=pixs_pertexture[ind_mono-4*w + 2] ; //vicino superiore
			mean+=pixs_pertexture[ind_mono+4*w + 2] ; //vicino sottostante
			mean+=pixs_pertexture[ind_mono+4 + 2]; //vicino dx
			mean+=pixs_pertexture[ind_mono-4 + 2]; //vicino sx
			mean+=pixs_pertexture[ind_mono +2];
			pixs_pertexture[ind_mono + 2]=mean/5;
	
		}
		//il load del nuovo array, nell'oggetto texture, lo faccio nel metodo update()
		text_toupdate=true;
}

void ofApp::draw(){
	cam1.begin(); //quando attivo easyCam, il sistema di riferimento cambia! 0,0,0 sta al centro della finestra, y positivo verso alto, x positivo verso destra, z positivo fuori dallo schermo
	//se invec enon lo attivo, il sistema di riferimento sta già in basso a sinistra, ma...(???)
		ofPushMatrix();
			ofTranslate(-ofGetWidth()*0.5,-ofGetHeight()*0.5,0); //quindi traslo sistema in modo tale che l'origine stia in basso a sinistra
			text.draw(0,0,800,800);
		ofPopMatrix();
	cam1.end();
}




