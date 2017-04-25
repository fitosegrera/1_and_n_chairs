#pragma once

#include "ofMain.h"
#include "ofxLibwebsockets.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		string getImageURL(const char* cmd);
		void transformImage();

		ofImage image;
		bool drawImage, loadImage;
		int w, h;

		//WEBSOCKETS
		int port;
		string address;
		uint64_t lastReconnectTime;
        ofxLibwebsockets::Client client;
        void onConnect( ofxLibwebsockets::Event& args );
        void onOpen( ofxLibwebsockets::Event& args );
        void onClose( ofxLibwebsockets::Event& args );
        void onIdle( ofxLibwebsockets::Event& args );
        void onMessage( ofxLibwebsockets::Event& args );
        void onBroadcast( ofxLibwebsockets::Event& args );
		
		ofXml XML; //config file
        
        int isFullScreen;
};
