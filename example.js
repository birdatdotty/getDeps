#!/usr/bin/env getDeps
var d = new Deps();

// Задаем пути для поиска (берет случайнаю библиотеку из найденных)
d.addSearchPath("/home/android/Qt/5.12.8/Src/qtbase/lib/");
d.addSearchPath("/usr/lib/gcc/i686-w64-mingw32/");
d.addSearchPath("/home/android/Qt/5.12.7/Src/gnuwin32/bin/");

// запускаем поиск для нужного файла  ($ example.js myapp.exe)
d.deps(file);

//d.print();

// копируем найденные библиотеки в директорию
d.copyTo("/tmp/123");

console.log("finish");
