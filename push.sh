#!/bin/bash

git add .
read -p "请输入 Comment: " -r COMMENT
git commit -m "$COMMENT"
git push -u gitlink "master"
git push -u origin "master"
git push -u github "master"
