# CLAUDE.md — Proyecto Semestral: Métricas de Centralidad en Redes

## Contexto del proyecto

Proyecto semestral de la materia Estructuras de Datos. Grupos de 3 personas.
Lenguaje: **C++17**. Build: **CMake + MinGW/GCC**.

**Deadlines:**
- Entrega parcial (sin nota): 10 de junio de 2026 — PDF en `Entrega1/`
- Entrega final: 26 de junio de 2026 a las 23:59
- Presentación oral: después de vacaciones de invierno (~julio 2026)

**Lo que pide el enunciado:**
1. Implementar ADT Grafo general (dirigido/no dirigido, ponderado/no ponderado)
2. Implementar 7 métricas de centralidad (5 obligatorias + 2 adicionales)
3. Experimentos: tiempo de construcción, memoria, tiempo promedio de cada métrica (10 runs, media + varianza), impacto de agregar/quitar aristas
4. Informe PDF (máx 25 páginas) + código en GitHub

---

## Estado actual del proyecto

### Completado
- ADT Grafo (lista de adyacencia): `src/graph/`
- 7 métricas: `src/metrics/`
- Parsers para ambos datasets: `src/parsers/`
- Framework de experimentos: `src/experiments/`
- Tests unitarios: `tests/`
- Experimentos corridos, CSVs generados: `results/csv/`
- Gráficos generados: `results/plots/`
- Entrega 1 (PDF LaTeX): `Entrega1/main.tex` y `Entrega1/main.pdf`
- Documento de comprensión del problema: `comprension_problema.md`
- EdgeExperiment con 3 zonas (hub/periferico/aleatorio): `src/experiments/EdgeExperiment`
- Referencias bibliográficas completas en `Entrega1/main.tex` (6 fuentes con DOI)

### Pendiente
- Informe final en LaTeX (sección experimental con datos reales, conclusiones)
- Subir a GitHub y agregar link al informe
- Nombres del grupo en portada de `Entrega1/main.tex`

---

## Estructura del proyecto

```
Proyecto Estructura de datos/
├── src/
│   ├── main.cpp                      # Punto de entrada: carga datasets y corre experimentos
│   ├── graph/
│   │   ├── Graph.h / Graph.cpp       # ADT lista de adyacencia
│   ├── metrics/
│   │   ├── IMetric.h                 # Interfaz base con ProgressFn callback
│   │   ├── DegreeCentrality
│   │   ├── BetweennessCentrality     # Algoritmo de Brandes O(V*E)
│   │   ├── ClosenessCentrality       # BFS + normalizacion Wasserman-Faust
│   │   ├── PageRank                  # Iterativo, damping=0.85
│   │   ├── AverageShortestPath       # BFS por vertice
│   │   ├── EigenvectorCentrality     # Power iteration
│   │   └── ClusteringCoefficient    # Conteo de triangulos
│   ├── parsers/
│   │   ├── IParser.h
│   │   ├── EdgeListParser            # "src dst [weight]", salta # y %, mapeo string->int
│   │   └── PajekParser               # Formato .net: *Vertices + *Arcs
│   ├── experiments/
│   │   ├── ExperimentRunner          # 10 runs -> media/varianza, guarda CSV, barra progreso
│   │   └── EdgeExperiment            # Impacto de agregar/quitar aristas
│   └── utils/
│       ├── Timer.h                   # High-resolution timer
│       ├── Stats.h                   # mean(), variance(), meanScores()
│       ├── BFS.h                     # bfsDistances() compartido entre metricas
│       └── ProgressBar.h             # Barra de progreso en terminal
├── tests/
│   ├── test_graph.cpp
│   └── test_metrics.cpp
├── data/
│   ├── dataset1/yeast.edgelist       # Proteinas levadura: 6526V, 532180E, tab-separated
│   └── dataset2/2000.net ... 2018.net # Trade Network: Pajek, 161V, dirigido+ponderado
├── results/
│   ├── csv/yeast.csv, trade.csv      # Resultados de experimentos
│   └── plots/                        # PNGs generados por plot_results.py
├── scripts/
│   └── plot_results.py               # Genera graficos desde results/csv/ (pandas+matplotlib)
├── Entrega1/
│   ├── main.tex                      # PDF entrega parcial 10 jun
│   ├── main.pdf
│   └── compilar.bat
├── comprension_problema.md           # Descripcion detallada de las 7 metricas con formulas
├── CMakeLists.txt
└── build.bat                         # Compila y ejecuta desde la raiz del proyecto
```

