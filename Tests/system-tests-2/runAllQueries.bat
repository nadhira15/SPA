@echo off
title system test 1 queries

..\..\Code\Debug\AutoTester source-nesting.txt Calls.txt out-Calls.xml
..\..\Code\Debug\AutoTester source-nesting.txt CallsStar.txt out-CallsStar.xml

echo FINISHED
pause
exit