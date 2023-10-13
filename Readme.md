# Proyecto desarrollado por:
- Elizabeth Bravo Campos
- Gustavo González Gutierrez
- Esteban Suazo Montalba

En este proyecto se implementó una metaheurística GRASP para mejorar los algoritmos greedy de la entrega anterior.

## Contenido de la entrega:
1. greedy-probabilista.cpp : Es el algoritmo greedy al que posteriormente se le aplica una Local Search.
1. T5Grasp.cpp : Es el algoritmo de GRASP finalmente implementado, diseñado para correr una instancia a la vez.
1. GraspDataset.cpp : Son los mismos algoritmos que en el archivo anterior pero adaptados para correr 100 instancias en una ejecución, de tamaño m x l.

## Parámetros:
1. En el caso de T5Grasp.cpp los parámetros usables son:

* -i : Indica la instancia a ejecutar.
* -t : Indica el tiempo maximo de ejecucion del algoritmo.
* -it : Intentos maximos que tendrá el LS para mejorar la solución entregada por el Greedy
* -d : El grado de determinismo que usará el greedy-probabilista.

1. En el caso de GraspDataset.cpp los parámetros usables son:
* Los mismos pero se elimina la instancia a ejecutar.

## Instrucciones de compilación
- Si desea compilar T5Grasp para probar la implementación con una sola instancia:
```
g++ T5Grasp.cpp greedy-probabilista.cpp -o <Nombre archivo salida>
```
- Si por otro lado desea compilar GraspDataset para ejecutar 100 instancias de una vez:
```
g++ GraspDataset.cpp greedy-probabilista.cpp -o <Nombre archivo salida>
```

## Instrucciones de ejecución
- Una vez creado el/los ejecutables en el caso de T5Grasp se tiene:
```
./<Nombre archivo salida> -i <instancia> -t <tiempo> -it <intentos> -d <determinismo>
```
- En el caso de GraspDataset.cpp
```
./<Nombre archivo salida> -t <tiempo> -it <intentos> -d <determinismo>
```
**Cabe destacar que las 100 instancias deben estar en la misma carpeta que el ejecutable creado**