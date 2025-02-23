git clone https://github.com/google/googletest.git
cd googletest
mkdir build && cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
# cat compile_commands.json 


# Create a tidy
cat > "../.clang-tidy" <<EOF
Checks: >
  -*,
  modernize-*,
  readability-*,
  performance-*,
  bugprone-*

WarningsAsErrors: 'bugprone-*'
FormatStyle: file

EOF


clang-tidy ../googletest/src/gtest-all.cc --fix
find ../googletest/src -name '*.cc' | xargs -n1 clang-tidy --fix


# # Add these lines to enable clang-tidy integration
# find_program(CLANG_TIDY clang-tidy)
# if(CLANG_TIDY)
#     message(STATUS "Clang-Tidy found: ${CLANG_TIDY}")
#     set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY};-checks=modernize-*,-modernize-use-trailing-return-type;--fix")
# endif()