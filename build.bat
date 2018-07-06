@echo off

cls

setlocal
set path=%path%;%mingw32%

g++ main.cpp -std=c++17 -static && a in.png out.png