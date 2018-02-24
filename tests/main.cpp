#include <iostream>
#include <iomanip>
#include <string>

#include "ImageQualityAssessment.h"
using namespace std;

int main(int argc, char* argv[]){
    string image_path;
    if (argc == 2){
        image_path = argv[1];
    }else{
        cout << "Usage:"<< argv[0] << " image_path" << endl;
        return -1;
    }

    cv::Mat imageData = cv::imread(image_path.c_str());

    // sharpness
    double defrto = DefRto(imageData);
    cout << setw(10) << "DefRto: " << defrto << endl;

    // color exception
    float cast, da, db;
    colorException(imageData, cast, da, db);
    string colorDes1 = (da > 0)?"偏红":"偏绿";
    string colorDes2 = (db > 0)?"偏黄":"偏蓝";
    cout << setw(10) << "colorEpt: "
        << da << " " << colorDes1 << " "
        << db << " " << colorDes2 << endl;

    // brightness exception
    float brightcast, brightda;
    brightnessException(imageData, brightcast, brightda);
    string brightDes = (brightda > 0)?"偏亮":"偏暗";
    cout << setw(10) << "brightEpt: "
        << brightda << " " << brightDes << endl;

    // colorfulness  assessment
    double colorfulNess = ColorfulNess(imageData);
    cout << setw(10) << "colorRich: " << colorfulNess << endl;

    return 0;
}
