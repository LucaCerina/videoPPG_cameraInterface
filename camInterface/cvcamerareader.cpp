#include<cvcamerareader.h>

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

bool cameraReader::initCamera(int device)
{
    //load camera
    captureDevice.open(device);
    if(captureDevice.isOpened())
    {
        frameRate = (int) captureDevice.get(CV_CAP_PROP_FPS);
        //BRUTTO DA SISTEMARE
        if(frameRate == 0)
        {
            DShowLib::Grabber* m_pGrabber = new DShowLib::Grabber();
            m_pGrabber->openDevByDisplayName("DFK 23UM021");
            frameRate = m_pGrabber->getFPS();
            qDebug() << frameRate;
        }
        return true;
    }
    else
        return false;
}

bool cameraReader::checkCamera(int device)
{
    //load camera
    captureDevice.open(device);

    if(captureDevice.isOpened())
    {
        cameraInit = true;
        return true;
    }
    else
        return false;
    captureDevice.release();
}

void cameraReader::Play()
{
    if(!isRunning())
    {
        if(isStopped())
            stop = false;
        qDebug() << "stop is " << stop;
        start(LowPriority);
    }
}

void cameraReader::Stop()
{
    stop = true;
}

void cameraReader::run()
{
    int delay = 1000/frameRate;
    qDebug() << "delay set";
    while(!stop)
    {
        qDebug() << "try to read a frame";
        if(!captureDevice.read(frame))
            stop = true;
        faceDetect(frame);
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
        emit processedImage(img);
        this->msleep(delay);
    }
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
