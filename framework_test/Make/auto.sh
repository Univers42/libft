#!/bin/sh

# Usage: ./auto.sh [directory]
# Default directory is current directory
DIR="${1:-.}"

# Resolve script directory (so Makefile.in/makefile.in is located relative to this script)
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

export CXX=c++
export CXXFLAGS="-Wall -Wextra -Werror"

# Load variables from .env if it exists
if [ -f ./autotools/.env ]; then
	set -a
	. ./autotools/.env
	set +a
	echo "loaded variables from .env"
fi

# Find all .c and .cpp files in the specified directory, print only the filename (no path)
export SRC="$(find "$DIR" -type f \( -name '*.c' -o -name '*.cpp' \) -exec basename {} \; | tr '\n' ' ' | sed 's/ $//')"
echo "Detected source files: $SRC"

# Prefer Makefile.in but accept makefile.in as fallback
if [ -f "$SCRIPT_DIR/Makefile.in" ]; then
	TEMPLATE="$SCRIPT_DIR/Makefile.in"
elif [ -f "$SCRIPT_DIR/makefile.in" ]; then
	TEMPLATE="$SCRIPT_DIR/makefile.in"
else
	echo "Error: neither Makefile.in nor makefile.in found in $SCRIPT_DIR" >&2
	exit 1
fi

# Generate Makefile from template (use envsubst to replace ${SRC})
envsubst '${SRC}' < "$TEMPLATE" > "$DIR/Makefile"
echo "Generated Makefile at $DIR/Makefile"
