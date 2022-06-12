# IMAGINE - Image Filters 
 
    Usage: imagine [IMG_IN] [IMG_OUT] [OPTION...] 

## Options: 
    [-h | --help] [COMMAND] 
Show command info. 

    [-f | --filter] [FILTER_NAME] [N_THREADS] [ARG...]
Applies the defined filter to the input image.  You can apply this command many times and the filters will be executed in the requested order.

    [-s | --single-thread]
Run all desired filters in a single thread.

    [-m | --multi-thread] [N_THREADS]

Run all desired filters in a certain number of threads.

    [-b | --batch]
Apply filters to a batch of images in a folder (IMG_IN) andsave them in another directory (IMG_OUT).


## Filters:
    plain [N_THREADS] [HUE]

    blackwhite [N_THREADS]

    contrast [N_THREADS] [LEVEL]

    bright [N_THREADS] [BRIGHT]

    shades [N_THREADS] [SCALE]

    merge [N_THREADS] [IMG_2] [ALPHA]

    frame [N_THREADS] [WIDTH] [HUE]

    blur [N_THREADS] [LEVEL]

    zoom [N_THREADS] [LEVEL]

    edge [N_THREADS]

## Examples: 
    imagine /imgs/motor.ppm /images/output.ppm -m 6 -f contrast 50 -f frame 20 200 
       
Will apply the contrast and frame filters in that order in 6 threads. 

    imagine ./imgs/totoro.ppm /images/output.ppm -f zoom 6 4 -f blackwhite 2 -f bright 1 0.5 
Will apply zoom with 6 threads, blackwhite with 2 and bright with one in that order. 

    imagine ./imgs/ /images/out/ -b -m 2 -f zoom 4 -f blur 10
Will apply zoom and blur to all the images in the './imgs' directory with 2 threads. 