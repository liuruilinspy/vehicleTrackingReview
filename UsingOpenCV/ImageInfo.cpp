#include "stdafx.h"
#include "ImageInfo.h"

#define MAX_OVERLAP 0
#define SPLITTER '$'
#define ASSIGNER '='

void split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
}

bool sortStartFrame(ImageInfo a, ImageInfo b){
	return a.startFrame < b.startFrame;
}

ImageInfoSet::ImageInfoSet(std::vector<string> imageNameList){
	baseIndex = 0;
	matchIndex = baseIndex;

	std::vector<std::string> elems;
	std::vector<std::string> keyAndValue;
	for (int i = 0; i < imageNameList.size(); i++){
		string curImageName = imageNameList[i];
		elems.clear();
		split(curImageName, SPLITTER, elems);

		ImageInfo imageInfo;
		imageInfo.imageName = curImageName;
		imageInfo.videoName = "";
		for (int j = 0; j < elems.size(); j++){
			string curAttri = elems[j];
			keyAndValue.clear();
			split(curAttri, ASSIGNER, keyAndValue);
			
			if (keyAndValue.size() != 2) continue;

			if (keyAndValue[0].compare("name") == 0){
				imageInfo.videoName = keyAndValue[1];
			}
			else if (keyAndValue[0].compare("startframe") == 0){
				imageInfo.startFrame = stol(keyAndValue[1]);
			}
			else if (keyAndValue[0].compare("endframe") == 0){
				imageInfo.endFrame = stol(keyAndValue[1]);
			}
			else if (keyAndValue[0].compare("track") == 0){
				imageInfo.trackNum = stoi(keyAndValue[1]);
			}
			else if (keyAndValue[0].compare("startPosX") == 0){
				imageInfo.startPos.x = stod(keyAndValue[1]);
			}
			else if (keyAndValue[0].compare("startPosY") == 0){
				imageInfo.startPos.y = stod(keyAndValue[1]);
			}
			else if (keyAndValue[0].compare("endPosX") == 0){
				imageInfo.endPos.x = stod(keyAndValue[1]);
			}
			else if (keyAndValue[0].compare("endPosY") == 0){
				imageInfo.endPos.y = stod(keyAndValue[1]);
			}
		}

		if (imageInfo.videoName.compare("") == 0) continue;
		imageInfo.srcIndex = i;
		this->allImages.push_back(imageInfo);
	}

	sort(allImages.begin(), allImages.end(), sortStartFrame);

	for (int i = 0; i < allImages.size(); i++){
		ImageInfo curInfo = allImages[i];
		srcIndex2objIndex[curInfo.srcIndex] = i;
	}
}

ImageInfoSet::~ImageInfoSet(){

}

ImageInfo* ImageInfoSet::getNextMatchImageInfo(int movement, int thresFrameDiff, int thresDistance){
	int myBaseIndex = baseIndex;
	int myMatchIndex = matchIndex;

	ImageInfo baseImage = allImages[myBaseIndex];
	int myTestIndex = (movement == 0) ? myMatchIndex : (movement > 0)? myMatchIndex + 1 : myMatchIndex - 1;
	while (myTestIndex >= 0 && myTestIndex < allImages.size()){
		if (myTestIndex == myBaseIndex){
			myTestIndex = (movement >= 0) ? myTestIndex + 1 : myTestIndex - 1;
			continue;
		}

		ImageInfo testImage = allImages[myTestIndex];
		long frameDiff = testImage.startFrame - baseImage.endFrame;
		cv::Point2d mv(testImage.startPos - baseImage.endPos);
		long distance = sqrt(mv.ddot(mv));
		if (frameDiff >= -MAX_OVERLAP && frameDiff <= thresFrameDiff && distance <= thresDistance){
			matchIndex = myTestIndex;
			return &allImages[myTestIndex];
		}

		myTestIndex = (movement>=0) ? myTestIndex + 1 : myTestIndex - 1;
	}

	return NULL;
}

ImageInfo* ImageInfoSet::getNextBaseImageInfo(int movement, bool& hasPotentialMatch, int thresFrameDiff, int thresDistance){
	int initBaseIndex = baseIndex;

	int targetIndex = (movement == 0) ? initBaseIndex : movement > 0 ? initBaseIndex + 1 : initBaseIndex - 1;
	if (targetIndex >= allImages.size() || targetIndex <0){
		return NULL;
	}
	
	baseIndex = targetIndex;
	matchIndex = baseIndex;

	hasPotentialMatch = false;
	if (getNextMatchImageInfo(+1, thresFrameDiff, thresDistance) != NULL){
		hasPotentialMatch = true;
	}
	matchIndex = baseIndex;
	return &allImages[baseIndex];
}