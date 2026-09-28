################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ruhmi_model/compute_sub_0000.c \
../src/ruhmi_model/compute_sub_0001.c \
../src/ruhmi_model/kernel_library_int.c \
../src/ruhmi_model/kernel_library_utils.c \
../src/ruhmi_model/model.c \
../src/ruhmi_model/sub_0002_command_stream.c \
../src/ruhmi_model/sub_0002_invoke.c \
../src/ruhmi_model/sub_0002_model_data.c \
../src/ruhmi_model/sub_0002_tensors.c 

C_DEPS += \
./src/ruhmi_model/compute_sub_0000.d \
./src/ruhmi_model/compute_sub_0001.d \
./src/ruhmi_model/kernel_library_int.d \
./src/ruhmi_model/kernel_library_utils.d \
./src/ruhmi_model/model.d \
./src/ruhmi_model/sub_0002_command_stream.d \
./src/ruhmi_model/sub_0002_invoke.d \
./src/ruhmi_model/sub_0002_model_data.d \
./src/ruhmi_model/sub_0002_tensors.d 

OBJS += \
./src/ruhmi_model/compute_sub_0000.o \
./src/ruhmi_model/compute_sub_0001.o \
./src/ruhmi_model/kernel_library_int.o \
./src/ruhmi_model/kernel_library_utils.o \
./src/ruhmi_model/model.o \
./src/ruhmi_model/sub_0002_command_stream.o \
./src/ruhmi_model/sub_0002_invoke.o \
./src/ruhmi_model/sub_0002_model_data.o \
./src/ruhmi_model/sub_0002_tensors.o 

SREC += \
ekra8p1_lcd_cam.srec 

MAP += \
ekra8p1_lcd_cam.map 


# Each subdirectory must supply rules for building sources it contributes
src/ruhmi_model/%.o: ../src/ruhmi_model/%.c
	$(file > $@.in,-mthumb -mfloat-abi=hard -mcpu=cortex-m85+nopacbti -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RAFSP_EK_RA8P1_ -D_RA_CORE=CPU0 -D_RA_ORDINAL=1 -DENABLE_ROS_HOST=0 -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/ra_gen" -I"." -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/ra_cfg/fsp_cfg/bsp" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/ra_cfg/fsp_cfg" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/src" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/ra/fsp/inc" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/ra/fsp/inc/api" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/ra/fsp/inc/instances" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/ra/arm/CMSIS_6/CMSIS/Core/Include" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/mtk3_bsp2" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/mtk3_bsp2/config" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/mtk3_bsp2/include" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/mtk3_bsp2/mtkernel/kernel/knlinc" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/src/SEGGER_RTT" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/Application" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/ra/fsp/src/r_mipi_csi" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/ra/fsp/src/r_vin" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/src/ruhmi_model" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/ra/fsp/src/rm_ethosu" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/ra/npu/tflite-micro" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/ra/npu/ruy" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/ra/npu/gemmlowp" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/ra/npu/ethos-u-core-driver/include" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/ra/npu/ethos-u-core-software/lib/layer_by_layer_profiler/include" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/ra/npu/ethos-u-core-software/lib/ethosu_monitor/include" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/ra/npu/ethos-u-core-software/lib/ethosu_profiler/include" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/ra/npu/ethos-u-core-software/lib/crc/include" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/ra/npu/ethos-u-core-software/lib/arm_profiler/include" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/ra/arm/CMSIS-View/EventRecorder/Include" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/ra/arm/CMSIS-View/EventRecorder/Config" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/ra/npu/flatbuffers/include" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/ra/arm/CMSIS-NN/Include" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/src/microros/include" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/src/microros/include/rcl" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/src/microros/include/rcutils" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/src/microros/include/rmw" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/src/microros/include/rosidl_runtime_c" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/src/microros/include/rosidl_typesupport_interface" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/src/microros/include/rcl_action" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/src/microros/include/action_msgs" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/src/microros/include/unique_identifier_msgs" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/src/microros/include/builtin_interfaces" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/src/microros/include/std_msgs" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/src/microros/include/rclc" -I"C:/Users/tsuto/e2_studio/workspace-fsp6.4.0/ekra8p1_lcd_cam/src/microros" -std=c99 -Wno-stringop-overflow -Wno-format-truncation -flax-vector-conversions --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

