# simulation-of-COVID-19

## 理论上的模拟结果
1. 不封城，能产生永久免疫：感染人数先大幅上升，后大幅下降，如同倒U形。
2. 封城，能产生永久免疫：感染人数先大幅上升，封城后缓慢上升，后大幅下降
3. 不封城，不能产生永久免疫：先大幅上升，后大幅下降，然后再上升，但幅度比上次要小，再次下降······如此循环往复，最终感染人数维持在一个相对稳定的值

经过测试，在恰当调整相应参数的情况下，能够产生相对正确的模拟效果


## 难点
1. 人可以分为健康，被感染，在方舱，居家，免疫四种状态，各种状态之间关系复杂。
2. 在原有代码基础上进行增量更新，容易导致代码混乱。
3. C面向过程的特性使得其在面对稍大一点的项目就显得繁琐，尤其是这个题目十分适合使用支持面向对象的语言来编写。

## 收获
1. 对一些常用小模块的抽象
2. 输出格式化字符串到字符数组里面
3. malloc生成的变量的作用域
4. Git版本管理工具的使用