import math
import sys

def calc(inner_depth, outer_depth, sides):
  r1 = 12.5
  r2 = 13.5
  r3 = r1 - inner_depth
  r4 = r2 + outer_depth
  theta = 90 - (90 * (sides - 2) / sides)
  l1 = r3 * math.sin(math.pi * theta / 180)
  l2 = r4 * math.tan(math.pi * theta / 180)
  sides_per_cross_board = 1 if 2*l2 >= 3.5 else 2
  sides_per_board = math.ceil(sides / (2 * sides_per_cross_board))
  length = r4 - r3
  board_length = length * sides_per_board

  print("inner_depth: " + str(inner_depth))
  print("outer_depth: " + str(outer_depth))
  print("sides: " + str(sides))
  print("base: " + str(l2*2) + "/" + str(l1*2))
  print("length: " + str(length))
  print("board length: " + str(board_length))
  print("theta: " + str(theta))


calc(float(sys.argv[1]), float(sys.argv[2]), float(sys.argv[3]))
