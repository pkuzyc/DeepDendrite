import sys
import os

# os.system("module purge")

cmd = "../../install/x86_64/bin/nrnivmodl ../../src/all_mechanisms"
os.system(cmd)

# generate model with fully distributed spines
cmd = "./x86_64/special gen_model.py 1.3"
if not os.path.isdir("./coredat/coredat_1150cell_400_1.0hz_back_noise_1.3spine"):
    os.system(cmd)

# simulate with DeepDendrite
cmd = "../../install/bin/deepdendrite -d ./coredat/coredat_1150cell_400_1.0hz_back_noise_1.3spine -e 1000 -v -86 --cell-permute 3 --cell-nthread 32 --gpu"
os.system(cmd)
os.system("mv out.dat spk_full_spine.dat")

# calculate the spike probability and plot 
# figure for full spine models
cmd = "python3 gen_fig.py spk_full_spine.dat fig_prob_full_spine.png"
os.system(cmd)

#generate few spine model
cmd = "./x86_64/special gen_model.py 0"
if not os.path.isdir("./coredat/coredat_1150cell_400_1.0hz_back_noise_nospine"):
    os.system(cmd)

# simulate few spine models with DeepDendrite
cmd = "../../install/bin/deepdendrite -d ./coredat/coredat_1150cell_400_1.0hz_back_noise_nospine -e 1000 -v -86 --cell-permute 3 --cell-nthread 16 --gpu"
os.system(cmd)
os.system("mv out.dat spk_few_spine.dat")

# calculate the spike probability and plot 
# figure for few spine models
cmd = "python3 gen_fig.py spk_few_spine.dat fig_prob_few_spine.png"
os.system(cmd)