---

## Como compilar y ejecutar

```powershell
# Primera vez o si cambia CMakeLists.txt
build.bat

# Recompilar rapido (desde cualquier lugar)
cd "d:\Proyecto Estructura de datos\build"
mingw32-make -j4

# IMPORTANTE: siempre ejecutar desde la raiz (las rutas data/ son relativas)
cd "d:\Proyecto Estructura de datos"
.\build\centralidad.exe

# Tests
.\build\test_graph.exe
.\build\test_metrics.exe

# Generar graficos (desde cualquier directorio)
python "d:\Proyecto Estructura de datos\scripts\plot_results.py"

# Compilar PDF entrega
cd Entrega1
compilar.bat
```

---

## Datasets

### Dataset 1: Yeast PPI
- Archivo: `data/dataset1/yeast.edgelist`
- Formato: `proteina_A\tproteina_B` (tab-separated, sin header)
- Cada arista aparece dos veces (A->B y B->A): el parser deduplica
- Parser: `EdgeListParser(directed=false, delimiter='\t')`
- Mapeo string->int automatico en el parser

### Dataset 2: Trade Network
- Archivos: `data/dataset2/{2000,2005,2010,2015,2018}.net`
- Formato Pajek: seccion `*Vertices` (id "pais" x y z) + seccion `*Arcs` (src dst weight)
- Dirigido, ponderado (peso = valor comercial en USD)
- Parser: `PajekParser(directed=true)`
- Los 5 años se combinan en un grafo: se conserva el peso maximo por par

---

## Resultados experimentales

### Yeast PPI (6526 vertices, 532180 aristas)
- Build time: ~3900 ms | Memoria: 24150 KB

| Metrica               | Media (ms)  | Varianza (ms^2) | Min       | Max       |
|-----------------------|-------------|-----------------|-----------|-----------|
| Degree Centrality     | 0.729       | 0.007           | 0.641     | 0.923     |
| Betweenness           | 109452      | 87823           | 109039    | 110020    |
| Closeness             | 60020       | 3365            | 59903     | 60128     |
| PageRank              | 298         | 177             | 290       | 337       |
| Avg Shortest Path     | 60042       | 4577            | 59964     | 60158     |
| Eigenvector           | 411         | 178             | 404       | 451       |
| Clustering Coeff      | 10493       | 341             | 10459     | 10514     |

**Impacto +5 aristas por zona (Betweenness / Closeness / ASP):**
- Hub:        -4.3e-05% / +2.5e-05% / -2.3e-05%  (cambio insignificante)
- Periferico: -0.007%   / +0.003%   / -0.004%
- Aleatorio:  -0.001%   / +0.0004%  / -0.0005%

**Impacto -5 aristas por zona (Betweenness / Closeness / ASP):**
- Hub:        +5.0e-05% / -2.8e-05% / +2.8e-05%  (cambio insignificante)
- Periferico: -0.158%   / -0.060%   / -0.082%
- Aleatorio:  -0.053%   / -0.020%   / -0.027%

Nota: cambios minimos en todas las zonas por la alta densidad (5 aristas sobre 532K).
Los nodos perifericos muestran mayor impacto relativo al tener menos conexiones de base.

### Trade Network (161 vertices, 22475 aristas)
- Build time: ~551 ms | Memoria: 571 KB

