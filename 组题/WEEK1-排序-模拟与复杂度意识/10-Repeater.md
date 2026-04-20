> 训练周次：WEEK1 排序、模拟与复杂度意识（4.27-5.3）
> 原题来源：北京大学 考研 038
> 原题路径：`schools/北京大学/038-Repeater.md`
> 难度 / 范围：中等 / 递归,模拟

# Repeater

#### 题目描述

Harmony is indispensible in our daily life and no one can live without it----may be Facer is the only exception. One day it is rumored that repeat painting will create harmony and then hundreds of people started their endless drawing. Their paintings were based on a small template and a simple method of duplicating. Though Facer can easily imagine the style of the whole picture, but he cannot find the essential harmony. Now you need to help Facer by showing the picture on computer.

You will be given a template containing only one kind of character and spaces, and the template shows how the endless picture is created----use the characters as basic elements and put them in the right position to form a bigger template, and then repeat and repeat doing that. Here is an example.

```

# #

 #      <-template

# #

```

So the $ Level \ 1 $ picture will be

```

# #

 #

# #

```

$ Level \ 2 $ picture will be

```

# #     # #

 #       #

# #     # #

    # #  

     #   

    # #  

# #    # #

 #      #

# #    # #

```

#### 输入格式

The input contains multiple test cases.

The first line of each case is an integer $ N $, representing the size of the template is $ N \times N $ ($ N $ could only be $ 3 $, $ 4 $ or $ 5 $).

Next $ N $ lines describe the template.

The following line contains an integer $ Q $, which is the $ Scale \ Level $ of the picture.

Input is ended with a case of $ N = 0 $.

It is guaranteed that the size of one picture will not exceed $ 3000 \times 3000 $.

#### 输出格式

For each test case, just print the $ Level \ Q $ picture by using the given template.

#### 输入样例
```
3
# #
 #
# #
1
4
 OO
O  O
O  O
 OO
2
0
```

#### 输出样例
```
# #
 # 
# #
     OO  OO     
    O  OO  O    
    O  OO  O    
     OO  OO     
 OO          OO 
O  O        O  O
O  O        O  O
 OO          OO 
 OO          OO 
O  O        O  O
O  O        O  O
 OO          OO 
     OO  OO     
    O  OO  O    
    O  OO  O    
     OO  OO
```
