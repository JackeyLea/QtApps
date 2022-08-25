/*我们不使用提取特征方式，而是采用纯像素数据作为输入，分别使用KNN与SVM对数据集进行训练与测试，比较他们的正确率
  KNN是最简单的机器学习方法、主要是计算目标与模型之间的空间向量距离得到最终预测分类结果。
*/

#include <iostream>
#include <string>
#include <fstream>
#include <time.h>

#include <opencv2/opencv.hpp>

class KNNTT
{
public:
    KNNTT() {}

    float getRate()
    {
        return _rate;
    }

    int getPredictResult()
    {
        return _predictResult;
    }

    //数字大端转换为小段
    inline int reverseDigit(int num)
    {
        unsigned char c1, c2, c3, c4;
        c1 = num & 255;
        c2 = (num >> 8) & 255;
        c3 = (num >> 16) & 255;
        c4 = (num >> 24) & 255;

        return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
    }

    //0->train 1->test
    //读取图像数据集
    cv::Mat readImagesData(int mode)
    {
        switch (mode)
        {
        case 0: //train
            f.open("train-images.idx3-ubyte", std::ios::binary);
            std::cout << "->读取训练用的图像数据." << std::endl;
            break;
        case 1: //test
            f.open("t10k-images.idx3-ubyte", std::ios::binary);
            std::cout << "->读取测试用的图像数据." << std::endl;
            break;
        }

        if (!f.is_open())
        {
            std::cout << "->无法读取图像数据" << std::endl;
            exit(-1);
        }

        /*
        byte 0 - 3 : Magic Number(Not to be used)
        byte 4 - 7 : Total number of images in the dataset
        byte 8 - 11 : rows of each image in the dataset
         byte 12 - 15 : cols of each image in the dataset
        */
        int magic_number = 0;
        int number_of_images = 0;
        int height = 0;
        int width = 0;

        f.read((char *)&magic_number, sizeof(magic_number));
        magic_number = reverseDigit(magic_number);

        f.read((char *)&number_of_images, sizeof(number_of_images));
        number_of_images = reverseDigit(number_of_images);
        std::cout << "->图像数量是：" << number_of_images << std::endl;

        f.read((char *)&height, sizeof(height));
        height = reverseDigit(height);

        f.read((char *)&width, sizeof(width));
        width = reverseDigit(width);

        cv::Mat train_images = cv::Mat(number_of_images, height * width, CV_8UC1);
        for (int i = 0; i < number_of_images; i++)
        { //第几张图
            for (int r = 0; r < height; ++r)
            {
                for (int c = 0; c < width; ++c)
                {
                    unsigned char temp = 0;
                    f.read((char *)&temp, sizeof(temp));
                    train_images.at<uchar>(i, r * width + c) = (int)temp;
                    //                if(i==0){
                    //                    Mat digit=Mat(height,width,CV_8UC1);
                    //                    digit.at<uchar>(r,c)=(int)temp;
                    //                    imwrite("digit.png",digit);//输出第一张图片
                    //                }
                }
            }
        }
        train_images.convertTo(train_images, CV_32F);
        f.close();
        std::cout << "->数据集图像数据读取完毕。" << std::endl;

        return train_images;
    }

    //读取标记数据集
    cv::Mat readLabelsData(int mode)
    {
        switch (mode)
        {
        case 0: //train
            f.open("train-labels.idx1-ubyte");
            std::cout << "->读取训练标签。" << std::endl;
            break;
        case 1: //test
            f.open("t10k-labels.idx1-ubyte");
            std::cout << "->读取测试标签。" << std::endl;
            break;
        }

        if (!f.is_open())
        {
            std::cout << "->无法读取标签数据" << std::endl;
            exit(-1);
        }
        /*
         byte 0 - 3 : Magic Number(Not to be used)
         byte 4 - 7 : Total number of labels in the dataset
        */
        int magic_number = 0;
        int number_of_labels = 0;
        f.read((char *)&magic_number, sizeof(magic_number));
        magic_number = reverseDigit(magic_number);

        f.read((char *)&number_of_labels, sizeof(number_of_labels));
        number_of_labels = reverseDigit(number_of_labels);
        std::cout << "->标签数量为：" << number_of_labels << std::endl;

        cv::Mat labels = cv::Mat(number_of_labels, 1, CV_8UC1);
        for (long int i = 0; i < number_of_labels; i++)
        {
            unsigned char temp = 0;
            f.read((char *)&temp, sizeof(temp));
            labels.at<uchar>(i, 0) = temp;
        }
        labels.convertTo(labels, CV_32S);
        f.close();
        std::cout << "->数据集标签数据读取完毕." << std::endl;

        return labels;
    }

