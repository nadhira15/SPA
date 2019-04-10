@echo off
title system test 1 queries

..\..\Code\Debug\AutoTester sourceAlphanumeric.txt Calls.txt out-Calls.xml
..\..\Code\Debug\AutoTester sourceAlphanumeric.txt CallsStar.txt out-CallsStar.xml

echo FINISHED
pause
exit