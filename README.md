# Sonic Nexus v5
This is the Summer 2008 Demo of [Sonic Nexus](https://info.sonicretro.org/Sonic_Nexus) made by Taxman, ported to RSDKv5.

This is a direct port of the game, using little to no code from other RSDKv5 projects. There are also no enchancments made to the original game, save for widescreen support.

# How to Build

## Get the source code
First, you will need to clone the repository containing the source code. To do this, you need to install Git, which you can find [here](https://git-scm.com/downloads).

Clone the repo **recursively**, using:
`git clone https://github.com/MegAmi24/Sonic-Nexus-v5 --recursive`

If you've already cloned the repo, run these commands inside of the repository to ensure the clone is up-to-date:
```
git pull
git submodule update --remote --init --recursive
```

## Building

### Building with CMake
No external dependencies are required; simply type the following commands in the repository's root directory:
```
cmake -B build
cmake --build build --config release
```

The resulting build will be located somewhere in `build/` depending on your system.

The following CMake arguments are available when compiling:
- Use these on the `cmake -B build` step like so: `cmake -B build -DRETRO_REVISION=2`

#### Sonic Nexus v5 flags
- `RETRO_REVISION`: What RSDKv5 revision to compile for. Takes an integer, defaults to `3` (RSDKv5U).
- `RETRO_MOD_LOADER_VER`: Manually sets the mod loader version. Takes an integer, defaults to the current latest version.
- `GAME_INCLUDE_EDITOR`: Whether or not to include functions for use in certain RSDKv5 scene editors. Takes a boolean, defaults to `on`.

### Building in Visual Studio
Simply open the solution and build the project. Make sure that the architecture, engine revision, and mod loader version match with the target engine binary.

### Building for Android
After [setting up the RSDKv5(U) decompilation for building](https://github.com/Rubberduckycooly/RSDKv5-Decompilation#how-to-build), you will need to add a symbolic link for the directory to the repository so that the game will be built as part of the APK.
To add the symlink, write the appropriate command in `[RSDKv5-decompilation-root]/android/app/jni`:
  * Windows: `mklink /d Game "[path-to-game-repo]"`
  * Linux: `ln -s "[path-to-game-repo]" Game`

After adding the symlink, build the RSDKv5(U) decompilation in Android Studio.

# Special Thanks
* [Rubberduckycooly](https://github.com/Rubberduckycooly) and [st×tic](https://github.com/stxticOVFL) for the original RSDK decompilations.
* Everyone in the [Retro Engine Modding Server](https://dc.railgun.works/retroengine).
* Taxman and the Nexus Team for the original game.

# Disclaimer
Sonic Nexus is a fangame by Taxman and the Nexus Team. Sonic the Hedgehog and all related characters are property of SEGA and Sonic Team. This game is fan made and is not intended for retail.
