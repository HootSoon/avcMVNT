**AI Lab Student Debian 13 Desktop (x86_64)**\
A desktop testbench was provided for development

Enter the ubuntu 20.04 development environment via the terminal command:
```
distrobox
```

**Build Dependencies and Toolchain (ARM64)**
 - CUDA 12.0
 - cudnn 9.0
 - TensorRT 8.5

**CUDA**
```
wget https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2004/arm64/cuda-ubuntu2004.pin
sudo mv cuda-ubuntu2004.pin /etc/apt/preferences.d/cuda-repository-pin-600
wget https://developer.download.nvidia.com/compute/cuda/12.0.0/local_installers/cuda-tegra-repo-ubuntu2004-12-0-local_12.0.0-1_arm64.deb
sudo dpkg -i cuda-tegra-repo-ubuntu2004-12-0-local_12.0.0-1_arm64.deb
sudo cp /var/cuda-tegra-repo-ubuntu2004-12-0-local/cuda-*-keyring.gpg /usr/share/keyrings/
sudo apt-get update
sudo apt-get -y install cuda
```

**cudnn**
```
wget https://developer.download.nvidia.com/compute/cudnn/9.0.0/local_installers/cudnn-local-tegra-repo-ubuntu2004-9.0.0_1.0-1_arm64.deb
sudo dpkg -i cudnn-local-tegra-repo-ubuntu2004-9.0.0_1.0-1_arm64.deb
sudo cp /var/cudnn-local-tegra-repo-ubuntu2004-9.0.0/cudnn-*-keyring.gpg /usr/share/keyrings/
sudo apt-get update
sudo apt-get -y install cudnn
```
