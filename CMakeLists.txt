cmake_minimum_required(VERSION 3.21)
project(FunctionalLab11 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/utf-8 /W4 /permissive- /EHsc)
else()
    add_compile_options(-Wall -Wextra)
endif()

function(add_lab target source)
    add_executable(${target} ${source})
endfunction()

add_lab(part1_variant8 part1/part1_variant8.cpp)
add_lab(part2_variant8 part2/part2_variant8.cpp)
add_lab(part3_variant8 part3/part3_variant8.cpp)
add_lab(part4_variant8 part4/part4_variant8.cpp)
add_lab(task1_original part5/task1_original.cpp)
add_lab(task1_fixed    part5/task1_fixed.cpp)
add_lab(task2_lambda_capture part5/task2_lambda_capture.cpp)
add_lab(task3_fixed    part5/task3_fixed.cpp)
add_lab(task4_fixed    part5/task4_fixed.cpp)
add_lab(part6_variant8 part6/part6_variant8.cpp)

# Початкові програми, які НЕ компілюються (завдання 3 і 4 частини 5).
# Вони виключені зі стандартної збірки; щоб побачити повідомлення компілятора:
#   cmake --build build --target task3_broken
#   cmake --build build --target task4_broken
add_executable(task3_broken EXCLUDE_FROM_ALL part5/task3_broken.cpp)
add_executable(task4_broken EXCLUDE_FROM_ALL part5/task4_broken.cpp)
