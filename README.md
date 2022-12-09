# VoiceRepeater
The Voice Repeater project is a simple program detecting voice (sound) from an audioinput to the Daisy Patch Submodule and playing back the detected sound to the user.
This is a final Project for the course MUMT 306 Music and Audio Computing 1 at McGill University.

Please refer to the Wiki for the project report (scope, research, results, etc.).

## Getting the Source

There are a few ways to clone and initialize the repo (with its submodules).

You can do either of the following:

```sh
git clone --recursive https://github.com/SimonNM/MUMT306-VoiceRepeater
```

or

```sh
git clone https://github.com/SimonNM/MUMT306-VoiceRepeater
git submodule update --init
```

## Compiling wavae_daisy

If this is the first time building the project and have not compiled the libraries make sure to follow the next subsection.
Alternaitvely, you may execute the `build_all` task in VS code.

To build simply run the command `make` in clone directory.

To bootload the generated binary files to the Daisy patch sm run the command `make program-dfu`.

The build can also be cleared with the command `make clean`.

Alternatively, VS code tasks are available to perform the above.

### Compiling Libraries

Once you have the repository and the submodules (libDaisy) properly cloned, and the toolchain installed (for details see the [Daisy Wiki](https://github.com/electro-smith/DaisyWiki/wiki) for platform specific instructions) it's time to build the libraries.

To build libraries simply run:

`./build_libs.sh`

This is the same as going to library's directory and running `make`.

This may take a few minutes depending on your computer's hardware. But should have the following output when finished:

```sh
$ ./build_libs.sh 
building libDaisy . . .
rm -fR build
arm-none-eabi-ar: creating build/libdaisy.a
done.
```

## Updating the submodules

To pull everything for the repo and submodules:

```sh
git pull --recurse-submodules
```

to only pull changes for the submodules:

```sh
git submodule update --remote
```

Alternatively, you can simply run git commands from within the submodule and they will be treated as if you were in that repository instead of Daisy_Examples

Whenenever there are changes to the libraries (whether from pulling from git or manually editing the libraries) they will need to be rebuilt. This can be done by either running `./build_libs.sh` or entering the directory of the library with changes and running `make`.