################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../examples/BasicDrawing.cpp \
../examples/BlobExample.cpp \
../examples/ChangeConstrast.cpp \
../examples/DisplayImage.cpp \
../examples/DrawingAndText.cpp \
../examples/FileSmoothing.cpp \
../examples/LoadImage.cpp \
../examples/SaveImage.cpp \
../examples/TrackingDetection.cpp 

OBJS += \
./examples/BasicDrawing.o \
./examples/BlobExample.o \
./examples/ChangeConstrast.o \
./examples/DisplayImage.o \
./examples/DrawingAndText.o \
./examples/FileSmoothing.o \
./examples/LoadImage.o \
./examples/SaveImage.o \
./examples/TrackingDetection.o 

CPP_DEPS += \
./examples/BasicDrawing.d \
./examples/BlobExample.d \
./examples/ChangeConstrast.d \
./examples/DisplayImage.d \
./examples/DrawingAndText.d \
./examples/FileSmoothing.d \
./examples/LoadImage.d \
./examples/SaveImage.d \
./examples/TrackingDetection.d 


# Each subdirectory must supply rules for building sources it contributes
examples/%.o: ../examples/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/opencv -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


