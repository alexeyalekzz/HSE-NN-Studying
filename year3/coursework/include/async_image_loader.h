//
// Created by Alexey Klimov on 13.01.2022.
//

#ifndef ASYNCIMAGELOADER_H_
#define ASYNCIMAGELOADER_H_


#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <readerwriterqueue.h>


using moodycamel::ReaderWriterQueue;

class AsyncImageLoader {
public:
    explicit AsyncImageLoader(const std::string& folder, size_t start_frame,
                              size_t last_frame, bool color = true)
        : curr(start_frame)
        , end(last_frame)
        , mQueue(last_frame - start_frame)
        , mDatasetFolder(folder)
        , mIsColor(color)
        , mThread([start_frame, last_frame, this]() {
            for(size_t i = start_frame; (i < last_frame) && !mIsFinish; ++i) {
                cv::Mat l, r;
                syncLoad(i, l, r);
                mQueue.emplace(std::move(l), std::move(r));
            }
        })
    {}

    bool get(cv::Mat& left, cv::Mat& right) {
        if (curr == end) return false;
        std::pair<cv::Mat, cv::Mat> temp{};
        while (!mQueue.peek()) {}

        bool res = mQueue.try_dequeue(temp);
        left = std::move(temp.first);
        right = std::move(temp.second);

        curr++;
        return res;
    }

    ~AsyncImageLoader() {
        mIsFinish = true;
        mThread.join();
    }

private:
    bool syncLoad(size_t i, cv::Mat& left, cv::Mat& right) {
        // Directory indexes for both images in KITTI dataset
        size_t fIdx1 = mIsColor ? 2 : 0,
               fIdx2 = mIsColor ? 3 : 1;

        return loadImg(i, fIdx1, left) & loadImg(i, fIdx2, right);
    }

    bool loadImg(size_t i, size_t fIdx, cv::Mat& imgDst) {
        char fileName[128] = {};
        sprintf(fileName, (mDatasetFolder + "image_%d/%06d.png").c_str(), fIdx, i);
        imgDst = cv::imread(fileName);

        if (mIsColor && imgDst.data)
            cv::cvtColor(imgDst, imgDst, cv::COLOR_BGR2GRAY);

        return imgDst.data;
    }

    size_t curr, end;
    std::atomic<bool> mIsFinish = false;
    ReaderWriterQueue<std::pair<cv::Mat, cv::Mat>> mQueue;
    std::thread mThread;
    const std::string mDatasetFolder;
    bool mIsColor;
};

#endif // ASYNCIMAGELOADER_H_
