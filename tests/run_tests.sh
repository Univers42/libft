#!/usr/bin/env bash
# ===========================================================================
# libft test orchestrator: builds libft TWICE (libc + ft_malloc), compiles
# every unit test against both, then for each test asserts:
#   1. exit code 0 in the libc build   (all checks passed)
#   2. exit code 0 in the ft build      (all checks passed AND 0 bytes leaked)
#   3. libc stdout == ft stdout         (functional equivalence: same results)
#   4. valgrind clean on the libc build (libc-side leak/UB oracle)
# Usage:  tests/run_tests.sh <all | test_name>
# ===========================================================================
set -u

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT"

SEL="${1:-all}"
UNIT_DIR="tests/units"
HARNESS="tests/harness"
BIN_DIR="build/bin/test"
OUT_DIR="build/test/out"
FT_LIB="build/test/ft/lib/libft.a"
LIBC_LIB="build/test/libc/lib/libft.a"
FTM_INC="srcs/memory/ft_malloc/include"

CC=cc
WARN="-Wall -Wextra"
BASEFLAGS="-DXALLOC_NO_CONFIG -I. -Iinclude -I$HARNESS -g"

RED=$'\033[1;31m'; GRN=$'\033[1;32m'; YEL=$'\033[1;33m'; CYN=$'\033[1;36m'; RST=$'\033[0m'

mkdir -p "$BIN_DIR" "$OUT_DIR"

# ---- pick tests --------------------------------------------------------------
if [ "$SEL" = "all" ]; then
	mapfile -t TESTS < <(find "$UNIT_DIR" -name 'test_*.c' | sort)
else
	f="$UNIT_DIR/${SEL%.c}.c"
	[ -f "$f" ] || { echo "${RED}no such test: $SEL${RST} (looked for $f)"; exit 2; }
	TESTS=("$f")
fi
[ "${#TESTS[@]}" -gt 0 ] || { echo "${RED}no tests found in $UNIT_DIR${RST}"; exit 2; }

have_vg=0; command -v valgrind >/dev/null 2>&1 && have_vg=1

# Run a freshly-built binary, retrying transient exec failures. rc 126/127 =
# ETXTBSY / "text file busy": some linkers briefly keep the output file open, so
# exec'ing it immediately can race. We also build via temp+mv (below) to avoid
# it, but retry here as belt-and-suspenders. Returns the program's real rc.
run_retry() { # $1=binary $2=stdout-file $3=stderr-file
	local rc tries=0
	while :; do
		"$1" >"$2" 2>"$3"; rc=$?
		if { [ $rc -eq 126 ] || [ $rc -eq 127 ]; } && [ $tries -lt 8 ]; then
			tries=$((tries + 1)); sleep 0.15; continue
		fi
		break
	done
	return $rc
}

# ---- build both libft archives (cached; incremental on re-runs) --------------
echo "${CYN}▸ building libft (ft_malloc)…${RST}"
make -s BUILD_DIR=build/test/ft   EXTRA_CFLAGS=-DXALLOC_NO_CONFIG "$FT_LIB"   || { echo "${RED}ft archive build failed${RST}"; exit 3; }
echo "${CYN}▸ building libft (libc)…${RST}"
make -s BUILD_DIR=build/test/libc FTM_SRCS= EXTRA_CFLAGS=-DXALLOC_NO_CONFIG "$LIBC_LIB" || { echo "${RED}libc archive build failed${RST}"; exit 3; }

pass=0; fail=0; FAILED=()

printf '\n%-22s %-7s %-7s %-7s %-9s\n' "TEST" "libc" "ft" "diff" "valgrind"
printf '%.0s─' {1..58}; echo

for src in "${TESTS[@]}"; do
	name="$(basename "${src%.c}")"
	bl="$BIN_DIR/${name}_libc"
	bf="$BIN_DIR/${name}_ft"
	st="ok"

	# compile both
	if ! $CC $WARN $BASEFLAGS "$src" "$LIBC_LIB" -lm -o "$bl" 2>"$OUT_DIR/$name.cc.libc"; then
		printf '%-22s %sCOMPILE-ERR (libc)%s\n' "$name" "$RED" "$RST"; FAILED+=("$name"); fail=$((fail+1)); continue
	fi
	if ! $CC $WARN $BASEFLAGS -DHAVE_FT_MALLOC -I"$FTM_INC" "$src" "$FT_LIB" -pthread -lm -o "$bf" 2>"$OUT_DIR/$name.cc.ft"; then
		printf '%-22s %sCOMPILE-ERR (ft)%s\n' "$name" "$RED" "$RST"; FAILED+=("$name"); fail=$((fail+1)); continue
	fi

	# run libc
	"$bl" >"$OUT_DIR/$name.libc.out" 2>"$OUT_DIR/$name.libc.err"; rcl=$?
	[ $rcl -eq 0 ] && cL="${GRN}PASS${RST}" || { cL="${RED}rc=$rcl${RST}"; st="bad"; }

	# run ft
	"$bf" >"$OUT_DIR/$name.ft.out" 2>"$OUT_DIR/$name.ft.err"; rcf=$?
	[ $rcf -eq 0 ] && cF="${GRN}PASS${RST}" || { cF="${RED}rc=$rcf${RST}"; st="bad"; }

	# diff logical output
	if diff -q "$OUT_DIR/$name.libc.out" "$OUT_DIR/$name.ft.out" >/dev/null; then
		cD="${GRN}same${RST}"
	else
		cD="${RED}DIFF${RST}"; st="bad"
	fi

	# valgrind on the libc build (authoritative libc-side leak/UB oracle)
	if [ $have_vg -eq 1 ]; then
		if valgrind --quiet --error-exitcode=77 --leak-check=full \
			--errors-for-leak-kinds=all "$bl" >/dev/null 2>"$OUT_DIR/$name.vg"; then
			cV="${GRN}clean${RST}"
		else
			cV="${RED}LEAK/UB${RST}"; st="bad"
		fi
	else
		cV="${YEL}skip${RST}"
	fi

	printf '%-22s %-16b %-16b %-16b %-18b\n' "$name" "$cL" "$cF" "$cD" "$cV"
	if [ "$st" = "ok" ]; then pass=$((pass+1)); else FAILED+=("$name"); fail=$((fail+1)); fi
done

printf '%.0s─' {1..58}; echo
if [ $fail -eq 0 ]; then
	echo "${GRN}✓ ALL ${pass} TEST(S) PASSED — libc≡ft, no leaks${RST}"
	exit 0
fi
echo "${RED}✗ ${fail} failed:${RST} ${FAILED[*]}"
echo "${YEL}  inspect: $OUT_DIR/<name>.{libc,ft}.{out,err}, .vg${RST}"
exit 1
