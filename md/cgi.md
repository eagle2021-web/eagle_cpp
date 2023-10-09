1. 更新apt
   执行命令：sudo apt-get update

2. 安装Apache2
   执行命令：sudo apt install apache2

3. 修改Apache2配置
   执行命令：sudo vim /etc/apache2/sites-enabled/000-default.conf，将标注行取消注释，并更新保存
```shell
cat <<EOF >/etc/apache2/sites-enabled/000-default.conf
<VirtualHost *:80>
        # The ServerName directive sets the request scheme, hostname and port that
        # the server uses to identify itself. This is used when creating
        # redirection URLs. In the context of virtual hosts, the ServerName
        # specifies what hostname must appear in the request's Host: header to
        # match this virtual host. For the default virtual host (this file) this
        # value is not decisive as it is used as a last resort host regardless.
        # However, you must set it for any further virtual host explicitly.
        #ServerName www.example.com

        ServerAdmin webmaster@localhost
        DocumentRoot /var/www/html

        # Available loglevels: trace8, ..., trace1, debug, info, notice, warn,
        # error, crit, alert, emerg.
        # It is also possible to configure the loglevel for particular
        # modules, e.g.
        #LogLevel info ssl:warn

        ErrorLog ${APACHE_LOG_DIR}/error.log
        CustomLog ${APACHE_LOG_DIR}/access.log combined

        # For most configuration files from conf-available/, which are
        # enabled or disabled at a global level, it is possible to
        # include a line for only one particular virtual host. For example the
        # following line enables the CGI configuration for this host only
        # after it has been globally disabled with "a2disconf".
        Include conf-available/serve-cgi-bin.conf
</VirtualHost>

# vim: syntax=apache ts=4 sw=4 sts=4 sr noet
EOF
```


执行命令：sudo vim /etc/apache2/conf-available/serve-cgi-bin.conf
    将蓝色标注处的代码修改成下述这样，即将 /usr/lib/cgi-bin/替换成 /var/www/html/cgi-bin/，
    插入红色标注行代码 AddHandler cgi-script cgi，并更新保存

```shell
cat <<EOF > /etc/apache2/conf-available/serve-cgi-bin.conf
<IfModule mod_alias.c>
        <IfModule mod_cgi.c>
                Define ENABLE_USR_LIB_CGI_BIN
        </IfModule>

        <IfModule mod_cgid.c>
                Define ENABLE_USR_LIB_CGI_BIN
        </IfModule>

        <IfDefine ENABLE_USR_LIB_CGI_BIN>
                ScriptAlias /cgi-bin/ /var/www/html/cgi-bin/ #####
                <Directory "/var/www/html/cgi-bin"> #####
                        AllowOverride None
                        Options +ExecCGI -MultiViews +SymLinksIfOwnerMatch
                        Require all granted
                        AddHandler cgi-script cgi #####
                </Directory>
        </IfDefine>
</IfModule>
EOF
```

执行命令：sudo vim /etc/apache2/mods-available/cgid.load
    插入红色标注代码 AddHandler cgi-script .cgi .pl .py .sh，并更新保存
```shell
cat <<EOF > /etc/apache2/mods-available/cgid.load22
LoadModule cgid_module /usr/lib/apache2/modules/mod_cgid.so
AddHandler cgi-script .cgi .pl .py .sh
EOF
```


执行命令： sudo ln -s /etc/apache2/mods-available/cgid.load /etc/apache2/mods-enabled/cgid.load

4. 重启Apache2
```shell
sudo /etc/init.d/apache2 restart
```

5. 新建cgi-bin文件夹
```shell
   sudo mkdir /var/www/html/cgi-bin/
```

```c
#include <stdio.h>
int main()
{
    printf("Content-Type: text/html\r\n");
    printf("Hello World!\n");
    return 0;
}
```

更新保存

(3). 编译源文件，执行命令：
```shell
sudo gcc /var/www/html/cgi-bin/helloworld.c -o /var/www/html/cgi-bin/helloworld.cgi
```

(4). 修改文件权限，执行命令：
```shell
sudo chmod 755 /var/www/html/cgi-bin/helloworld.cgi
```

7. 测试
   浏览器输入网址：http://h131/cgi-bin/helloworld.cgi ，(localhost可替换成服务器的IP) 在页面可以看到如下内容，表明CGI环境配置成功


https://blog.csdn.net/qq_28742063/article/details/127600795