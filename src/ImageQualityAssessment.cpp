#include "ImageQualityAssessment.h"
using namespace cv;

/********************************************************************************
*函数描述： ColorfulNess 计算并返回一幅图像的颜色丰富度
*函数参数： frame  彩色帧图
*函数返回值：double 颜色丰富度表示值
*********************************************************************************/
double ColorfulNess(Mat frame)
{
    // split image to 3 channels (B,G,R)
    Mat channelsBGR[3];
    cv::split(frame, channelsBGR);

    // rg = R - G
    // yb = 0.5*(R + G) - B
    Mat rg, yb;
    cv::absdiff(channelsBGR[2], channelsBGR[1], rg);
    cv::absdiff(0.5*(channelsBGR[2] + channelsBGR[1]), channelsBGR[0], yb);

    // calculate the mean and std for rg and yb
    Mat rgMean, rgStd; // 1*1矩阵
    meanStdDev(rg, rgMean, rgStd);
    Mat ybMean, ybStd; // 1*1矩阵
    meanStdDev(yb, ybMean, ybStd);

    // calculate the mean and std for rgyb
    double stdRoot, meanRoot;
    stdRoot = sqrt(pow(rgStd.at<double>(0, 0), 2)
            + pow(ybStd.at<double>(0, 0), 2));
    meanRoot = sqrt(pow(rgMean.at<double>(0, 0), 2)
            + pow(ybMean.at<double>(0, 0), 2));

    // return colorfulNess
    return stdRoot + (0.3 * meanRoot);
}

/********************************************************************************
*函数描述： DefRto 计算并返回一幅图像的清晰度
*函数参数： frame  彩色帧图
*函数返回值：double 清晰度表示值，针对该视频，当清晰度小于10为模糊，大于14为清楚
*********************************************************************************/
double DefRto(Mat frame)
{
    Mat gray;
    cvtColor(frame,gray,CV_BGR2GRAY);

    IplImage tmp = gray;
    IplImage *img = &tmp;
    //IplImage *img = &IplImage(tmp);
    double temp = 0;
    double DR = 0;
    int i,j;//循环变量
    int height=img->height;
    int width=img->width;
    int step=img->widthStep/sizeof(uchar);
    uchar *data=(uchar*)img->imageData;
    double num = width*height;

    for(i=0;i<height;i++)
    {
        for(j=0;j<width;j++)
        {
            temp += sqrt((pow((double)(data[(i+1)*step+j]-data[i*step+j]),2) + pow((double)(data[i*step+j+1]-data[i*step+j]),2)));
            temp += abs(data[(i+1)*step+j]-data[i*step+j])+abs(data[i*step+j+1]-data[i*step+j]);
        }
    }
    DR = temp/num;
    return DR;
}

/********************************************************************************************
*函数描述： calcCast    计算并返回一幅图像的色偏度以及，色偏方向
*函数参数： InputImg    需要计算的图片，BGR存放格式，彩色（3通道），灰度图无效
*           cast        计算出的偏差值，小于1表示比较正常，大于1表示存在色偏
*           da          红/绿色偏估计值，da大于0，表示偏红；da小于0表示偏绿
*           db          黄/蓝色偏估计值，db大于0，表示偏黄；db小于0表示偏蓝
*函数返回值：   返回值通过cast、da、db三个应用返回，无显式返回值
*********************************************************************************************/
void colorException(Mat InputImg,float& cast,float& da,float& db)
{
    Mat LABimg;
    cvtColor(InputImg,LABimg,CV_BGR2Lab);//参考http://blog.csdn.net/laviewpbt/article/details/9335767
                                       //由于OpenCV定义的格式是uint8，这里输出的LABimg从标准的0～100，-127～127，-127～127，被映射到了0～255，0～255，0～255空间
    float a=0,b=0;
    int HistA[256],HistB[256];
    for(int i=0;i<256;i++)
    {
        HistA[i]=0;
        HistB[i]=0;
    }
    for(int i=0;i<LABimg.rows;i++)
    {
        for(int j=0;j<LABimg.cols;j++)
        {
            a+=float(LABimg.at<cv::Vec3b>(i,j)[1]-128);//在计算过程中，要考虑将CIE L*a*b*空间还原 后同
            b+=float(LABimg.at<cv::Vec3b>(i,j)[2]-128);
            int x=LABimg.at<cv::Vec3b>(i,j)[1];
            int y=LABimg.at<cv::Vec3b>(i,j)[2];
            HistA[x]++;
            HistB[y]++;
        }
    }
    da=a/float(LABimg.rows*LABimg.cols);
    db=b/float(LABimg.rows*LABimg.cols);
    float D =sqrt(da*da+db*db);
    float Ma=0,Mb=0;
    for(int i=0;i<256;i++)
    {
        Ma+=abs(i-128-da)*HistA[i];//计算范围-128～127
        Mb+=abs(i-128-db)*HistB[i];
    }
    Ma/=float((LABimg.rows*LABimg.cols));
    Mb/=float((LABimg.rows*LABimg.cols));
    float M=sqrt(Ma*Ma+Mb*Mb);
    float K=D/M;
    cast = K;
    return;
}

/*********************************************************************************************************************************************************
*函数描述： brightnessException     计算并返回一幅图像的色偏度以及，色偏方向
*函数参数： InputImg    需要计算的图片，BGR存放格式，彩色（3通道），灰度图无效
*           cast        计算出的偏差值，小于1表示比较正常，大于1表示存在亮度异常；当cast异常时，da大于0表示过亮，da小于0表示过暗
*函数返回值：   返回值通过cast、da两个引用返回，无显式返回值
**********************************************************************************************************************************************************/
void brightnessException (Mat InputImg,float& cast,float& da)
{
    Mat GRAYimg;
    cvtColor(InputImg,GRAYimg,CV_BGR2GRAY);
    float a=0;
    int Hist[256];
    for(int i=0;i<256;i++)
    Hist[i]=0;
    for(int i=0;i<GRAYimg.rows;i++)
    {
        for(int j=0;j<GRAYimg.cols;j++)
        {
            a+=float(GRAYimg.at<uchar>(i,j)-128);//在计算过程中，考虑128为亮度均值点
            int x=GRAYimg.at<uchar>(i,j);
            Hist[x]++;
        }
    }
    da=a/float(GRAYimg.rows*InputImg.cols);
    float D =abs(da);
    float Ma=0;
    for(int i=0;i<256;i++)
    {
        Ma+=abs(i-128-da)*Hist[i];
    }
    Ma/=float((GRAYimg.rows*GRAYimg.cols));
    float M=abs(Ma);
    float K=D/M;
    cast = K;
    return;
}
