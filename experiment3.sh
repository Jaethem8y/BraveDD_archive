#!/usr/bin/env bash
#SBATCH --account=ciardo-lab
#SBATCH --nodes=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=128G
#SBATCH --time=90:00:00
#SBATCH --mail-type=END,FAIL
files=(
eratosthenes-500.txt.csv
autocar08_b.txt.csv
firewire-16.txt.csv
firewire-07.txt.csv
BART-005-unf.txt.csv
)
mkdir -p UBDD
mkdir -p REX
for file in "${files[@]}"; do
     new="${file%.csv}"
    ./build/examples/06_state_space_generation "/lustre/hdd/LAS/ciardo-lab/jae/files/${new}" roar > "/lustre/hdd/LAS/ciardo-lab/jae/BraveDD_archive/UBDD/${new}.csv"
    ./build/examples/06_state_space_generation "/lustre/hdd/LAS/ciardo-lab/jae/files/${new}" rex > "/lustre/hdd/LAS/ciardo-lab/jae/BraveDD_archive/REX/${new}.csv"
done
