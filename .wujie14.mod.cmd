savedcmd_wujie14.mod := printf '%s\n'   wujie14-km.o wujie14-perfmode.o wujie14-wmi-event.o wujie14-kb.o | awk '!x[$$0]++ { print("./"$$0) }' > wujie14.mod
