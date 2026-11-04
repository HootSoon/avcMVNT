#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>
#include <fstream>
#include <opencv2/dnn/all_layers.hpp>

using namespace cv;
using namespace std;
using namespace dnn;

int main() {
    string modelWeights = "";
    vector<string> class_names;
    ifstream ifs(string(file_path + "").c_str());
    string line; 

    //load in classnames from file location
    while (getline(ifs, line))
    {
        cout << line << endl;
        class_names.push_back(line);
    }

    auto net = readNet(file_path + "", file_path + "", "TensorFlow");

    Videocapture cap(0);

    //run on either cpu or gpu 
    //net.setPreferableBackend(DNN_BACKEND_CUDA);
    //net.setPreferableTarget(DNN_TARGET_CUDA);

    float min_confidence_score = 0.5;

    while(cap.isOpened())
    {
        //load image
        Mat image;
        bool isSuccess = cap.read(image);


        if (!isSuccess)
        {
            cout << "Cannot load image" << endl;
            break;
        }

        int image_height = image.cols
        int image_width = image.rows

        auto start = getTickCount();

        //creates a blob from the image
        Mat blob = blobFromImage(image, 1.0, Size(300, 300), Scalar(127.5, 127.5, 127.5), true, false)

        net.setInput(blob);

        Mat output = net.forward();

        auto end = getTickCount();

        //tbc

    }
}