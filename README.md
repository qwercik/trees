# trees
Program testujący działanie drzew binarnych: BST oraz AVL

## Obsługa
Przy uruchomieniu programu należy podać dwa dodatkowe parametry: rodzaj drzewa oraz operację, którą chcemy przetestować

Pierwszym rodzajem drzewa jest `bst`. Wspierane operacje: `create`, `find-min`, `print`, `dsw`.
Drugim rodzajem drzewa jest `avl`. Wspierane operacje: `create`, `find-min`, `print`.

Na standardowe wejście należy podać listę liczb do przetestowania; liczby powinny być rozdzielone dowolnymi białymi znakami, zalecane są spacje.
Przy czym należy uważać, by podawać liczby wyłącznie malejące, w przeciwnym przypadku program testujący może się, kolokwialnie mówiąc, wysypać! Nie jest to błąd implementacji drzew, a specjalne założenie, które pozwoliło znacznie przyspieszyć i uskutecznić testy - bez tego nie byłbym w stanie szybko wygenerować drzewa BST o rozmiarze 50 milionów elementów.

