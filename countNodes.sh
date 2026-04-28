#!/usr/bin/env bash
#SBATCH --account=ciardo-lab
#SBATCH --nodes=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=128G
#SBATCH --time=90:00:00
#SBATCH --mail-type=END,FAIL
files=(
shield_t_rv_050_a.txt.csv
shield_t_rv_040_a.txt.csv
shield_t_rv_030_a.txt.csv
shield_t_rv_020_a.txt.csv
MAPKbis-53-2-0.txt.csv
shield_t_rv_010_a.txt.csv
des_00_a.txt.csv
shield_t_rv_005_a.txt.csv
shield_t_rv_004_a.txt.csv
des_02_a.txt.csv
shield_t_rv_003_a.txt.csv
des_01_a.txt.csv
Egfr-020-1-0.txt.csv
shield_t_rv_002_a.txt.csv
shield_t_iip_010_a.txt.csv
shield_t_iip_005_a.txt.csv
shield_t_iip_004_a.txt.csv
shield_t_iip_003_a.txt.csv
shield_t_iip_002_a.txt.csv
shield_t_iip_001_a.txt.csv
)
mkdir -p COUNT_REX 
for file in "${files[@]}"; do
     new="${file%.csv}"
    ./build/examples/06_state_space_generation "/lustre/hdd/LAS/ciardo-lab/jae/files/${new}" rex > "/lustre/hdd/LAS/ciardo-lab/jae/another/BraveDD_archive/COUNT_REX/${new}.csv"
done
