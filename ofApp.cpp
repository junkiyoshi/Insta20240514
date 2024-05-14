#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openFrameworks");

	ofBackground(39);
	ofSetLineWidth(2);
	ofEnableDepthTest();

	auto radius = 12;
	auto x_span = radius * sqrt(3);
	auto flg = true;
	for (float y = -360 * 1.5; y < 360 * 1.5; y += radius * 1.5) {

		for (float x = -360 * 1.5; x < 360 * 1.5; x += x_span) {

			glm::vec3 location;
			if (flg) {

				location = glm::vec3(x, y, 0);
			}
			else {

				location = glm::vec3(x + (x_span / 2), y, 0);
			}

			this->location_list.push_back(location);
		}
		flg = !flg;
	}

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	this->face.clear();
	this->frame.clear();

	for (int i = 0; i < this->location_list.size(); i++) {

		this->setHexagonToMesh(this->face, this->frame, this->location_list[i], 12);
	}

}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	this->face.draw();
	this->frame.draw();

	this->cam.end();

	/*
	int start = 300;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
void ofApp::setHexagonToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float radius) {

	auto noise_value = ofNoise(glm::vec3(location.x * 0.002, location.y * 0.002, ofGetFrameNum() * 0.006));
	auto height = 0;
	auto max_height = 600;

	if (noise_value > 0.32 && noise_value <= 0.38) {

		height = ofMap(noise_value, 0.32, 0.38, 0, max_height);
	}

	if (noise_value > 0.38 && noise_value < 0.52) {

		height = max_height;
	}

	if (noise_value > 0.52 && noise_value <= 0.58) {

		height = ofMap(noise_value, 0.52, 0.58, max_height, 0);
	}

	for (int deg = 90; deg < 450; deg += 60) {

		auto face_radius = radius - 0.5;
		auto face_index = face_target.getNumVertices();
		auto frame_index = frame_target.getNumVertices();

		vector<glm::vec3> vertices;
		vertices.push_back(glm::vec3(0, 0, 0));
		vertices.push_back(glm::vec3(face_radius * cos(deg * DEG_TO_RAD), face_radius * sin(deg * DEG_TO_RAD), 0));
		vertices.push_back(glm::vec3(face_radius * cos((deg + 60) * DEG_TO_RAD), face_radius * sin((deg + 60) * DEG_TO_RAD), 0));
		vertices.push_back(glm::vec3(0, 0, height));
		vertices.push_back(glm::vec3(face_radius * cos((deg + 60) * DEG_TO_RAD), face_radius * sin((deg + 60) * DEG_TO_RAD), height));
		vertices.push_back(glm::vec3(face_radius * cos(deg * DEG_TO_RAD), face_radius * sin(deg * DEG_TO_RAD), height));

		for (auto& vertex : vertices) {

			vertex += location;
		}

		face_target.addVertices(vertices);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2);
		face_target.addIndex(face_index + 3); face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5);
		face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 4);
		face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 4); face_target.addIndex(face_index + 5);

		frame_target.addVertices(vertices);

		frame_target.addIndex(frame_index + 1); frame_target.addIndex(frame_index + 2);
		frame_target.addIndex(frame_index + 4); frame_target.addIndex(frame_index + 5);
		frame_target.addIndex(frame_index + 1); frame_target.addIndex(frame_index + 5);
		frame_target.addIndex(frame_index + 2); frame_target.addIndex(frame_index + 4);

		face_target.addColor(ofColor(239));
		face_target.addColor(ofColor(239));
		face_target.addColor(ofColor(239));
		face_target.addColor(ofColor(ofMap(height, 0, max_height, 239, 39)));
		face_target.addColor(ofColor(ofMap(height, 0, max_height, 239, 39)));
		face_target.addColor(ofColor(ofMap(height, 0, max_height, 239, 39)));

		frame_target.addColor(ofColor(39));
		frame_target.addColor(ofColor(39));
		frame_target.addColor(ofColor(39));
		frame_target.addColor(ofColor(ofMap(height, 0, max_height, 39, 239)));
		frame_target.addColor(ofColor(ofMap(height, 0, max_height, 39, 239)));
		frame_target.addColor(ofColor(ofMap(height, 0, max_height, 39, 239)));

	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}