# 声明最小版本
cmake_minimum_required(VERSION 3.18.1)

# 声明项目名称
project(test)
#[[
################################## 首先说最常见的if else  ##################################
# 在讲if else之前，先看一下变量的使用
# 一般使用set(SET)命令声明一个变量
# 下面声明了一个变量BL 它的值是1
set(BL 1)
# 移除一个变量,这样，这个变量就不能用了
unset(BL)


# 下面这些都是代表true
# true: 1, ON, YES, TRUE, Y,非0值
# 下面这些代表false
# false： 0, OFF， NO， FALSE， N， IGNORE， NOTFOUND

# 比如下面定义一个变量
set(BT FALSE)
set(TEST NO)
if (${BT})
	message("if  BT is true")
elseif(${TEST})
	message("if TEST is true")
else()
	message("else else else")
endif() # 这里是结束，别忘了，其中elsif可以不要，直接结束
]]

####################################### 再说 for循环 ######################################

# 在讲for循环之前还要插入讲一下数组的定义

# 下面定义了一个数组,从1 ～ 10
set(array_list 1 2 3 4 5 6 7 8 9 10)
#[[
# 第一种，常用的
foreach(i ${array_list})
	message(" i = ${i}")
endforeach()
]]

#[[
# 第二种
foreach(i IN LISTS array_list)
	message(" i = ${i}")
endforeach()
]]

#[[
# 第三种 RANGE
foreach(i RANGE 1 10 2) # 后面三个数字的意思是，从1～10的范围，每次步长为2, 打印1,3,5,7,9
	message(" i = ${i}")
endforeach()
]]

#[[
# 第四种 RANGE
foreach(i RANGE 10)    # 如果只有一个数字，那就是从0开始，到10,打印0～10
	message(" i = ${i}")
endforeach()
]]

#[[ 
# 第五种 直接列表型
foreach(i 1 2 3 4 5 6)   # 直接循环 1~6
	message(" i = ${i}")
endforeach()
]]



#################################### 再说 while 循环 ######################################
# 格式:
#while(表达式)
#       COMMAND(ARGS...)
#endwhile(表达式)文章来源地址https://www.yii666.com/blog/366023.html

# 示例
while(NOT a STREQUAL "xxx")
	set(a "${a}x")
	message(">>>>>>a = ${a}")
endwhile()

