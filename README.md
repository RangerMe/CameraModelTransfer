# CameraModelTransfer
Provide common methods for camera model transfer.
Its function includes:
* Transfer camera intrinsic models between following models:
    * Universal 
    * Kannala Brandt model 
    * Mei (To be done)
* Show two model's disortion curve (To be done)
## Camera Models
### Universal
The universal camera model uses following parameters and a disortion curve to describe a camera:
```
_W          image width
_H          image height
_CU         optic center
_CV         optic center
_C          skew parameter, c
_D          skew parameter, d
_E          skew parameter, e
_FOV_AT_CU  filed of vision, at optic center, in "u" direction
_FOV_AT_CV  filed of vision, at optic center, in "v" direction
```
The **disortion curve** is a relationship between light ray's angle of incidence and the radius of light ray's landing point on chip. This disortion curve is usually obtained by calibration, and following camera models' aim is to simulate this disortion curve.
### Kannala Brandt
About the Kannala Brandt camera model, please see the paper:
```
Juho Kannala and Sami S. Brandt,
A Generic Camera Model and Calibration Method for Conventional, Wide-Angle, and Fish-Eye Lenses, in IEEE Transactions on Pattern Analysis and Machine Intelligence, vol. 28, no. 8, pp. 1335-1340, Aug. 2006.
```
The Kannala Brandt camera model uses following parameters to describe the camera:
```
_W          image width
_H          image height
_CU         optic center
_CV         optic center
_MU         length per pixel, on chip, in "u" direction
_MV         length per pixel, on chip, in "v" direction
_K1,_K2...  disortion parameters, K1 is fixed to 1
```
### Mei
To be done

## Build this project
### Dependencies
Currently does not have any dependencies.
### Build
```
cd CamTransfer
mkdir build
cd build
cmake ..
make
```
A executable file can be found in directory ***CamTransfer/build/***.
## Usage
Before using this project, you need to have a config file and a original camera model file.
### Config file
In the config file, you shall write down following terms:
```
_help                 print help message or not,
                      could be [tree] or [false]
_path_to_ori_model    required, path to model to
                      be transfered
_path_to_save_model   optional, path to save 
                      calculated model
_target_model_type    target model type, could be:
                      1. UNIVERSAL
                      2. KANNALA_BRANDT
_show_offset          show disortion curve offset
                      or not,could be [tree] or 
                      [false]
_curve_size           Curve size, could be [NULL] 
                      or [a number], by default, 
                      it is 1001
_curve_step           Curve step(angle), coule be 
                      [NULL] or [a number], by 
                      default, it is 0.1 degree
```
An example file looks like:
```
_help = false
_path_to_ori_model = universal.txt
_path_to_save_model = NULL
_target_model_type = KANNALA_BRANDT
_show_offset = false
_curve_size = NULL
_curve_step = NULL
```
When you set one term to "NULL", this term will be set as default value. 
**Note that the config term name and the value shall be divided by "=" with two spaces on double side. The spaces are necessary, do not elimiate them.**
### Camera Model File - Universal
A typical "Universal" camera model file shall include all parameters described above. It should look like:
```
_TYPE = UNIVERSAL      # parameter type
_W = ***
_H = ***
_CU = ***
_CV = ***
_C = ***
_D = ***
_E = ***
_FOV_AT_CU = ***
_FOV_AT_CV = ***
_DISORT_STEP = 0.1     # disortion angle step, 
                         by default 0.1 degree
_DISORT_SIZE = 1001	   # disortion curve size, 
                         by default 1001
_DISORT	=			   # disortion curve, 
                         first column is angle, in degree, 
                         second column is radius, in mm
0.000000 ***
0.100000 ***
0.200000 ***
0.300000 ***
...
```
The first term "_TYPE" shall indicate correct camera model type.
### Camera Model File - Kannala Brandt
A typical "Kannala Brandt" camera model file shall include all parameters described above. It should look like:
```
_TYPE = KANNALA_BRANDT
_W = ***
_H = ***
_K1 = 1.000000
_K2 = ***
_K3 = ***
_K4 = ***
_K5 = ***
_CU = ***
_CV = ***
_MU = ***
_MV = ***
```
The first term "_TYPE" shall indicate correct camera model type.
### Camera Model File - Mei
To be done
### Use the project
```
Usage:  ./CamTransfer [Path to config file]
```

