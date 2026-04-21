#!/usr/bin/env bash
#SBATCH --account=ciardo-lab
#SBATCH --nodes=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=128G
#SBATCH --time=90:00:00
#SBATCH --mail-type=END,FAIL
files=(
rwmutex-r10w50.txt.csv
dekker-15.txt.csv
shield_s_ppp_002_a.txt.csv
shield_s_iip_002_a.txt.csv
medleyA01.txt.csv
shield_t_ppp_002_a.txt.csv
gpufp_12_b.txt.csv
deploy_4_a.txt.csv
autocar10_b.txt.csv
gpufp_16_a.txt.csv
deploy_5_a.txt.csv
afcs_02_a.txt.csv
shield_s_rv_004_a.txt.csv
)
mkdir -p REXC
mkdir -p UBDDC
for file in "${files[@]}"; do
      new="${file%.csv}"
    ./build/examples/06_state_space_generation "/lustre/hdd/LAS/ciardo-lab/jae/files/${new}" rex > "/lustre/hdd/LAS/ciardo-lab/jae/BraveDD_archive/REXC/${new}.csv"
    ./build/examples/06_state_space_generation "/lustre/hdd/LAS/ciardo-lab/jae/files/${new}" roar > "/lustre/hdd/LAS/ciardo-lab/jae/BraveDD_archive/UBDDC/${new}.csv"
done
