# Image Processing USF Spring 2017 HW2
## Processes:
T = Threshold [0,255], D = degree [-90,90], C = color: r,g,b = 0,1,2

FOR GRADIENT IMAGE: set T = -1 and D = -99

FOR BINARIZED EDGES: set T = [0,255] and D = -99

FOR BINARIZED DIRECTIONAL EDGES: set T = [0,255] and D = [-90,90]

ALL EDGE DETECTION DONE USING PREWITT 3x3 WEIGHTS UNLESS OTHERWISE STATED

1.edgs T D X Y Sx Sy
does edge detection on grayscale image.

2.edc T D C X Y Sx Sy
does edge detection on specific color R,G,or B. Set C = [0,2]: R,G,B = 0,1,2.

3.edrgb T D X Y Sx Sy
does edge detection using set addition with R,G,and B for binarization. Bool = (R or G or B).

4.edi T D X Y Sx Sy
does edge detection on I.

5.edh T D X Y Sx Sy
does edge detection on H.

6.eds T D X Y Sx Sy
does edge detection on S.

7.edgs5 T D X Y Sx Sy
does edge detection on grayscale image using prewitt5x5.

## Input File Format:
input/file output/file #Processes ProcessName Parameters

## Compile and Run:
Output images written in output-images/ directory after execution
```
git clone https://github.com/sawyermade/image_processing.git
cd image_processing/hw3
make
./hw3 parameters.txt
```

## Parameters file
```
cat parameters.txt
```

