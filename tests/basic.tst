list /
create /ltest 123
create /ltest/a 456
create /ltest/b 789
list /ltest
remove /ltest/b
list /ltest
list /lmissing_key
list /
remove /ltest/a
remove /ltest
create BADPATH 123
create /ctest 123
create /ctest 123
create /ctest/b abc
create /cfoo/c def
remove /ctest/b
remove /ctest
remove BADPATH
create /rtest 123
remove /rtest
remove /rtest
remove /rfoo
create /rtest abc
create /rtest/b def
remove /rtest
remove /rtest/b
remove /rtest
get BADKEY
set BADKEY 123
create /gtest 123
get /gtest
set /gtest foo
get /gtest
get /gmissing_key
set /gmissing_key bar
remove /gtest
