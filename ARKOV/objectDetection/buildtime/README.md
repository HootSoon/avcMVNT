**Build Dependencies and Toolchain**
 - CUDA 12.0
 - cudnn 9.0
 - TensorRT 10.13

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

**TensorRT**
```
wget https://developer.download.nvidia.com/compute/tensorrt/10.13.0/local_installers/nv-tensorrt-local-repo-ubuntu2004-10.13.0-cuda-11.8_1.0-1_amd64.deb
sudo dpkg -i nv-tensorrt-local-repo-ubuntu2004-10.13.0-cuda-11.8_1.0-1_amd64.deb
sudo cp /var/nv-tensorrt-local-repo-ubuntu2004-10.13.0-cuda-11.8/nv-tensorrt-*-keyring.gpg /usr/share/keyrings
sudo apt-get update
sudo apt-get -y install tensorrt
```
