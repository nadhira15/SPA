@echo off
title system test 4 queries

..\..\Code\Release\AutoTester SourceAffectsProcedure.txt Affects.txt out-Affects.xml
..\..\Code\Release\AutoTester SourceAffectsProcedure.txt AffectsStar.txt out-AffectsStar.xml


echo FINISHED
pause
exit