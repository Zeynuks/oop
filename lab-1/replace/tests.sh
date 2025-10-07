#!/bin/bash

PROGRAM="./cmake-build-debug/replace"

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

run_test() {
    local description="$1"
    local command="$2"
    local expected_output="$3"
    local expected_error="$4"
    local input_file_content="$5"

    if [[ "$command" == *"<file>"* ]]; then
        local temp_file
        temp_file=$(create_temp_file "$input_file_content")
        command=${command//<file>/$temp_file}
    fi

    local output
    output=$(eval "$command" 2>&1)
    local exit_code=$?

    local output_match=1
    local error_match=1

    if [ -n "$expected_output" ]; then
        if ! diff -q <(echo -e "$expected_output") <(echo -e "$output") >/dev/null; then
            output_match=0
        fi
    fi

    if [ -n "$expected_error" ]; then
        if ! echo "$output" | grep -q "$expected_error"; then
            error_match=0
        fi
    fi

    if [ $exit_code -ne 0 ] && [ -z "$expected_error" ]; then
        printf "Test %2d: %-50s [${ERROR_COLOR}ERROR${DEFAULT_COLOR}]\n" $TEST_NUM "$description"
        printf "Command failed with exit code %d:\n%s\n" $exit_code "$command"
        ((FAILED++))
    elif [ $output_match -eq 1 ] && [ $error_match -eq 1 ]; then
        printf "Test %2d: %-50s [${PASSED_COLOR}PASSED${DEFAULT_COLOR}]\n" $TEST_NUM "$description"
        ((PASSED++))
    else
        printf "Test %2d: %-50s [${ERROR_COLOR}FAILED${DEFAULT_COLOR}]\n" $TEST_NUM "$description"
        printf "${PASSED_COLOR}Command:${DEFAULT_COLOR} %s\n" "$command"
        if [ $output_match -eq 0 ]; then
            echo -e "Expected output:\n$expected_output"
            echo -e "Actual output:\n$output"
        fi
        if [ $error_match -eq 0 ]; then
            echo -e "Expected error: $expected_error"
            echo -e "Actual output:\n$output"
        fi
        ((FAILED++))
    fi

    ((TEST_NUM++))
    [ -n "$temp_file" ] && rm -f "$temp_file"
}

run_test "Invalid arguments (3 args)" \
         "$PROGRAM a b c" \
         "" \
         "ERROR: Invalid arguments. Use -h | --help for usage info."

run_test "Invalid arguments (6 args)" \
         "$PROGRAM a b c d e f" \
         "" \
         "ERROR: Invalid arguments. Use -h | --help for usage info."

run_test "File mode (normal replacement)" \
         "$PROGRAM <file> output.txt old new" \
         "" \
         "" \
         "old1 old2 old3"
[ "$(cat output.txt)" = "new1 new2 new3" ] && rm output.txt || FAILED=$((FAILED+1))

run_test "File mode (empty search string)" \
         "$PROGRAM <file> output.txt '' new" \
         "" \
         "" \
         "old1 old2 old3"
[ "$(cat output.txt)" = "old1 old2 old3" ] && rm output.txt || FAILED=$((FAILED+1))

run_test "File mode (empty replace string)" \
         "$PROGRAM <file> output.txt old ''" \
         "" \
         "" \
         "old1 old2 old3"
[ "$(cat output.txt)" = "1 2 3" ] && rm output.txt || FAILED=$((FAILED+1))

run_test "File mode (nonexistent input)" \
         "$PROGRAM nofile.txt output.txt a b" \
         "" \
         "ERROR: Failed to open input file: nofile.txt"

run_test "File mode (empty file)" \
         "$PROGRAM <file> output.txt a b" \
         "" \
         "" \
         ""
[ -f output.txt ] && rm output.txt

run_test "Stdin mode (normal)" \
         "printf 'search\nreplace\nline with search\n' | $PROGRAM" \
         "line with replace" \
         ""

run_test "Stdin mode (empty input line)" \
         "printf 'a\nb\n\n' | $PROGRAM" \
         "" \
         "ERROR: Replacement string is empty"

run_test "Stdin mode (empty replace)" \
         "printf 'search\n\nline with search\n' | $PROGRAM" \
         "line with " \
         ""

run_test "Stdin mode (empty search)" \
         "printf '\nreplace\nline\n' | $PROGRAM" \
         "line" \
         ""

HELP_TEXT=$'Usage:\n  replace <inputFile> <outputFile> <searchString> <replacementString>\n  replace               # interactive stdin mode\n  replace -h | --help   # show this help message'

run_test "Help mode (-h)" \
         "$PROGRAM -h" \
         "$HELP_TEXT" \
         ""

run_test "Help mode (--help)" \
         "$PROGRAM --help" \
         "$HELP_TEXT" \
         ""

echo "Tests passed: $PASSED"
echo "Tests failed: $FAILED"
exit $FAILED
