##  v0.1.x VS v0.2.x


> Why we refactor the pinpoint_c_agent (v0.1.x)

1. Hard to support all php versions.
2. Slow, v0.1.x version hooks all functions.
3. Not support ZTS.

### Data route map

#### v0.1.x

> AOP base on zend_excuted_hook

![How does it work](../images/principle_v0.1.x.png)
#### v0.2.x

> AOP base on classloader (like java-classloaders)

![How does it work](../images/principle_v0.2.x.png)

#### PHP Version Compatibility

Agent version|PHP5.5.x|PHP5.6.x|PHP7.x |php-zts
----|-----|----|-----|---
v0.1.x|✔|✔|✔|✘
v0.2.x|✘|✘|✔|✔

#### PHP Framework

Framework|v0.1.x|v0.2.x
----|-----|----|
Laravel|✔|✔
ThinkPHP|✔|✔
EasySwoole|✘|✘

> Note

1. We are trying to support PHP asynchrnous Framework.Such as Workerman and EasySwoole
2. If your php application not support composer(such as woredpress,phpwind etc),the only choise is v0.1.x.

#### Stability

`v0.2.x > v0.1.x`


#### Maintainability(Dynamically)

```
✔: Not block user's application.
✘: Block user's application: php-fpm/apache should be restart
```

Functional|v0.1.x|v0.2.x
----|-----|----
1.Update plugins(CRUD) |✘|✔ [How to ?](https://github.com/eeliu/php_simple_aop#how-to-reload-all-plugins)
2.Update pinpoint collector|✘|✔
3.Update pinpiont_php.so(pinpoint.so)|✘|✘



#### Performance Loss Under Stress Test

> Test Scene

```
Test Object: PHPMYAdmin
URL: http://dev-mysql-phpmyadmin/db_structure.php?server=1&db=testDB
Plugins Amount: 4
Hooked Times: 30
```

> Test Result

Case|MTT
---|----
phpmyadmin without pinpoint|387.28ms
phpmyadmin with pinpoint-php v0.1|511.46ms
phpmyadmin with pinpoint-php v0.2|398.26ms


```
MTT: Mean RTT Time 
```

> By the way,pinpoint is an APM system, not a performance enhancement system. Loss can't avoiding from now on.


### Contributors's words

As composer is widedly used, v0.2.x is our long-time support version,
v0.1.x could be obsoleted in future.
While, we could fix some fatal error into v0.1.x.