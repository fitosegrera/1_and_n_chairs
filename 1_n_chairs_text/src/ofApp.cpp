#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    //Config file
    if( XML.load("config.xml") ){
        cout<< "config.xml loaded!"<< endl;
        address = XML.getValue<string>("address");
        port = XML.getValue<int>("port");
        fontSize = XML.getValue<int>("fontSize");
        marginX = XML.getValue<int>("marginX");
        isFullScreen = XML.getValue<int>("fullScreen");
        w = XML.getValue<int>("width");
        h = isFullScreen = XML.getValue<int>("height");
    }

    ofSetWindowShape(w, h);

	//WEBSOCKETS CONFIG
	ofSetLogLevel(OF_LOG_VERBOSE);
	client.connect(address, port);
	ofSetLogLevel(OF_LOG_ERROR);
	client.addListener(this);

    lastReconnectTime = ofGetElapsedTimeMillis();
	drawText = false;

	messageArrived = false;

	ofTrueTypeFont::setGlobalDpi(72);
    font.load("verdana.ttf", fontSize, true, true);
	inputMessage = "";
	
	loader.load("loader.png");
	loading = false;
	
	ofHideCursor();
}

//--------------------------------------------------------------
string ofApp::makeTextBox(string s){
    string lines = "";
    string singleLine = "";
    int arrLength = ofSplitString(s, " ").size();

    for(int i=0; i<arrLength; i++){
        ofRectangle r = font.getStringBoundingBox(singleLine, 0, 0);
        if(r.width > ofGetScreenWidth()-marginX){
            string word = ofSplitString(s, " ")[i];
            singleLine += word + " \n";
            lines += singleLine;
            singleLine = "";
        }else{
            if(i == arrLength-1){
                lines = lines + singleLine + ofSplitString(s, " ")[i];
            }else{
                singleLine += ofSplitString(s, " ")[i] + " ";
            }
        }
    }

    return lines;
}

//--------------------------------------------------------------
void ofApp::update(){
	if(client.isConnected()){
		if(messageArrived){
			client.send("$text;ready");
			messageArrived = false;
		}
    }else{
        uint64_t now = ofGetElapsedTimeMillis();
        if (now - lastReconnectTime > 5000) {
            cout<< "Reconnecting..."<< endl;
            client.close();
            client.connect(address, port); 
            lastReconnectTime = ofGetElapsedTimeMillis();
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	if(client.isConnected()){
		if(drawText){
			ofBackground(255);
			ofSetColor(0);
			font.drawStringCentered(makeTextBox(inputMessage), ofGetWidth()/2, ofGetHeight()/2);
		}else{
			ofBackground(0);
			if(loading){	
				ofSetColor(255);
				ofPushMatrix();
					ofTranslate(ofGetWidth()/2 - 16, ofGetHeight()/2 - 16);
					ofPushMatrix();
						ofTranslate(32, 32);
						ofRotate(ofGetFrameNum() * 5);
						loader.draw(-16,-16);
					ofPopMatrix();
				ofPopMatrix();
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::onConnect( ofxLibwebsockets::Event& args ){
    cout<<"on connected"<<endl;
}

//--------------------------------------------------------------
void ofApp::onOpen( ofxLibwebsockets::Event& args ){
    cout<<"on open"<<endl;
    client.send("$text;");

    //Set window shape
    if(isFullScreen == 1){
        ofSetFullscreen(true);
    }
}

//--------------------------------------------------------------
void ofApp::onClose( ofxLibwebsockets::Event& args ){
    cout<<"on close"<<endl;
}

//--------------------------------------------------------------
void ofApp::onIdle( ofxLibwebsockets::Event& args ){
    cout<<"on idle"<<endl;
}

//--------------------------------------------------------------
void ofApp::onMessage( ofxLibwebsockets::Event& args ){
    //cout<<"got message "<<args.message<<endl;
    if(args.message == "$reset;"){
    	drawText = false;
    }else if(args.message == "$load;"){
        loading = true;
    }else if(args.message == "$display;"){
        drawText = true;
        loading = false;
    }else if(args.message == "$shutdown;"){
        system("sudo shutdown -h now");
    }else{
    	inputMessage = args.message;
    	messageArrived = true;
    }
}

//--------------------------------------------------------------
void ofApp::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"got broadcast "<<args.message<<endl;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
