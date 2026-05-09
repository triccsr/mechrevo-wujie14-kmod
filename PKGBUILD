# Maintainer: local

pkgname=wujie14-km-dkms
_dkms_name=wujie14-km
pkgver=1.1
pkgrel=1
pkgdesc="Mechrevo WUJIE14 performance mode and keyboard backlight DKMS module"
arch=('x86_64')
url="https://github.com/xuwd1/mechrevo-wujie14-kmod"
license=('GPL-3.0-or-later')
depends=('dkms')
optdepends=(
    'linux-headers: build the module for the stock Arch kernel'
    'linux-lts-headers: build the module for the LTS kernel'
    'power-profiles-daemon: KDE/desktop power profile integration'
)
install="${pkgname}.install"
source=()
sha256sums=()

package() {
    local src_root="${startdir}"
    local dkms_dir="${pkgdir}/usr/src/${_dkms_name}-${pkgver}"

    install -Dm644 "${src_root}/LICENSE" "${pkgdir}/usr/share/licenses/${pkgname}/LICENSE"

    install -dm755 "${dkms_dir}"
    install -m644 \
        "${src_root}/Kbuild" \
        "${src_root}/dkms.conf" \
        "${src_root}/wujie14-km.c" \
        "${src_root}/wujie14-km.h" \
        "${src_root}/wujie14-perfmode.c" \
        "${src_root}/wujie14-perfmode.h" \
        "${src_root}/wujie14-wmi-event.c" \
        "${src_root}/wujie14-wmi-event.h" \
        "${src_root}/wujie14-kb.c" \
        "${src_root}/wujie14-kb.h" \
        "${dkms_dir}/"
}
