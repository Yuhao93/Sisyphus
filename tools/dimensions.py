import math
import sys

sides = int(sys.argv[1])
outer_radius = float(sys.argv[2])
if len(sys.argv) < 4:
  thickness = [1, 2]
else:
  thickness = [float(sys.argv[3])]

inner_angle = math.radians((sides - 2) * 180 / sides)
half_inner_angle = inner_angle / 2
outer_length = 2 * outer_radius / math.tan(half_inner_angle)

for t in thickness:
  inner_length = outer_length - (2 * t / math.tan(half_inner_angle))
  inner_radius = inner_length / (2 * math.cos(half_inner_angle))
  print ('*** For a ' + str(sides) + '-gon with thickness ' + str(t) + ':')
  print ('  outer_radius = ' + str(outer_radius))
  print ('  inner_radius = ' + str(inner_radius))
  print ('  outer_length = ' + str(outer_length))
  print ('  inner_length = ' + str(inner_length))
  print ('  plank_thickness = ' + str(t))
  print ('  ring_thickness = ' + str(outer_radius - inner_radius))
  print ()
