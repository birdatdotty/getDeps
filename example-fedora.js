#!/usr/bin/env getDeps
var d = new Deps();

// Задаем пути для поиска (берет случайнаю библиотеку из найденных)
d.addSearchPath("/usr/i686-w64-mingw32/sys-root/mingw/bin/");

// Загружаем список иключений ("KERNEL32.dll", "msvcrt.dll", "USER32.dll", ...)
d.loadJSONLibrary("/tmp/ignoryLib.json");

// Загружаем список найденных библиотек
d.loadJSONLibrary("/tmp/22.json")

// objdump перечисляет библиотеки как "\tИмя DLL: any.dll"
d.setDLLName("\tИмя DLL: ");

// запускаем поиск для нужного файла  ($ example.js myapp.exe)
d.deps(file);

// вывод для отладки (тут я просто смотрел некоторые значения через qInfo)
d.print();

// сохраняем уже найденные библиотеки
d.saveJSONLibrary("/tmp/22.json")

// копируем найденные библиотеки в директорию
d.copyTo("/tmp/12313");


console.log("finish");
console.log(args[2])
