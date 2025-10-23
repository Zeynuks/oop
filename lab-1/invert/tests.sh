#!/bin/bash

PROGRAM="./cmake-build-debug/invert"

PASSED=0
FAILED=0
TEST_NUM=1
ERROR_COLOR="\033[31m"
PASSED_COLOR="\033[32m"
DEFAULT_COLOR="\033[0m"

create_temp_file() {
    local content="$1"
    local temp_file
    temp_file=$(mktemp)
    echo -e "$content" > "$temp_file"
    echo "$temp_file"
}

# normalize whitespace: convert tabs/newlines/multiple spaces -> single space, trim edges
normalize() {
    sed -E 's/[[:space:]]+/ /g' | sed -E 's/^ //; s/ $//'
}

run_test() {
    local description="$1"
    local command_template="$2"
    local expected_output_raw="$3"
    local expected_error_substr="$4"
    local file_content="$5"

    unset temp_file
    if [[ "$command_template" == *"<file>"* ]]; then
        temp_file=$(create_temp_file "$file_content")
        command=${command_template//<file>/$temp_file}
    else
        command="$command_template"
    fi

    # run command, capture stdout+stderr combined
    output=$(eval "$command" 2>&1)
    exit_code=$?

    output_norm=$(printf "%s" "$output" | normalize)
    expected_output_norm=$(printf "%s" "$expected_output_raw" | normalize)

    output_match=1
    error_match=1

    if [ -n "$expected_output_raw" ]; then
        if [ "$output_norm" != "$expected_output_norm" ]; then
            output_match=0
        fi
    fi

    if [ -n "$expected_error_substr" ]; then
        if ! printf "%s" "$output" | grep -q -- "$expected_error_substr"; then
            error_match=0
        fi
    fi

    if [ $exit_code -ne 0 ] && [ -z "$expected_error_substr" ]; then
        printf "Test %2d: %-50s [${ERROR_COLOR}ERROR${DEFAULT_COLOR}]\n" $TEST_NUM "$description"
        printf "Command failed with exit code %d:\n%s\n" $exit_code "$command"
        printf "Captured output:\n%s\n" "$output"
        ((FAILED++))
    elif [ $output_match -eq 1 ] && [ $error_match -eq 1 ]; then
        printf "Test %2d: %-50s [${PASSED_COLOR}PASSED${DEFAULT_COLOR}]\n" $TEST_NUM "$description"
        ((PASSED++))
    else
        printf "Test %2d: %-50s [${ERROR_COLOR}FAILED${DEFAULT_COLOR}]\n" $TEST_NUM "$description"
        printf "${PASSED_COLOR}Command:${DEFAULT_COLOR} %s\n" "$command"
        if [ $output_match -eq 0 ]; then
            echo -e "Expected (normalized):\n$expected_output_norm"
            echo -e "Actual (normalized):\n$output_norm"
        fi
        if [ $error_match -eq 0 ]; then
            echo -e "Expected error substring: $expected_error_substr"
            echo -e "Actual output:\n$output"
        fi
        ((FAILED++))
    fi

    ((TEST_NUM++))
    [ -n "$temp_file" ] && rm -f "$temp_file"
}

#
# Tests
#

# NOTE:
# - New program prints matrix elements using operator<< (fixed, precision 3) with spacing,
#   so we normalize whitespace before comparison.
# - DefineAppMode in the provided code treats argc==5 as "File" mode, but ProcessFile uses argv[1] as filename.
#   Therefore we call the program with 4 dummy args + filename to trigger "File" mode:
#     ./invert a b c <file>
#

run_test "Reading matrix from stdin" \
         "echo -e $'1\t2\t3\n0\t1\t4\n5\t6\t0' | $PROGRAM" \
         $' -24.000   18.000    5.000\n 20.000  -15.000   -4.000\n -5.000    4.000    1.000' \
         ""

run_test "Singular matrix (det=0)" \
         "echo -e $'1\t2\t3\n2\t4\t6\n1\t2\t3' | $PROGRAM" \
         "" \
         "singular"  # ищем подстроку 'singular' в сообщении об ошибке

run_test "Not enough numbers in matrix (invalid input)" \
         "echo -e $'1\t2\t3\n4\t5\n6\t7\t8' | $PROGRAM" \
         "" \
         "Invalid"   # ожидаем любую строку с "Invalid" (operator>> бросает "Invalid input for matrix element")

run_test "Help output" \
         "$PROGRAM -h" \
         $'Usage: invert [option] [file]\nOptions:\n  -h            Show this help message\nIf no file is specified, input is expected from stdin.' \
         ""

run_test "Too many arguments" \
         "$PROGRAM arg1 arg2 arg3 arg4 arg5" \
         "" \
         "Invalid arguments. Use -h | --help"

run_test "Zero matrix (all zeros) -> singular" \
         "echo -e $'0\t0\t0\n0\t0\t0\n0\t0\t0' | $PROGRAM" \
         "" \
         "singular"

run_test "Matrix with negative numbers (possibly singular)" \
         "echo -e $'1\t1\t0\n1\t1\t0\n0\t0\t2' | $PROGRAM" \
         "" \
         "singular"

echo "Tests passed: $PASSED"
echo "Tests failed: $FAILED"

exit $FAILED
