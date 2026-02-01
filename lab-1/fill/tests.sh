#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROGRAM="$SCRIPT_DIR/../../cmake-build-debug/lab-1/fill/fill"

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
    local file_content="$4"

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

    if [ "$output_norm" == "$expected_output_norm" ]; then
        printf "Test %2d: %-50s [${PASSED_COLOR}PASSED${DEFAULT_COLOR}]\n" $TEST_NUM "$description"
        ((PASSED++))
    else
        printf "Test %2d: %-50s [${ERROR_COLOR}FAILED${DEFAULT_COLOR}]\n" $TEST_NUM "$description"
        echo "Expected:"
        echo "$expected_output_raw"
        echo "Got:"
        echo "$output"
        ((FAILED++))
    fi

    ((TEST_NUM++))
    [ -n "$temp_file" ] && rm -f "$temp_file"
}

input1=$'

        ###############
        ########     ###
   ######          ####
#####      O     ####
####################'
expected1=$'

        ###############
        ########.....###
   ######..........####
#####......O.....####
####################'

run_test "Small fill example 1" \
         "$PROGRAM <file> /dev/stdout" \
         "$expected1" \
         "$input1"

input2=$'###############################
         #                             #
         #                             ##
         #     O                         #
         #                              #
         #                              #
         #                              #
         #              O               #
         ################################'
expected2=$'###############################
            #.............................#
            #.............................##
            #.....O.........................#
            #..............................#
            #..............................#
            #..............................#
            #..............O...............#
            ################################'

run_test "Multiple fill points example 2" \
         "$PROGRAM <file> /dev/stdout" \
         "$expected2" \
         "$input2"

input3=$'    ####################
            #                   #
            #       ####       #
            #    O  #  #       ###
            #       #  #         #
             ########  ###########

                               ###
                  #######      # #
                  #     #      #  ##
                  #  O  #     #     #
                  #     #     ######
                  #######'
expected3=$'    ####################
   #...................#
   #.......####.......#
   #....O..#  #.......###
   #.......#  #.........#
    ########  ###########

                      ###
         #######      # #
         #.....#      #  ##
         #..O..#     #     #
         #.....#     ######
         #######'

run_test "Complex fill example 3" \
         "$PROGRAM <file> /dev/stdout" \
         "$expected3" \
         "$input3"

input4=""
expected4=""

run_test "Empty grid" \
         "$PROGRAM <file> /dev/stdout" \
         "$expected4" \
         "$input4"

input5=$'#####\n#####\n#####'
expected5=$'#####\n#####\n#####'

run_test "Walls only grid" \
         "$PROGRAM <file> /dev/stdout" \
         "$expected5" \
         "$input5"

input6=$'     \n  O  '
expected6=$'     \n  O '

run_test "O surrounded by walls / out of bounds" \
         "$PROGRAM <file> /dev/stdout" \
         "$expected6" \
         "$input6"

echo "Tests passed: $PASSED"
echo "Tests failed: $FAILED"
exit $FAILED
