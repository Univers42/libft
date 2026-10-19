#!/usr/bin/env bash
# Packs a built libft into the release tarball a consumer downloads instead of
# compiling it.
#
# usage: scripts/dist.sh <version> <arch> <build dir> <out dir>
#
# The tarball unpacks to one directory laid out exactly like this repository,
# so it drops in wherever a libft checkout used to be: -I libft,
# libft/build/lib/libft.a and libft/srcs/... all keep working. That means every
# header (several of include/ reach into ../srcs/) and every source too -- a
# consumer can still read, step into or mutate the code it links. What it does
# NOT carry is a Makefile: an unpacked package is unmistakably prebuilt, not a
# checkout to build. BUILDINFO records what produced it.
#
# Owner, group and mtimes are fixed and gzip leaves its own timestamp out, so
# the tarball carries nothing about the runner that packed it.
set -euo pipefail

version=$1 arch=$2 build=$3 out=$4
name="libft-$version-linux-$arch"
stage=$(mktemp -d)
trap 'rm -rf "$stage"' EXIT
pkg="$stage/$name"

for lib in libft.a libft.so; do
	if [ ! -s "$build/lib/$lib" ]; then
		echo "dist: $build/lib/$lib is missing, build it first" >&2
		exit 1
	fi
done

mkdir -p "$pkg/build/lib"
cp "$build/lib/libft.a" "$build/lib/libft.so" "$pkg/build/lib/"
find libft.h include srcs -path srcs/memory/ft_malloc -prune \
	-o -type f \( -name '*.c' -o -name '*.h' \) -print0 \
	| xargs -0 cp --parents -t "$pkg"
cp README.md "$pkg/"

{
	echo "version   $version"
	echo "commit    $(git rev-parse HEAD 2>/dev/null || echo unknown)"
	echo "target    linux-$arch"
	echo "compiler  $(${CC:-cc} --version | head -1)"
	echo "glibc     $(getconf GNU_LIBC_VERSION 2>/dev/null || echo unknown)"
	echo "build     make dist: -O3 -g -fPIC, LTO=0, FT_MALLOC=0 (libc allocator)"
} >"$pkg/BUILDINFO"

epoch=${SOURCE_DATE_EPOCH:-$(git log -1 --format=%ct 2>/dev/null || echo 0)}
mkdir -p "$out"
tar --sort=name --owner=0 --group=0 --numeric-owner --mtime="@$epoch" \
	-C "$stage" -cf - "$name" | gzip -9n >"$out/$name.tar.gz"
(cd "$out" && sha256sum "$name.tar.gz" >"$name.tar.gz.sha256")

echo "packed $out/$name.tar.gz ($(du -h "$out/$name.tar.gz" | cut -f1))"
cat "$out/$name.tar.gz.sha256"
