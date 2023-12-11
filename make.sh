export QMAKE_SYSROOT=/home/wangyichen/linux-sdk/rk3588_linux_sdk/buildroot/output/rockchip_rk3588/host/aarch64-buildroot-linux-gnu/sysroot

QMAKE=/home/wangyichen/linux-sdk/rk3588_linux_sdk/buildroot/output/rockchip_rk3588/host/bin/qmake

${QMAKE}
make -j32
