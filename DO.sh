#!/bin/bash
TIME=$(date +%d-%m-%Y_%H%M%S)
mkdir Backups/"$TIME"
mv duo Backups/"$TIME"
cp main.cpp Backups/"$TIME"
#cp main.c Backups/"$TIME"
make