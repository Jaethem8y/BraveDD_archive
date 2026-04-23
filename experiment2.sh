#!/usr/bin/env bash
#SBATCH --account=ciardo-lab
#SBATCH --nodes=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=128G
#SBATCH --time=90:00:00
#SBATCH --mail-type=END,FAIL
files=(
firewire-01.txt.csv
medleyA09.txt.csv
railroad-010-pt.txt.csv
firewire-04.txt.csv
des_02_a.txt.csv
gpufp_16_b.txt.csv
stigelec10-a.txt.csv
des_00_a.txt.csv
co4-08.txt.csv
shield_t_rv_003_b.txt.csv
)
mkdir -p UBDD
mkdir -p REX
for file in "${files[@]}"; do
     new="${file%.csv}"
    ./build/examples/06_state_space_generation "/lustre/hdd/LAS/ciardo-lab/jae/files/${new}" roar > "/lustre/hdd/LAS/ciardo-lab/jae/BraveDD_archive/UBDD/${new}.csv"
    ./build/examples/06_state_space_generation "/lustre/hdd/LAS/ciardo-lab/jae/files/${new}" rex > "/lustre/hdd/LAS/ciardo-lab/jae/BraveDD_archive/REX/${new}.csv"
done
