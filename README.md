# videoPPG: cameraInterface
QT graphical interface used to interact with the Imaging Source DFK23 camera and to record patient details for the AFCam project by `Corino et al., Computing in Cardiology (CinC), 2017`. From `Cerina et al., Influence of acquisition frame-rate and video compression techniques on pulse-rate variability estimation from vPPG signal, 2019`

## Dependencies
- QT Creator (tested on v5.7) with compiler to build the project
- OpenCV (tested on v3.1) library
- Imaging Source (IC) Imaging Control v3.4 library (it will also install a tool to setup the camera)
The caminterface.pro file should be modified to target the correct path on the build pc
Right now cameraInterface works only on Microsoft Windows due to Imaging Source libraries!

## How to use the system
- Connect the camera to the PC via USB3.0 cable
- Open the camera setup tool ans set binning to 2x
- TODO add the rest of the process

## Original contributors
* Luca Cerina, formerly Politecnico di Milano
* Professor Luca Mainardi, Politecnico di Milano
* Professor Riccardo Barbieri, Politecnico di Milano
* Luca Iozzia PhD, TeiaCare, formerly Politecnico di Milano
* Valentina D.A. Corino PhD, Politecnico di Milano

## Project repositories
* Conversion of video to raw RGB signal: [video2raw](https://github.com/LucaCerina/videoPPG_video2raw)
* Processing of raw RGB signal to Heart Rate Variability [raw2hrv](https://github.com/LucaCerina/videoPPG_raw2hrv)

## To cite this work
If you use this project in a scientific paper, please cite:

```
@article{cerina2019influence,
  title={Influence of acquisition frame-rate and video compression techniques on pulse-rate variability estimation from vPPG signal},
  author={Cerina, Luca and Iozzia, Luca and Mainardi, Luca},
  journal={Biomedical Engineering/Biomedizinische Technik},
  volume={64},
  number={1},
  pages={53--65},
  year={2019},
  publisher={De Gruyter}
}
```
