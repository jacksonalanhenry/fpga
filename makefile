# Testbench selection
TEST ?= phase_accumulator

# Source mapping
VERILOG_SOURCES = rtl/$(TEST).v
CPP_SOURCES = sim/$(TEST)_testbench.cpp

# Output directories and files
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj_dir_$(TEST)
OUTPUTS_DIR = sim/outputs
EXE = $(OBJ_DIR)/V$(TEST)
WAVEFILE = $(OUTPUTS_DIR)/$(TEST)_wave.vcd

# Verilator options
VERILATOR_FLAGS = -Wall --cc --exe --trace --Mdir $(OBJ_DIR)

# Default target
all: run

# Build the testbench executable
$(EXE): $(VERILOG_SOURCES) $(CPP_SOURCES) | $(OBJ_DIR)
	verilator $(VERILATOR_FLAGS) $(VERILOG_SOURCES) $(CPP_SOURCES) -o V$(TEST)
	make -C $(OBJ_DIR) -f V$(TEST).mk V$(TEST)

# Run simulation
run: $(EXE) | $(OUTPUTS_DIR)
	./$(EXE) $(WAVEFILE)

# Open waveform
wave: run
	gtkwave $(WAVEFILE) --autosavename

all_tests:
	$(MAKE) TEST=clk_divider
	$(MAKE) TEST=osc_square
	$(MAKE) TEST=phase_accumulator

# Directories
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OUTPUTS_DIR):
	mkdir -p $(OUTPUTS_DIR)

# Clean everything
clean:
	rm -rf $(BUILD_DIR)
	rm -f sim/outputs/*.vcd
	rm -f V*

.PHONY: all run wave clean

