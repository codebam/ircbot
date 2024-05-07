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
          buildInputs = with pkgs; [
            libircclient
          ];
          buildPhase = ''
            make main
          '';
          installPhase = ''
            mkdir -p $out/bin
            cp main $out/bin
          '';
        };
      devShell = pkgs.mkShell {
        buildInputs = with pkgs; [
          clang-tools
          libircclient
        ];
      };
    }
  );
}
