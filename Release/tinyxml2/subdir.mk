################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tinyxml2/tinyxml2.cpp 

OBJS += \
./tinyxml2/tinyxml2.o 

CPP_DEPS += \
./tinyxml2/tinyxml2.d 


# Each subdirectory must supply rules for building sources it contributes
tinyxml2/%.o: ../tinyxml2/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -std=c++0x -I"/home/andrey/works/dyn_sheduling/include" -I"/home/andrey/works/dyn_sheduling/tinyxml2" -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


