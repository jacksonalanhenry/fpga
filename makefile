# Testbench selection
TEST ?= nco

# Source mapping
VERILOG_SOURCES = rtl/$(TEST).v rtl/phase_accumulator.v rtl/sine_wave_rom.v
CPP_SOURCES = sim/$(TEST)_testbench.cpp

# Output directories and files
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj_dir_$(TEST)
OUTPUTS_DIR = sim/outputs
EXE = $(OBJ_DIR)/V$(TEST)
WAVEFILE = $(OUTPUTS_DIR)/$(TEST)_wave.vcd
WAVEFORMS = $(OUTPUTS_DIR)/$(TEST)_wave.gtkw

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
	@# Create the .gtkw file if it doesn't exist
	@if [ ! -f $(WAVEFORMS) ]; then \
		echo "Creating empty waveform layout: $(WAVEFORMS)"; \
		touch $(WAVEFORMS); \
	fi
	gtkwave --autosavename --rcvar "splash_disable on" $(WAVEFILE)

all_tests:
	$(MAKE) TEST=clk_divider
	$(MAKE) TEST=osc_square
	$(MAKE) TEST=phase_accumulator
	$(MAKE) TEST=sin_wave_lut

# Directories
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OUTPUTS_DIR):
	mkdir -p $(OUTPUTS_DIR)

# Clean everything
clean:
	rm -rf $(BUILD_DIR)
	rm -f sim/outputs/*.vcd
	rm -f sim/outputs/*.gtkw
	rm -f V*

# Create clangd's compile_commands using bear
compile_commands:
	rm -f compile_commands.json
	bear -- make all_tests

# Paths
GEN_BIN = build/gen_sin_lut
HEX_OUT = sim/lut/sine_rom.hex

# Target to build and run the LUT generator
gen_sin_lut: $(HEX_OUT)

$(HEX_OUT): sim/lut/generate_sin_lut.cpp
	@mkdir -p $(dir $(GEN_BIN))
	@g++ -std=c++17 -o $(GEN_BIN) sim/lut/generate_sin_lut.cpp
	@$(GEN_BIN)

.PHONY: all run wave clean compile_commands gen_sin_lut

