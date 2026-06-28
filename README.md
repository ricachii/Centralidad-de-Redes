# Métricas de Centralidad en Redes

Implementación en C++ de 7 métricas de centralidad (Degree, Betweenness, Closeness, PageRank, ASP, Eigenvector, Clustering) aplicadas sobre redes reales: Yeast PPI y Trade Network.

Proyecto semestral — Estructuras de Datos 2026.  
**Integrantes:** Nicolás Ricciardi, Enzo Levancini, Máximo Beltrán

---

## Requisitos

| Herramienta | Versión mínima |
|---|---|
| CMake | 3.16 |
| MinGW-w64 / GCC | 14.x |
| Python | 3.x (solo para gráficos) |
| pandas + matplotlib | cualquier versión reciente |

```powershell
pip install pandas matplotlib
```

---

## Compilar y ejecutar

### Primera vez
```powershell
.\build.bat
```

### Recompilar después de cambios
```powershell
cd build
mingw32-make -j4
```

### Ejecutar experimentos
**Siempre desde la raíz del proyecto** (las rutas `data/` son relativas):
```powershell
cd "d:\Proyecto Estructura de datos"
.\build\centralidad.exe
```

El programa carga ambos datasets, corre cada métrica 10 veces, guarda CSVs en `results/csv/` y experimenta con agregar/quitar aristas en distintas zonas de la red.

> Betweenness en Yeast tarda ~18 minutos en total (10 runs × ~109 segundos cada una).

### Correr tests
```powershell
.\build\test_graph.exe
.\build\test_metrics.exe
```

### Generar gráficos
```powershell
python scripts/plot_results.py
```

---

## Estructura del proyecto

```
.
├── src/
│   ├── main.cpp                  # Punto de entrada
│   ├── graph/
│   │   ├── Graph.h               # Interfaz del ADT grafo
│   │   └── Graph.cpp             # Implementación: lista de adyacencia
│   ├── metrics/
│   │   ├── IMetric.h             # Interfaz común de todas las métricas
│   │   ├── DegreeCentrality
│   │   ├── BetweennessCentrality
│   │   ├── ClosenessCentrality
│   │   ├── PageRank
│   │   ├── AverageShortestPath
│   │   ├── EigenvectorCentrality
│   │   └── ClusteringCoefficient
│   ├── parsers/
│   │   ├── EdgeListParser        # Formato "src dst [peso]"
│   │   └── PajekParser           # Formato .net con *Vertices y *Arcs
│   ├── experiments/
│   │   ├── ExperimentRunner      # 10 runs → media/varianza, guarda CSV
│   │   └── EdgeExperiment        # Impacto de agregar/quitar aristas por zona
│   └── utils/
│       ├── Timer.h               # Cronómetro de alta resolución
│       ├── Stats.h               # mean(), variance()
│       ├── BFS.h                 # bfsDistances() compartido entre métricas
│       └── ProgressBar.h         # Barra de progreso en terminal
├── tests/
│   ├── test_graph.cpp
│   └── test_metrics.cpp
├── data/
│   ├── dataset1/yeast.edgelist   # Red de proteínas de levadura
│   └── dataset2/*.net            # Red de comercio (2000–2018)
├── results/
│   ├── csv/                      # Resultados en CSV
│   └── plots/                    # Gráficos PNG
├── scripts/
│   └── plot_results.py
├── Entrega1/
│   ├── main.tex                  # Fuente LaTeX
│   └── main.pdf                  # PDF compilado
├── CMakeLists.txt
└── build.bat
```

---

## El ADT Grafo

`Graph` soporta grafos dirigidos y no dirigidos, con o sin pesos.  
Implementado con lista de adyacencia: `unordered_map<int, vector<Edge>>`.

```cpp
Graph g(false);          // no dirigido
g.addEdge(1, 2, 3.5);   // agrega arista con peso
g.removeEdge(1, 2);
g.hasEdge(1, 2);         // O(deg)
g.neighbors(1);          // O(1) — devuelve referencia al vector
g.vertexCount();
g.edgeCount();
g.memoryBytes();
```

| Operación | Complejidad |
|---|---|
| addVertex / addEdge | O(1) esperado |
| removeEdge / hasEdge | O(deg(v)) |
| Construcción completa | O(V + E) |

---

## Las 7 métricas

Todas implementan `IMetric` y devuelven `unordered_map<int, double>`.

| Métrica | Complejidad | Algoritmo |
|---|---|---|
| Degree Centrality | O(V + E) | Recorrido de vecinos |
| Betweenness Centrality | O(V · E) | Brandes (BFS por fuente) |
| Closeness Centrality | O(V · (V+E)) | BFS + Wasserman-Faust |
| PageRank | O(k · (V+E)) | Iteración hasta convergencia, d=0.85 |
| Average Shortest Path | O(V · (V+E)) | BFS por fuente |
| Eigenvector Centrality | O(k · (V+E)) | Power Iteration |
| Clustering Coefficient | O(V · deg²) | Conteo de triángulos |

---

## Datasets

### Yeast PPI — Red de proteínas de levadura
- 6.526 proteínas, 532.180 interacciones
- No dirigido, no ponderado

### Trade Network — Comercio internacional
- 161 países, 22.475 relaciones (5 años combinados: 2000–2018)
- Dirigido, ponderado (USD)
- Se toma el peso máximo por par entre todos los años

---

## Resultados experimentales

### Tiempos de cómputo (media, 10 runs)

| Métrica | Yeast (6526V) | Trade (161V) |
|---|---|---|
| Degree Centrality | 0.73 ms | 0.025 ms |
| PageRank | 298 ms | 24.9 ms |
| Eigenvector Centrality | 411 ms | 34.5 ms |
| Clustering Coefficient | 10.5 s | 699 ms |
| Average Shortest Path | 60.0 s | 702 ms |
| Closeness Centrality | 60.0 s | 705 ms |
| Betweenness Centrality | 109.5 s | 722 ms |

### Experimento de aristas (impacto en Betweenness)

El experimento prueba agregar/quitar 5 aristas en tres zonas distintas de la red:

| Zona | Yeast +5 aristas | Trade +5 aristas |
|---|---|---|
| Hub (top 10% grado) | -0.00004% | 0% (ya saturado) |
| Periférico (bottom 10%) | -0.007% | +5.86% |
| Aleatorio | -0.001% | -0.189% |

En Yeast la red es tan densa que cualquier cambio local es insignificante.  
En Trade, agregar aristas en nodos periféricos abre caminos nuevos que reorganizan el flujo global.
