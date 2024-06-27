SRC_DIR := src
INC_DIR := inc
OBJ_DIR := build
TMP_DIR := tmp

# 使用g++作为编译器来编译C和C++代码
CXX := clang++
CFLAGS := -I$(INC_DIR) -g #-Wall 
CXXFLAGS := $(CFLAGS) # -std=c++11 # 为C++代码添加额外的编译选项

# 找到C和C++的源文件
C_SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
CPP_SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)

# 创建对象文件路径
C_OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SRC_FILES))
CPP_OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_SRC_FILES))
OBJ_FILES := $(C_OBJ_FILES) $(CPP_OBJ_FILES)

$(shell mkdir -p $(OBJ_DIR))

TARGET := compiler

default: $(TARGET)

# 编译C源文件
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CXX) $(CFLAGS) -c $< -o $@

# 编译C++源文件
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJ_FILES)
	@$(CXX) $(CXXFLAGS) $^ -o $@

.PHONY: clean test count

# test: $(TARGET) 
# 	./test.sh
tname ?= 00_main
test := 2023test/$(tname)

run: $(TARGET)
	./compiler -S  -o tmp/$(tname).S $(test).sy

clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET) build obj *.s *.S *.out tmp/*

count:
	@find src inc -type f | xargs wc -l

sycount:
# @find 2023 -type f | xargs wc -l
	find 2023test -type f -name "*.sy" | wc -l


