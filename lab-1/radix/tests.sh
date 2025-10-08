PASSED=0
FAILED=0
TEST_NUM=1
ERROR_COLOR="\033[31m"
PASSED_COLOR="\033[32m"
DEFAULT_COLOR="\033[0m"

run_test() {
    local description="$1"
    local command="$2"
    local expected_output="$3"
    local expected_error="$4"

    local output
    local error
    output=$(eval "$command" 2>&1)
    local exit_code=$?

    if [ $exit_code -ne 0 ] && [ -z "$expected_error" ]; then
        printf "Test %2d: %-50s [${ERROR_COLOR}ERROR${DEFAULT_COLOR}]\n" $TEST_NUM "$description"
        printf "Command failed with exit code %d:\n%s\n" $exit_code "$command"
        ((FAILED++))
        ((TEST_NUM++))
        [ -n "$temp_file" ] && rm -f "$temp_file"
        return
    fi

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

    if [ $output_match -eq 1 ] && [ $error_match -eq 1 ]; then
        local result="PASSED"
        local color=$PASSED_COLOR
        ((PASSED++))
    else
        local result="FAILED"
        local color=$ERROR_COLOR
        ((FAILED++))
    fi

    printf "Test %2d: %-50s [${color}%s${DEFAULT_COLOR}]\n" $TEST_NUM "$description" "$result"

    if [ "$result" = "FAILED" ]; then
        printf "${PASSED_COLOR}Command:${DEFAULT_COLOR} %s\n" "$command"
        if [ $output_match -eq 0 ] && [ $error_match -eq 1 ]; then
            echo "Expected output: $(echo -e "$expected_output")"
            echo "Actual output: $(echo -e "$output")"
        else
            echo "Expected ERROR: $expected_error"
            echo "Actual ERROR: $(echo -e "$output")"
        fi
    fi

    ((TEST_NUM++))
}

run_test "Convert hex to decimal (1F -> 31)" \
    "./cmake-build-debug/radix 16 10 1F" \
    "31" \
    "" \
    0

run_test "Convert binary to decimal (1010 -> 10)" \
    "./cmake-build-debug/radix 2 10 1010" \
    "10" \
    "" \
    0

run_test "Convert decimal to hex (255 -> FF)" \
    "./cmake-build-debug/radix 10 16 255" \
    "FF" \
    "" \
    0

run_test "Convert negative decimal to binary (-10 -> -1010)" \
    "./cmake-build-debug/radix 10 2 -10" \
    "-1010" \
    "" \
    0

run_test "Convert zero from decimal to binary (0 -> 0)" \
    "./cmake-build-debug/radix 10 2 0" \
    "0" \
    "" \
    0

run_test "Convert max int value to hex" \
    "./cmake-build-debug/radix 10 16 2147483647" \
    "7FFFFFFF" \
    "" \
    0

run_test "Convert min int value to hex" \
    "./cmake-build-debug/radix 10 16 -2147483648" \
    "-80000000" \
    "" \
    0

run_test "Invalid source radix (greater than 36)" \
    "./cmake-build-debug/radix 37 10 10" \
    "" \
    "ERROR: invalid radix" \
    1

run_test "Invalid destination radix (less than 2)" \
    "./cmake-build-debug/radix 10 1 10" \
    "" \
    "ERROR: invalid radix" \
    1

run_test "Invalid character for source radix" \
    "./cmake-build-debug/radix 10 2 1F" \
    "" \
    "ERROR: invalid character in input" \
    1

run_test "Too few arguments" \
    "./cmake-build-debug/radix 10 2" \
    "" \
    "Usage: radix.exe <source notation> <destination notation> <value>" \
    1

run_test "Too many arguments" \
    "./cmake-build-debug/radix 10 2 255 extra" \
    "" \
    "Usage: radix.exe <source notation> <destination notation> <value>" \
    1

run_test "Convert base 36 to decimal (Z -> 35)" \
    "./cmake-build-debug/radix 36 10 Z" \
    "35" \
    "" \
    0

run_test "Convert decimal to base 36 (35 -> Z)" \
    "./cmake-build-debug/radix 10 36 35" \
    "Z" \
    "" \
    0

run_test "Convert octal to decimal (17 -> 15)" \
    "./cmake-build-debug/radix 8 10 17" \
    "15" \
    "" \
    0

run_test "Convert base 11 to decimal (A -> 10)" \
    "./cmake-build-debug/radix 11 10 A" \
    "10" \
    "" \
    0

run_test "Convert lowercase hex to decimal (1f -> 31)" \
    "./cmake-build-debug/radix 16 10 1f" \
    "31" \
    "" \
    0

run_test "Zero value (base 10 to binary)" \
    "./cmake-build-debug/radix 10 2 0" \
    "0" \
    "" \
    0

run_test "Convert negative hex to decimal (-FF -> -255)" \
    "./cmake-build-debug/radix 16 10 -FF" \
    "-255" \
    "" \
    0

run_test "Overflow when converting to integer" \
    "./cmake-build-debug/radix 10 2 9999999999999999999999999999" \
    "" \
    "ERROR: value out of range for int" \
    1

run_test "Empty input value" \
    "./cmake-build-debug/radix 10 2 \"\"" \
    "" \
    "ERROR: empty string" \
    1

run_test "Convert large number (overflow)" \
    "./cmake-build-debug/radix 10 2 2147483648" \
    "" \
    "ERROR: value out of range for int" \
    1

run_test "Convert large negative number (overflow)" \
    "./cmake-build-debug/radix 10 2 -2147483649" \
    "" \
    "ERROR: value out of range for int" \
    1

echo "Tests passed: $PASSED"
echo "Tests failed: $FAILED"

exit $FAILED