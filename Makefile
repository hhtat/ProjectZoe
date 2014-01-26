.PHONY: clean

all: obj bin bin/am335x-bone.dtb bin/xzoe bin/pru0_kb.bin bin/pru1_lcd.bin

obj:
	mkdir -p obj

bin:
	mkdir -p bin

bin/am335x-bone.dtb: am335x-bone.dts
	dtc -I dts -O dtb -o bin/am335x-bone.dtb am335x-bone.dts

bin/xzoe: obj/xzoe.o obj/pru.o libprussdrv.a
	gcc -O3 -Wall -Werror -o bin/xzoe obj/xzoe.o obj/pru.o libprussdrv.a -lpthread -lX11 -lXtst -lXdamage

obj/xzoe.o: xzoe.c pru.h
	gcc -O3 -c -Wall -Werror xzoe.c -o obj/xzoe.o

obj/pru.o: pru.c pru.h prussdrv.h
	gcc -O3 -c -Wall -Werror pru.c -o obj/pru.o

bin/pru0_kb.bin: pru0_kb.p
	./pasm -V3 -b pru0_kb.p bin/pru0_kb

bin/pru1_lcd.bin: pru1_lcd.p
	./pasm -V3 -b pru1_lcd.p bin/pru1_lcd

clean:
	rm -rf obj
	rm -rf bin

