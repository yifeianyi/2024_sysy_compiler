#!/bin/bash

CountAC=0
CountWA=0
ACtest=()
# 声明一个函数
assert() {
  # 输入值 为参数1
  input="$1"
  # 读取$input.out的数据作为期望值
  expected=$(cat "2023test/$input.out")
  ./compiler  "2023test/$input.sy" "-S"  "-o" tmp/$input.s 

  # 编译rvcc产生的汇编文件
  if [ ! -d "build" ]; then
      mkdir "build"
  fi
  $RISCV/bin/riscv64-unknown-linux-gnu-gcc -static -o build/$input.out tmp/$input.s

  # 运行生成出来目标文件
  $RISCV/bin/qemu-riscv64 -L $RISCV/sysroot ./build/$input.out

  # 获取程序返回值，存入 实际值
  actual="$?"
  # echo $actual
  # 判断实际值，是否为预期值
  if [ "$actual" = "$expected" ]; then
    echo -e "\e[32m$input         AC \e[0m"
    CountAC=$((CountAC + 1))
    ACtest+=("$input")
  else
    # echo -e "\e[31m$input  Expected:$expected  Actual:$actual           WA \e[0m"
    echo -e "\e[31m$input   WA \e[0m"
    CountWA=$((CountWA + 1))
    # exit 1
  fi
}


# 数组用于存储文件名的前缀字符
# prefixes=()
# # 遍历 test 目录下的所有文件
# for file in 2023test/*; do
#     if [ -f "$file" ]; then
#       filename=$(basename "$file")
#       name="${filename%.*}"
      
#       # 添加前缀字符到数组，并进行去重处理
#       if [[ ! " ${prefixes[@]} " =~ " ${name} " ]]; then
#           prefixes+=("$name")
#       fi
#     fi
# done

# # 遍历数组中的文件名并输出
# for prefix in "${prefixes[@]}"; do
#     # echo "$prefix"
#     assert "$prefix"
# done

# echo -e "\e[32m AC:$CountAC   \e[0m" "\e[31m WA:$CountWA \e[0m"


# for file in "${ACtest[@]}"; do
#     echo -e "\e[32m$file\e[0m"
# done


assert "00_main.sy"
# assert "04_arr_defn3"

# --------- if test -------------
# assert "22_if_test3"
# assert "23_if_test4"
# assert "24_if_test5"

# --------- while-if test -------------
# assert "26_while_test1"
# assert "27_while_test2"
# # assert "28_while_test3"
# assert "31_while_if_test1"
# assert "32_while_if_test2"
# assert "33_while_if_test3"
echo OK
