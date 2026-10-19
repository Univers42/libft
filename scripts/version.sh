#!/usr/bin/env bash
# libft's version, read off its git history. Releases are tags vMAJOR.MINOR.PATCH.
#
#   scripts/version.sh current   the last release, plus "+dev" once HEAD is past it
#   scripts/version.sh next      the release the commits since the last one call
#                                for -- or nothing at all when they ship nothing
#   scripts/version.sh notes     those commits, as Markdown release notes
#
# WHETHER to release is decided by what changed, not by how it is described:
# only commits touching what the package ships (libft.h, include/, srcs/, the
# Makefile) make a release. A push that only touches CI, tests or docs still
# gets packaged and checked by the release workflow, and publishes nothing.
#
# WHICH number moves is read off those commits' subjects (Conventional
# Commits): "type!:" or a BREAKING CHANGE footer bumps MAJOR, any feat bumps
# MINOR, everything else PATCH. Plenty of subjects here just say "update", so a
# commit that declares no type is never read as "nothing changed": it is a
# PATCH like any other fix.
set -euo pipefail

# The first release of this scheme. The old 1.00 tag (2025) predates it and is
# not semver, so it is not matched below and does not count.
INITIAL=1.1.0
SHIPPED=(libft.h include srcs Makefile ':(exclude)srcs/memory/ft_malloc')
TAG_GLOB='v[0-9]*.[0-9]*.[0-9]*'

last_tag() {
	git describe --tags --abbrev=0 --match "$TAG_GLOB" 2>/dev/null || true
}

current() {
	local tag
	tag=$(last_tag)
	if [ -z "$tag" ]; then
		echo "0.0.0+dev"
	elif [ "$(git rev-parse HEAD)" = "$(git rev-parse "$tag^{commit}")" ]; then
		echo "${tag#v}"
	else
		echo "${tag#v}+dev"
	fi
}

next() {
	local tag major minor patch subjects bodies
	tag=$(last_tag)
	if [ -z "$tag" ]; then
		echo "$INITIAL"
		return
	fi
	[ -n "$(git diff --name-only "$tag" HEAD -- "${SHIPPED[@]}")" ] || return 0

	IFS=. read -r major minor patch <<<"${tag#v}"
	subjects=$(git log --no-merges --format='%s' "$tag..HEAD" -- "${SHIPPED[@]}")
	bodies=$(git log --no-merges --format='%b' "$tag..HEAD" -- "${SHIPPED[@]}")
	if grep -qE '^[a-z]+(\([^)]*\))?!:' <<<"$subjects" \
		|| grep -qE '^BREAKING[ -]CHANGE:' <<<"$bodies"; then
		echo "$((major + 1)).0.0"
	elif grep -qE '^feat(\([^)]*\))?:' <<<"$subjects"; then
		echo "$major.$((minor + 1)).0"
	else
		echo "$major.$minor.$((patch + 1))"
	fi
}

# One section per kind of change, each line "subject (sha)". The first release
# has no previous tag to start from, and listing every commit since 2025 helps
# nobody, so it says what it is instead.
notes() {
	local tag log
	tag=$(last_tag)
	if [ -z "$tag" ]; then
		echo "First versioned release of libft as a prebuilt package."
		return
	fi
	log=$(git log --no-merges --format='%s (%h)' "$tag..HEAD" -- "${SHIPPED[@]}")
	section "Breaking changes" '^[a-z]+(\([^)]*\))?!:' <<<"$log"
	section "Features" '^feat(\([^)]*\))?:' <<<"$log"
	section "Fixes" '^fix(\([^)]*\))?:' <<<"$log"
	section "Performance" '^perf(\([^)]*\))?:' <<<"$log"
	section "Other changes" '' <<<"$log"
	echo "Full diff: https://github.com/Univers42/libft/compare/$tag...v$(next)"
}

# Prints the lines of stdin matching $2 under a "### $1" heading. An empty
# pattern takes whatever no other section claimed.
section() {
	local lines
	if [ -n "$2" ]; then
		lines=$(grep -E "$2" || true)
	else
		lines=$(grep -vE '^[a-z]+(\([^)]*\))?!:|^(feat|fix|perf)(\([^)]*\))?:' || true)
	fi
	[ -n "$lines" ] || return 0
	printf '### %s\n\n' "$1"
	sed 's/^/- /' <<<"$lines"
	echo
}

case "${1:-}" in
	current) current ;;
	next) next ;;
	notes) notes ;;
	*)
		echo "usage: $0 current|next|notes" >&2
		exit 2
		;;
esac
