################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra_gen/common_data.c \
../ra_gen/hal_data.c \
../ra_gen/main.c \
../ra_gen/net_thread.c \
../ra_gen/pin_data.c \
../ra_gen/vector_data.c 

C_DEPS += \
./ra_gen/common_data.d \
./ra_gen/hal_data.d \
./ra_gen/main.d \
./ra_gen/net_thread.d \
./ra_gen/pin_data.d \
./ra_gen/vector_data.d 

OBJS += \
./ra_gen/common_data.o \
./ra_gen/hal_data.o \
./ra_gen/main.o \
./ra_gen/net_thread.o \
./ra_gen/pin_data.o \
./ra_gen/vector_data.o 

SREC += \
TrustZone_NS_Ethernet.srec 

MAP += \
TrustZone_NS_Ethernet.map 


# Each subdirectory must supply rules for building sources it contributes
ra_gen/%.o: ../ra_gen/%.c
	$(file > $@.in,-mcpu=cortex-m33 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RA_TZ_NONSECURE -D_RA_CORE=CM33 -D_RA_ORDINAL=2 -I"D:/DTEE/subf-TEE/subf_TEE_NS/src" -I"." -I"D:/DTEE/subf-TEE/subf_TEE_NS/ra/fsp/inc" -I"D:/DTEE/subf-TEE/subf_TEE_NS/ra/fsp/inc/api" -I"D:/DTEE/subf-TEE/subf_TEE_NS/ra/fsp/inc/instances" -I"D:/DTEE/subf-TEE/subf_TEE_NS/ra/fsp/src/rm_freertos_port" -I"D:/DTEE/subf-TEE/subf_TEE_NS/ra/aws/FreeRTOS/FreeRTOS/Source/include" -I"D:/DTEE/subf-TEE/subf_TEE_NS/ra/arm/CMSIS_6/CMSIS/Core/Include" -I"D:/DTEE/subf-TEE/subf_TEE_NS/ra_gen" -I"D:/DTEE/subf-TEE/subf_TEE_NS/ra_cfg/fsp_cfg/bsp" -I"D:/DTEE/subf-TEE/subf_TEE_NS/ra_cfg/fsp_cfg" -I"D:/DTEE/subf-TEE/subf_TEE_NS/ra_cfg/aws" -I"D:/DTEE/subf-TEE/subf_TEE_NS/src/my_nsc_api" -I"D:/DTEE/subf-TEE/subf_TEE_NS/ra/aws/FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/source/include" -I"D:/DTEE/subf-TEE/subf_TEE_NS/ra/fsp/src/rm_freertos_plus_tcp" -std=c99 -Wno-stringop-overflow -Wno-format-truncation --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

