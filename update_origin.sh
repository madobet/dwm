#!/bin/sh
git remote add dwm git://git.suckless.org/dwm
git pull dwm master &&
git checkout master &&
git rebase --rebase-merges dwm/master
