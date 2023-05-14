# Sonic Riders Tournament Edition: certified bitch edition

This is quite simply a playground for Riders modification. For actual progress and information, you'll want to go to [sonicriderssrte](https://github.com/RidersBoulevard/sonicriderste).

## Building

### Required packages

* [devkitPro](https://devkitpro.org/wiki/Getting_Started)
* python3
* cmake **(Should be installed through devkitPro's pacman)**

### Building

For maximum compatibility, you should use the MSYS2 terminal included with your devkitPro installation. The following instructions assume you are using the MSYS2 terminal.

1. Make sure you have the correct version of cmake installed. You can do this in the terminal via `pacman -S cmake`.
2. Make sure you have Python installed in your MSYS2 environment. Since this environment is mostly "isolated" from the rest of your computer, if you haven't installed python3 before in the terminal, you can do so via `pacman -S python3`.
3. Make sure you have devKitPro's CMake utilities installed. You can do this in the terminal via `pacman -S dkp-cmake-common-utils`.
4. Clone the repo to your computer via `git clone` or any other similar way. **(This step doesn't necessarily have to go through the MSYS2 terminal.)**
5. Once cloned, navigate to the `tools` directory in the terminal using the `cd` command. Then run the `makeTools.sh` script via this command: `./makeTools.sh`.
6. After that, make a new folder in the root folder of the project, where the build files will be going to. For example, you can name the folder `build`.
7. Navigate into that directory using the `cd` command again. Then run the following command to generate the Makefiles: `$DEVKITPRO/devkitPPC/bin/powerpc-eabi-cmake ../`
8. Run `make` in the same directory to build the DOL file. The `main.dol` file will be built and compiled into the same build folder you created.

## Contributions

Contributions and PRs are welcome.
