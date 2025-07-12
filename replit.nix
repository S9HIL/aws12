{ pkgs }: {
	deps = [
   pkgs.gti
   pkgs.unzipNLS
		pkgs.clang
		pkgs.ccls
		pkgs.gdb
		pkgs.gnumake
	];
}