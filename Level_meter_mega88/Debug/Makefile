################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../average.c \
../lcd_lib.c \
../main.c \
../ScanKeys.c \
../tools.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
average.o \
lcd_lib.o \
main.o \
ScanKeys.o \
tools.o

OBJS_AS_ARGS +=  \
average.o \
lcd_lib.o \
main.o \
ScanKeys.o \
tools.o

C_DEPS +=  \
average.d \
lcd_lib.d \
main.d \
ScanKeys.d \
tools.d

C_DEPS_AS_ARGS +=  \
average.d \
lcd_lib.d \
main.d \
ScanKeys.d \
tools.d

OUTPUT_FILE_PATH +=Level_meter_mega88.elf

OUTPUT_FILE_PATH_AS_ARGS +=Level_meter_mega88.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler











./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 5.4.0
	$(QUOTE)D:\Program Files\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG -DF_CPU=1000000  -I"D:\Program Files\7.0\Packs\atmel\ATmega_DFP\1.0.98\include"  -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atmega88p -B "D:\Program Files\7.0\Packs\atmel\ATmega_DFP\1.0.98\gcc\dev\atmega88p" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 5.4.0
	$(QUOTE)D:\Program Files\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="Level_meter_mega88.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=atmega88p -B "D:\Program Files\7.0\Packs\atmel\ATmega_DFP\1.0.98\gcc\dev\atmega88p"  
	@echo Finished building target: $@
	"D:\Program Files\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "Level_meter_mega88.elf" "Level_meter_mega88.hex"
	"D:\Program Files\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "Level_meter_mega88.elf" "Level_meter_mega88.eep" || exit 0
	"D:\Program Files\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "Level_meter_mega88.elf" > "Level_meter_mega88.lss"
	"D:\Program Files\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "Level_meter_mega88.elf" "Level_meter_mega88.srec"
	"D:\Program Files\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-size.exe" "Level_meter_mega88.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "Level_meter_mega88.elf" "Level_meter_mega88.a" "Level_meter_mega88.hex" "Level_meter_mega88.lss" "Level_meter_mega88.eep" "Level_meter_mega88.map" "Level_meter_mega88.srec" "Level_meter_mega88.usersignatures"
	