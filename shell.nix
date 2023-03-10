with import <nixpkgs> {};
{
     testEnv = stdenv.mkDerivation {
       name = "helloTest";
       buildInputs = [stdenv ncurses];
     };
}
