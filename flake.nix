{
  description = "Wait for a keypress on an evdev device";

  inputs.flake-utils.url = "github:numtide/flake-utils";
  inputs.nixpkgs.url = "github:NixOS/nixpkgs";

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem
      (system:
        let
          pkgs = nixpkgs.legacyPackages.${system};
          input-utils = pkgs.stdenv.mkDerivation {
            name = "input-utils";
            version = "0.0.1";
            src = ./.;
            nativeBuildInputs = with pkgs; [ pkg-config gnumake ];
            buildInputs = with pkgs; [ libevdev ];
            installPhase = ''
              mkdir -p $out/bin
              cp ./wait-for-keypress ./find-input-device $out/bin/
            '';
          };
        in
        {
          packages.input-utils = input-utils;
          defaultPackage = input-utils;
        }
      );
}
