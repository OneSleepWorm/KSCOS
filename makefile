CC = gcc
CXX = g++
ifeq ($(DEBUG),1)
OPT = -O0 -g
else
OPT = -O2
endif
CFLAGS = -std=c99 $(OPT) -Wall -I.
CXXFLAGS = -std=c++11 $(OPT) -Wall -I.
LDFLAGS = -leasyx

BUILD_DIR = build/mk

OBJS = \
	build/master.o \
	build/cmd.o \
	build/lfs.o \
	build/lfs_config.o \
	build/lfs_util.o \
	build/KSCOS.o \
	build/KSCimg.o \
	build/KSCfont.o \
	build/KSCdisplay.o \
	build/W25Q64.o 

TARGET = $(BUILD_DIR)/KSCOS.exe

all: prebuild $(OBJS) $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

prebuild:
	if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"

$(BUILD_DIR)/master.o: master.c | prebuild
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/cmd.o: src/cmd.c | prebuild
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/lfs.o: littlefs/lfs.c | prebuild
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/lfs_config.o: littlefs/lfs_config.c | prebuild
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/lfs_util.o: littlefs/lfs_util.c | prebuild
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/KSCOS.o: src/KSCdraw.c | prebuild
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/KSCimg.o: src/KSCimg.c | prebuild
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/KSCfont.o: src/KSCfont.c | prebuild
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/KSCdisplay.o: src/KSCdisplay.c | prebuild
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/W25Q64.o: src/W25Q64.c | prebuild
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)"

reboot:
	powershell -ExecutionPolicy Bypass -Command "make clean; make; make run"

reboot-debug:
	powershell -ExecutionPolicy Bypass -Command "make clean; make DEBUG=1; make run"

.PHONY: all clean prebuild reboot reboot-debug

run:
	./$(TARGET)
