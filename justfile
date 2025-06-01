build:
	./build.sh

DISK_SYSTEM_STARTUP := "System_6.0.6_System_Startup.dsk"
DISK_CONWAY := "../build/Conway.dsk"
ROM := "vMac.ROM"
[working-directory: 'emulator']
launch:
	minivmac {{ DISK_SYSTEM_STARTUP }} {{ DISK_CONWAY }} -r {{ ROM }}

APPL_CONWAY := "../build/Conway.APPL"
DISK_AUTOQUIT := "./software-disks/autoquit-1.1.1.dsk"
MINIVMAC_PATH := `which minivmac`
[working-directory: 'emulator']
launchAPPL:
	LaunchAPPL \
		-e minivmac \
		--minivmac-path {{ MINIVMAC_PATH }} \
		--minivmac-dir "" \
		--minivmac-rom {{ ROM }} \
		--system-image {{ DISK_SYSTEM_STARTUP }} \
		--autoquit-image {{ DISK_AUTOQUIT }} \
	 	{{ APPL_CONWAY }}
