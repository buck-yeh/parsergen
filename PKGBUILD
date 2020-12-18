# This is an example PKGBUILD file. Use this as a start to creating your own,
# and remove these comments. For more information, see 'man PKGBUILD'.
# NOTE: Please fill out the license field for your package! If it is unknown,
# then please put 'unknown'.

# Maintainer: Your Name <youremail@domain.com>
pkgname=parsergen
pkgver=1.5.0
pkgrel=1
epoch=
pkgdesc='LR1/GLR parser generator into Modern C++ code which must be built with bux library'
arch=('x86_64')
url='https://github.com/buck-yeh/parsergen.git'
license=('MIT')
groups=()
depends=()
makedepends=('cmake' 'make' 'gcc' 'git' 'binutils' 'fmt')
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
	rm -Rf "$pkgname"
	mkdir -p "$pkgname"
	cd "$pkgname" || return 1
	git clone -b main --single-branch $url .
}

build() {
	cd "$pkgname" || return 1
	rm -Rf CMakeCache.txt CMakeFiles/
	cmake .
	make -j
}

check() {
	cd "$pkgname" || return 1
	if [[ -x ParserGen/parsergen && -x ParserGen/grammarstrip && -x ScannerGen/scannergen && -f ScannerGen/RE_Suite.txt ]]; then
		return 0
	else
		return 1
	fi
}

package() {
	cd "$pkgname" || return 1
	cp ParserGen/parsergen ParserGen/grammarstrip ScannerGen/scannergen ScannerGen/RE_Suite.txt "$pkgdir/"
}
