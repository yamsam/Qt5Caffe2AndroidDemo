# Caffe2 Qt5 Android AI Demo

simple AI demo App for android written by Qt5 and Caffe2.

### Description

Caffe2 is a powerful deep learning framework and Qt is also great GUI toolkit.
Fortunately they both support mobile and are written by c++.
My idea is just simple, "If i can mix them, i could make a deep learning app for android
by writing c++ only !"

### Demo

### Usage
When you touch your display, this program will crop the center part of the display image and
give it to the neural network for classification and then show the result message on top of the display.

### Features
* Use Caffe2, deep learning framework to classify images 
* Use QtQuick Camera to capture a display image
* Use Squeeze Net, a light neural network for mobile, trained by 1000 class imagenet
* You can make your AI App by writing only c++

### Related Projects
This project is based on the following projects.
 * https://github.com/caffe2/AICamera
 * https://github.com/stephenquan/MyVideoFilterApp

### License
This project is licensed under the MIT License
