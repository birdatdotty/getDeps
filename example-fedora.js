#!/usr/bin/env getDeps
var d = new Deps();

// Задаем пути для поиска (берет случайнаю библиотеку из найденных)
d.addSearchPath("/usr/i686-w64-mingw32/sys-root/mingw/bin/");

// objdump перечисляет библиотеки как "\tИмя DLL: any.dll"
d.setDLLName("\tИмя DLL: ");

// запускаем поиск для нужного файла  ($ example.js myapp.exe)
d.deps(file);

//d.print();

// копируем найденные библиотеки в директорию
d.copyTo("/tmp/123");

console.log("finish");
