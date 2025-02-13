PWDD := $(shell pwd)
BLOCKS := $(shell basename $(PWDD))

# ---- Include Partitioned Makefiles ----

CONFIG = caravel_user_project
USER_PROJECT_VERILOG = $(shell dirname `pwd`)

export COCOTB_REDUCED_LOG_FMT=1
# Change this line if you want to use existing cocotb test modules:
export PYTHONPATH := $(DESIGNS)/verilog/rtl/<your design python tests>
export LIBPYTHON_LOC=$(shell cocotb-config --libpython)


include $(MCW_ROOT)/verilog/dv/make/env.makefile
include $(MCW_ROOT)/verilog/dv/make/var.makefile
include $(MCW_ROOT)/verilog/dv/make/cpu.makefile
include $(MCW_ROOT)/verilog/dv/make/sim.makefile

# change the project.hex to your projects firmware file
coco_test: silife_test.hex
	rm -rf sim_build/
	mkdir sim_build/

	iverilog -Ttyp -DFUNCTIONAL -DSIM -DUSE_POWER_PINS -DSILIFE_TEST -DUNIT_DELAY=#1 \
	-f$(VERILOG_PATH)/includes/includes.rtl.caravel \
	-fincludes.rtl.silife -o sim_build/sim.vvp silife_tb.v

    # change this line to choose the comma separated test cases and the name of your python test module
	TESTCASE=test_life MODULE=test_silife vvp -M $$(cocotb-config --prefix)/cocotb/libs -m libcocotbvpi_icarus sim_build/sim.vvp
	! grep failure results.xml

coco_test_gl: silife_test.hex
	rm -rf sim_build/
	mkdir sim_build/

	iverilog -Ttyp -DFUNCTIONAL -DSIM -DUSE_POWER_PINS -DUNIT_DELAY=#1 \
	-f$(VERILOG_PATH)/includes/includes.rtl.caravel \
	-f$(USER_PROJECT_VERILOG)/includes/includes.gl.$(CONFIG) -o sim_build/sim.vvp silife_tb.v

    # change this line to choose the comma separated test cases and the name of your python test module
	TESTCASE=test_life MODULE=test_silife vvp -M $$(cocotb-config --prefix)/cocotb/libs -m libcocotbvpi_icarus sim_build/sim.vvp
	! grep failure results.xml

objdump: silife_test.elf
	$(GCC_PATH)/$(GCC_PREFIX)-objdump -d $^
