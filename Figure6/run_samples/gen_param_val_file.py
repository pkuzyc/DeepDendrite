import os 
import sys
import numpy as np

w_file = sys.argv[-3]
src_filename = sys.argv[-2]
save_file = sys.argv[-1]

w = np.load(w_file)
src_file = open(src_filename, "r")
nweights = w.shape[0]

out_lines = []
for i, line in enumerate(src_file.readlines()):
    if i == 0:
        continue
    val = w[(i - 1) % nweights, -1]
    out_lines.append("%s %f\n"%(line[:-1], val))

with open(save_file, "w") as f:
    f.writelines("%d\n"%len(out_lines))
    f.writelines(out_lines)
