{
  inputs = {
    retro68 = {
      url = "github:autc04/Retro68";
    };
    utils = {
      url = "github:numtide/flake-utils";
    };
  };

  outputs = {
    self,
    nixpkgs,
    retro68,
    utils,
  }:
    utils.lib.eachDefaultSystem (
      system: let
        pkgs = import nixpkgs {inherit system;};
        minivmac-custom = with pkgs;
          minivmac.overrideAttrs {
            # Copied entirely from upstream, but with modified setup options
            configurePhase = ''
              ${lib.getExe' buildPackages.stdenv.cc "cc"} setup/tool.c -o setup_t

              # This is the line that I changed
              NEW_OPTS="-m 512Ke -speed z -magnify 1 -bg 1 -ta 2"
              ./setup_t -t lx64 $NEW_OPTS > setup.sh

              # Patch hardcoded references in setup.sh to cross-aware counterparts
              substituteInPlace setup.sh --replace 'gcc ' '${stdenv.cc.targetPrefix}cc '
              substituteInPlace setup.sh --replace 'strip --strip-unneeded' '${stdenv.cc.targetPrefix}strip --strip-unneeded'

              sh < ./setup.sh
            '';
          };
      in {
        devShell = with pkgs;
          mkShell {
            buildInputs = [
              minivmac-custom
              retro68.packages.${system}.tools
            ];
          };
      }
    );
}
