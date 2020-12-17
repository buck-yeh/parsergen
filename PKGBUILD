# This is an example PKGBUILD file. Use this as a start to creating your own,
# and remove these comments. For more information, see 'man PKGBUILD'.
# NOTE: Please fill out the license field for your package! If it is unknown,
# then please put 'unknown'.

# Maintainer: Your Name <youremail@domain.com>
pkgname=parsergen
pkgver=1.4.1
pkgrel=1
epoch=
pkgdesc='LR1/GLR parser generator into Modern C++ code which must be built with bux library'
arch=('x86_64')
url='https://github.com/buck-yeh/parsergen.git'
license=('MIT')
groups=()
depends=()
makedepends=('cmake' 'make' 'gcc' 'git')
checkdepends=()
optdepends=()
provides=()
conflicts=()
replaces=()
backup=()
options=()
install=
changelog=
source=() #"$pkgname-$pkgver.tar.gz")
noextract=()
md5sums=()
validpgpkeys=()

prepare() {
	rm -Rf "$pkgname-$pkgver"
	mkdir -p "$pkgname-$pkgver"
	cd "$pkgname-$pkgver"
	git clone -b main --single-branch $url .
}

build() {
	cd "$pkgname-$pkgver"
	rm -Rf CMakeCache.txt CMakeFiles/
	cmake .
	make -j
}

check() {
	echo "Enter check()"
	cd "$pkgname-$pkgver"
	#make -k check
}

package() {
	echo "Enter package()"
	cd "$pkgname-$pkgver"
	#make DESTDIR="$pkgdir/" install
}
