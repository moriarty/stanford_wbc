#!/bin/sh


##################################################
#
# BEGIN old instructions (for recycling...)
#
# HOW TO CREATE COVERAGE REPORTS WITH THIS SCRIPT
# 
#  cd build
#  rm -rf * && cmake -DCOVERAGE=true .. && make -j 4
#  cd ..
#  lcov --directory build --zerocounters
#  cd build
#  ../runtests.sh 
#  cd ..
#  lcov --directory build --capture --output-file wbc.info
#  genhtml wbc.info -o stanford-wbc-lcov/
#
## to do: make these standalone?
# wbcnet/testStreamBufMgr
# wbcnet/testMQSpeed
# wbcnet/testSockWrapMuldex
# wbc/tests/testDirectoryServer
# wbc/tests/testBehaviorParser
#
## to do: needs to find the test module's .so
# wbcnet/testTestModule
#
## segfaults on purpose
# wbc/tests/testDtorCheck
#
## segfaults
# wbc_plugins/wbc_plugins/robotlog/test
#
## lcov did not like these (truncated output files?)
#    wbcnet/testLogWithoutLog4cxx \
#    wbcnet/testDelayHistogram \
#    wbcnet/testLogDisabled \
#    wbcnet/testLogWithLog4cxx \
#    saimatrix/test_SAILapack \
#
# END old instructions
#
# BEGIN old test list
#
#    jspace/tests/testServoProxy \
#    wbcnet/testPack \
#    wbcnet/testTaskAtomizer \
#    wbcnet/testMQWrap \
#    wbcnet/testMuldex \
#    wbcnet/testEndian \
#    wbcnet/testID \
#    wbcnet/testProxy \
#    wbcnet/testFactory \
#    jspace/tests/testJspace \
#    tao/testTAO \
#    wbc/tests/testProcess \
#    wbc_tinyxml/xmltest; do
#
# END old test list
##################################################

MSG=""
FAIL=""
NOTFOUND=""

for test in \
    tao/testTAO \
    jspace/tests/testJspace \
    opspace/testTask \
    opspace/testFactory; do
    if [ -x $test ]; then
	$test 2>&1
	if [ $? -eq 0 ]; then
	    MSG="$MSG\n$test OK"
	else
	    MSG="$MSG\n$test failed"
	    FAIL="$FAIL $test"
	fi
    else
	MSG="$MSG\n$test not found"
	NOTFOUND="$NOTFOUND $test"
    fi
done

echo $MSG
if [ -n "$NOTFOUND" ]; then
    echo
    echo "Not found:$NOTFOUND"
else
    echo
    echo "All tests were found."
fi
if [ -n "$FAIL" ]; then
    echo
    echo "Failures in:$FAIL"
    exit 1
else
    if [ -n "$MSG" ]; then
	echo
	echo "All tests passed."
    else
	echo
	echo "No tests were run."
    fi
fi
