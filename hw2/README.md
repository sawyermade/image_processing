# Image Processing USF Spring 2017 HW2
## Processes:
1.sca a b c d X Y Sx Sy
stretches all R, G, and B values from a,b to c,d in ROI. 

2.scr a b c d X Y Sx Sy
stretches R value from a,b to c,d in ROI.

3.scg a b c d X Y Sx Sy
stretches G value from a,b to c,d in ROI.

4.scb a b c d X Y Sx Sy
stretches B value from a,b to c,d in ROI.

5.si a b c d X Y Sx Sy
stretches I value from a,b to c,d in ROI.

6.ss a b c d X Y Sx Sy
stretches S value from a,b to c,d in ROI.

7.sh a b c d X Y Sx Sy
stretches H value from a,b to c,d in ROI. ***a,b,c,d values for H are [0,360] instead of [0,255]

8.sgs a b c d X Y Sx Sy
stretches Gray Scale image from a,b to c,d in ROI.

## Input File Format:
input/file output/file #Processes ProcessName Parameters

## Compile and Run:
Output images written in output-images/ directory after execution
```
git clone https://github.com/sawyermade/image_processing.git
cd image_processing/hw2
make
./hw2 parameters.txt
```

## Parameters file
```
cat parameters.txt
```

