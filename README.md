# Conway-Macintosh

A port of Conway's Game of Life for 68000 based Macintoshes.

It's written in C++ and built using [Retro68](https://github.com/autc04/Retro68).

## Usage

`.dsk` files are available on the Releases page.

## Building (For developers)

Prerequisites

- Install Docker
- Acquire Universal Interfaces (Not included in repo due to copyright restrictions)
- Set the path to your universal interfaces directory in the `.env` file

Instructions

1. Clone the repo
2. Open a shell
3. Change working directory to the root of the repo you cloned in step 1
4. Run the `build.sh` script

Once it's done running, the build outputs should be in the `./build` directory.

The first build will take a little while (depending on your internet
connection) because it'll download a large container image. Subsequent builds
will re-use this image and not take nearly as long.

## More Development Notes

During the development of this project, I used a few tools to improve the development experience.
They're all optional, but they really streamline the workflow.

[//]: # (TODO: continue explaining)

##### [Just](https://github.com/casey/just)

- justfile -> uses the "Just" command runner to make things easier to run (i.e.: "./build.sh && big-launch-command"
  turns into "just build launchAPPL". These "recipes" are configured in the repo's `justfile`.

##### [Nix](https://nixos.org/)

- nix stuff for easily patching and using minivmac and LaunchAPPL during development
  This is configured in the repo's `flake.nix` and `flake.lock` files.

Additionally, [nix-direnv](https://github.com/nix-community/nix-direnv) can be used to automatically
drop you into the aforementioned nix development shell when entering the directory.
It uses the repo's `.envrc` file to do this.
