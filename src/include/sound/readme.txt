The music for the buzzer is a simple array of "note frequencies" and "note
durations" pairs.

It's a 3-step process:

Download a simple MIDI song.

Use this MIDI to CSV tool: https://www.fourmilab.ch/webtools/midicsv/ to
convert the binary midi into plaintext.

Convert it to a C-array with the midi2byte.rb Ruby script.

Optionally listen and tweak it a bit.