| Metrica               | Media (ms) | Varianza (ms^2) | Min    | Max    |
|-----------------------|------------|-----------------|--------|--------|
| Degree Centrality     | 0.025      | 0.000015        | 0.020  | 0.032  |
| Betweenness           | 722        | 1602            | 647    | 786    |
| Closeness             | 705        | 735             | 673    | 739    |
| PageRank              | 24.9       | 0.18            | 24.3   | 25.7   |
| Avg Shortest Path     | 702        | 644             | 672    | 737    |
| Eigenvector           | 34.5       | 0.049           | 34.3   | 34.9   |
| Clustering Coeff      | 699        | 581             | 666    | 727    |

**Impacto +5 aristas por zona (Betweenness / Closeness / ASP):**
- Hub:        +0%      / +0%      / +0%       (hubs ya saturados, sin aristas nuevas posibles)
- Periferico: +5.86%   / +0.354%  / +1.135%
- Aleatorio:  -0.189%  / +0.013%  / -0.018%

**Impacto -5 aristas por zona (Betweenness / Closeness / ASP):**
- Hub:        +0.189%  / -0.021%  / +0.018%
- Periferico: +0.189%  / -0.010%  / +0.018%
- Aleatorio:  +0.189%  / -0.017%  / +0.018%

Nota: hubs en Trade no admiten aristas nuevas (grafo casi completo, ~87% densidad).
Quitar aristas produce el mismo delta en Betweenness/ASP en las 3 zonas por la alta densidad.

---

## Decisiones de diseño relevantes

**Lista de adyacencia sobre matriz:** todos los datasets son redes dispersas. Yeast
con lista usa 24 MB vs ~340 MB con matriz (6526^2 entradas).

**Brandes para Betweenness:** O(V*E) en lugar del naive O(V^2 * E). Con Yeast
esto es la diferencia entre horas y dias de computo.

**Normalizacion Wasserman-Faust en Closeness:** maneja grafos desconectados
correctamente. Sin esta normalizacion, vertices aislados distorsionan los resultados.

**ProgressFn callback en IMetric:** permite que el ExperimentRunner muestre
progreso en tiempo real sin acoplar las metricas a la interfaz de usuario.
Metricas lentas (Betweenness, Closeness, ASP) llaman progress(done, total) por vertice.

**Swap en vez de copia en PageRank/Eigenvector:** evita copiar el mapa completo
O(V) en cada iteracion. Se usan dos mapas y se intercambian con .swap().

**bfsDistances() compartido:** Closeness y ASP tenian BFS duplicado. Extraido
a `utils/BFS.h` como funcion inline.

**Deduplicacion en EdgeListParser:** yeast lista cada arista dos veces (A->B y B->A).
El parser usa un unordered_set para saltar duplicados al parsear como no dirigido.

**Combinacion de años en Trade:** se toma el peso maximo por par (src, dst) entre
los 5 años disponibles. Esto produce la red mas completa sin duplicar aristas.

**EdgeExperiment con zonas (Hub/Periferico/Aleatorio):** `EdgeExperiment::Zone` selecciona
nodos fuente desde el 10% superior (hub) o inferior (periferico) por grado. Permite
comparar el impacto de cambios estructurales en distintas partes de la red, cumpliendo
el requisito del enunciado de "probar en distintos lugares de la red".

---

## Convenciones del codigo

- Sin tildes ni ñ en comentarios ni strings de output (terminal Windows)
- Sin comentarios que expliquen QUE hace el codigo (solo POR QUE cuando no es obvio)
- Header-only para utilities simples (Timer, Stats, BFS, ProgressBar)
- Toda metrica implementa `IMetric` con `compute(const Graph&, ProgressFn) const`
- Toda metrica nueva sigue el patron: .h en `src/metrics/`, .cpp en `src/metrics/`

---

## Metricas adicionales (las 2 propias)

Se eligieron **Eigenvector Centrality** y **Clustering Coefficient** porque:
- Son bien documentadas con formulas claras (faciles de citar)
- Son conceptualmente distintas de las 5 base (no hay solapamiento)
- Tienen interpretacion biologica/economica directa en ambos datasets
- Sus algoritmos (Power Iteration, conteo de triangulos) son pedagogicamente interesantes
