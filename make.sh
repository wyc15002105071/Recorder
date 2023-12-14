#SDK_DIR=/home/wangyichen/linux-sdk/rk3588_linux_sdk
SDK_DIR=/home2/shinengqiang/RK3588_EVB
export QMAKE_SYSROOT=${SDK_DIR}/buildroot/output/rockchip_rk3588/host/aarch64-buildroot-linux-gnu/sysroot

QMAKE=${SDK_DIR}/buildroot/output/rockchip_rk3588/host/bin/qmake

${QMAKE}
make -j32
