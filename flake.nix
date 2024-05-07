{
  inputs = {
    utils.url = "github:numtide/flake-utils";
  };
  outputs = { self, nixpkgs, utils }: utils.lib.eachDefaultSystem (system:
    let
      pkgs = nixpkgs.legacyPackages.${system};
    in
    {
      packages.ircbot = pkgs.stdenv.mkDerivation
        {
          name = "ircbot";
          src = ./.;
          nativeBuildInputs = with pkgs; [
            pkg-config
          ];
          buildInputs = with pkgs; [
            meson
            ninja
            openssl
          ];
          buildPhase = ''
            meson compile
          '';
          installPhase = ''
            mkdir -p $out/bin
            cp ircbot $out/bin
          '';
        };
      devShell = pkgs.mkShell {
        buildInputs = with pkgs; [
          pkg-config
          openssl
          meson
          ninja
          clang-tools
        ];
      };
    }
  );
}
