#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <math.h>
using namespace std;
using namespace cv;
Point getNextLoaction(Mat &result,double &maxValue, Point &maxLoc, int temp_width, int temp_height);//��ȡ��һ��ƥ���
Mat CalculateNcc(Mat src, Mat temp);//����ncc�е�result
/*
ȱ�ٸ�Ѱ�ҽ�����ƥ�����Ѳ���getOptimalLevels();
*/
int getOptimalLevels(Mat temp);
//���������ٵ�ƥ�䣬���س�����ֵ�����е�
vector<Point> pyramidMatch(Mat src, Mat temp, int nLevels,double thresh);
/*
1.������ʱֻ���嵥������������Point ���������Զ���struct����vector���е����ͣ��ڻ���ʱ��Ӧ�ð����Ƕ���Ϣ����ģ��߶���Ϣ
*/
void DrawMatchResult(vector<Point> pt,Mat display,int width,int height);
vector<Mat> pyramidImg(Mat src, int nLevels);//����ͼ��Ľ�����
/*
����������Ը���һ��̰���ȣ����Ų�ȵ����߶���ֵ���Խ���
*/
vector<Point> getMulMatchPoint(Mat &result, double thresh,int temp_width,int temp_height);//��ȡÿ�������ֵ�����ƥ���
//����ƥ��ĵ���²㸽��������ncc,�ҵ�����ò���ֵ�ĵ�ļ���
vector<Point> SearchArea(vector<Point> pt, Size area,int level,double thresh, Mat pyr_src, Mat pyr_temp);