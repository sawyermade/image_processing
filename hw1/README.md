# Image Processing USF Spring 2017 HW1
## Processes:
bin = grayscale binarization
binc = color binarization
1Dsmooth = 1D smoothing **this is faster
2Dsmooth = 2D smoothing

## Parameters for Processes:
bin T X Y Sx Sy
binc TC Cr Cg Cb X Y Sx Sy
1Dsmooth WS X Y Sx Sy
2Dsmooth WS X Y Sx Sy
*WS is odd number that will be squared

## Input File Format:
input/file output/file #Processes ProcessName Parameters

## Compile and Run:
Output images written in output-images/ directory after execution
```
git clone https://github.com/sawyermade/image_processing.git 
cd image_processing/hw1
make
./hw1 parameters.txt
```

## Parameters file
```
cat parameters.txt
```

