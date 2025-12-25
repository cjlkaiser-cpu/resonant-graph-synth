# Resonant Graph Synth - Contexto Claude Code

## Descripcion

Sintetizador fisico-armonico basado en una red de resonadores acoplados.
El sonido emerge de como la energia se propaga por un grafo musical.

## Estado Actual

**Fase:** Pre-Alpha (M0 pendiente)
**Proximo:** Setup proyecto JUCE con CMake

## Stack

- C++17
- JUCE 7.x
- CMake
- Standalone primero, luego VST3/AU

## Arquitectura DSP

```
MIDI -> VoiceManager -> Exciter -> ResonatorGraph -> Output
                                        |
                              N nodos (Resonators)
                              conectados por grafo
```

### Clases Core

| Clase | Responsabilidad |
|-------|-----------------|
| `Resonator` | Karplus-Strong extendido, un nodo del grafo |
| `Exciter` | Genera senal de excitacion (pluck/strike/bow) |
| `ResonatorGraph` | Contenedor de nodos + matriz de acoplamiento |
| `GraphTopology` | Define conexiones: Chromatic, Fifths, Tonnetz |
| `Voice` | Una nota activa |
| `VoiceManager` | Polifonia, voice stealing |

## Convenios

- **Codigo:** Ingles
- **UI/Docs:** Espanol
- **Archivos:** PascalCase para clases, kebab-case para recursos
- **Namespaces:** `rgs::` (resonant graph synth)

## Estructura

```
src/
├── core/       # DSP puro, sin dependencias JUCE (excepto juce_audio_basics)
├── gui/        # Componentes JUCE
└── presets/    # Sistema de presets
```

## Build

```bash
# Configurar
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Compilar
cmake --build build

# Ejecutar standalone
./build/ResonantGraphSynth_artefacts/Standalone/ResonantGraphSynth
```

## Tests

```bash
cmake --build build --target tests
./build/tests/tests
```

## Referencia

- Proyecto hermano: `/Users/carlos/Projects/EigenLab/Physics/Physics Sound Lab/generativos/sympathetic-12`
- Karplus-Strong: sympathetic-mini tiene implementacion limpia
- Modelo fisico de ondas: sympathetic-strings
