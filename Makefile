.PHONY: clean

all: obj bin bin/am335x-bone.dtb bin/xzoe bin/pru1_lcd.bin

obj:
	mkdir -p obj

bin:
	mkdir -p bin

bin/am335x-bone.dtb: am335x-bone.dts
	dtc -I dts -O dtb -o bin/am335x-bone.dtb am335x-bone.dts

bin/xzoe: obj/xzoe.o obj/pru1_lcd.o libprussdrv.a
	gcc -Wall -Werror -o bin/xzoe obj/xzoe.o obj/pru1_lcd.o libprussdrv.a -lpthread -lX11 -lXdamage

obj/xzoe.o: xzoe.c pru1_lcd.h
	gcc -c -Wall -Werror xzoe.c -o obj/xzoe.o

obj/pru1_lcd.o: pru1_lcd.c pru1_lcd.h prussdrv.h
	gcc -c -Wall -Werror pru1_lcd.c -o obj/pru1_lcd.o

bin/pru1_lcd.bin: pru1_lcd.p
	./pasm -V3 -b pru1_lcd.p bin/pru1_lcd

clean:
	rm -rf obj
	rm -rf bin

