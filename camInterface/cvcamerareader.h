#ifndef CVCAMERAREADER_H
#define CVCAMERAREADER_H
#include<QMutex>
#include<QThread>
#include<QImage>
#include<QWaitCondition>
#include<QMessageBox>
#include<QDebug>
#include<QSettings>
#include<QFile>
#include<QApplication>
#include<QElapsedTimer>
#include<QDir>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/objdetect/objdetect.hpp>
#include<tisudshl.h>
#include<Grabber.h>
#ifdef _WIN32
#include <Windows.h>
#endif

class cameraReader: public QThread
{
	Q_OBJECT

private:
	bool stop;
	bool cameraInit;
	bool recording;
	QMutex mutex;
	QWaitCondition condition;
	cv::Mat frame;
	double frameRate;
	cv::VideoCapture captureDevice;
	cv::VideoWriter outputDevice;
	cv::Mat RGBFrame;
	QImage img;
	cv::CascadeClassifier faceCatch;
	//timer variables
	QElapsedTimer *recTimer = new QElapsedTimer();
	qint64 tEnd;
	std::vector<qint64> timeStamps;
	//methods
	bool initClassifier();
	void faceDetect(cv::Mat &input);
	void writeTimestamps();
	DShowLib::Grabber *m_pGrabber;
	DShowLib::FrameHandlerSink::tFHSPtr m_pSink;
	int nFrames,frameCounter;
	QString settingsFile;
	QString metaDataVideo;
	QString outVideoName;
	QString timeDataName;

signals:
	void processedImage(const QImage &image);
	void detectedFace(bool value);
	void recordCompleted();
public slots:
	void onRecordStarted(bool value);

protected:
	void run();
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
