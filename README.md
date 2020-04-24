# trees
Program testujący działanie drzew binarnych: BST oraz AVL

## Kompilacja
Najpierw program trzeba sobie skompilować. Aby to zrobić, można skorzystać z następujących komend:
```bash
$ mkdir build/
$ cd build
$ cmake ..
$ make
```

Jeżeli wszystko się powiedzie, program będzie dostępny w lokalizacji `build/trees`.

## Obsługa
Przy uruchomieniu programu należy podać dwa dodatkowe parametry: rodzaj drzewa oraz operację, którą chcemy przetestować

Pierwszym rodzajem drzewa jest `bst`. Wspierane operacje: `create`, `find-min`, `print`, `dsw`.
Drugim rodzajem drzewa jest `avl`. Wspierane operacje: `create`, `find-min`, `print`.

Na standardowe wejście należy podać listę liczb do przetestowania; liczby powinny być rozdzielone dowolnymi białymi znakami, zalecane są spacje.
Przy czym należy uważać, by podawać liczby wyłącznie malejące, w przeciwnym przypadku program testujący może się, kolokwialnie mówiąc, wysypać! Nie jest to błąd implementacji drzew, a specjalne założenie, które pozwoliło znacznie przyspieszyć i uskutecznić testy - bez tego nie byłbym w stanie szybko wygenerować drzewa BST o rozmiarze 50 milionów elementów.

Dodatkowo w tym katalogu znajduje się skrypt `gen.py`, który generuje malejący ciąg liczb o podanym rozmiarze. Można go wykorzystać do testów.

Przykład:
```bash
$ ./gen.py 10000 | ./build/trees bst find-min
```

