# Image Processing USF Spring 2017 HW2
## Processes:
1.histcv X Y Sx Sy
does histogram equalization on the image. works on both color and gray scale.

2.sobelgs T X Y Sx Sy
*For gradient image, set T = -1
runs sobel edge detector, gets x and y gradient, then finds the sqrt(dx*dx + dy*dy).

3.cannygs T X Y Sx Sy
runs canny edge detector.

## Input File Format:
input/file output/file #Processes ProcessName Parameters

## Install OpenCV
```
# Ubuntu
sudo apt install libopencv-dev
```

## Compile and Run:
Output images written in output-images/ directory after execution
```
git clone https://github.com/sawyermade/image_processing.git
cd image_processing/hw4
make
./hw4 parameters.txt
```

## Parameters file
```
cat parameters.txt
```

