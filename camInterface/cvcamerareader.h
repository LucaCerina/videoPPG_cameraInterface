#ifndef CVCAMERAREADER_H
#define CVCAMERAREADER_H
#include<QMutex>
#include<QThread>
#include<QImage>
#include<QWaitCondition>
#include<QMessageBox>
#include<QDebug>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/objdetect/objdetect.hpp>
#include<tisudshl.h>
#ifdef _WIN32
#include <Windows.h>
#endif


using namespace cv;

class cameraReader: public QThread
{
    Q_OBJECT

private:
    bool stop;
    bool cameraInit;
    QMutex mutex;
    QWaitCondition condition;
    Mat frame;
    int frameRate;
    VideoCapture captureDevice;
    Mat RGBFrame;
    QImage img;
    CascadeClassifier faceCatch;
    //methods
    bool initClassifier();
    void faceDetect(Mat &input);

signals:
    void processedImage(const QImage &image);
    void detectedFace(bool value);
protected:
    void run();
    void msleep(int ms);
public:
    //Constructor
    cameraReader(QObject *parent=0);
    //Destructor
    ~cameraReader();
    //Load camera
    bool initCamera(int device);
    bool checkCamera(int device);
    //methods
    void Play();
    void Stop();
    bool isStopped() const;
    bool isInitialized() const;


};

#endif // CVCAMERAREADER_H
