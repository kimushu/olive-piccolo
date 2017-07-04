
#########################################################################
#######          MAKEFILE FOR GENERATING FLASH BOOT FILE          #######
#########################################################################

#########################################################################
# This file is intended to be included by Makefile of each application
#
#
# The following variables must be defined before including this file:
# - ELF
# - BSP_ROOT_DIR
# - QUARTUS_PROJECT_DIR
#
# The following variables may be defined to override the default behavior:
# - RBF_FILE
# - COMBINED_RBF
#
#########################################################################

ifeq ($(RBF_FILE),)
RBF_FILE := $(firstword $(wildcard $(QUARTUS_PROJECT_DIR)/output_files/*.rbf) $(wildcard $(QUARTUS_PROJECT_DIR)/*.rbf))
endif

ifeq ($(COMBINED_RBF),)
COMBINED_RBF := $(dir $(ELF))/$(notdir $(basename $(RBF_FILE)))_$(notdir $(basename $(ELF))).rbf
endif

combined_rbf: $(COMBINED_RBF)

$(COMBINED_RBF): $(ELF) $(RBF_FILE)
	tclsh $(BSP_ROOT_DIR)/drivers/tools/flash_boot_gen.tcl -o $@ --elf $(ELF) $(addprefix --rbf ,$(RBF_FILE)) --offset 0x0 --compress none

.DELETE_ON_ERROR: $(COMBINED_RBF)

clean: flash_boot_clean

flash_boot_clean:
	@$(RM) $(COMBINED_RBF)
