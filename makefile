CC        := gcc
LD        := gcc

SRC_DIR 	:= src
BUILD_DIR 	:= build

#MODULES   	:= shared client server
SRC_DIRS   	:= $(addprefix SRC_DIR/,$(MODULES))
BUILD_DIRS 	:= $(addprefix BUILD_DIR,$(MODULES))

rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

SRC_FILES   := $(call rwildcard,$(SRC_DIR)/,*.cpp)
SHARED_SRC	:= $(call rwildcard,$(SRC_DIR)/shared/,*.cpp)
CLIENT_SRC	:= $(call rwildcard,$(SRC_DIR)/client/,*.cpp)
SERVER_SRC	:= $(call rwildcard,$(SRC_DIR)/server/,*.cpp)

OBJ_FILES 	:= $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))
SHARED_OBJ 	:= $(patsubst $(SRC_DIR)/shared/%.cpp,$(BUILD_DIR)/shared/%.o,$(SRC_FILES))
CLIENT_OBJ 	:= $(patsubst $(SRC_DIR)/client/%.cpp,$(BUILD_DIR)/client/%.o,$(SRC_FILES))
SERVER_OBJ 	:= $(patsubst $(SRC_DIR)/server/%.cpp,$(BUILD_DIR)/server/%.o,$(SRC_FILES))


OBJ       := $(patsubst src/%.cpp,build/%.o,$(SRC))
INCLUDES  := $(addprefix -I ,$(SRC_DIR))

define make-goal
build/%: src/%.cpp
	$(CC) $(INCLUDES) -c $$< -o $$@
endef

.PHONY: build/shared build/client build/server checkdirs

all: checkdirs crush.exe server.exe

build/shared: $(SHARED_OBJ)

build/client: build/shared $(CLIENT_OBJ)

build/server: build/shared $(SERVER_OBJ)

crush.exe: $(SHARED_OBJ) $(CLIENT_OBJ)
	$(LD) $^ -o $@

server.exe: $(SHARED_OBJ) $(SERVER_OBJ)
	$(LD) $^ -o $@

$(OBJ_FILES):
	$(CC) $(INCLUDES) -c $(patsubst $(BUILD_DIR)/%.o,$(SRC_DIR)/%.cpp,$@) -o $@

checkdirs: $(BUILD_DIRS)

$(BUILD_DIRS):
	@mkdir -p $@

clean:
	@rm -rf $(BUILD_DIR)

$(foreach ofile,$(OBJ_FILES),$(eval $(call make-goal,$(ofile))))
