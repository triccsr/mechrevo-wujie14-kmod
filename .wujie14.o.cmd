savedcmd_wujie14.o := ld -m elf_x86_64 -z noexecstack --no-warn-rwx-segments   -r -o wujie14.o @wujie14.mod  ; /usr/lib/modules/7.0.3-arch1-2/build/tools/objtool/objtool --hacks=jump_label --hacks=noinstr --hacks=skylake --ibt --orc --retpoline --rethunk --sls --static-call --uaccess --prefix=16  --link  --module wujie14.o

wujie14.o: $(wildcard /usr/lib/modules/7.0.3-arch1-2/build/tools/objtool/objtool)
