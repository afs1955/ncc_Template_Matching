#include "match_demo.h"
Point getNextLoaction(Mat &result, double &maxValue, Point &maxLoc, int temp_width, int temp_height)
{
	/*
		���ڻ���ncc��ģ��ƥ�䣬result�����ֵԽ��ĵ�
		��Ѱ��һ����Ĺ�����ģ�帽������һ��ƥ��ֵ���ϴ󣬽�����������0��
	*/
	//imshow("result", result);
	int startX = maxLoc.x - temp_width / 5;
	int startY = maxLoc.y - temp_height / 5;
	int endX = maxLoc.x + temp_width / 5;
	int endY = maxLoc.y + temp_height / 5;
	if (startX < 0){
		startX = 0;	
	}
	if (startY < 0) {
		startY = 0;
	}
	if (endX > result.cols - 1){
		endX = result.cols;
	}
	if (endY > result.rows - 1) {
		endY = result.rows;
	}
	//�������������resultֵ�޸�
	for (int i = startY; i < endY; i++) {
		for (int j = startX; j < endX; j++) {
 			result.at<float>(i, j) = 0;
		}
	}
	double minValue;
	Point minLoc;
	minMaxLoc(result, &minValue, &maxValue, &minLoc, &maxLoc);
	//normalize(result, result, 0, 1, NORM_MINMAX);
	/*if (new_maxValue < thresh) {
		cout << "�����ڴ�����ֵ��ƥ���" << endl;
		return Point(-1,-1);
	}*/
	//imshow("result_end", result);
	return maxLoc;
}
Mat CalculateNcc(Mat src, Mat temp)
{
	Mat result;
	int result_w = src.cols - temp.cols + 1;
	int result_h = src.rows - temp.rows + 1;
	result.create(result_h, result_w, CV_32FC1);
	matchTemplate(src, temp, result, TM_CCOEFF_NORMED);
	//normalize(result, result, 0, 1, NORM_MINMAX);
	return result;
}
vector<Mat> pyramidImg(Mat src, int nLevels)
{
	//����Ϊ�˱�֤ƥ��׼ȷ�Ƚ���ģ��ƥ��ʹ�õ���resize˫���Բ�ֵ����pyrDown()����
	vector<Mat> imgs;
	imgs.push_back(src);
	while (nLevels > 1)
	{
		//resize(src, src, Size(0, 0), 0.5, 0.5, INTER_LINEAR);
		pyrDown(src, src, Size(src.cols / 2, src.rows / 2));
		imgs.push_back(src);
		nLevels--;
	}
	return imgs;
}
vector<Point> getMulMatchPoint(Mat &result, double thresh, int temp_width, int temp_height)
{
	vector<Point> pt;//�洢���
	double minValue, maxValue;
	Point minLoc, maxLoc;
	minMaxLoc(result, &minValue, &maxValue, &minLoc, &maxLoc);
	//double mid = (minValue + maxValue) / 2.0;
	//cout << "�м���ֵ" << mid << endl;
	while (maxValue >= thresh)
	{
		//cout << maxLoc.x << " " << maxLoc.y << endl;
		pt.push_back(maxLoc);
		maxLoc = getNextLoaction(result, maxValue, maxLoc, temp_width, temp_height);
		maxValue = maxValue;
	}
	return pt;
}
/*
��ʱֻ���Ƕ�ģ�岻�漰����Ƕȣ����ý���������ȥƥ��
@pyr_src:�洢Դͼ�������
@pyr_temp:�洢ģ�������
������ײ�ĵõ���ƥ���@<vector>Point pt;
*/
vector<Point> pyramidMatch(Mat src, Mat temp, int nLevels, double thresh)//thresholdΪ��ֵ
{

	vector<Mat> pyr_src;
	vector<Mat> pyr_temp;
	//vector<Point> pt;
	pyr_src = pyramidImg(src, nLevels);
	pyr_temp = pyramidImg(temp, nLevels);
	/*for (int i = 0; i < nLevels ; i++)
	{
		imshow("pyr_src" + i, pyr_src[i]);
		imshow("pyr_temp" + i, pyr_temp[i]);

	}*/
	Mat imgNcc =CalculateNcc(pyr_src[nLevels - 1], pyr_temp[nLevels - 1]);//top-result
	int temp_width = pyr_temp[nLevels - 1].cols;
	int temp_height = pyr_temp[nLevels - 1].rows;
	double th = thresh;
	for (int i = 0; i < nLevels - 1; i++)
	{
		th *= 0.9;
	}
	cout << "��߲����ֵ��" << th << endl;
	vector<Point> tpl = getMulMatchPoint(imgNcc, th, temp_width, temp_height);//���ܸ߲㷵������ƥ��� 
	cout << "��߲�ƥ��ĵ�" << endl;
	cout << "��߲�ƥ�����"<<tpl.size() << endl;
	for (int i = 0; i < tpl.size(); i++)
	{
		cout << tpl[i].x << " " << tpl[i].y << endl;
	}
	cout << "------------" << endl;
	int k = nLevels - 1;
	//for (int i = 0; i < tpl.size(); i++)
	//{
	//	//cout << tpl[i].x << " " << tpl[i].y << endl;
	//	pt.push_back(Point(tpl[i].x*int(pow(2, k)), tpl[i].y*int(pow(2, k))));
	//	//cout << pt[i].x << " " << pt[i].y << endl;
	//}
	//return pt;
	/*
	*****
	*/
	while (k > 0)
	{
		vector<Point>tmp;
		tmp = SearchArea(tpl, Size(5, 5),k-1,thresh,pyr_src[k - 1], pyr_temp[k - 1]);
		cout << "��������" << k - 1 << "��" <<"�ҵ�"<<tmp.size()<<"����"<< endl;
		for (int i = 0; i < tmp.size(); i++)
		{
			cout << tmp[i].x << " " << tmp[i].y << endl;
		}
		k--;
		tpl = tmp;
	}
	return tpl;
	
}
void DrawMatchResult(vector<Point> pt, Mat display, int width, int height)//���ƽ��
{
	cout << "������" << pt.size() << "����" << endl;
	for (int i = 0; i < pt.size(); i++)
	{
		//cout << pt[i].x << " " << pt[i].y << endl;
		rectangle(display, pt[i], Point(pt[i].x + width, pt[i].y + height), Scalar(255, 255, 255));
	}
}
vector<Point> SearchArea(vector<Point>pt, Size area,int level,double thresh,Mat pyr_src,Mat pyr_temp)
{
	vector<Point> res;
	Mat result;
	result.create(area, CV_32FC1);
	for (int i = 0; i < level; i++)
	{
		thresh *= 0.9;
	}
	cout <<  "��ǰ��" << level<< "��ֵ"<<thresh<< endl;
	for (int i = 0; i < pt.size(); i++)
	{
		Point p;
		p.x = pt[i].x * 2;
		p.y = pt[i].y * 2;
		int pys_width = area.width + pyr_temp.cols - 1;
		int pys_height = area.height + pyr_temp.rows - 1;
		int x=0, y=0;
		x = p.x - int((area.width / 2));//���Ͻǵ�
		y = p.y - int((area.height / 2));
		//bug����result���߽���
		if ((x + pys_width <= pyr_src.cols) && (y + pys_height <= pyr_src.rows)&&x>=0&&y>=0)
		{
			Rect rect(x, y, pys_width, pys_height);
			Mat s = pyr_src(rect);
			matchTemplate(s, pyr_temp, result, TM_CCOEFF_NORMED);
			//normalize(result, result, 0, 1, NORM_MINMAX);
			double minValue, maxValue;
			Point minLoc, maxLoc;
			minMaxLoc(result, &minValue, &maxValue, &minLoc, &maxLoc);
			if (maxValue >= thresh) {
				//cout << maxLoc.x << "----" << maxLoc.y << endl;
				//cout << "���λ��" << x+maxLoc.x << " " << y+maxLoc.y << endl;
				//cout << maxValue << endl;
				res.push_back(Point(x + maxLoc.x, y + maxLoc.y));
			}
		}
	}
	return res;
}
int getOptimalLevels(Mat temp)
{
	int nLevels = 1;
	int width = temp.cols;
	int height = temp.rows;
	while (width >= 16 && height >= 16)
	{
		width /= 2;
		height /= 2;
		nLevels++;
	}
	return nLevels;
}