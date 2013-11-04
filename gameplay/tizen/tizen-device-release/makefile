-include ../makefile.init

RM := rm -f

# All of the sources participating in the build are defined here
# -include sources.mk
O_SRCS := 
CPP_SRCS := 
C_UPPER_SRCS := 
C_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
CXX_SRCS := 
C++_SRCS := 
CC_SRCS := 
OBJS := 
C++_DEPS := 
C_DEPS := 
CC_DEPS := 
ARCHIVES := 
CPP_DEPS := 
C_UPPER_DEPS := 
CXX_DEPS := 

CPP_SRCS_COMMON :=

DEFINES := -D__TIZEN__

# Every subdirectory with source files must be described here
SUBDIRS := \
src \

# -include src/subdir.mk
# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS_COMMON += $(wildcard ../../src/*.cpp)
CPP_SRCS_COMMON += $(wildcard ../../src/lua/*.cpp)
CPP_SRCS_COMMON += $(wildcard ../../src/social/*.cpp)

EXCLUDE_TIZEN := ../../src/PlatformLinux.cpp \
                 ../../src/PlatformWindows.cpp \
				 ../../src/PlatformMacOSX.cpp \
				 ../../src/PlatformBlackBerry.cpp \
				 ../../src/PlatformiOS.cpp \
				 ../../src/PlatformAndroid.cpp \
				 ../../src/gameplay-main-% \

TMP := $(CPP_SRCS_COMMON)

CPP_SRCS_COMMON := $(filter-out $(EXCLUDE_TIZEN), $(TMP))

TMP_FILES += $(patsubst ../../src/%, ./src/common/%, $(CPP_SRCS_COMMON))

OBJS += $(TMP_FILES:.cpp=.o)

CPP_DEPS += $(TMP_FILES:.cpp=.d)

BULLET_INCLUDE := ../../../external-deps/bullet/include/
PNG_INCLUDE := ../../../external-deps/libpng/include/
ZLIB_INCLUDE := ../../../external-deps/zlib/include/
LUA_INCLUDE := ../../../external-deps/lua/include/
COMMON_INCLUDE := -I../../src -I$(BULLET_INCLUDE) -I$(PNG_INCLUDE) -I$(ZLIB_INCLUDE) -I$(LUA_INCLUDE) 

dir_guard=@mkdir -p $(@D)

# Each subdirectory must supply rules for building sources it contributes
src/common/%.o: ../../src/%.cpp
	$(dir_guard)
	@echo 'Building file: $<'
	@echo 'Invoking: C++ Compiler'
	$(CC) $(DEFINES) -I"$(PROJPATH)/inc" -O2 -Wall -c -fmessage-length=0 $(COMMON_INCLUDE) -I$(ROOTSTRAP)/usr/include/libxml2 -I$(SDKPATH)/library -I$(ROOTSTRAP)/usr/include -I$(ROOTSTRAP)/usr/include/osp  $(TC_COMPILER_MISC) $(RS_COMPILER_MISC) -Wno-overloaded-virtual  -fPIC --sysroot=$(ROOTSTRAP)  -D_APP_LOG -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

# -include objects.mk
USER_OBJS :=

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: libgameplay.a secondary-outputs

# Tool invocations
libgameplay.a: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Archiver'
	arm-linux-gnueabi-ar -r  "libgameplay.a" $(OBJS) $(USER_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(ARCHIVES)$(CPP_DEPS)$(C_UPPER_DEPS)$(CXX_DEPS) libgameplay.a
	-@echo ' '

secondary-outputs:

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
