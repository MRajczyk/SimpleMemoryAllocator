#
# DANTE :: System Automatycznych Testów Jednostkowych oraz Akceptacyjnych
# Tomasz Jaworski, 2018-2020
#
# Plik wygenerowany automatycznie
# Znacznik czasowy: 2021-11-22 22:52:17.242471
#

cmake_minimum_required(VERSION 3.17)
project(project1 C)

# Przyjmij standard C11
set(CMAKE_C_STANDARD 11)

# Ustaw opcje kompilatora (z raportu Dante)
add_compile_options(
      "-D_ANSI_OUTPUT"
      "-fmax-errors=5"
      "-Wno-parentheses"
      "-Wextra"
      "-Werror"
      "-fdiagnostics-color"
      "-xc"
      "-Wall"
      "-Wno-error=implicit-fallthrough"
      "-D_GNU_SOURCE"
      "-Wno-error=unused-parameter"
      "-std=c11"
      "-pedantic"
      "-Wno-error=parentheses"
      "-DINSIDE_DANTE"
      "-ggdb3"
      "-Werror=vla"

)

# Ustaw opcje konsolidatora (z raportu Dante)
add_link_options(
        "-ggdb3"
        "-Wl,-wrap,main"
        "-Wl,-cref"
        "-Wl,-Map=main.map"
)

# Utwórz target 'project1' i dodaj do niego pliki źródlowe:
add_executable(project1
        "1_9.c"
        "heap.c"
        "unit_helper_v2.c"
        "unit_test_v2.c"
        "rdebug.c"
        "memmanager.c"
)

# Dołącz niezbędne biblioteki
target_link_libraries(project1
        "pthread"
        "m"
)