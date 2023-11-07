#include <iostream>
#include <vector>
#include <fstream>

#include <thread>
#include <atomic>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/core/core.hpp"

std::atomic<long long> numberOfStars(0);

// Finding the stars via converting image to b/w and looking for a bright objects
cv::Mat findStars(const cv::Mat &spaceImage) {
    cv::Mat resultImage = spaceImage;

    cv::Mat grayImage;
    cv::cvtColor(resultImage, grayImage, cv::COLOR_BGR2GRAY);

    cv::Mat binaryImage;
    cv::threshold(grayImage, binaryImage, 180, 255, cv::THRESH_BINARY);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binaryImage, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (const auto &contour : contours) {
        cv::Point2f center;
        float radius;
        cv::minEnclosingCircle(contour, center, radius);
        cv::circle(resultImage, center, static_cast<int>(radius+4), cv::Scalar(0, 0, 255), 2);
        //cv::drawContours(resultImage, contours, -1, cv::Scalar(0, 0, 255), 2);
    }

    numberOfStars += static_cast<long long>(contours.size());
    return resultImage;
}

std::vector<std::vector<cv::Mat>> getPartition(const cv::Mat &image) {
    int rows = 10, cols = 10;

    int partWidth = image.cols / cols;
    int partHeight = image.rows / rows;

    std::vector<std::vector<cv::Mat>> imageParts(rows);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cv::Rect roi(j * partWidth, i * partHeight, partWidth, partHeight);
            cv::Mat part = image(roi);
            imageParts[i].push_back(part);
        }
    }

    return imageParts;
}

cv::Mat mergeParts(std::vector<std::vector<cv::Mat>> &imageParts) {
    // Concatenate horizontally
    std::vector<cv::Mat> imageRows(imageParts.size());
    for (int i = 0; i < imageParts.size(); i++) {
        imageRows[i] = imageParts[i][0];
        for (int j = 1; j < imageParts[i].size(); j++) {
            cv::hconcat(imageRows[i], imageParts[i][j], imageRows[i]);
        }
    }

    // Concatenate vertically
    cv::Mat resultImage = imageRows[0];
    for (int i = 1; i < imageRows.size(); i++) {
        cv::vconcat(resultImage, imageRows[i], resultImage);
    }

    return resultImage;
}

int main(int argc, char **argv) {
    //cv::Mat spaceImage = cv::imread("C:/Users/egorm/ClionProjects/LookUpTheSkies/image.png");
    cv::Mat spaceImage = cv::imread(argv[1]);
    std::cerr << "Image: " << argv[0] << std::endl;
    if (spaceImage.empty()) {
        std::cerr << "The image could not be opened." << std::endl;
        return -1;
    }

    std::vector<std::vector<cv::Mat>> imageParts = getPartition(spaceImage);


    std::vector<std::thread> threads;
    for (auto &imageRow : imageParts) {
        for (auto &imagePart : imageRow) {
            threads.emplace_back([&imagePart] {
                imagePart = findStars(imagePart);
            });
        }
    }

    for (auto &th : threads) {
        th.join();
    }


    cv::Mat output = mergeParts(imageParts);

    std::ofstream output_data("output_data.txt");
    output_data << numberOfStars;
    output_data.close();

    cv::imwrite("output.jpg", output);
    std::cout << "Number of stars: " << numberOfStars << std::endl;

    return 0;
}

//сv::drawContours(spaceImage, contours, -1, cv::Scalar(0, 0, 255), 2);