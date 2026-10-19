# Bibliothèque de matrices en C++

Une classe `Matrix<T>` générique (header-only) avec les opérations classiques de l'algèbre linéaire.

## Fonctionnalités

- Template : fonctionne avec `int`, `double`, `float`...
- Construction lisible : `Matrix<int> m{{1, 2}, {3, 4}};`
- Opérateurs `+`, `-`, `*` (matrice et scalaire), `==`, `<<`
- Transposée, matrice identité
- **Déterminant** (élimination de Gauss avec pivot partiel)
- **Inverse** (Gauss-Jordan)
- Vérification des dimensions avec exceptions (`std::invalid_argument`, `std::out_of_range`)

## Exemple

```cpp
#include "matrix.hpp"

Matrix<double> a{{2, 1}, {1, 3}};
std::cout << a * a.inverse();   // matrice identité
std::cout << a.determinant();   // 5
```

## Compiler et tester

```bash
cmake -B build
cmake --build build
./build/matrix_demo
ctest --test-dir build --output-on-failure
```

## Notions abordées

Templates, surcharge d'opérateurs, `initializer_list`, lambdas, exceptions, RAII avec `std::vector`, optimisation de l'ordre des boucles pour le cache.
