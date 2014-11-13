#pragma once
#include "ofMain.h"

//programma che carica un'immagine in bin/data, la passa ad una texture, inserendo anche l'alpha
//channel non presente nell'immagine caricata, e poi ogni volta che viene premuto un tasto esegue
//una running average sull'immagine 

using namespace std;


class ofApp: public ofBaseApp{

ofTexture text;
ofImage img;
ofEasyCam cam1;
unsigned char * pixs_pertexture; //in questo ci mettiamo il contenuto dell'immagine letta, che poi carichiamo nella texture
//siccome non si possono ritirare fuori i pixel da una texture, per modificare la texture, dobbiamo lavorare sempre su questo
//mantenendolo aggiornato, e poi rifare il load in texture ogni volta
bool text_toupdate;

public:
	ofApp(){}
	void setup();
	void draw();
	void update();
	void keyPressed(int key);
};