## STUDENTBENCH BUILD INSTRUCTIONS
This build routine will provide a nearly identical environment to the one found installed on ARKOV's Jetson.

Following this guide on your desired machine (Student Bench in this example) may expedite the development processes of ARKOV's codebase.

*Note:* While this guide will attempt to remain general, it is based on Student Bench and will thus be a linux (`bash`) oriented guide. macOS (`zsh`) users should find the provided commands compatible; Windows (`Powershell`) users may either adapt commands as needed or install Windows Subsystem for Linux (WSL) and follow as normal.

**Contents**

- [Download and Install](#download-and-install)

- [Using SDK Manager](#using-sdk-manager)

- WIP

# Prerequisites
Install the following programs based on your platform:

- Docker

- WSL (optional)

# Download and Install
 - Download the [NVIDIA SDK Manager](https://developer.download.nvidia.com/sdkmanager/redirects/sdkmanager-docker-image-ubuntu2004.html) docker image

 - From a terminal, load the docker image

*bash*

```
docker load -i ./sdkmanager-26.1.0.14392-Ubuntu_20.04_docker.tar.gz
```

 - Optionally tag the image for convenience

*bash*

```
docker tag sdkmanager:26.1.0.14392-Ubuntu_20.04 sdkmanager:latest
```

 - Run the image to launch SDK Manager

*bash*

```
docker run -it --rm sdkmanager --cli
```

# Using SDK Manager
- Login as NVIDIA Developer (*requires account creation*)

- Install Options:

    - Download Only

    - Jetson

    - Target Hardware

    - Jetson AGX Orin modules

    - Linux

    - y (show all release versions)

        - \[TODO: CONFIRM JETPACK VERSION]
