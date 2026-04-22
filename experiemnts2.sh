#!/usr/bin/env bash
#SBATCH --account=ciardo-lab
#SBATCH --nodes=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=128G
#SBATCH --time=90:00:00
#SBATCH --mail-type=END,FAIL

mkdir ubddtotal
for file in "/lustre/hdd/LAS/ciardo-lab/jae/files/*"; do
./build/examples/06_state_space_generation file ubddtotal> "/lustre/hdd/LAS/ciardo-lab/jae/BraveDD_archive/ubddtotal/${file}.csv"
done
