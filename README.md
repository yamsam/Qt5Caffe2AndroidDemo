# Caffe2 Qt5 Android AI Demo

simple AI demo App for android written by Qt5 and Caffe2.

### Description

Caffe2 is a powerful deep learning framework and Qt is also great GUI toolkit.
Fortunately they both support mobile and are written by c++.
My idea is just simple, "If I can mix them, I could make a deep learning app for android
by writing c++ only !"

### Demo
![result](https://github.com/yamsam/Qt5Caffe2AndroidDemo/blob/media/demo.gif)

### Usage
When you touch your display, this program will crop the center part of the display image and
give it to the neural network for classification and then show the result message on top of the display.

### Features
* Use Caffe2, deep learning framework to classify images 
* Use QtQuick Camera and QAbstractVideoFilter to capture a display image
* Use Squeeze Net, a light neural network for mobile, trained by 1000 class imagenet
* You can make your AI App by writing only c++

### Build
1) Prepare Qt5(android_armv7) and Qt Creator 
   - tested with with Qt 5.10.1, qt creatror 4.4.1 on windows 10 and ubuntu 16.04
2) Build Caffe2 android library
   - latest caffe2 makes large libraries now, its over 600MB, so I can't upload them into GitHub. You can build your own caffe2 libraries or use my build binary here (https://drive.google.com/drive/folders/1dqYYQkEszpO6efFXN4blUEaRR8BgDTYt?usp=sharing)  
   - put them into caffe2lib directory under this project directory
3) Compile with Qt5 android_armv7 kit and Deploy it into your android

### Related Projects
This project is based on the following projects.
 * https://github.com/caffe2/AICamera
 * https://github.com/stephenquan/MyVideoFilterApp

### License
This project is licensed under the MIT License
