#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    //Config file
    if( XML.load("config.xml") ){
        cout<< "config.xml loaded!"<< endl;
        address = XML.getValue<string>("address");
        port = XML.getValue<int>("port");
        isFullScreen = XML.getValue<int>("fullScreen");
        w = XML.getValue<int>("width");
        h = XML.getValue<int>("height");
        useAPI = XML.getValue<int>("useAPI");
    }

    ofSetWindowShape(w, h);

	//WEBSOCKETS CONFIG
	ofSetLogLevel(OF_LOG_VERBOSE);
	client.connect(address, port);
	ofSetLogLevel(OF_LOG_ERROR);
	client.addListener(this);

    lastReconnectTime = ofGetElapsedTimeMillis();

	drawImage = false;
	loadImage = false;
	
	loader.load("loader.png");
	loading = false;
	
	ofHideCursor();
}

//--------------------------------------------------------------
string ofApp::getImageURL(const char* cmd){
    //cout<< "Searching for image " + ofToString(cmd)<< endl;
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    // cout<< "RESULT: " + result<< endl;
	return result;
}

//--------------------------------------------------------------
void ofApp::transformImage(){
    
    float maxWidth = ofGetHeight();
    float maxHeight = ofGetWidth();
    float ratio = 0;
    float width = image.getWidth();
    float height = image.getHeight();
    
    cout<< "MAX H: "<< ofToString(maxHeight)<< endl;
	cout<< "MAX W: "<< ofToString(maxWidth)<< endl;
	cout<< "IMG Height: "<< ofToString(height)<< endl;
	cout<< "IMG Width: "<< ofToString(width)<< endl;
		
    if(width>height){
		cout<< "Horizontal image!"<< endl;
		width = width / height * maxHeight ;
		height = maxHeight;
	}
	
	else if(height>width){
		cout<< "Vertical image!"<< endl;
		width = maxWidth ;
		height = height / width * maxWidth;
	}
	
	else if(height==width){
		cout<< "Square image!"<< endl;
		width = width / height * maxHeight ;
		height = maxHeight;
	}
	
	cout<< "rescaled Height: "<< ofToString(height)<< endl;
	cout<< "rescaled Width: "<< ofToString(width)<< endl;
		
    //image.resize(ofGetHeight(), ofGetWidth());
    image.resize(width, height);
    image.rotate90(3);
}

//--------------------------------------------------------------
void ofApp::update(){
    if(client.isConnected()){
    	if(loadImage){
    		image.load("data/image.jpg");
            	transformImage();
    		loadImage = false;
            	client.send("$image;ready");
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
    	// ofSetColor(255);
    	if(drawImage){
            if(image.isAllocated()){
				image.draw(-(image.getWidth()-ofGetWidth())/2,-(image.getHeight()-ofGetHeight())/2);
				//image.draw(0,0);
            }
    	}else{ 
			if(loading){
				//cout<< loading<< endl;
				ofPushMatrix();
				ofTranslate(ofGetWidth()/2-16, ofGetHeight()/2-16);
					ofPushMatrix();
					ofTranslate(32,32);
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
    client.send("$image;");

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

	string allTogether = "";
	for(int i=0; i<(ofSplitString(args.message, " ")).size();i++){
		allTogether = allTogether + ofSplitString(args.message, " ")[i];
        if(i<(ofSplitString(args.message, " ")).size()-1){
            allTogether += "_";
        }
	}

	cout<< allTogether<< endl;

    if(args.message == "$reset;"){
    	drawImage = false;
    }else if(args.message == "$load;"){
        loading = true;
    }else if(args.message == "$display;"){
        drawImage = true;
        loading = false;
    }else if(args.message == "$shutdown;"){
        system("sudo shutdown -h now");
    }else{
		string url = "";
    	if(useAPI==1){
			url = getImageURL(("python data/app.py " + allTogether).c_str());
		}else if(useAPI == 0){
			url = getImageURL(("node data/app.js " + allTogether).c_str());
		}
    	cout<< "URL: " + url<< endl;
    	loadImage = true;
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
