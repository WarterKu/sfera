## Introduction ##

Sfera is capable to use multiple OpenCL platforms and multiple OpenCL devices available. By default, Sfera will use all (and only) the GPU devices available. You may want select a sub-set of all available devices or use also no-GPU devices (i.e. CPU). This can be achieved by editing the [Configuration file](ConfigurationFile.md).

## Using no-GPU devices ##

By default, Sfera will select only GPU devices. It is possible to use any kind of OpenCL device by setting the following property:

```
opencl.devices.useonlygpus=false
```

## Selecting a subset of devices ##

By default, Sfera will use all OpenCL devices available. You can select a sub-set by setting the following property:

```
opencl.devices.select=01
```

**opencl.devices.select** must have as many digit as devices available on your platform. If a digit is **1**, the device with the corresponding index will be selected otherwise it will be skipped.
For instance if your platform has an OpenCL CPU device (listed as #1) and 2 x OpenCL (listed as #2 and #3), you can select the first only the first GPU with:

```
opencl.devices.select=010
```

## Workload tuning ##

By default, Sfera will assign the same amount of work (i.e. [number of pass per frame](ConfigurationFile#Number_of_pass_per_frame.md)) to all selected devices. In the case the devices don't offer the same performances, you have to hand tune the amount of work assigned to each device.
For instance, if you have 3 GPUs, you can hand tune the amount of pass assigned to each GPU with the following properties:

```
##################################
# Multi-GPU
##################################
opencl.devices.useonlygpus=false
# Intel CPU device is #1, AMD CPU #2, HD5870 #3, HD5870 #4, HD5850 #5
opencl.devices.select=01110
# HD5870 #1
opencl.devices.0.sampleperpass=4
# HD5870 #2
opencl.devices.1.sampleperpass=4
# HD5850 #1
opencl.devices.2.sampleperpass=3
```