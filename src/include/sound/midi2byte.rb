#!/usr/bin/env ruby

# array of notes, c4 is the middle C with an array index of 60
notes = [ 
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "c0",
  "c0x",
  "d0",
  "d0x",
  "e0",
  "f0",
  "f0x",
  "g0",
  "g0x",
  "a0",
  "a0x",
  "b0",
  "c1",
  "c1x",
  "d1",
  "d1x",
  "e1",
  "f1",
  "f1x",
  "g1",
  "g1x",
  "a1",
  "a1x",
  "b1",
  "c2",
  "c2x",
  "d2",
  "d2x",
  "e2",
  "f2",
  "f2x",
  "g2",
  "g2x",
  "a2",
  "a2x",
  "b2",
  "c3",
  "c3x",
  "d3",
  "d3x",
  "e3",
  "f3",
  "f3x",
  "g3",
  "g3x",
  "a3",
  "a3x",
  "b3",
  "c4",
  "c4x",
  "d4",
  "d4x",
  "e4",
  "f4",
  "f4x",
  "g4",
  "g4x",
  "a4",
  "a4x",
  "b4",
  "c5",
  "c5x",
  "d5",
  "d5x",
  "e5",
  "f5",
  "f5x",
  "g5",
  "g5x",
  "a5",
  "a5x",
  "b5",
  "c6",
  "c6x",
  "d6",
  "d6x",
  "e6",
  "f6",
  "f6x",
  "g6",
  "g6x",
  "a6",
  "a6x",
  "b6",
  "c7",
  "c7x",
  "d7",
  "d7x",
  "e7",
  "f7",
  "f7x",
  "g7",
  "g7x",
  "a7",
  "a7x",
  "b7",
  "c8",
  "c8x",
  "d8",
  "d8x",
  "e8",
  "f8",
  "f8x",
  "g8",
  "g8x",
  "a8",
  "a8x",
  "b8"
]

cycles_per_quarter_note = 0
quarter_notes_per_minute = 0
current_on_time = 0
current_off_time = 0
last_off_time = 0
current_note = 0
transpose = 1;
idx = 0

puts "const uint16_t g_music[] = {"

IO.foreach(ARGV[0]) do |line|

  if /Header/i =~ line then
    line_parts = line.split(%r{,\s*})
    cycles_per_quarter_note = line_parts[5].to_f
  end

  if /Tempo/i =~ line then
    line_parts = line.split(%r{,\s*})
    quarter_notes_per_minute = (60000000/line_parts[3].to_f)
    print quarter_notes_per_minute.to_i.round.to_s + ", "
  end

  if /Note_on_c/i =~ line then
    line_parts = line.split(%r{,\s*})
    current_on_time = line_parts[1].to_f
    current_note = notes[line_parts[4].to_f+(12*transpose)]
    if (last_off_time != 0 && (current_on_time - last_off_time > 0)) then
      print "#{(4.0/((current_on_time - last_off_time)/cycles_per_quarter_note)).to_i.round}, "
      print "BUZZ_PAUSE" + ", "
    end
  end

  if /Note_off_c/i =~ line then
    line_parts = line.split(%r{,\s*})
    last_off_time, current_off_time = line_parts[1].to_f, line_parts[1].to_f
    print "#{(4.0/((current_off_time - current_on_time)/cycles_per_quarter_note)).to_i.round}, "
    print current_note + ", "
    idx += 1
    print "\n" if (idx % 6 == 0) 
  end
  
end

puts "BUZZ_END};"

