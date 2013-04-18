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

OBJ_FILES 	:= $(patsubst $(SRC_DIR)%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))
SHARED_OBJ 	:= $(addprefix $(BUILD_DIR)/shared/,$(notdir $(SHARED_SRC:.cpp=.o)))
CLIENT_OBJ 	:= $(addprefix $(BUILD_DIR)/client/,$(notdir $(CLIENT_SRC:.cpp=.o)))
SERVER_OBJ 	:= $(addprefix $(BUILD_DIR)/server/,$(notdir $(SERVER_SRC:.cpp=.o)))


OBJ       := $(patsubst src/%.cpp,build/%.o,$(SRC))
INCLUDES  := $(addprefix -I ,$(SRC_DIR))

define make-goal
$1/%.o: %.cpp
	$(CC) $(INCLUDES) -c $$< -o $$@
endef

.PHONY: build/shared build/client build/server checkdirs

all: checkdirs crush.exe server.exe


#@echo $(SHARED_OBJ)
#@echo $(SHARED_SRC)

build/shared: $(SHARED_OBJ)
	@mkdir -p $@
	$(eval $(foreach file,$?, $(call make-goal,$file)))

build/client: $(SHARED_SRC) $(CLIENT_FILES)
	@mkdir -p $@
	$(CC) $(INCLUDES) -c $?

build/server: $(SHARED_SRC)
	$(SERVER_FILES)
	@mkdir -p $@
	$(CC) $(INCLUDES) -c $?

crush.exe: $(SHARED_OBJ) $(CLIENT_OBJ)
	$(LD) $^ -o $@

server.exe: $(SHARED_OBJ) $(SERVER_OBJ)
	$(LD) $^ -o $@



checkdirs: $(BUILD_DIRS)

$(BUILD_DIRS):
	@mkdir -p $@

clean:
	@rm -rf $(BUILD_DIR)

$(foreach ofile,$(OBJ_FILES),$(eval $(call make-goal,$(ofile))))
