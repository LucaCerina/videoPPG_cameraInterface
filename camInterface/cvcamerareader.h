#ifndef CVCAMERAREADER_H
#define CVCAMERAREADER_H
#include<QMutex>
#include<QThread>
#include<QImage>
#include<QWaitCondition>
#include<QMessageBox>
#include<QDebug>
#include <QSettings>
#include<QFile>
#include<QApplication>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/objdetect/objdetect.hpp>
#include<tisudshl.h>
#include<Grabber.h>
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
    bool recording;
    QMutex mutex;
    QWaitCondition condition;
    Mat frame;
    double frameRate;
    VideoCapture captureDevice;
    VideoWriter outputDevice;
    Mat RGBFrame;
    QImage img;
    CascadeClassifier faceCatch;
    //methods
    bool initClassifier();
    void faceDetect(Mat &input);
    DShowLib::Grabber *m_pGrabber;
    DShowLib::FrameHandlerSink::tFHSPtr m_pSink;
    int nFrames,frameCounter;
    QString settingsFile;
    QString metaDataVideo;
    QString outVideoName;

signals:
    void processedImage(const QImage &image);
    void detectedFace(bool value);
    void recordCompleted();
public slots:
    void onRecordStarted(bool value);

protected:
    void run();
    void msleep(int ms);
public:
    //Constructor
    cameraReader(QObject *parent=0);
    //Destructor
    ~cameraReader();
    //Load camera
    bool initCamera(int device,QString outName);
    bool checkCamera(int device);
    //methods
    void Play();
    void Stop();
    bool isStopped() const;
    bool isInitialized() const;
};

#endif // CVCAMERAREADER_H
