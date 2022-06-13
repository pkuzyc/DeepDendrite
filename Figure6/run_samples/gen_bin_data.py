import numpy as np
import struct
import os
import sys

data_name = sys.argv[-2]
with np.load(data_name, allow_pickle=True) as f:
    x_train, y_train = f['x_train'] / 255, f['y_train']
    x_test, y_test = f['x_test'] / 255, f['y_test']

dest_dir = sys.argv[-1] 

interval_train, interval_test = 5 / (x_train.flatten() + 0.01), 5 / (x_test.flatten() + 0.01)
pack_train, pack_test = np.stack((interval_train, 9. + interval_train), axis=-1).flatten(), np.stack((interval_test, 9. + interval_test), axis=-1).flatten()
y_train = y_train.astype(np.int32)
y_test = y_test.astype(np.int32)
print (x_train.dtype)

if not os.path.isdir(dest_dir):
    os.mkdir(dest_dir)

with open(os.path.join(dest_dir, "stim_img_train"), "wb") as f:
    f.write(struct.pack("ii", x_train.shape[0], 784*2))
    f.write(pack_train.tobytes())
    f.write(y_train.tobytes())
with open(os.path.join(dest_dir, "stim_img_test"), "wb") as f:
    f.write(struct.pack("ii", x_test.shape[0], 784*2))
    f.write(pack_test.tobytes())
    f.write(y_test.tobytes())
print (x_train.shape, y_train.shape)
print (x_test.shape, y_test.shape)
