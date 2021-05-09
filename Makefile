.PHONY: clean all

all: 
	cd libopencm3 && make
	cd openfat && make
	cd example && make

clean:
	cd libopencm3 && make $@
	cd openfat && make $@
	cd example && make $@