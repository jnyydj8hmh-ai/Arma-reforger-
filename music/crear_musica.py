#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
crear_musica.py - Genera una cancion y la guarda como un archivo .wav
No necesita instalar nada: solo usa Python.

Uso:
    python3 crear_musica.py
Crea el archivo "cancion.wav" en la misma carpeta.
"""

import math
import struct
import wave

SAMPLE_RATE = 44100          # calidad del sonido (muestras por segundo)
VOLUMEN     = 0.35           # 0.0 (silencio) a 1.0 (maximo)
TEMPO       = 120            # pulsos por minuto

# Notas musicales y su frecuencia en Hz (octava 4)
NOTAS = {
    "C": 261.63, "C#": 277.18, "D": 293.66, "D#": 311.13,
    "E": 329.63, "F": 349.23, "F#": 369.99, "G": 392.00,
    "G#": 415.30, "A": 440.00, "A#": 466.16, "B": 493.88,
    "-": 0.0,  # silencio
}


def frecuencia(nota):
    """Convierte algo como 'A4' o 'C#5' en su frecuencia en Hz."""
    if nota == "-":
        return 0.0
    octava = int(nota[-1])
    base = NOTAS[nota[:-1]]
    return base * (2 ** (octava - 4))


def nota_a_muestras(nota, tiempos):
    """Genera las muestras de una nota. 'tiempos' = duracion en pulsos."""
    freq = frecuencia(nota)
    duracion = tiempos * (60.0 / TEMPO)
    total = int(SAMPLE_RATE * duracion)
    muestras = []
    for i in range(total):
        if freq == 0.0:
            muestras.append(0.0)
            continue
        t = i / SAMPLE_RATE
        # onda principal + un armonico para que suene mas rico
        onda = math.sin(2 * math.pi * freq * t)
        onda += 0.3 * math.sin(2 * math.pi * freq * 2 * t)
        # envolvente: sube y baja para que no suene seco (fade in/out)
        env = min(1.0, i / 400.0, (total - i) / 400.0)
        muestras.append(onda * env)
    return muestras


# La melodia: cada par es (nota, duracion en pulsos)
# Esta es "Oda a la Alegria" de Beethoven. Cambiala por la que quieras.
MELODIA = [
    ("E4", 1), ("E4", 1), ("F4", 1), ("G4", 1),
    ("G4", 1), ("F4", 1), ("E4", 1), ("D4", 1),
    ("C4", 1), ("C4", 1), ("D4", 1), ("E4", 1),
    ("E4", 1.5), ("D4", 0.5), ("D4", 2),
    ("E4", 1), ("E4", 1), ("F4", 1), ("G4", 1),
    ("G4", 1), ("F4", 1), ("E4", 1), ("D4", 1),
    ("C4", 1), ("C4", 1), ("D4", 1), ("E4", 1),
    ("D4", 1.5), ("C4", 0.5), ("C4", 2),
]


def main():
    muestras = []
    for nota, dur in MELODIA:
        muestras.extend(nota_a_muestras(nota, dur))

    # Normalizar y pasar a enteros de 16 bits
    pico = max(1.0, max(abs(m) for m in muestras))
    datos = b"".join(
        struct.pack("<h", int(m / pico * VOLUMEN * 32767))
        for m in muestras
    )

    with wave.open("cancion.wav", "w") as f:
        f.setnchannels(1)      # mono
        f.setsampwidth(2)      # 16 bits
        f.setframerate(SAMPLE_RATE)
        f.writeframes(datos)

    segundos = len(muestras) / SAMPLE_RATE
    print("Listo: cancion.wav ({:.1f} segundos)".format(segundos))


if __name__ == "__main__":
    main()
