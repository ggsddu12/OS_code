## git工作流

```sh
git checkout -b "io"  #创建工作分支
# do some changes"
git add.            
git commit -m "some changes"
git push origin io      #push 工作分支
git checkout master     #切换到主分支 
git pull origin master  #更新主分支
git checkout io         #切换到本地工作分支
git rebase master       #尝试将本地分支与主分支合并

git push -f origin io   #将合并的分支推到远端仓库，建立pull request
#远端仓库审查代码，合格后squash and merge，将分支上的commit多个合并为一个，并将分支与主分支合并
git checkout master #本地切换到主分支
git branch -D io    #删除工作分支
git pull origin master #重新pull远程仓库


```