#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/geometry.hpp>
#include <iostream>

using namespace cv;
using namespace std;


int hueMin = 0, hueMax = 179, satMin = 0, satMax = 255, valMin = 0, valMax = 255;

vector<vector<int>> colors = {
    {140, 0, 102, 179, 255, 255},  
    {160, 100, 50, 180, 255, 255} 
};
vector<Scalar> colorValues = {
    {0, 0, 255},    
    {0, 255, 0}     
};

struct canvasColor {
    Point cpoint;
    int colorIndex;
};





canvasColor findColor(Mat& img) {

    Mat imgHSV;
    cvtColor(img, imgHSV, COLOR_BGR2HSV);

    for (int i = 0; i < colors.size(); i++) {
        Scalar lower(colors[i][0], colors[i][1], colors[i][2]);
        Scalar upper(colors[i][3], colors[i][4], colors[i][5]);

        Mat mask;
        inRange(imgHSV, lower, upper, mask);

        vector<vector<Point>> contours;
        findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        for (int j = 0; j < contours.size(); j++) {
            double area = contourArea(contours[j]);
            if (area > 1000) {
                drawContours(img, contours, j, colorValues[i], 2);
                Rect boundRect = boundingRect(contours[j]);
                rectangle(img, boundRect.tl(), boundRect.br(), colorValues[i], 2);
                return {Point(boundRect.x + boundRect.width / 2, boundRect.y ), i};

            }
        }
        
    }
    return {Point(-1, -1), -1};
}

 void drawOnCanvas(Mat &img, vector<canvasColor> colorPoints,vector<Scalar> colorValues) {
    for (const auto& point : colorPoints) {
        if (point.cpoint.x != -1 && point.cpoint.y != -1) {
            circle(img, point.cpoint, 10, colorValues[point.colorIndex], FILLED);
        }
    }
    }

int main() {



    VideoCapture cap(0);
    Mat img;
            

    vector<canvasColor> newPoints;

    // namedWindow("Image");
    // createTrackbar("Hue Min", "Image", &hueMin, 179);
    // createTrackbar("Hue Max", "Image", &hueMax, 179);
    // createTrackbar("Sat Min", "Image", &satMin, 255);
    // createTrackbar("Sat Max", "Image", &satMax, 255);
    // createTrackbar("Val Min", "Image", &valMin, 255);
    // createTrackbar("Val Max", "Image", &valMax, 255);
 
  
    while (true) {
        cap.read(img);


        if (img.empty()) {
          
            continue;;
        }

        canvasColor colorPoint = findColor(img);
        newPoints.push_back(colorPoint);
        drawOnCanvas(img, newPoints, colorValues);
        // cvtColor(img, imgHSV, COLOR_BGR2HSV);
        // inRange(imgHSV, Scalar(hueMin, satMin, valMin), Scalar(hueMax, satMax, valMax), mask);
        imshow("Image", img);
        // imshow("mask", mask);
        if (waitKey(1) == 27) break;  
    }
    return 0;
}
