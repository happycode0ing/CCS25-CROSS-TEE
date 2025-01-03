################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/g_canfd0_guard.c \
../src/g_canfd1_guard.c \
../src/g_uart4_guard.c \
../src/hal_entry.c 

C_DEPS += \
./src/g_canfd0_guard.d \
./src/g_canfd1_guard.d \
./src/g_uart4_guard.d \
./src/hal_entry.d 

OBJS += \
./src/g_canfd0_guard.o \
./src/g_canfd1_guard.o \
./src/g_uart4_guard.o \
./src/hal_entry.o 

SREC += \
TrustZone_S_Ethernet.srec 

MAP += \
TrustZone_S_Ethernet.map 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	$(file > $@.in,-mcpu=cortex-m33 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -mcmse -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RA_TZ_SECURE -D_RA_CORE=CM33 -D_RA_ORDINAL=1 -I"D:/DTEE/subf-TEE/subf_TEE_S/src" -I"." -I"D:/DTEE/subf-TEE/subf_TEE_S/ra/fsp/inc" -I"D:/DTEE/subf-TEE/subf_TEE_S/ra/fsp/inc/api" -I"D:/DTEE/subf-TEE/subf_TEE_S/ra/fsp/inc/instances" -I"D:/DTEE/subf-TEE/subf_TEE_S/ra/arm/CMSIS_6/CMSIS/Core/Include" -I"D:/DTEE/subf-TEE/subf_TEE_S/ra_gen" -I"D:/DTEE/subf-TEE/subf_TEE_S/ra_cfg/fsp_cfg/bsp" -I"D:/DTEE/subf-TEE/subf_TEE_S/ra_cfg/fsp_cfg" -std=c99 -Wno-stringop-overflow -Wno-format-truncation --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

