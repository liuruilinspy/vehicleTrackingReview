#pragma once

#define DEFAULT_FRAMEDIFF_THRES 10
#define DEFAULT_DISTANCE_THRES 200

#define BASE_TYPE 0
#define MATCH_TYPE 1

struct ImageInfo{
	string imageName;
	string videoName;
	long startFrame;
	long endFrame;
	cv::Point2d startPos;
	cv::Point2d endPos;
	unsigned int trackNum;
	int srcIndex;
};

class ImageInfoSet
{
public:
	vector<ImageInfo> allImages;
	map<int, int> srcIndex2objIndex;
	int baseIndex;
	int matchIndex;

public:
	ImageInfoSet(std::vector<string> imageNameList);
	~ImageInfoSet();

	//depending on the value of movement(>0||==0||<0), the following function returns the next/current/previous image in the list
	ImageInfo* getNextMatchImageInfo(int movement, int thresFrameDiff = DEFAULT_FRAMEDIFF_THRES, int thresDistance = DEFAULT_DISTANCE_THRES);
	ImageInfo* getNextBaseImageInfo(int movment, bool& hasPotentialMatch, int thresFrameDiff = DEFAULT_FRAMEDIFF_THRES, int thresDistance = DEFAULT_DISTANCE_THRES);
};

