#include <functional>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <chrono>
#include <fstream>
#include <cstdio>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <ctime>
#include <unistd.h>

#include "sio_client.h"
#include "ConvertImage.h"
#include "alpr.h"
#include <time.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/videoio/videoio.hpp"

using namespace std;
using namespace sio;
using namespace cv;

// Global variables
Mat frame; //current frame
int keyboard; //input from keyboard

/** Function Headers */

void processVideo(char* videoFilename);
void sendData(string name,string data);

int main()
{
	//encode image
//	ImagemConverter imgCvt;
//	Mat image = imread("1.jpg", 1);
//	std::string encodeStr = imgCvt.mat2str(image);
	//std::cout << "encoded image: " <<  encodeStr;

	//decode image
	//Mat frame = imgCvt.str2mat(encodeStr);
	//imwrite("demmo.jpg", frame);

	//write to file
//	fstream f;
//	f.open("output.txt", ios::out);
//	f << encodeStr;
//	f.close();

	//start socket io
    //sio::client h;
    //h.connect("http://localhost:3000/");
    //string content = "done";
    //h.socket()->emit("csd", encodeStr);
    //std::this_thread::sleep_for(std::chrono::milliseconds(5000));

	//create GUI windows
	namedWindow("Frame");

	processVideo("DNG-fr-cam-160802-partial-short.mp4");
	//destroy GUI windows
	destroyAllWindows();

    return 0;
}

/**
* @function processVideo
*/
void processVideo(char* videoFilename) {
	std::vector<string> allMatches;
	std::string s;
	alpr::Alpr openalpr("vi_biendai_20151209", "/home/nhtoan/openalpr/runtime_data/config/vi_biendai_20151209.conf");
	openalpr.setTopN(20);
	openalpr.setDefaultRegion("vi_biendai_20151209");
	if (openalpr.isLoaded() == false)
		{
		    std::cerr << "Error loading OpenALPR" << std::endl;

		}
	//create the capture object
	VideoCapture capture(videoFilename);
	if (!capture.isOpened()) {
		//error in opening the video input
		cerr << "Unable to open video file: " << videoFilename << endl;
		exit(EXIT_FAILURE);
	}
	//read input data. ESC or 'q' for quitting
	while ((char)keyboard != 'q' && (char)keyboard != 27) {
		//read the current frame
		if (!capture.read(frame)) {
			cerr << "Unable to read next frame." << endl;
			cerr << "Exiting..." << endl;
			exit(EXIT_FAILURE);
		}

		std::vector<alpr::AlprRegionOfInterest> regionsOfInterest;

		regionsOfInterest.push_back(alpr::AlprRegionOfInterest(frame.cols/2, frame.rows/2, frame.cols, frame.rows));
		alpr::AlprResults results = openalpr.recognize(frame.data, frame.elemSize(), frame.cols, frame.rows, regionsOfInterest);
		// Iterate through the results.  There may be multiple plates in an image,
		// and each plate return sthe top N candidates.

		for (int i = 0; i < results.plates.size(); i++)
		{
		  alpr::AlprPlateResult plate = results.plates[i];
		  // std::cout << "plate" << i << ": " << results.plates.size() << " results" << std::endl;
          float currentConfidence = 0;
			for (int k = 0; k < plate.topNPlates.size(); k++)
			{
			  alpr::AlprPlate candidate = plate.topNPlates[k];
			  if (candidate.matches_template == 1 &&
					 candidate.overall_confidence > currentConfidence &&
					  std::find(allMatches.begin(), allMatches.end(), candidate.characters) == allMatches.end()) {
				  currentConfidence = candidate.overall_confidence;
				  std::cout << "FOUND: " << candidate.characters << std::endl;
				  allMatches.push_back(candidate.characters);
//				  s="/home/nhtoan/workspace/FinalProject/image/"+candidate.characters+".jpg";
//				  imwrite(s, frame);
				  ImagemConverter imgCvt;
				  std::string encodeStr = imgCvt.mat2str(frame);
				  sendData("csd", candidate.characters+ ' ' +encodeStr);
				  //sendData("csdout", candidate.characters+ ' ' +encodeStr);
				  usleep(10000000);
			  }
			}
		}
		imshow("Frame", frame);
		//get the input from the keyboard
		keyboard = waitKey(30);
	}
	//delete capture object
	capture.release();
}
void sendData(string name,string data){
	sio::client h;
	h.connect("http://localhost:3000/");
	h.socket()->emit(name, data);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

