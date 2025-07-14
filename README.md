运行前记得填参数：
-platform vnc:size=1280x800,depth=32,port=5900
作为仓库协作者，将本地修改更新到 GitHub 仓库的步骤如下：
1. 克隆仓库（首次操作）
如果你还没有克隆仓库到本地，先执行以下命令：
bash
git clone https://github.com/用户名/仓库名.git
cd 仓库名
2. 创建并切换到新分支（建议）
为避免直接修改主分支，创建一个新分支进行开发：
bash
git checkout -b 我的新分支名
3. 进行代码修改
在本地编辑器中修改文件，完成后保存。
4. 提交修改到本地仓库
bash
# 查看文件状态（可选）
git status

# 添加所有修改的文件
git add .

# 提交修改并添加描述
git commit -m "本次修改的描述，例如：修复了登录功能的bug"
5. 同步远程仓库最新代码
在推送前，确保你的分支与远程主分支保持同步：
bash
# 切换回主分支
git checkout main

# 拉取远程主分支最新代码
git pull origin main

# 切回你的分支，并将主分支更新合并到当前分支
git checkout 我的新分支名
git merge main
6. 推送修改到远程仓库
bash
git push origin 我的新分支名
7. 创建 Pull Request（PR）
打开 GitHub 仓库页面，切换到你的分支。
点击 Compare & pull request 按钮。
填写 PR 标题和描述，说明你的修改内容。
提交 PR，等待仓库管理员审核合并。
8. 清理分支（合并后）
PR 合并后，可以删除本地和远程的临时分支：
bash
# 删除本地分支
git branch -d 我的新分支名

# 删除远程分支
git push origin --delete 我的新分支名


注意事项
避免直接推送主分支：除非仓库有明确要求，否则始终通过 PR 提交修改。
保持提交信息清晰：使用有意义的 commit 信息，方便他人理解修改内容。
解决冲突：如果合并时出现冲突，需手动修改冲突文件后再次提交。
按照以上步骤操作，你就能规范地将修改提交到仓库并等待审核。
