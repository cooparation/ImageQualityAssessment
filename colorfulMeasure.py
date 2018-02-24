# coding:utf-8
import sys
import cv2
import numpy as np

def image_colorfulness(image):
    #将图片分为B,G,R三部分（注意，这里得到的R、G、B为向量而不是标量）
    (B, G, R) = cv2.split(image.astype("float"))

    #rg = R - G
    rg = np.absolute(R - G)

    #yb = 0.5 * (R + G) - B
    yb = np.absolute(0.5 * (R + G) - B)

    #计算rg和yb的平均值和标准差
    (rbMean, rbStd) = (np.mean(rg), np.std(rg))
    (ybMean, ybStd) = (np.mean(yb), np.std(yb))

    #计算rgyb的标准差和平均值
    stdRoot = np.sqrt((rbStd ** 2) + (ybStd ** 2))
    meanRoot = np.sqrt((rbMean ** 2) + (ybMean ** 2))

    # 返回颜色丰富度C
    return stdRoot + (0.3 * meanRoot)

if __name__ == "__main__":
    image_path = './tmp/000002.jpg'
    if(len(sys.argv) == 2):
        image_path = sys.argv[1]
    image = cv2.imread(image_path)
    print(image_colorfulness(image))
