#!/bin/sh

set -e

TEST_PATH="$( pwd )/test"
ENTRYPOINT="$( pwd )/run.sh"
CURRENT_EXEC=""

log () {
	>&2 python <<- EOF
	s = '$@'
	pad_left = ( 80 - len(s) - 2 ) / 2
	pad_right = 80 - pad_left - len(s) - 2
	out = '>' * pad_left + ' ' + s + ' ' + '<' * pad_right
	print out
EOF
}

cleanup() {
	if [ -f "${CURRENT_EXEC}" ]; then
		rm "${CURRENT_EXEC}"
	fi
}

trap cleanup EXIT

run () {
	log Compiling $( basename "$1.test" )
	gcc -o "$1.test" "$1"
	CURRENT_EXEC="$1.test"
	log Running $( basename "${CURRENT_EXEC}" )
	${ENTRYPOINT} "${CURRENT_EXEC}"
	cleanup
	log Passed
}

files=$(find "${TEST_PATH}" -type f -name "*.c")
for file in $files;
do
	run "${file}";
done

log All tests passed.
