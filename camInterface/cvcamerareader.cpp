#include<cvcamerareader.h>

using namespace DShowLib;

cameraReader::cameraReader(QObject *parent)
    :QThread(parent)
{
    stop = true;
    cameraInit = false;
    initClassifier();
}

cameraReader::~cameraReader()
{
    mutex.lock();
    stop = true;
    captureDevice.release();
    condition.wakeOne();
    mutex.unlock();
    wait();
}

bool cameraReader::initClassifier()
{
    if(!faceCatch.load("C:\\Users\\Bio-tec\\Documents\\opencv\\sources\\data\\haarcascades\\haarcascade_frontalface_alt.xml"))
    {
       QMessageBox msgBox;
       msgBox.setText("Cannot load face detector.");
       msgBox.exec();
    }
    return true;
}

bool cameraReader::initCamera(int device,QString outName)
{
    recording = false;
    frameRate = 0;
    //load camera
    QString settingsFile = QApplication::applicationDirPath()+"/cameraSettings.ini";
    QSettings cameraSetting(settingsFile,QSettings::IniFormat);
    if(QFile(settingsFile).exists())
    {
        //captureDevice.open(device);
        m_pGrabber = new Grabber();
        m_pGrabber->openDevByDisplayName(cameraSetting.value("camIndex").toString().toStdWString());
        m_pGrabber->setVideoFormat(cameraSetting.value("camFormat").toString().toStdWString());
        m_pGrabber->closeDev();
        captureDevice.open(device);
        frameRate = cameraSetting.value("camFPS").toDouble();
        if(captureDevice.set(CV_CAP_PROP_FPS,frameRate))
            qDebug() << "setup ok " << captureDevice.get(CV_CAP_PROP_FPS);
        //m_pGrabber->setFPS(cameraSetting.value("camFPS").toDouble());
        //frameRate = m_pGrabber->getFPS();
        //load video
        qDebug() << "file " << outName;
        if(!outputDevice.open(outName.toStdString(), CV_FOURCC('H','F','Y','U'), frameRate, Size(640, 480)))
            qDebug() << "failed to open video";
        //set number of frames
        nFrames = cameraSetting.value("videoDuration").toInt()*(int)frameRate;
        return true;
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("You have to setup camera for the first time");
        msgBox.exec();
        return false;
    }
}

bool cameraReader::checkCamera(int device)
{
    //load camera
    captureDevice.open(device);

    if(captureDevice.isOpened())
    {
        cameraInit = true;
        captureDevice.release();
        return true;
    }
    else
        return false;
}

void cameraReader::Play()
{
    if(!isRunning())
    {
        if(isStopped())
            stop = false;
        start(LowPriority);
    }
}

void cameraReader::Stop()
{
    stop = true;
}

void cameraReader::run()
{
    //int delay = 1000/frameRate;
    //qDebug() << "delay set";
    frameCounter = 0;
    qDebug() << "open " << captureDevice.isOpened();
    while(!stop && frameCounter<nFrames)
    {
        if(!captureDevice.read(frame))
            stop = true;
        if(frame.channels()==3)
        {
            cv::cvtColor(frame,RGBFrame,COLOR_BGR2RGB);
            img = QImage((const unsigned char*)(RGBFrame.data),
                         RGBFrame.cols,RGBFrame.rows,QImage::Format_RGB888);

        }
        else
        {
            img = QImage((const unsigned char*)(frame.data),
                         frame.cols,frame.rows,QImage::Format_Indexed8);
        }
        if(recording)
        {
            outputDevice << frame;
            ++frameCounter;
        }
        else
        {
            faceDetect(frame);
        }
        emit processedImage(img);
        //this->msleep(delay);
    }
    emit(recordCompleted());
    stop = true;
    outputDevice.release();
    recording = false;
}

void cameraReader::faceDetect(Mat &input)
{
    std::vector<Rect> faces;
    faceCatch.detectMultiScale(input,faces,1.2,3,CASCADE_FIND_BIGGEST_OBJECT|CASCADE_SCALE_IMAGE,Size(30,30));
    if(faces.size()==1)
        emit detectedFace(true);
    else
        emit detectedFace(false);
}

void cameraReader::msleep(int ms)
{
    //struct timespec ts = {ms / 1000, (ms % 1000)*1000*1000};
    //nanosleep(&ts, NULL);
    Sleep(ms);
}

bool cameraReader::isStopped() const
{
    return this->stop;
}

bool cameraReader::isInitialized() const
{
    return this->cameraInit;
}

void cameraReader::onRecordStarted(bool value)
{
    recording = value;
}
