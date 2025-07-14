运行前记得填参数：
-platform vnc:size=1280x800,depth=32,port=5900

删除现有远程地址：
git remote remove origin

添加新的远程地址：
git remote add origin https://github.com/VidocQAQ/yihui.git

验证：
git remote -v

添加修改的文件：
git add .

这次提交的信息写在这：
git commit -m "XXX"

拉取远程仓库进行同步：
git pull origin main --allow-unrelated-histories

push到远程仓库：
git push origin main
