

## Introduction ##

Sfera ha many tunable parameters and both the quality of the output and performance can be changed by editing the default configuration file stored in the **gamedata/cfgs/default.cfg** file.
Alternative configuration files can be used by running Sfera with the **-o** command line option followed by the name of the configuration file:

```
./bin/Sfera -o gamedata/cfgs/high.cfg
```

## Screen resolution ##

The screen size resolution has a huge impact on the performance. For instance, running at 1280x720 requires a 4 time faster hardware than running at 640x360 at the same frame rate. The screen size resolution can be changed by editing the following properties in the configuration file:

```
screen.width=640
screen.height=360
```

## Number of pass per frame ##

The number of samples per pixel is another important parameter, both for the quality of the output and the frame rate. A lower number of pass will increase the frame rate but it will also produce an output with more noise.

```
renderer.sampleperpass=2
```

## Renderer type ##

The renderer is responsible to produce the final output image given, as input, the geometrical description of the frame. The available renderer are:

| **Type** | **Description** |
|:---------|:----------------|
| SINGLE\_CPU | A single thread, CPU-only renderer written in C++. It is mostly used for testing |
| MULTI\_CPU | A multi-thread, CPU-only renderer written in C++. It can be useful on platforms where OpenCL is not available. |
| OPENCL   | An OpenCL renderer, works on single and multiple OpenCL CPU and GPU devices |

```
renderer.type=OPENCL
```

## Image filter ##

The image filter is used to splat samples over the image plane. Disabling the filter will produce an image with high frequency noise. Using a light filter will produce a more blurred output and it is usually more visual pleasant than high frequency noise.
The available type of filters are:

| **Type** | **Description** |
|:---------|:----------------|
| NO\_FILTER | Filter disabled |
| BLUR\_LIGHT | a light Gaussian filter |
| BLUR\_HEAVY | an heavy Gaussian filter |
| BOX      | a box filter    |

```
renderer.filter.type=BLUR_LIGHT
```

## Frame blending (alias ghost effect) ##

Every single frame is blended with the previous one in order to reduce the noise. Less the camera move and more heavy the blend is. This achieve a kind of "ghost effect", quite similar to a rude motion blur.
It is possible to tune how heavy this effect is with the following properties:

```
renderer.ghostfactor.cameraedit=0.75
renderer.ghostfactor.nocameraedit=0.1
renderer.ghostfactor.time=1.5
```

**renderer.ghostfactor.cameraedit** is the weight of the new frame in the case of a moving camera (i.e. 0.75 means the new frame weight for a 75%).<br />
**renderer.ghostfactor.nocameraedit** is the weight of the new frame in the case of a not moving camera (i.e. 0.1 means the new frame weight for a 10%).<br />
**renderer.ghostfactor.time** is the time (in seconds) for the transition from a "moving camera" state to a "not moving camera" state and vice versa.

## Multi-GPUs support ##

A description of Multi-GPUs support related properties is available [here](MultiGPUsConfiguration.md).