# cameraInterface
QT graphical interface used to interact with the Imaging Source DFK23 camera and to record patient details for the AFCam project

## Dependencies
- QT Creator (tested on v5.7) with compiler to build the project
- OpenCV (tested on v3.1) library
- Imaging Source (IC) Imaging Control v3.4 library (it will also install a tool to setup the camera)
The caminterface.pro file should be modified to target the correct path on the build pc

## How to use the system
- Connect the camera to the PC via USB3.0 cable
- Open the camera setup tool ans set binning to 2x
- TODO add the rest of the process

## Original contributors
* Luca Cerina, ResMed, formerly Politecnico di Milano
* Professor Luca Mainardi, Politecnico di Milano
* Valentina D.A. Corino PhD, Politecnico di Milano
* Professor Riccardo Barbieri, Politecnico di Milano
