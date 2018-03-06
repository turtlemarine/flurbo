# Flurbo

Flurbo is the first private open-source intergalactic decentralized anonymous free crypto currency based on CryptoNote that is used by more than 84 alien species to easily send money online throughout the galaxy and all dimensions. Wubba Lubba Dub Dub!

- [Bitcointalk](https://bitcointalk.org)
- [Our Website](http://flurbo.cc)
- [Our Mining Pool](http://flurbo.cc/pool#)
- [Twitter](https://twitter.com/GetFlurbo)
- [Telegram Group](https://t.me/flurbotalk)
- [Telegram Channel](https://t.me/flurbo)

## Mission

Everyone want's extend life of "Rick and Morty" show for as long as possible and everyone want's see as much as possible seasons of it, as sooner as possible! We have a plan. The first step we take on ourselves, we created Flurbo currency and we will send a donation of 1,000,000,000 FLB to Dan Harmon and Justin Roiland. The second and most important step is entirely up to you. You ask how can you help? Just become a part of the Flurbo community and help it's grow! Let's make Flurbo great! Rikki-Tikki-Tavi, Bitch! Remember, it's all up to you and that's the waaaayyy the news goes! AIDS!

## How to start use Flurbo?

You can download Linux or Windows binaries of "**Flurbo Wallet**" CLI or GUI from [Flurbo Download Page](http://flurbo.cc/#download) and that's why I always say "Shumshumschilpiddydah!".

## Supporting the project

Flurbo is a 100% community-sponsored endeavor. If you want to join our efforts, the easiest thing you can do is support the project financially. All funds received will go to promote Flurbo and it's exchange rate. Bitcoin donations can be made to **http://flurbo.cc/#donate**.

The Bitcoin donation address is: `3JxwkWq4uP7CaLXUDqwRwtj1kkKfxcXzF6`

## License

See [LICENSE](LICENSE).

## Contributing

If you want to help out or maybe became a part of Flurbo developers, write to [@iMrPoopybutthole](https://t.me/iMrPoopybutthole) for a set of guidelines.

## Compiling Flurbo from source

### Dependencies

| Dep          | Min. version  | Vendored | Debian/Ubuntu pkg  | Arch pkg     | Fedora            | Optional | Purpose        |
| ------------ | ------------- | -------- | ------------------ | ------------ | ----------------- | -------- | -------------- |
| GCC          | 4.7.3         | NO       | `build-essential`  | `base-devel` | `gcc`             | NO       |                |
| CMake        | 3.2.0         | NO       | `cmake`            | `cmake`      | `cmake`           | NO       |                |
| pkg-config   | any           | NO       | `pkg-config`       | `base-devel` | `pkgconf`         | NO       |                |
| Boost        | 1.58          | NO       | `libboost-all-dev` | `boost`      | `boost-devel`     | NO       | C++ libraries  |
| OpenSSL      | basically any | NO       | `libssl-dev`       | `openssl`    | `openssl-devel`   | NO       | sha256 sum     |
| libzmq       | 3.0.0         | NO       | `libzmq3-dev`      | `zeromq`     | `cppzmq-devel`    | NO       | ZeroMQ library |
| libunbound   | 1.4.16        | YES      | `libunbound-dev`   | `unbound`    | `unbound-devel`   | NO       | DNS resolver   |
| libsodium    | ?             | NO       | `libsodium-dev`    | ?            | `libsodium-devel` | NO       | libsodium      |
| libminiupnpc | 2.0           | YES      | `libminiupnpc-dev` | `miniupnpc`  | `miniupnpc-devel` | YES      | NAT punching   |
| libunwind    | any           | NO       | `libunwind8-dev`   | `libunwind`  | `libunwind-devel` | YES      | Stack traces   |
| liblzma      | any           | NO       | `liblzma-dev`      | `xz`         | `xz-devel`        | YES      | For libunwind  |
| libreadline  | 6.3.0         | NO       | `libreadline6-dev` | `readline`   | `readline-devel`  | YES      | Input editing  |
| ldns         | 1.6.17        | NO       | `libldns-dev`      | `ldns`       | `ldns-devel`      | YES      | SSL toolkit    |
| expat        | 1.1           | NO       | `libexpat1-dev`    | `expat`      | `expat-devel`     | YES      | XML parsing    |
| GTest        | 1.5           | YES      | `libgtest-dev`^    | `gtest`      | `gtest-devel`     | YES      | Test suite     |
| Doxygen      | any           | NO       | `doxygen`          | `doxygen`    | `doxygen`         | YES      | Documentation  |
| Graphviz     | any           | NO       | `graphviz`         | `graphviz`   | `graphviz`        | YES      | Documentation  |

### Build instructions

Flurbo uses the CMake build system and a top-level [Makefile](Makefile)
that invokes cmake commands as needed.

#### On Linux and OS X

* Install the dependencies
* Change to the root of the source code directory and build:
```
cd flurbo
make
```

* The resulting executables can be found in "build/release/bin"

* Add PATH="$PATH:$HOME/flurbo/build/release/bin" to ".profile"

* Run Flurbo with "flurbod --detach"

* **Optional**: build and run the test suite to verify the binaries:
```
make release-test
```
*NOTE*: "core_tests" test may take a few hours to complete.

* **Optional**: to build binaries suitable for debugging:
```
make debug
```
* **Optional**: to build statically-linked binaries:
```
make release-static
```
* **Optional**: build documentation in "doc/html" (omit "HAVE_DOT=YES" if "graphviz" is not installed):
```
HAVE_DOT=YES doxygen Doxyfile
```
#### On Windows:

Binaries for Windows are built on Windows using the MinGW toolchain within
[MSYS2 environment](http://msys2.github.io). The MSYS2 environment emulates a
POSIX system. The toolchain runs within the environment and *cross-compiles*
binaries that can run outside of the environment as a regular Windows
application.

**Preparing the build environment**

* Download and install the [MSYS2 installer](http://msys2.github.io), either the 64-bit or the 32-bit package, depending on your system.
* Open the MSYS shell via the `MSYS2 Shell` shortcut
* Update packages using pacman:  
```
pacman -Syuu  
```
* Exit the MSYS shell using Alt+F4  
* Edit the properties for the "MSYS2 Shell" shortcut changing "msys2_shell.bat" to "msys2_shell.cmd -mingw64" for 64-bit builds or "msys2_shell.cmd -mingw32" for 32-bit builds
* Restart MSYS shell via modified shortcut and update packages again using pacman:  
```
pacman -Syuu  
```
* Install dependencies:
  To build for 64-bit Windows:
```
pacman -S mingw-w64-x86_64-toolchain make mingw-w64-x86_64-cmake mingw-w64-x86_64-boost mingw-w64-x86_64-openssl mingw-w64-x86_64-zeromq mingw-w64-x86_64-libsodium
```
To build for 32-bit Windows:
```
pacman -S mingw-w64-i686-toolchain make mingw-w64-i686-cmake mingw-w64-i686-boost mingw-w64-i686-openssl mingw-w64-i686-zeromq mingw-w64-i686-libsodium
```
* Open the MingW shell via "MinGW-w64-Win64 Shell" shortcut on 64-bit Windows
  or "MinGW-w64-Win64 Shell" shortcut on 32-bit Windows. Note that if you are
  running 64-bit Windows, you will have both 64-bit and 32-bit MinGW shells.

**Building**

* If you are on a 64-bit system, run:
```
make release-static-win64
```
* If you are on a 32-bit system, run:
```
make release-static-win32
```
* The resulting executables can be found in "build/release/bin"

### Building portable statically linked binaries

By default, in either dynamically or statically linked builds, binaries target the specific host processor on which the build happens and are not portable to other processors. Portable binaries can be built using the following targets:

```
make release-static-linux-x86_64 - builds binaries on Linux on x86_64 portable across POSIX systems on x86_64 processors
make release-static-linux-i686 - builds binaries on Linux on x86_64 or i686 portable across POSIX systems on i686 processors
make release-static-linux-armv8 - builds binaries on Linux portable across POSIX systems on armv8 processors
make release-static-linux-armv7 - builds binaries on Linux portable across POSIX systems on armv7 processors
make release-static-linux-armv6 - builds binaries on Linux portable across POSIX systems on armv6 processors
make release-static-win64 - builds binaries on 64-bit Windows portable across 64-bit Windows systems
make release-static-win32 - builds binaries on 64-bit or 32-bit Windows portable across 32-bit Windows systems
```

## Running flurbod

The build places the binary in "bin/" sub-directory within the build directory
from which cmake was invoked (repository root by default). To run in
foreground:
```
./bin/flurbod
```
To list all available options, run "./bin/flurbod --help".  Options can be
specified either on the command line or in a configuration file passed by the
"--config-file" argument.  To specify an option in the configuration file, add
a line with the syntax "argumentname=value", where "argumentname" is the name
of the argument without the leading dashes, for example "log-level=1".

To run in background:
```
./bin/flurbod --log-file flurbod.log --detach
```
To run as a systemd service, copy
[flurbod.service](utils/systemd/flurbod.service) to "/etc/systemd/system/" and
[flurbod.conf](utils/conf/flurbod.conf) to "/etc/". The [example
service](utils/systemd/flurbod.service) assumes that the user "flurbo" exists
and its home is the data directory specified in the [example
config](utils/conf/flurbod.conf).

After operation will be complete you can start using Flurbo by running this files:
* **flurbod** - Flurbo Blockchain Synchronization Daemon (this file you will need to run first, before flurbo-wallet-cli).
* **flurbo-wallet-cli** - Flurbo Wallet Application (this file you will need to run second, right after flurbod will start synchronization with blockchain network).

~ That's all, bitches! ~
