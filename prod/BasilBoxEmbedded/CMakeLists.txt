cmake_minimum_required(VERSION 3.15)

project(BasilBoxEmbedded LANGUAGES C CXX ASM)

find_bsp(
	ID com.sysprogs.project_importers.stm32.cubemx
	SOURCE_PROJECT BasilBoxEmbedded.ioc
	CONFIGURATION "com.sysprogs.toolchainoptions.arm.libctype=--specs=nano.specs -u _printf_float -u _scanf_float")

bsp_include_directories(Drivers/protobuf)
add_bsp_based_executable(
	NAME BasilBoxEmbedded
	GENERATE_BIN
	GENERATE_MAP
	GENERATE_HEX)

target_include_directories(BasilBoxEmbedded PRIVATE Drivers/protobuf)
