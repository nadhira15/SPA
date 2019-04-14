@echo off
title system test 3 queries

..\..\Code\Release\AutoTester sourceAlphanumeric.txt Affects.txt out-Affects.xml
..\..\Code\Release\AutoTester sourceAlphanumeric.txt AffectsStar.txt out-AffectsStar.xml

..\..\Code\Release\AutoTester sourceAlphanumeric.txt Calls.txt out-Calls.xml
..\..\Code\Release\AutoTester sourceAlphanumeric.txt CallsStar.txt out-CallsStar.xml

..\..\Code\Release\AutoTester sourceAlphanumeric.txt Next.txt out-Next.xml
..\..\Code\Release\AutoTester sourceAlphanumeric.txt NextStar.txt out-NextStar.xml

..\..\Code\Release\AutoTester sourceAlphanumeric.txt NoCondition.txt out-NoCondition.xml

..\..\Code\Release\AutoTester sourceAlphanumeric.txt Boolean.txt out-Boolean.xml

..\..\Code\Release\AutoTester sourceAlphanumeric.txt PatternWhile.txt out-PatternWhile.xml


echo FINISHED
pause
exit