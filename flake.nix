{
  description = "Wait for a keypress on an evdev device";

  inputs.flake-utils.url = "github:numtide/flake-utils";
  #inputs.nixpkgs.url = "github:NixOS/nixpkgs";

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem
      (system:
        let
          pkgs = nixpkgs.legacyPackages.${system};
          wait-for-keypress = pkgs.stdenv.mkDerivation {
            name = "wait-for-keypress";
            version = "0.0.1";
            src = ./.;
            nativeBuildInputs = with pkgs; [ pkg-config gnumake ];
            buildInputs = with pkgs; [ libevdev ];
            installPhase = "cp ./wait_for_keypress $out";
          };
        in
        {
          packages.wait-for-keypress = wait-for-keypress;
          defaultPackage = wait-for-keypress;
          apps.wait-for-keypress = flake-utils.lib.mkApp {
            drv = wait-for-keypress;
          };
          defaultApp = wait-for-keypress;
        }
      );
}
