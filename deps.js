#!/usr/bin/env getDeps
var d = new Deps();

d.addSearchPath("/home/android/Qt/5.12.8/Src/qtbase/lib/");
d.addSearchPath("/usr/lib/gcc/i686-w64-mingw32/");
d.addSearchPath("/home/android/Qt/5.12.7/Src/gnuwin32/bin/");

d.searchNewLibrary(file);

d.deps(file);

d.print();

d.copyTo("/tmp/123");

console.log("finish");
