# Roadmap

## Completado

### M0: Scaffolding
- [x] Proyecto JUCE con CMake
- [x] Build standalone + VST3 + AU
- [x] Estructura de directorios
- [x] Icono personalizado

### M1: Single Resonator
- [x] Clase `Resonator` (Karplus-Strong)
- [x] Parametros: frequency, damping, brightness
- [x] Excitacion por MIDI

### M2: Graph Engine
- [x] `ResonatorGraph` con 12 nodos
- [x] Matriz de acoplamiento
- [x] Topologias: Chromatic, Fifths, Tonnetz, Harmonic
- [x] Visualizacion de energia (GraphView)
- [x] Teclado MIDI integrado

---

## Pendiente

### M3: Voice Manager
- [ ] Polifonia real (8-16 voces)
- [ ] Voice stealing
- [ ] Note-off con decay natural

### M4: GUI Mejorada
- [ ] Edicion visual de conexiones
- [ ] Mutear nodos individuales
- [ ] Espectrograma

### M5: Presets
- [ ] Sistema JSON/ValueTree
- [ ] Piano Resonante
- [ ] Sitar Extendido
- [ ] Campana Coral
- [ ] Pad Estructural

### M6: DSP Avanzado
- [ ] Excitacion bow (sostenida)
- [ ] Damping dependiente de frecuencia
- [ ] Inharmonicidad por nodo
- [ ] Reverb global

### M7: Plugin Polish
- [ ] Parametros automatizables
- [ ] State save/load robusto
- [ ] Validacion en DAWs

### M8: Release
- [ ] GUI profesional
- [ ] Manual de usuario
- [ ] Video demo

---

## Topologias

| Nombre | Conexiones | Caracter |
|--------|------------|----------|
| Chromatic | +-1 semitono | Denso, cluster |
| Fifths | Circulo de quintas | Armonico, abierto |
| Tonnetz | M3/m3/P5 | Neo-Riemanniano |
| Harmonic | Serie armonica | Brillante |
