# Qt样式表

## 1.1 Qt样式表介绍

Qt样式表是一个可以自定义部件外观的强大机制，样式表的概念、术语、语法均受到HTML的层叠样式表(Cascading Style Sheets,CSS)的启发。

样式表可通过QApplication::setStyleSheet()函数将其设置到整个应用程序上，也可以使用QWidget::setStyleSheet()将其设置到指定的部件或子部件上，不同级别均可设置样式表，称为样式表的层叠。样式表也可通过设计模式编辑样式表。

## 1.2 Qt样式表语法

（1）样式规则

每个样式规则由选择器和声明组成。

选择器：指定受该规则影响的部件。

声明：指定这个部件上要设置的熟悉

如：QPushButton{color:red} 

QPushButton是选择器；

{color:red}是声明；

color是属性；

red是值。

选择器{属性1：值1；属性2：值2；属性3：值3}

此规则指定了QPushButton和它的子类使用红色作为前景色。**样式表中一般不区分大小写**，如color与coLOR表相同属性，**但类名、对象名及****Qt****属性名区分大小写**。

声明中的多组“属性：值”列表以分号“；”隔开，如

QPushButton{color:red;background-color:white}

（2）选择器类型

| **选择器** | **示例**                  | **说明**                                                 |
| ---------- | ------------------------- | -------------------------------------------------------- |
| 通用选择器 | *                         | 匹配所有部件                                             |
| 类型选择器 | QPushButton               | 匹配所有QPushButton实例及它的所有子类                    |
| 属性选择器 | QPushButton[flat=”false”] | 匹配QPushButton的属性flat为false的实例                   |
| 类选择器   | .QPushButton              | 匹配所有QPushButton实例，但不包括它的子类                |
| ID选择器   | QPushButton#okButton      | 匹配所有QPushButton中的okButton对象的实例                |
| 后代选择器 | QDialog QPushButton       | 匹配所有的QPushButton实例，它们必须是QDialog的子孙部件   |
| 孩子选择器 | QDialog>QPushButton       | 匹配所有的QPushButton实例，它们必须是QDialog的直接子部件 |

 

（3）子部件

对于一些复杂的部件修改样式，可能需要访问它们的子部件，如QComboBox的下拉按钮，QSpinBox的向上、向下箭头灯。

如：

下拉按钮设置图片属性：

QComboBox::drop-down{image:url(dropdown.png)}  

（4）伪状态

①选择器可以包含为状态来限制规则在部件的指定状态上的应用。伪状态在选择器之后，用冒号隔离。如

鼠标悬停在按钮上时被应用：

QPushButton:hover{color:white}

鼠标不悬停在按钮上时被应用(!表否定)：

QPushButton:!hover{color:white}

②伪状态可多个连用，达到逻辑与效果。如

鼠标悬停在一个被选中的QCheckBox部件上时才应用规则：

QCheckBox:hover:checked{color:white}

③伪状态可通过逗号达到逻辑或效果。如

QCheckBox:hover,checked{color:white}

④伪状态和子部件联合使用。如

QComboBox::drop-down:hover{image:url(dropdown_bright.png)}

（5）冲突解决

几个样式规则对相同的属性指定不同的值时会产生冲突。如

QPushButton#okButton{color:gray}

QPushButton{color:red}

冲突原则：特殊的选择器优先。此例中QPushButton#okButton代表的是单一对象，而不是一个类的所有实例，所以okButton的文本颜色会是灰色的。同样的有伪状态的比没有伪状态的优先。

①层叠

样式表设置在QApplication上、父部件上或子部件上。部件有效的样式表示通过部件的祖先样式表和QApplication上的样式表合并得到的，当发生冲突时，部件自己的样式表优先于任何继承的样式表。

②继承

​     当使用Qt样式时，部件并不会自动从父部件继承字体和样色样式设置。如一个QPushButton包含在一个QGroupBox中，这里对QGroupBox设置样式表：

​    qApp->setStyleSheet(“QGroupBox{color:red;}”);

​     但没有对QPushButton设置样式表，这时QPushButton会使用系统颜色，而不会继承QGroupBox的颜色，如果想要QGroupBox的颜色设置到其子部件上，可以这样：

 qApp->setStyleSheet(“QGroupBox,QGroupBox*{color:red;}”)



`UI界面设计样式表`

![image](D:\QtProject\Qt_ChattingSoftware\qtqq_images\styleSheetui.png)



`通过简单代码设计后的运行效果`

![image](D:\QtProject\Qt_ChattingSoftware\qtqq_images\styleSheet.png)

