用到自己的环境中需要调整的地方
1.提示找不到工作集记得调整到自己已经下载好的
2.提示找不到Qt5Cored.dll等等dll时记得将项目属性——》Qt Project Setting中Qt Installation改成你自己的（我是msvc2017->QT5.12）
3.遇到error starting process /uic：
将项目属性——》链接器——》常规——》附加库目录加上$(QTDIR)\LIB

注意：本项目的Qt modules在项目属性——》Qt Project Setting——》Qt modules中可以编辑

# compile Environment
1.VS2017
2.Qt5.12.1 MSVC 2017
3.MySQL 8.0.30
