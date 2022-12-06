/***********************************************************************
参考课程资料修改而成
************************************************************************/
#include <iostream>
#include <opencv2/opencv.hpp>
#include <io.h>
#include "edge_detection.h"
#include <time.h>
#include <algorithm>

using namespace std;
using namespace cv;


string SAVE = "";
string LOAD = "";

void int2str(const int& int_temp, string& string_temp) {
    stringstream stream;
    stream << int_temp;
    string_temp = stream.str();   //此处也可以用 stream>>string_temp  
}

/********************************************
 * 字符串分割
 *******************************************/
void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c) {
    std::string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while (std::string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2 - pos1));

        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if (pos1 != s.length())
        v.push_back(s.substr(pos1));
}

/*********************************************
*查找字符c最后出现的位置
**********************************************/
int findchar(string str, unsigned char c) {
    if (str.length() <= 0) {
        return 0;
    }

    for (int i = str.length() - 1; i >= 0; i--) {
        if (str[i] == c) {
            return i + 1;
        }
    }
    return 0;
}

vector<string> readFiles(string path) {
    intptr_t hFiles;
    struct _finddata_t FileInfo;
    vector<string> ret;
    string p;
    p.assign(path).append("\\*.bmp");
    hFiles = _findfirst(p.c_str(), &FileInfo);
    if (-1 == hFiles) return {};
    else {
        do {
            printf("%s\n", FileInfo.name);
            ret.emplace_back(path + "\\" + FileInfo.name);
        } while (!_findnext(hFiles, &FileInfo));
    }
    _findclose(hFiles);
    return ret;
}

int main() {
    vector<string> files = readFiles(LOAD);
    for (auto& path : files) {
        Mat srcImg = imread(path.c_str(), 0);
        //vector<string> stringVec;
        //SplitString(tmpStr, stringVec,"/");
        imshow("22", srcImg);
        // waitKey();
        //************预处理过程********************
        if (!srcImg.empty()) {
            int gau_win_width = 25;   // 取奇数
            cv::Mat outImg = srcImg.colRange(min_y, max_y);
            cv::GaussianBlur(srcImg, srcImg, cv::Size(5, 5), 2, 2);
            int pos = findchar(path, '\\');
            vector<vector<int>> u_b_1 = edge_detect(srcImg, min_y, max_y, 1);
            vector<int> u_y = u_b_1[0];
            vector<int> b_y = u_b_1[1];
            addMask((Mat&)outImg, (vector<int>&) u_y, (vector<int>&) b_y);
            imshow("11", outImg);
            // waitKey();
            string s_name = path.substr(pos, path.length() - pos);
            cout << s_name << endl;
            string save_name = SAVE + s_name;
            imwrite(save_name, outImg);   //保存ROI图像
        }
    }
}
