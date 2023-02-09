#!/bin/sh
export LD_LIBRARY_PATH="."
export LD_PRELOAD="libft_malloc.so"
# export MALLOCDEBUG=output:script.txt,fill:"AA",fill_free:"FfF",report_allocations,validate_ptrs
# export MALLOCDEBUG=output:script.txt

$@
# valgrind $@
