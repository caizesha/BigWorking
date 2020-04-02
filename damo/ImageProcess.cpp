#include "pch.h"
#include "ImageProcess.h"



void pointMappingProcess(Mat& image1, Mat& image2, Mat& image1Out, Mat& image2Out, Mat& out, int detection, int mapping){
	vector<KeyPoint>keypoints1, keypoints2;
	Mat discriptions1, discriptions2;
	//���ּ�ⷽ��
	if (detection == 0) {
		Ptr<SIFT>sift = SIFT::create(200,3,0.04,20.0);
		//���ؼ���
		sift->detect(image1, keypoints1);
		sift->detect(image2, keypoints2);
		//��ͼ�л��ƹؼ��㣨������ԭͼ
		drawKeypoints(image1, keypoints1, image1Out, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
		drawKeypoints(image2, keypoints2, image2Out, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
		//������������
		sift->compute(image1, keypoints1, discriptions1);
		sift->compute(image2, keypoints2, discriptions2);
	}
	else {
		Ptr<SURF>surf = SURF::create(1000.0);
		surf->detect(image1, keypoints1);
		surf->detect(image2, keypoints2);
		drawKeypoints(image1, keypoints1, image1Out);
		drawKeypoints(image2, keypoints2, image2Out);
		surf->compute(image1, keypoints1, discriptions1);
		surf->compute(image2, keypoints2, discriptions2);
	}
	//��¼
	vector<DMatch>matches;
	Ptr<DescriptorMatcher>matcher;
	switch (mapping){
	case 0: {
		matcher = DescriptorMatcher::create("BruteForce");
		break;
	}
	case 1: {
		matcher = DescriptorMatcher::create("FlannBased");
		break;
	}
	default:
		break;
	}
	matcher->match(discriptions1, discriptions2, matches);
	sort(matches.begin(), matches.end());
	//vector<DMatch> match(matches.begin(), matches.begin() + int(matches.size() / 2));
	vector<DMatch> match(matches.begin(), matches.begin() + min(50,int(matches.size()/2)));
	//����ƥ��ͼ
	drawMatches(image1, keypoints1, image2, keypoints2, match, out);
}
