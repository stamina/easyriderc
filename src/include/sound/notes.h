/* Copyright (C) 2021 Bas Brugman
 * http://www.visionnaire.nl
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#ifndef EASY_NOTES_H_INCLUDED
#define EASY_NOTES_H_INCLUDED

#define BUZZ_END 0
#define BUZZ_PAUSE 1

#define a0 36364   // note freq: 27.50 Hz
#define a0x 34317  // note freq: 29.14 Hz
#define b0 32394   // note freq: 30.87 Hz
#define c0 61162   // note freq: 16.35 Hz
#define c0x 57737  // note freq: 17.32 Hz
#define d0 54496   // note freq: 18.35 Hz
#define d0x 51414  // note freq: 19.45 Hz
#define e0 48544   // note freq: 20.60 Hz
#define f0 45809   // note freq: 21.83 Hz
#define f0x 43253  // note freq: 23.12 Hz
#define g0 40816   // note freq: 24.50 Hz
#define g0x 38521  // note freq: 25.96 Hz
#define a1 18182   // note freq: 55.00 Hz
#define a1x 17161  // note freq: 58.27 Hz
#define b1 16197   // note freq: 61.74 Hz
#define c1 30581   // note freq: 32.70 Hz
#define c1x 28860  // note freq: 34.65 Hz
#define d1 27241   // note freq: 36.71 Hz
#define d1x 25714  // note freq: 38.89 Hz
#define e1 24272   // note freq: 41.20 Hz
#define f1 22910   // note freq: 43.65 Hz
#define f1x 21622  // note freq: 46.25 Hz
#define g1 20408   // note freq: 49.00 Hz
#define g1x 19264  // note freq: 51.91 Hz
#define a2 9091    // note freq: 110.00 Hz
#define a2x 8581   // note freq: 116.54 Hz
#define b2 8099    // note freq: 123.47 Hz
#define c2 15288   // note freq: 65.41 Hz
#define c2x 14430  // note freq: 69.30 Hz
#define d2 13620   // note freq: 73.42 Hz
#define d2x 12857  // note freq: 77.78 Hz
#define e2 12134   // note freq: 82.41 Hz
#define f2 11453   // note freq: 87.31 Hz
#define f2x 10811  // note freq: 92.50 Hz
#define g2 10204   // note freq: 98.00 Hz
#define g2x 9631   // note freq: 103.83 Hz
#define a3 4545    // note freq: 220.00 Hz
#define a3x 4290   // note freq: 233.08 Hz
#define b3 4050    // note freq: 246.94 Hz
#define c3 7645    // note freq: 130.81 Hz
#define c3x 7216   // note freq: 138.59 Hz
#define d3 6811    // note freq: 146.83 Hz
#define d3x 6428   // note freq: 155.56 Hz
#define e3 6068    // note freq: 164.81 Hz
#define f3 5727    // note freq: 174.61 Hz
#define f3x 5405   // note freq: 185.00 Hz
#define g3 5102    // note freq: 196.00 Hz
#define g3x 4816   // note freq: 207.65 Hz
#define a4 2273    // note freq: 440.00 Hz
#define a4x 2145   // note freq: 466.16 Hz
#define b4 2025    // note freq: 493.88 Hz
#define c4 3822    // note freq: 261.63 Hz
#define c4x 3608   // note freq: 277.18 Hz
#define d4 3405    // note freq: 293.66 Hz
#define d4x 3214   // note freq: 311.13 Hz
#define e4 3034    // note freq: 329.63 Hz
#define f4 2863    // note freq: 349.23 Hz
#define f4x 2703   // note freq: 369.99 Hz
#define g4 2551    // note freq: 392.00 Hz
#define g4x 2408   // note freq: 415.30 Hz
#define a5 1136    // note freq: 880.00 Hz
#define a5x 1073   // note freq: 932.33 Hz
#define b5 1012    // note freq: 987.77 Hz
#define c5 1911    // note freq: 523.25 Hz
#define c5x 1804   // note freq: 554.37 Hz
#define d5 1703    // note freq: 587.33 Hz
#define d5x 1607   // note freq: 622.25 Hz
#define e5 1517    // note freq: 659.26 Hz
#define f5 1432    // note freq: 698.46 Hz
#define f5x 1351   // note freq: 739.99 Hz
#define g5 1276    // note freq: 783.99 Hz
#define g5x 1204   // note freq: 830.61 Hz
#define a6 568     // note freq: 1760.00 Hz
#define a6x 536    // note freq: 1864.66 Hz
#define b6 506     // note freq: 1975.53 Hz
#define c6 956     // note freq: 1046.50 Hz
#define c6x 902    // note freq: 1108.73 Hz
#define d6 851     // note freq: 1174.66 Hz
#define d6x 804    // note freq: 1244.51 Hz
#define e6 758     // note freq: 1318.51 Hz
#define f6 716     // note freq: 1396.91 Hz
#define f6x 676    // note freq: 1479.98 Hz
#define g6 638     // note freq: 1567.98 Hz
#define g6x 602    // note freq: 1661.22 Hz
#define a7 284     // note freq: 3520.00 Hz
#define a7x 268    // note freq: 3729.31 Hz
#define b7 253     // note freq: 3951.07 Hz
#define c7 478     // note freq: 2093.00 Hz
#define c7x 451    // note freq: 2217.46 Hz
#define d7 426     // note freq: 2349.32 Hz
#define d7x 402    // note freq: 2489.02 Hz
#define e7 379     // note freq: 2637.02 Hz
#define f7 358     // note freq: 2793.83 Hz
#define f7x 338    // note freq: 2959.96 Hz
#define g7 319     // note freq: 3135.96 Hz
#define g7x 301    // note freq: 3322.44 Hz
#define a8 142     // note freq: 7040.00 Hz
#define a8x 134    // note freq: 7458.62 Hz
#define b8 127     // note freq: 7902.13 Hz
#define c8 239     // note freq: 4186.01 Hz
#define c8x 225    // note freq: 4434.92 Hz
#define d8 213     // note freq: 4698.64 Hz
#define d8x 201    // note freq: 4978.03 Hz
#define e8 190     // note freq: 5274.04 Hz
#define f8 179     // note freq: 5587.65 Hz
#define f8x 169    // note freq: 5919.91 Hz
#define g8 159     // note freq: 6271.93 Hz
#define g8x 150    // note freq: 6644.88 Hz

#endif