    void train()
    {
        std::cout << "KNN方式训练数据开始" << std::endl;
        cv::Mat train_images = readImagesData(0);
        if (train_images.size == 0)
            return;

        cv::Mat train_labels = readLabelsData(0);
        if (train_labels.size == 0)
            return;
        std::cout << "->成功读取图像和标签" << std::endl;

        std::cout << "->KNN训练开始" << std::endl;
        time_start = (double)clock();
        cv::Ptr<cv::ml::KNearest> knn = cv::ml::KNearest::create();
        cv::Ptr<cv::ml::TrainData> tdata = cv::ml::TrainData::create(train_images, cv::ml::ROW_SAMPLE, train_labels);
        knn->train(tdata);
        knn->setDefaultK(5);
        knn->setIsClassifier(true);
        knn->save("knn.xml");
        time_end = (double)clock();
        std::cout << "->KNN训练数据已成功保存" << std::endl;
        std::cout << "->KNN训练耗时：" << (time_end - time_start) / 1000.0 <<"ms" << std::endl;
        std::cout << "KNN训练结束。" << std::endl;
    }

    void test()
    {
        std::cout << "KNN测试开始" << std::endl;

        std::ifstream file("knn.xml");
        if (!file.is_open())
        {
            std::cout << "->没有训练结果文件" << std::endl;
            return;
        }

        std::cout << "->开始导入KNN训练结果文件" << std::endl;
        cv::Ptr<cv::ml::KNearest> knn = cv::Algorithm::load<cv::ml::KNearest>("knn.xml");
        std::cout << "->已成功导入KNN训练结果文件" << std::endl;

        std::cout << "->开始导入测试数据" << std::endl;
        cv::Mat tData = readImagesData(1);
        if (tData.size == 0)
            return;
        cv::Mat tLabel = readLabelsData(1);
        if (tLabel.size == 0)
            return;
        std::cout << "->已成功导入测试数据" << std::endl;

        float total = tData.rows;
        float correct = 0;

        std::cout << "->KNN测试开始" << std::endl;
        time_start = (double)clock();
        cv::Rect rect;
        rect.x = 0;
        rect.height = 1;
        rect.width = (28 * 28);
        for (int i = 0; i < total; i++)
        {
            int actual = tLabel.at<int>(i);
            rect.y = i;
            cv::Mat oneImage = tData(rect);
            cv::Mat result;
            float predicted = knn->predict(oneImage, result);
            int digit = static_cast<int>(predicted);
            if (digit == actual)
            {
                correct++;
            }
        }
        time_end = (double)clock();
        _rate = correct / total * 100.0;
        std::cout << "->识别准确率是：" << _rate << std::endl;
        std::cout << "->KNN训练耗时：" << (time_end - time_start) / 1000.0 << "ms" << std::endl;
        std::cout << "KNN测试结束" << std::endl;
    }

    int predict(cv::Mat &img)
    {
        std::cout << "knn预测开始" << std::endl;
        std::ifstream file("knn.xml");
        if (!file.is_open())
        {
            std::cout << "->KNN训练结果文件knn.xml不存在" << std::endl;
            return -1;
        }

        std::cout << "->开始加载knn模型" << std::endl;
        cv::Ptr<cv::ml::KNearest> knn = cv::Algorithm::load<cv::ml::KNearest>("knn.xml");
        std::cout << "->knn模型加载完毕" << std::endl;

        cv::Mat result; //没有用，用于占位置
        float predicted = knn->predict(img, result);
        _predictResult = static_cast<int>(predicted);
        std::cout << "knn预测结束" << std::endl;

        return 0;
    }

private:
    std::ifstream f;

    float _rate;
    int _predictResult;
    double time_start;
    double time_end;
};

int main()
{
    KNNTT knntt;
    knntt.train();
    knntt.test();
}