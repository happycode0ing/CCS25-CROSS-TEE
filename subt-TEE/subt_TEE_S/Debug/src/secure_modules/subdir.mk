################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/secure_modules/house_owner.c 

C_DEPS += \
./src/secure_modules/house_owner.d 

OBJS += \
./src/secure_modules/house_owner.o 

SREC += \
subt_TEE_S.srec 

MAP += \
subt_TEE_S.map 


# Each subdirectory must supply rules for building sources it contributes
src/secure_modules/%.o: ../src/secure_modules/%.c
	$(file > $@.in,-mcpu=cortex-m33 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -mcmse -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RA_TZ_SECURE -D_RA_CORE=CM33 -D_RA_ORDINAL=1 -I"D:/Workspace_e2s/subt_TEE_S/src" -I"." -I"D:/Workspace_e2s/subt_TEE_S/ra/fsp/inc" -I"D:/Workspace_e2s/subt_TEE_S/ra/fsp/inc/api" -I"D:/Workspace_e2s/subt_TEE_S/ra/fsp/inc/instances" -I"D:/Workspace_e2s/subt_TEE_S/ra/arm/CMSIS_6/CMSIS/Core/Include" -I"D:/Workspace_e2s/subt_TEE_S/ra_gen" -I"D:/Workspace_e2s/subt_TEE_S/ra_cfg/fsp_cfg/bsp" -I"D:/Workspace_e2s/subt_TEE_S/ra_cfg/fsp_cfg" -std=c99 -Wno-stringop-overflow -Wno-format-truncation --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

