import numpy as np
import os
import sys
import pdb

core_output = np.load(sys.argv[-2])
labels = np.load(sys.argv[-1])
core_res = np.argmax(core_output, axis = 0)
nimg = core_res.shape[0]
labels = labels[:nimg]
ncorrect = np.where(core_res == labels)[0].shape[0]
print("nimg:%d acc:%f"%(nimg, ncorrect * 1.0 / nimg))
