################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ConvertImage.cpp \
../src/proccess.cpp \
../src/sio_client.cpp \
../src/sio_client_impl.cpp \
../src/sio_packet.cpp \
../src/sio_socket.cpp 

OBJS += \
./src/ConvertImage.o \
./src/proccess.o \
./src/sio_client.o \
./src/sio_client_impl.o \
./src/sio_packet.o \
./src/sio_socket.o 

CPP_DEPS += \
./src/ConvertImage.d \
./src/proccess.d \
./src/sio_client.d \
./src/sio_client_impl.d \
./src/sio_packet.d \
./src/sio_socket.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include/boost -I/home/nhtoan/openalpr/ -I/usr/local/include -I/home/nhtoan/socket.io-client-cpp/lib/websocketpp -I/home/nhtoan/socket.io-client-cpp/lib/rapidjson/include -I/home/nhtoan/socket.io-client-cpp/src -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


