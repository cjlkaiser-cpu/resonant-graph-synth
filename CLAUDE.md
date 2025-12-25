# Resonant Graph Synth

Sintetizador C++/JUCE con grafo de resonadores acoplados.

## Estado

**Fase:** Alpha (M0-M2 completados)
**Funciona:** Standalone + VST3 + AU

## Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j8

# Ejecutar
open "build/ResonantGraphSynth_artefacts/Release/Standalone/Resonant Graph Synth.app"
```

## Arquitectura

```
MIDI -> PluginProcessor -> ResonatorGraph (12 nodos) -> Output
                                  |
                          Coupling matrix
                          (Fifths/Tonnetz/etc)
```

## Clases Core

| Clase | Archivo | Funcion |
|-------|---------|---------|
| `Resonator` | core/Resonator.cpp | Karplus-Strong + damping + brightness |
| `ResonatorGraph` | core/ResonatorGraph.cpp | 12 nodos + topologias + propagacion |
| `Exciter` | core/Exciter.cpp | Pluck/Strike impulsos |
| `GraphView` | gui/GraphView.cpp | Visualizacion circular |

## Parametros

- `damping` (0.9 - 0.9999): Duracion
- `brightness` (0 - 1): Filtro LP
- `coupling` (0 - 1): Resonancia simpatica
- `topology` (0-3): Chromatic/Fifths/Tonnetz/Harmonic

## Pendiente

Ver ROADMAP.md para M3-M8:
- Polifonia real (VoiceManager)
- Presets instrumentales
- GUI mejorada
- Excitacion bow

## Referencia

- Origen: `~/Projects/EigenLab/Physics/Physics Sound Lab/generativos/sympathetic-12`
- Modelo Karplus-Strong: sympathetic-mini
