#include <iostream>
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
    double defrto = DefRto(imageData);
    cout << "DefRto " << defrto << endl;

    float cast, da, db;
    colorException(imageData, cast, da, db);
    string colorDes1 = (da > 0)?"偏红":"偏绿";
    string colorDes2 = (db > 0)?"偏黄":"偏蓝";
    cout << "colorEpt "
        << da << " " << colorDes1 << " "
        << db << " " << colorDes2 << endl;
    
    float brightcast, brightda;
    brightnessException(imageData, brightcast, brightda);
    string brightDes = (brightda > 0)?"偏亮":"偏暗";
    cout << "brightnessEpt "
        << brightda << " " << brightDes << endl;
    
    double colorfulNess = ColorfulNess(imageData);

    cout << "colorfulNess " << colorfulNess << endl;

}
