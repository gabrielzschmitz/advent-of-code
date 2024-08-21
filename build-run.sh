#!/bin/sh

# Usage: ./build-run.sh <year> <day> <part> <input|test>
# Example: ./build-run.sh 2023 24 1 test

# Check for the correct number of arguments
if [ "$#" -ne 4 ]; then
    echo "Usage: $0 <year> <day> <part> <input|test>"
    exit 1
fi

YEAR=$1
DAY=$2
PART=$3
FILE_TYPE=$4

# Validate inputs
if ! [[ "$YEAR" =~ ^[0-9]{4}$ ]]; then
    echo "Invalid year. Must be a 4-digit number."
    exit 1
fi

if ! [[ "$DAY" =~ ^[0-9]{2}$ ]]; then
    echo "Invalid day. Must be a 2-digit number."
    exit 1
fi

if ! [[ "$PART" =~ ^[1-2]$ ]]; then
    echo "Invalid part. Must be 1 or 2."
    exit 1
fi

if [[ "$FILE_TYPE" != "input" && "$FILE_TYPE" != "test" ]]; then
    echo "Invalid file type. Must be 'input' or 'test'."
    exit 1
fi

# Set directories and files
DAY_DIR="${YEAR}/day${DAY}"
BUILD_DIR="${DAY_DIR}/build"
SOURCE_FILE="${DAY_DIR}/part${PART}.cpp"
OUTPUT_FILE="${BUILD_DIR}/part${PART}"

# Create build directory if it does not exist
mkdir -p "$BUILD_DIR"

# Compile the source file
g++ "$SOURCE_FILE" -o "$OUTPUT_FILE"

# Check if the build was successful
if [ $? -ne 0 ]; then
    echo "Build failed."
    exit 1
fi

# Run the application with the specified input or test file
INPUT_FILE="${DAY_DIR}/${FILE_TYPE}"
if [ ! -f "$INPUT_FILE" ]; then
    echo "Input file or test file not found: $INPUT_FILE"
    exit 1
fi

"$OUTPUT_FILE" "$INPUT_FILE"
