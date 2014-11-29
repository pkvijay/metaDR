TIME="3s" # amount of time to wait for server before communicating with it
TESTS=(create getset list remove) # filenames of tests to run
TESTDIR="tests/" # directory containing tests
SERVER="server/server" # path to server executable
SHELL="shell/shell" # path to shell executable
DBFILE="kvStore.db" # path to database file
 
pkill -f $SERVER # kill existing server (if applicable)
pkill -f $SHELL # kill existing shell (if applicable)
for TEST in ${TESTS[*]}; do # perform each test
  TESTIN=$TESTDIR$TEST".tst"
  TESTOUT=$TESTDIR$TEST".out"
  echo "Testing "$TESTIN
  rm -f $DBFILE # remove extant database file
  echo " starting up "$SERVER"... waiting "$TIME
  $SERVER > /dev/null &
  PID=$!
  sleep $TIME
  echo " diff-ing your output with "$TESTOUT"..."
  DIFF=$($SHELL 127.0.0.1 $TESTIN | diff $TESTOUT -)
  if [[ "$DIFF" = "" ]]; then
    echo " PASS: Your output and "$TESTOUT" are identical"
  else
    echo " FAIL: Your output and "$TESTOUT" differ as follows..."
    echo $DIFF
  fi
  disown $PID # don't get output for the server dying
  kill -9 $PID 2> /dev/null # kill the server
  echo
done
rm -f $DBFILE # remove leftover database file
