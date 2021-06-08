# Scala

[<img src="https://scalaproject.io/img/card.jpg">](https://scalaproject.io)

Scala is a privacy-focused cryptocurrency project linking distributed technologies to build a globally scalable and mobile-friendly ecosystem. We provide an opaque blockchain, untraceable payments, unlinkable transactions, and blockchain-analysis resistance.

Our vision is to distribute wealth for everyone and every device. We want to be the people’s coin. To achieve this goal, we focus on implementing solutions that are mobile-friendly and energy-efficient. There are about 4 billion mobile devices in the world, and we want to make sure that our ecosystem takes that into account. We also take a lot of time to improve the user experience so that our applications can be used easily by anyone.

Obviously, our goal is also to bring useful utilities and to solve real-world problems, in a way that is economically viable for the project and to sustain our growth. We take advantage of next-gen technologies such as IPFS, WebTorrent and custom software to provide the utility of data storage and we use the proven CryptoNote protocol for providing the financial aspects.

- Website: [scalaproject.io](https://scalaproject.io)
- Mail: [hello@scalaproject.io](mailto:hello@scalaproject.io)
- Discord: [chat.scalaproject.io](http://chat.scalaproject.io)

## Table of Contents

-  [Announcements](#announcements)
-  [Compiling Scala from source](#compiling-scala-from-source)
-  [More information](#more-information)
-  [Licenses](#licenses)

## Announcements

- You can follow our [twitter](https://twitter.com/scalahq) to get all the new information regarding the project.

## Compiling Scala from source

### Notes

We no longer support 32-bit platforms, we use go-ipfs extensively as part of our IPFS implementation and they no longer support 32-bit, hence we had to remove it.

### Dependencies

The following table summarizes the tools and libraries required to build. A few of the libraries are also included in this repository (marked as
"Vendored"). By default, the build uses the library installed on the system, and ignores the vendored sources. However, if no library is found installed on the system, then the vendored source will be built and used. The vendored sources are also used for statically-linked builds because distribution packages often include only shared library binaries (`.so`) but not static library archives (`.a`).

| Dep         | Min. version  | Debian/Ubuntu pkg    | Arch pkg     | Void pkg          | Fedora pkg          |
| ----------- | ------------- | -------------------- | ------------ | ----------------- | ------------------- |
| GCC         | 4.7.3         | `build-essential`    | `base-devel` | `base-devel`      | `gcc`               |
| CMake       | 3.5           | `cmake`              | `cmake`      | `cmake`           | `cmake`             |
| pkg-config  | any           | `pkg-config`         | `base-devel` | `base-devel`      | `pkgconf`           |
| Boost       | 1.58          | `libboost-all-dev`   | `boost`      | `boost-devel`     | `boost-devel`       |
| OpenSSL     | basically any | `libssl-dev`         | `openssl`    | `libressl-devel`  | `openssl-devel`     |
| libzmq      | 3.0.0         | `libzmq3-dev`        | `zeromq`     | `zeromq-devel`    | `zeromq-devel`      |
| OpenPGM     | ?             | `libpgm-dev`         | `libpgm`     |                   | `openpgm-devel`     |
| libnorm[2]  | ?             | `libnorm-dev`        |              |                   |                     |
| libunbound  | 1.4.16        | `libunbound-dev`     | `unbound`    | `unbound-devel`   | `unbound-devel`     |
| libsodium   | ?             | `libsodium-dev`      | `libsodium`  | `libsodium-devel` | `libsodium-devel`   |
| libunwind   | any           | `libunwind8-dev`     | `libunwind`  | `libunwind-devel` | `libunwind-devel`   |
| liblzma     | any           | `liblzma-dev`        | `xz`         | `liblzma-devel`   | `xz-devel`          |
| libreadline | 6.3.0         | `libreadline6-dev`   | `readline`   | `readline-devel`  | `readline-devel`    |
| ldns        | 1.6.17        | `libldns-dev`        | `ldns`       | `libldns-devel`   | `ldns-devel`        |
| expat       | 1.1           | `libexpat1-dev`      | `expat`      | `expat-devel`     | `expat-devel`       |
| GTest       | 1.5           | `libgtest-dev`[1]    | `gtest`      | `gtest-devel`     | `gtest-devel`       |
| Doxygen     | any           | `doxygen`            | `doxygen`    | `doxygen`         | `doxygen`           |
| Graphviz    | any           | `graphviz`           | `graphviz`   | `graphviz`        | `graphviz`          |
| lrelease    | ?             | `qttools5-dev-tools` | `qt5-tools`  | `qt5-tools`       | `qt5-linguist`      |
| libhidapi   | ?             | `libhidapi-dev`      | `hidapi`     | `hidapi-devel`    | `hidapi-devel`      |
| libusb      | ?             | `libusb-dev`         | `libusb`     | `libusb-devel`    | `libusb-devel`      |
| libprotobuf | ?             | `libprotobuf-dev`    | `protobuf`   | `protobuf-devel`  | `protobuf-devel`    |
| protoc      | ?             | `protobuf-compiler`  | `protobuf`   | `protobuf`        | `protobuf-compiler` |


[1] On Debian/Ubuntu `libgtest-dev` only includes sources and headers. You must
build the library binary manually. This can be done with the following command ```sudo apt-get install libgtest-dev && cd /usr/src/gtest && sudo cmake . && sudo make && sudo mv libg* /usr/lib/ ```
[2] libnorm-dev is needed if your zmq library was built with libnorm, and not needed otherwise

Install all dependencies at once on Debian/Ubuntu:

``` sudo apt update && sudo apt install build-essential cmake pkg-config libboost-all-dev libssl-dev libzmq3-dev libunbound-dev libsodium-dev libunwind8-dev liblzma-dev libreadline6-dev libldns-dev libexpat1-dev doxygen graphviz libpgm-dev qttools5-dev-tools libhidapi-dev libusb-dev libprotobuf-dev protobuf-compiler ```

Install all dependencies at once on macOS with the provided Brewfile:
``` brew update && brew bundle --file=contrib/brew/Brewfile ```

FreeBSD one liner for required to build dependencies
```pkg install git gmake cmake pkgconf boost-libs libzmq libsodium```

### Cloning the repository

Clone recursively to pull-in needed submodule(s):

`$ git clone --recursive https://github.com/scala-network/scala`

If you already have a repo cloned, initialize and update:

`$ cd scala && git submodule init && git submodule update`

### Build instructions

Scala uses the CMake build system and a top-level [Makefile](Makefile) that
invokes cmake commands as needed.

#### On Linux and macOS

* Install the dependencies

* Change to the root of the source code directory, change to the most recent release branch, and build:

  ```bash
  cd scala
  make
  ```

  *Optional*: If your machine has several cores and enough memory, enable parallel build by running `make -j<number of threads>` instead of `make`. For this to be worthwhile, the machine should have one core and about 2GB of RAM available per thread.


* The resulting executables can be found in `build/release/bin`

* Add `PATH="$PATH:$HOME/scala/build/release/bin"` to `.profile`

* Run Scala with `scalad --detach`

* **Optional**: build and run the test suite to verify the binaries:

  ```bash
  make release-test
  ```

  *NOTE*: `core_tests` test may take a few hours to complete.

* **Optional**: to build binaries suitable for debugging:

  ```bash
  make debug
  ```

* **Optional**: to build statically-linked binaries:

  ```bash
  make release-static
  ```

Dependencies need to be built with -fPIC. Static libraries usually aren't, so you may have to build them yourself with -fPIC. Refer to their documentation for how to build them.

* **Optional**: build documentation in `doc/html` (omit `HAVE_DOT=YES` if `graphviz` is not installed):

  ```bash
  HAVE_DOT=YES doxygen Doxyfile
  ```

#### On Windows:

Binaries for Windows are built on Windows using the MinGW toolchain within [MSYS2 environment](https://www.msys2.org). The MSYS2 environment emulates a POSIX system. The toolchain runs within the environment and *cross-compiles* binaries that can run outside of the environment as a regular Windows application.

**Preparing the build environment**

* Download and install the [MSYS2 installer](https://www.msys2.org), the 64-bit package, depending on your system.

* Open the MSYS shell via the `MSYS2 Shell` shortcut

* Update packages using pacman:

  ```bash
  pacman -Syu
  ```

* Exit the MSYS shell using Alt+F4  

* Edit the properties for the `MSYS2 Shell` shortcut changing "msys2_shell.bat" to "msys2_shell.cmd -mingw64" 

* Restart MSYS shell via modified shortcut and update packages again using pacman:  

  ```bash
  pacman -Syu
  ```


* Install dependencies:

  To build for 64-bit Windows:

  ```bash
  pacman -S mingw-w64-x86_64-toolchain make mingw-w64-x86_64-cmake mingw-w64-x86_64-boost mingw-w64-x86_64-openssl mingw-w64-x86_64-zeromq mingw-w64-x86_64-libsodium mingw-w64-x86_64-hidapi
  ```

* Open the MingW shell via `MinGW-w64-Win64 Shell` shortcut on 64-bit Windows

**Cloning**

* To git clone, run:

  ```bash
  git clone --recursive https://github.com/scala-network/scala.git
  ```

**Building**

* Change to the cloned directory, run:

  ```bash
  cd scala
  ```

* If you would like a specific [version/tag](https://github.com/scala-project/scala/tags), do a git checkout for that version. eg. 'v7.5.0'. If you don't care about the version and just want binaries from master, skip this step:

  ```bash
  git checkout v7.5.0
  ```

* To start building run:

  ```bash
  make release-static-win64
  ```

* The resulting executables can be found in `build/release/bin`

* **Optional**: to build Windows binaries suitable for debugging , run:

  ```bash
  make debug-static-win64
  ```

* The resulting executables can be found in `build/debug/bin`

## More Information

A lot more information about the project is available on the [wiki](https://wiki.scalaproject.io) and we also hang out a lot on [discord](https://chat.scalaproject.io) we would love to discuss new ideas and projects with you <3

## Licenses

Copyright (c) 2018-2021 The Scala Network.

Copyright (c) 2014-2021 The Monero Project. 

Portions Copyright (c) 2012-2013 The Cryptonote developers.

Also please see the LICENSE file for more information.
