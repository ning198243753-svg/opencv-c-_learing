#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/geometry.hpp>
#include <iostream>

using namespace cv;
using namespace std;

vector<Point> initialPoints;
vector<Point> conpoly;
vector<Point2f> finalPoints;

int MAX_Area = 0;

vector<Point> getInitialPoints(Mat& img) {
    Mat imgGray, imgBlur, imgCanny, imgDilated;

    cvtColor(img, imgGray, COLOR_BGR2GRAY);
    GaussianBlur(imgGray, imgBlur, Size(5, 5), 1);
    Canny(imgBlur, imgCanny, 50, 150);
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
    dilate(imgCanny, imgDilated, kernel);

    vector<vector<Point>> contours;
    findContours(imgDilated, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    vector<Point> points;
    
    for (const auto& contour : contours) {
        approxPolyDP(contour, conpoly, 0.02 * arcLength(contour, true), true);
        double area = contourArea(conpoly);
        if (conpoly.size() == 4 && area > MAX_Area) {
            MAX_Area = area;
            points = conpoly;
            // drawContours(img, vector<vector<Point>>{conpoly}, -1, Scalar(255, 0, 255), 2);
        }
        
    }
    return points;
}


vector<Point2f> reoder(vector<Point> points) {
    vector<Point2f> newPoints(4);
    vector<int> sumPoints, subPoints;

    for (const auto& point : points) {
        sumPoints.push_back(point.x + point.y);
        subPoints.push_back(point.x - point.y);
    }

    newPoints[0] = points[min_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]; // top-left
    newPoints[3] = points[max_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]; // bottom-right
    newPoints[2] = points[min_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]; // top-right
    newPoints[1] = points[max_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]; // bottom-left

    return newPoints;
}

int main() {
    Mat img = imread("Resources/paper.jpg"); // Replace with your image path

    resize(img, img, Size(), 0.5, 0.5); // Resize for better visibility
    vector<Point> initialPoints = getInitialPoints(img);
    finalPoints = reoder(initialPoints);
    Mat Matrix = getPerspectiveTransform(finalPoints, vector<Point2f>{Point2f(0.0f, 0.0f), Point2f(300.0f, 0.0f), Point2f(0.0f, 400.0f), Point2f(300.0f, 400.0f)});
    Mat imgWarped;
    warpPerspective(img, imgWarped, Matrix, Size(300, 400));

    imshow("Image", img);
    imshow("Warped Image", imgWarped);
    
    waitKey(0);


    return 0;
}