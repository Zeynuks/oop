#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROGRAM="$SCRIPT_DIR/../../cmake-build-debug/lab-1/crypt/crypt"

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
    echo -n "$content" > "$temp_file"
    echo "$temp_file"
}

normalize() {
    sed -E 's/[[:space:]]+/ /g' | sed -E 's/^ //; s/ $//'
}

assertFilesEqual() {
    local file1="$1"
    local file2="$2"
    if ! cmp -s "$file1" "$file2"; then
        echo -e "${ERROR_COLOR}Files differ:${DEFAULT_COLOR} $file1 != $file2"
        return 1
    fi
    return 0
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

# Ключи
VALID_KEY=42
MAX_KEY=255
MIN_KEY=0
TOO_LARGE_KEY=256
TOO_SMALL_KEY=-1
INVALID_KEY="abc"

run_test "Non-existent input file" \
         "$PROGRAM crypt /tmp/nonexist_file output.tmp $VALID_KEY" \
         "" \
         "Не удалось открыть файл"

run_test "Wrong number of arguments" \
         "$PROGRAM crypt" \
         "" \
         "Программа ожидает"

temp_file=$(create_temp_file "Hello")
run_test "Invalid key format" \
         "$PROGRAM crypt $temp_file output.tmp $INVALID_KEY" \
         "" \
         "stoi"

temp_file=$(create_temp_file "Hello")
run_test "Key too large" \
         "$PROGRAM crypt $temp_file output.tmp $TOO_LARGE_KEY" \
         "" \
         "Ключ должен быть от 0 до 255"

temp_file=$(create_temp_file "Hello")
run_test "Key too small" \
         "$PROGRAM crypt $temp_file output.tmp $TOO_SMALL_KEY" \
         "" \
         "Ключ должен быть от 0 до 255"

temp_file=$(create_temp_file "Hello")
run_test "Invalid mode" \
         "$PROGRAM invalid_mode $temp_file output.tmp $VALID_KEY" \
         "" \
         "Некорректный режим"

plain_file=$(create_temp_file "Hello, world!")
crypted_file=$(mktemp)
decrypted_file=$(mktemp)

run_test "Encrypt file with valid key" \
         "$PROGRAM crypt $plain_file $crypted_file $VALID_KEY" \
         "" \
         ""

run_test "Decrypt file with valid key" \
         "$PROGRAM decrypt $crypted_file $decrypted_file $VALID_KEY" \
         "" \
         ""

assertFilesEqual "$plain_file" "$decrypted_file"

plain_file=$(create_temp_file "Test string")
crypted_file_min=$(mktemp)
decrypted_file_min=$(mktemp)

"$PROGRAM" crypt "$plain_file" "$crypted_file_min" "$MIN_KEY"
"$PROGRAM" decrypt "$crypted_file_min" "$decrypted_file_min" "$MIN_KEY"
assertFilesEqual "$plain_file" "$decrypted_file_min"

crypted_file_max=$(mktemp)
decrypted_file_max=$(mktemp)

"$PROGRAM" crypt "$plain_file" "$crypted_file_max" "$MAX_KEY"
"$PROGRAM" decrypt "$crypted_file_max" "$decrypted_file_max" "$MAX_KEY"
assertFilesEqual "$plain_file" "$decrypted_file_max"

empty_file=$(create_temp_file "")
crypted_empty=$(mktemp)
decrypted_empty=$(mktemp)

"$PROGRAM" crypt "$empty_file" "$crypted_empty" "$VALID_KEY"
"$PROGRAM" decrypt "$crypted_empty" "$decrypted_empty" "$VALID_KEY"
assertFilesEqual "$empty_file" "$decrypted_empty"

multi_file=$(create_temp_file $'Line1\nLine2\nLine3')
crypted_multi=$(mktemp)
decrypted_multi=$(mktemp)

"$PROGRAM" crypt "$multi_file" "$crypted_multi" "$VALID_KEY"
"$PROGRAM" decrypt "$crypted_multi" "$decrypted_multi" "$VALID_KEY"
assertFilesEqual "$multi_file" "$decrypted_multi"

echo "Tests passed: $PASSED"
echo "Tests failed: $FAILED"
exit $FAILED
