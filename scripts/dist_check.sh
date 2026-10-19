#!/usr/bin/env bash
# Proves a release tarball works for someone who never built libft: unpacks it
# somewhere else, then compiles, links and runs a program against nothing but
# what the package ships -- once per compiler given.
#
# usage: scripts/dist_check.sh <libft-VERSION-linux-ARCH.tar.gz> [compiler...]
#
# Each link is one way a real consumer uses the library: a PIE executable (the
# default almost everywhere), a non-PIE one (Torturette links -no-pie), libft.a
# folded into a shared object (Torturette's LD_PRELOAD injector) and libft.so.
set -euo pipefail

tarball=$(realpath "$1")
shift
compilers=("$@")
[ ${#compilers[@]} -gt 0 ] || compilers=("${CC:-cc}")

base=$(basename "$tarball" .tar.gz)
version=${base#libft-}
version=${version%-linux-*}
work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT
fail=0
ko() {
	echo "KO   $*"
	fail=1
}

if [ -f "$tarball.sha256" ]; then
	(cd "$(dirname "$tarball")" && sha256sum -c "$base.tar.gz.sha256")
fi

pkg=$work/libft
mkdir "$pkg"
tar -xzf "$tarball" -C "$pkg" --strip-components=1
lib=$pkg/build/lib

for f in libft.h include/xalloc_config.h include/ft_version.h build/lib/libft.a \
	build/lib/libft.so srcs/stdio/input/get_next_line.c BUILDINFO; do
	[ -s "$pkg/$f" ] || ko "missing from the package: $f"
done
[ ! -e "$pkg/Makefile" ] || ko "the package ships a Makefile"
! grep -q 'define HAVE_FT_MALLOC' "$pkg/include/xalloc_config.h" \
	|| ko "built with ft_malloc: a consumer interposing malloc would see none of it"

# An LTO archive is tied to the exact compiler that wrote it; a package for
# strangers has to be plain machine code. GCC leaves .gnu.lto_ sections, clang
# leaves bitcode members nm cannot read at all.
if objdump -h "$lib/libft.a" 2>/dev/null | grep -q '\.gnu\.lto_'; then
	ko "libft.a carries GCC LTO bytecode"
fi
symbols=$(nm --defined-only "$lib/libft.a" 2>/dev/null | grep -c ' T ' || true)
[ "$symbols" -gt 100 ] || ko "libft.a: only $symbols functions readable by nm"
echo "libft.a: $(ar t "$lib/libft.a" | wc -l) objects, $symbols functions"
sed 's/^/  /' "$pkg/BUILDINFO"

cat >"$work/smoke.c" <<'EOF'
#include "libft.h"

static int	check(int ok, const char *what)
{
	if (!ok)
		dprintf(2, "smoke: %s\n", what);
	return (!ok);
}

int	main(int argc, char **argv)
{
	char	buf[64];
	char	*dup;
	char	*line;
	int		fd;
	int		lines;
	int		bad;

	bad = check(argc == 3, "usage: smoke <file> <version>");
	if (bad)
		return (2);
	bad |= check(ft_atoi("  -42") == -42, "ft_atoi");
	dup = ft_strdup("torturette");
	bad |= check(dup && ft_strlen(dup) == 10, "ft_strdup/ft_strlen");
	free(dup);
	ft_snprintf(buf, sizeof(buf), "%s-%d-%x", "ok", 7, 255);
	bad |= check(ft_strncmp(buf, "ok-7-ff", 8) == 0, "ft_snprintf");
	bad |= check(ft_strncmp(ft_version(), LIBFT_VERSION, 64) == 0,
			"ft_version() differs from the LIBFT_VERSION macro");
	bad |= check(ft_strncmp(ft_version(), argv[2], 64) == 0,
			"ft_version() differs from the package name");
	fd = open(argv[1], O_RDONLY);
	lines = 0;
	while (fd >= 0 && (line = get_next_line(fd)) != NULL)
	{
		lines++;
		free(line);
	}
	if (fd >= 0)
		close(fd);
	bad |= check(lines == 3, "get_next_line did not read 3 lines");
	return (bad);
}
EOF
cat >"$work/plugin.c" <<'EOF'
#include "libft.h"

int	plugin_len(const char *s)
{
	return ((int)ft_strlen(s));
}
EOF
printf 'one\ntwo\nthree\n' >"$work/three.txt"

for cc in "${compilers[@]}"; do
	if ! command -v "$cc" >/dev/null 2>&1; then
		ko "$cc: not installed"
		continue
	fi
	echo "== $($cc --version | head -1)"
	out=$work/$(basename "$cc")
	inc=(-Wall -Wextra -I"$pkg")
	run() {
		local what=$1
		shift
		if "$@" >"$out.log" 2>&1; then
			echo "OK   $what"
		else
			ko "$what"
			sed 's/^/     /' "$out.log"
		fi
	}
	run "PIE executable" "$cc" "${inc[@]}" "$work/smoke.c" "$lib/libft.a" -ldl -pthread -o "$out-pie"
	run "  runs" "$out-pie" "$work/three.txt" "$version"
	run "non-PIE executable" "$cc" "${inc[@]}" -no-pie "$work/smoke.c" "$lib/libft.a" -ldl -pthread -o "$out-nopie"
	run "  runs" "$out-nopie" "$work/three.txt" "$version"
	run "shared object with libft.a inside" "$cc" "${inc[@]}" -shared -fPIC "$work/plugin.c" "$lib/libft.a" \
		-Wl,--exclude-libs,ALL -ldl -pthread -o "$out-plugin.so"
	run "libft.so" "$cc" "${inc[@]}" "$work/smoke.c" -L"$lib" -lft -Wl,-rpath,"$lib" -ldl -pthread -o "$out-so"
	run "  runs" "$out-so" "$work/three.txt" "$version"
done

[ "$fail" -eq 0 ] && echo "package OK: $base" || echo "package BROKEN: $base"
exit "$fail"
